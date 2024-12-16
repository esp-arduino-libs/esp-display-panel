/*
 * SPDX-FileCopyrightText: 2024-2025 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */
#pragma once

#include <concepts>
#include "esp_panel_utils.h"
#include "drivers/host/esp_panel_host_i2c.hpp"
#include "esp_panel_io_expander_base.hpp"

namespace esp_panel::drivers {

/**
 * @brief Check whether the type is derived from `esp_expander::Base`
 *
 * @tparam T The type to be checked
 */
template<class T>
concept CheckIsIO_ExpanderAdaptee = std::is_base_of_v<esp_expander::Base, T>;

template <class T>
requires CheckIsIO_ExpanderAdaptee<T>
class IO_ExpanderAdapter: public IO_Expander, public T {
public:
    IO_ExpanderAdapter(const BasicAttributes &attr, const esp_expander::Base::Config &config):
        IO_Expander(attr, config),
        T(processConfig(config))
    {
    }

    ~IO_ExpanderAdapter() override;

    bool init() override;
    bool begin() override;
    bool del() override;
    bool isOverState(esp_expander::Base::State state) const override
    {
        return T::isOverState(state);
    }
    esp_expander::Base *getBase() override
    {
        return static_cast<esp_expander::Base *>(this);
    }

public:
    std::shared_ptr<HostI2C> _host = nullptr;
};

template <class T>
requires CheckIsIO_ExpanderAdaptee<T>
IO_ExpanderAdapter<T>::~IO_ExpanderAdapter()
{
    ESP_UTILS_LOG_TRACE_ENTER_WITH_THIS();

    ESP_UTILS_CHECK_FALSE_EXIT(del(), "Delete failed");

    ESP_UTILS_LOG_TRACE_EXIT_WITH_THIS();
}

template <class T>
requires CheckIsIO_ExpanderAdaptee<T>
bool IO_ExpanderAdapter<T>::init()
{
    ESP_UTILS_LOG_TRACE_ENTER_WITH_THIS();

    ESP_UTILS_CHECK_FALSE_RETURN(T::init(), false, "Init base failed");

    // Since the host full configuration is converted from the partial configuration, we need to call `init` to
    // get the full configuration
    if (!this->IO_Expander::isSkipInitHost()) {
        auto host_config = this->getConfig().getHostFullConfig();
        ESP_UTILS_CHECK_NULL_RETURN(host_config, false, "Get host full config failed");

        auto host_id = this->getConfig().host_id;
        _host = HostI2C::getInstance(host_id, *host_config);
        ESP_UTILS_CHECK_NULL_RETURN(_host, false, "Get I2C host(%d) instance failed", host_id);

        ESP_UTILS_LOGD("Get I2C host(%d) instance", host_id);
    }

    ESP_UTILS_LOG_TRACE_EXIT_WITH_THIS();

    return true;
}

template <class T>
requires CheckIsIO_ExpanderAdaptee<T>
bool IO_ExpanderAdapter<T>::begin()
{
    ESP_UTILS_LOG_TRACE_ENTER_WITH_THIS();

    ESP_UTILS_CHECK_FALSE_RETURN(!this->isOverState(T::State::BEGIN), false, "Already begun");

    if (!this->isOverState(T::State::INIT)) {
        ESP_UTILS_CHECK_FALSE_RETURN(this->init(), false, "Init failed");
    }

    if (_host != nullptr) {
        int host_id = this->getConfig().host_id;
        ESP_UTILS_CHECK_FALSE_RETURN(_host->begin(), false, "Init host(%d) failed", host_id);
        ESP_UTILS_LOGD("Begin I2C host(%d)", host_id);
    }

    ESP_UTILS_CHECK_FALSE_RETURN(T::begin(), false, "Begin base failed");

    ESP_UTILS_LOG_TRACE_EXIT_WITH_THIS();

    return true;
}

template <class T>
requires CheckIsIO_ExpanderAdaptee<T>
bool IO_ExpanderAdapter<T>::del()
{
    ESP_UTILS_LOG_TRACE_ENTER_WITH_THIS();

    if (_host != nullptr) {
        _host = nullptr;
        int host_id = this->getConfig().host_id;
        ESP_UTILS_CHECK_FALSE_RETURN(
            HostI2C::tryReleaseInstance(host_id), false, "Release I2C host(%d) failed", host_id
        );
    }

    ESP_UTILS_CHECK_FALSE_RETURN(T::del(), false, "Delete base failed");

    ESP_UTILS_LOG_TRACE_EXIT_WITH_THIS();

    return true;
}

} // namespace esp_panel::drivers
