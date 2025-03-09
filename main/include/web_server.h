/* ************************************************************************** */
/*                                                                            */
/*                                                              ++            */
/*   web_server.h                                              +**+   +*  *   */
/*                                                             ##%#*###*+++   */
/*   By: aboyreau <bnzlvosnb@mozmail.com>                     +**+ -- ##+     */
/*                                                            # *   *. #*     */
/*   Created: 2025/03/08 09:22:15 by aboyreau          **+*+  * -_._-   #+    */
/*   Updated: 2025/03/08 10:26:19 by aboyreau          +#-.-*  +         *    */
/*                                                     *-.. *   ++       #    */
/* ************************************************************************** */

#ifndef WEB_SERVER_H
#define WEB_SERVER_H

#ifndef TAG
#define TAG "HTTP_SERVER"
#endif

#include "esp_http_server.h"

#define IP_ADDRESS_MAX_LENGTH 16
#define HTML_LENGTH			  32

#define HTML_REPLACEME		  "{{ dns_dump }}"
#define HTML_REPLACEME_LENGTH 14

extern const char index_html_start[] asm("_binary_index_html_start");
extern const char index_html_end[] asm("_binary_index_html_end");

void	  http_server(void *trie_head);
esp_err_t get_handler(httpd_req_t *req);
esp_err_t post_handler(httpd_req_t *req);

#endif
