/* ************************************************************************** */
/*                                                                            */
/*                                                              ++            */
/*   dns_log_questions.c                                       +**+   +*  *   */
/*                                                             ##%#*###*+++   */
/*   By: aboyreau <bnzlvosnb@mozmail.com>                     +**+ -- ##+     */
/*                                                            # *   *. #*     */
/*   Created: 2025/03/01 11:04:24 by aboyreau          **+*+  * -_._-   #+    */
/*   Updated: 2025/03/01 20:09:24 by aboyreau          +#-.-*  +         *    */
/*                                                     *-.. *   ++       #    */
/* ************************************************************************** */

#include "cc.h"
#include "dns.h"
#include "esp_log.h"
#include "pstring.h"

#include <string.h>
#include <sys/unistd.h>

static const char *dns_type(enum dns_type_e type)
{
	return dns_types[type];
}

static const char *dns_class(enum dns_class_e class)
{
	return dns_classes[class];
}

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
		.qname = question,
		.qtype = type,
		.qclass = class
	};
	// clang-format on
}

void dns_log_question(struct dns_question question)
{
	ESP_LOGI(TAG, "qname	%d	%d", question.qtype, question.qclass);

	write(1, question.qname + 1, pstr8_len(question.qname));
	write(1, "\t", 1);
	write(1, dns_type(question.qtype), 1);
	write(1, "\t", 1);
	write(1, dns_class(question.qclass), 2);
	write(1, "\t\n", 2);
}
