/*
 * SPDX-FileCopyrightText: 2023-2024 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#pragma once

#include <memory>
#include "freertos/FreeRTOS.h"
#include "freertos/semphr.h"
#include "drivers/bus/esp_panel_bus_base.hpp"
#include "port/esp_lcd_touch.h"

namespace esp_panel::drivers {

/**
 * @brief The touch device objdec class
 *
 * @note  This class is a base class for all Touchs. Due to it is a virtual class, users cannot use it directly
 */
class Touch {
public:
    /**
     * @brief The class used for storing touch points
     *
     */
    class Point {
    public:
        Point() = default;
        Point(uint16_t x, uint16_t y, uint16_t strength): x(x), y(y), strength(strength) {}

        bool operator==(Point p)
        {
            return ((p.x == x) && (p.y == y));
        }
        bool operator!=(Point p)
        {
            return ((p.x != x) || (p.y != y));
        }

        uint16_t x = 0;
        uint16_t y = 0;
        uint16_t strength = 0;
    };

    struct Config {
        esp_lcd_touch_config_t getTouchConfig(void) const
        {
            return {
                .x_max = static_cast<uint16_t>(x_max),
                .y_max = static_cast<uint16_t>(y_max),
                .rst_gpio_num = static_cast<gpio_num_t>(rst_gpio_num),
                .int_gpio_num = static_cast<gpio_num_t>(int_gpio_num),
                .levels = {
                    .reset = static_cast<unsigned int>(levels_reset),
                    .interrupt = static_cast<unsigned int>(levels_interrupt),
                },
                .flags = {
                    .swap_xy = 0,
                    .mirror_x = 0,
                    .mirror_y = 0,
                },
                .process_coordinates = nullptr,
                .interrupt_callback = nullptr,
                .user_data = nullptr,
                .driver_data = nullptr,
            };
        }

        static Config create(uint16_t width, uint16_t height, int rst_io, int int_io)
        {
            return {
                .x_max = width,
                .y_max = height,
                .rst_gpio_num = rst_io,
                .int_gpio_num = int_io,
            };
        }

        int x_max = 0;
        int y_max = 0;
        int rst_gpio_num = -1;
        int int_gpio_num = -1;
        int levels_reset = 0;
        int levels_interrupt = 0;
    };

    struct Attributes {
        std::string name = "Unknown";
    };

    struct State {
        bool swap_xy = false;
        bool mirror_x = false;
        bool mirror_y = false;
    };

    using InterruptTriggerCallback = bool (*)(void *);

    /**
     * @brief Construct a new touch device
     *
     * @param bus    Pointer to panel bus
     * @param width  The width of the touch screen
     * @param height The height of the touch screen
     * @param rst_io The reset pin of the touch screen, set to `-1` if not used
     * @param int_io The interrupt pin of the touch screen, set to `-1` if not used
     * @param attr   The attributes of the touch device
     *
     */
    Touch(Bus *touch_bus, uint16_t width, uint16_t height, int rst_io, int int_io, const Attributes &attr):
        bus(touch_bus, [](Bus *) {}),
        touch_config(Config::create(width, height, rst_io, int_io).getTouchConfig()),
        _attributes(attr)
    {
    }

    Touch(std::shared_ptr<Bus> touch_bus, const Config &config, const Attributes &attr):
        bus(touch_bus),
        touch_config(config.getTouchConfig()),
        _attributes(attr)
    {
    }

    /**
     * @brief Destroy the touch device
     *
     */
    virtual ~Touch() = default;

    /**
     * @brief Configure the levels of the reset and interrupt signals
     *
     * @param reset_level     The level of the reset signal
     * @param interrupt_level The level of the interrupt signal
     */
    void configLevels(int reset_level, int interrupt_level);

    [[deprecated("Deprecated and will be removed in the next major version. Please use `configLevels()` instead")]]
    void configResetActiveLevel(uint8_t level)
    {
        touch_config.levels.reset = level;
    }

    [[deprecated("Deprecated and will be removed in the next major version. Please use `configLevels()` instead")]]
    void configInterruptActiveLevel(uint8_t level)
    {
        touch_config.levels.interrupt = level;
    }

