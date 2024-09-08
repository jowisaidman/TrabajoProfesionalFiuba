#include "integration_test.h"
#include "shared_memory.h"

void test_init_shared_memory() {
    SharedMemory shared_memory;
    init_shared_memory(&shared_memory);

    assert(shared_memory.size == 0);
    for (int i = 0; i < TABLE_SIZE; i++) {
        assert(shared_memory.buckets[i].key == 0);
    }

    printf("test_init_shared_memory passed.\n");
}

void test_save_node_variable() {
    SharedMemory shared_memory;
    init_shared_memory(&shared_memory);

    const char* msg1 = "Hello, World!";
    uint16_t key1 = 42;

    save_node_variable(&shared_memory, msg1, key1);
    Node* node1 = get_node_variable(&shared_memory, key1);

    assert(node1 != NULL);
    assert(node1->key == key1);
    assert(strcmp(node1->value, msg1) == 0);

    const char* msg2 = "Good bye world";
    uint16_t key2 = 84;

    save_node_variable(&shared_memory, msg2, key2);
    Node* node2 = get_node_variable(&shared_memory, key2);

    assert(node2 != NULL);
    assert(node2->key == key2);
    assert(strcmp(node2->value, msg2) == 0);

    printf("test_save_node_variable passed.\n");
}

void test_get_node_variable_not_found() {
    SharedMemory shared_memory;
    init_shared_memory(&shared_memory);

    Node* node = get_node_variable(&shared_memory, 9999); // No vamos a tener memoria para 9999 lugares, vemos que sea NULL

    assert(node == NULL);

    printf("test_get_node_variable_not_found passed.\n");
}

void test_shared_memory(int activate) {
    if (activate) {
        test_init_shared_memory();
        test_save_node_variable();
        test_get_node_variable_not_found();
    }
}