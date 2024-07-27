#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "token_ring.h"
#include "../networking/networking.h"
#include "../spi/spi.h"

#define SHIFT_INTERVAL 5; //TODO: lo mandamos a una config?
#define MAX_DEVICE_ID 4;

void token_ring_init(*TokenRing token_ring, uint8_t device_id) {
    token_ring->device_id = device_id;
}

void wait_for_next_shift(*TokenRing token_ring) {
    const uint8_t msg;
    uint16_t len;
    on_sibling_message(&msg, len); // TODO: como parseamos el msg?
    while token_ring->device_id != msg {
        sleep(SHIFT_INTERVAL);
        on_sibling_message(&msg, len); 
        // TODO: parsear el mensaje para que el while compare si termina
    }
}

void send_next_shift(*TokenRing token_ring) {
    uint8_t new_shift = (token_ring->device_id + 1) % MAX_DEVICE_ID;
    broadcast_to_siblings(new_shift, 1);
}