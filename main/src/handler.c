/* ************************************************************************** */
/*                                                                            */
/*                                                              ++            */
/*   handler.c                                                 +**+   +*  *   */
/*                                                             ##%#*###*+++   */
/*   By: aboyreau <bnzlvosnb@mozmail.com>                     +**+ -- ##+     */
/*                                                            # *   *. #*     */
/*   Created: 2025/02/19 12:37:58 by aboyreau          **+*+  * -_._-   #+    */
/*   Updated: 2025/03/01 19:48:14 by aboyreau          +#-.-*  +         *    */
/*                                                     *-.. *   ++       #    */
/* ************************************************************************** */

#include "cc.h"
#include "dns.h"
#include "esp_log.h"
#include "lwip/sockets.h"
#include "pstring.h"
#include "trie.h"

#include <lwip/netdb.h>
#include <string.h>
#include <sys/param.h>

#ifndef TAG
#define TAG "HANDLER"
#endif

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
	int				   messagelen = 0;
	struct dns_packet  packet;
	struct dns_header *header = &packet.header;

	static struct trie_s head = {0};

	messagelen = receive_packet(sock, source_addr, &packet);
	if (messagelen < 0)
	{
		ESP_LOGE(TAG, "Error occurred during receive: errno %d", errno);
		return -1;
	}
	dns_ntoh_header(header);

	// Update header
	header->qr		= 1;
	header->opcode	= 0;
	header->rd		= 0;
	header->ra		= 0;
	header->rcode	= 0;
	header->ancount = 0;

	uint16_t index = DNS_HEADER_BYTE_SIZE;
	for (uint16_t i = 0; i < header->qdcount; i++)
	{
		char *question_address = (char *) &packet + index;
		ESP_LOGI(TAG, "Reading from: %p", (void *) question_address);
		struct dns_question q =
			dns_parse_question((void *) question_address, messagelen - index);
		dns_log_question(q);

		int ip = (int) trie_get(&head, q.qname);
		if (ip == 0)
		{
			// TODO Recursive query
		}
		struct dns_rr an = {
			.type	  = htons(1),
			.class	  = htons(1),
			.ttl	  = htonl(0),
			.rdlength = htons(4),
			.rddata	  = (uint32_t) htonl(ip)
		};
		// Copy the DNS labels
		memcpy(packet.raw + index, question_address, pstr8_len(q.qname) + 2);
		index += pstr8_len(q.qname) + 2;
		memcpy(packet.raw + index, &an, sizeof(an));
		index += sizeof(uint32_t);
		header->ancount++;
	}

	// build the answer

	dns_ntoh_header(header);

	sendto(
		sock,
		&packet,
		512,
		// DNS_HEADER_BYTE_SIZE + pstr8_len(dns) + 5 + pstr8_len(dns) + 1 +
		// sizeof(struct dns_rr),
		0,
		(struct sockaddr *) source_addr,
		sizeof(*source_addr)
	);
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
