/* ************************************************************************** */
/*                                                                            */
/*                                                              ++            */
/*   dns.c                                                     +**+   +*  *   */
/*                                                             ##%#*###*+++   */
/*   By: aboyreau <bnzlvosnb@mozmail.com>                     +**+ -- ##+     */
/*                                                            # *   *. #*     */
/*   Created: 2025/02/17 22:58:38 by aboyreau          **+*+  * -_._-   #+    */
/*   Updated: 2025/02/19 13:09:52 by aboyreau          +#-.-*  +         *    */
/*                                                     *-.. *   ++       #    */
/* ************************************************************************** */

#include "dns.h"

#include <stdio.h>

int dns_write_header(struct dns_header *header, char *response)
{
	// clang-format off
	return snprintf(
		response,
		HEADER_SIZE + 1, // +1 for extra '\0' at the end
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
