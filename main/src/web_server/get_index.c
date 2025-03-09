/* ************************************************************************** */
/*                                                                            */
/*                                                              ++            */
/*   get_index.c                                               +**+   +*  *   */
/*                                                             ##%#*###*+++   */
/*   By: aboyreau <bnzlvosnb@mozmail.com>                     +**+ -- ##+     */
/*                                                            # *   *. #*     */
/*   Created: 2025/03/08 09:18:02 by aboyreau          **+*+  * -_._-   #+    */
/*   Updated: 2025/03/08 14:40:46 by aboyreau          +#-.-*  +         *    */
/*                                                     *-.. *   ++       #    */
/* ************************************************************************** */

#include "lwip/inet.h"
#include "trie.h"
#include "web_server.h"

#include <esp_http_server.h>
#include <esp_log.h>
#include <nvs_flash.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/param.h>
#include <unistd.h>

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

static char *strrev(char *s)
{
	uint8_t l = 0;
	uint8_t r = strlen(s) - 1;

	while (l < r)
	{
		s[l] = (char) (s[l] ^ s[r]);
		s[r] = (char) (s[l] ^ s[r]);
		s[l] = (char) (s[l] ^ s[r]);

		l++;
		r--;
	}
	return s;
}

static void trie_send_chunk(void *chunk, void *ip)
{
	if (pstr8_len(chunk) == 0)
		return;
	void *req = getreq();
	if (req == NULL)
		return;

	char dns_ip_addr_buf[DNS_NAME_MAX_LENGTH + IP_ADDRESS_MAX_LENGTH + HTML_LENGTH];
	struct in_addr addr = {(int) ip};

	sprintf(
		dns_ip_addr_buf,
		"<tr><td>%s</td><td>%s</td></tr>",
		strrev(chunk),
		inet_ntoa(addr)
	);
	httpd_resp_send_chunk(req, dns_ip_addr_buf, (ssize_t) strlen(dns_ip_addr_buf));
}

esp_err_t get_handler(httpd_req_t *req)
{
	trie_t *head			 = req->user_ctx;
	char   *replace_me_start = strstr(index_html_start, HTML_REPLACEME);
	char   *replace_me_end	 = replace_me_start + HTML_REPLACEME_LENGTH;

	httpd_resp_send_chunk(req, index_html_start, replace_me_start - index_html_start);

	setreq(req);
	trie_apply(head, trie_send_chunk);

	httpd_resp_send_chunk(req, replace_me_end, index_html_end - replace_me_end);
	httpd_resp_send_chunk(req, "", 0);
	return ESP_OK;
}
