/* ************************************************************************** */
/*                                                                            */
/*                                                              ++            */
/*   dns_log_packet.c                                          +**+   +*  *   */
/*                                                             ##%#*###*+++   */
/*   By: aboyreau <bnzlvosnb@mozmail.com>                     +**+ -- ##+     */
/*                                                            # *   *. #*     */
/*   Created: 2025/03/02 10:55:23 by aboyreau          **+*+  * -_._-   #+    */
/*   Updated: 2025/03/02 12:27:46 by aboyreau          +#-.-*  +         *    */
/*                                                     *-.. *   ++       #    */
/* ************************************************************************** */

#include "dns.h"
#include "pstring.h"

#include <sys/unistd.h>

void dns_log_packet(struct dns_packet *packet, uint16_t packetsize)
{
	write(1, "HEADER:\n", 8);
	dns_log_header(&packet->header);

	write(1, "QUESTIONS:\n", 11);
	uint16_t index = DNS_HEADER_BYTE_SIZE;
	for (uint16_t i = 0; i < packet->header.qdcount; i++)
	{
		char			   *question_address = (char *) packet + index;
		struct dns_question q =
			dns_parse_question((void *) question_address, packetsize - index);
		dns_log_question(q);
		index += pstr8_len(q.qname) + 4;
		free(q.qname);
	}

	write(1, "ANSWERS:\n", 9);
	for (uint16_t i = 0; i < packet->header.ancount; i++)
	{
		char			   *question_address = (char *) packet + index;
		struct dns_question q =
			dns_parse_question((void *) question_address, packetsize - index);
		dns_log_question(q);
		index += pstr8_len(q.qname) + 9;
		free(q.qname);
	}

	// write(1, "NAME SERVERS:\n", 14);
	// for (uint16_t i = 0; i < packet->header.nscount; i++)
	// {
	// 	char			   *question_address = (char *) packet + index;
	// 	struct dns_question q =
	// 		dns_parse_question((void *) question_address, packetsize - index);
	// 	dns_log_question(q);
	// 	index += pstr8_len(q.qname) + 9;
	//	free(q.qname);
	// }

	// write(1, "ADDITIONAL RESSOURCES:\n", 23);
	// for (uint16_t i = 0; i < packet->header.arcount; i++)
	// {
	// 	char			   *question_address = (char *) packet + index;
	// 	struct dns_question q =
	// 		dns_parse_question((void *) question_address, packetsize - index);
	// 	dns_log_question(q);
	// 	index += pstr8_len(q.qname) + 9;
	//	free(q.qname);
	// }
}
