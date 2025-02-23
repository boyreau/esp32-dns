/* ************************************************************************** */
/*                                                                            */
/*                                                              ++            */
/*   dns.c                                                     +**+   +*  *   */
/*                                                             ##%#*###*+++   */
/*   By: aboyreau <bnzlvosnb@mozmail.com>                     +**+ -- ##+     */
/*                                                            # *   *. #*     */
/*   Created: 2025/02/17 22:58:38 by aboyreau          **+*+  * -_._-   #+    */
/*   Updated: 2025/02/22 19:33:55 by aboyreau          +#-.-*  +         *    */
/*                                                     *-.. *   ++       #    */
/* ************************************************************************** */

#include "cc.h"
#include "dns.h"
#include "esp_log.h"

const char *TAG = "DNS";

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
