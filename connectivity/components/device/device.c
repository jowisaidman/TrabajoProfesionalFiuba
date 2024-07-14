#include "esp_event.h"
#include "esp_log.h"
#include "esp_event.h"
#include "esp_netif.h"
#include "esp_system.h"
#include "freertos/FreeRTOS.h"
#include "freertos/event_groups.h"
#include "freertos/task.h"
#include "nvs_flash.h"
#include <string.h>

#include "device.h"

#define LOGGING_TAG "DEVICE"

void device_init(DevicePtr device_ptr) {
  device_ptr->mode = nan;
  device_ptr->state = d_inactive;

  // Initialize NVS
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
}

void device_init_ap(DevicePtr device_ptr, uint8_t channel, const char *wifi_network_prefix ,const char *device_uuid, const char *password, uint8_t max_sta_connections) {
  // Generate the wifi ssid
  char wifi_ssid[32];
  memset(wifi_ssid, 0, sizeof(wifi_ssid));
  strcpy(wifi_ssid, wifi_network_prefix);
  strcat(wifi_ssid, "_");
  strcat(wifi_ssid, device_uuid);

  ap_init(&device_ptr->access_point, channel, wifi_ssid, password, max_sta_connections);
};

void device_reset(DevicePtr device_ptr) {
  if (device_ptr->state == d_active) {
    if (device_ptr->mode == ap) {
      device_stop_ap(device_ptr);
    } else if (device_ptr->mode == station) {
      device_stop_station(device_ptr);
    } 
    // else if (device_ptr->mode == ap_station) {
    //   device_stop_ap_station(device_ptr);
    // }
    device_ptr->state = d_inactive;
  }
  device_ptr->mode = nan;
}

void device_set_mode(DevicePtr device_ptr, Device_Mode mode) {
  if (device_ptr->mode == mode) {
    ESP_LOGI(LOGGING_TAG, "Device mode is already setted");
    return;
  }
  device_reset(device_ptr);
  device_ptr->mode = mode;
  if (mode == ap) {
    device_start_ap(device_ptr);
  } else if (mode == station) {
    device_start_station(device_ptr);
  }
  // else if (mode == ap_station) {
  //   device_start_ap_station(device_ptr);
  // }
}

void device_start_ap(DevicePtr device_ptr) {
  if (ap_is_initialized(&device_ptr->access_point)) {
    ap_start(&device_ptr->access_point);
    device_ptr->state = d_active;
  } else {
    ESP_LOGE(LOGGING_TAG, "Access Point is not initialized");
  }
};

void device_stop_ap(DevicePtr device_ptr) {
  // if (device_ptr->state == d_active && device_ptr->mode == ap && ap_is_active(&device_ptr->access_point)) {
  // }
  ap_stop(&device_ptr->access_point);
};

void device_restart_ap(DevicePtr device_ptr) {
  ap_restart(&device_ptr->access_point);
};
