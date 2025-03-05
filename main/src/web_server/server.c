/* ************************************************************************** */
/*                                                                            */
/*                                                              ++            */
/*   server.c                                                  +**+   +*  *   */
/*                                                             ##%#*###*+++   */
/*   By: aboyreau <bnzlvosnb@mozmail.com>                     +**+ -- ##+     */
/*                                                            # *   *. #*     */
/*   Created: 2025/03/03 18:30:42 by aboyreau          **+*+  * -_._-   #+    */
/*   Updated: 2025/03/04 00:34:13 by aboyreau          +#-.-*  +         *    */
/*                                                     *-.. *   ++       #    */
/* ************************************************************************** */

#include "server.h"

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

esp_err_t get_handler(httpd_req_t *req)
{
	/* Send a simple response */
	httpd_resp_send(req, index_html_start, index_html_end - index_html_start);
	return ESP_OK;
}

/* URI handler structure for GET /uri */
httpd_uri_t uri_get =
	{.uri = "/", .method = HTTP_GET, .handler = get_handler, .user_ctx = NULL};

void http_server(void *_)
{
	httpd_handle_t server	= NULL;
	httpd_config_t config	= HTTPD_DEFAULT_CONFIG();
	config.lru_purge_enable = true;

	// Start the httpd server
	ESP_LOGI(TAG, "Starting server on port: '%d'", config.server_port);
	if (httpd_start(&server, &config) == ESP_OK)
	{
		// Set URI handlers
		ESP_LOGI(TAG, "Registering URI handlers");
		httpd_register_uri_handler(server, &uri_get);
	}
	else
	{
		ESP_LOGI(TAG, "Error starting server!");
		return;
	}

	while (server)
	{
		sleep(300);
	}
}
