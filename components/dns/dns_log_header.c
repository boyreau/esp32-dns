/* ************************************************************************** */
/*                                                                            */
/*                                                              ++            */
/*   dns_log_header.c                                          +**+   +*  *   */
/*                                                             ##%#*###*+++   */
/*   By: aboyreau <bnzlvosnb@mozmail.com>                     +**+ -- ##+     */
/*                                                            # *   *. #*     */
/*   Created: 2025/03/01 18:09:20 by aboyreau          **+*+  * -_._-   #+    */
/*   Updated: 2025/03/01 18:33:15 by aboyreau          +#-.-*  +         *    */
/*                                                     *-.. *   ++       #    */
/* ************************************************************************** */

#include "dns.h"
#include "esp_log.h"

void dns_log_header(struct dns_header *header)
{
	ESP_LOGI(
		TAG,
		"\nid: %016X\n"	 // id
		"qr: %d\n"		 // qr		- query or response
		"opcode: %04X\n" // opcode	- query type
		"aa: %01X\n"	 // aa		- authoritative answer
		"tc: %01X\n"	 // tc 		- truncated
		"rd: %01X\n"	 // rd		- recursion desired
		"ra: %d\n"		 // ra		- recursion available
		"z: %03X\n"		 // z		- reserved for future use
		"rcode: %04X\n"	 // rcode	- response code
		"qdcount: %u\n"	 // qdcount	- how many questions ?
		"ancount: %u\n"	 // ancount	- how many answers ?
		"nscount: %u\n"	 // nscount	- how many name server resource
						 // record in the authority record session
		"arcount: %u\n", // arcount	- how many resources record in
						 // the additionnal record section

		header->id,
		header->qr,
		header->opcode,
		header->aa,
		header->tc,
		header->rd,
		header->ra,
		header->z,
		header->rcode,
		header->qdcount,
		header->ancount,
		header->nscount,
		header->arcount
	);
}
