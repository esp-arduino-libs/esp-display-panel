/*
 * SPDX-FileCopyrightText: 2023-2025 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#pragma once

#include "esp_panel_lcd_base.hpp"

namespace esp_panel::drivers {

/**
 * @brief The ST7262 LCD device class
 *
 * @note  This class is a derived class of `LCD`, user can use it directly
 */
class LCD_ST7262: public LCD {
public:
    /**
     * Here are some default values for the device
     */
    static constexpr BasicAttributes BASIC_ATTRIBUTES_DEFAULT = {
        .name = "ST7262",
    };
// *INDENT-OFF*
    static constexpr BasicBusSpecificationMap initBusSpecifications()
    {
        return {
            {
                ESP_PANEL_BUS_TYPE_RGB, BasicBusSpecification{
                    .functions = (1U << BasicBusSpecification::FUNC_INVERT_COLOR) |
                                 (1U << BasicBusSpecification::FUNC_MIRROR_X) |
                                 (1U << BasicBusSpecification::FUNC_MIRROR_Y) |
                                 (1U << BasicBusSpecification::FUNC_SWAP_XY) |
                                 (1U << BasicBusSpecification::FUNC_GAP) |
                                 (1U << BasicBusSpecification::FUNC_DISPLAY_ON_OFF),
                },
            },
        };
    }
// *INDENT-OFF*

    /**
     * @brief Construct the LCD device with separate parameters
     *
     * @note  This function uses some default values to config the LCD device, use `config*()` functions to change them
     * @note  Vendor specific initialization commands can be different between manufacturers, should consult the LCD
     *        supplier for them and use `configVendorCommands()` to configure
     *
     * @param[in] panel_bus   Simple pointer of panel bus
     * @param[in] color_bits  Bits per pixel. Not used in this driver, can be ignored
     * @param[in] rst_io      Reset pin, set to -1 if not used
     */
    LCD_ST7262(Bus *panel_bus, uint8_t color_bits = 0, int rst_io = -1):
        LCD(BASIC_ATTRIBUTES_DEFAULT, panel_bus, color_bits, rst_io)
    {
    }

    /**
     * @brief Construct the LCD device with configuration
     *
     * @note  Vendor specific initialization commands can be different between manufacturers, should consult the LCD
     *        supplier for them
     *
     * @param[in] panel_bus Smart pointer of panel bus
     * @param[in] config    LCD configuration
     */
    LCD_ST7262(std::shared_ptr<Bus> panel_bus, const Config &config):
        LCD(BASIC_ATTRIBUTES_DEFAULT, panel_bus, config)
    {
    }

    /**
     * @brief Destroy the LCD device
     */
    ~LCD_ST7262() override;

    /**
     * @brief Initialize the LCD device.
     *
     * @note  This function should be called after bus is begun
     * @note  This function typically calls `esp_lcd_new_panel_*()` to create the refresh panel
     *
     * @return true if success, otherwise false
     */
    bool init() override;
    /**
     * @brief Reset the LCD. If the `rst_io` is not set, this function will do reset by software instead of hardware
     *
     * @note  This function should be called after `init()`
     * @note  This function typically calls `esp_lcd_panel_reset()` to reset the LCD device
     *
     * @return true if success, otherwise false
     */
    bool reset() override;

private:
    static const BasicBusSpecificationMap _bus_specifications;
};

} // namespace esp_panel::drivers

/**
 * @deprecated Deprecated. Please use `esp_panel::drivers::LCD_ST7262
 *             instead.
 */
typedef esp_panel::drivers::LCD_ST7262 LCD_ST7262 __attribute__((deprecated("Deprecated and will be removed in the \
next major version. Please use `esp_panel::drivers::LCD_ST7262` instead.")));
