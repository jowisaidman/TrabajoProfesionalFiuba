idf_component_register(SRCS "channel_selector.c"
                    INCLUDE_DIRS "."
                    REQUIRES esp_wifi nvs_flash esp_event)