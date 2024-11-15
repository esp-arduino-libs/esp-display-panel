/*
 * SPDX-FileCopyrightText: 2024 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */
#include "esp_panel_utils.h"
#include "esp_panel_bus.hpp"

namespace esp_panel::drivers {

std::shared_ptr<Bus> BusFactory::create(int type, const void *config)
{
    ESP_UTILS_LOG_TRACE_ENTER();

    ESP_UTILS_LOGD("Param: type(%d[%s]), config(@%p)", type, getTypeString(type).c_str(), config);
    ESP_UTILS_CHECK_FALSE_RETURN(config != nullptr, nullptr, "Invalid config");

    std::shared_ptr<Bus> bus = nullptr;
    switch (type) {
    case ESP_PANEL_BUS_TYPE_I2C:
        ESP_UTILS_CHECK_EXCEPTION_RETURN(
            (bus = esp_utils::make_shared<Bus_I2C>(*static_cast<const Bus_I2C::Config *>(config))), nullptr,
            "Create %s bus failed", getTypeString(type).c_str()
        );
        break;
    case ESP_PANEL_BUS_TYPE_SPI:
        ESP_UTILS_CHECK_EXCEPTION_RETURN(
            (bus = esp_utils::make_shared<Bus_SPI>(*static_cast<const Bus_SPI::Config *>(config))), nullptr,
            "Create %s bus failed", getTypeString(type).c_str()
        );
        break;
#if SOC_LCD_RGB_SUPPORTED
    case ESP_PANEL_BUS_TYPE_RGB:
        ESP_UTILS_CHECK_EXCEPTION_RETURN(
            (bus = esp_utils::make_shared<Bus_RGB>(*static_cast<const Bus_RGB::Config *>(config))), nullptr,
            "Create %s bus failed", getTypeString(type).c_str()
        );
        break;
#endif // SOC_LCD_RGB_SUPPORTED
#if SOC_MIPI_DSI_SUPPORTED
    case ESP_PANEL_BUS_TYPE_MIPI_DSI:
        ESP_UTILS_CHECK_EXCEPTION_RETURN(
            (bus = esp_utils::make_shared<Bus_DSI>(*static_cast<const Bus_DSI::Config *>(config))), nullptr,
            "Create %s bus failed", getTypeString(type).c_str()
        );
        break;
#endif // SOC_MIPI_DSI_SUPPORTED
    default:
        ESP_UTILS_CHECK_FALSE_RETURN(false, nullptr, "Unknown type");
    }

    ESP_UTILS_LOG_TRACE_EXIT();

    return bus;
}

std::string BusFactory::getTypeString(int type)
{
    switch (type) {
    case ESP_PANEL_BUS_TYPE_I2C:
        return "I2C";
    case ESP_PANEL_BUS_TYPE_SPI:
        return "SPI";
    case ESP_PANEL_BUS_TYPE_QSPI:
        return "QSPI";
    case ESP_PANEL_BUS_TYPE_RGB:
        return "RGB";
    case ESP_PANEL_BUS_TYPE_MIPI_DSI:
        return "MIPI-DSI";
    default:
        return "Unknown";
    }
}

} // namespace esp_panel::drivers
