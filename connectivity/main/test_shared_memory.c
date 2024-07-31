#include "test_shared_memory.h"

void test_init_shared_memory() {
    SharedMemory shared_memory;
    init_shared_memory(&shared_memory);

    assert(shared_memory.size == 0);
    for (int i = 0; i < TABLE_SIZE; i++) {
        assert(shared_memory.buckets[i].key == 0);
    }

    printf("test_init_shared_memory passed.\n");
}

void test_shared_memory(int activate) {
    if (activate) {
        test_init_shared_memory();
    }
}