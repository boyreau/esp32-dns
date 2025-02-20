/* ************************************************************************** */
/*                                                                            */
/*                                                              ++            */
/*   server.h                                                  +**+   +*  *   */
/*                                                             ##%#*###*+++   */
/*   By: aboyreau <bnzlvosnb@mozmail.com>                     +**+ -- ##+     */
/*                                                            # *   *. #*     */
/*   Created: 2025/02/19 12:37:48 by aboyreau          **+*+  * -_._-   #+    */
/*   Updated: 2025/02/19 13:59:13 by aboyreau          +#-.-*  +         *    */
/*                                                     *-.. *   ++       #    */
/* ************************************************************************** */

#ifndef SERVER_H
#define SERVER_H

#include <lwip/netdb.h>

// int server_setup(int addr_family, struct sockaddr_in6 *dest_addr);
int handle_request(int sock, struct sockaddr_storage *source_addr);

#endif
