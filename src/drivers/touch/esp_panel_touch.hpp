/*
 * SPDX-FileCopyrightText: 2023-2025 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#pragma once

#include <thread>
#include <variant>
#include <memory>
#include <shared_mutex>
#include "freertos/FreeRTOS.h"
#include "freertos/semphr.h"
#include "esp_panel_utils.h"
#include "drivers/bus/esp_panel_bus.hpp"
#include "port/esp_lcd_touch.h"

namespace esp_panel::drivers {

/**
 * @brief Touch point data structure
 *
 * Contains x/y coordinates and touch strength information for a single touch point
 */
struct TouchPoint {
    /**
     * @brief Compare if two touch points are equal
     *
     * @param[in] p Touch point to compare with
     * @return `true` if points are equal, `false` otherwise
     */
    bool operator==(TouchPoint p)
    {
        return ((p.x == x) && (p.y == y));
    }

    /**
     * @brief Compare if two touch points are not equal
     *
     * @param[in] p Touch point to compare with
     * @return `true` if points are not equal, `false` otherwise
     */
    bool operator!=(TouchPoint p)
    {
        return ((p.x != x) || (p.y != y));
    }

    /**
     * @brief Print touch point information to debug log
     */
    void print() const
    {
        ESP_UTILS_LOGI("x(%d), y(%d), strength(%d)", x, y, strength);
    }

    int x = -1;          /*!< X coordinate of touch point in pixels */
    int y = -1;          /*!< Y coordinate of touch point in pixels */
    int strength = -1;   /*!< Strength/pressure of touch point */
};

/**
 * @brief The touch button type, which is a pair of button index and state
 */
using TouchButton = std::pair<int, uint8_t>;

/**
 * @brief Base class for all touch screen devices
 *
 * This class provides the common interface and functionality for touch screen devices.
 * Implements core touch functionality including initialization, coordinate transformation,
 * point/button reading, and interrupt handling.
 *
 * @note This is an abstract base class and cannot be instantiated directly
 */
class Touch {
public:
    /**
     * @brief Default maximum values for touch points and buttons
     */
    static constexpr int POINTS_MAX_NUM = ESP_PANEL_CONF_TOUCH_MAX_POINTS;
    static constexpr int BUTTONS_MAX_NUM = ESP_PANEL_CONF_TOUCH_MAX_BUTTONS;

    /**
     * @brief Panel handle type definition
     */
    using PanelHandle = esp_lcd_touch_handle_t;

    /**
     * @brief Function pointer type for interrupt callbacks
     *
     * @param[in] user_data User provided data pointer that will be passed to the callback
     * @return `true` if a context switch is required, `false` otherwise
     */
    using FunctionInterruptCallback = bool (*)(void *user_data);

    /**
     * @brief Basic attributes for touch device configuration
     */
    struct BasicAttributes {
        const char *name = "Unknown";   /*!< Touch controller chip name */
        int max_points_num = 0;         /*!< Maximum number of touch points supported */
        int max_buttons_num = 0;        /*!< Maximum number of touch buttons supported */
    };

    /**
     * @brief Configuration structure for touch device
     */
    struct Config {

        /**
         * @brief Full configuration type alias for ESP LCD touch configuration
         */
        using DeviceFullConfig = esp_lcd_touch_config_t;

        /**
         * @brief Simplified partial configuration structure for touch device
         */
        struct DevicePartialConfig {
            int x_max = 0;              /*!< Maximum X coordinate value */
            int y_max = 0;              /*!< Maximum Y coordinate value */
            int rst_gpio_num = -1;      /*!< Reset GPIO pin number (-1 if unused) */
            int int_gpio_num = -1;      /*!< Interrupt GPIO pin number (-1 if unused) */
            int levels_reset = 0;       /*!< Reset signal active level */
            int levels_interrupt = 0;   /*!< Interrupt signal active level */
        };

        /**
         * @brief Convert partial configuration to full configuration
         *
         * @note This method converts DevicePartialConfig to DeviceFullConfig when needed
         */
        void convertPartialToFull();

        /**
         * @brief Print the current device configuration for debugging
         */
        void printDeviceConfig() const;

        /**
         * @brief Get pointer to the full device configuration
         *
         * @return Pointer to DeviceFullConfig, or nullptr if not available
         */
        const DeviceFullConfig *getDeviceFullConfig() const;

