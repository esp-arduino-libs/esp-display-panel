/*
 * SPDX-FileCopyrightText: 2024 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#pragma once

#include "soc/soc_caps.h"
#include "sdkconfig.h"

/**
 * @brief Macros for backlight
 *
 */
#define ESP_PANEL_BACKLIGHT_TYPE_SWITCH_GPIO    (0)
#define ESP_PANEL_BACKLIGHT_TYPE_PWM_LEDC       (1)
#define ESP_PANEL_BACKLIGHT_TYPE_CUSTOM         (2)

/**
 * @brief Macros for bus type
 *
 */
#define ESP_PANEL_BUS_TYPE_SPI                  (0)
#define ESP_PANEL_BUS_TYPE_QSPI                 (1)
#define ESP_PANEL_BUS_TYPE_RGB                  (2)
#define ESP_PANEL_BUS_TYPE_I2C                  (3)
#define ESP_PANEL_BUS_TYPE_I80                  (4)
#define ESP_PANEL_BUS_TYPE_MIPI_DSI             (5)

/**
 * @brief  Macros for LCD color format bits
 *
 */
#define ESP_PANEL_LCD_COLOR_BITS_RGB565         (16)
#define ESP_PANEL_LCD_COLOR_BITS_RGB666         (18)
#define ESP_PANEL_LCD_COLOR_BITS_RGB888         (24)

/**
 * @brief This macro is used to generate the I2C panel IO configuration according to the touch panel name.
 *
 * @param[in] name Touch panel name
 *
 * Taking GT911 as an example, the following is the actual code after macro expansion:
 *
 *      ESP_PANEL_TOUCH_I2C_PANEL_IO_CONFIG(GT911) => ESP_LCD_TOUCH_IO_I2C_GT911_CONFIG()
 */
#define _ESP_PANEL_TOUCH_I2C_PANEL_IO_CONFIG(name) ESP_LCD_TOUCH_IO_I2C_ ## name ## _CONFIG()
#define ESP_PANEL_TOUCH_I2C_PANEL_IO_CONFIG(name)  _ESP_PANEL_TOUCH_I2C_PANEL_IO_CONFIG(name)

/**
 * @brief This macro is used to generate the I2C panel IO configuration according to the touch panel name and address.
 *
 * @param[in] name Touch panel name
 * @param[in] addr I2C address of the touch panel
 *
 * Taking GT911 as an example, the following is the actual code after macro expansion:
 *
 *      ESP_PANEL_TOUCH_I2C_PANEL_IO_CONFIG_WITH_ADDR(GT911, 0x14) => ESP_LCD_TOUCH_IO_I2C_GT911_CONFIG_WITH_ADDR(0x14)
 */
#define _ESP_PANEL_TOUCH_I2C_PANEL_IO_CONFIG_WITH_ADDR(name, addr) ESP_LCD_TOUCH_IO_I2C_ ## name ## _CONFIG_WITH_ADDR(addr)
#define ESP_PANEL_TOUCH_I2C_PANEL_IO_CONFIG_WITH_ADDR(name, addr)  _ESP_PANEL_TOUCH_I2C_PANEL_IO_CONFIG_WITH_ADDR(name, addr)

/**
 * @brief This macro is used to generate the SPI panel IO configuration according to the touch panel name.
 *
 * @param[in] name  Touch panel name
 * @param[in] cs_io Chip select IO number
 *
 * Taking XPT2046 as an example, the following is the actual code after macro expansion:
 *
 *      ESP_PANEL_TOUCH_SPI_PANEL_IO_CONFIG(XPT2046, 5) => ESP_LCD_TOUCH_IO_SPI_XPT2046_CONFIG(5)
 */
#define _ESP_PANEL_TOUCH_SPI_PANEL_IO_CONFIG(name, cs_io) ESP_LCD_TOUCH_IO_SPI_ ## name ## _CONFIG(cs_io)
#define ESP_PANEL_TOUCH_SPI_PANEL_IO_CONFIG(name, cs_io)  _ESP_PANEL_TOUCH_SPI_PANEL_IO_CONFIG(name, cs_io)

/**
 * @brief Formatter for single LCD vendor command with 8-bit parameter
 *
 * @param[in] delay_ms Delay in milliseconds after this command
 * @param[in] command  LCD command
 * @param ...      Array of 8-bit command parameters, should be like `{data0, data1, data2, ...}`
 *
 */
#define ESP_PANEL_LCD_CMD_WITH_8BIT_PARAM(delay_ms, command, ...) {command, (uint8_t []) __VA_ARGS__, sizeof((uint8_t []) \
                                                                __VA_ARGS__), delay_ms}
/**
 * @brief Formatter for single LCD vendor command with no parameter
 *
 * @param[in] delay_ms Delay in milliseconds after this command
 * @param[in] command  LCD command
 *
 */
#define ESP_PANEL_LCD_CMD_WITH_NONE_PARAM(delay_ms, command) {command, (uint8_t []){ 0x00 }, 0, delay_ms}
