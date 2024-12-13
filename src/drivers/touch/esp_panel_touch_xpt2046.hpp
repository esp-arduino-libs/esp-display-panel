/*
 * SPDX-FileCopyrightText: 2023-2024 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#pragma once

#include "port/esp_lcd_touch_xpt2046.h"
#include "esp_panel_touch_base.hpp"

namespace esp_panel::drivers {

/**
 * @brief XPT2046 touch device object class
 *
 * @note  This class is a derived class of `ESP_PanelTouch`, user can use it directly
 */
class TouchXPT2046 : public Touch {
public:
    constexpr static Attributes ATTRIBUTES_DEFAULT = {
        .name = "XPT2046",
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
    TouchXPT2046(Bus *bus, uint16_t width, uint16_t height, int rst_io = -1, int int_io = -1):
        Touch(bus, width, height, rst_io, int_io, ATTRIBUTES_DEFAULT)
    {
    }

    /**
     * @brief Construct a new touch device in a complex way, the `init()` function should be called after this function
     *
     * @param bus    Pointer to panel bus
     * @param config Touch device configuration
     */
    TouchXPT2046(std::shared_ptr<Bus> bus, const Config &config): Touch(bus, config, ATTRIBUTES_DEFAULT) {}

    /**
     * @brief Destroy the LCD device
     *
     */
    ~TouchXPT2046() override;

    /**
     * @brief Startup the touch device
     *
     * @return true if success, otherwise false
     *
     */
    bool begin(void) override;
};

} // namespace esp_panel::drivers

/**
 * @deprecated This type is deprecated and will be removed in the next major version.
 *             Please use `esp_panel::drivers::TouchXPT2046` instead.
 *
 */
typedef esp_panel::drivers::TouchXPT2046 ESP_PanelTouch_XPT2046 __attribute__((deprecated("Deprecated and will be removed in the next major version. Please use `esp_panel::drivers::TouchXPT2046` instead.")));
