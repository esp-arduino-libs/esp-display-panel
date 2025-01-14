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
#include "drivers/bus/esp_panel_bus_factory.hpp"
#include "port/esp_panel_lcd_vendor_types.h"
#include "esp_panel_lcd_conf_internal.h"

namespace esp_panel::drivers {

/**
 * @brief Base class for all LCD devices
 *
 * This class provides the common interface and functionality for LCD devices.
 * Implements core LCD functionality including initialization, coordinate transformation,
 * bitmap drawing, and interrupt handling.
 *
 * @note This is an abstract base class and cannot be instantiated directly
 */
class LCD {
public:
    /**
     * @brief Default maximum number of frame buffers supported
     */
    static constexpr int FRAME_BUFFER_MAX_NUM = 3;

    /**
     * @brief Panel handle type definition for refresh operations
     */
    using RefreshPanelHandle = esp_lcd_panel_handle_t;

    /**
     * @brief Function pointer type for bitmap drawing completion callback
     *
     * @param[in] user_data User provided data pointer that will be passed to the callback
     * @return `true` if a context switch is required, `false` otherwise
     */
    using FunctionDrawBitmapFinishCallback = bool (*)(void *user_data);

    /**
     * @brief Function pointer type for refresh completion callback
     *
     * @param[in] user_data User provided data pointer that will be passed to the callback
     * @return `true` if a context switch is required, `false` otherwise
     */
    using FunctionRefreshFinishCallback = bool (*)(void *user_data);

    /**
     * @brief Basic bus specification structure for LCD devices
     */
    struct BasicBusSpecification {
        /**
         * @brief Supported LCD functions enumeration
         */
        enum Function : uint8_t {
            FUNC_INVERT_COLOR = 0,  /*!< Invert display colors */
            FUNC_MIRROR_X,          /*!< Mirror display along X axis */
            FUNC_MIRROR_Y,          /*!< Mirror display along Y axis */
            FUNC_SWAP_XY,           /*!< Swap X and Y coordinates */
            FUNC_GAP,               /*!< Set display gap */
            FUNC_DISPLAY_ON_OFF,    /*!< Control display on/off */
            FUNC_MAX,               /*!< Maximum function index */
        };

        /**
         * @brief Print bus specification information for debugging
         *
         * @param[in] bus_name Optional bus name to include in output
         */
        void print(std::string bus_name = "") const;

        /**
         * @brief Get string representation of supported color bits
         *
         * @return String describing supported color bit depths
         */
        std::string getColorBitsString() const;

        /**
         * @brief Check if a specific function is supported
         *
         * @param[in] func Function to check support for
         * @return `true` if function is supported, `false` otherwise
         */
        bool isFunctionValid(Function func) const
        {
            return functions.test(func);
        }

        int x_coord_align = 1;              /*!< Required X coordinate alignment in pixels (default: 1, must be power of 2) */
        int y_coord_align = 1;              /*!< Required Y coordinate alignment in pixels (default: 1, must be power of 2) */
        std::vector<int> color_bits;        /*!< List of supported color bit depths */
        std::bitset<FUNC_MAX> functions;    /*!< Bitmap of supported functions */
    };

    /**
     * @brief Map type for storing bus specifications by bus type
     */
    using BasicBusSpecificationMap = std::map<int, BasicBusSpecification>;

    /**
     * @brief Basic attributes structure for LCD device
     */
    struct BasicAttributes {
        const char *name = "Unknown";           /*!< LCD controller chip name */
        BasicBusSpecification basic_bus_spec;   /*!< Bus interface specifications */
    };

    /**
     * @brief Simplified device configuration structure
     */
    struct DevicePartialConfig {
        int reset_gpio_num = -1;                    /*!< Reset GPIO pin number (-1 if unused) */
        int rgb_ele_order = static_cast<int>(LCD_RGB_ELEMENT_ORDER_RGB); /*!< RGB color element order */
        int bits_per_pixel = 16;                    /*!< Color depth in bits per pixel */
        bool flags_reset_active_high = 0;           /*!< Reset signal active high flag */
    };
    using DeviceFullConfig = esp_lcd_panel_dev_config_t;

