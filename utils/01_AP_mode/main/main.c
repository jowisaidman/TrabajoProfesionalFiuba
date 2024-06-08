/*  WiFi softAP Example

   This example code is in the Public Domain (or CC0 licensed, at your option.)

   Unless required by applicable law or agreed to in writing, this
   software is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
   CONDITIONS OF ANY KIND, either express or implied.
*/
#include <string.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_mac.h"
#include "esp_wifi.h"
#include "esp_event.h"
#include "esp_log.h"
#include "nvs_flash.h"

#include "lwip/err.h"
#include "lwip/sys.h"
#include "lwip/sockets.h"
#include <lwip/netdb.h>

#include <string.h>
#include <sys/param.h>
#include "freertos/task.h"
#include "freertos/event_groups.h"
#include "esp_system.h"
#include "esp_event.h"
#include "esp_log.h"
#include "nvs_flash.h"
#include "esp_netif.h"

/* The examples use WiFi configuration that you can set via project configuration menu.

   If you'd rather not, just change the below entries to strings with
   the config you want - ie #define EXAMPLE_WIFI_SSID "mywifissid"
*/
#define EXAMPLE_ESP_WIFI_SSID CONFIG_ESP_WIFI_SSID
#define EXAMPLE_ESP_WIFI_PASS ""
#define EXAMPLE_ESP_WIFI_CHANNEL CONFIG_ESP_WIFI_CHANNEL
#define EXAMPLE_MAX_STA_CONN 2

static const char *TAG = "wifi softAP";

struct access_point_t {
    wifi_config_t wifi_config;
};

#ifdef CONFIG_EXAMPLE_SOCKET_IP_INPUT_STDIN
#include "addr_from_stdin.h"
#endif

#if defined(CONFIG_EXAMPLE_IPV4)
#define HOST_IP_ADDR CONFIG_EXAMPLE_IPV4_ADDR
#elif defined(CONFIG_EXAMPLE_IPV6)
#define HOST_IP_ADDR CONFIG_EXAMPLE_IPV6_ADDR
#else
#define HOST_IP_ADDR ""
#endif

#define PORT 3333

static const char *payload = "Message from ESP32 ";


// static void udp_client_task(void *pvParameters)
// {
//     char rx_buffer[128];
//     char host_ip[] = HOST_IP_ADDR;
//     int addr_family = 0;
//     int ip_protocol = 0;

//     while (1) {

// #if defined(CONFIG_EXAMPLE_IPV4)
//         struct sockaddr_in dest_addr;
//         dest_addr.sin_addr.s_addr = inet_addr(HOST_IP_ADDR);
//         dest_addr.sin_family = AF_INET;
//         dest_addr.sin_port = htons(PORT);
//         addr_family = AF_INET;
//         ip_protocol = IPPROTO_IP;
// #elif defined(CONFIG_EXAMPLE_IPV6)
//         struct sockaddr_in6 dest_addr = { 0 };
//         inet6_aton(HOST_IP_ADDR, &dest_addr.sin6_addr);
//         dest_addr.sin6_family = AF_INET6;
//         dest_addr.sin6_port = htons(PORT);
//         dest_addr.sin6_scope_id = esp_netif_get_netif_impl_index(EXAMPLE_INTERFACE);
//         addr_family = AF_INET6;
//         ip_protocol = IPPROTO_IPV6;
// #elif defined(CONFIG_EXAMPLE_SOCKET_IP_INPUT_STDIN)
//         struct sockaddr_storage dest_addr = { 0 };
//         ESP_ERROR_CHECK(get_addr_from_stdin(PORT, SOCK_DGRAM, &ip_protocol, &addr_family, &dest_addr));
// #endif

//         int sock = socket(addr_family, SOCK_DGRAM, ip_protocol);
//         if (sock < 0) {
//             ESP_LOGE(TAG, "Unable to create socket: errno %d", errno);
//             break;
//         }

//         // Set timeout
//         struct timeval timeout;
//         timeout.tv_sec = 10;
//         timeout.tv_usec = 0;
//         setsockopt (sock, SOL_SOCKET, SO_RCVTIMEO, &timeout, sizeof timeout);

//         ESP_LOGI(TAG, "Socket created, sending to %s:%d", HOST_IP_ADDR, PORT);

//         while (1) {

//             int err = sendto(sock, payload, strlen(payload), 0, (struct sockaddr *)&dest_addr, sizeof(dest_addr));
//             if (err < 0) {
//                 ESP_LOGE(TAG, "Error occurred during sending: errno %d", errno);
//                 break;
//             }
//             ESP_LOGI(TAG, "Message sent");

//             struct sockaddr_storage source_addr; // Large enough for both IPv4 or IPv6
//             socklen_t socklen = sizeof(source_addr);
//             int len = recvfrom(sock, rx_buffer, sizeof(rx_buffer) - 1, 0, (struct sockaddr *)&source_addr, &socklen);

