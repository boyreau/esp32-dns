/* ************************************************************************** */
/*                                                                            */
/*                                                              ++            */
/*   handler.c                                                 +**+   +*  *   */
/*                                                             ##%#*###*+++   */
/*   By: aboyreau <bnzlvosnb@mozmail.com>                     +**+ -- ##+     */
/*                                                            # *   *. #*     */
/*   Created: 2025/02/19 12:37:58 by aboyreau          **+*+  * -_._-   #+    */
/*   Updated: 2025/03/02 13:12:34 by aboyreau          +#-.-*  +         *    */
/*                                                     *-.. *   ++       #    */
/* ************************************************************************** */

#include "dns.h"
#include "pstring.h"
#include "trie.h"

#include <cc.h>
#include <esp_log.h>
#include <lwip/netdb.h>
#include <lwip/sockets.h>
#include <string.h>
#include <sys/param.h>

#ifndef TAG
#define TAG "HANDLER"
#endif

static int
receive_packet(int sock, struct sockaddr_storage *source_addr, void *packet)
{
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
	trie_add(&head, "\nsciodog.fr", 603187885);

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
	header->arcount = 0;

	char	*rr		  = malloc(DNS_PACKET_SIZE_MAX);
	uint16_t rr_index = 0;
	uint16_t index	  = DNS_HEADER_BYTE_SIZE;
	for (uint16_t i = 0; i < header->qdcount; i++)
	{
		char			   *question_address = (char *) &packet + index;
		struct dns_question q =
			dns_parse_question((void *) question_address, messagelen - index);
		dns_log_question(q);

		uint32_t ip = (uint32_t) trie_get(&head, q.qname);
		if (ip == 0)
		{
			// TODO Recursive query
			free(q.qname);
			continue;
		}
		struct dns_rr an = {
			.type	  = htons(1),
			.class	  = htons(1),
			.ttl	  = htonl(0),
			.rdlength = htons(4),
			.rddata	  = htonl(ip)
		};
		// Copy the DNS labels (+2: leading size byte and trailing empty byte)
		memcpy(rr + rr_index, question_address, pstr8_len(q.qname) + 2);
		rr_index += pstr8_len(q.qname) + 2;
		index += pstr8_len(q.qname) + 2;

		// Copy the resource record informations
		memcpy(rr + rr_index, &an, sizeof(an));
		rr_index += sizeof(an);

		index += 4; // qtype + qclass size

		free(q.qname);
		header->ancount++;
	}

	dns_ntoh_header(header);
	memcpy(packet.raw + index, rr, rr_index);

	sendto(
		sock,
		&packet,
		index + rr_index,
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
