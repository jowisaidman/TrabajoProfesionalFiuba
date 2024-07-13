#include <lwip/netdb.h>
#include <string.h>
#include <unistd.h>

#include "esp_event.h"
#include "esp_log.h"
#include "esp_wifi.h"
#include "freertos/FreeRTOS.h"
#include "freertos/event_groups.h"
#include "lwip/err.h"
#include "lwip/sockets.h"
#include "lwip/sys.h"
#include "nvs_flash.h"

#include "station.h"
#include "access_point.h"

/* General config */
#define LOGGING_TAG "MAIN"

/* Device configuration */
#define DEVICE_WIFI_NETWORK_PREFIX CONFIG_DEVICE_WIFI_NETWORK_PREFIX
#define DEVICE_WIFI_NETWORK_PASSWORD CONFIG_DEVICE_WIFI_NETWORK_PASSWORD
#define DEVICE_IS_ROOT_NODE CONFIG_DEVICE_IS_ROOT_NODE 
#define DEVICE_UUID CONFIG_DEVICE_UUID
#define DEVICE_ORIENTATION CONFIG_DEVICE_ORIENTATION // N = 0 S = 1 E = 2 W = 3
#define DEVICE_INITIAL_MODE CONFIG_DEVICE_INITIAL_MODE


/* Access point config */
#define SOFTAP_CHANNEL_TO_EMIT 3

void app_main(void) {
  ESP_LOGI(LOGGING_TAG, "Device UUID: %s", DEVICE_UUID);
  ESP_LOGI(LOGGING_TAG, "Device orientation: %d", DEVICE_ORIENTATION);
  ESP_LOGI(LOGGING_TAG, "Device is root node: %d", DEVICE_IS_ROOT_NODE);
  ESP_LOGI(LOGGING_TAG, "Device initial mode: %d", DEVICE_INITIAL_MODE);
  ESP_LOGI(LOGGING_TAG, "Device wifi network prefix: %s", DEVICE_WIFI_NETWORK_PREFIX);
  
  if (DEVICE_INITIAL_MODE == 0) {
    ESP_LOGI(LOGGING_TAG, "ESP on AP mode");
    
    esp_err_t ret = nvs_flash_init();
    if (ret == ESP_ERR_NVS_NO_FREE_PAGES || ret == ESP_ERR_NVS_NEW_VERSION_FOUND) {
      ESP_ERROR_CHECK(nvs_flash_erase());
      ret = nvs_flash_init();
    }
    ESP_ERROR_CHECK(ret);

    ap_init(SOFTAP_CHANNEL_TO_EMIT, DEVICE_WIFI_NETWORK_PREFIX, DEVICE_UUID, DEVICE_WIFI_NETWORK_PASSWORD);
  } else if (DEVICE_INITIAL_MODE == 1) {
    ESP_LOGI(LOGGING_TAG, "ESP on STA mode");
    init_station_mode();

    while (true) {
      struct wifi_ap_record_t_owned record = discover_wifi_ap(DEVICE_WIFI_NETWORK_PREFIX, DEVICE_ORIENTATION, DEVICE_UUID);
      if(record.found) {
        ESP_LOGI(LOGGING_TAG, "WIFI FOUND! SSID: %s. Initiating connection.", record.ap_info.ssid);
        wifi_config_t wifi_config = {};
        strcpy((char *)wifi_config.sta.ssid, (const char *)record.ap_info.ssid);
        strcpy((char *)wifi_config.sta.password, DEVICE_WIFI_NETWORK_PASSWORD);
        connect_to_wifi(wifi_config);

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
