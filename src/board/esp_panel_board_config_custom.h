/*
 * SPDX-FileCopyrightText: 2023-2025 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#pragma once

// *INDENT-OFF*

#include "esp_panel_board_conf_internal.h"

#if !defined(ESP_PANEL_BOARD_FILE_SKIP) && ESP_PANEL_BOARD_DEFAULT_USE_CUSTOM
    #ifdef ESP_PANEL_BOARD_CUSTOM_FILE_PATH
        #define __TO_STR_AUX(x) #x
        #define __TO_STR(x) __TO_STR_AUX(x)
        #include __TO_STR(ESP_PANEL_BOARD_CUSTOM_FILE_PATH)
        #undef __TO_STR_AUX
        #undef __TO_STR
    #elif defined(ESP_PANEL_BOARD_INCLUDE_CUSTOM_SIMPLE)
        #include "esp_panel_board_custom.h"
    #elif defined(ESP_PANEL_BOARD_INCLUDE_OUTSIDE_CUSTOM)
        #include "../../../esp_panel_board_custom.h"
    #elif defined(ESP_PANEL_BOARD_INCLUDE_CUSTOM_INSIDE)
        #include "../../esp_panel_board_custom.h"
    #endif
#endif

#if !defined(ESP_PANEL_BOARD_INCLUDE_CUSTOM_INSIDE)
    /**
     * There are three purposes to include the this file:
     *  1. Convert configuration items starting with `CONFIG_` to the required configuration items.
     *  2. Define default values for configuration items that are not defined to keep compatibility.
     *  3. Check if missing configuration items
     */
    #include "esp_panel_board_kconfig_custom.h"
#endif

/**
 * Check if the current configuration file version is compatible with the library version
 */
#if !defined(ESP_PANEL_BOARD_FILE_SKIP) && ESP_PANEL_BOARD_DEFAULT_USE_CUSTOM
    /* File `esp_panel_board_custom.h` */
    // If the version is not defined, set it to `0.1.0`
    #if !defined(ESP_PANEL_BOARD_CUSTOM_FILE_VERSION_MAJOR) && \
        !defined(ESP_PANEL_BOARD_CUSTOM_FILE_VERSION_MINOR) && \
        !defined(ESP_PANEL_BOARD_CUSTOM_FILE_VERSION_PATCH)
        #define ESP_PANEL_BOARD_CUSTOM_FILE_VERSION_MAJOR 0
        #define ESP_PANEL_BOARD_CUSTOM_FILE_VERSION_MINOR 1
        #define ESP_PANEL_BOARD_CUSTOM_FILE_VERSION_PATCH 0
    #endif

    #if ESP_PANEL_BOARD_CUSTOM_FILE_VERSION_MAJOR != ESP_PANEL_BOARD_CUSTOM_VERSION_MAJOR
        #error "The `esp_panel_board_custom.h` file version is not compatible. Please update it with the file from the library"
    #elif ESP_PANEL_BOARD_CUSTOM_FILE_VERSION_MINOR < ESP_PANEL_BOARD_CUSTOM_VERSION_MINOR
        #warning "The `esp_panel_board_custom.h` file version is outdated. Some new configurations are missing"
    #elif ESP_PANEL_BOARD_CUSTOM_FILE_VERSION_MINOR > ESP_PANEL_BOARD_CUSTOM_VERSION_MINOR
        #warning "The `esp_panel_board_custom.h` file version is newer than the library. Some new configurations are not supported"
    #endif
#endif

// *INDENT-ON*
