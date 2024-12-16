/*
 * SPDX-FileCopyrightText: 2024-2025 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */
#pragma once

// From the library `ESP32_IO_Expander`
#include "chip/esp_expander_base.hpp"

namespace esp_panel::drivers {

/**
 * @brief The IO expander base class
 */
class IO_Expander {
public:
    /**
     * @brief The IO expander device basic attributes structure
     */
    struct BasicAttributes {
        const char *name = "Unknown";   /*!< The chip name. Default is `Unknown` */
    };

    using Config = esp_expander::Base::Config;

    /**
     * @brief Construct the IO expander with configuration
     *
     * @param[in] attr   The IO expander attributes
     * @param[in] config The IO expander configuration
     */
    IO_Expander(const BasicAttributes &attr, const esp_expander::Base::Config &config):
        _attritues(attr)
    {
        is_skip_init_host = config.skip_init_host;
    }

    virtual ~IO_Expander() = default;

    virtual bool init() = 0;
    virtual bool begin() = 0;
    virtual bool del() = 0;
    virtual bool isOverState(esp_expander::Base::State state) const = 0;
    virtual esp_expander::Base *getBase() = 0;

    /**
     * @brief Skip the initialization of the host
     *
     * @note  This function should be called before `init()`
     * @note  This function is useful when the host needs to be initialized outside the driver
     *
     * @return true if success, otherwise false
     */
    bool skipInitHost()
    {
        ESP_UTILS_LOG_TRACE_ENTER_WITH_THIS();
        ESP_UTILS_CHECK_FALSE_RETURN(
            !isOverState(esp_expander::Base::State::INIT), false, "Should be called before init()"
        );
        is_skip_init_host = true;
        ESP_UTILS_LOG_TRACE_EXIT_WITH_THIS();
        return true;
    }

    const BasicAttributes &getBasicAttributes() const
    {
        return _attritues;
    }

protected:
    bool isSkipInitHost()
    {
        return is_skip_init_host;
    }

    static esp_expander::Base::Config processConfig(const esp_expander::Base::Config &config)
    {
        esp_expander::Base::Config ret_config = config;
        ret_config.skip_init_host = true;
        return ret_config;
    }

private:
    bool is_skip_init_host = false;
    BasicAttributes _attritues = {};
};

} // namespace esp_panel::drivers
