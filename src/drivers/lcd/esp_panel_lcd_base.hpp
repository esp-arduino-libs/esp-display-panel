/*
 * SPDX-FileCopyrightText: 2023-2025 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#pragma once

#include <variant>
#include <map>
#include <memory>
#include "soc/soc_caps.h"
#include "esp_lcd_panel_ops.h"
#include "esp_lcd_panel_vendor.h"
#include "freertos/FreeRTOS.h"
#include "freertos/semphr.h"
#include "esp_panel_utils.h"
#include "drivers/bus/esp_panel_bus_base.hpp"
#include "port/esp_panel_lcd_vendor_types.h"

namespace esp_panel::drivers {

/**
 * @brief The LCD device class
 *
 * @note  This class is a base class for all LCDs. Due to it is a virtual class, users cannot use it directly
 */
class LCD {
public:
    /**
     * Here are some default values for the LCD
     */
    static constexpr int FRAME_BUFFER_MAX_NUM = 3;

    using RefreshPanelHandle = esp_lcd_panel_handle_t;
    using FunctionDrawBitmapFinishCallback = bool (*)(void *);
    using FunctionRefreshFinishCallback = bool (*)(void *);

    /**
     * @brief The LCD basic bus specification structure
     */
    struct BasicBusSpecification {
        enum Function : uint8_t {
            FUNC_INVERT_COLOR = 0,  /*!< Invert color */
            FUNC_MIRROR_X,          /*!< Mirror X axis */
            FUNC_MIRROR_Y,          /*!< Mirror Y axis */
            FUNC_SWAP_XY,           /*!< Swap X-Y axis */
            FUNC_GAP,               /*!< Set gap */
            FUNC_DISPLAY_ON_OFF,    /*!< Display on/off */
            FUNC_MAX,               /*!< The count of functions */
        };

        void print(std::string bus_name = "") const;
        std::string getColorBitsString() const;
        bool isFunctionValid(Function func) const
        {
            return functions.test(func);
        }

        int x_coord_align = 1;  /*!< X coordinate alignment. Default is `1` */
        int y_coord_align = 1;  /*!< Y coordinate alignment. Default is `1` */
        std::vector<int> color_bits;        /*!< Supported color bits */
        std::bitset<FUNC_MAX> functions;    /*!< Supported functions */
    };

    /**
     * @brief The LCD basic bus specification map, key is the bus type
     */
    using BasicBusSpecificationMap = std::map<int, BasicBusSpecification>;

    /**
     * @brief The LCD basic attributes structure.
     */
    struct BasicAttributes {
        const char *name = "Unknown";           /*!< The LCD chip name. Default is `Unknown` */
        BasicBusSpecification basic_bus_spec;   /*!< The bus specification */
    };

    /**
     * @brief The LCD configuration structure
     */
    struct Config {
        using DeviceFullConfig = esp_lcd_panel_dev_config_t;
        using VendorFullConfig = esp_panel_lcd_vendor_config_t;

        struct DevicePartialConfig {
            int reset_gpio_num = -1;
            int rgb_ele_order = static_cast<int>(LCD_RGB_ELEMENT_ORDER_RGB);
            int bits_per_pixel = 16;
            bool flags_reset_active_high = 0;
        };
        struct VendorPartialConfig {
            const esp_panel_lcd_vendor_init_cmd_t *init_cmds = nullptr;
            int init_cmds_size = 0;
            bool flags_mirror_by_cmd = 1;
            bool flags_enable_io_multiplex = 0;
        };

        void convertPartialToFull();
        void printDeviceConfig() const;
        void printVendorConfig() const;

        const DeviceFullConfig *getDeviceFullConfig() const;

        const VendorFullConfig *getVendorFullConfig() const;

        // Device
        std::variant<DeviceFullConfig, DevicePartialConfig> device = {};
        // Vendor
        std::variant<VendorFullConfig, VendorPartialConfig> vendor = {};
    };

    /**
     * @brief The LCD transformation structure
     */
    struct Transformation {
        bool swap_xy = false;
        bool mirror_x = false;
        bool mirror_y = false;
        int gap_x = 0;
        int gap_y = 0;
    };

