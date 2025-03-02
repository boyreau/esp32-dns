/* ************************************************************************** */
/*                                                                            */
/*                                                              ++            */
/*   main.c                                                    +**+   +*  *   */
/*                                                             ##%#*###*+++   */
/*   By: aboyreau <bnzlvosnb@mozmail.com>                     +**+ -- ##+     */
/*                                                            # *   *. #*     */
/*   Created: 2025/02/24 02:29:51 by aboyreau          **+*+  * -_._-   #+    */
/*   Updated: 2025/03/02 16:10:59 by aboyreau          +#-.-*  +         *    */
/*                                                     *-.. *   ++       #    */
/* ************************************************************************** */

#include "esp_event.h"
#include "esp_netif.h"
#include "nvs_flash.h"
#include "protocol_examples_common.h"
#include "server.h"

#include <lwip/netdb.h>
#include <stdio.h>
#include <string.h>
#include <sys/param.h>

void app_main(void)
{
	ESP_ERROR_CHECK(nvs_flash_init());
	ESP_ERROR_CHECK(esp_netif_init());
	ESP_ERROR_CHECK(esp_event_loop_create_default());

	/* This helper function configures Wi-Fi or Ethernet, as selected in
	 * menuconfig. Read "Establishing Wi-Fi or Ethernet Connection" section in
	 * examples/protocols/README.md for more information about this function.
	 */
	ESP_ERROR_CHECK(example_connect());
	setvbuf(stdout, NULL, _IONBF, 0);
	xTaskCreate(udp_server_task, "udp_server", 3072, (void *) AF_INET, 5, NULL);
	// xTaskCreate(tcp_server_task, "web interface", 1024, (void *) AF_INET, 5,
	// NULL);
}
