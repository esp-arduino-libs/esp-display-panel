/*
 * SPDX-FileCopyrightText: 2023-2025 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#pragma once

// *INDENT-OFF*

#include "esp_panel_conf_internal.h"

#ifndef ESP_PANEL_BOARD_FILE_SKIP
    /* If "esp_panel_board_*.h" are available from here, try to use them later */
    #if __has_include("esp_panel_board_supported.h")
        #ifndef ESP_PANEL_BOARD_INCLUDE_SUPPORTED_SIMPLE
            #define ESP_PANEL_BOARD_INCLUDE_SUPPORTED_SIMPLE
        #endif
    #elif __has_include("../../../esp_panel_board_supported.h")
        #ifndef ESP_PANEL_BOARD_INCLUDE_SUPPORTED_OUTSIDE
            #define ESP_PANEL_BOARD_INCLUDE_SUPPORTED_OUTSIDE
        #endif
    #else
        #ifndef ESP_PANEL_BOARD_INCLUDE_SUPPORTED_INSIDE
            #define ESP_PANEL_BOARD_INCLUDE_SUPPORTED_INSIDE
        #endif
    #endif

    #if __has_include("esp_panel_board_custom.h")
        #ifndef ESP_PANEL_BOARD_INCLUDE_CUSTOM_SIMPLE
            #define ESP_PANEL_BOARD_INCLUDE_CUSTOM_SIMPLE
        #endif
    #elif __has_include("../../../esp_panel_board_custom.h")
        #ifndef ESP_PANEL_BOARD_INCLUDE_OUTSIDE_CUSTOM
            #define ESP_PANEL_BOARD_INCLUDE_OUTSIDE_CUSTOM
        #endif
    #else
        #ifndef ESP_PANEL_BOARD_INCLUDE_CUSTOM_INSIDE
            #define ESP_PANEL_BOARD_INCLUDE_CUSTOM_INSIDE
        #endif
    #endif

    /* If "esp_panel_board_*.h" are not skipped, include them */
    #ifdef ESP_PANEL_BOARD_SUPPORTED_FILE_PATH
        #define __TO_STR_AUX(x) #x
        #define __TO_STR(x) __TO_STR_AUX(x)
        #include __TO_STR(ESP_PANEL_BOARD_SUPPORTED_FILE_PATH)
        #undef __TO_STR_AUX
        #undef __TO_STR
    #elif defined(ESP_PANEL_BOARD_INCLUDE_SUPPORTED_SIMPLE)
        #include "esp_panel_board_supported.h"
    #elif defined(ESP_PANEL_BOARD_INCLUDE_SUPPORTED_OUTSIDE)
        #include "../../../esp_panel_board_supported.h"
    #elif defined(ESP_PANEL_BOARD_INCLUDE_SUPPORTED_INSIDE)
        #include "../../esp_panel_board_supported.h"
    #endif

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

#if ESP_PANEL_BOARD_DEFAULT_USE_CUSTOM
/**
 * Since the board configuration macros are defined in the `esp_panel_board_custom.h` file,
 * but we don't want to export them to the outside except the `ESP_PANEL_BOARD_DEFAULT_USE_CUSTOM` macro,
 * so we need to undefine them here.
 */
#include "esp_panel_board_undef.h"
#endif

/* Default board configuration */
#ifndef ESP_PANEL_BOARD_DEFAULT_USE_SUPPORTED
    #ifdef CONFIG_ESP_PANEL_BOARD_DEFAULT_USE_SUPPORTED
        #define ESP_PANEL_BOARD_DEFAULT_USE_SUPPORTED CONFIG_ESP_PANEL_BOARD_DEFAULT_USE_SUPPORTED
    #else
        #define ESP_PANEL_BOARD_DEFAULT_USE_SUPPORTED 0
    #endif
#endif
#ifndef ESP_PANEL_BOARD_DEFAULT_USE_CUSTOM
    #ifdef CONFIG_ESP_PANEL_BOARD_DEFAULT_USE_CUSTOM
        #define ESP_PANEL_BOARD_DEFAULT_USE_CUSTOM CONFIG_ESP_PANEL_BOARD_DEFAULT_USE_CUSTOM
    #else
        #define ESP_PANEL_BOARD_DEFAULT_USE_CUSTOM 0
    #endif
#endif

/* Check if select both custom and supported board */
#if ESP_PANEL_BOARD_DEFAULT_USE_SUPPORTED && ESP_PANEL_BOARD_DEFAULT_USE_CUSTOM
    #error "Please select either a custom or a supported development board, cannot enable both simultaneously"
#endif

/* Check if using a default board */
#define ESP_PANEL_BOARD_USE_DEFAULT     (ESP_PANEL_BOARD_DEFAULT_USE_SUPPORTED || ESP_PANEL_BOARD_DEFAULT_USE_CUSTOM)

