#include <string.h>
#include <stdint.h>
#include <stdbool.h>

static const uint16_t SOUTH_NORTH_CHANNELS[] = {1, 3, 5, 7, 9, 11, 13};
static const uint16_t EAST_WEST_CHANNELS[] = {2, 4, 6, 8, 10, 12};
static const char* ACTUAL_DEVICES_CONNECTED[] = {"0002"};

bool is_in_list(uint16_t number, const uint16_t list[]) {
  int size = sizeof(list) / sizeof(list[0]);
  for (int i = 0; i < size; i++) {
      if (list[i] == number) {
        return true;
      }
  }
  return false;
}

bool is_in_list_char(char* item, const char* list[]) {
  int size = sizeof(list) / sizeof(list[0]);
  for (int i = 0; i < size; i++) {
      if (list[i] == item) {
        return true;
      }
  }
  return false;
}

bool is_channel_allowed(uint16_t orientation, uint16_t channel) {
  if (orientation == 0 || orientation == 1) {
    return is_in_list(channel, SOUTH_NORTH_CHANNELS);
  } else if (orientation == 2 || orientation == 3) {
    return is_in_list(channel, EAST_WEST_CHANNELS);
  } else {
    return false; //TODO: levantar exception?
  }
}

bool is_network_allowed(char* device_uuid, char* network_prefix, char* network_name) {
  return ((strstr(network_name, network_prefix) != NULL) && (strstr(network_name, device_uuid) == NULL));
}

bool can_connect_to_node(char* network_name) {
  // Separate the prefix from the uuid <prefix>_<uuid>
  char* token = strtok(network_name, "_");
  token = strtok(NULL, "_");
  ESP_LOGI("station", "Token: %s", token);
  return !is_in_list_char(token, ACTUAL_DEVICES_CONNECTED);
}