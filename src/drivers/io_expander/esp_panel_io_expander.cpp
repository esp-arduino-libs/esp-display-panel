/*
 * SPDX-FileCopyrightText: 2024 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include "esp_panel_utils.h"
#include "esp_io_expander.hpp"
#include "esp_panel_conf_internal.h"
#include "esp_panel_io_expander_adapter.hpp"
#include "esp_panel_io_expander.hpp"

namespace esp_panel::drivers {

std::shared_ptr<IO_Expander> IO_ExpanderFactory::create(Chip name, const esp_expander::Base::Config &config)
{
    ESP_UTILS_LOGD(
        "Param: name(%d[%s]), config(@%p)", static_cast<int>(name), getChipNameString(name).c_str(), &config
    );

    std::shared_ptr<IO_Expander> device = nullptr;
    switch (name) {
    case Chip::TCA95XX_8BIT:
        ESP_UTILS_CHECK_EXCEPTION_RETURN(
            (device = esp_utils::make_shared<IO_ExpanderAdapter<esp_expander::TCA95XX_8BIT>>(config)), nullptr,
            "Create %s failed", getChipNameString(name).c_str()
        );
        break;
    default:
        ESP_UTILS_CHECK_FALSE_RETURN(false, nullptr, "Unknown chip");
    }

    return device;
}

std::string IO_ExpanderFactory::getChipNameString(Chip name)
{
    switch (name) {
    case Chip::TCA95XX_8BIT:
        return "TCA95XX_8BIT";
    default:
        return "Unknown";
    }
}

} // namespace esp_panel::drivers
