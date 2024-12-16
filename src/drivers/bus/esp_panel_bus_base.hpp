/*
 * SPDX-FileCopyrightText: 2023-2025 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#pragma once

#include <bitset>
#include <string>
#include "esp_lcd_types.h"
#include "esp_lcd_panel_io.h"

namespace esp_panel::drivers {

/**
 * @brief The bus base class
 *
 * @note  This class is a base class for all types of bus. Due to it is a virtual class, users cannot use it directly
 */
class Bus {
public:
    using ControlPanelHandle = esp_lcd_panel_io_handle_t;

    /**
     * @brief The bus basic attributes structure
     */
    struct BasicAttributes {
        int type = -1;                  /*!< The bus type. Default is `-1` */
        const char *name = "Unknown";   /*!< The bus name. Default is `Unknown` */
    };

    /**
     * @brief The driver state enumeration
     */
    enum class State : uint8_t {
        DEINIT = 0,
        INIT,
        BEGIN,
    };

    /**
     * @brief Construct the bus with basic attributes
     *
     * @param[in] attr The bus attributes
     */
    Bus(const BasicAttributes &attr): _basic_attributes(attr) {}

    /**
     * @brief Destroy the bus
     */
    virtual ~Bus() = default;

    /**
     * @brief Initialize the bus
     *
     * @return true if success, otherwise false
     */
    virtual bool init() = 0;

    /**
     * @brief Startup the bus
     *
     * @return true if success, otherwise false
     */
    virtual bool begin() = 0;

    /**
     * @brief Delete the bus, release the resources
     *
     * @note  After calling this function, users should call `init()` to re-init the bus
     *
     * @return true if success, otherwise false
     */
    virtual bool del() = 0;

    /**
     * @brief Delete the control panel
     *
     * @note  This function typically call `esp_lcd_panel_io_del()` to delete the control panel
     *
     * @return true if success, otherwise false
     */
    bool delControlPanel();

    /**
     * @brief Read the register data
     *
     * @param[in]  address   The address of the register
     * @param[out] data      The buffer to store the register data
     * @param[in]  data_size The size of the data (in bytes)
     *
     * @return true if success, otherwise false
     */
    bool readRegisterData(uint32_t address, void *data, uint32_t data_size) const;

    /**
     * @brief Write the register data
     *
     * @param[in] address   The address of the register
     * @param[in] data      The buffer to store the register data
     * @param[in] data_size The size of the data (in bytes)
     *
     * @return true if success, otherwise false
     */
    bool writeRegisterData(uint32_t address, const void *data, uint32_t data_size) const;

    /**
     * @brief Write the color data
     *
     * @param[in] address   The address of the register
     * @param[in] data      The buffer to store the color data
     * @param[in] data_size The size of the data (in bytes)
     *
     * @return true if success, otherwise false
     */
    bool writeColorData(uint32_t address, const void *color, uint32_t color_size) const;

    /**
     * @brief Disable the control panel
     *
     * @note  This function is useful for some "3-wire SPI + RGB" LCD panels, which need to disable the control panel
     *        when the `auto_del_panel_io/enable_io_multiplex` flag is set
     */
    void disableControlPanelHandle()
    {
        control_panel = nullptr;
    }

    /**
     * @brief Check if the driver has reached or passed the specified state
     *
     * @param[in] state The state to check against current state
     *
     * @return true if current state >= given state, otherwise false
     */
    bool isOverState(State state) const
    {
        return (_state >= state);
    }

    /**
     * @brief Get the bus attributes
     *
     * @return The bus attributes
     */
    const BasicAttributes &getBasicAttributes() const
    {
        return _basic_attributes;
    }

    /**
     * @brief Get the control panel handle. Users can use this handle to call low-level functions (esp_lcd_panel_io_*())
     *
     * @return Control panel if success, otherwise nullptr
     */
    ControlPanelHandle getControlPanelHandle() const
    {
        return control_panel;
    }

    /**
     * @deprecated Deprecated. Please use `getBasicAttributes().type`
     *             instead.
     *
     * @TODO: Remove in the next major version
     */
    [[deprecated("Deprecated. Please use `getBasicAttributes().type` \
    instead.")]]
    int getType()
    {
        return getBasicAttributes().type;
    }

protected:
    void setState(State state)
    {
        _state = state;
    }

    bool isControlPanelValid() const
    {
        return (control_panel != nullptr);
    }

    ControlPanelHandle control_panel = nullptr;  /*!< Should be created by the derived class */

private:
    State _state = State::DEINIT;
    BasicAttributes _basic_attributes = {};
};

} // namespace esp_panel::drivers

/**
 * @deprecated Deprecated. Please use `esp_panel::drivers::Bus` instead.
 *
 * @TODO: Remove in the next major version
 */
typedef esp_panel::drivers::Bus ESP_PanelBus __attribute__((deprecated("Deprecated and will be removed in the next \
major version. Please use `esp_panel::drivers::Bus` instead.")));
