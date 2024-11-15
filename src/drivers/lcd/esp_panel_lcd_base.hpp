/*
 * SPDX-FileCopyrightText: 2023-2024 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#pragma once

#include <memory>
#include "soc/soc_caps.h"
#include "esp_lcd_panel_ops.h"
#include "esp_lcd_panel_vendor.h"
#include "freertos/FreeRTOS.h"
#include "freertos/semphr.h"
#include "drivers/bus/esp_panel_bus_base.hpp"
#include "port/esp_panel_lcd_vendor_types.h"

namespace esp_panel::drivers {

/**
 * @brief The LCD device class
 *
 * @note  This class is a base class for all LCDs. Due to it is a virtual class, users cannot use it directly
 *
 */
class LCD {
public:
    constexpr static int FRAME_BUFFER_MAX_NUM = 3;

#if SOC_MIPI_DSI_SUPPORTED
    enum class DSI_PatternType {
        NONE = MIPI_DSI_PATTERN_NONE,
        BAR_HORIZONTAL = MIPI_DSI_PATTERN_BAR_HORIZONTAL,
        BAR_VERTICAL = MIPI_DSI_PATTERN_BAR_VERTICAL,
        BER_VERTICAL = MIPI_DSI_PATTERN_BER_VERTICAL,
    };

    typedef DSI_PatternType DsiPatternType __attribute__((deprecated("Deprecated and will be removed in the next major version. Please use `DSI_PatternType` instead")));
#endif

    struct Config {
        esp_lcd_panel_dev_config_t getPanelConfig(void) const
        {
            return {
                .reset_gpio_num = panel_reset_gpio_num,
                .rgb_ele_order = static_cast<lcd_rgb_element_order_t>(panel_rgb_ele_order),
                .data_endian = LCD_RGB_DATA_ENDIAN_BIG,
                .bits_per_pixel = static_cast<uint32_t>(panel_bits_per_pixel),
                .flags = {
                    .reset_active_high = panel_flags_reset_active_high,
                },
                .vendor_config = nullptr,
            };
        }

        esp_panel_lcd_vendor_config_t getVendorConfig(void) const
        {
            return {
                .init_cmds = vendor_init_cmds,
                .init_cmds_size = static_cast<unsigned int>(vendor_init_cmds_size),
                .flags = {
                    .mirror_by_cmd = vendor_flags_mirror_by_cmd,
                    .enable_io_multiplex = vendor_flags_enable_io_multiplex,
                },
            };
        }

        static Config create(uint8_t color_bits, int rst_io)
        {
            return {
                .panel_reset_gpio_num = rst_io,
                .panel_bits_per_pixel = color_bits,
            };
        }

        // General
        int panel_reset_gpio_num = -1;
        int panel_rgb_ele_order = static_cast<int>(LCD_RGB_ELEMENT_ORDER_RGB);
        int panel_bits_per_pixel = 16;
        bool panel_flags_reset_active_high = 0;
        // Vendor
        const esp_panel_lcd_vendor_init_cmd_t *vendor_init_cmds = nullptr;
        int vendor_init_cmds_size = 0;
        bool vendor_flags_mirror_by_cmd = 1;
        bool vendor_flags_enable_io_multiplex = 0;
    };

    struct Attributes {
        bool disable_mirror = 0;
        bool disable_swap_xy = 0;
        bool disable_gap = 0;
        bool disable_display_on_off = 0;
        uint8_t x_coord_align = 1;
        uint8_t y_coord_align = 1;
        std::string name = "Unknown";
    };

    struct State {
        bool swap_xy = 0;
        bool mirror_x = 0;
        bool mirror_y = 0;
        uint16_t gap_x = 0;
        uint16_t gap_y = 0;
    };

    using DrawBitmapFinishCallback = bool (*)(void *);
    using RefreshFinishCallback = bool (*)(void *);

    /**
     * @brief Construct a new LCD device in a simple way, the `init()` function should be called after this function
     *
     * @note  This function uses some default values to config the LCD device, please use `config*()` functions to
     *        change them
     * @note  Vendor specific initialization can be different between manufacturers, should consult the LCD supplier
     *        for initialization sequence code, and use `configVendorCommands()` to configure
     *
     * @param lcd_bus     Pointer of LCD bus
     * @param color_bits  Bits per pixel
     * @param rst_io      Reset pin
     */
    LCD(Bus *lcd_bus, int color_bits, int rst_io, Attributes attr):
        bus(lcd_bus, [](Bus *) {}),
        _attributes(attr)
    {
        auto config = Config::create(color_bits, rst_io);
        panel_config = config.getPanelConfig();
        vendor_config = config.getVendorConfig();
    }

