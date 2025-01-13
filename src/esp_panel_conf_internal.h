/*
 * SPDX-FileCopyrightText: 2023-2024 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#pragma once

/* Include type definitions before including configuration header file */
#include "esp_panel_types.h"
#include "esp_panel_versions.h"

// *INDENT-OFF*

/* Handle special Kconfig options */
#ifndef ESP_PANEL_CONF_KCONFIG_IGNORE
    #include "sdkconfig.h"
    #ifdef CONFIG_ESP_PANEL_CONF_FILE_SKIP
        #define ESP_PANEL_CONF_FILE_SKIP
    #endif
#endif

#ifndef ESP_PANEL_CONF_FILE_SKIP
/* If "esp_panel_drivers_conf.h" is available from here, try to use it later */
    #ifdef __has_include
        #if __has_include("esp_panel_drivers_conf.h")
            #ifndef ESP_PANEL_CONF_INCLUDE_SIMPLE
                #define ESP_PANEL_CONF_INCLUDE_SIMPLE
            #endif
        #elif __has_include("../../esp_panel_drivers_conf.h")
            #ifndef ESP_PANEL_CONF_INCLUDE_OUTSIDE
                #define ESP_PANEL_CONF_INCLUDE_OUTSIDE
            #endif
        #else
            #define ESP_PANEL_CONF_INCLUDE_INSIDE
        #endif
    #else
        #define ESP_PANEL_CONF_INCLUDE_OUTSIDE
    #endif
#endif

/* If "esp_panel_drivers_conf.h" is not skipped, include it */
#ifndef ESP_PANEL_CONF_FILE_SKIP
    #ifdef ESP_PANEL_CONF_PATH                          /* If there is a path defined for "esp_panel_drivers_conf.h" use it */
        #define __TO_STR_AUX(x) #x
        #define __TO_STR(x) __TO_STR_AUX(x)
        #include __TO_STR(ESP_PANEL_CONF_PATH)
        #undef __TO_STR_AUX
        #undef __TO_STR
    #elif defined(ESP_PANEL_CONF_INCLUDE_SIMPLE)        /* Or simply include if "esp_panel_drivers_conf.h" is available */
        #include "esp_panel_drivers_conf.h"
    #elif defined(ESP_PANEL_CONF_INCLUDE_OUTSIDE)       /* Or include if "../../esp_panel_drivers_conf.h" is available */
        #include "../../esp_panel_drivers_conf.h"
    #elif defined(ESP_PANEL_CONF_INCLUDE_INSIDE)        /* Or include the default configuration */
        #include "../esp_panel_drivers_conf.h"
    #endif
#endif

#ifndef ESP_PANEL_CONF_INCLUDE_INSIDE
    /**
     * There are two purposes to include the this file:
     *  1. Convert configuration items starting with `CONFIG_` to the required configuration items.
     *  2. Define default values for configuration items that are not defined to keep compatibility.
     */
    #include "esp_panel_conf_kconfig.h"
#endif

/**
 * Check if the current configuration file version is compatible with the library version
 */
/* File `esp_panel_drivers_conf.h` */
#ifndef ESP_PANEL_CONF_FILE_SKIP
    // If the version is not defined, set it to `0.1.0`
    #if !defined(ESP_PANEL_DRIVERS_CONF_FILE_VERSION_MAJOR) && \
        !defined(ESP_PANEL_DRIVERS_CONF_FILE_VERSION_MINOR) && \
        !defined(ESP_PANEL_DRIVERS_CONF_FILE_VERSION_PATCH)
        #define ESP_PANEL_DRIVERS_CONF_FILE_VERSION_MAJOR 0
        #define ESP_PANEL_DRIVERS_CONF_FILE_VERSION_MINOR 1
        #define ESP_PANEL_DRIVERS_CONF_FILE_VERSION_PATCH 0
    #endif
    // Check if the current configuration file version is compatible with the library version
    #if ESP_PANEL_DRIVERS_CONF_FILE_VERSION_MAJOR != ESP_PANEL_DRIVERS_CONF_VERSION_MAJOR
        #error "The file `esp_panel_drivers_conf.h` version is not compatible. Please update it with the file from the library"
    #elif ESP_PANEL_DRIVERS_CONF_FILE_VERSION_MINOR < ESP_PANEL_DRIVERS_CONF_VERSION_MINOR
        #warning "The file `esp_panel_drivers_conf.h` version is outdated. Some new configurations are missing"
    #elif ESP_PANEL_DRIVERS_CONF_FILE_VERSION_PATCH > ESP_PANEL_VERSION_PATCH
        #warning "The file `esp_panel_drivers_conf.h` version is newer than the library. Some new configurations are not supported"
    #endif /* ESP_PANEL_CONF_INCLUDE_INSIDE */
#endif /* ESP_PANEL_CONF_FILE_SKIP */

// *INDENT-ON*
