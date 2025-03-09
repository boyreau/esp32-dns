/* ************************************************************************** */
/*                                                                            */
/*                                                              ++            */
/*   server.c                                                  +**+   +*  *   */
/*                                                             ##%#*###*+++   */
/*   By: aboyreau <bnzlvosnb@mozmail.com>                     +**+ -- ##+     */
/*                                                            # *   *. #*     */
/*   Created: 2025/03/03 18:30:42 by aboyreau          **+*+  * -_._-   #+    */
/*   Updated: 2025/03/08 22:47:03 by aboyreau          +#-.-*  +         *    */
/*                                                     *-.. *   ++       #    */
/* ************************************************************************** */

#include "web_server.h"

#include <esp_http_server.h>
#include <esp_log.h>
#include <nvs_flash.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/param.h>
#include <unistd.h>

/* A simple example that demonstrates how to create GET and POST
 * handlers for the web server.
 */

#ifndef TAG
#define TAG "WEB"
#endif

void http_server(void *trie_head)
{
	httpd_handle_t server	 = NULL;
	httpd_config_t config	 = HTTPD_DEFAULT_CONFIG();
	config.lru_purge_enable	 = true;
	config.keep_alive_enable = false;

	if (httpd_start(&server, &config) != ESP_OK)
		return;

	httpd_uri_t uri_get = {
		.uri	  = "/",
		.method	  = HTTP_GET,
		.handler  = get_handler,
		.user_ctx = trie_head
	};
	httpd_uri_t uri_post = {
		.uri	  = "/",
		.method	  = HTTP_POST,
		.handler  = post_handler,
		.user_ctx = trie_head
	};
	httpd_register_uri_handler(server, &uri_get);
	httpd_register_uri_handler(server, &uri_post);

	while (server)
		sleep(4);
}