    /**
     * @brief Simplified vendor configuration structure
     */
    struct VendorPartialConfig {
        const esp_panel_lcd_vendor_init_cmd_t *init_cmds = nullptr; /*!< Vendor initialization commands */
        int init_cmds_size = 0;                     /*!< Size of initialization commands array (in bytes) */
        bool flags_mirror_by_cmd = 1;               /*!< Enable mirroring via commands */
        bool flags_enable_io_multiplex = 0;         /*!< Enable IO pin multiplexing */
    };
    using VendorFullConfig = esp_panel_lcd_vendor_config_t;

    using DeviceConfig = std::variant<DevicePartialConfig, DeviceFullConfig>;
    using VendorConfig = std::variant<VendorPartialConfig, VendorFullConfig>;

    /**
     * @brief Configuration structure for LCD device
     */
    struct Config {
        /**
         * @brief Convert partial configurations to full configurations
         */
        void convertPartialToFull();

        /**
         * @brief Print device configuration for debugging
         */
        void printDeviceConfig() const;

        /**
         * @brief Print vendor configuration for debugging
         */
        void printVendorConfig() const;

        /**
         * @brief Get pointer to full device configuration
         *
         * @return Pointer to full device configuration, nullptr if not available
         */
        const DeviceFullConfig *getDeviceFullConfig() const;

        /**
         * @brief Get pointer to full vendor configuration
         *
         * @return Pointer to full vendor configuration, nullptr if not available
         */
        const VendorFullConfig *getVendorFullConfig() const;

        DeviceConfig device = DevicePartialConfig{};     /*!< Device configuration storage */
        VendorConfig vendor = VendorPartialConfig{};     /*!< Vendor configuration storage */
    };

    /**
     * @brief LCD coordinate transformation settings
     */
    struct Transformation {
        bool swap_xy = false;    /*!< Swap X/Y coordinates when true */
        bool mirror_x = false;   /*!< Mirror X coordinate when true */
        bool mirror_y = false;   /*!< Mirror Y coordinate when true */
        int gap_x = 0;          /*!< X axis gap offset in pixels */
        int gap_y = 0;          /*!< Y axis gap offset in pixels */
    };

    /**
     * @brief Driver state enumeration
     */
    enum class State : uint8_t {
        DEINIT = 0,     /*!< Driver is not initialized */
        INIT,           /*!< Driver is initialized */
        RESET,          /*!< Driver is reset */
        BEGIN,          /*!< Driver is started and ready */
    };

#if SOC_MIPI_DSI_SUPPORTED
    /**
     * @brief DSI color bar test pattern types
     */
    enum class DSI_ColorBarPattern : uint8_t {
        NONE = MIPI_DSI_PATTERN_NONE,                       /*!< No test pattern */
        BAR_HORIZONTAL = MIPI_DSI_PATTERN_BAR_HORIZONTAL,   /*!< Horizontal color bars */
        BAR_VERTICAL = MIPI_DSI_PATTERN_BAR_VERTICAL,       /*!< Vertical color bars */
        BER_VERTICAL = MIPI_DSI_PATTERN_BER_VERTICAL,       /*!< Vertical BER pattern */
    };
#endif

// *INDENT-OFF*
    /**
     * @brief Construct the LCD device with individual parameters
     *
     * @param[in] attr Basic attributes for the LCD device
     * @param[in] bus Bus interface for communicating with the LCD device
     * @param[in] color_bits Color depth in bits per pixel (16 for RGB565, 18 for RGB666, 24 for RGB888)
     * @param[in] rst_io Reset GPIO pin number (-1 if not used)
     * @note This constructor uses default values for most configuration parameters. Use config*() functions to
     *       customize
     * @note Vendor initialization commands may vary between manufacturers. Consult LCD supplier for them and use
     *       `configVendorCommands()` to configure
     */
    LCD(const BasicAttributes &attr, Bus *bus, int color_bits, int rst_io):
        _basic_attributes(attr),
        _bus(bus),
        _config{
            .device = DevicePartialConfig{
                .reset_gpio_num = rst_io,
                .bits_per_pixel = color_bits,
            },
        }
    {
    }

