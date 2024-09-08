#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "token_ring.h"
#include "../routing/routing.h"
#include "../spi/spi.h"

#define MAX_DEVICE_ID 4

void token_ring_init(TokenRing* token_ring, uint8_t device_id) {
    token_ring->device_id = device_id;
}

bool is_device_shift(TokenRing* token_ring, TokenRingMessage* message) {
    return message->data[0] == token_ring->device_id;
}

void token_ring_message_init(TokenRingMessage* token_ring) {
    memset(token_ring->data, 0, MESSAGE_LENGTH_BYTES);
    token_ring->data[MESSAGE_LENGTH_BYTES-1] = '\0';
}

void update_token_ring_message(TokenRingMessage* message, uint16_t new_connection, uint8_t device_id) {
    uint8_t offset = 1 + device_id * 2; // skip first byte for shift id.
    message->data[0] = (device_id + 1) % MAX_DEVICE_ID;
    if (offset < MESSAGE_LENGTH_BYTES - 1) {
        message->data[offset] = (new_connection >> 8) & 0xFF;
        message->data[offset + 1] = new_connection & 0xFF;
    }
}