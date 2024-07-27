#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define TABLE_SIZE 100

typedef struct Node {
    char* value;
    uint16_t key;
} Node;

typedef struct SharedMemory { // TODO: chequear si queremos guardar la info en algo asi como un hashmap
    Node** buckets;
    int size;
} SharedMemory;


void init_shared_memory(*SharedMemory shared_memory);
void save_node_variable(*SharedMemory shared_memory,const char *msg, uint16_t key);
Node* get_node_variable(*SharedMemory shared_memory, uint16_t key);