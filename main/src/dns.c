/* ************************************************************************** */
/*                                                                            */
/*                                                              ++            */
/*   dns.c                                                     +**+   +*  *   */
/*                                                             ##%#*###*+++   */
/*   By: aboyreau <bnzlvosnb@mozmail.com>                     +**+ -- ##+     */
/*                                                            # *   *. #*     */
/*   Created: 2025/02/17 22:58:38 by aboyreau          **+*+  * -_._-   #+    */
/*   Updated: 2025/02/20 16:32:16 by aboyreau          +#-.-*  +         *    */
/*                                                     *-.. *   ++       #    */
/* ************************************************************************** */

#include "cc.h"
#include "dns.h"
#include "esp_log.h"
#include "llist.h"

#include <stdio.h>
#include <string.h>

static const char *TAG = __FILE__;

int dns_write_header(struct dns_header *header, char *response)
{
	// clang-format off
	return snprintf(
		response,
		DNS_HEADER_SIZE + 1, // +1 for extra '\0' at the end
		"%016X" // id
		"1"		// qr		- query or response
		"%04X"	// opcode	- query type
		"%01X"	// aa		- authoritative answer
		"%01X"	// tc 		- truncated
		"%01X"	// rd		- recursion desired
		"0"	// ra		- recursion available
		"%03X"	// z		- reserved for future use
		"%04X" 	// rcode	- response code
		"%016X" // qdcount	- how many questions ?
		"%016X"	// ancount	- how many answers ?
		"%016X" // nscount	- how many name server resource record in the authority record session
		"%016X",// arcount	- how many resources record in the additionnal record section
		header->id,
		header->opcode,
		header->aa,
		header->tc,
		header->rd,
		header->z,
		header->rcode,
		header->qdcount,
		header->ancount,
		header->nscount,
		header->arcount
	);
	// clang-format on
}

int dns_read_header(struct dns_header *header, FILE *inputstream)
{
	size_t red = fread(header, sizeof(*header), 1, inputstream);
	if (red == 0 && ferror(inputstream))
		return -1;
	return 0;
}

int dns_read_labels(label_list **labels, char *message, size_t messagelen)
{
	uint8_t qsize = 0;
	size_t	index = 0;

	while (1)
	{
		if (index - 1 > messagelen)
			break;
		qsize = *(uint8_t *) (message + index);
		index++;

		if (qsize == 0)
			break;
		ESP_LOGI(TAG, "Size: %d", qsize);

		char *question = malloc(qsize + 1);
		memcpy(question, message + index, qsize);
		question[qsize] = 0;

		label_list *new = malloc(sizeof(*new));
		new->label		= question;
		new->next		= NULL;

		lst_add_back(labels, new);
		index += qsize;
	}
	ESP_LOGI(TAG, "DNS labels parsed");
	return 0;
}

void dns_log_header(struct dns_header *header)
{
	ESP_LOGD(
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
		header->id,
		header->opcode,
		header->aa,
		header->tc,
		header->rd,
		header->z,
		header->rcode,
		header->qdcount,
		header->ancount,
		header->nscount,
		header->arcount
	);
}

void dns_ntoh_header(struct dns_header *header)
{
	header->id		= ntohs(header->id);
	header->flags	= ntohs(header->flags);
	header->qdcount = ntohs(header->qdcount);
	header->ancount = ntohs(header->ancount);
	header->nscount = ntohs(header->nscount);
	header->arcount = ntohs(header->arcount);
}
