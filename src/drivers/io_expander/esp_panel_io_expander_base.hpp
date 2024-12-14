/*
 * SPDX-FileCopyrightText: 2024 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */
#pragma once

// From the library `ESP32_IO_Expander`
#include "chip/esp_expander_base.hpp"

namespace esp_panel::drivers {

class IO_Expander {
public:
    struct Attributes {
        const char *name = nullptr;
    };

    using Config = esp_expander::Base::Config;

    IO_Expander(const esp_expander::Base::Config &config, Attributes attr):
        _attritues(attr)
    {
        _flags.skip_init_host = config.skip_init_host;
    }

    virtual ~IO_Expander() = default;

    virtual bool init(void) = 0;
    virtual bool begin(void) = 0;
    virtual bool del(void) = 0;
    virtual esp_expander::Base *getAdaptee(void) = 0;

    const Attributes &getAttributes(void)
    {
        return _attritues;
    }

protected:
    bool checkIsSkipInitHost(void)
    {
        return _flags.skip_init_host;
    }

    static esp_expander::Base::Config getBaseConfig(const esp_expander::Base::Config &config)
    {
        esp_expander::Base::Config base_config = config;
        base_config.skip_init_host = true;

        return base_config;
    }

private:
    struct {
        uint8_t skip_init_host: 1;
    } _flags = {};
    Attributes _attritues = {};
};

} // namespace esp_panel::drivers
