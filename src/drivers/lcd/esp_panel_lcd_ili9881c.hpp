/*
 * SPDX-FileCopyrightText: 2024-2025 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#pragma once

#include "esp_panel_lcd.hpp"

namespace esp_panel::drivers {

/**
 * @brief LCD driver class for ILI9881C display controller
 *
 */
class LCD_ILI9881C: public LCD {
public:
    /**
     * @brief Default basic attributes for ILI9881C LCD controller
     */
    static constexpr BasicAttributes BASIC_ATTRIBUTES_DEFAULT = {
        .name = "ILI9881C",
    };

// *INDENT-OFF*
    /**
     * @brief Initialize bus specifications map with supported configurations
     *
     * @return Map containing bus specifications for supported bus types
     */
    static constexpr BasicBusSpecificationMap initBusSpecifications()
    {
        return {
            {
                ESP_PANEL_BUS_TYPE_MIPI_DSI, BasicBusSpecification{
                    .color_bits = {
                        ESP_PANEL_LCD_COLOR_BITS_RGB565,
                        ESP_PANEL_LCD_COLOR_BITS_RGB666,
                        ESP_PANEL_LCD_COLOR_BITS_RGB888
                    },
                    .functions = (1U << BasicBusSpecification::FUNC_INVERT_COLOR) |
                                 (1U << BasicBusSpecification::FUNC_MIRROR_X) |
                                 (1U << BasicBusSpecification::FUNC_MIRROR_Y) |
                                 (1U << BasicBusSpecification::FUNC_DISPLAY_ON_OFF),
                },
            },
        };
    }
// *INDENT-ON*

    /**
     * @brief Construct the LCD device with individual parameters
     *
     * @param[in] bus Bus interface for communicating with the LCD device
     * @param[in] color_bits Color depth in bits per pixel (16 for RGB565, 18 for RGB666, 24 for RGB888)
     * @param[in] rst_io Reset GPIO pin number (-1 if not used)
     * @note This constructor uses default values for most configuration parameters. Use config*() functions to
     *       customize
     * @note Vendor initialization commands may vary between manufacturers. Consult LCD supplier for them and use
     *       `configVendorCommands()` to configure
     */
    LCD_ILI9881C(Bus *bus, uint8_t color_bits, int rst_io = -1):
        LCD(BASIC_ATTRIBUTES_DEFAULT, bus, color_bits, rst_io)
    {
    }

    /**
     * @brief Construct the LCD device with full configuration
     *
     * @param[in] bus Bus interface for communicating with the LCD device
     * @param[in] config Complete LCD configuration structure
     * @note Vendor initialization commands may vary between manufacturers. Consult LCD supplier for them
     */
    LCD_ILI9881C(Bus *bus, const Config &config):
        LCD(BASIC_ATTRIBUTES_DEFAULT, bus, config)
    {
    }

    /**
     * @brief Destroy the LCD device and free resources
     */
    ~LCD_ILI9881C() override;

    /**
     * @brief Initialize the LCD device
     *
     * @return `true` if initialization successful, `false` otherwise
     * @note This function must be called after bus interface is initialized
     * @note Creates panel handle by calling `esp_lcd_new_panel_*()` internally
     */
    bool init() override;

private:
    static const BasicBusSpecificationMap _bus_specifications;
};

} // namespace esp_panel::drivers

/**
 * @brief Backward compatibility type alias for `ESP_PanelLcd_ILI9881C` class
 * @deprecated Use `esp_panel::drivers::LCD_ILI9881C` instead. Will be removed in next major version
 */
using ESP_PanelLcd_ILI9881C [[deprecated("Use `esp_panel::drivers::LCD_ILI9881C` instead")]] =
    esp_panel::drivers::LCD_ILI9881C;
