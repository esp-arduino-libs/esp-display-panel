/*
 * SPDX-FileCopyrightText: 2024 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */
#pragma once

#include <string_view>
#include "esp_panel_utils.h"
#include "drivers/host/esp_panel_host_i2c.hpp"
#include "esp_panel_io_expander_base.hpp"

namespace esp_panel::drivers {

template <class T>
class IO_ExpanderAdapter: public IO_Expander, public T {
    static_assert(std::is_base_of<esp_expander::Base, T>::value, "`T` must be derived from `esp_expander::Base`");

public:
    IO_ExpanderAdapter(const esp_expander::Base::Config &config, const Attributes &attr):
        IO_Expander(config, attr),
        T(getBaseConfig(config))
    {
    }

    ~IO_ExpanderAdapter() override;

    bool init(void) override;
    bool begin(void) override;
    bool del(void) override;
    esp_expander::Base *getAdaptee(void) override
    {
        return static_cast<esp_expander::Base *>(this);
    }

public:
    std::shared_ptr<BusHost_I2C> _host = nullptr;
};

template <class T>
IO_ExpanderAdapter<T>::~IO_ExpanderAdapter()
{
    ESP_UTILS_LOG_TRACE_ENTER_WITH_THIS();

    ESP_UTILS_CHECK_FALSE_EXIT(del(), "Delete failed");

    ESP_UTILS_LOG_TRACE_EXIT_WITH_THIS();
}

template <class T>
bool IO_ExpanderAdapter<T>::init(void)
{
    ESP_UTILS_LOG_TRACE_ENTER_WITH_THIS();

    ESP_UTILS_CHECK_FALSE_RETURN(!this->checkIsInit(), false, "Already initialized");

    if (!this->IO_Expander::checkIsSkipInitHost()) {
        int host_id = static_cast<int>(this->getHostID());
        _host = BusHost_I2C::getInstance(host_id, this->getHostConfig());
        ESP_UTILS_CHECK_NULL_RETURN(_host, false, "Get I2C host(%d) instance failed", host_id);
        ESP_UTILS_LOGD("Get I2C host(%d) instance", host_id);
    }

    ESP_UTILS_CHECK_FALSE_RETURN(T::init(), false, "Init base failed");

    ESP_UTILS_LOG_TRACE_EXIT_WITH_THIS();

    return true;
}

template <class T>
bool IO_ExpanderAdapter<T>::begin(void)
{
    ESP_UTILS_LOG_TRACE_ENTER_WITH_THIS();

    ESP_UTILS_CHECK_FALSE_RETURN(this->checkIsInit(), false, "Not initialized");
    ESP_UTILS_CHECK_FALSE_RETURN(!this->checkIsBegun(), false, "Already begun");

    if (_host != nullptr) {
        int host_id = static_cast<int>(this->getHostID());
        ESP_UTILS_CHECK_FALSE_RETURN(_host->begin(), false, "Init host(%d) failed", host_id);
        ESP_UTILS_LOGD("Begin I2C host(%d)", host_id);
    }

    ESP_UTILS_CHECK_FALSE_RETURN(T::begin(), false, "Begin base failed");

    ESP_UTILS_LOG_TRACE_EXIT_WITH_THIS();

    return true;
}

template <class T>
bool IO_ExpanderAdapter<T>::del(void)
{
    ESP_UTILS_LOG_TRACE_ENTER_WITH_THIS();

    if (this->checkIsInit()) {
        if (_host != nullptr) {
            _host = nullptr;
            int host_id = static_cast<int>(this->getHostID());
            ESP_UTILS_CHECK_FALSE_RETURN(
                BusHost_I2C::tryReleaseInstance(host_id), false, "Release I2C host(%d) failed", host_id
            );
        }
    }

    ESP_UTILS_CHECK_FALSE_RETURN(T::del(), false, "Delete base failed");

    ESP_UTILS_LOG_TRACE_EXIT_WITH_THIS();

    return true;
}

} // namespace esp_panel::drivers
