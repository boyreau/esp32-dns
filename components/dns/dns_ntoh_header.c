/* ************************************************************************** */
/*                                                                            */
/*                                                              ++            */
/*   dns_ntoh_header.c                                         +**+   +*  *   */
/*                                                             ##%#*###*+++   */
/*   By: aboyreau <bnzlvosnb@mozmail.com>                     +**+ -- ##+     */
/*                                                            # *   *. #*     */
/*   Created: 2025/03/01 18:10:32 by aboyreau          **+*+  * -_._-   #+    */
/*   Updated: 2025/03/01 18:10:55 by aboyreau          +#-.-*  +         *    */
/*                                                     *-.. *   ++       #    */
/* ************************************************************************** */

#include "cc.h"
#include "dns.h"

void dns_ntoh_header(struct dns_header *header)
{
	header->id		= ntohs(header->id);
	header->flags	= ntohs(header->flags);
	header->qdcount = ntohs(header->qdcount);
	header->ancount = ntohs(header->ancount);
	header->nscount = ntohs(header->nscount);
	header->arcount = ntohs(header->arcount);
}
