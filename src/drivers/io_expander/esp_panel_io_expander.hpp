/*
 * SPDX-FileCopyrightText: 2024 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */
#pragma once

#include <memory>
#include <unordered_map>
#include <string>
#include "esp_panel_io_expander_base.hpp"

namespace esp_panel::drivers {

class IO_ExpanderFactory {
public:
    using CreateFunction = std::shared_ptr<IO_Expander> (*)(const IO_Expander::Config &config);

    IO_ExpanderFactory() = default;
    ~IO_ExpanderFactory() = default;

    static std::shared_ptr<IO_Expander> create(std::string name, const IO_Expander::Config &config);

private:
    static const std::unordered_map<std::string, CreateFunction> _name_function_map;
};

} // namespace esp_panel::drivers