    /**
     * @brief Construct the LCD device with full configuration
     *
     * @param[in] attr Basic attributes for the LCD device
     * @param[in] bus Bus interface for communicating with the LCD device
     * @param[in] config Complete LCD configuration structure
     * @note Vendor initialization commands may vary between manufacturers. Consult LCD supplier for them
     */
    LCD(const BasicAttributes &attr, Bus *bus, const Config &config):
        _basic_attributes(attr),
        _bus(bus),
        _config(config)
    {
    }
// *INDENT-OFF*

    /**
     * @brief Destroy the LCD device and free resources
     */
    virtual ~LCD() = default;

    /**
     * @brief Configure the vendor initialization commands
     *
     * @param[in] init_cmd The initialization commands
     * @param[in] init_cmd_size The size of the initialization commands in bytes
     * @return `true` if successful, `false` otherwise
     * @note This function should be called before `init()`
     * @note This function is invalid for the single "RGB" bus which doesn't have a control panel to transmit commands
     * @note Vendor specific initialization commands can be different between manufacturers, should consult the LCD
     *       supplier for them
     * @note There are two formats for the sequence code:
     *       1. Raw data: {command, (uint8_t []){ data0, data1, ... }, data_size, delay_ms}
     *       2. Formatter: ESP_PANEL_LCD_CMD_WITH_8BIT_PARAM(delay_ms, command, { data0, data1, ... }) and
     *                     ESP_PANEL_LCD_CMD_WITH_NONE_PARAM(delay_ms, command)
     */
    bool configVendorCommands(const esp_panel_lcd_vendor_init_cmd_t init_cmd[], uint32_t init_cmd_size);

    /**
     * @brief Configure driver to mirror by command
     *
     * @param[in] en true: enable, false: disable
     * @return `true` if successful, `false` otherwise
     * @note This function should be called before `init()`
     * @note After using this function, the `mirror()` function will be implemented by LCD command. Otherwise, the
     *       `mirror()`function will be implemented by software
     * @note This function is conflict with `configAutoReleaseBus()`, please don't use them at the same time
     * @note This function is only valid for the "3-wire SPI + RGB" bus
     */
    bool configMirrorByCommand(bool en);

    /**
     * @brief Configure driver to enable IO multiplex function
     *
     * @param[in] en true: enable, false: disable
     * @return `true` if successful, `false` otherwise
     * @note This function should be called before `init()`
     * @note If the "3-wire SPI" interface are sharing pins of the "RGB" interface to save GPIOs, please call this function to
     *       release the bus object and pins (except CS signal)
     * @note The control panel will be deleted automatically after calling `init()` function
     * @note This function is conflict with `configMirrorByCommand()`, please don't use them at the same time
     * @note This function is only valid for the "3-wire SPI + RGB" bus
     */
    bool configEnableIO_Multiplex(bool en);

    /**
     * @brief Configure the color order of LCD
     *
     * @param[in] reverse_order true: BGR order, false: RGB order
     * @return `true` if successful, `false` otherwise
     * @note This function should be called before `init()`
     */
    bool configColorRGB_Order(bool reverse_order);

    /**
     * @brief Configure the reset active level of LCD
     *
     * @param[in] level 0: low level, 1: high level
     * @return `true` if successful, `false` otherwise
     * @note This function should be called before `init()`
     */
    bool configResetActiveLevel(int level);

    /**
     * @brief Initialize the LCD device
     *
     * @return `true` if successful, `false` otherwise
     * @note This function should be called after bus is begun
     * @note This function typically calls `esp_lcd_new_panel_*()` to create the refresh panel
     */
    virtual bool init() = 0;

    /**
     * @brief Startup the LCD device
     *
     * @return `true` if successful, `false` otherwise
     * @note This function should be called after `init()`
     * @note This function typically calls `esp_lcd_panel_init()` to initialize the refresh panel
     */
    bool begin();

    /**
     * @brief Reset the LCD
     *
     * @return `true` if successful, `false` otherwise
     * @note This function should be called after `init()`
     * @note This function typically calls `esp_lcd_panel_reset()` to reset the refresh panel
     * @note If the RESET pin is not set, this function will do reset by software instead of hardware
     */
    virtual bool reset();

    /**
     * @brief Delete the LCD device, release the resources
     *
     * @return `true` if successful, `false` otherwise
     * @note This function typically calls `esp_lcd_panel_del()` to delete the refresh panel
     */
    bool del();

