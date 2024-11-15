/*
 * SPDX-FileCopyrightText: 2024 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */
#pragma once

#include <memory>
#include <string>
#include "esp_panel_io_expander_base.hpp"

namespace esp_panel::drivers {

class IO_ExpanderFactory {
public:
    enum class Chip {
        CH422G,
        HT8574,
        TCA95XX_8BIT, TCA95XX_16BIT,
    };

    IO_ExpanderFactory() = default;
    ~IO_ExpanderFactory() = default;

    static std::shared_ptr<IO_Expander> create(Chip name, const esp_expander::Base::Config &config);

    static std::string getChipNameString(Chip name);
};

} // namespace esp_panel::drivers
