/*
 * SPDX-FileCopyrightText: 2024 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */
#pragma once

#include <memory>
#include <unordered_map>
#include <string>
#include "esp_panel_types.h"
#include "esp_panel_bus_base.hpp"
#include "esp_panel_bus_dsi.hpp"
#include "esp_panel_bus_i2c.hpp"
#include "esp_panel_bus_qspi.hpp"
#include "esp_panel_bus_rgb.hpp"
#include "esp_panel_bus_spi.hpp"

namespace esp_panel::drivers {

class BusFactory {
public:
    using CreateFunction = std::shared_ptr<Bus> (*)(const void *config);

    BusFactory() = default;
    ~BusFactory() = default;

    static std::shared_ptr<Bus> create(int type, const void *config);
    static std::string getTypeNameString(int type);

private:
    static const std::unordered_map<int, std::pair<std::string, CreateFunction>> _type_name_function_map;
};

} // namespace esp_panel::drivers
