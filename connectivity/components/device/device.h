#include "esp_wifi.h"

#include "../access_point/access_point.h"

/*
* State enum to manage the state of the Access Point
*/
typedef enum {
  ap = 0,  // Access Point is active
  station = 1, // Access Point is inactive
  ap_station = 2
} Device_Mode;

typedef enum {
  d_active,  // Access Point is active
  d_inactive // Access Point is inactive
} Device_State;


/*
* Struct to manage the Access Point
*/
struct Device {
  // Members
  char ssid[32];
  char password[64];
  uint8_t channel;
  Device_Mode mode;
  Device_State state;
  AccessPoint access_point;
};

typedef struct Device Device;

// Methods
void device_init(Device device, uint8_t channel, const char *wifi_network_prefix ,const char *device_uuid, const char *password);