#include "shared_memory.h"

void init_shared_memory(*SharedMemory shared_memory) {
    shared_memory->buckets = malloc(TABLE_SIZE * sizeof(Node*));
    shared_memory->size = 0;

    for (int i = 0; i < TABLE_SIZE; i++) {
        shared_memory->buckets[i] = NULL;
    }
}

void save_node_variable(*SharedMemory shared_memory, char *msg, uint16_t key) {
    Node* new_node = malloc(sizeof(Node)); // TODO: que hacemos si falla el malloc?
    strcpy(new_node->value, msg);
    new_node->key = key;
    shared_memory->size++;
    shared_memory->buckets[shared_memory->size] = new_node;
}

Node* get_node_variable(*SharedMemory shared_memory, uint16_t key) {
    return hashmap->buckets[index]; //TODO: chequear si con devolver el puntero alcanza
}