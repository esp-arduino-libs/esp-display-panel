/*
 * SPDX-FileCopyrightText: 2024-2025 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */
#pragma once

#include <unordered_map>
#include <memory>
#include <string>
#include <variant>
#include "soc/soc_caps.h"
#include "esp_panel_types.h"
#include "esp_panel_bus_base.hpp"
#include "esp_panel_bus_dsi.hpp"
#include "esp_panel_bus_i2c.hpp"
#include "esp_panel_bus_qspi.hpp"
#include "esp_panel_bus_rgb.hpp"
#include "esp_panel_bus_spi.hpp"

/**
 * @brief The bus factory class
 */
namespace esp_panel::drivers {

class BusFactory {
public:
    /**
     * @brief The bus configuration
     */
    using Config = std::variant <
                   BusI2C::Config, BusSPI::Config, BusQSPI::Config
#if SOC_LCD_RGB_SUPPORTED
                   , BusRGB::Config
#endif // SOC_LCD_RGB_SUPPORTED
#if SOC_MIPI_DSI_SUPPORTED
                   , BusDSI::Config
#endif // SOC_MIPI_DSI_SUPPORTED
                   >;

    /**
     * @brief The constructor function to create the bus device
     */
    using FunctionDeviceConstructor = std::shared_ptr<Bus> (*)(const Config &config);

    BusFactory() = default;
    ~BusFactory() = default;

    /**
     * @brief Create a new bus device with configuration.
     *
     * @param config The bus configuration
     *
     * @return The shared pointer to the device if success, otherwise return `nullptr`
     */
    static std::shared_ptr<Bus> create(const Config &config);

    /**
     * @brief Get the bus type of the configuration
     *
     * @param config The bus configuration
     *
     * @return The bus type (`ESP_PANEL_BACKLIGHT_TYPE_*`) if success, otherwise return `-1`
     */
    static int getConfigType(const Config &config);

    /**
     * @brief Get the bus type name string
     *
     * @param type The bus type (`ESP_PANEL_BACKLIGHT_TYPE_*`)
     *
     * @return The bus type name string if success, otherwise return `Unknown`
     */
    static std::string getTypeNameString(int type);

private:
    static const std::unordered_map<int, std::pair<std::string, FunctionDeviceConstructor>> _type_constructor_map;
};

} // namespace esp_panel::drivers