    /**
     * @brief The driver state enumeration
     */
    enum class State : uint8_t {
        DEINIT = 0,
        INIT,
        RESET,
        BEGIN,
    };

#if SOC_MIPI_DSI_SUPPORTED
    /**
     * @brief The DSI pattern pattern
     */
    enum class DSI_ColorBarPattern : uint8_t {
        NONE = MIPI_DSI_PATTERN_NONE,                       /*!< No pattern */
        BAR_HORIZONTAL = MIPI_DSI_PATTERN_BAR_HORIZONTAL,   /*!< Horizontal bar pattern */
        BAR_VERTICAL = MIPI_DSI_PATTERN_BAR_VERTICAL,       /*!< Vertical bar pattern */
        BER_VERTICAL = MIPI_DSI_PATTERN_BER_VERTICAL,       /*!< Vertical BER pattern */
    };

    // TODO: Remove in the next major version
    typedef DSI_ColorBarPattern DsiPatternType __attribute__((deprecated("Deprecated and will be removed in the next \
    major version. Please use `DSI_ColorBarPattern` instead")));
#endif

// *INDENT-OFF*
    /**
     * @brief Construct the LCD device with separate parameters
     *
     * @note  This function uses some default values to config the LCD device, use `config*()` functions to change them
     * @note  Vendor specific initialization commands can be different between manufacturers, should consult the LCD
     *        supplier for them and use `configVendorCommands()` to configure
     *
     * @param[in] attr        The basic attributes
     * @param[in] panel_bus   Simple pointer of panel bus
     * @param[in] color_bits  Bits per pixel (16/18/24)
     * @param[in] rst_io      Reset pin, set to -1 if not used
     */
    LCD(const BasicAttributes &attr, Bus *panel_bus, int color_bits, int rst_io):
        bus(panel_bus, [](Bus *) {}),
        _basic_attributes(attr),
        _config{
            .device = Config::DevicePartialConfig{
                .reset_gpio_num = rst_io,
                .bits_per_pixel = color_bits,
            },
        }
    {
    }

    /**
     * @brief Construct the LCD device with configuration
     *
     * @note  Vendor specific initialization commands can be different between manufacturers, should consult the LCD
     *        supplier for them
     *
     * @param[in] attr      The basic attributes
     * @param[in] panel_bus Smart pointer of panel bus
     * @param[in] config    LCD configuration
     */
    LCD(const BasicAttributes &attr, std::shared_ptr<Bus> panel_bus, const Config &config):
        bus(panel_bus),
        _basic_attributes(attr),
        _config(config)
    {
    }
// *INDENT-OFF*

    /**
     * @brief Destroy the LCD device
     */
    virtual ~LCD() = default;

    /**
     * Here are some functions to configure the LCD. These functions should be called before `init()`
     */
    /**
     * @brief Configure the vendor initialization commands. This function is invalid for the single "RGB" bus which
     *        doesn't have a control panel to transmit commands
     *
     * @note  This function should be called before `init()`
     * @note  Vendor specific initialization commands can be different between manufacturers, should consult the LCD
     *        supplier for them
     * @note  There are two formats for the sequence code:
     *          1. Raw data: {command, (uint8_t []){ data0, data1, ... }, data_size, delay_ms}
     *          2. Formatter: ESP_PANEL_LCD_CMD_WITH_8BIT_PARAM(delay_ms, command, { data0, data1, ... }) and
     *                        ESP_PANEL_LCD_CMD_WITH_NONE_PARAM(delay_ms, command)
     *
     * @param[in] init_cmd      The initialization commands
     * @param[in] init_cmd_size The size of the initialization commands in bytes
     *
     * @return true if success, otherwise false
     */
    bool configVendorCommands(const esp_panel_lcd_vendor_init_cmd_t init_cmd[], uint32_t init_cmd_size);

    /**
     * @brief Configure driver to mirror by command, default is false (by software). This function is only valid for the
     *        "3-wire SPI + RGB" bus.
     *
     * @note  This function should be called before `init()`
     * @note  After using this function, the `mirror()` function will be implemented by LCD command. Otherwise, the
     *        `mirror()`function will be implemented by software
     * @note  This function is conflict with `configAutoReleaseBus()`, please don't use them at the same time
     *
     * @param[in] en true: enable, false: disable
     *
     * @return true if success, otherwise false
     */
    bool configMirrorByCommand(bool en);

