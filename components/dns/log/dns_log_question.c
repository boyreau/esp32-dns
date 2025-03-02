/* ************************************************************************** */
/*                                                                            */
/*                                                              ++            */
/*   dns_log_question.c                                        +**+   +*  *   */
/*                                                             ##%#*###*+++   */
/*   By: aboyreau <bnzlvosnb@mozmail.com>                     +**+ -- ##+     */
/*                                                            # *   *. #*     */
/*   Created: 2025/03/01 11:04:24 by aboyreau          **+*+  * -_._-   #+    */
/*   Updated: 2025/03/02 11:07:03 by aboyreau          +#-.-*  +         *    */
/*                                                     *-.. *   ++       #    */
/* ************************************************************************** */

#include "dns.h"
#include "pstring.h"

#include <sys/unistd.h>

static const char *dns_type(enum dns_type_e type)
{
	return dns_types[type];
}

static const char *dns_class(enum dns_class_e class)
{
	return dns_classes[class];
}

void dns_log_question(struct dns_question question)
{
	write(1, question.qname + 1, pstr8_len(question.qname));
	write(1, "\t", 1);
	write(1, dns_type(question.qtype), 1);
	write(1, "\t", 1);
	write(1, dns_class(question.qclass), 2);
	write(1, "\t\n", 2);
}
