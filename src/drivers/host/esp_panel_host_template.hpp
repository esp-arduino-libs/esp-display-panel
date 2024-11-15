/*
 * SPDX-FileCopyrightText: 2024 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */
#pragma once

#include "esp_panel_utils.h"

namespace esp_panel::drivers {

/**
 * @brief The base bus host template class. This class implements a variant of the `Singleton pattern`
 *
 * @tparam Derived Derived class
 * @tparam Config  Configuration type
 * @tparam N       Number of instances
 *
 */
template <class Derived, typename Config, int N>
class BusHost {
public:
    /* Remove copy constructor and copy assignment operator */
    BusHost(const BusHost &) = delete;
    BusHost &operator=(const BusHost &) = delete;

    /**
     * @brief Destroy the host
     *
     */
    virtual ~BusHost() = default;

    /**
     * @brief Startup the host
     *
     * @return true if success, otherwise false
     *
     */
    virtual bool begin(void) = 0;

    /**
     * @brief Get the host ID
     *
     * @return The host ID
     *
     */
    int getID(void) const
    {
        return _id;
    }

    /**
     * @brief Get a instance of the host. If the instance is not exist, it will be created
     *
     * @param[in] id     The host ID
     * @param[in] config The host configuration
     *
     * @return The instance pointer of the derived class
     *
     */
    static std::shared_ptr<Derived> getInstance(int id, const Config &config);

    /**
     * @brief Try to release the instance. If the instance is not used by other objects, it will be released. Otherwise,
     *        it will calibrate the configuration between the instance and the new configuration
     *
     * @param[in] id The host ID
     *
     * @return true if success, otherwise false
     *
     */
    static bool tryReleaseInstance(int id);

protected:
    /* Only derived class can construct the base host */
    BusHost(int id, const Config &config): config(config), _id(id) {}

    /**
     * @brief Check if the device is already begun
     *
     * @return true if the device is already begun, otherwise false
     *
     */
    bool checkIsBegun(void) const
    {
        return flags.is_begun;
    }

    struct {
        uint8_t is_begun: 1;
    } flags = {};
    Config config = {};

private:
    virtual bool calibrateConfig(const Config &config) = 0;

    int _id = -1;
    static std::array<std::shared_ptr<Derived>, N> _instance_array;
};

template <class Derived, typename Config, int N>
std::array<std::shared_ptr<Derived>, N> BusHost<Derived, Config, N>::_instance_array = {};

template <class Derived, typename Config, int N>
bool BusHost<Derived, Config, N>::tryReleaseInstance(int id)
{
    ESP_UTILS_LOG_TRACE_ENTER();

    ESP_UTILS_LOGD("Param: id(%d)", id);
    ESP_UTILS_CHECK_FALSE_RETURN((size_t)id < _instance_array.size(), false, "Invalid ID");

    if ((_instance_array[id] != nullptr) && (_instance_array[id].use_count() == 1)) {
        _instance_array[id] = nullptr;
        ESP_UTILS_LOGD("Release host(%d)", id);
    }

    ESP_UTILS_LOG_TRACE_EXIT();

    return true;
}

template <class Derived, typename Config, int N>
std::shared_ptr<Derived> BusHost<Derived, Config, N>::getInstance(int id, const Config &config)
{
    ESP_UTILS_LOG_TRACE_ENTER();

    ESP_UTILS_LOGD("Param: id(%d), config(@%p)", id, &config);
    ESP_UTILS_CHECK_FALSE_RETURN((size_t)id < _instance_array.size(), nullptr, "Invalid host ID");

    if (_instance_array[id] == nullptr) {
        ESP_UTILS_CHECK_EXCEPTION_RETURN(
            (_instance_array[id] = esp_utils::make_shared<Derived>(id, config)), nullptr, "Create instance failed"
        );
        ESP_UTILS_LOGD("Derived not exist, create new instance(@%p)", _instance_array[id].get());
    } else {
        ESP_UTILS_LOGD("Derived already exist(@%p)", _instance_array[id].get());

        Config new_config = config;
        ESP_UTILS_CHECK_FALSE_RETURN(
            _instance_array[id]->calibrateConfig(new_config), nullptr,
            "Calibrate configuration failed, attempt to configure host with a incompatible configuration"
        );
    }

    ESP_UTILS_LOG_TRACE_EXIT();

    return _instance_array[id];
}

} // namespace esp_panel::drivers
