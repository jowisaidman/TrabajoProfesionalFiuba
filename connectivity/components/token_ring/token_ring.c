#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "token_ring.h"
#include "../routing/routing.h"
#include "../spi/spi.h"

#define SHIFT_INTERVAL 5 //TODO: lo mandamos a una config?
#define MAX_DEVICE_ID 4

void token_ring_init(TokenRing* token_ring, uint8_t device_id) {
    token_ring->device_id = device_id;
}

bool is_device_shift(TokenRing* token_ring, TokenRingMessage* message) {
    return message->data[0] == token_ring->device_id;
}

//void send_next_shift(TokenRing* token_ring, TokenRingMessage* message, char* message_update) {
//    if (message_update != null) {
//        // Setear el nuevo update en el message
//    }
//    uint8_t new_shift_id = (token_ring->device_id + 1) % MAX_DEVICE_ID;
//    message->device_id;
//    broadcast_to_siblings(message, 38);
//}

void token_ring_message_init(TokenRingMessage* token_ring) {
    memset(token_ring->data, 0, MESSAGE_LENGTH_BYTES);
    token_ring->data[MESSAGE_LENGTH_BYTES-1] = '\0';
}

void update_token_ring_message(TokenRingMessage* message, uint16_t new_connection, uint8_t device_id) {
    uint8_t offset = 1 + device_id * 2; // skip first byte for shift id.
    if (offset < MESSAGE_LENGTH_BYTES - 1) {
        message->data[offset] = (new_connection >> 8) & 0xFF;
        message->data[offset + 1] = new_connection & 0xFF;
    }
}