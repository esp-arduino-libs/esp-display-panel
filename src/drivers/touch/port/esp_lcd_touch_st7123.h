/*
 * SPDX-FileCopyrightText: 2023-2025 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */
#pragma once

#include "esp_lcd_touch.h"
#if ESP_PANEL_DRIVERS_TOUCH_ENABLE_ST7123

#ifdef __cplusplus
extern "C" {
#endif

#define ESP_LCD_TOUCH_ST7123_VER_MAJOR    (1)
#define ESP_LCD_TOUCH_ST7123_VER_MINOR    (0)
#define ESP_LCD_TOUCH_ST7123_VER_PATCH    (0)

/**
 * @brief Create a new ST7123 touch driver
 *
 * @note  The I2C communication should be initialized before use this function.
 *
 * @param io LCD panel IO handle, it should be created by `esp_lcd_new_panel_io_i2c()`
 * @param config Touch panel configuration
 * @param tp Touch panel handle
 * @return
 *      - ESP_OK: on success
 */
esp_err_t esp_lcd_touch_new_i2c_st7123(const esp_lcd_panel_io_handle_t io, const esp_lcd_touch_config_t *config, esp_lcd_touch_handle_t *tp);

/**
 * @brief I2C address of the ST7123 controller
 *
 */
#define ESP_LCD_TOUCH_IO_I2C_ST7123_ADDRESS     (0x55)

/**
 * @brief Touch IO configuration structure
 *
 */
#define ESP_LCD_TOUCH_IO_I2C_ST7123_CONFIG()                \
    {                                                       \
        .dev_addr = ESP_LCD_TOUCH_IO_I2C_ST7123_ADDRESS,    \
        .control_phase_bytes = 1,                           \
        .lcd_cmd_bits = 16,                                 \
        .flags =                                            \
        {                                                   \
            .disable_control_phase = 1,                     \
        }                                                   \
    }

#ifdef __cplusplus
}
#endif

#endif // ESP_PANEL_DRIVERS_TOUCH_ENABLE_ST7123