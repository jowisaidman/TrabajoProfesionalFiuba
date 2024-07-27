#include "esp_wifi.h"

#include "../access_point/access_point.h"
#include "../station/station.h"

/*
* State enum to manage the state of the Access Point
*/
typedef enum {
  nan,
  ap,  // Access Point is active
  station, // Access Point is inactive
  ap_station
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
  char device_uuid[32];
  uint8_t device_orientation;
  bool device_is_root;
  char ssid[32];
  char password[64];
  uint8_t channel;
  Device_Mode mode;
  Device_State state;
  AccessPointPtr access_point_ptr;
  StationPtr station_ptr;
};

typedef struct Device Device;
typedef struct Device* DevicePtr;

// Methods
void device_init(DevicePtr device_ptr);
void device_init_ap(DevicePtr device_ptr, uint8_t channel, const char *wifi_network_prefix ,const char *device_uuid, const char *password, uint8_t max_sta_connections);
void device_set_mode(DevicePtr device_ptr, Device_Mode mode);
void device_reset(DevicePtr device_ptr);
void device_start_ap(DevicePtr device_ptr);
void device_stop_ap(DevicePtr device_ptr);
void device_restart_ap(DevicePtr device_ptr);
void device_start_station(DevicePtr device_ptr);
void device_stop_station(DevicePtr device_ptr);
void device_restart_station(DevicePtr device_ptr);
// void device_start_ap_station(DevicePtr device_ptr);
// void device_stop_ap_station(DevicePtr device_ptr);
// void device_restart_ap_station(DevicePtr device_ptr);
// void device_start_tcp_server(DevicePtr device_ptr);
// void device_stop_tcp_server(DevicePtr device_ptr);
// void device_restart_tcp_server(DevicePtr device_ptr);
// void device_handle_message(DevicePtr device_ptr);

// Quiza podemos agregar un flag para saber a quien va el mensaje
// Aca tendriamos el handle message y llamamos al station 