/*
 * SPDX-FileCopyrightText: 2025 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#pragma once

#include "esp_panel_board_conf_internal.h"
#if ESP_PANEL_BOARD_DEFAULT_USE_SUPPORTED
#include "esp_panel_board_config_supported.h"
#elif ESP_PANEL_BOARD_DEFAULT_USE_CUSTOM
#include "esp_panel_board_config_custom.h"
#endif
#include "esp_panel_board_config_internal.hpp"

/**
 * Define the name of drivers
 */
#ifdef ESP_PANEL_BOARD_LCD_BUS_TYPE
#if ESP_PANEL_BOARD_LCD_BUS_TYPE == ESP_PANEL_BUS_TYPE_SPI
#define ESP_PANEL_BOARD_LCD_BUS_NAME    SPI
#elif ESP_PANEL_BOARD_LCD_BUS_TYPE == ESP_PANEL_BUS_TYPE_QSPI
#define ESP_PANEL_BOARD_LCD_BUS_NAME    QSPI
#elif ESP_PANEL_BOARD_LCD_BUS_TYPE == ESP_PANEL_BUS_TYPE_RGB
#define ESP_PANEL_BOARD_LCD_BUS_NAME    RGB
#elif ESP_PANEL_BOARD_LCD_BUS_TYPE == ESP_PANEL_BUS_TYPE_I2C
#define ESP_PANEL_BOARD_LCD_BUS_NAME    I2C
#elif ESP_PANEL_BOARD_LCD_BUS_TYPE == ESP_PANEL_BUS_TYPE_I80
#define ESP_PANEL_BOARD_LCD_BUS_NAME    I80
#elif ESP_PANEL_BOARD_LCD_BUS_TYPE == ESP_PANEL_BUS_TYPE_MIPI_DSI
#define ESP_PANEL_BOARD_LCD_BUS_NAME    DSI
#else
#error "Unknown LCD bus type selected!"
#endif
#endif
#ifdef ESP_PANEL_BOARD_TOUCH_BUS_TYPE
#if ESP_PANEL_BOARD_TOUCH_BUS_TYPE == ESP_PANEL_BUS_TYPE_SPI
#define ESP_PANEL_BOARD_TOUCH_BUS_NAME    SPI
#elif ESP_PANEL_BOARD_TOUCH_BUS_TYPE == ESP_PANEL_BUS_TYPE_I2C
#define ESP_PANEL_BOARD_TOUCH_BUS_NAME    I2C
#else
#error "Unknown touch bus type selected!"
#endif
#endif
#ifdef ESP_PANEL_BOARD_BACKLIGHT_TYPE
#if ESP_PANEL_BOARD_BACKLIGHT_TYPE == ESP_PANEL_BACKLIGHT_TYPE_SWITCH_GPIO
#define ESP_PANEL_BOARD_BACKLIGHT_NAME    SwitchGPIO
#elif ESP_PANEL_BOARD_BACKLIGHT_TYPE == ESP_PANEL_BACKLIGHT_TYPE_SWITCH_EXPANDER
#define ESP_PANEL_BOARD_BACKLIGHT_NAME    SwitchExpander
#elif ESP_PANEL_BOARD_BACKLIGHT_TYPE == ESP_PANEL_BACKLIGHT_TYPE_PWM_LEDC
#define ESP_PANEL_BOARD_BACKLIGHT_NAME    PWM_LEDC
#elif ESP_PANEL_BOARD_BACKLIGHT_TYPE == ESP_PANEL_BACKLIGHT_TYPE_CUSTOM
#define ESP_PANEL_BOARD_BACKLIGHT_NAME    Custom
#else
#error "Unknown backlight type selected!"
#endif
#endif

/**
 * Define the default configuration of the board
 */
#if ESP_PANEL_BOARD_DEFAULT_USE_CUSTOM

#define ESP_PANEL_BOARD_DEFAULT_CONFIG esp_panel::BOARD_CUSTOM_CONFIG

#elif ESP_PANEL_BOARD_DEFAULT_USE_SUPPORTED

