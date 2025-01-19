/*
 * SPDX-FileCopyrightText: 2023-2025 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#pragma once

#include "esp_panel_lcd_conf_internal.h"
#include "esp_panel_lcd.hpp"

namespace esp_panel::drivers {

/**
 * @brief LCD driver class for SPD2010 display controller
 *
 */
class LCD_SPD2010: public LCD {
public:
    /**
     * @brief Default basic attributes for SPD2010 LCD controller
     */
    static constexpr BasicAttributes BASIC_ATTRIBUTES_DEFAULT = {
        .name = "SPD2010",
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
                ESP_PANEL_BUS_TYPE_SPI, BasicBusSpecification{
                    .x_coord_align = 4,
                    .color_bits = {
                        ESP_PANEL_LCD_COLOR_BITS_RGB565,
                        ESP_PANEL_LCD_COLOR_BITS_RGB666,
                    },
                    .functions = (1U << BasicBusSpecification::FUNC_INVERT_COLOR) |
                                 (1U << BasicBusSpecification::FUNC_MIRROR_X) |
                                 (1U << BasicBusSpecification::FUNC_MIRROR_Y) |
                                 (1U << BasicBusSpecification::FUNC_GAP) |
                                 (1U << BasicBusSpecification::FUNC_DISPLAY_ON_OFF),
                },
            },
            {
                ESP_PANEL_BUS_TYPE_QSPI, BasicBusSpecification{
                    .x_coord_align = 4,
                    .color_bits = {
                        ESP_PANEL_LCD_COLOR_BITS_RGB565,
                        ESP_PANEL_LCD_COLOR_BITS_RGB666,
                    },
                    .functions = (1U << BasicBusSpecification::FUNC_INVERT_COLOR) |
                                 (1U << BasicBusSpecification::FUNC_MIRROR_X) |
                                 (1U << BasicBusSpecification::FUNC_MIRROR_Y) |
                                 (1U << BasicBusSpecification::FUNC_GAP) |
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
    LCD_SPD2010(Bus *bus, uint8_t color_bits, int rst_io = -1):
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
    LCD_SPD2010(Bus *bus, const Config &config):
        LCD(BASIC_ATTRIBUTES_DEFAULT, bus, config)
    {
    }

    /**
     * @brief Destroy the LCD device and free resources
     */
    ~LCD_SPD2010() override;

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
 * @brief Backward compatibility type alias for `ESP_PanelLcd_SPD2010` class
 * @deprecated Use `esp_panel::drivers::LCD_SPD2010` instead. Will be removed in next major version
 */
using ESP_PanelLcd_SPD2010 [[deprecated("Use `esp_panel::drivers::LCD_SPD2010` instead")]] =
    esp_panel::drivers::LCD_SPD2010;