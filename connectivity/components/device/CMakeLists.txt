idf_component_register(SRCS "device.c"
                    INCLUDE_DIRS "."
                    REQUIRES access_point esp_wifi nvs_flash esp_event server)