        /**
         * @brief Device configuration storage using variant to hold either full or partial config
         */
        std::variant<DeviceFullConfig, DevicePartialConfig> device = {};
    };

    /**
     * @brief Touch coordinate transformation settings
     */
    struct Transformation {
        bool swap_xy = false;    /*!< Swap X/Y coordinates */
        bool mirror_x = false;    /*!< Mirror X coordinate */
        bool mirror_y = false;    /*!< Mirror Y coordinate */
    };

    /**
     * @brief The driver state enumeration
     */
    enum class State : uint8_t {
        DEINIT = 0,
        INIT,
        BEGIN,
    };

// *INDENT-OFF*
    /**
     * @brief Construct a touch device instance with individual configuration parameters
     *
     * @param attr Basic attributes defining the touch device capabilities and features
     * @param bus Bus interface for communicating with the touch device
     * @param width Panel width in pixels
     * @param height Panel height in pixels
     * @param rst_io Reset GPIO pin number (-1 if unused)
     * @param int_io Interrupt GPIO pin number (-1 if unused)
     */
    Touch(const BasicAttributes &attr, Bus *bus, uint16_t width, uint16_t height, int rst_io, int int_io):
        _basic_attributes(attr),
        _bus(bus),
        _config{
            .device = Config::DevicePartialConfig{
                .x_max = width,
                .y_max = height,
                .rst_gpio_num = rst_io,
                .int_gpio_num = int_io,
            },
        }
    {
    }

    /**
     * @brief Construct a touch device instance with configuration
     *
     * @param[in] attr Basic attributes defining the touch device capabilities and features
     * @param[in] bus Pointer to the bus interface for communicating with the touch device
     * @param[in] config Configuration structure containing device settings and parameters
     */
    Touch(const BasicAttributes &attr, Bus *bus, const Config &config):
        _basic_attributes(attr),
        _bus(bus),
        _config(config)
    {
    }
// *INDENT-ON*

    /**
     * @brief Destroy the touch device
     */
    virtual ~Touch() = default;

    /**
     * @brief Configure reset signal active level
     *
     * @param[in] level Active level for reset signal
     */
    void configResetActiveLevel(int level);

    /**
     * @brief Configure interrupt signal active level
     *
     * @param[in] level Active level for interrupt signal
     */
    void configInterruptActiveLevel(int level);

    /**
     * @brief Initialize the touch device
     *
     * @return `true` if successful, `false` otherwise
     */
    bool init();

    /**
     * @brief Startup the touch device
     *
     * @return `true` if successful, `false` otherwise
     *
     * @note This function should be called after `init()`
     */
    virtual bool begin() = 0;

    /**
     * @brief Delete the touch device and release resources
     *
     * @return `true` if successful, `false` otherwise
     *
     * @note This function typically calls `esp_lcd_touch_del()` to delete the touch panel
     */
    bool del();

    /**
     * @brief Attach interrupt callback function
     *
     * @param[in] callback Function to be called on interrupt
     * @param[in] user_data User data to pass to callback function
     * @return `true` if successful, `false` otherwise
     *
     * @note This function should be called after `init()`
     */
    bool attachInterruptCallback(FunctionInterruptCallback callback, void *user_data = nullptr);

    /**
     * @brief Swap X and Y coordinates
     *
     * @param[in] en true to enable swapping, false to disable
     * @return `true` if successful, `false` otherwise
     *
     * @note This function should be called after `begin()`
     */
    bool swapXY(bool en);

    /**
     * @brief Mirror X axis coordinates
     *
     * @param[in] en true to enable mirroring, false to disable
     * @return `true` if successful, `false` otherwise
     *
     * @note This function should be called after `begin()`
     */
    bool mirrorX(bool en);

    /**
     * @brief Mirror Y axis coordinates
     *
     * @param[in] en true to enable mirroring, false to disable
     * @return `true` if successful, `false` otherwise
     *
     * @note This function should be called after `begin()`
     */
    bool mirrorY(bool en);