    /**
     * @brief Initialize the touch device, the `begin()` function should be called after this function.
     *
     * @note  This function intends to create a semaphore for the interrupt if the interrupt pin is set and the
     *        `interrupt_callback` is not used by the users. If successful, the driver will enable the functionality of
     *        blocking reads.
     *
     * @return true if success, otherwise false
     */
    bool init(void);

    /**
     * @brief Startup the touch device
     *
     * @return true if success, otherwise false
     */
    virtual bool begin(void) = 0;

    /**
     * @brief Delete the touch device, release the resources
     *
     * @note  This function should be called after `begin()`
     * @note  This function typically calls `esp_lcd_touch_del()` to delete the touch device
     *
     * @return true if success, otherwise false
     */
    bool del(void);

    /**
     * @brief Attach a callback function, which will be called when the refreshing is finished
     *
     * @param callback  The callback function
     * @param user_data The user data which will be passed to the callback function
     *
     * @return true if success, otherwise false
     */
    bool attachInterruptCallback(InterruptTriggerCallback callback, void *user_data = nullptr);

    /**
     * @brief Swap the X and Y axis
     *
     * @note  This function should be called after `begin()`
     * @note  This function typically calls `esp_lcd_touch_set_swap_xy()` to mirror the axes
     *
     * @param en true: enable, false: disable
     *
     * @return true if success, otherwise false
     */
    bool swapXY(bool en);

    /**
     * @brief Mirror the X axis
     *
     * @note  This function should be called after `begin()`
     * @note  This function typically calls `esp_lcd_touch_set_mirror_x()` to mirror the axis
     *
     * @param en true: enable, false: disable
     *
     * @return true if success, otherwise false
     */
    bool mirrorX(bool en);

    /**
     * @brief Mirror the Y axis
     *
     * @note  This function should be called after `begin()`
     * @note  This function typically calls `esp_lcd_touch_set_mirror_y()` to mirror the axis
     *
     * @param en true: enable, false: disable
     *
     * @return true if success, otherwise false
     */
    bool mirrorY(bool en);

    /**
     * @brief Read the raw data from the touch device, then users should call `getPoints()` to get the points, or call
     *        `getButtonState()` to get the button state
     *
     * @note  This function should be called after `begin()`
     * @note  This function typically calls `esp_lcd_touch_read_data()` and `esp_lcd_touch_get_coordinates()` to read
     *        the raw data
     * @note  If the interrupt pin is set, this function will be blocked until either the interrupt occurs or a timeout
     *        is triggered
     *
     * @param max_points_num The max number of the points to read
     * @param timeout_ms     The timeout of waiting for the interrupt, it is only used when the interrupt pin is set. Set to
     *                       `-1` if waiting forever
     *
     * @return true if success, otherwise false
     */
    bool readRawData(uint8_t max_points_num = CONFIG_ESP_LCD_TOUCH_MAX_POINTS, int timeout_ms = 0);

    /**
     * @brief Get the touch points. This function should be called immediately after the `readRawData()` function
     *
     * @note  This function should be called after `begin()`
     *
     * @param points The buffer to store the points
     * @param num    The number of the points to read
     *
     * @return The number of the points read, `-1` if failed
     */
    int getPoints(Point points[], uint8_t num = 1);

    /**
     * @brief Get the button state. This function should be called immediately after the `readRawData()` function
     *
     * @note  This function should be called after `begin()`
     *
     * @param index The index of the button
     *
     * @return The state of the button, `-1` if failed
     */
    int getButtonState(uint8_t index = 0);

    /**
     * @brief Read the points from the touch device. This function is a combination of `readRawData()` and `getPoints()`
     *
     * @note  This function should be called after `begin()`
     * @note  If the interrupt pin is set, this function will be blocked until either the interrupt occurs or a timeout
     *        is triggered
     *
     * @param points     The buffer to store the points
     * @param num        The number of the points to read
     * @param timeout_ms The timeout of waiting for the interrupt, it is only used when the interrupt pin is set. Set to
     *                   `-1` if waiting forever
     *
     * @return The number of the points read, `-1` if failed
     */
    int readPoints(Point points[], uint8_t num = 1, int timeout_ms = 0);

