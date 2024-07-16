#include <lwip/netdb.h>
#include <string.h>
#include <unistd.h>

#include "esp_event.h"
#include "esp_log.h"
#include "esp_wifi.h"
#include "esp_event.h"
#include "esp_mac.h"
#include "esp_netif.h"
#include "freertos/FreeRTOS.h"
#include "freertos/event_groups.h"
#include "lwip/err.h"
#include "lwip/sockets.h"
#include "lwip/sys.h"
#include "nvs_flash.h"

// #include "station.h"
// #include "access_point.h"
#include "device.h"

/* General config */
#define LOGGING_TAG "MAIN"

/* Device configuration */
#define DEVICE_WIFI_NETWORK_PREFIX CONFIG_DEVICE_WIFI_NETWORK_PREFIX
#define DEVICE_WIFI_NETWORK_PASSWORD CONFIG_DEVICE_WIFI_NETWORK_PASSWORD
#define DEVICE_IS_ROOT_NODE CONFIG_DEVICE_IS_ROOT_NODE
#define DEVICE_UUID CONFIG_DEVICE_UUID
#define DEVICE_ORIENTATION CONFIG_DEVICE_ORIENTATION  // N = 0 S = 1 E = 2 W = 3
#define DEVICE_INITIAL_MODE CONFIG_DEVICE_INITIAL_MODE

/* Access point config */
#define SOFTAP_CHANNEL_TO_EMIT 3
#define SOFTAP_MAX_STA_CONNECTIONS 1

void wifi_event_handler(void *arg, esp_event_base_t event_base, int32_t event_id, void *event_data) {
  if (event_id == WIFI_EVENT_AP_STACONNECTED) {
    wifi_event_ap_staconnected_t *event = (wifi_event_ap_staconnected_t *)event_data;
    ESP_LOGI(LOGGING_TAG, "station " MACSTR " join, AID=%d", MAC2STR(event->mac), event->aid);
  } else if (event_id == WIFI_EVENT_AP_STADISCONNECTED) {
    wifi_event_ap_stadisconnected_t *event = (wifi_event_ap_stadisconnected_t *)event_data;
    ESP_LOGI(LOGGING_TAG, "station " MACSTR " leave, AID=%d", MAC2STR(event->mac), event->aid);
  } else if (event_id == IP_EVENT_AP_STAIPASSIGNED) {
    ip_event_ap_staipassigned_t *event = (ip_event_ap_staipassigned_t *)event_data;
    ESP_LOGI(LOGGING_TAG, "station ip:" IPSTR ", mac:" MACSTR "", IP2STR(&event->ip), MAC2STR(event->mac));
  }
}




void app_main(void) {
  ESP_LOGI(LOGGING_TAG, "Device UUID: %s", DEVICE_UUID);
  ESP_LOGI(LOGGING_TAG, "Device orientation: %d", DEVICE_ORIENTATION);
  ESP_LOGI(LOGGING_TAG, "Device is root node: %d", DEVICE_IS_ROOT_NODE);
  ESP_LOGI(LOGGING_TAG, "Device initial mode: %d", DEVICE_INITIAL_MODE);
  ESP_LOGI(LOGGING_TAG, "Device wifi network prefix: %s", DEVICE_WIFI_NETWORK_PREFIX);

  esp_err_t ret = nvs_flash_init();
  if (ret == ESP_ERR_NVS_NO_FREE_PAGES || ret == ESP_ERR_NVS_NEW_VERSION_FOUND) {
    ESP_ERROR_CHECK(nvs_flash_erase());
    ret = nvs_flash_init();
  }
  ESP_ERROR_CHECK(ret);


  ESP_ERROR_CHECK(esp_netif_init());
  ESP_ERROR_CHECK(esp_event_loop_create_default());

  wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();
  ESP_ERROR_CHECK(esp_wifi_init(&cfg));

  if (DEVICE_INITIAL_MODE == 0) {
    ESP_LOGI(LOGGING_TAG, "ESP on AP mode");
    
    AccessPoint ap = {};
    AccessPointPtr ap_ptr = &ap;

    esp_event_handler_instance_t instance_any_id;
    ESP_ERROR_CHECK(esp_event_handler_instance_register(WIFI_EVENT,
                                                        ESP_EVENT_ANY_ID,
                                                        &wifi_event_handler,
                                                        NULL,
                                                        &instance_any_id));

    // Print info of ap->wifi_config
    ESP_ERROR_CHECK(esp_wifi_set_mode(WIFI_MODE_AP));

    // Generate the wifi ssid
    char wifi_ssid[32];
    memset(wifi_ssid, 0, sizeof(wifi_ssid));
    strcpy(wifi_ssid, DEVICE_WIFI_NETWORK_PREFIX);
    strcat(wifi_ssid, "_");
    strcat(wifi_ssid, DEVICE_UUID);

    ap_init(ap_ptr, SOFTAP_CHANNEL_TO_EMIT, wifi_ssid, DEVICE_WIFI_NETWORK_PASSWORD, SOFTAP_MAX_STA_CONNECTIONS);
    ap_start(ap_ptr);
    sleep(20);
    ap_set_ssid(ap_ptr, "tpp_0003");
    ap_update(ap_ptr);
  } else if (DEVICE_INITIAL_MODE == 1) {
    ESP_LOGI(LOGGING_TAG, "ESP on STA mode");

    Station station = {};
    StationPtr station_ptr = &station;

    station_init(station_ptr, DEVICE_WIFI_NETWORK_PREFIX, DEVICE_ORIENTATION, DEVICE_UUID, DEVICE_WIFI_NETWORK_PASSWORD);

    while (true) {
      station_find_ap(station_ptr);
      if (station_found_ap(station_ptr)) {
        ESP_LOGI(LOGGING_TAG, "WIFI FOUND! Initiating connection.");
        station_start(station_ptr);
        
        wait_connection_established();
        break;
      } else {
        ESP_LOGE(LOGGING_TAG, "No wifi found, trying to reconnect in 10 seconds");
        sleep(10);
      }
    }
  } else if (DEVICE_INITIAL_MODE == 2) {
    ESP_LOGI(LOGGING_TAG, "ESP_WIFI_MODE_APSTA");
  } else {
    ESP_LOGE(LOGGING_TAG, "Invalid initial mode");
  }
}