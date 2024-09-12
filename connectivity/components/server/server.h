// struct Server {
//   // Members
//   char device_uuid[32];
//   uint8_t device_orientation;
//   uint8_t device_is_root;
//   char ssid[32];
//   char password[64];
//   uint8_t channel;
//   Device_Mode mode;
//   Device_State state;
//   AccessPoint access_point;
//   AccessPointPtr access_point_ptr;
//   Station station;
//   StationPtr station_ptr;
// };

// typedef struct Server Server;
// typedef struct Server* ServerPtr;

void create_server();
void delete_server();