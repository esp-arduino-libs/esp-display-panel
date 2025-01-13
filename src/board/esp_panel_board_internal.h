/*
 * SPDX-FileCopyrightText: 2023-2025 Espressif Systems (Shanghai) CO LTD
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
    #ifdef CONFIG_ESP_PANEL_CONF_BOARD_FILE_SKIP
        #define ESP_PANEL_CONF_BOARD_FILE_SKIP
    #endif
#endif

#ifndef ESP_PANEL_CONF_BOARD_FILE_SKIP
    /* If "ESP_Panel_*_Board.h" are available from here, try to use them later */
    #ifdef __has_include
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
    #else
        #ifndef ESP_PANEL_BOARD_INCLUDE_OUTSIDE_CUSTOM
            #define ESP_PANEL_BOARD_INCLUDE_OUTSIDE_CUSTOM
        #endif
        #ifndef ESP_PANEL_BOARD_INCLUDE_SUPPORTED_INSIDE
            #define ESP_PANEL_BOARD_INCLUDE_SUPPORTED_INSIDE
        #endif
    #endif
#endif

#ifndef ESP_PANEL_CONF_FILE_SKIP
    /* If "ESP_Panel_Board_*.h" are not skipped, include them */
    #ifdef ESP_PANEL_BOARD_SUPPORTED_FILE_PATH                  /* If there is a path defined for "esp_panel_board_supported.h", use it */
        #define __TO_STR_AUX(x) #x
        #define __TO_STR(x) __TO_STR_AUX(x)
        #include __TO_STR(ESP_PANEL_BOARD_SUPPORTED_FILE_PATH)
        #undef __TO_STR_AUX
        #undef __TO_STR
    #elif defined(ESP_PANEL_BOARD_INCLUDE_SUPPORTED_SIMPLE)     /* Or simply include if "esp_panel_board_supported.h" is available */
        #include "esp_panel_board_supported.h"
    #elif defined(ESP_PANEL_BOARD_INCLUDE_SUPPORTED_OUTSIDE)    /* Or include if "../../esp_panel_board_supported.h" is available */
        #include "../../../esp_panel_board_supported.h"
    #elif defined(ESP_PANEL_BOARD_INCLUDE_SUPPORTED_INSIDE)     /* Or include default configuration */
        #include "../../esp_panel_board_supported.h"
    #endif
    #ifdef ESP_PANEL_BOARD_CUSTOM_FILE_PATH                     /* If there is a path defined for "esp_panel_board_custom.h" use it */
        #define __TO_STR_AUX(x) #x
        #define __TO_STR(x) __TO_STR_AUX(x)
        #include __TO_STR(ESP_PANEL_BOARD_CUSTOM_FILE_PATH)
        #undef __TO_STR_AUX
        #undef __TO_STR
    #elif defined(ESP_PANEL_BOARD_INCLUDE_CUSTOM_SIMPLE)        /* Or simply include if "esp_panel_board_custom.h" is available */
        #include "esp_panel_board_custom.h"
    #elif defined(ESP_PANEL_BOARD_INCLUDE_OUTSIDE_CUSTOM)       /* Or include if "../../esp_panel_board_custom.h" is available */
        #include "../../../esp_panel_board_custom.h"
    #elif defined(ESP_PANEL_BOARD_INCLUDE_CUSTOM_INSIDE)        /* Or include default configuration */
        #include "../../esp_panel_board_custom.h"
    #endif
#endif

#if !defined(ESP_PANEL_BOARD_INCLUDE_SUPPORTED_INSIDE) && !defined(ESP_PANEL_BOARD_INCLUDE_CUSTOM_INSIDE)
    /**
     * There are two purposes to include the this file:
     *  1. Convert configuration items starting with `CONFIG_` to the required configuration items.
     *  2. Define default values for configuration items that are not defined to keep compatibility.
     */
    #include "esp_panel_board_kconfig.h"
#endif

/* Check if select both custom and supported board */
#if ESP_PANEL_BOARD_DEFAULT_USE_SUPPORTED && ESP_PANEL_BOARD_DEFAULT_USE_CUSTOM
    #error "Please select either a custom or a supported development board, cannot enable both simultaneously"
