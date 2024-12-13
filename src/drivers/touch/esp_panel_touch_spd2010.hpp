/*
 * SPDX-FileCopyrightText: 2023-2024 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#pragma once

#include "port/esp_lcd_touch_spd2010.h"
#include "esp_panel_touch_base.hpp"

namespace esp_panel::drivers {

/**
 * @brief SPD2010 touch device object class
 *
 * @note  This class is a derived class of `ESP_PanelTouch`, user can use it directly
 */
class TouchSPD2010 : public Touch {
public:
    constexpr static Attributes ATTRIBUTES_DEFAULT = {
        .name = "SPD2010",
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
    TouchSPD2010(Bus *bus, uint16_t width, uint16_t height, int rst_io = -1, int int_io = -1):
        Touch(bus, width, height, rst_io, int_io, ATTRIBUTES_DEFAULT)
    {
    }

    /**
     * @brief Construct a new touch device in a complex way, the `init()` function should be called after this function
     *
     * @param bus    Pointer to panel bus
     * @param config Touch device configuration
     */
    TouchSPD2010(std::shared_ptr<Bus> bus, const Config &config): Touch(bus, config, ATTRIBUTES_DEFAULT) {}

    /**
     * @brief Destroy the LCD device
     *
     */
    ~TouchSPD2010() override;

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
 *             Please use `esp_panel::drivers::TouchSPD2010` instead.
 *
 */
typedef esp_panel::drivers::TouchSPD2010 ESP_PanelTouch_SPD2010 __attribute__((deprecated("Deprecated and will be removed in the next major version. Please use `esp_panel::drivers::TouchSPD2010` instead.")));