    /**
     * @brief Read the button state from the touch device. This function is a combination of `readRawData()` and
     *        `getButtonState()`
     *
     * @note  This function should be called after `begin()`
     * @note  If the interrupt pin is set, this function will be blocked until either the interrupt occurs or a timeout
     *        is triggered
     *
     * @param index      The index of the button
     * @param timeout_ms The timeout of waiting for the interrupt, it is only used when the interrupt pin is set. Set to
     *                   `-1` if waiting forever
     *
     * @return The state of the button, `-1` if failed
     */
    int readButtonState(uint8_t index = 0, int timeout_ms = 0);

    /**
     * @brief Check if the interrupt function is enabled
     *
     * @return true if enabled, otherwise false
     */
    bool checkIsInterruptEnabled(void)
    {
        return (touch_config.interrupt_callback == onInterruptTrigger);
    }

    const State &getState(void)
    {
        return _state;
    }

    const Attributes &getAttributes(void)
    {
        return _attributes;
    }

    /**
     * @brief Get the panel touch_handle of LCD device
     *
     * @return The touch_handle of the LCD panel, or nullptr if fail
     */
    esp_lcd_touch_handle_t getHandle(void)
    {
        return touch_handle;
    }

    /**
     * @brief Get the panel bus
     *
     * @return The pointer of the LCD Bus, or nullptr if fail
     */
    Bus *getBus(void)
    {
        return bus.get();
    }

    [[deprecated("Deprecated and will be removed in the next major version. Please use `checkIsInterruptEnabled()` instead")]]
    bool isInterruptEnabled(void)
    {
        return checkIsInterruptEnabled();
    }

    [[deprecated("Deprecated and will be removed in the next major version. Please use `getStateSwapXY()` instead")]]
    bool getSwapXYFlag(void)
    {
        return getState().swap_xy;
    }

    [[deprecated("Deprecated and will be removed in the next major version. Please use `getStateMirrorX()` instead")]]
    bool getMirrorXFlag(void)
    {
        return getState().mirror_x;
    }

    [[deprecated("Deprecated and will be removed in the next major version. Please use `getStateMirrorY()` instead")]]
    bool getMirrorYFlag(void)
    {
        return getState().mirror_y;
    }

protected:
    bool checkIsInit(void)
    {
        return (_flags.is_init);
    }

    /**
     * @brief Check if the device is already begun
     *
     * @return true if the device is already begun, otherwise false
     *
     */
    bool checkIsBegun(void)
    {
        return (touch_handle != nullptr);
    }

    std::shared_ptr<Bus> bus = nullptr;
    esp_lcd_touch_config_t touch_config = {};
    esp_lcd_touch_handle_t touch_handle = nullptr;

private:
    struct CallbackData {
        void *touch_ptr;
        void *user_data;
    };

    static void onInterruptTrigger(esp_lcd_touch_handle_t tp);

    struct {
        uint8_t is_init: 1;
    } _flags = {};
    State _state = {};
    Attributes _attributes = {};
    uint8_t _points_num = 0;
    uint8_t _buttons_state[CONFIG_ESP_LCD_TOUCH_MAX_BUTTONS] = {};
    Point _points[CONFIG_ESP_LCD_TOUCH_MAX_POINTS] = {};
    struct {
        CallbackData data;
        InterruptTriggerCallback on_interrupt_trigger_callback;
        SemaphoreHandle_t on_interrupt_trigger_sem;
    } _callback = {};
};

} // namespace esp_panel::drivers

/**
 * @deprecated This type is deprecated and will be removed in the next major version. Please use `esp_panel::drivers::Touch` directly.
 *
 */
typedef esp_panel::drivers::Touch ESP_PanelTouch __attribute__((deprecated("Deprecated and will be removed in the next major version. Please use `esp_panel::drivers::Touch` instead.")));;

/**
 * @deprecated This type is deprecated and will be removed in the next major version. Please use `esp_panel::drivers::Touch::Point` directly.
 *
 */
typedef esp_panel::drivers::Touch::Point ESP_PanelTouchPoint __attribute__((deprecated("Deprecated and will be removed in the next major version. Please use `esp_panel::drivers::Touch::Point` instead.")));