    /**
     * @brief Draw the bitmap to the LCD without waiting for the drawing to finish
     *
     * @param[in] x_start X coordinate of the start point, the range is [0, lcd_width - 1]
     * @param[in] y_start Y coordinate of the start point, the range is [0, lcd_height - 1]
     * @param[in] width Width of the bitmap, the range is [1, lcd_width]
     * @param[in] height Height of the bitmap, the range is [1, lcd_height]
     * @param[in] color_data Pointer of the color data array
     * @return `true` if successful, `false` otherwise
     * @note This function should be called after `begin()`
     * @note This function typically calls `esp_lcd_panel_draw_bitmap()` to draw the bitmap
     * @note This function is non-blocking, the drawing will be finished in the background
     * @note The bitmap data should not be modified until the drawing is finished
     * @note For bus which not use DMA operation (like RGB), this function typically uses `memcpy()` to copy the
     *       bitmap data to frame buffer. So the bitmap data can be immediately modified
     */
    bool drawBitmap(uint16_t x_start, uint16_t y_start, uint16_t width, uint16_t height, const uint8_t *color_data);

    /**
     * @brief Draw the bitmap to the LCD device with a timeout to wait for the drawing to finish
     *
     * @param[in] x_start X coordinate of the start point, range [0, lcd_width - 1]
     * @param[in] y_start Y coordinate of the start point, range [0, lcd_height - 1]
     * @param[in] width Width of the bitmap, range [1, lcd_width]
     * @param[in] height Height of the bitmap, range [1, lcd_height]
     * @param[in] color_data Pointer to the color data array
     * @param[in] timeout_ms Timeout in milliseconds, -1 means wait forever
     * @return `true` if successful, `false` otherwise or timeout
     * @note This function should be called after `begin()`
     * @note This function not only calls `esp_lcd_panel_draw_bitmap()` but also waits for the drawing to finish
     * @note This function is blocking until drawing finishes, so bitmap data can be modified after return
     * @note For bus which not use DMA operation (like RGB), this function is same as `drawBitmap()`
     */
    bool drawBitmapWaitUntilFinish(uint16_t x_start, uint16_t y_start, uint16_t width, uint16_t height,
        const uint8_t *color_data, int timeout_ms = -1);

    /**
     * @brief Mirror the X axis
     *
     * @param[in] en true to enable, false to disable
     * @return `true` if successful, `false` otherwise
     * @note This function should be called after `begin()`
     * @note This function typically calls `esp_lcd_panel_mirror()` to mirror the axis
     */
    bool mirrorX(bool en);

    /**
     * @brief Mirror the Y axis
     *
     * @param[in] en true to enable, false to disable
     * @return `true` if successful, `false` otherwise
     * @note This function should be called after `begin()`
     * @note This function typically calls `esp_lcd_panel_mirror()` to mirror the axis
     */
    bool mirrorY(bool en);

    /**
     * @brief Swap the X and Y axes
     *
     * @param[in] en true to enable, false to disable
     * @return `true` if successful, `false` otherwise
     * @note This function should be called after `begin()`
     * @note This function typically calls `esp_lcd_panel_swap_xy()` to swap the axes
     */
    bool swapXY(bool en);

    /**
     * @brief Set the gap in X axis
     *
     * @param[in] gap Gap in pixels
     * @return `true` if successful, `false` otherwise
     * @note This function should be called after `begin()`
     * @note This function typically calls `esp_lcd_panel_set_gap()` to set the gap
     */
    bool setGapX(uint16_t gap);

    /**
     * @brief Set the gap in Y axis
     *
     * @param[in] gap Gap in pixels
     * @return `true` if successful, `false` otherwise
     * @note This function should be called after `begin()`
     * @note This function typically calls `esp_lcd_panel_set_gap()` to set the gap
     */
    bool setGapY(uint16_t gap);

    /**
     * @brief Invert the color of each pixel
     *
     * @param[in] en true to invert, false to restore
     * @return `true` if successful, `false` otherwise
     * @note This function should be called after `begin()`
     * @note This function typically calls `esp_lcd_panel_invert_color()` to invert the color
     */
    bool invertColor(bool en);

