#include "esp_event.h"
#include "esp_log.h"
#include "esp_event.h"
#include "esp_netif.h"
#include "esp_system.h"
#include "freertos/FreeRTOS.h"
#include "freertos/event_groups.h"
#include "freertos/task.h"
#include "nvs_flash.h"

#include "access_point.h"
// #include "../server/server.h"

#include <string.h>

static const char *TAG = "AP";

void ap_init(AccessPointPtr ap, uint8_t wifi_channel, const char *wifi_ssid, const char *wifi_password, uint8_t wifi_max_sta_conn) {
  // Populate the Access Point wifi_config_t 
  strcpy((char *)ap->wifi_config.ap.ssid, wifi_ssid);
  ap->wifi_config.ap.ssid_len = strlen(wifi_ssid);
  strcpy((char *)ap->wifi_config.ap.password, wifi_password);
  ap->wifi_config.ap.channel = wifi_channel;
  ap->wifi_config.ap.max_connection = wifi_max_sta_conn;
  if (strlen(wifi_password) == 0) {
    ap->wifi_config.ap.authmode = WIFI_AUTH_OPEN;
  } else {
    ap->wifi_config.ap.authmode = WIFI_AUTH_WPA2_PSK;
  }
  ap->wifi_config.ap.pmf_cfg.required = true;
  esp_netif_create_default_wifi_ap();
  ESP_ERROR_CHECK(esp_wifi_set_config(WIFI_IF_AP, &ap->wifi_config));
  ap->initialized = true;
}

bool ap_is_initialized(AccessPointPtr ap) {
  return ap->initialized;
}

bool ap_is_active(AccessPointPtr ap) {
  return ap->state == active;
}

void ap_print_info(AccessPointPtr ap) {
  ESP_LOGI(TAG, "wifi_config.ap.channel: %d", ap->wifi_config.ap.channel);
  ESP_LOGI(TAG, "wifi_config.ap.max_connection: %d", ap->wifi_config.ap.max_connection);
  ESP_LOGI(TAG, "wifi_config.ap.authmode: %d", ap->wifi_config.ap.authmode);
  ESP_LOGI(TAG, "wifi_config.ap.ssid: %s", ap->wifi_config.ap.ssid);
  ESP_LOGI(TAG, "wifi_config.ap.ssid_len: %d", ap->wifi_config.ap.ssid_len);
  ESP_LOGI(TAG, "wifi_config.ap.password: %s", ap->wifi_config.ap.password);
}

void ap_set_channel(AccessPointPtr ap, uint8_t channel) {
  ap->wifi_config.ap.channel = channel;
  ESP_LOGI(TAG, "Channel set on: %u", ap->channel);
};

void ap_set_ssid(AccessPointPtr ap, const char *ssid) {
  strcpy((char *)ap->wifi_config.ap.ssid, ssid);
  ap->wifi_config.ap.ssid_len = strlen(ssid);
};

void ap_set_password(AccessPointPtr ap, const char *password){
  strcpy((char *)ap->wifi_config.ap.password, password);
};

void ap_update(AccessPointPtr ap) {
  if (ap->state == active) {
    ap_stop(ap);
  }
  ESP_ERROR_CHECK(esp_wifi_set_config(WIFI_IF_AP, &ap->wifi_config));
  ap_start(ap);
};

void ap_start(AccessPointPtr ap) {
  ap->state = active;
  ESP_ERROR_CHECK(esp_wifi_start());
};

void ap_stop(AccessPointPtr ap){
  ESP_LOGI(TAG, "Stoping AP");
  ap->state = inactive;
  ESP_ERROR_CHECK(esp_wifi_stop());
};

void ap_restart(AccessPointPtr ap) {
  ap_stop(ap);
  ap_start(ap);
};
