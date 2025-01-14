idf_component_register(
    SRCS "test_app_main.cpp" "test_spi_lcd.cpp"
    PRIV_REQUIRES esp_lcd driver
    WHOLE_ARCHIVE
)
