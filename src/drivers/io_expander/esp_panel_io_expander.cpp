/*
 * SPDX-FileCopyrightText: 2024-2025 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include "esp_panel_utils.h"
#include "esp_io_expander.hpp"
#include "esp_panel_conf_internal.h"
#include "esp_panel_io_expander_adapter.hpp"
#include "esp_panel_io_expander.hpp"

namespace esp_panel::drivers {

#define DEVICE_CREATOR(chip) \
    [](const IO_Expander::Config &config) { \
        std::shared_ptr<IO_Expander> device = nullptr; \
        ESP_UTILS_CHECK_EXCEPTION_RETURN( \
            (device = utils::make_shared<IO_ExpanderAdapter<esp_expander::chip>>( \
                IO_Expander::BasicAttributes{#chip}, config \
            )), nullptr, "Create " #chip " failed" \
        ); \
        return device; \
    }
#define ITEM_CONTROLLER(chip) \
    {#chip, DEVICE_CREATOR(chip)}

const std::unordered_map<std::string, IO_ExpanderFactory::FunctionDeviceConstructor>
IO_ExpanderFactory::_name_function_map = {
    ITEM_CONTROLLER(CH422G),
    ITEM_CONTROLLER(HT8574),
    ITEM_CONTROLLER(TCA95XX_8BIT),
    ITEM_CONTROLLER(TCA95XX_16BIT),
};

std::shared_ptr<IO_Expander> IO_ExpanderFactory::create(std::string name, const IO_Expander::Config &config)
{
    ESP_UTILS_LOGD("Param: name(%s), config(@%p)", name.c_str(), &config);

    auto it = _name_function_map.find(name);
    ESP_UTILS_CHECK_FALSE_RETURN(it != _name_function_map.end(), nullptr, "Unknown controller: %s", name.c_str());

    std::shared_ptr<IO_Expander> device = it->second(config);
    ESP_UTILS_CHECK_NULL_RETURN(device, nullptr, "Create device failed");

    return device;
}

} // namespace esp_panel::drivers
