/* ************************************************************************** */
/*                                                                            */
/*                                                              ++            */
/*   main.c                                                    +**+   +*  *   */
/*                                                             ##%#*###*+++   */
/*   By: aboyreau <bnzlvosnb@mozmail.com>                     +**+ -- ##+     */
/*                                                            # *   *. #*     */
/*   Created: 2025/02/24 02:29:51 by aboyreau          **+*+  * -_._-   #+    */
/*   Updated: 2025/03/06 11:59:01 by aboyreau          +#-.-*  +         *    */
/*                                                     *-.. *   ++       #    */
/* ************************************************************************** */

#include "esp_event.h"
#include "esp_netif.h"
#include "mdns.h"
#include "nvs_flash.h"
#include "protocol_examples_common.h"
#include "server.h"
#include "trie.h"

#include <lwip/netdb.h>
#include <stdio.h>
#include <string.h>
#include <sys/param.h>

static void mDNS_server(void)
{
	// initialize mDNS service
	esp_err_t err = mdns_init();
	if (err)
	{
		printf("MDNS Init failed: %d\n", err);
		return;
	}

	mdns_hostname_set("dns");
	mdns_instance_name_set("My DNS on ESP32");

	mdns_service_add(NULL, "_http", "_tcp", 80, NULL, 0);
	mdns_service_instance_name_set("_http", "_tcp", "dns administration");

	mdns_service_add(NULL, "_dns", "_udp", 53, NULL, 0);
	mdns_service_instance_name_set("_dns", "_udp", "dns server");
}

void app_main(void)
{
	ESP_ERROR_CHECK(nvs_flash_init());
	ESP_ERROR_CHECK(esp_netif_init());
	ESP_ERROR_CHECK(esp_event_loop_create_default());

	/* This helper function configures Wi-Fi or Ethernet, as selected in
	 * menuconfig. Read "Establishing Wi-Fi or Ethernet Connection" section
	 * in examples/protocols/README.md for more information about this
	 * function.
	 */
	ESP_ERROR_CHECK(example_connect());
	setvbuf(stdout, NULL, _IONBF, 0);

	static struct trie_s head		 = {0};
	pstr8_t				 custom_name = "\nrf.godoics\0";
	trie_add(&head, custom_name, 603187885);

	xTaskCreate(
		udp_server_task,
		"dns_server",
		4096,
		(void *[]) {(void *) AF_INET, &head},
		5,
		NULL
	);
	xTaskCreate(http_server, "http_server", 4096, &head, 5, NULL);
	mDNS_server();
}