    /**
     * @brief Turn the display on or off
     *
     * @param[in] enable_on true to turn on, false to turn off
     * @return `true` if successful, `false` otherwise
     * @note This function should be called after `begin()`
     * @note This function typically calls `esp_lcd_panel_disp_on_off()` to control the display
     */
    bool setDisplayOnOff(bool enable_on);

    /**
     * @brief Attach a callback function to be called when bitmap drawing finishes
     *
     * @param[in] callback Function to be called on completion
     * @param[in] user_data User data to pass to callback function
     * @return `true` if successful, `false` otherwise
     * @note For bus which not use DMA operation (like RGB), callback is called at end of draw function
     * @note For other bus types, callback is called when DMA operation completes
     */
    bool attachDrawBitmapFinishCallback(FunctionDrawBitmapFinishCallback callback, void *user_data = nullptr);

    /**
     * @brief Attach a callback function to be called when frame buffer refresh finishes
     *
     * @param[in] callback Function to be called on completion
     * @param[in] user_data User data to pass to callback function
     * @return `true` if successful, `false` otherwise
     * @note Only valid for RGB/MIPI-DSI bus which maintains frame buffer (GRAM)
     * @note Callback should be in IRAM if:
     *       1. For MIPI-DSI bus when `CONFIG_LCD_DSI_ISR_IRAM_SAFE` is set
     *       2. For RGB bus when `CONFIG_LCD_RGB_ISR_IRAM_SAFE` is set and XIP on PSRAM disabled
     */
    bool attachRefreshFinishCallback(FunctionRefreshFinishCallback callback, void *user_data = nullptr);

    /**
     * @brief Draw color bars for testing
     *
     * @param[in] width Width of color bars, typically LCD width
     * @param[in] height Height of color bars, typically LCD height
     * @return `true` if successful, `false` otherwise
     * @note This function should be called after `begin()`
     * @note Each bar represents 1 color bit. For 16-bit color depth, there will be 16 bars
     * @note If height not divisible by bits_per_pixel, remaining area filled white
     */
    bool colorBarTest(uint16_t width, uint16_t height);

#if SOC_MIPI_DSI_SUPPORTED
    /**
     * @brief Show DSI color bar test pattern
     *
     * @param[in] pattern Color bar pattern to display
     * @return `true` if successful, `false` otherwise
     * @note This function should be called after `begin()`
     * @note Used for testing MIPI-DSI bus functionality
     */
    bool DSI_ColorBarPatternTest(DSI_ColorBarPattern pattern);
#endif

    /**
     * @brief Check if driver has reached specified state
     *
     * @param[in] state State to check against
     * @return `true` if current state >= given state, `false` otherwise
     */
    bool isOverState(State state)
    {
        return (_state >= state);
    }

    /**
     * @brief Check if LCD function is supported
     *
     * @param[in] function Function to check
     * @return `true` if function is supported, `false` otherwise
     */
    bool isFunctionSupported(BasicBusSpecification::Function function)
    {
        return _basic_attributes.basic_bus_spec.isFunctionValid(function);
    }

    /**
     * @brief Get frame buffer color depth in bits
     *
     * @return Color depth in bits, or -1 if failed
     */
    int getFrameColorBits();

    /**
     * @brief Get frame buffer by index
     *
     * @param[in] index Frame buffer index, range [0, LCD::FRAME_BUFFER_MAX_NUM-1]
     * @return Frame buffer pointer, or nullptr if failed
     * @note This function should be called after `begin()`
     * @note Only valid for RGB/MIPI-DSI bus which maintains frame buffer (GRAM)
     */
    void *getFrameBufferByIndex(uint8_t index = 0);

    /**
     * @brief Get LCD basic attributes
     *
     * @return Reference to basic attributes structure
     */
    const BasicAttributes &getBasicAttributes()
    {
        return _basic_attributes;
    }

    /**
     * @brief Get LCD transformation settings
     *
     * @return Reference to transformation structure
     */
    const Transformation &getTransformation()
    {
        return _transformation;
    }

    /**
     * @brief Get LCD configuration
     *
     * @return Reference to configuration structure
     */
    const Config &getConfig()
    {
        return _config;
    }

