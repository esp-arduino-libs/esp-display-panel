/*
 * SPDX-FileCopyrightText: 2024-2025 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */
#pragma once

// *INDENT-OFF*

#ifndef ESP_PANEL_BOARD_USE_BACKLIGHT
    #ifdef CONFIG_ESP_PANEL_BOARD_USE_BACKLIGHT
        #define ESP_PANEL_BOARD_USE_BACKLIGHT CONFIG_ESP_PANEL_BOARD_USE_BACKLIGHT
    #else
        #define ESP_PANEL_BOARD_USE_BACKLIGHT 0
    #endif
#endif

#if ESP_PANEL_BOARD_USE_BACKLIGHT
#ifndef ESP_PANEL_BOARD_BACKLIGHT_TYPE
    #ifdef CONFIG_ESP_PANEL_BOARD_BACKLIGHT_TYPE
        #define ESP_PANEL_BOARD_BACKLIGHT_TYPE CONFIG_ESP_PANEL_BOARD_BACKLIGHT_TYPE
    #else
        #error "Missing configuration: ESP_PANEL_BOARD_BACKLIGHT_TYPE"
    #endif
#endif

#if (ESP_PANEL_BOARD_BACKLIGHT_TYPE == ESP_PANEL_BACKLIGHT_TYPE_SWITCH_GPIO) || \
    (ESP_PANEL_BOARD_BACKLIGHT_TYPE == ESP_PANEL_BACKLIGHT_TYPE_SWITCH_EXPANDER) || \
    (ESP_PANEL_BOARD_BACKLIGHT_TYPE == ESP_PANEL_BACKLIGHT_TYPE_PWM_LEDC)
    #ifndef ESP_PANEL_BOARD_BACKLIGHT_IO
        #ifdef CONFIG_ESP_PANEL_BOARD_BACKLIGHT_IO
            #define ESP_PANEL_BOARD_BACKLIGHT_IO CONFIG_ESP_PANEL_BOARD_BACKLIGHT_IO
        #else
            #error "Missing configuration: ESP_PANEL_BOARD_BACKLIGHT_IO"
        #endif
    #endif

    #ifndef ESP_PANEL_BOARD_BACKLIGHT_ON_LEVEL
        #ifdef CONFIG_ESP_PANEL_BOARD_BACKLIGHT_ON_LEVEL
            #define ESP_PANEL_BOARD_BACKLIGHT_ON_LEVEL CONFIG_ESP_PANEL_BOARD_BACKLIGHT_ON_LEVEL
        #else
            #error "Missing configuration: ESP_PANEL_BOARD_BACKLIGHT_ON_LEVEL"
        #endif
    #endif
#endif

#ifndef ESP_PANEL_BOARD_BACKLIGHT_IDLE_OFF
    #ifdef CONFIG_ESP_PANEL_BOARD_BACKLIGHT_IDLE_OFF
        #define ESP_PANEL_BOARD_BACKLIGHT_IDLE_OFF CONFIG_ESP_PANEL_BOARD_BACKLIGHT_IDLE_OFF
    #else
        #define ESP_PANEL_BOARD_BACKLIGHT_IDLE_OFF 0
    #endif
#endif
#endif // ESP_PANEL_BOARD_USE_BACKLIGHT

// *INDENT-ON*