    /**
     * @brief Configure driver to enable IO multiplex function, default is false. This function is only valid for the
     *        "3-wire SPI + RGB" bus.
     *
     * @note  This function should be called before `init()` and the control panel will be deleted automatically after
     *        calling `init()` function.
     * @note  If the "3-wire SPI" interface are sharing pins of the "RGB" interface to save GPIOs, please call
     *        this function to release the bus object and pins (except CS signal). And then, the "3-wire SPI" interface
     *        cannot be used to transmit commands any more.
     * @note  This function is conflict with `configMirrorByCommand()`, please don't use them at the same time
     *
     * @param en true: enable, false: disable
     *
     * @return true if success, otherwise false
     */
    bool configEnableIO_Multiplex(bool en);

    /**
     * @brief Configure the color order of LCD to BGR, default is RGB.
     *
     * @note  This function should be called before `init()`
     *
     * @param[in] reverse_order true: BGR order, false: RGB order
     *
     * @return true if success, otherwise false
     */
    bool configColorRGB_Order(bool reverse_order);

    /**
     * @brief Configure the reset active level of LCD.
     *
     * @note  This function should be called before `init()`
     *
     * @param[in] level 0: low level, 1: high level
     *
     * @return true if success, otherwise false
     */
    bool configResetActiveLevel(int level);

    /**
     * @brief Initialize the LCD device.
     *
     * @note  This function should be called after bus is begun
     * @note  This function typically calls `esp_lcd_new_panel_*()` to create the refresh panel
     *
     * @return true if success, otherwise false
     */
    virtual bool init() = 0;

    /**
     * @brief Startup the LCD device
     *
     * @note  This function should be called after `init()`
     * @note  This function typically calls `esp_lcd_panel_init()` to initialize the refresh panel
     *
     * @return true if success, otherwise false
     */
    bool begin();

    /**
     * @brief Reset the LCD. If the RESET pin is not set, this function will do reset by software instead of hardware
     *
     * @note  This function should be called after `init()`
     * @note  This function typically calls `esp_lcd_panel_reset()` to reset the refresh panel
     *
     * @return true if success, otherwise false
     */
    virtual bool reset();

    /**
     * @brief Delete the LCD device, release the resources
     *
     * @note  This function typically calls `esp_lcd_panel_del()` to delete the refresh panel
     *
     * @return true if success, otherwise false
     */
    bool del();

    /**
     * @brief Draw the bitmap to the LCD without waiting for the drawing to finish
     *
     * @note  This function should be called after `begin()`
     * @note  This function typically calls `esp_lcd_panel_draw_bitmap()` to draw the bitmap
     * @note  This function is non-blocking, the drawing will be finished in the background. So the bitmap data should
     *        not be modified until the drawing is finished
     * @note  For bus which not use DMA operation (like RGB), this function typically uses `memcpy()` to copy the
     *        bitmap data to the frame buffer. So the bitmap data can be immediately modified
     *
     * @param[in] x_start    X coordinate of the start point, the range is [0, lcd_width - 1]
     * @param[in] y_start    Y coordinate of the start point, the range is [0, lcd_height - 1]
     * @param[in] width      Width of the bitmap, the range is [1, lcd_width]
     * @param[in] height     Height of the bitmap, the range is [1, lcd_height]
     * @param[in] color_data Pointer of the color data array
     *
     * @return true if success, otherwise false
     */
    bool drawBitmap(uint16_t x_start, uint16_t y_start, uint16_t width, uint16_t height, const uint8_t *color_data);

    /**
     * @brief Draw the bitmap to the LCD device with a timeout to wait for the drawing to finish
     *
     * @note  This function should be called after `begin()`
     * @note  This function not only calls `esp_lcd_panel_draw_bitmap()` to draw the bitmap, but also waits for the
     *        drawing to finish until the timeout is reached.
     * @note  This function is blocking and will quit after the drawing is finished. So the bitmap data can be
     *        immediately modified
     * @note  For bus which not use DMA operation when drawing (like RGB), this function is same as
     *        `drawBitmap()`
     *
     * @param[in] x_start    X coordinate of the start point, the range is [0, lcd_width - 1]
     * @param[in] y_start    Y coordinate of the start point, the range is [0, lcd_height - 1]
     * @param[in] width      Width of the bitmap, the range is [1, lcd_width]
     * @param[in] height     Height of the bitmap, the range is [1, lcd_height]
     * @param[in] color_data Pointer of the color data array
     * @param[in] timeout_ms Timeout in milliseconds, -1 means wait forever
     *
     * @return true if success, otherwise false or timeout
     */
    bool drawBitmapWaitUntilFinish(
        uint16_t x_start, uint16_t y_start, uint16_t width, uint16_t height, const uint8_t *color_data,
        int timeout_ms = -1
    );