#endif

/* Check if using a default board */
#define ESP_PANEL_BOARD_USE_DEFAULT     (ESP_PANEL_BOARD_DEFAULT_USE_SUPPORTED || ESP_PANEL_BOARD_DEFAULT_USE_CUSTOM)

/* For using a supported board, include the supported board header file */
#if ESP_PANEL_BOARD_DEFAULT_USE_SUPPORTED
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

    // Include board specific header file
    /* Espressif */
    #if defined(BOARD_ESP32_C3_LCDKIT)
        #include "board/espressif/ESP32_C3_LCDKIT.h"
    #elif defined(BOARD_ESP32_S3_BOX)
        #include "board/espressif/ESP32_S3_BOX.h"
    #elif defined(BOARD_ESP32_S3_BOX_3)
        #include "board/espressif/ESP32_S3_BOX_3.h"
    #elif defined(BOARD_ESP32_S3_BOX_3_BETA)
        #include "board/espressif/ESP32_S3_BOX_3_BETA.h"
    #elif defined(BOARD_ESP32_S3_BOX_LITE)
        #include "board/espressif/ESP32_S3_BOX_LITE.h"
    #elif defined(BOARD_ESP32_S3_EYE)
        #include "board/espressif/ESP32_S3_EYE.h"
    #elif defined(BOARD_ESP32_S3_KORVO_2)
        #include "board/espressif/ESP32_S3_KORVO_2.h"
    #elif defined(BOARD_ESP32_S3_LCD_EV_BOARD)
        #include "board/espressif/ESP32_S3_LCD_EV_BOARD.h"
    #elif defined(BOARD_ESP32_S3_LCD_EV_BOARD_V1_5)
        #include "board/espressif/ESP32_S3_LCD_EV_BOARD_V1_5.h"
    #elif defined(BOARD_ESP32_S3_LCD_EV_BOARD_2)
        #include "board/espressif/ESP32_S3_LCD_EV_BOARD_2.h"
    #elif defined(BOARD_ESP32_S3_LCD_EV_BOARD_2_V1_5)
        #include "board/espressif/ESP32_S3_LCD_EV_BOARD_2_V1_5.h"
    #elif defined(BOARD_ESP32_S3_USB_OTG)
        #include "board/espressif/ESP32_S3_USB_OTG.h"
    #elif defined(BOARD_ESP32_P4_FUNCTION_EV_BOARD)
        #include "board/espressif/ESP32_P4_FUNCTION_EV_BOARD.h"
    /* Elecrow */
    #elif defined(BOARD_ELECROW_CROWPANEL_7_0)
        #include "board/elecrow/CROWPANEL_7_0.h"
    /* M5Stack */
    #elif defined(BOARD_M5STACK_M5CORE2)
        #include "board/m5stack/M5CORE2.h"
    #elif defined(BOARD_M5STACK_M5DIAL)
        #include "board/m5stack/M5DIAL.h"
    #elif defined(BOARD_M5STACK_M5CORES3)
        #include "board/m5stack/M5CORES3.h"
    /* Jingcai */
    #elif defined(BOARD_ESP32_4848S040C_I_Y_3)
        #include "board/jingcai/ESP32_4848S040C_I_Y_3.h"
    /* Waveshare */
    #elif defined(BOARD_WAVESHARE_ESP32_S3_TOUCH_LCD_1_85)
        #include "board/waveshare/ESP32_S3_Touch_LCD_1_85.h"
    #elif defined(BOARD_WAVESHARE_ESP32_S3_TOUCH_LCD_2_1)
        #include "board/waveshare/ESP32_S3_Touch_LCD_2_1.h"
    #elif defined(BOARD_WAVESHARE_ESP32_S3_TOUCH_LCD_4_3)
        #include "board/waveshare/ESP32_S3_Touch_LCD_4_3.h"
    #elif defined(BOARD_WAVESHARE_ESP32_S3_TOUCH_LCD_4_3_B)
        #include "board/waveshare/ESP32_S3_Touch_LCD_4_3_B.h"
    #elif defined(BOARD_WAVESHARE_ESP32_S3_TOUCH_LCD_5)
        #include "board/waveshare/ESP32_S3_Touch_LCD_5.h"
    #elif defined(BOARD_WAVESHARE_ESP32_S3_TOUCH_LCD_5_B)
        #include "board/waveshare/ESP32_S3_Touch_LCD_5_B.h"
    #elif defined(BOARD_WAVESHARE_ESP32_S3_TOUCH_LCD_7)
        #include "board/waveshare/ESP32_S3_Touch_LCD_7.h"
    #elif defined(BOARD_WAVESHARE_ESP32_P4_NANO)
        #include "board/waveshare/ESP32_P4_NANO.h"
    #else
        #error "Unknown board selected! Please check file `esp_panel_board_supported.h` and make sure only one board is enabled."
    #endif