    /**
     * @brief Construct a new LCD device in a complex way, the `init()` function should be called after this function
     *
     * @param lcd_bus Pointer of LCD bus
     * @param config  LCD configuration
     */
    LCD(std::shared_ptr<Bus> lcd_bus, const Config &config, Attributes attr):
        bus(lcd_bus),
        _attributes(attr)
    {
        panel_config = config.getPanelConfig();
        vendor_config = config.getVendorConfig();
        // Bind the vendor configuration
        panel_config.vendor_config = &vendor_config;
    }

    /**
     * @brief Destroy the LCD device
     *
     */
    virtual ~LCD() = default;

    /**
     * @brief Configure the vendor initialization commands. This function should be called before `init()`
     *
     * @note  Vendor specific initialization can be different between manufacturers, should consult the LCD supplier
     *        for initialization sequence code.
     * @note  There are two formats for the sequence code:
     *          1. Raw data: {command, (uint8_t []){ data0, data1, ... }, data_size, delay_ms}
     *          2. Formatter: ESP_PANEL_LCD_CMD_WITH_8BIT_PARAM(delay_ms, command, { data0, data1, ... }) and
     *                       ESP_PANEL_LCD_CMD_WITH_NONE_PARAM(delay_ms, command)
     *
     * @return true if success, otherwise false
     */
    bool configVendorCommands(const esp_panel_lcd_vendor_init_cmd_t init_cmd[], uint32_t init_cmd_size);

    /**
     * @brief Configure the color order of LCD to BGR, default is RGB. This function should be called before `init()`
     *
     * @param reverse_order true: BGR order, false: RGB order
     *
     */
    bool configColorRgbOrder(bool reverse_order);

    /**
     * @brief Configure the reset active level of LCD, default is RGB. This function should be called before `init()`
     *
     * @param level 0: low level, 1: high level
     *
     */
    bool configResetActiveLevel(int level);

    /**
     * @brief Configure driver to mirror by command, default is false (by software). This function should be called before
     *        `init()`. Only valid for RGB interface.
     *
     * @note  After using this function, the `mirror()` function will be implemented by LCD command. Otherwise, the
     *        `mirror()`function will be implemented by software
     * @note  This function is conflict with `configAutoReleaseBus()`, please don't use them at the same time
     *
     * @param en true: enable, false: disable
     *
     */
    bool configMirrorByCommand(bool en);

    /**
     * @brief Configure driver to release bus automatically, default is false. This function should be called before
     *        `init()`. Only valid for RGB interface.
     *
     * @note  If the "3-wire SPI" interface are sharing pins of the "RGB" interface to save GPIOs, please call
     *        this function to release the bus object and pins (except CS signal). And then, the "3-wire SPI" interface
     *        cannot be used to transmit commands any more.
     * @note  This function is conflict with `configMirrorByCommand()`, please don't use them at the same time
     *
     * @param en true: enable, false: disable
     *
     * @return true if success, otherwise false
     */
    [[deprecated("Deprecated and will be removed in the next major version. Please use `configEnableIO_Multiplex()` instead")]]
    bool configAutoReleaseBus(bool en)
    {
        return configEnableIO_Multiplex(en);
    }

    /**
     * @brief Configure driver to enable IO multiplex function, default is false. This function should be called before
     *        `init()` and the panel IO will be deleted automatically after calling `init()` function. Only valid for
     *        RGB interface.
     *
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
     * @brief Initialize the LCD device, the `begin()` function should be called after this function
     *
     * @note  This function typically calls `esp_lcd_new_panel_*()` to create the LCD panel panel_handle
     *
     * @return true if success, otherwise false
     */
    virtual bool init(void) = 0;

    /**
     * @brief Startup the LCD device
     *
     * @note  This function should be called after `init()`
     * @note  This function typically calls `esp_lcd_panel_init()` to initialize the LCD device
     *
     * @return true if success, otherwise false
     */
    bool begin(void);

    /**
     * @brief Reset the LCD. If the `rst_io` is not set, this function will do reset by software instead of hardware
     *
     * @note  This function should be called after `init()`
     * @note  This function typically calls `esp_lcd_panel_reset()` to reset the LCD device
     *
     * @return true if success, otherwise false
     */
    virtual bool reset(void);

    /**
     * @brief Delete the LCD device, release the resources
     *
     * @note  This function should be called after `init()`
     * @note  This function typically calls `esp_lcd_panel_del()` to delete the LCD device
     *
     * @return true if success, otherwise false
     */
    bool del(void);