    /**
     * @brief Mirror the X axis
     *
     * @note  This function should be called after `begin()`
     * @note  This function typically calls `esp_lcd_panel_mirror()` to mirror the axis
     *
     * @param[in] en true: enable, false: disable
     *
     * @return true if success, otherwise false
     */
    bool mirrorX(bool en);

    /**
     * @brief Mirror the Y axis
     *
     * @note  This function should be called after `begin()`
     * @note  This function typically calls `esp_lcd_panel_mirror()` to mirror the axis
     *
     * @param[in] en true: enable, false: disable
     *
     * @return true if success, otherwise false
     */
    bool mirrorY(bool en);

    /**
     * @brief Swap the X and Y axis
     *
     * @note  This function should be called after `begin()`
     * @note  This function typically calls `esp_lcd_panel_swap_xy()` to mirror the axes
     *
     * @param[in] en true: enable, false: disable
     *
     * @return true if success, otherwise false
     */
    bool swapXY(bool en);

    /**
     * @brief Set the Gap in the X axis
     *
     * @note  This function should be called after `begin()`
     * @note  This function typically calls `esp_lcd_panel_set_gap()` to set the gap
     *
     * @param[in] gap The gap in pixel
     *
     * @return true if success, otherwise false
     */
    bool setGapX(uint16_t gap);

    /**
     * @brief Set the Gap in the Y axis
     *
     * @note  This function should be called after `begin()`
     * @note  This function typically calls `esp_lcd_panel_set_gap()` to set the gap
     *
     * @param[in] gap The gap in pixel
     *
     * @return true if success, otherwise false
     */
    bool setGapY(uint16_t gap);

    /**
     * @brief Invert every bit of pixel color data, like from `0x55` to `0xAA`
     *
     * @note  This function should be called after `begin()`
     * @note  This function typically calls `esp_lcd_panel_invert_color()` to invert the color
     *
     * @param[in] en true: invert, false: restore
     *
     * @return true if success, otherwise false
     */
    bool invertColor(bool en);

    /**
     * @brief  Set the display on or off
     *
     * @note  This function should be called after `begin()`
     * @note  This function typically calls `esp_lcd_panel_disp_on_off()` to control the display
     *
     * @param[in] enable_on true: on, false: off
     *
     * @return true if success, otherwise false
     */
    bool setDisplayOnOff(bool enable_on);

    /**
     * @brief Attach a callback function, which will be called when the drawing of the bitmap is finished.
     *
     * @note  For bus which not use DMA operation when drawing (like RGB), this function will be called at the end of
     *        the `drawBitmap()` or `drawBitmapWaitUntilFinish()` function
     * @note  For other bus, the function will be called when DMA operation is finished
     *
     * @param[in] callback  The callback function. Its return value decides whether a high priority task has been waken
     *                      up by this function
     * @param[in] user_data The user data which will be passed to the callback function. This function shares the same
     *                      user data with `attachRefreshFinishCallback()`. Default is `nullptr`
     *
     * @return true if success, otherwise false
     */
    bool attachDrawBitmapFinishCallback(FunctionDrawBitmapFinishCallback callback, void *user_data = nullptr);

    /**
     * @brief Attach a callback function, which will be called when the frame buffer refreshing is finished. This
     *        function is only valid for RGB/MIPI-DSI bus which maintains the frame buffer (GRAM).
     *
     * @param[in] callback  The callback function, its return value decides whether a high priority task has been waken
     *                      up by this function. There are two situations that the callback function should be placed in
     *                      IRAM (add `IRAM_ATTR` before the function): 1. For MIPI-DSI bus, when
     *                      `CONFIG_LCD_DSI_ISR_IRAM_SAFE` is set. 2. For RGB bus, when `CONFIG_LCD_RGB_ISR_IRAM_SAFE`
     *                      is set, if the "XIP on PSRAM" is not enabled.
     * @param[in] user_data The user data which will be passed to the callback function. This function shares the same
     *                      user data with `attachDrawBitmapFinishCallback()`. Default is `nullptr`
     *
     * @return true if success, otherwise false
     */
    bool attachRefreshFinishCallback(FunctionRefreshFinishCallback callback, void *user_data = nullptr);

