struct Server {
  // Members
  char addr_str[128];
  int addr_family;
  int ip_protocol;
  int keepAlive;
  int keepIdle;
  int keepInterval;
  int keepCount;
  int listen_sock;
  int sock;
  TaskHandle_t server_task_handle;
};

typedef struct Server Server;
typedef struct Server* ServerPtr;

void create_server(ServerPtr server_ptr);
void delete_server(ServerPtr server_ptr);
void init_server(ServerPtr server_ptr);