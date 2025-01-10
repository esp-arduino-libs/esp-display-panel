/*
 * SPDX-FileCopyrightText: 2023-2024 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#pragma once

#include "base/esp_lcd_touch_chsc6540.h"
#include "ESP_PanelTouch.h"

/**
 * @brief CHSC6540 touch device object class
 *
 * @note  This class is a derived class of `ESP_PanelTouch`, user can use it directly
 */
class ESP_PanelTouch_CHSC6540 : public ESP_PanelTouch {
public:
    /**
     * @brief Construct a new touch device in a simple way, the `init()` function should be called after this function
     *
     * @param bus    Pointer to panel bus
     * @param width  The width of the touch screen
     * @param height The height of the touch screen
     * @param rst_io The reset pin of the touch screen, set to `-1` if not used
     * @param int_io The interrupt pin of the touch screen, set to `-1` if not used
     */
    ESP_PanelTouch_CHSC6540(ESP_PanelBus *bus, uint16_t width, uint16_t height, int rst_io = -1, int int_io = -1);

    /**
     * @brief Construct a new touch device in a complex way, the `init()` function should be called after this function
     *
     * @param bus    Pointer to panel bus
     * @param config Touch device configuration
     */
    ESP_PanelTouch_CHSC6540(ESP_PanelBus *bus, const esp_lcd_touch_config_t &config);

    /**
     * @brief Destroy the LCD device
     *
     */
    ~ESP_PanelTouch_CHSC6540() override;

    /**
     * @brief Startup the touch device
     *
     * @return true if success, otherwise false
     */
    bool begin(void) override;
};
