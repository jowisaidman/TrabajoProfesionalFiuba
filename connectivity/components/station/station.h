/*
 * @brief Initialize the WiFi stack in station mode
 */
void init_station_mode();
/*
 * @brief Discover the AP with the name like 'ESP_' and return the AP
 * information to connect
 * @param wifi_ssid_like The name of the AP to discover
 * @return wifi_ap_record_t The AP information to connect
 */
wifi_ap_record_t discover_wifi_ap(const char* wifi_ssid_like);

/*
* @brief Event handler for WiFi events for station mode
* @param arg The argument passed during handler registration
* @param event_base The event base
* @param event_id The event id
* @param event_data The event data
*/
static void event_handler(void* arg, esp_event_base_t event_base,
                          int32_t event_id, void* event_data);

/*
 * @brief Connect to the AP with the given SSID and password
 * @param ssid The SSID of the AP to connect
 * @param password The password of the AP to connect
 */
void connect_to_wifi(wifi_config_t wifi_config);

/*
 * @brief Wait until the connection is established
 */
void wait_connection_established();