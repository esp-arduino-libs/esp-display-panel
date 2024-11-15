/*
 * SPDX-FileCopyrightText: 2023-2024 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#pragma once

#include "port/esp_lcd_touch_cst816s.h"
#include "esp_panel_touch_base.hpp"

namespace esp_panel::drivers {

/**
 * @brief CST816S touch device object class
 *
 * @note  This class is a derived class of `ESP_PanelTouch`, user can use it directly
 */
class TouchCST816S : public Touch {
public:
    constexpr static Attributes ATTRIBUTES_DEFAULT = {
        .name = "CST816S",
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
    TouchCST816S(Bus *bus, uint16_t width, uint16_t height, int rst_io = -1, int int_io = -1):
        Touch(bus, width, height, rst_io, int_io, ATTRIBUTES_DEFAULT)
    {
    }

    /**
     * @brief Construct a new touch device in a complex way, the `init()` function should be called after this function
     *
     * @param bus    Pointer to panel bus
     * @param config Touch device configuration
     */
    TouchCST816S(std::shared_ptr<Bus> bus, const Config &config): Touch(bus, config, ATTRIBUTES_DEFAULT) {}

    /**
     * @brief Destroy the LCD device
     *
     */
    ~TouchCST816S() override;

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
 *             Please use `esp_panel::drivers::TouchCST816S` instead.
 *
 */
typedef esp_panel::drivers::TouchCST816S ESP_PanelTouch_CST816S __attribute__((deprecated("Deprecated and will be removed in the next major version. Please use `esp_panel::drivers::TouchCST816S` instead.")));
