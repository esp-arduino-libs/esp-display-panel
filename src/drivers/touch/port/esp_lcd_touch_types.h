/*
 * SPDX-FileCopyrightText: 2025 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#pragma once

/**
 * @brief This macro is used to generate the I2C control panel configuration according to the touch panel name.
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
 * @brief This macro is used to generate the I2C control panel configuration according to the touch panel name and
 *        address.
 *
 * @param[in] name Touch panel name
 * @param[in] addr I2C address of the touch panel
 *
 * Taking GT911 as an example, the following is the actual code after macro expansion:
 *
 *      ESP_PANEL_TOUCH_I2C_PANEL_IO_CONFIG_WITH_ADDR(GT911, 0x14) => ESP_LCD_TOUCH_IO_I2C_GT911_CONFIG_WITH_ADDR(0x14)
 */
#define _ESP_PANEL_TOUCH_I2C_PANEL_IO_CONFIG_WITH_ADDR(name, addr) \
    ESP_LCD_TOUCH_IO_I2C_ ## name ## _CONFIG_WITH_ADDR(addr)
#define ESP_PANEL_TOUCH_I2C_PANEL_IO_CONFIG_WITH_ADDR(name, addr) \
    _ESP_PANEL_TOUCH_I2C_PANEL_IO_CONFIG_WITH_ADDR(name, addr)

/**
 * @brief This macro is used to generate the SPI control panel configuration according to the touch panel name.
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