#endif

/**
 * Check if the current configuration file version is compatible with the library version
 */
/* File `esp_panel_board_custom.h` & `esp_panel_board_supported.h` */
#if ESP_PANEL_BOARD_USE_DEFAULT
    /* File `esp_panel_board_supported.h` */
    // Only check this file versions if using a supported board and not skip the file
    #if ESP_PANEL_BOARD_DEFAULT_USE_SUPPORTED && !defined(ESP_PANEL_CONF_BOARD_FILE_SKIP)
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
            #error "The file `esp_panel_board_supported.h` version is not compatible. Please update it with the file from the library"
        #elif ESP_PANEL_BOARD_SUPPORTED_FILE_VERSION_MINOR < ESP_PANEL_BOARD_SUPPORTED_VERSION_MINOR
            #warning "The file `esp_panel_board_supported.h` version is outdated. Some new configurations are missing"
        #elif ESP_PANEL_BOARD_SUPPORTED_FILE_VERSION_MINOR > ESP_PANEL_BOARD_SUPPORTED_VERSION_MINOR
            #warning "The file `esp_panel_board_supported.h` version is newer than the library. Some new configurations are not supported"
        #endif
    #endif

    /* File `esp_panel_board_custom.h` */
    // If the version is not defined, set it to `0.1.0`
    #if !defined(ESP_PANEL_BOARD_CUSTOM_FILE_VERSION_MAJOR) && \
        !defined(ESP_PANEL_BOARD_CUSTOM_FILE_VERSION_MINOR) && \
        !defined(ESP_PANEL_BOARD_CUSTOM_FILE_VERSION_PATCH)
        #define ESP_PANEL_BOARD_CUSTOM_FILE_VERSION_MAJOR 0
        #define ESP_PANEL_BOARD_CUSTOM_FILE_VERSION_MINOR 1
        #define ESP_PANEL_BOARD_CUSTOM_FILE_VERSION_PATCH 0
    #endif
    // Check if the current configuration file version is compatible with the library version
    // Must check the major version
    #if ESP_PANEL_BOARD_CUSTOM_FILE_VERSION_MAJOR != ESP_PANEL_BOARD_CUSTOM_VERSION_MAJOR
        #error "The file `esp_panel_board_custom.h` version is not compatible. Please update it with the file from the library"
    #endif
    // Only check this file versions if using a custom board and not skip the file
    #if ESP_PANEL_BOARD_DEFAULT_USE_CUSTOM && !defined(ESP_PANEL_CONF_BOARD_FILE_SKIP)
        #if ESP_PANEL_BOARD_CUSTOM_FILE_VERSION_MINOR < ESP_PANEL_BOARD_CUSTOM_VERSION_MINOR
            #warning "The file `esp_panel_board_custom.h` version is outdated. Some new configurations are missing"
        #elif ESP_PANEL_BOARD_CUSTOM_FILE_VERSION_MINOR > ESP_PANEL_BOARD_CUSTOM_VERSION_MINOR
            #warning "The file `esp_panel_board_custom.h` version is newer than the library. Some new configurations are not supported"
        #endif
    #endif
#endif /* ESP_PANEL_BOARD_USE_DEFAULT */

// *INDENT-ON*
