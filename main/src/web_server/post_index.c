/* ************************************************************************** */
/*                                                                            */
/*                                                              ++            */
/*   post_index.c                                              +**+   +*  *   */
/*                                                             ##%#*###*+++   */
/*   By: aboyreau <bnzlvosnb@mozmail.com>                     +**+ -- ##+     */
/*                                                            # *   *. #*     */
/*   Created: 2025/03/08 10:08:22 by aboyreau          **+*+  * -_._-   #+    */
/*   Updated: 2025/03/08 22:21:24 by aboyreau          +#-.-*  +         *    */
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

	va_start(l, formdata);

	for (char *str = va_arg(l, char *); str != NULL; str = va_arg(l, char *))
	{
		char *dest	= va_arg(l, char *);
		char *value = strstr(formdata, str);
		if (value == NULL)
			continue;
		value = strchr(value, '=');
		if (value == NULL)
		{
			dest[0] = 0;
			continue;
		}
		dest[0] = (char) (strchr(value, '&') - value);
		memcpy(dest + 1, value, dest[0]);
	}
	va_end(l);
}

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

esp_err_t post_handler(httpd_req_t *req)
{
	trie_t *head = req->user_ctx;
	char   *content;

	ESP_LOGI(TAG, "Request received");
	content = receive_content(req);
	if (content == NULL)
	{
		ESP_LOGE(TAG, "Content receive failed");
		return ESP_FAIL;
	}

	ESP_LOGI(TAG, "Request body received");
	char dns[DNS_NAME_MAX_LENGTH]  = {0}; // Will be filled as a pstring
	char ip[IP_ADDRESS_MAX_LENGTH] = {0};
	parse_formdata(content, "DNS", dns, "IP", ip, NULL);
	ESP_LOGI(TAG, "Request body parsed");

	struct sockaddr_in sa;
	inet_pton(AF_INET, ip, &(sa.sin_addr));

	trie_add(head, dns, sa.sin_addr.s_addr);
	ESP_LOGI(TAG, "DNS entry registered");

	free(content);
	httpd_resp_set_status(req, "302");
	httpd_resp_set_hdr(req, "Location", "http://dns.local");
	httpd_resp_send(req, "", 0);

	ESP_LOGI(TAG, "Request handled");
	return ESP_OK;
}