    /**
     * @brief Draw color bar from top left to bottom right, the order is "B-G-R". This function is used for testing.
     *
     * @note  This function should be called after `begin()`
     * @note  Every bar indicates 1 bit of color. For example, if the `bits_per_pixel` is 16, there will be 16 bars.
     * @note  If the `height` can't be divided by `bits_per_pixel`, the reset area will be filled with white color.
     *
     * @param width  The width of the color bar, typically is the LCD width
     * @param height The height of the color bar, typically is the LCD height
     *
     * @return true if success, otherwise false
     */
    bool colorBarTest(uint16_t width, uint16_t height);

#if SOC_MIPI_DSI_SUPPORTED
    /**
     * @brief Show the DSI color bar pattern. This function is used for testing MIPI-DSI bus.
     *
     * @note  This function should be called after `begin()`
     *
     * @param[in] pattern The DSI color bar pattern pattern
     *
     * @return true if success, otherwise false
     */
    bool DSI_ColorBarPatternTest(DSI_ColorBarPattern pattern);
#endif // SOC_MIPI_DSI_SUPPORTED

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
     * @brief Check if the LCD function is supported by the device
     *
     * @return true if the function is supported, otherwise false
     */
    bool isFunctionSupported(BasicBusSpecification::Function function)
    {
        return _basic_attributes.basic_bus_spec.isFunctionValid(function);
    }

    /**
     * @brief Get the bits of frame pixel color
     *
     * @return The bits of frame pixel color, or -1 if fail
     */
    int getFrameColorBits();

    /**
     * @brief Get the frame buffer by index. This function only valid for RGB/MIPI-DSI bus which maintains the frame
     *        buffer (GRAM)
     *
     * @note  This function should be called after `begin()`
     *
     * @param index The index of the frame buffer, the range is [0, LCD::FRAME_BUFFER_MAX_NUM-1]. Default is 0
     *
     * @return The pointer of the frame buffer, or nullptr if fail
     */
    void *getFrameBufferByIndex(uint8_t index = 0);

    /**
     * @brief Get the LCD basic attributes
     *
     * @return The basic attributes
     */
    const BasicAttributes &getBasicAttributes()
    {
        return _basic_attributes;
    }

    /**
     * @brief Get the LCD transformation
     *
     * @return The transformation
     */
    const Transformation &getTransformation()
    {
        return _transformation;
    }

    /**
     * @brief Get the LCD configuration
     *
     * @return LCD configuration
     */
    const Config &getConfig()
    {
        return _config;
    }

    /**
     * @brief Get the LCD bus
     *
     * @return The pointer of the bus, or nullptr if fail
     */
    Bus *getBus()
    {
        return bus.get();
    }

    /**
     * @brief Get the LCD refresh panel handle. Users can use this handle to call low-level `esp_lcd_panel_*()`
     *        functions
     *
     * @return Refresh panel if success, otherwise nullptr
     */
    RefreshPanelHandle getRefreshPanelHandle()
    {
        return refresh_panel;
    }