    /**
     * @brief Get LCD bus interface
     *
     * @return Pointer to bus interface
     */
    Bus *getBus()
    {
        return _bus;
    }

    /**
     * @brief Get LCD refresh panel handle
     *
     * @return Panel handle if available, nullptr otherwise
     * @note Can be used to call low-level esp_lcd_panel_* functions directly
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
#endif

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
#endif

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

    [[deprecated("Deprecated. Please use `getRefreshPanelHandle()` instead")]]
    RefreshPanelHandle getHandle()
    {
        return getRefreshPanelHandle();
    }

protected:
    /**
     * @brief Process device initialization
     *
     * @param[in] bus_specs Bus specifications from derived class
     * @return `true` if successful, `false` otherwise
     * @note Must be called at start of derived class init()
     */
    bool processDeviceOnInit(const BasicBusSpecificationMap &bus_specs);

    /**
     * @brief Check if bus interface is valid
     *
     * @return `true` if valid, `false` otherwise
     */
    bool isBusValid()
    {
        return (_bus != nullptr);
    }

    /**
     * @brief Set driver state
     *
     * @param[in] state New state to set
     */
    void setState(State state)
    {
        _state = state;
    }

    RefreshPanelHandle refresh_panel = nullptr;      /*!< Refresh panel handle */

private:
    /**
     * @brief Interrupt handling structure
     */
    struct Interruption {
        /**
         * @brief Callback data structure
         */
        struct CallbackData {
            void *lcd_ptr = nullptr;      /*!< Pointer to LCD instance */
            void *user_data = nullptr;    /*!< User provided data */
        };

        CallbackData data = {};                                           /*!< Callback data */
        FunctionDrawBitmapFinishCallback on_draw_bitmap_finish = nullptr; /*!< Draw completion callback */
        FunctionRefreshFinishCallback on_refresh_finish = nullptr;        /*!< Refresh completion callback */
        SemaphoreHandle_t draw_bitmap_finish_sem = nullptr;              /*!< Draw completion semaphore */
        std::shared_ptr<StaticSemaphore_t> on_draw_bitmap_finish_sem_buffer = nullptr; /*!< Semaphore buffer */
    };

    /**
     * @brief Get device full configuration
     *
     * @return Reference to full device configuration
     */
    DeviceFullConfig &getDeviceFullConfig();

    /**
     * @brief Get vendor full configuration
     *
     * @return Reference to full vendor configuration
     */
    VendorFullConfig &getVendorFullConfig();

#if SOC_LCD_RGB_SUPPORTED
    const BusRGB::RefreshPanelFullConfig *getBusRGB_RefreshPanelFullConfig();
#endif

#if SOC_MIPI_DSI_SUPPORTED
    const BusDSI::RefreshPanelFullConfig *getBusDSI_RefreshPanelFullConfig();
#endif

    IRAM_ATTR static bool onDrawBitmapFinish(void *panel_io, void *edata, void *user_ctx);
    IRAM_ATTR static bool onRefreshFinish(void *panel_io, void *edata, void *user_ctx);

    BasicAttributes _basic_attributes = {};     /*!< Basic device attributes */
    Bus *_bus = nullptr;                        /*!< Bus interface */
    Config _config = {};                        /*!< Device configuration */
    State _state = State::DEINIT;               /*!< Current driver state */
    Transformation _transformation = {};        /*!< Coordinate transformation settings */
    Interruption _interruption = {};            /*!< Interrupt handling */
};

} // namespace esp_panel::drivers

#if SOC_MIPI_DSI_SUPPORTED
/**
 * @brief Deprecated type alias for backward compatibility
 * @deprecated Use `esp_panel::drivers::LCD::DSI_ColorBarPattern` instead. This alias will be removed in the next
 *             major version.
 */
using DsiPatternType [[deprecated("Please use `esp_panel::drivers::LCD::DSI_ColorBarPattern` instead")]] =
    esp_panel::drivers::LCD::DSI_ColorBarPattern;
#endif

/**
 * @brief Deprecated type alias for backward compatibility
 * @deprecated Use `esp_panel::drivers::LCD` instead. This alias will be removed in the next major version.
 */
using ESP_PanelLcd [[deprecated("Please use `esp_panel::drivers::LCD` instead")]] = esp_panel::drivers::LCD;
