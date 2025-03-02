/* ************************************************************************** */
/*                                                                            */
/*                                                              ++            */
/*   dns_parse_question.c                                      +**+   +*  *   */
/*                                                             ##%#*###*+++   */
/*   By: aboyreau <bnzlvosnb@mozmail.com>                     +**+ -- ##+     */
/*                                                            # *   *. #*     */
/*   Created: 2025/03/02 11:06:34 by aboyreau          **+*+  * -_._-   #+    */
/*   Updated: 2025/03/02 12:23:05 by aboyreau          +#-.-*  +         *    */
/*                                                     *-.. *   ++       #    */
/* ************************************************************************** */

#include "dns.h"

#include <cc.h>
#include <string.h>
#include <sys/unistd.h>

struct dns_question dns_parse_question(pstr8_t question, size_t packetlength)
{
	pstr8_t dns = dns_read_labels(question, packetlength);

	uint16_t type;
	memcpy(&type, (char *) question + pstr8_len(dns) + 2, sizeof(uint16_t));
	type = ntohs(type);

	uint16_t class;
	memcpy(
		&class,
		(char *) question + pstr8_len(dns) + 2 + sizeof(type),
		sizeof(uint16_t)
	);
	class = ntohs(class);

	// clang-format off
	return (struct dns_question) {
		.qname = dns,
		.qtype = type,
		.qclass = class
	};
	// clang-format on
}
