#include <stdio.h>
#include <stdlib.h>

typedef struct {
    uint8_t device_id,
} TokenRing;

void token_ring_init(*TokenRing token_ring, uint8_t device_id);
void wait_for_next_shift(*TokenRing token_ring);
void send_next_shift(*TokenRing token_ring);