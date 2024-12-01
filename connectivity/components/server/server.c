/* BSD Socket API Example

   This example code is in the Public Domain (or CC0 licensed, at your option.)

   Unless required by applicable law or agreed to in writing, this
   software is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
   CONDITIONS OF ANY KIND, either express or implied.
*/



#include "esp_event.h"
#include "esp_log.h"
#include "esp_netif.h"
#include "esp_system.h"
#include "esp_wifi.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "lwip/err.h"
#include "lwip/sockets.h"
#include "lwip/sys.h"
#include "nvs_flash.h"
#include "esp_netif.h"

#include <lwip/netdb.h>
#include <string.h>
#include <sys/param.h>

#include "server.h"

#define PORT CONFIG_EXAMPLE_PORT
#define KEEPALIVE_IDLE CONFIG_EXAMPLE_KEEPALIVE_IDLE
#define KEEPALIVE_INTERVAL CONFIG_EXAMPLE_KEEPALIVE_INTERVAL
#define KEEPALIVE_COUNT CONFIG_EXAMPLE_KEEPALIVE_COUNT

static const char *LOGGING_TAG = "example";



static int listen_sock = -1;
static int sock = -1;

static void do_retransmit(const int sock) {
  int len;
  char rx_buffer[128];

  do {
    len = recv(sock, rx_buffer, sizeof(rx_buffer) - 1, 0);
    ESP_LOGI(LOGGING_TAG, "Received %d bytes", len);
    if (len < 0) {
      ESP_LOGE(LOGGING_TAG, "Error occurred during receiving: errno %d", errno);
    } else if (len == 0) {
      ESP_LOGW(LOGGING_TAG, "Connection closed");
    } else {
      rx_buffer[len] = 0;  // Null-terminate whatever is received and treat it like a string
      ESP_LOGI(LOGGING_TAG, "Received %d bytes: %s", len, rx_buffer);
       
      // send() can return less bytes than supplied length.
      // Walk-around for robust implementation.
      int to_write = len;
      while (to_write > 0) {
        int written = send(sock, rx_buffer + (len - to_write), to_write, 0);
        ESP_LOGI(LOGGING_TAG, "Sent %d bytes: %s", to_write, rx_buffer + (len - to_write));
        if (written < 0) {
          ESP_LOGE(LOGGING_TAG, "Error occurred during sending: errno %d", errno);
          // Failed to retransmit, giving up
          return;
        }
        to_write -= written;
      }
    }
  } while (len > 0);
}

