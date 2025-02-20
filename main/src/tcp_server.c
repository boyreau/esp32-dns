/* BSD Socket API Example

   This example code is in the Public Domain (or CC0 licensed, at your option.)

   Unless required by applicable law or agreed to in writing, this
   software is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
   CONDITIONS OF ANY KIND, either express or implied.
*/
#include "dns.h"
#include "esp_event.h"
#include "esp_log.h"
#include "esp_netif.h"
#include "lwip/sockets.h"
#include "nvs_flash.h"
#include "protocol_examples_common.h"
#include "server.h"

#include <lwip/netdb.h>
#include <stdio.h>
#include <string.h>
#include <sys/param.h>

#define PORT 53

static const char *TAG = "example";

static int create_socket(int addr_family, struct sockaddr_in6 *dest_addr)
{
	int					ip_protocol	  = IPPROTO_IP;
	struct sockaddr_in *dest_addr_ip4 = (struct sockaddr_in *) dest_addr;
	dest_addr_ip4->sin_addr.s_addr	  = htonl(INADDR_ANY);
	dest_addr_ip4->sin_family		  = AF_INET;
	dest_addr_ip4->sin_port			  = htons(PORT);

	return socket(addr_family, SOCK_DGRAM, ip_protocol);
}

static int bind_socket_to_port(int sock, struct sockaddr_in6 *dest_addr)
{
	int enable = 1;
	lwip_setsockopt(sock, IPPROTO_IP, IP_TTL, &(int) {60}, sizeof(int));
	lwip_setsockopt(sock, IPPROTO_IP, IP_PKTINFO, &enable, sizeof(enable));
	lwip_setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, &enable, sizeof(enable));
	lwip_setsockopt(sock, SOL_SOCKET, SO_REUSEPORT, &enable, sizeof(enable));

	return bind(sock, (struct sockaddr *) dest_addr, sizeof(*dest_addr));
}

static int server_setup(int addr_family, struct sockaddr_in6 *dest_addr) // NOLINT
{
	int sock = create_socket(addr_family, dest_addr);
	if (sock < 0)
	{
		ESP_LOGE(TAG, "Unable to create socket: errno %d", errno);
		return -1;
	}
	ESP_LOGI(TAG, "Socket created");

	int err = bind_socket_to_port(sock, dest_addr);
	if (err < 0)
	{
		ESP_LOGE(TAG, "Socket unable to bind: errno %d", errno);
		close(sock);
		return -1;
	}
	ESP_LOGI(TAG, "Socket bound, port %d", PORT);
	return sock;
}

static void udp_server_task(void *pvParameters)
{
	int					addr_family = (int) pvParameters;
	struct sockaddr_in6 dest_addr;

	while (1)
	{
		int						sock = server_setup(addr_family, &dest_addr);
		struct sockaddr_storage source_addr;

		int err = handle_request(sock, &source_addr);
		if (err < 0)
		{
			ESP_LOGE(TAG, "Shutting down socket and restarting...");
			shutdown(sock, 0);
			close(sock);
		}
	}
	vTaskDelete(NULL);
}

void app_main(void)
{
	ESP_ERROR_CHECK(nvs_flash_init());
	ESP_ERROR_CHECK(esp_netif_init());
	ESP_ERROR_CHECK(esp_event_loop_create_default());

	/* This helper function configures Wi-Fi or Ethernet, as selected in
	 * menuconfig. Read "Establishing Wi-Fi or Ethernet Connection" section in
	 * examples/protocols/README.md for more information about this function.
	 */
	ESP_ERROR_CHECK(example_connect());
	xTaskCreate(udp_server_task, "udp_server", 4096, (void *) AF_INET, 5, NULL);
}
