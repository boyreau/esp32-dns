/* ************************************************************************** */
/*                                                                            */
/*                                                              ++            */
/*   handler.h                                                 +**+   +*  *   */
/*                                                             ##%#*###*+++   */
/*   By: aboyreau <bnzlvosnb@mozmail.com>                     +**+ -- ##+     */
/*                                                            # *   *. #*     */
/*   Created: 2025/02/24 02:31:40 by aboyreau          **+*+  * -_._-   #+    */
/*   Updated: 2025/03/06 10:37:11 by aboyreau          +#-.-*  +         *    */
/*                                                     *-.. *   ++       #    */
/* ************************************************************************** */

#ifndef HANDLER_H
#define HANDLER_H

#include <lwip/netdb.h>

int handle_request(int sock, struct sockaddr_storage *source_addr, void *trie_head);

#endif
