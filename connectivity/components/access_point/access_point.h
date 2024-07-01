
/*
 * @brief Event handler for WiFi events in AP mode
 * @param arg system event data
 * @param event_base event base
 * @param event_id event id
 * @param event_data event data
 */
static void wifi_event_handler(void *arg, esp_event_base_t event_base, int32_t event_id, void *event_data);

/*
 * @brief Initialize the WiFi in AP mode
 */
void ac_init(uint8_t channel, const char *device_uuid);