    /**
     * @brief Draw the bitmap to the LCD without waiting for the drawing to finish
     *
     * @note  This function should be called after `begin()`
     * @note  This function typically calls `esp_lcd_panel_draw_bitmap()` to draw the bitmap
     * @note  This function is non-blocking, the drawing will be finished in the background. So the bitmap data should
     *        not be modified until the drawing is finished
     * @note  For RGB interface, this function is same as `drawBitmapWaitUntilFinish()`
     *
     * @param x_start    X coordinate of the start point, the range is [0, lcd_width - 1]
     * @param y_start    Y coordinate of the start point, the range is [0, lcd_height - 1]
     * @param width      Width of the bitmap, the range is [1, lcd_width]
     * @param height     Height of the bitmap, the range is [1, lcd_height]
     * @param color_data Pointer of the color data array
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
     * @note  For RGB interface, this function is same as `drawBitmap()`
     *
     * @param x_start    X coordinate of the start point, the range is [0, lcd_width - 1]
     * @param y_start    Y coordinate of the start point, the range is [0, lcd_height - 1]
     * @param width      Width of the bitmap, the range is [1, lcd_width]
     * @param height     Height of the bitmap, the range is [1, lcd_height]
     * @param color_data Pointer of the color data array
     * @param timeout_ms Timeout in milliseconds, -1 means wait forever
     *
     * @return true if success, otherwise false or timeout
     */
    bool drawBitmapWaitUntilFinish(uint16_t x_start, uint16_t y_start, uint16_t width, uint16_t height,
                                   const uint8_t *color_data, int timeout_ms = -1);

    /**
     * @brief Mirror the X axis
     *
     * @note  This function should be called after `begin()`
     * @note  This function typically calls `esp_lcd_panel_mirror()` to mirror the axis
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
     * @note  This function typically calls `esp_lcd_panel_mirror()` to mirror the axis
     *
     * @param en true: enable, false: disable
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
     * @param en true: enable, false: disable
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
     * @param gap The gap in pixel
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
     * @param gap The gap in pixel
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
     * @param en true: invert, false: restore
     *
     * @return true if success, otherwise false
     */
    bool invertColor(bool en);

    /**
     * @brief  Set the display on or off
     *
     * @param enable_on true: display on, false: display off
     *
     * @note  This function should be called after `begin()`
     * @note  This function typically calls `esp_lcd_panel_disp_on_off()` to control the display
     *
     * @return true if success, otherwise false
     */
    bool setDisplayOnOff(bool enable_on);

    [[deprecated("Deprecated and will be removed in the next major version. Please use `setDisplayOnOff(true)` instead")]]
    bool displayOn(void)
    {
        return setDisplayOnOff(true);
    }

    [[deprecated("Deprecated and will be removed in the next major version. Please use `setDisplayOnOff(false)` instead")]]
    bool displayOff(void)
    {
        return setDisplayOnOff(false);
    }

    /**
     * @brief Attach a callback function, which will be called when the refreshing is finished
     *
     * @note  For RGB LCD, the function will be called when VSYNC end signal is detected, which means
     *        the whole frame refreshing is finished
     * @note  For other LCDs, the function will be called when every single drawing is finished
     *
     * @param callback  The callback function. Its return value decides whether a high priority task has been waken up
     *                  by this function
     * @param user_data The user data which will be passed to the callback function. This function shares the same user
     *                  data with `attachRefreshFinishCallback()`.
     */
    bool attachDrawBitmapFinishCallback(DrawBitmapFinishCallback callback, void *user_data = nullptr);

    /**
     * @brief Attach a callback function, which will be called when the frame buffer refreshing is finished
     *
     * @note  For RGB LCD, the function will be called when VSYNC end signal is detected, which means
     *        the whole frame refreshing is finished
     * @note  For other LCDs, the function will be called when every single drawing is finished
     *
     * @param callback  The callback function. Its return value decides whether a high priority task has been waken up
     *                  by this function
     * @param user_data The user data which will be passed to the callback function. This function shares the same user
     *                  data with `attachDrawBitmapFinishCallback()`.
     */
    bool attachRefreshFinishCallback(RefreshFinishCallback callback, void *user_data = nullptr);