    /**
     * @brief Read raw data from touch device
     *
     * @param[in] points_num Expected number of points to read
     * @param[in] max_buttons_num Expected number of buttons to read
     * @param[in] timeout_ms Timeout in milliseconds for interrupt wait
     * @return `true` if successful, `false` otherwise
     *
     * @note This function should be called after `begin()`
     * @note If interrupt pin is set, this function blocks until interrupt occurs or timeout
     * @note Set timeout_ms to -1 for infinite wait
     */
    bool readRawData(int points_num, int max_buttons_num, int timeout_ms);

    /**
     * @brief Get touch points from raw data
     *
     * @param[out] points Buffer to store touch points
     * @param[in] num Maximum number of points to store
     * @return Number of points read if successful, -1 on failure
     *
     * @note This function should be called after `begin()`
     * @note Call this function immediately after `readRawData()`
     */
    int getPoints(TouchPoint points[], uint8_t num);

    /**
     * @brief Get touch points from raw data
     *
     * @param[out] points Vector to store touch points
     * @return `true` if successful, `false` otherwise
     *
     * @note This function should be called after `begin()`
     * @note Call this function immediately after `readRawData()`
     */
    bool getPoints(utils::vector<TouchPoint> &points);

    /**
     * @brief Get touch buttons from raw data
     *
     * @param[out] buttons Buffer to store button states
     * @param[in] num Number of buttons to read
     * @return Number of buttons read if successful, -1 on failure
     *
     * @note This function should be called after `begin()`
     * @note Call this function immediately after `readRawData()`
     */
    int getButtons(TouchButton buttons[], uint8_t num);

    /**
     * @brief Get touch buttons from raw data
     *
     * @param[out] buttons Vector to store button states
     * @return `true` if successful, `false` otherwise
     *
     * @note This function should be called after `begin()`
     * @note Call this function immediately after `readRawData()`
     */
    bool getButtons(utils::vector<TouchButton> &buttons);

    /**
     * @brief Get state of specific button from raw data
     *
     * @param[in] index Button index
     * @return Button state if successful, -1 on failure
     *
     * @note This function should be called after `begin()`
     * @note Call this function immediately after `readRawData()`
     */
    int getButtonState(int index);

    /**
     * @brief Read touch points with timeout
     *
     * @param[out] points Buffer to store touch points
     * @param[in] num Number of points to read
     * @param[in] timeout_ms Timeout in milliseconds for interrupt wait
     * @return Number of points read if successful, -1 on failure
     *
     * @note This function should be called after `begin()`
     * @note This combines `readRawData()` and `getPoints()`
     * @note Set timeout_ms to -1 for infinite wait
     */
    int readPoints(TouchPoint points[], uint8_t num, int timeout_ms);

    /**
     * @brief Read touch points with timeout
     *
     * @param[out] points Vector to store touch points
     * @param[in] timeout_ms Timeout in milliseconds for interrupt wait
     * @return `true` if successful, `false` otherwise
     *
     * @note This function should be called after `begin()`
     * @note This combines `readRawData()` and `getPoints()`
     * @note Set timeout_ms to -1 for infinite wait
     */
    bool readPoints(utils::vector<TouchPoint> &points, int timeout_ms);

    /**
     * @brief Read touch buttons with timeout
     *
     * @param[out] buttons Buffer to store button states
     * @param[in] num Number of buttons to read
     * @param[in] timeout_ms Timeout in milliseconds for interrupt wait
     * @return Number of buttons read if successful, -1 on failure
     *
     * @note This function should be called after `begin()`
     * @note This combines `readRawData()` and `getButtons()`
     * @note Set timeout_ms to -1 for infinite wait
     */
    int readButtons(TouchButton buttons[], uint8_t num, int timeout_ms);

    /**
     * @brief Read touch buttons with timeout
     *
     * @param[out] buttons Vector to store button states
     * @param[in] timeout_ms Timeout in milliseconds for interrupt wait
     * @return `true` if successful, `false` otherwise
     *
     * @note This function should be called after `begin()`
     * @note This combines `readRawData()` and `getButtons()`
     * @note Set timeout_ms to -1 for infinite wait
     */
    bool readButtons(utils::vector<TouchButton> &buttons, int timeout_ms);

    /**
     * @brief Read state of specific button with timeout
     *
     * @param[in] index Button index to read
     * @param[in] timeout_ms Timeout in milliseconds for interrupt wait
     * @return Button state if successful, -1 on failure
     *
     * @note This function should be called after `begin()`
     * @note This combines `readRawData()` and `getButtonState()`
     * @note Set timeout_ms to -1 for infinite wait
     */
    int readButtonState(uint8_t index, int timeout_ms);

