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
 *
 ***********************************************************/
#include <onlp/platformi/sfpi.h>
#include "onlp_platform_defaults_int.h"
#include "onlp_platform_defaults_log.h"

__ONLP_DEFAULTI_IMPLEMENTATION(onlp_sfpi_init(void));
__ONLP_DEFAULTI_IMPLEMENTATION(onlp_sfpi_bitmap_get(onlp_sfp_bitmap_t* bmap));
__ONLP_DEFAULTI_IMPLEMENTATION(onlp_sfpi_is_present(int port));
__ONLP_DEFAULTI_IMPLEMENTATION(onlp_sfpi_eeprom_read(int port, uint8_t data[256]));
__ONLP_DEFAULTI_IMPLEMENTATION(onlp_sfpi_enable_set(int port, int enable));
__ONLP_DEFAULTI_IMPLEMENTATION(onlp_sfpi_enable_get(int port, int* enable));
__ONLP_DEFAULTI_IMPLEMENTATION(onlp_sfpi_post_insert(int port, sff_info_t* sff_info));
__ONLP_DEFAULTI_IMPLEMENTATION(onlp_sfpi_reset(int port));
__ONLP_DEFAULTI_IMPLEMENTATION(onlp_sfpi_status_get(int port, uint32_t* status));
__ONLP_DEFAULTI_IMPLEMENTATION(onlp_sfpi_port_map(int port, int* rport));
__ONLP_DEFAULTI_IMPLEMENTATION(onlp_sfpi_denit(void));
__ONLP_DEFAULTI_VIMPLEMENTATION(onlp_sfpi_debug(int port, aim_pvs_t* pvs));
__ONLP_DEFAULTI_IMPLEMENTATION(onlp_sfpi_ioctl(int port, va_list vargs));