//             // Error occurred during receiving
//             if (len < 0) {
//                 ESP_LOGE(TAG, "recvfrom failed: errno %d", errno);
//                 break;
//             }
//             // Data received
//             else {
//                 rx_buffer[len] = 0; // Null-terminate whatever we received and treat like a string
//                 ESP_LOGI(TAG, "Received %d bytes from %s:", len, host_ip);
//                 ESP_LOGI(TAG, "%s", rx_buffer);
//                 if (strncmp(rx_buffer, "OK: ", 4) == 0) {
//                     ESP_LOGI(TAG, "Received expected message, reconnecting");
//                     break;
//                 }
//             }

//             vTaskDelay(2000 / portTICK_PERIOD_MS);
//         }

//         if (sock != -1) {
//             ESP_LOGE(TAG, "Shutting down socket and restarting...");
//             shutdown(sock, 0);
//             close(sock);
//         }
//     }
//     vTaskDelete(NULL);
// }


static void wifi_event_handler(void *arg, esp_event_base_t event_base,
                               int32_t event_id, void *event_data)
{
    if (event_id == WIFI_EVENT_AP_STACONNECTED)
    {
        wifi_event_ap_staconnected_t *event = (wifi_event_ap_staconnected_t *)event_data;
        ESP_LOGI(TAG, "station " MACSTR " join, AID=%d",
                 MAC2STR(event->mac), event->aid);
    }
    else if (event_id == WIFI_EVENT_AP_STADISCONNECTED)
    {
        wifi_event_ap_stadisconnected_t *event = (wifi_event_ap_stadisconnected_t *)event_data;
        ESP_LOGI(TAG, "station " MACSTR " leave, AID=%d",
                 MAC2STR(event->mac), event->aid);
    } else if (event_id == IP_EVENT_AP_STAIPASSIGNED) {
        ESP_LOGI(TAG, "Se assigno IP");
        // Print the IP assigned to the station and the MAC address of the station
        ip_event_ap_staipassigned_t *event = (ip_event_ap_staipassigned_t *)event_data;
        ESP_LOGI(TAG, "station ip:" IPSTR ", mac:" MACSTR "",
                 IP2STR(&event->ip), MAC2STR(event->mac));
        // xTaskCreate(udp_client_task, "udp_client", 4096, NULL, 5, NULL);

    }
}

void wifi_init_softap(void) {

    struct access_point_t ap;
    memset(&ap, 0, sizeof(ap));
    wifi_config_t wifi_config = {
        .ap = {
            .ssid = EXAMPLE_ESP_WIFI_SSID,
            .ssid_len = strlen(EXAMPLE_ESP_WIFI_SSID),
            .channel = EXAMPLE_ESP_WIFI_CHANNEL,
            .authmode = WIFI_AUTH_OPEN,
            .max_connection = EXAMPLE_MAX_STA_CONN,
            .pmf_cfg = {
                .required = true,
            },

        },
    };

    memcpy(&ap.wifi_config, &wifi_config, sizeof(wifi_config));

    ESP_ERROR_CHECK(esp_netif_init());
    ESP_ERROR_CHECK(esp_event_loop_create_default());
    esp_netif_create_default_wifi_ap();

    wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();
    ESP_ERROR_CHECK(esp_wifi_init(&cfg));

    esp_event_handler_instance_t instance_any_id;
    esp_event_handler_instance_t instance_got_ip;
    ESP_ERROR_CHECK(esp_event_handler_instance_register(WIFI_EVENT,
                                                        ESP_EVENT_ANY_ID,
                                                        &wifi_event_handler,
                                                        NULL,
                                                        &instance_any_id));

    ESP_ERROR_CHECK(esp_event_handler_instance_register(IP_EVENT,
                                                        IP_EVENT_AP_STAIPASSIGNED,
                                                        &wifi_event_handler,
                                                        NULL,
                                                        &instance_got_ip));

    

    ESP_ERROR_CHECK(esp_wifi_set_mode(WIFI_MODE_AP));
    ESP_ERROR_CHECK(esp_wifi_set_config(WIFI_IF_AP, &(ap.wifi_config)));
    ESP_ERROR_CHECK(esp_wifi_start());

    ESP_LOGI(TAG, "wifi_init_softap finished. SSID:%s password:%s channel:%d",
             EXAMPLE_ESP_WIFI_SSID, EXAMPLE_ESP_WIFI_PASS, EXAMPLE_ESP_WIFI_CHANNEL);
}

void app_main(void)
{
    // Initialize NVS
    esp_err_t ret = nvs_flash_init();
    if (ret == ESP_ERR_NVS_NO_FREE_PAGES || ret == ESP_ERR_NVS_NEW_VERSION_FOUND)
    {
        ESP_ERROR_CHECK(nvs_flash_erase());
        ret = nvs_flash_init();
    }
    ESP_ERROR_CHECK(ret);

    ESP_LOGI(TAG, "ESP_WIFI_MODE_AP");
    wifi_init_softap();
}
