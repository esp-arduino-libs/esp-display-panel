/*
 * SPDX-FileCopyrightText: 2023-2024 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#pragma once

#include "port/esp_lcd_touch_gt911.h"
#include "esp_panel_touch_base.hpp"

namespace esp_panel::drivers {

/**
 * @brief GT911 touch device object class
 *
 * @note  This class is a derived class of `ESP_PanelTouch`, user can use it directly
 */
class TouchGT911 : public Touch {
public:
    constexpr static Attributes ATTRIBUTES_DEFAULT = {
        .name = "GT911",
    };

    /**
     * @brief Construct a new touch device in a simple way, the `init()` function should be called after this function
     *
     * @param bus    Pointer to panel bus
     * @param width  The width of the touch screen
     * @param height The height of the touch screen
     * @param rst_io The reset pin of the touch screen, set to `-1` if not used
     * @param int_io The interrupt pin of the touch screen, set to `-1` if not used
     */
    TouchGT911(Bus *bus, uint16_t width, uint16_t height, int rst_io = -1, int int_io = -1):
        Touch(bus, width, height, rst_io, int_io, ATTRIBUTES_DEFAULT)
    {
    }

    /**
     * @brief Construct a new touch device in a complex way, the `init()` function should be called after this function
     *
     * @param bus    Pointer to panel bus
     * @param config Touch device configuration
     */
    TouchGT911(std::shared_ptr<Bus> bus, const Config &config): Touch(bus, config, ATTRIBUTES_DEFAULT) {}

    /**
     * @brief Destroy the LCD device
     *
     */
    ~TouchGT911() override;

    /**
     * @brief Startup the touch device
     *
     * @return true if success, otherwise false
     */
    bool begin(void) override;
};

} // namespace esp_panel::drivers

/**
 * @deprecated This type is deprecated and will be removed in the next major version.
 *             Please use `esp_panel::drivers::TouchGT911` instead.
 *
 */
typedef esp_panel::drivers::TouchGT911 ESP_PanelTouch_GT911 __attribute__((deprecated("Deprecated and will be removed in the next major version. Please use `esp_panel::drivers::TouchGT911` instead.")));
