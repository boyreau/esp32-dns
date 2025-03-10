/* ************************************************************************** */
/*                                                                            */
/*                                                              ++            */
/*   get_index.c                                               +**+   +*  *   */
/*                                                             ##%#*###*+++   */
/*   By: aboyreau <bnzlvosnb@mozmail.com>                     +**+ -- ##+     */
/*                                                            # *   *. #*     */
/*   Created: 2025/03/08 09:18:02 by aboyreau          **+*+  * -_._-   #+    */
/*   Updated: 2025/03/10 12:14:02 by aboyreau          +#-.-*  +         *    */
/*                                                     *-.. *   ++       #    */
/* ************************************************************************** */

#include "esp_err.h"
#include "lwip/inet.h"
#include "lwip/sockets.h"
#include "pstring.h"
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

#define BUF_LENGTH (DNS_NAME_MAX_LENGTH + IP_ADDRESS_MAX_LENGTH + HTML_LENGTH)

static void trie_send_chunk(const void *chunk, const void *ip)
{
	void   *req;
	char	dns_ip_addr_buf[BUF_LENGTH] = "\b<tr><td>";
	char	ip_buf[IP_ADDRESS_MAX_LENGTH];
	pstr8_t dns = pstr8_dup(chunk);

	if (pstr8_len(dns) == 0)
		return;
	req = getreq();

	if (req == NULL)
		return;

	ESP_LOGD(TAG, "Params: ip: %lu", (uint32_t) ip);
	write(1, (char *) dns + 1, *(char *) dns);

	lwip_inet_ntop(
		AF_INET,
		&((struct in_addr) {.s_addr = ntohl((uint32_t) ip)}),
		ip_buf + 1,
		IP_ADDRESS_MAX_LENGTH - 1
	);
	ip_buf[0] = (char) strlen(ip_buf + 1);

	pstr8_cat(dns_ip_addr_buf, pstr8_rev(dns));
	pstr8_cat(dns_ip_addr_buf, "\t</td><td>");
	pstr8_cat(dns_ip_addr_buf, ip_buf);
	pstr8_cat(dns_ip_addr_buf, "\n</td></tr>");

	free(dns);

	int ret =
		httpd_resp_send_chunk(req, dns_ip_addr_buf + 1, pstr8_len(dns_ip_addr_buf));
	if (ret != ESP_OK)
		setreq(NULL); // Stops further calls
}

esp_err_t get_handler(httpd_req_t *req)
{
	trie_t *head			 = req->user_ctx;
	char   *replace_me_start = strstr(index_html_start, HTML_REPLACEME);
	char   *replace_me_end	 = replace_me_start + HTML_REPLACEME_LENGTH;

	httpd_resp_send_chunk(req, index_html_start, replace_me_start - index_html_start);

	setreq(req);
	ESP_LOGI(TAG, "Retrieving values");
	trie_apply(head, trie_send_chunk);

	httpd_resp_send_chunk(req, replace_me_end, index_html_end - replace_me_end);
	httpd_resp_send_chunk(req, "", 0);
	return ESP_OK;
}