static void tcp_server_task(void *pvParameters) {
  char addr_str[128];
  int addr_family = (int)pvParameters;
  int ip_protocol = 0;
  int keepAlive = 1;
  int keepIdle = KEEPALIVE_IDLE;
  int keepInterval = KEEPALIVE_INTERVAL;
  int keepCount = KEEPALIVE_COUNT;
  struct sockaddr_storage dest_addr;

  if (addr_family == AF_INET) {
    struct sockaddr_in *dest_addr_ip4 = (struct sockaddr_in *)&dest_addr;
    dest_addr_ip4->sin_addr.s_addr = htonl(INADDR_ANY);
    dest_addr_ip4->sin_family = AF_INET;
    dest_addr_ip4->sin_port = htons(PORT);
    ip_protocol = IPPROTO_IP;
  }

  listen_sock = socket(addr_family, SOCK_STREAM, ip_protocol);
  if (listen_sock < 0) {
    ESP_LOGE(LOGGING_TAG, "Unable to create socket: errno %d", errno);
    shutdown(listen_sock,0);
    close(listen_sock);
    vTaskDelete(NULL);
    return;
  }
  int opt = 1;
  setsockopt(listen_sock, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

  ESP_LOGI(LOGGING_TAG, "Socket created");

  int err = bind(listen_sock, (struct sockaddr *)&dest_addr, sizeof(dest_addr));
  if (err != 0) {
    ESP_LOGE(LOGGING_TAG, "Socket unable to bind: errno %d", errno);
    ESP_LOGE(LOGGING_TAG, "IPPROTO: %d", addr_family);
    shutdown(listen_sock,0);
    close(listen_sock);
    vTaskDelete(NULL);
    return;
  }
  ESP_LOGI(LOGGING_TAG, "Socket bound, port %d", PORT);

  err = listen(listen_sock, 1);
  if (err != 0) {
    ESP_LOGE(LOGGING_TAG, "Error occurred during listen: errno %d", errno);
    shutdown(listen_sock,0);
    close(listen_sock);
    vTaskDelete(NULL);
    return;
  }

  ESP_LOGI(LOGGING_TAG, "Socket listening");

  struct sockaddr_storage source_addr;  // Large enough for both IPv4 or IPv6
  socklen_t addr_len = sizeof(source_addr);
  sock = accept(listen_sock, (struct sockaddr *)&source_addr, &addr_len);
  if (sock < 0) {
    ESP_LOGE(LOGGING_TAG, "Unable to accept connection: errno %d", errno);
    shutdown(sock,0);
    close(sock);
    shutdown(listen_sock,0);
    close(listen_sock); 
    vTaskDelete(NULL);
    return;
  }

  // Set tcp keepalive option
  setsockopt(sock, SOL_SOCKET, SO_KEEPALIVE, &keepAlive, sizeof(int));
  setsockopt(sock, IPPROTO_TCP, TCP_KEEPIDLE, &keepIdle, sizeof(int));
  setsockopt(sock, IPPROTO_TCP, TCP_KEEPINTVL, &keepInterval, sizeof(int));
  setsockopt(sock, IPPROTO_TCP, TCP_KEEPCNT, &keepCount, sizeof(int));
  // Convert ip address to string
  if (source_addr.ss_family == PF_INET) {
    inet_ntoa_r(((struct sockaddr_in *)&source_addr)->sin_addr, addr_str, sizeof(addr_str) - 1);
  }
  ESP_LOGI(LOGGING_TAG, "Socket accepted ip address: %s", addr_str);

  do_retransmit(sock);

  if (listen_sock > 0) {
    shutdown(listen_sock, 0);
    close(listen_sock);
  }
  ESP_LOGI(LOGGING_TAG, "Server listen socket deleted");
  if (sock > 0) {
    shutdown(sock, 0);
    close(sock);
  }
  ESP_LOGI(LOGGING_TAG, "Server socket deleted");
  vTaskDelete(NULL);
}

// static TaskHandle_t server_task_handle = NULL;

void init_server(ServerPtr server_ptr) {
  server_ptr->listen_sock = -1;
  server_ptr->sock = -1;
  server_ptr->server_task_handle = NULL;
}

void create_server(ServerPtr server_ptr) {
  // xTaskCreate(tcp_server_task, "tcp_server", 4096, (void *)AF_INET, 5, &server_task_handle);
  // volatile eTaskState state = eTaskGetState(server_task_handle);
  // ESP_LOGI(LOGGING_TAG, "Task state: %d", state);
  xTaskCreatePinnedToCore(tcp_server_task, "tcp_server", 4096, (void *)AF_INET, 5, &server_ptr->server_task_handle, 1);
  volatile eTaskState state = eTaskGetState(server_ptr->server_task_handle);
  ESP_LOGI(LOGGING_TAG, "Task state: %d", state);
}

void delete_server(ServerPtr server_ptr) {
  volatile eTaskState state = eTaskGetState(server_ptr->server_task_handle);
  ESP_LOGI(LOGGING_TAG, "Task state: %d", state);
  ESP_LOGI(LOGGING_TAG, "Shutting down server socket");
  ESP_LOGI(LOGGING_TAG, "listen_sock: %d", server_ptr->listen_sock);
  ESP_LOGI(LOGGING_TAG, "sock: %d", server_ptr->sock);
  if(state == eRunning) {
    vTaskDelete(server_ptr->server_task_handle);
  }
  if (server_ptr->listen_sock > 0) {
    shutdown(server_ptr->listen_sock, 0);
    close(server_ptr->listen_sock);
  }
  ESP_LOGI(LOGGING_TAG, "Server listen socket deleted");
  if (server_ptr->sock > 0) {
    shutdown(server_ptr->sock, 0);
    close(server_ptr->sock);
  }
  ESP_LOGI(LOGGING_TAG, "Server socket deleted");
  ESP_LOGI(LOGGING_TAG, "Server socket deleted");
  state = eTaskGetState(server_ptr->server_task_handle);
  ESP_LOGI(LOGGING_TAG, "Task state: %d", state);
}