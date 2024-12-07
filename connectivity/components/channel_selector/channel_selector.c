#include "channel_selector.h"

int get_channel_for_orientation(int orientation, int jumps_from_root) {
    int channels[] = {NORTH_CHANNELS, SOUTH_CHANNELS, EAST_CHANNELS, WEST_CHANNELS};
    int channel_index = jumps_from_root % 4;
    return channels[orientation][channel_index];
}