/* ************************************************************************** */
/*                                                                            */
/*                                                              ++            */
/*   post_index.c                                              +**+   +*  *   */
/*                                                             ##%#*###*+++   */
/*   By: aboyreau <bnzlvosnb@mozmail.com>                     +**+ -- ##+     */
/*                                                            # *   *. #*     */
/*   Created: 2025/03/08 10:08:22 by aboyreau          **+*+  * -_._-   #+    */
/*   Updated: 2025/03/09 20:01:37 by aboyreau          +#-.-*  +         *    */
/*                                                     *-.. *   ++       #    */
/* ************************************************************************** */

#include "esp_err.h"
#include "esp_http_server.h"
#include "esp_log.h"
#include "lwip/sockets.h"
#include "trie.h"
#include "web_server.h"

#include <stdarg.h>
#include <string.h>
#include <sys/unistd.h>

// Time-optimized parse_formdata, faster with at least 4 entries
// Time complexity:
//  C(n) = 256n + n
//  O(n)
//  for one entry: 2
// Space-complexity: at most 3mb for 50k records
//  C(n) = formdata + struct + hash(formdata) = 3n
//  O(n)
//
// static void parse_formdata_t(__attribute__((unused)) char *formdata, ...)
// {
// 	//	Put formdata into a hashmap :
// 	//	for each key-value:
// 	//		hash the key
// 	//		store the hash-value pair
// 	//
// 	//	for each field:
// 	//		hash the field key
// 	//		retrieve the value
// 	//		if no value:
// 	//			continue
// 	//		memcpy
// 	//
// }

// Space-optimized parse_formdata, faster with less than 4 entries
// Time complexity:
//  C(n) = (n² + n) / 2
//  O(n²)
//
// Space complexity: at most 2mb for 50k records
//  C(n) = 2n
//  O(n)
//
// Fields are handled as pstr8_t.
static void parse_formdata(char *formdata, ...)
{
	va_list l;
	char   *formdata_key;
	char   *dest;
	char   *key = NULL;

	va_start(l, formdata);
	while (1)
	{
		key = va_arg(l, char *);
		if (key == NULL)
			break;

		dest		 = va_arg(l, char *);
		formdata_key = strstr(formdata, key);
		if (formdata_key == NULL)
		{
			dest[0] = 0;
			continue;
		}

		char *value_start = strchr(formdata_key, '=');
		if (value_start == NULL)
		{
			dest[0] = 0;
			continue;
		}
		value_start++;

		char *value_end = strchr(value_start, '&');
		if (value_end == NULL)
			value_end = strchr(value_start, '\0');
		dest[0] = (char) (value_end - value_start);
		memcpy(dest + 1, value_start, dest[0]);
		dest[(dest[0] + 1)] = 0;
		ESP_LOGI(TAG, "Key: %s; value: %s", key, dest + 1);
	}
	va_end(l);
}

static char *receive_content(httpd_req_t *req)
{
	int	  recv;
	char *content;

	content = malloc(req->content_len + 1);
	if (content == NULL)
		return NULL;
	recv = httpd_req_recv(req, content, req->content_len);
	if (recv <= 0)
	{
		free(content);
		return NULL;
	}
	content[req->content_len] = 0;
	return content;
}

static void display_trie(void *s, void *ip)
{
	(void) ip;
	write(1, s, strlen(s));
	write(1, "\n", 1);
}

esp_err_t post_handler(httpd_req_t *req)
{
	trie_t			  *head = req->user_ctx;
	char			  *content;
	char			   dns[DNS_NAME_MAX_LENGTH]	 = {0};
	char			   ip[IP_ADDRESS_MAX_LENGTH] = {0};
	struct sockaddr_in sa;

	content = receive_content(req);
	if (content == NULL)
	{
		ESP_LOGE(TAG, "Content receive failed");
		return ESP_FAIL;
	}

	parse_formdata(content, "DNS", dns, "IP", ip, NULL);

	lwip_inet_pton(AF_INET, ip, &(sa.sin_addr));
	pstr8_rev((pstr8_t) dns);
	trie_add(head, dns, ntohl(sa.sin_addr.s_addr));
	free(content);
	trie_apply(head, display_trie);

	httpd_resp_set_status(req, "302");
	httpd_resp_set_hdr(req, "Location", "http://dns.local");
	httpd_resp_send(req, "", 0);
	return ESP_OK;
}