/* For using a supported board, include the supported board header file */
#if ESP_PANEL_BOARD_DEFAULT_USE_SUPPORTED
    #include "esp_panel_board_kconfig_supported.h"

    // Check if multiple boards are enabled
    #if \
        /* Espressif */ \
        defined(BOARD_ESP32_C3_LCDKIT) \
        + defined(BOARD_ESP32_S3_BOX) \
        + defined(BOARD_ESP32_S3_BOX_3) \
        + defined(BOARD_ESP32_S3_BOX_3_BETA) \
        + defined(BOARD_ESP32_S3_BOX_LITE) \
        + defined(BOARD_ESP32_S3_EYE) \
        + defined(BOARD_ESP32_S3_KORVO_2) \
        + defined(BOARD_ESP32_S3_LCD_EV_BOARD) \
        + defined(BOARD_ESP32_S3_LCD_EV_BOARD_V1_5) \
        + defined(BOARD_ESP32_S3_LCD_EV_BOARD_2) \
        + defined(BOARD_ESP32_S3_LCD_EV_BOARD_2_V1_5) \
        + defined(BOARD_ESP32_S3_USB_OTG) \
        + defined(BOARD_ESP32_P4_FUNCTION_EV_BOARD) \
        /* Elecrow */ \
        + defined(BOARD_ELECROW_CROWPANEL_7_0) \
        /* M5Stack */ \
        + defined(BOARD_M5STACK_M5CORE2) \
        + defined(BOARD_M5STACK_M5DIAL) \
        + defined(BOARD_M5STACK_M5CORES3) \
        /* JingCai */ \
        + defined(BOARD_ESP32_4848S040C_I_Y_3) \
        /* Waveshare */ \
        + defined(BOARD_WAVESHARE_ESP32_S3_TOUCH_LCD_1_85) \
        + defined(BOARD_WAVESHARE_ESP32_S3_TOUCH_LCD_2_1) \
        + defined(BOARD_WAVESHARE_ESP32_S3_TOUCH_LCD_4_3) \
        + defined(BOARD_WAVESHARE_ESP32_S3_TOUCH_LCD_4_3_B) \
        + defined(BOARD_WAVESHARE_ESP32_S3_TOUCH_LCD_5) \
        + defined(BOARD_WAVESHARE_ESP32_S3_TOUCH_LCD_5_B) \
        + defined(BOARD_WAVESHARE_ESP32_S3_TOUCH_LCD_7) \
        + defined(BOARD_WAVESHARE_ESP32_P4_NANO) \
        > 1
        #error "Multiple boards enabled! Please check file `esp_panel_board_supported.h` and make sure only one board is enabled."
    #endif
#endif

/**
 * Check if the current configuration file version is compatible with the library version
 */
/* File `esp_panel_board_supported.h` */
// Only check this file versions if using a supported board and not skip the file
#if ESP_PANEL_BOARD_DEFAULT_USE_SUPPORTED && !defined(ESP_PANEL_BOARD_FILE_SKIP)
    // If the version is not defined, set it to `0.1.0`
    #if !defined(ESP_PANEL_BOARD_SUPPORTED_FILE_VERSION_MAJOR) && \
        !defined(ESP_PANEL_BOARD_SUPPORTED_FILE_VERSION_MINOR) && \
        !defined(ESP_PANEL_BOARD_SUPPORTED_FILE_VERSION_PATCH)
        #define ESP_PANEL_BOARD_SUPPORTED_FILE_VERSION_MAJOR 0
        #define ESP_PANEL_BOARD_SUPPORTED_FILE_VERSION_MINOR 1
        #define ESP_PANEL_BOARD_SUPPORTED_FILE_VERSION_PATCH 0
    #endif

    // Check if the current configuration file version is compatible with the library version
    #if ESP_PANEL_BOARD_SUPPORTED_FILE_VERSION_MAJOR != ESP_PANEL_BOARD_SUPPORTED_VERSION_MAJOR
        #error "The `esp_panel_board_supported.h` file version is not compatible. Please update it with the file from the library"
    #elif ESP_PANEL_BOARD_SUPPORTED_FILE_VERSION_MINOR < ESP_PANEL_BOARD_SUPPORTED_VERSION_MINOR
        #warning "The `esp_panel_board_supported.h` file version is outdated. Some new configurations are missing"
    #elif ESP_PANEL_BOARD_SUPPORTED_FILE_VERSION_MINOR > ESP_PANEL_BOARD_SUPPORTED_VERSION_MINOR
        #warning "The `esp_panel_board_supported.h` file version is newer than the library. Some new configurations are not supported"
    #endif
#endif

// *INDENT-ON*
