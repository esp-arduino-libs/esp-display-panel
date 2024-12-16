/*
 * SPDX-FileCopyrightText: 2024-2025 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */
#pragma once

#include <memory>
#include "esp_panel_utils.h"

namespace esp_panel::drivers {

/**
 * @brief The base bus host template class. This class implements a variant of the `Singleton pattern`
 *
 * @tparam Derived Derived class
 * @tparam Config  Configuration type
 * @tparam N       Number of instances
 */
template <class Derived, typename Config, int N>
class Host {
public:
    static_assert(N > 0, "Number of instances must be greater than 0");

    using HostHandle = void *;

    /**
     * @brief The driver state enumeration
     */
    enum class State : uint8_t {
        DEINIT = 0,
        BEGIN,
    };

    /* Remove copy constructor and copy assignment operator */
    Host(const Host &) = delete;
    Host &operator=(const Host &) = delete;

    /**
     * @brief Destroy the host
     */
    virtual ~Host() = default;

    /**
     * @brief Startup the host
     *
     * @return true if success, otherwise false
     */
    virtual bool begin() = 0;

    /**
     * @brief Get the ID of the host
     *
     * @return The host ID
     */
    int getID() const
    {
        return _id;
    }

    /**
     * @brief Get the handle of the host
     *
     * @return The handle of the host
     */
    HostHandle getHandle() const
    {
        return host_handle;
    }

    /**
     * @brief Check if the driver has reached or passed the specified state
     *
     * @param[in] state The state to check against current state
     *
     * @return true if current state >= given state, otherwise false
     */
    bool isOverState(State state)
    {
        return (_state >= state);
    }

    /**
     * @brief Get a instance of the host. If the instance is not exist, it will be created
     *
     * @param[in] id     The host ID
     * @param[in] config The host configuration
     *
     * @return The instance pointer of the derived class
     */
    static std::shared_ptr<Derived> getInstance(int id, const Config &config);

    /**
     * @brief Try to release the instance. If the instance is not used by other objects, it will be released. Otherwise,
     *        it will calibrate the configuration between the instance and the new configuration
     *
     * @param[in] id The host ID
     *
     * @return true if success, otherwise false
     */
    static bool tryReleaseInstance(int id);

protected:
    /* Only derived class can construct the base host */
    Host(int id, const Config &config): config(config), _id(id) {}

    void setState(State state)
    {
        _state = state;
    }

    Config config = {};
    HostHandle host_handle = nullptr;

private:
    virtual bool calibrateConfig(const Config &config) = 0;

    int _id = -1;
    State _state = State::DEINIT;
    static std::array<std::shared_ptr<Derived>, N> _instances;
};

template <class Derived, typename Config, int N>
std::array<std::shared_ptr<Derived>, N> Host<Derived, Config, N>::_instances = {};

template <class Derived, typename Config, int N>
bool Host<Derived, Config, N>::tryReleaseInstance(int id)
{
    ESP_UTILS_LOG_TRACE_ENTER();

    ESP_UTILS_LOGD("Param: id(%d)", id);
    ESP_UTILS_CHECK_FALSE_RETURN((size_t)id < _instances.size(), false, "Invalid ID");

    if ((_instances[id] != nullptr) && (_instances[id].use_count() == 1)) {
        _instances[id] = nullptr;
        ESP_UTILS_LOGD("Release host(%d)", id);
    }

    ESP_UTILS_LOG_TRACE_EXIT();

    return true;
}

template <class Derived, typename Config, int N>
std::shared_ptr<Derived> Host<Derived, Config, N>::getInstance(int id, const Config &config)
{
    ESP_UTILS_LOG_TRACE_ENTER();

    ESP_UTILS_LOGD("Param: id(%d), config(@%p)", id, &config);
    ESP_UTILS_CHECK_FALSE_RETURN((size_t)id < _instances.size(), nullptr, "Invalid host ID");

    if (_instances[id] == nullptr) {
        ESP_UTILS_CHECK_EXCEPTION_RETURN(
            (_instances[id] = utils::make_shared<Derived>(id, config)), nullptr, "Create instance failed"
        );
        ESP_UTILS_LOGD("No instance exist, create new one(@%p)", _instances[id].get());
    } else {
        ESP_UTILS_LOGD("Instance exist(@%p)", _instances[id].get());

        Config new_config = config;
        ESP_UTILS_CHECK_FALSE_RETURN(
            _instances[id]->calibrateConfig(new_config), nullptr,
            "Calibrate configuration failed, attempt to configure host with a incompatible configuration"
        );
    }

    ESP_UTILS_LOG_TRACE_EXIT();

    return _instances[id];
}

} // namespace esp_panel::drivers
