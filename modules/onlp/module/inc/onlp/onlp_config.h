/**************************************************************************//**
 *
 * @file
 * @brief onlp Configuration Header
 *
 * @addtogroup onlp-config
 * @{
 *
 *****************************************************************************/
#ifndef __ONLP_CONFIG_H__
#define __ONLP_CONFIG_H__

#ifdef GLOBAL_INCLUDE_CUSTOM_CONFIG
#include <global_custom_config.h>
#endif
#ifdef ONLP_INCLUDE_CUSTOM_CONFIG
#include <onlp_custom_config.h>
#endif

/* <auto.start.cdefs(ONLP_CONFIG_HEADER).header> */
#include <AIM/aim.h>
/**
 * ONLP_CONFIG_INCLUDE_LOGGING
 *
 * Include or exclude logging. */


#ifndef ONLP_CONFIG_INCLUDE_LOGGING
#define ONLP_CONFIG_INCLUDE_LOGGING 1
#endif

/**
 * ONLP_CONFIG_LOG_OPTIONS_DEFAULT
 *
 * Default enabled log options. */


#ifndef ONLP_CONFIG_LOG_OPTIONS_DEFAULT
#define ONLP_CONFIG_LOG_OPTIONS_DEFAULT AIM_LOG_OPTIONS_DEFAULT
#endif

/**
 * ONLP_CONFIG_LOG_BITS_DEFAULT
 *
 * Default enabled log bits. */


#ifndef ONLP_CONFIG_LOG_BITS_DEFAULT
#define ONLP_CONFIG_LOG_BITS_DEFAULT AIM_LOG_BITS_DEFAULT
#endif

/**
 * ONLP_CONFIG_LOG_CUSTOM_BITS_DEFAULT
 *
 * Default enabled custom log bits. */


#ifndef ONLP_CONFIG_LOG_CUSTOM_BITS_DEFAULT
#define ONLP_CONFIG_LOG_CUSTOM_BITS_DEFAULT 0
#endif

/**
 * ONLP_CONFIG_PORTING_STDLIB
 *
 * Default all porting macros to use the C standard libraries. */


#ifndef ONLP_CONFIG_PORTING_STDLIB
#define ONLP_CONFIG_PORTING_STDLIB 1
#endif

/**
 * ONLP_CONFIG_PORTING_INCLUDE_STDLIB_HEADERS
 *
 * Include standard library headers for stdlib porting macros. */


#ifndef ONLP_CONFIG_PORTING_INCLUDE_STDLIB_HEADERS
#define ONLP_CONFIG_PORTING_INCLUDE_STDLIB_HEADERS ONLP_CONFIG_PORTING_STDLIB
#endif

/**
 * ONLP_CONFIG_INCLUDE_UCLI
 *
 * Include generic uCli support. */


#ifndef ONLP_CONFIG_INCLUDE_UCLI
#define ONLP_CONFIG_INCLUDE_UCLI 0
#endif

/**
 * ONLP_CONFIG_INCLUDE_PLATFORM_ERROR_CHECK
 *
 * Include platform error checking at initialization. */


#ifndef ONLP_CONFIG_INCLUDE_PLATFORM_ERROR_CHECK
#define ONLP_CONFIG_INCLUDE_PLATFORM_ERROR_CHECK 1
#endif

/**
 * ONLP_CONFIG_INCLUDE_SHLOCK_GLOBAL_INIT
 *
 * Include global shlock initialization at module init time. */


#ifndef ONLP_CONFIG_INCLUDE_SHLOCK_GLOBAL_INIT
#define ONLP_CONFIG_INCLUDE_SHLOCK_GLOBAL_INIT 0
#endif

/**
 * ONLP_CONFIG_INCLUDE_PLATFORM_STATIC
 *
 * Specify the platform name directly as a compile-time option. */


#ifndef ONLP_CONFIG_INCLUDE_PLATFORM_STATIC
#define ONLP_CONFIG_INCLUDE_PLATFORM_STATIC 0
#endif

/**
 * ONLP_CONFIG_PLATFORM_STATIC
 *
 * The name of the static platform if configured. */


#ifndef ONLP_CONFIG_PLATFORM_STATIC
#define ONLP_CONFIG_PLATFORM_STATIC "unknown"
#endif

/**
 * ONLP_CONFIG_PLATFORM_FILENAME
 *
 * The local filename containing the current platform identifier. */


#ifndef ONLP_CONFIG_PLATFORM_FILENAME
#define ONLP_CONFIG_PLATFORM_FILENAME "/etc/onl_platform"
#endif



/**
 * All compile time options can be queried or displayed
 */

/** Configuration settings structure. */
typedef struct onlp_config_settings_s {
    /** name */
    const char* name;
    /** value */
    const char* value;
} onlp_config_settings_t;

/** Configuration settings table. */
/** onlp_config_settings table. */
extern onlp_config_settings_t onlp_config_settings[];

/**
 * @brief Lookup a configuration setting.
 * @param setting The name of the configuration option to lookup.
 */
const char* onlp_config_lookup(const char* setting);

/**
 * @brief Show the compile-time configuration.
 * @param pvs The output stream.
 */
int onlp_config_show(struct aim_pvs_s* pvs);

/* <auto.end.cdefs(ONLP_CONFIG_HEADER).header> */

#include "onlp_porting.h"

#endif /* __ONLP_CONFIG_H__ */
/* @} */
