/* ************************************************************************** */
/*                                                                            */
/*                                                              ++            */
/*   server.c                                                  +**+   +*  *   */
/*                                                             ##%#*###*+++   */
/*   By: aboyreau <bnzlvosnb@mozmail.com>                     +**+ -- ##+     */
/*                                                            # *   *. #*     */
/*   Created: 2025/03/03 18:30:42 by aboyreau          **+*+  * -_._-   #+    */
/*   Updated: 2025/03/06 12:02:41 by aboyreau          +#-.-*  +         *    */
/*                                                     *-.. *   ++       #    */
/* ************************************************************************** */

#include "server.h"
#include "trie.h"

#include <esp_http_server.h>
#include <esp_log.h>
#include <nvs_flash.h>
#include <stdlib.h>
#include <string.h>
#include <sys/param.h>
#include <unistd.h>

extern const char index_html_start[] asm("_binary_index_html_start");
extern const char index_html_end[] asm("_binary_index_html_end");

/* A simple example that demonstrates how to create GET and POST
 * handlers for the web server.
 */

#ifndef TAG
#define TAG "WEB"
#endif

static void **req(void)
{
	static void *req = NULL;

	return &req;
}

static void *getreq(void)
{
	return *req();
}

static void setreq(void *new_req)
{
	*req() = new_req;
}

static void trie_send_chunk(void *chunk, void *ip)
{
	if (pstr8_len(chunk) == 0)
		return;
	void *req = getreq();
	if (req == NULL)
		return;
	httpd_resp_send_chunk(req, "<tr>", 4);
	httpd_resp_send_chunk(req, (char *) chunk + 1, pstr8_len(chunk));
	httpd_resp_send_chunk(req, "</tr>", 5);
}

esp_err_t get_handler(httpd_req_t *req)
{
	trie_t *head			 = req->user_ctx;
	char   *replace_me_start = strstr(index_html_start, "{{ dns_dump }}");
	char   *replace_me_end	 = replace_me_start + 14;

	httpd_resp_send_chunk(req, index_html_start, replace_me_start - index_html_start);

	setreq(req);
	trie_iter(head, trie_send_chunk);

	httpd_resp_send_chunk(req, replace_me_end, replace_me_end - index_html_start);
	httpd_resp_send_chunk(req, "", 0);
	return ESP_OK;
}

void http_server(void *trie_head)
{
	httpd_handle_t server	= NULL;
	httpd_config_t config	= HTTPD_DEFAULT_CONFIG();
	config.lru_purge_enable = true;

	if (httpd_start(&server, &config) != ESP_OK)
		return;

	httpd_uri_t uri_get = {
		.uri	  = "/",
		.method	  = HTTP_GET,
		.handler  = get_handler,
		.user_ctx = trie_head
	};
	httpd_register_uri_handler(server, &uri_get);

	while (server)
		sleep(4);
}