    /**
     * @brief Reset touch points data
     */
    void resetPoints()
    {
        _points.clear();
    }

    /**
     * @brief Reset touch buttons data
     */
    void resetButtons()
    {
        _buttons.clear();
    }

    /**
     * @brief Check if driver has reached specified state
     *
     * @param[in] state State to check against
     * @return `true` if current state >= given state, `false` otherwise
     */
    bool isOverState(State state) const
    {
        return (_state >= state);
    }

    /**
     * @brief Check if interrupt function is enabled
     *
     * @return `true` if enabled, `false` otherwise
     */
    bool isInterruptEnabled() const
    {
        if (std::holds_alternative<Config::DeviceFullConfig>(_config.device)) {
            return (std::get<Config::DeviceFullConfig>(_config.device).int_gpio_num >= 0);
        }
        return (std::get<Config::DevicePartialConfig>(_config.device).int_gpio_num >= 0);
    }

    /**
     * @brief Get touch basic attributes
     *
     * @return Reference to basic attributes structure
     */
    const BasicAttributes &getBasicAttributes() const
    {
        return _basic_attributes;
    }

    /**
     * @brief Get touch transformation settings
     *
     * @return Reference to transformation structure
     */
    const Transformation &getTransformation() const
    {
        return _transformation;
    }

    /**
     * @brief Get touch configuration
     *
     * @return Reference to configuration structure
     */
    const Config &getConfig() const
    {
        return _config;
    }

    /**
     * @brief Get touch bus interface
     *
     * @return Pointer to bus interface, nullptr if not available
     */
    Bus *getBus()
    {
        return _bus;
    }

    /**
     * @brief Get panel handle of touch device
     *
     * @return Touch panel handle if successful, nullptr otherwise
     */
    PanelHandle getPanelHandle()
    {
        return touch_panel;
    }

    /**
     * @brief Configure the levels of the reset and interrupt signals
     *
     * @param[in] reset_level     The level of the reset signal
     * @param[in] interrupt_level The level of the interrupt signal
     * @note This function is deprecated, please use `configResetActiveLevel()` and `configInterruptActiveLevel()`
     *       instead
     */
    [[deprecated("Deprecated. Please use `configResetActiveLevel()` and `configInterruptActiveLevel()` instead")]]
    void configLevels(int reset_level, int interrupt_level)
    {
        configResetActiveLevel(reset_level);
        configInterruptActiveLevel(interrupt_level);
    }

    /**
     * @brief Read raw touch data
     *
     * @param[in] points_num Number of points to read, -1 means read all points
     * @param[in] timeout_ms Timeout in milliseconds
     * @return `true` if successful, `false` otherwise
     * @note This function is deprecated, please use `readRawData()` instead
     */
    [[deprecated("Deprecated.")]]
    bool readRawData(int points_num = -1, int timeout_ms = 0)
    {
        return readRawData(points_num, -1, timeout_ms);
    }

    /**
     * @brief Read touch points data
     *
     * @param[out] points Array to store touch points
     * @param[in] num Number of points to read
     * @return Number of points read
     * @note This function is deprecated, please use `readPoints()` instead
     */
    [[deprecated("Deprecated.")]]
    int readPoints(TouchPoint points[], uint8_t num)
    {
        return readPoints(points, num, 0);
    }

    /**
     * @brief Read touch button state
     *
     * @param[in] index Button index
     * @return Button state
     * @note This function is deprecated, please use `readButtonState()` instead
     */
    [[deprecated("Deprecated.")]]
    int readButtonState(uint8_t index = 0)
    {
        return readButtonState(index, 0);
    }

    /**
     * @brief Get swap XY flag
     *
     * @return `true` if X-Y coordinates are swapped, `false` otherwise
     * @note This function is deprecated, please use `getTransformation().swap_xy` instead
     */
    [[deprecated("Deprecated. Please use `getTransformation().swap_xy` instead")]]
    bool getSwapXYFlag() const
    {
        return getTransformation().swap_xy;
    }

    /**
     * @brief Get mirror X flag
     *
     * @return `true` if X coordinate is mirrored, `false` otherwise
     * @note This function is deprecated, please use `getTransformation().mirror_x` instead
     */
    [[deprecated("Deprecated. Please use `getTransformation().mirror_x` instead")]]
    bool getMirrorXFlag() const
    {
        return getTransformation().mirror_x;
    }

