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
  bool initialized;
  AccessPoint_State state;
  wifi_config_t wifi_config;
};

typedef struct AccessPoint AccessPoint;
typedef struct AccessPoint* AccessPointPtr;

// Methods
void ap_init(AccessPointPtr ap, uint8_t wifi_channel, const char *wifi_ssid , const char *wifi_password, uint8_t wifi_max_sta_conn);
void ap_print_info(AccessPointPtr ap);
bool ap_is_initialized(AccessPointPtr ap);
bool ap_is_active(AccessPointPtr ap);
void ap_set_channel(AccessPointPtr ap, uint8_t channel);
void ap_set_ssid(AccessPointPtr ap, const char *ssid);
void ap_set_password(AccessPointPtr ap, const char *password);
void ap_update(AccessPointPtr ap);
void ap_start(AccessPointPtr ap);
void ap_stop(AccessPointPtr ap);
void ap_restart(AccessPointPtr ap);