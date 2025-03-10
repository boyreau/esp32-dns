/* ************************************************************************** */
/*                                                                            */
/*                                                              ++            */
/*   server.h                                                  +**+   +*  *   */
/*                                                             ##%#*###*+++   */
/*   By: aboyreau <bnzlvosnb@mozmail.com>                     +**+ -- ##+     */
/*                                                            # *   *. #*     */
/*   Created: 2025/02/19 12:37:48 by aboyreau          **+*+  * -_._-   #+    */
/*   Updated: 2025/03/08 09:22:30 by aboyreau          +#-.-*  +         *    */
/*                                                     *-.. *   ++       #    */
/* ************************************************************************** */

#ifndef SERVER_H
#define SERVER_H

#ifndef TAG
#define TAG "SERVER"
#endif

// DNS
#include <lwip/netdb.h>
int	 server_setup(int addr_family, struct sockaddr_in6 *dest_addr);
int	 create_socket(int addr_family, struct sockaddr_in6 *dest_addr);
int	 bind_socket_to_port(int sock, struct sockaddr_in6 *dest_addr);
void udp_server_task(void *pvParameters);

// HTTP

#endif
