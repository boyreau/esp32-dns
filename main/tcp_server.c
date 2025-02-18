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

#include <lwip/netdb.h>
#include <stdio.h>
#include <string.h>
#include <sys/param.h>

#define PORT 53

static const char *TAG = "example";

static void server_retrieve_sender_ip(struct sockaddr_storage *source_addr)
{
	struct msghdr	msg;
	struct cmsghdr *cmsgtmp;
	u8_t			cmsg_buf[CMSG_SPACE(sizeof(struct in_pktinfo))];
	char			addr_str[128];

	msg.msg_control	   = cmsg_buf;
	msg.msg_controllen = sizeof(cmsg_buf);
	msg.msg_flags	   = 0;
	msg.msg_name	   = (struct sockaddr *) &source_addr;
	msg.msg_namelen	   = sizeof(*source_addr);

	// Get the sender's ip address as string
	if (source_addr->ss_family == PF_INET)
	{
		inet_ntoa_r(
			((struct sockaddr_in *) source_addr)->sin_addr,
			addr_str,
			sizeof(addr_str) - 1
		);
		for (cmsgtmp = CMSG_FIRSTHDR(&msg); cmsgtmp != NULL;
			 cmsgtmp = CMSG_NXTHDR(&msg, cmsgtmp))
		{
			if (cmsgtmp->cmsg_level == IPPROTO_IP &&
				cmsgtmp->cmsg_type == IP_PKTINFO)
			{
				struct in_pktinfo *pktinfo;
				pktinfo = (struct in_pktinfo *) CMSG_DATA(cmsgtmp);
				ESP_LOGI(TAG, "dest ip: %s", inet_ntoa(pktinfo->ipi_addr));
			}
		}
	}
}

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
	lwip_setsockopt(sock, IPPROTO_IP, IP_PKTINFO, &enable, sizeof(enable));
	// Set timeout
	struct timeval timeout;
	timeout.tv_sec	= 10;
	timeout.tv_usec = 0;
	setsockopt(sock, SOL_SOCKET, SO_RCVTIMEO, &timeout, sizeof timeout);

	return bind(sock, (struct sockaddr *) dest_addr, sizeof(*dest_addr));
}

static int setup_server(int addr_family, struct sockaddr_in6 *dest_addr)
{
	int sock = create_socket(addr_family, dest_addr);
	if (sock < 0)
	{
		ESP_LOGE(TAG, "Unable to create socket: errno %d", errno);
		return -1;
	}
	ESP_LOGI(TAG, "Socket created");

	int err = bind_socket_to_port(sock, &dest_addr);
	if (err < 0)
	{
		ESP_LOGE(TAG, "Socket unable to bind: errno %d", errno);
		close(sock);
		return -1
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
		int						sock = setup_server();
		struct sockaddr_storage source_addr;
		socklen_t				socklen = sizeof(source_addr);

		while (1)
		{
			ESP_LOGI(TAG, "Waiting for DNS query header");

			// Receive DNS request header
			struct dns_header header;
			int				  len = recvfrom(
				  sock,
				  &header,
				  sizeof(header),
				  0,
				  (struct sockaddr *) &source_addr,
				  &socklen
			  );
			if (len < 0)
			{
				ESP_LOGE(
					TAG,
					"Error occurred during recvfrom: errno %d: %s",
					errno,
					strerror(errno)
				);
				close(sock);
				sock = -1;
				break;
			}
			server_retrieve_sender_ip(&source_addr);

			ESP_LOGI(
				TAG,
				"\nid: %016X\n"	 // id
				"qr: 1\n"		 // qr		- query or response
				"opcode: %04X\n" // opcode	- query type
				"aa: %01X\n"	 // aa		- authoritative answer
				"tc: %01X\n"	 // tc 		- truncated
				"rd: %01X\n"	 // rd		- recursion desired
				"ra: 0\n"		 // ra		- recursion available
				"z: %03X\n"		 // z		- reserved for future use
				"rcode: %04X\n"	 // rcode	- response code
				"qdcount: %u\n"	 // qdcount	- how many questions ?
				"ancount: %u\n"	 // ancount	- how many answers ?
				"nscount: %u\n"	 // nscount	- how many name server resource
								 // record in the authority record session
				"arcount: %u\n", // arcount	- how many resources record in
								 // the additionnal record section
				header.id,
				header.opcode,
				header.aa,
				header.tc,
				header.rd,
				header.z,
				header.rcode,
				header.qdcount,
				header.ancount,
				header.nscount,
				header.arcount
			);

			// Change header to match response
			header.qr	 = 0;
			header.aa	 = 0;
			header.rcode = 2;

			char temp[HEADER_SIZE + 1] = {0};
			dns_write_header(&header, temp);

			int err = sendto(
				sock,
				&header,
				sizeof(header),
				0,
				(struct sockaddr *) &source_addr,
				sizeof(source_addr)
			);
			if (err < 0)
			{
				ESP_LOGE(TAG, "Error occurred during sending: errno %d", errno);
				break;
			}

			// TODO: read dns body and answer
			// char response[512];

			// dns_write_header(&pkt.header, response);
			// dns_write_body(*pkt.data, response + 96);
			// free(pkt.data);
			// pkt.data	  = NULL;
			// pkt.data_size = 0;
			// memset(&pkt.header, 0, sizeof(pkt.header));

			// // send() can return less bytes than supplied length.
			// // Walk-around for robust implementation.
			// int to_write = strlen(response);
		}
		if (sock != -1)
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