    /**
     * @brief Get mirror Y flag
     *
     * @return `true` if Y coordinate is mirrored, `false` otherwise
     * @note This function is deprecated, please use `getTransformation().mirror_y` instead
     */
    [[deprecated("Deprecated. Please use `getTransformation().mirror_y` instead")]]
    bool getMirrorYFlag() const
    {
        return getTransformation().mirror_y;
    }

    /**
     * @brief Get the panel handle
     *
     * @return The handle of the touch panel, or NULL if fail
     * @note This function is deprecated, please use `getPanelHandle()` instead
     */
    [[deprecated("Deprecated. Please use `getPanelHandle()` instead")]]
    PanelHandle getHandle()
    {
        return getPanelHandle();
    }

protected:
    /**
     * @brief Set driver state
     *
     * @param[in] state New state to set
     */
    void setState(State state)
    {
        _state = state;
    }

    /**
     * @brief Set driver specific data
     *
     * @param[in] data Driver data pointer
     */
    void setDriverData(void *data)
    {
        getDeviceFullConfig().driver_data = data;
    }

    /**
     * @brief Check if bus interface is valid
     *
     * @return `true` if valid, `false` otherwise
     */
    bool isBusValid() const
    {
        return (_bus != nullptr);
    }

    /**
     * @brief Check if touch points are enabled
     *
     * @return `true` if enabled, `false` otherwise
     */
    bool isPointsEnabled() const
    {
        return (getBasicAttributes().max_points_num > 0);
    }

    /**
     * @brief Check if touch buttons are enabled
     *
     * @return `true` if enabled, `false` otherwise
     */
    bool isButtonsEnabled() const
    {
        return (getBasicAttributes().max_buttons_num > 0);
    }

    PanelHandle touch_panel = nullptr;  /*!< Touch panel handle */

private:
    /**
     * @brief Interrupt handling structure
     */
    struct Interruption {
        /**
         * @brief Callback data structure
         */
        struct CallbackData {
            void *touch_ptr = nullptr;     /*!< Pointer to touch instance */
            void *user_data = nullptr;     /*!< User provided data */
        };

        CallbackData data = {};                                 /*!< Callback data */
        FunctionInterruptCallback on_active_callback = nullptr; /*!< Interrupt callback function */
        SemaphoreHandle_t on_active_sem = nullptr;              /*!< Semaphore for interrupt sync */
        StaticSemaphore_t on_active_sem_buffer = {};            /*!< Static buffer for semaphore */
    };

    Config::DeviceFullConfig &getDeviceFullConfig();
    bool readRawDataPoints(int points_num);
    bool readRawDataButtons(int max_buttons_num);
    static void onInterruptActive(PanelHandle handle);

    BasicAttributes _basic_attributes = {};                 /*!< Basic device attributes */
    Bus *_bus = nullptr;                                    /*!< Bus interface pointer */
    Config _config = {};                                    /*!< Device configuration */
    State _state = State::DEINIT;                           /*!< Current driver state */
    Transformation _transformation = {};                    /*!< Coordinate transformation settings */
    std::shared_mutex _resource_mutex;                      /*!< Resource access mutex */
    utils::vector<TouchPoint> _points;                      /*!< Touch points buffer */
    utils::vector<TouchButton> _buttons;                    /*!< Touch buttons buffer */
    std::shared_ptr<Interruption> _interruption = nullptr;  /*!< Interrupt handling */
};

} // namespace esp_panel::drivers

/**
 * @brief Deprecated type alias for backward compatibility
 * @deprecated Use `esp_panel::drivers::TouchPoint` directly. This alias will be removed in the next major version.
 */
using ESP_PanelTouchPoint [[deprecated("Please use `esp_panel::drivers::TouchPoint` directly")]] =
    esp_panel::drivers::TouchPoint;

/**
 * @brief Deprecated type alias for backward compatibility
 * @deprecated Use `esp_panel::drivers::Touch` instead. This alias will be removed in the next major version.
 */
using ESP_PanelTouch [[deprecated("Please use `esp_panel::drivers::Touch` instead")]] =
    esp_panel::drivers::Touch;
