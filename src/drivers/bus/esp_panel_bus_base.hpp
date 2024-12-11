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
 * @brief Bus object class
 *
 * @note  This class is a base class for all types of bus. Due to it is a virtual class, users cannot use it directly
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
     * @brief Construct a new bus object, the `begin()` function should be called after this function
     *
     * @param attr Bus configuration
     */
    Bus(const Attributes &attr): _attributes(attr) {}

    /**
     * @brief Destroy the bus object
     *
     */
    virtual ~Bus() = default;

    /**
     * @brief Initialize the bus
     *
     * @return true if success, otherwise false
     */
    virtual bool init(void)
    {
        return true;
    }

    /**
     * @brief Startup the bus
     *
     * @return true if success, otherwise false
     */
    virtual bool begin(void) = 0;

    /**
     * @brief Delete the bus object, release the resources
     *
     * @return true if success, otherwise false
     */
    virtual bool del(void);

    /**
     * @brief Read the register data
     *
     * @param address   The address of the register
     * @param data      The buffer to store the register data
     * @param data_size The size of the data (in bytes)
     *
     * @return true if success, otherwise false
     */
    bool readRegisterData(uint32_t address, void *data, uint32_t data_size);

    /**
     * @brief Write the register data
     *
     * @param address   The address of the register
     * @param data      The buffer to store the register data
     * @param data_size The size of the data (in bytes)
     *
     * @return true if success, otherwise false
     */
    bool writeRegisterData(uint32_t address, const void *data, uint32_t data_size);

    /**
     * @brief Write the color data
     *
     * @param address   The address of the register
     * @param data      The buffer to store the color data
     * @param data_size The size of the data (in bytes)
     *
     * @return true if success, otherwise false
     */
    bool writeColorData(uint32_t address, const void *color, uint32_t color_size);

    const Attributes &getAttributes(void)
    {
        return _attributes;
    }

    /**
     * @brief Get the IO handle
     *
     * @return
     *      - NULL:   if fail
     *      - Others: the handle of bus
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

    [[deprecated("Deprecated and will be removed in the next major version. Please use `getAttributes().type` instead.")]]
    int getType(void)
    {
        return getAttributes().type;
    }

protected:
    bool checkIO_HandleValid(void)
    {
        return (io_handle != nullptr);
    }

    esp_lcd_panel_io_handle_t io_handle = nullptr;

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
