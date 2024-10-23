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

// Tests
#include "test_shared_memory.h"

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
#define SOFTAP_CHANNEL_TO_EMIT CONFIG_SOFTAP_CHANNEL_TO_EMIT
#define SOFTAP_MAX_STA_CONNECTIONS CONFIG_SOFTAP_MAX_STA_CONNECTIONS

#define RUN_TESTS 0 // 1 = run tests, 0 = run app TODO: set this from config

void app_main(void) {
  if (RUN_TESTS) {
      test_shared_memory(1);
      return;
  }

  ESP_LOGI(LOGGING_TAG, "Device UUID: %s", DEVICE_UUID);
  ESP_LOGI(LOGGING_TAG, "Device orientation: %d", DEVICE_ORIENTATION);
  ESP_LOGI(LOGGING_TAG, "Device is root node: %d", DEVICE_IS_ROOT_NODE);
  ESP_LOGI(LOGGING_TAG, "Device initial mode: %d", DEVICE_INITIAL_MODE);
  ESP_LOGI(LOGGING_TAG, "Device wifi network prefix: %s", DEVICE_WIFI_NETWORK_PREFIX);
  
  Device device = {};
  DevicePtr device_ptr = &device;
  
  device_init(device_ptr, DEVICE_UUID, DEVICE_ORIENTATION, DEVICE_WIFI_NETWORK_PREFIX, DEVICE_WIFI_NETWORK_PASSWORD, SOFTAP_CHANNEL_TO_EMIT, SOFTAP_MAX_STA_CONNECTIONS, DEVICE_IS_ROOT_NODE, DEVICE_INITIAL_MODE);

  device_set_mode(device_ptr, DEVICE_INITIAL_MODE);

  // create_server();

  // sleep(10);

  // delete_server();

  // device_set_network_ap(device_ptr, "10.202.0.1", "10.203.255.254", "255.254.0.0");

  // sleep(30);

  // device_set_network_ap(device_ptr, "192.168.3.1", "192.168.3.254", "255.255.255.0");
}