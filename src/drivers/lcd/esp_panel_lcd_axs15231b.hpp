/*
 * SPDX-FileCopyrightText: 2023-2024 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#pragma once

#include "esp_panel_lcd_base.hpp"

namespace esp_panel::drivers {

/**
 * @brief AXS15231B LCD device object class
 *
 * @note  This class is a derived class of `ESP_PanelLcd`, user can use it directly
 */
class LCD_AXS15231B: public LCD {
public:
    constexpr static Attributes ATTRIBUTES_DEFAULT = {
        .name = "AXS15231B",
    };

    /**
     * @brief Construct a AXS15231B LCD device
     *
     * @note  This function uses some default values to config the LCD device, please use `config*()` functions to
     *        change them
     * @note  Vendor specific initialization can be different between manufacturers, should consult the LCD supplier
     *        for initialization sequence code, and use `configVendorCommands()` to configure
     *
     * @param bus        Pointer of panel bus
     * @param color_bits Bits per pixel (16/18)
     * @param rst_io     Reset pin, set to -1 if no use
     */
    LCD_AXS15231B(Bus *bus, uint8_t color_bits, int rst_io = -1):
        LCD(bus, color_bits, rst_io, ATTRIBUTES_DEFAULT)
    {
    }

    /**
     * @brief Construct a new AXS15231B LCD device
     *
     * @param bus    Pointer of panel bus
     * @param config LCD configuration
     */
    LCD_AXS15231B(std::shared_ptr<Bus> bus, const Config &config):
        LCD(bus, config, ATTRIBUTES_DEFAULT)
    {
    }

    /**
     * @brief Destroy the LCD device
     *
     */
    ~LCD_AXS15231B() override;

    /**
     * @brief Initialize the LCD device
     *
     * @note  This function typically calls `esp_lcd_new_panel_*()` to create the LCD panel handle
     *
     * @return true if success, otherwise false
     */
    bool init(void) override;
};

} // namespace esp_panel::drivers

/**
 * @deprecated This type is deprecated and will be removed in the next major version.
 *             Please use `esp_panel::drivers::LCD_AXS15231B` instead.
 *
 */
typedef esp_panel::drivers::LCD_AXS15231B ESP_PanelLcd_AXS15231B __attribute__((deprecated("Deprecated and will be removed in the next major version. Please use `esp_panel::drivers::LCD_AXS15231B` instead.")));
