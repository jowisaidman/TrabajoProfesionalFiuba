#include "integration_test.h"
#include "token_ring.h"

void test_token_ring_init() {
    TokenRing token_ring;
    token_ring_init(&token_ring, 1);

    assert(token_ring.device_id == 1);

    printf("test_token_ring_init passed.\n");
}

void test_token_ring_message_init() {
    TokenRingMessage message;
    token_ring_message_init(&message);

    for (int i = 0; i < MESSAGE_LENGTH_BYTES - 1; i++) {
        assert(message.data[i] == 0);
    }
    assert(message.data[MESSAGE_LENGTH_BYTES - 1] == '\0');

    printf("test_token_ring_message_init passed.\n");
}

void test_is_device_shift() {
    TokenRing token_ring;
    token_ring_init(&token_ring, 3);

    TokenRingMessage message;
    token_ring_message_init(&message);

    // Simulate setting the first byte to the device_id to simulate a shift
    message.data[0] = 3;
    assert(is_device_shift(&token_ring, &message) == 1);

    // Change the shift_id to something else
    message.data[0] = 4;
    assert(is_device_shift(&token_ring, &message) == 0);

    printf("test_is_device_shift passed.\n");
}

void test_update_token_ring_message() {
    TokenRingMessage message;
    token_ring_message_init(&message);

    uint16_t new_connection = 12345;
    uint8_t device_id = 2;

    update_token_ring_message(&message, new_connection, device_id);

    // Check that the correct bytes in the data array were updated
    assert(message.data[5] == ((new_connection >> 8) & 0xFF));    // High byte
    assert(message.data[6] == (new_connection & 0xFF));           // Low byte

    printf("test_update_token_ring_message passed.\n");
}

void test_token_ring(int activate) {
    if (activate) {
        test_token_ring_init();
        test_token_ring_message_init();
        test_is_device_shift();
        test_update_token_ring_message();
    }
}
