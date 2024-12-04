struct Server {
  // Members
  int listen_sock;
  TaskHandle_t task;
  StackType_t xStack[ 2 ];
  StaticTask_t xTaskBuffer;
};

typedef struct Server Server;
typedef struct Server* ServerPtr;

void create_server(ServerPtr server_ptr);
void delete_server(ServerPtr server_ptr);
void init_server(ServerPtr server_ptr);
