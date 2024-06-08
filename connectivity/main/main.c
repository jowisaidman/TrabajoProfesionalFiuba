#include <lwip/netdb.h>
#include <string.h>

#include "esp_event.h"
#include "esp_log.h"
#include "esp_wifi.h"
#include "freertos/FreeRTOS.h"
#include "freertos/event_groups.h"
#include "lwip/err.h"
#include "lwip/sockets.h"
#include "lwip/sys.h"
#include "nvs_flash.h"
#include "access_point.h"
#include "station.h"

#define WIFI_NETWORK_PREFIX "myssid"
//#define IS_ROOT_NODE 0

void app_main(void) {
  if (false) {
    esp_err_t ret = nvs_flash_init();
    if (ret == ESP_ERR_NVS_NO_FREE_PAGES || ret == ESP_ERR_NVS_NEW_VERSION_FOUND) {
      ESP_ERROR_CHECK(nvs_flash_erase());
      ret = nvs_flash_init();
    }
    ESP_ERROR_CHECK(ret);
  
    ESP_LOGI("STATION", "ESP_WIFI_MODE_AP");
    wifi_init_softap();
  } else {
    init_station_mode();

    wifi_ap_record_t record = discover_wifi_ap(WIFI_NETWORK_PREFIX);

    wifi_config_t wifi_config = {};
    strcpy((char *)wifi_config.sta.ssid, (const char *)record.ssid);
    connect_to_wifi(wifi_config);

    wait_connection_established();
  }
}
