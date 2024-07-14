#include "esp_wifi.h"

/*
* State enum to manage the state of the Access Point
*/
typedef enum {
  active = 1,  // Access Point is active
  inactive = 0 // Access Point is inactive
} AccessPoint_State;

/*
* Struct to manage the Access Point
*/
struct AccessPoint {
  // Members
  char ssid[32];
  char password[64];
  uint8_t channel;
  AccessPoint_State state;
  wifi_config_t wifi_config;
};

typedef struct AccessPoint AccessPoint;
typedef struct AccessPoint* AccessPointPtr;

// Methods
void ap_init(AccessPointPtr ap, uint8_t wifi_channel, const char *wifi_ssid , const char *wifi_password, uint8_t wifi_max_sta_conn);
void ap_print_info(AccessPointPtr ap);
void ap_set_base_wifi_default_config(AccessPointPtr ap);
void ap_set_channel(AccessPointPtr ap, uint8_t channel);
void ap_set_ssid(AccessPointPtr ap, const char *ssid);
void ap_set_password(AccessPointPtr ap, const char *password);
void ap_update(AccessPointPtr ap, bool restart);
void ap_activate(AccessPointPtr ap);
void ap_deactivate(AccessPointPtr ap);
void ap_restart(AccessPointPtr ap);

/*
 * @brief Event handler for WiFi events in AP mode
 * @param arg system event data
 * @param event_base event base
 * @param event_id event id
 * @param event_data event data
 */
static void wifi_event_handler(void *arg, esp_event_base_t event_base, int32_t event_id, void *event_data);
