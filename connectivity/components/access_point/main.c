// #include "esp_mac.h"
// #include "esp_wifi.h"
// #include "esp_event.h"
// #include "esp_log.h"
// #include "nvs_flash.h"

// /*
// * @brief Event handler for WiFi events in AP mode
// * @param arg system event data
// * @param event_base event base
// * @param event_id event id
// * @param event_data event data
// */
// static void wifi_event_handler(void *arg, esp_event_base_t event_base,
//                               int32_t event_id, void *event_data) {
//   if (event_id == WIFI_EVENT_AP_STACONNECTED) {
//     wifi_event_ap_staconnected_t *event = (wifi_event_ap_staconnected_t *)event_data;
//     ESP_LOGI(TAG, "station " MACSTR " join, AID=%d",
//             MAC2STR(event->mac), event->aid);
//   } else if (event_id == WIFI_EVENT_AP_STADISCONNECTED) {
//     wifi_event_ap_stadisconnected_t *event = (wifi_event_ap_stadisconnected_t *)event_data;
//     ESP_LOGI(TAG, "station " MACSTR " leave, AID=%d",
//             MAC2STR(event->mac), event->aid);
//   } else if (event_id == IP_EVENT_AP_STAIPASSIGNED) {
//     ESP_LOGI(TAG, "Se assigno IP");
//     // Print the IP assigned to the station and the MAC address of the station
//     ip_event_ap_staipassigned_t *event = (ip_event_ap_staipassigned_t *)event_data;
//     ESP_LOGI(TAG, "station ip:" IPSTR ", mac:" MACSTR "",
//             IP2STR(&event->ip), MAC2STR(event->mac));
//     // xTaskCreate(udp_client_task, "udp_client", 4096, NULL, 5, NULL);
//   }
// }