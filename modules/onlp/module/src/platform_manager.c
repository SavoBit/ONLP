/************************************************************
 * <bsn.cl fy=2014 v=onl>
 *
 *           Copyright 2014 Big Switch Networks, Inc.
 *
 * Licensed under the Eclipse Public License, Version 1.0 (the
 * "License"); you may not use this file except in compliance
 * with the License. You may obtain a copy of the License at
 *
 *        http://www.eclipse.org/legal/epl-v10.html
 *
 * Unless required by applicable law or agreed to in writing,
 * software distributed under the License is distributed on an
 * "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND,
 * either express or implied. See the License for the specific
 * language governing permissions and limitations under the
 * License.
 *
 * </bsn.cl>
 ************************************************************
 *
 * This file implements the Platform Management infrastructure.
 *
 ***********************************************************/
#include <onlp/sys.h>
#include <onlp/psu.h>
#include <onlp/platformi/sysi.h>
#include <onlplib/mmap.h>
#include <timer_wheel/timer_wheel.h>
#include <OS/os_time.h>
#include <AIM/aim.h>
#include "onlp_log.h"
#include "onlp_int.h"
#include <sys/eventfd.h>
#include <errno.h>
#include <pthread.h>

/**
 * Timer wheel callback entry.
 */
typedef struct management_entry_s {
    /** Timer wheel for this entry */
    timer_wheel_entry_t twe;

    /** This is the callback for this timer */
    int (*manage)(void);

    /** This is the callback rate in microseconds */
    uint64_t rate;

    /** The name of this callback (for debugging) */
    const char* name;

    /** The number of times this has been called. */
    int calls;

} management_entry_t;

/**
 * Platform management control structure.
 */
typedef struct management_ctrl_s {
    timer_wheel_t* tw;

    int eventfd;
    pthread_t thread;

} management_ctrl_t;

/* This is the global control state */
static management_ctrl_t control__ = { NULL };


/*
 * Internal notification handler for PSU
 * status changes (all platforms)
 */
static int platform_psus_notify__(void);

/*
 * First Version : Static callback rates.
 * TODO: Allow individual platform callbacks to reregister
 * themselves at whatever rate they want.
 */
static management_entry_t management_entries[] =
    {
        {
            { },
            onlp_sysi_platform_manage_fans,
            /* Every 10 seconds */
            10*1000*1000,
            "Fans",
        },
        {
            { },
            onlp_sysi_platform_manage_leds,
            /* Every 2 seconds */
            2*1000*1000,
            "LEDs",

        },
        {
            { },
            platform_psus_notify__,
            /* Every second */
            1*1000*1000,
            "PSUs",
        }
    };


void
onlp_sys_platform_manage_init(void)
{
    if(control__.tw == NULL) {
        int i;
        uint64_t now = os_time_monotonic();

        control__.tw = timer_wheel_create(4, 512, now);

        for(i = 0; i < AIM_ARRAYSIZE(management_entries); i++) {
            management_entry_t* e = management_entries+i;
            timer_wheel_insert(control__.tw,  &e->twe, now + e->rate);
        }
    }
}


void
onlp_sys_platform_manage_now(void)
{
    management_entry_t* e;

    onlp_sys_platform_manage_init();

    while( (e = (management_entry_t*) timer_wheel_next(control__.tw,
                                                       os_time_monotonic())) ) {
        if(e->manage) {
            e->manage();
        }
        e->calls++;
        timer_wheel_insert(control__.tw, &e->twe, os_time_monotonic() + e->rate);
    }
}

