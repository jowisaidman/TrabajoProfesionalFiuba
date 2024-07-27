idf_component_register(SRCS "spi.c"
                    INCLUDE_DIRS "."
                    REQUIRES esp_wifi nvs_flash esp_event)