    /**
     * @brief Draw color bar from top left to bottom right, the order is BGR. This function is used for testing.
     *
     * @note  Every bar indicate 1 bit. For example, if the `bits_per_pixel` is `16`, there will be `16` bars.
     * @note  If the `height` can't be divided by `bits_per_pixel`, the reset area will be filled with white color.
     *
     * @param width  The width of the color bar
     * @param height The height of the color bar
     *
     * @return true if success, otherwise false
     */
    bool colorBarTest(uint16_t width, uint16_t height);

#if SOC_MIPI_DSI_SUPPORTED
    bool showDSI_Pattern(DSI_PatternType type);

    [[deprecated("Deprecated and will be removed in the next major version. Please use `showDSI_Pattern()` instead")]]
    bool showDsiPattern(DSI_PatternType type)
    {
        return showDSI_Pattern(type);
    }
#endif /* SOC_MIPI_DSI_SUPPORTED */

    /**
     * @brief Get the bits of pixel color
     *
     * @return
     *      - -1:     if fail
     *      - others: the bits of pixel color
     */
    int getColorBits(void);

    /**
     * @brief Get the frame buffer by index (default is 0), currently only valid for RGB/MIPI-DSI LCD
     *
     * @note  This function should be called after `begin()`
     *
     * @param index
     *
     * @return
     *      - nullptr:   if fail
     *      - others: the pointer of the frame buffer
     */
    void *getFrameBufferByIndex(uint8_t index = 0);

    const State &getState(void)
    {
        return _state;
    }

    const Attributes &getAttributes(void)
    {
        return _attributes;
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

#if SOC_LCD_RGB_SUPPORTED
    [[deprecated("This API is deprecated. Please use `getFrameBufferByIndex()` instead.")]]
    void *getRgbBufferByIndex(uint8_t index = 0)
    {
        return getFrameBufferByIndex(index);
    }
#endif // SOC_LCD_RGB_SUPPORTED

    /**
     * @deprecated Deprecated and will be removed in the next major version. Please use `getAttributes().x_coord_align`
     *             instead.
     *
     * @TODO: Remove in the next major version
     *
     */
    [[deprecated("Deprecated and will be removed in the next major version. Please use `getAttributes().x_coord_align` instead")]]
    uint8_t getXCoordAlign(void)
    {
        return getAttributes().x_coord_align;
    }

    /**
     * @deprecated Deprecated and will be removed in the next major version. Please use `getAttributes().y_coord_align`
     *             instead.
     *
     * @TODO: Remove in the next major version
     *
     */
    [[deprecated("Deprecated and will be removed in the next major version. Please use `getAttributes().y_coord_align` instead")]]
    uint8_t getYCoordAlign(void)
    {
        return getAttributes().y_coord_align;
    }

    /**
     * @brief Get the panel panel_handle
     *
     * @return The panel_handle of the LCD panel, or nullptr if fail
     */
    esp_lcd_panel_handle_t getHandle(void)
    {
        return panel_handle;
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

protected:
    bool loadVendorConfigFromBus(void);

    bool checkBusIsValid(void)
    {
        return (bus != nullptr);
    }

    bool checkIsInit(void)
    {
        return (panel_handle != nullptr);
    }

    /**
     * @brief Check if the device is already begun
     *
     * @return true if the device is already begun, otherwise false
     *
     */
    bool checkIsBegun(void)
    {
        return _flags.is_begun;
    }

    std::shared_ptr<Bus> bus = nullptr;
    esp_lcd_panel_dev_config_t panel_config = {};
    esp_panel_lcd_vendor_config_t vendor_config = {};
    esp_lcd_panel_handle_t panel_handle = nullptr;

private:
    using CallbackData = struct {
        void *lcd_ptr;
        void *user_data;
    };

    IRAM_ATTR static bool onDrawBitmapFinish(void *panel_io, void *edata, void *user_ctx);
    IRAM_ATTR static bool onRefreshFinish(void *panel_io, void *edata, void *user_ctx);

    struct {
        uint8_t is_begun: 1;
        uint8_t is_reset: 1;
    } _flags = {};
    struct {
        CallbackData data;
        DrawBitmapFinishCallback on_draw_bitmap_finish;
        RefreshFinishCallback on_refresh_finish;
        SemaphoreHandle_t draw_bitmap_finish_sem;
    } _callback = {};
    State _state = {};
    Attributes _attributes = {};
};

} // namespace esp_panel::drivers

/**
 * @deprecated Deprecated and will be removed in the next major version. Please use `esp_panel::drivers::LCD` instead.
 *
 * @TODO: Remove in the next major version
 *
 */
typedef esp_panel::drivers::LCD ESP_PanelLcd __attribute__((deprecated("Deprecated and will be removed in the next major version. Please use `esp_panel::drivers::LCD` instead.")));