/* Espressif */
#if defined(BOARD_ESP32_C3_LCDKIT)
#define ESP_PANEL_BOARD_DEFAULT_CONFIG esp_panel::BOARD_ESP32_C3_LCDKIT_CONFI   G
#elif defined(BOARD_ESP32_S3_BOX)
#define ESP_PANEL_BOARD_DEFAULT_CONFIG esp_panel::BOARD_ESP32_S3_BOX_CONFIG
#elif defined(BOARD_ESP32_S3_BOX_3)
#define ESP_PANEL_BOARD_DEFAULT_CONFIG esp_panel::BOARD_ESP32_S3_BOX_3_CONFIG
#elif defined(BOARD_ESP32_S3_BOX_3_BETA)
#define ESP_PANEL_BOARD_DEFAULT_CONFIG esp_panel::BOARD_ESP32_S3_BOX_3_BETA_CONFIG
#elif defined(BOARD_ESP32_S3_BOX_LITE)
#define ESP_PANEL_BOARD_DEFAULT_CONFIG esp_panel::BOARD_ESP32_S3_BOX_LITE_CONFIG
#elif defined(BOARD_ESP32_S3_EYE)
#define ESP_PANEL_BOARD_DEFAULT_CONFIG esp_panel::BOARD_ESP32_S3_EYE_CONFIG
#elif defined(BOARD_ESP32_S3_KORVO_2)
#define ESP_PANEL_BOARD_DEFAULT_CONFIG esp_panel::BOARD_ESP32_S3_KORVO_2_CONFIG
#elif defined(BOARD_ESP32_S3_LCD_EV_BOARD)
#define ESP_PANEL_BOARD_DEFAULT_CONFIG esp_panel::BOARD_ESP32_S3_LCD_EV_BOARD_CONFIG
#elif defined(BOARD_ESP32_S3_LCD_EV_BOARD_V1_5)
#define ESP_PANEL_BOARD_DEFAULT_CONFIG esp_panel::BOARD_ESP32_S3_LCD_EV_BOARD_V1_5_CONFIG
#elif defined(BOARD_ESP32_S3_LCD_EV_BOARD_2)
#define ESP_PANEL_BOARD_DEFAULT_CONFIG esp_panel::BOARD_ESP32_S3_LCD_EV_BOARD_2_CONFIG
#elif defined(BOARD_ESP32_S3_LCD_EV_BOARD_2_V1_5)
#define ESP_PANEL_BOARD_DEFAULT_CONFIG esp_panel::BOARD_ESP32_S3_LCD_EV_BOARD_2_V1_5_CONFIG
#elif defined(BOARD_ESP32_S3_USB_OTG)
#define ESP_PANEL_BOARD_DEFAULT_CONFIG esp_panel::BOARD_ESP32_S3_USB_OTG_CONFIG
#elif defined(BOARD_ESP32_P4_FUNCTION_EV_BOARD)
#define ESP_PANEL_BOARD_DEFAULT_CONFIG esp_panel::BOARD_ESP32_P4_FUNCTION_EV_BOARD_CONFIG
/* Elecrow */
#elif defined(BOARD_ELECROW_CROWPANEL_7_0)
#define ESP_PANEL_BOARD_DEFAULT_CONFIG esp_panel::BOARD_ELECROW_CROWPANEL_7_0_CONFIG
/* M5Stack */
#elif defined(BOARD_M5STACK_M5CORE2)
#define ESP_PANEL_BOARD_DEFAULT_CONFIG esp_panel::BOARD_M5STACK_M5CORE2_CONFIG
#elif defined(BOARD_M5STACK_M5DIAL)
#define ESP_PANEL_BOARD_DEFAULT_CONFIG esp_panel::BOARD_M5STACK_M5DIAL_CONFIG
#elif defined(BOARD_M5STACK_M5CORES3)
#define ESP_PANEL_BOARD_DEFAULT_CONFIG esp_panel::BOARD_M5STACK_M5CORES3_CONFIG
/* Jingcai */
#elif defined(BOARD_ESP32_4848S040C_I_Y_3)
#define ESP_PANEL_BOARD_DEFAULT_CONFIG esp_panel::BOARD_ESP32_4848S040C_I_Y_3_CONFIG
/* Waveshare */
#elif defined(BOARD_WAVESHARE_ESP32_S3_TOUCH_LCD_1_85)
#define ESP_PANEL_BOARD_DEFAULT_CONFIG esp_panel::BOARD_WAVESHARE_ESP32_S3_TOUCH_LCD_1_85_CONFIG
#elif defined(BOARD_WAVESHARE_ESP32_S3_TOUCH_LCD_2_1)
#define ESP_PANEL_BOARD_DEFAULT_CONFIG esp_panel::BOARD_WAVESHARE_ESP32_S3_TOUCH_LCD_2_1_CONFIG
#elif defined(BOARD_WAVESHARE_ESP32_S3_TOUCH_LCD_4_3)
#define ESP_PANEL_BOARD_DEFAULT_CONFIG esp_panel::BOARD_WAVESHARE_ESP32_S3_TOUCH_LCD_4_3_CONFIG
#elif defined(BOARD_WAVESHARE_ESP32_S3_TOUCH_LCD_4_3_B)
#define ESP_PANEL_BOARD_DEFAULT_CONFIG esp_panel::BOARD_WAVESHARE_ESP32_S3_TOUCH_LCD_4_3_B_CONFIG
#elif defined(BOARD_WAVESHARE_ESP32_S3_TOUCH_LCD_5)
#define ESP_PANEL_BOARD_DEFAULT_CONFIG esp_panel::BOARD_WAVESHARE_ESP32_S3_TOUCH_LCD_5_CONFIG
#elif defined(BOARD_WAVESHARE_ESP32_S3_TOUCH_LCD_5_B)
#define ESP_PANEL_BOARD_DEFAULT_CONFIG esp_panel::BOARD_WAVESHARE_ESP32_S3_TOUCH_LCD_5_B_CONFIG
#elif defined(BOARD_WAVESHARE_ESP32_S3_TOUCH_LCD_7)
#define ESP_PANEL_BOARD_DEFAULT_CONFIG esp_panel::BOARD_WAVESHARE_ESP32_S3_TOUCH_LCD_7_CONFIG
#elif defined(BOARD_WAVESHARE_ESP32_P4_NANO)
#define ESP_PANEL_BOARD_DEFAULT_CONFIG esp_panel::BOARD_WAVESHARE_ESP32_P4_NANO_CONFIG
#else
#error "Unknown board selected! Please check file `esp_panel_board_supported.h` and make sure only one board is enabled."
#endif

#endif // ESP_PANEL_BOARD_DEFAULT_USE_SUPPORTED