static void*
onlp_sys_platform_manage_thread__(void* vctrl)
{
    volatile management_ctrl_t* ctrl = (volatile management_ctrl_t*)(vctrl);

    /*
     * Wait on the eventfd for the specified timeout period.
     */
    for(;;) {

        fd_set fds;
        uint64_t now;
        struct timeval tv;
        timer_wheel_entry_t* twe;

        FD_ZERO(&fds);
        FD_SET(ctrl->eventfd, &fds);

        /*
         * Ask the timer wheel if there is an expiration in the next 2 seconds.
         */
        now = os_time_monotonic();
        twe = timer_wheel_peek(ctrl->tw, now + 20000000);

        if(twe == NULL) {
            /* Nothing in the next two seconds. */
            tv.tv_sec = 2;
            tv.tv_sec = 0;
        }
        else {
            tv.tv_sec = (twe->deadline - now) / 1000000;
            tv.tv_usec = (twe->deadline - now) % 1000000;
        }

        select(ctrl->eventfd+1, &fds, NULL, NULL, &tv);

        if(FD_ISSET(ctrl->eventfd, &fds)) {
            /* We've been asked to terminate. */
            AIM_LOG_MSG("Terminating.");
            /* Also signifies that we have exit */
            close(ctrl->eventfd);
            ctrl->eventfd = -1;
            return NULL;
        }

        /*
         * We don't bother to check the result of select() here.
         */
        onlp_sys_platform_manage_now();
    }
}

int
onlp_sys_platform_manage_start(void)
{
    onlp_sys_platform_manage_init();

    if(control__.eventfd > 0) {
        /* Already running */
        return 0;
    }

    if( (control__.eventfd = eventfd(0, EFD_SEMAPHORE)) < 0) {
        AIM_LOG_ERROR("eventfd create failed: %{errno}", errno);
        return -1;
    }

    if( (pthread_create(&control__.thread, NULL, onlp_sys_platform_manage_thread__,
                        &control__)) != 0) {
        AIM_LOG_ERROR("pthread create failed.");
        close(control__.eventfd);
        control__.eventfd = -1;
        return -1;
    }

    return 0;
}

int
onlp_sys_platform_manage_stop(void)
{
    if(control__.eventfd > 0) {
        uint64_t zero = 1;
        /* Tell the thread to exit */
        write(control__.eventfd, &zero, sizeof(zero));
        /* Wait for the thread to terminate */
        pthread_join(control__.thread, NULL);
    }
    return 0;
}


static int
platform_psus_notify__(void)
{
    static onlp_oid_t psu_oid_table[ONLP_OID_TABLE_SIZE] = {0};
    static onlp_psu_info_t psu_info_table[ONLP_OID_TABLE_SIZE];
    int i = 0;

    if(psu_oid_table[0] == 0) {
        /* We haven't retreived the system PSU oids yet. */
        onlp_sys_info_t si;
        onlp_oid_t* oidp;

        if(onlp_sys_info_get(&si) < 0) {
            AIM_LOG_ERROR("onlp_sys_info_get() failed.");
            return -1;
        }
        ONLP_OID_TABLE_ITER_TYPE(si.hdr.coids, oidp, PSU) {
            psu_oid_table[i++] = *oidp;
        }
    }

    for(i = 0; i < AIM_ARRAYSIZE(psu_oid_table); i++) {
        onlp_psu_info_t pi;
        int pid = ONLP_OID_ID_GET(psu_oid_table[i]);

        if(psu_oid_table[i] == 0) {
            break;
        }

        if(onlp_psu_info_get(psu_oid_table[i], &pi) < 0) {
            AIM_LOG_ERROR("Failure retreiving status of PSU ID %d",
                          pid);
            continue;
        }

        /*
         * Log any presences or failure transitions.
         */
        if(pi.status != psu_info_table[i].status) {
            uint32_t new = pi.status;
            uint32_t old = psu_info_table[i].status;

            if( !(old & 0x1) && (new & 0x1) ) {
                /* PSU Inserted */
                AIM_LOG_INFO("PSU %d has been inserted.", pid);
            }
            if( (old & 0x1) && !(new & 0x1) ) {
                /* PSU Removed */
                AIM_LOG_INFO("PSU %d has been removed.", pid);
            }
            if( (old & ONLP_PSU_STATUS_FAILED) && !(new & ONLP_PSU_STATUS_FAILED) ) {
                /* PSU recovery (seems unlikely) */
                AIM_LOG_INFO("PSU %d has recovered.", pid);
            }

            if( !(old & ONLP_PSU_STATUS_FAILED) && (new & ONLP_PSU_STATUS_FAILED) ) {
                /* PSU Failure */
                AIM_LOG_INFO("PSU %d has failed.", pid);
            }

            memcpy(psu_info_table+i, &pi, sizeof(pi));
        }
    }
    return 0;
}


