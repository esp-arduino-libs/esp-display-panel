/*
 * SPDX-FileCopyrightText: 2023-2024 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#pragma once

#include <string>
#include "esp_lcd_types.h"
#include "esp_lcd_panel_io.h"

namespace esp_panel::drivers {

/**
 * @brief bus class
 *
 * @note  This class is a base class for all types of bus. Due to it is a virtual class, users cannot use it directly
 *
 */
class Bus {
public:
    /**
     * @brief The attributes of base bus
     *
     */
    struct Attributes {
        int type = -1;                  /*<! The bus type. Default is `-1` */
        const char *name = nullptr;     /*<! The bus name. Default is `nullptr` */
    };

    /**
     * @brief Construct a new bus
     *
     * @param[in] attr The bus attributes
     *
     */
    Bus(const Attributes &attr): _attributes(attr) {}

    /**
     * @brief Destroy the bus
     *
     */
    virtual ~Bus() = default;

    /**
     * @brief Initialize the bus
     *
     * @return true if success, otherwise false
     *
     */
    virtual bool init(void)
    {
        return true;
    }

    /**
     * @brief Startup the bus
     *
     * @return true if success, otherwise false
     *
     */
    virtual bool begin(void) = 0;

    /**
     * @brief Delete the bus, release the resources
     *
     * @note  This function typically call `esp_lcd_panel_io_del()` to delete the IO handle
     * @note  After calling this function, users should call `init()` to re-init the bus
     *
     * @return true if success, otherwise false
     *
     */
    virtual bool del(void);

    /**
     * @brief Read the register data
     *
     * @param[in]  address   The address of the register
     * @param[out] data      The buffer to store the register data
     * @param[in]  data_size The size of the data (in bytes)
     *
     * @return true if success, otherwise false
     *
     */
    bool readRegisterData(uint32_t address, void *data, uint32_t data_size);

    /**
     * @brief Write the register data
     *
     * @param[in] address   The address of the register
     * @param[in] data      The buffer to store the register data
     * @param[in] data_size The size of the data (in bytes)
     *
     * @return true if success, otherwise false
     *
     */
    bool writeRegisterData(uint32_t address, const void *data, uint32_t data_size);

    /**
     * @brief Write the color data
     *
     * @param[in] address   The address of the register
     * @param[in] data      The buffer to store the color data
     * @param[in] data_size The size of the data (in bytes)
     *
     * @return true if success, otherwise false
     *
     */
    bool writeColorData(uint32_t address, const void *color, uint32_t color_size);

    /**
     * @brief Get the bus attributes
     *
     * @return The bus attributes
     *
     */
    const Attributes &getAttributes(void)
    {
        return _attributes;
    }

    /**
     * @brief Get the IO handle. Users can use this handle to call low-level functions (esp_lcd_panel_io_*()).
     *
     * @return Handle if success, otherwise nullptr
     *
     */
    esp_lcd_panel_io_handle_t getIO_Handle(void)
    {
        return io_handle;
    }

    /**
     * @brief Disable the IO handle
     *
     */
    void disableIO_Handle(void)
    {
        io_handle = nullptr;
    }

    /**
     * @deprecated Deprecated and will be removed in the next major version. Please use `getAttributes().type` instead.
     *
     * @TODO: Remove in the next major version
     *
     */
    [[deprecated("Deprecated and will be removed in the next major version. Please use `getAttributes().type` instead.")]]
    int getType(void)
    {
        return getAttributes().type;
    }

protected:
    /**
     * @brief Check if the IO handle is valid
     *
     * @return true if the IO handle is valid, otherwise false
     *
     */
    bool checkIO_HandleValid(void)
    {
        return (io_handle != nullptr);
    }

    esp_lcd_panel_io_handle_t io_handle = nullptr;  /*<! Low-level handle, which will be created by the derived class */

private:
    Attributes _attributes = {};
};

} // namespace esp_panel::drivers

/**
 * @deprecated Deprecated and will be removed in the next major version. Please use `esp_panel::drivers::Bus` instead.
 *
 * @TODO: Remove in the next major version
 *
 */
typedef esp_panel::drivers::Bus ESP_PanelBus __attribute__((deprecated("Deprecated and will be removed in the next major version. Please use `esp_panel::drivers::Bus` instead.")));
