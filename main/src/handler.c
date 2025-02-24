/* ************************************************************************** */
/*                                                                            */
/*                                                              ++            */
/*   handler.c                                                 +**+   +*  *   */
/*                                                             ##%#*###*+++   */
/*   By: aboyreau <bnzlvosnb@mozmail.com>                     +**+ -- ##+     */
/*                                                            # *   *. #*     */
/*   Created: 2025/02/19 12:37:58 by aboyreau          **+*+  * -_._-   #+    */
/*   Updated: 2025/02/24 03:34:54 by aboyreau          +#-.-*  +         *    */
/*                                                     *-.. *   ++       #    */
/* ************************************************************************** */

#include "dns.h"
#include "esp_log.h"
#include "llist.h"
#include "lwip/sockets.h"
#include "server.h"
#include "trie.h"

#include <lwip/netdb.h>
#include <stdio.h>
#include <string.h>
#include <sys/param.h>

static const char *TAG = __FILE__;

static int
receive_packet(int sock, struct sockaddr_storage *source_addr, void *packet)
{
	ESP_LOGI(TAG, "Dest: %p", (void *) packet);
	socklen_t socklen = sizeof(*source_addr);

	int len = recvfrom(
		sock,
		packet,
		DNS_PACKET_SIZE_MAX,
		0,
		(struct sockaddr *) source_addr,
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
		return -1;
	}
	return len;
}

static int receive_and_respond(int sock, struct sockaddr_storage *source_addr)
{
	int					  messagelen = 0;
	struct dns_packet	  packet;
	struct dns_header	 *header = (void *) &packet;
	struct dns_questions *questions =
		(void *) ((char *) &packet + DNS_HEADER_BYTE_SIZE);

	static struct trie_s head = {0};

	messagelen = receive_packet(sock, source_addr, &packet);
	if (messagelen < 0)
	{
		ESP_LOGE(TAG, "Error occurred during receive: errno %d", errno);
		return -1;
	}
	dns_ntoh_header(header);

	for (int i = 0; i < header->qdcount; i++)
	{
		pstr8_t dns = dns_read_labels((char *) questions, messagelen);
		int		ip	= (int) trie_get(&head, dns);
		if (ip == 0)
		{
			// TODO Recursive query
		}
		header->qr		= 1;
		header->opcode	= 0;
		header->rd		= 0;
		header->ra		= 0;
		header->qdcount = 1;
		header->ancount = 1;
		memcpy((char *) questions + pstr8_len(dns) + 1, dns, *dns + 1);
		// Build DNS answer and send it
		dns_ntoh_header(header);
		sendto(
			sock,
			header,
			512,
			0,
			(struct sockaddr *) source_addr,
			sizeof(*source_addr)
		);
	}
	return 0;
}

int handle_request(int sock, struct sockaddr_storage *source_addr)
{
	while (1)
	{
		if (receive_and_respond(sock, source_addr))
			return -1;
	}
}
