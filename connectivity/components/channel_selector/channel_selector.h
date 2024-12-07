#include <stdio.h>

#define NORTH_CHANNELS {1, 5, 9}
#define SOUTH_CHANNELS {2, 6, 10}
#define EAST_CHANNELS {3, 7, 11}
#define WEST_CHANNELS {4, 8, 12}


int get_channel_for_orientation(int orientation, int jumps_from_root);