    [[deprecated("Deprecated. Please use `configColorRGB_Order()` instead")]]
    bool configColorRgbOrder(bool reverse_order)
    {
        return configColorRGB_Order(reverse_order);
    }
    [[deprecated("Deprecated. Please use `configEnableIO_Multiplex()` instead")]]
    bool configAutoReleaseBus(bool en)
    {
        return configEnableIO_Multiplex(en);
    }
#if SOC_MIPI_DSI_SUPPORTED
    [[deprecated("Deprecated. Please use `DSI_ColorBarPatternTest()` instead")]]
    bool showDsiPattern(DSI_ColorBarPattern pattern)
    {
        return DSI_ColorBarPatternTest(pattern);
    }
#endif /* SOC_MIPI_DSI_SUPPORTED */
    [[deprecated("Deprecated. Please use `setDisplayOnOff(true)` instead")]]
    bool displayOn()
    {
        return setDisplayOnOff(true);
    }
    [[deprecated("Deprecated. Please use `setDisplayOnOff(false)` instead")]]
    bool displayOff()
    {
        return setDisplayOnOff(false);
    }
    [[deprecated("Deprecated. Please use `getTransformation().swap_xy` instead")]]
    bool getSwapXYFlag()
    {
        return getTransformation().swap_xy;
    }
    [[deprecated("Deprecated. Please use `getTransformation().mirror_x` instead")]]
    bool getMirrorXFlag()
    {
        return getTransformation().mirror_x;
    }
    [[deprecated("Deprecated. Please use `getTransformation().mirror_y` instead")]]
    bool getMirrorYFlag()
    {
        return getTransformation().mirror_y;
    }
    [[deprecated("Deprecated. Please use `getFrameColorBits()` instead")]]
    int getColorBits()
    {
        return getFrameColorBits();
    }
#if SOC_LCD_RGB_SUPPORTED
    [[deprecated("Deprecated. Please use `getFrameBufferByIndex()` instead")]]
    void *getRgbBufferByIndex(uint8_t index = 0)
    {
        return getFrameBufferByIndex(index);
    }
#endif // SOC_LCD_RGB_SUPPORTED
    [[deprecated("Deprecated. Please use `getBasicAttributes().basic_bus_spec.x_coord_align` instead")]]
    uint8_t getXCoordAlign()
    {
        return getBasicAttributes().basic_bus_spec.x_coord_align;
    }
    [[deprecated("Deprecated. Please use `getBasicAttributes().basic_bus_spec.y_coord_align` instead")]]
    uint8_t getYCoordAlign()
    {
        return getBasicAttributes().basic_bus_spec.y_coord_align;
    }

protected:
    /**
     * @brief Process the device on initialization.
     *
     * @note  This function must be called in the beginning of the `init()` of derived class
     *
     * @param[in] bus_specs The bus specifications provided by the derived class
     *
     * @return true if success, otherwise false
     */
    bool processDeviceOnInit(const BasicBusSpecificationMap &bus_specs);

    bool isBusValid()
    {
        return (bus != nullptr);
    }

    void setState(State state)
    {
        _state = state;
    }

    std::shared_ptr<Bus> bus = nullptr;
    RefreshPanelHandle refresh_panel = nullptr;

private:
    struct Interruption {
        struct CallbackData {
            void *lcd_ptr = nullptr;
            void *user_data = nullptr;
        };

        CallbackData data = {};
        FunctionDrawBitmapFinishCallback on_draw_bitmap_finish = nullptr;
        FunctionRefreshFinishCallback on_refresh_finish = nullptr;
        SemaphoreHandle_t draw_bitmap_finish_sem = nullptr;
        std::shared_ptr<StaticSemaphore_t> on_draw_bitmap_finish_sem_buffer = nullptr;
    };

    Config::DeviceFullConfig &getDeviceFullConfig()
    {
        if (std::holds_alternative<Config::DevicePartialConfig>(_config.device)) {
            _config.convertPartialToFull();
        }
        return std::get<Config::DeviceFullConfig>(_config.device);
    }

    Config::VendorFullConfig &getVendorFullConfig()
    {
        if (std::holds_alternative<Config::VendorPartialConfig>(_config.vendor)) {
            _config.convertPartialToFull();
        }
        return std::get<Config::VendorFullConfig>(_config.vendor);
    }

    IRAM_ATTR static bool onDrawBitmapFinish(void *panel_io, void *edata, void *user_ctx);
    IRAM_ATTR static bool onRefreshFinish(void *panel_io, void *edata, void *user_ctx);

    BasicAttributes _basic_attributes = {};
    Config _config = {};
    State _state = State::DEINIT;
    Transformation _transformation = {};
    Interruption _interruption = {};
};

} // namespace esp_panel::drivers

/**
 * @deprecated Deprecated. Please use `esp_panel::drivers::LCD
 *             instead.
 * @TODO: Remove in the next major version
 */
typedef esp_panel::drivers::LCD ESP_PanelLcd __attribute__((deprecated("Deprecated and will be removed in the next \
major version. Please use `esp_panel::drivers::LCD` instead.")));
