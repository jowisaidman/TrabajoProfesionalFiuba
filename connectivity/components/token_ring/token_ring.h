#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define MESSAGE_LENGTH_BYTES 10

typedef struct TokenRing {
    uint8_t device_id;
} TokenRing;

typedef struct TokenRingMessage {
    char data[MESSAGE_LENGTH_BYTES];
} TokenRingMessage;

void token_ring_init(TokenRing* token_ring, uint8_t device_id);
bool is_device_shift(TokenRing* token_ring, TokenRingMessage* message);
//void send_next_shift(TokenRing* token_ring, TokenRingMessage* message, char* message_update);
void token_ring_message_init(TokenRingMessage* token_ring);
void update_token_ring_message(TokenRingMessage* message, uint16_t new_connection, uint8_t device_id);