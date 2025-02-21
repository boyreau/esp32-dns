/* ************************************************************************** */
/*                                                                            */
/*                                                              ++            */
/*   server.c                                                  +**+   +*  *   */
/*                                                             ##%#*###*+++   */
/*   By: aboyreau <bnzlvosnb@mozmail.com>                     +**+ -- ##+     */
/*                                                            # *   *. #*     */
/*   Created: 2025/02/19 12:37:58 by aboyreau          **+*+  * -_._-   #+    */
/*   Updated: 2025/02/20 18:34:27 by aboyreau          +#-.-*  +         *    */
/*                                                     *-.. *   ++       #    */
/* ************************************************************************** */

#include "dns.h"
#include "esp_log.h"
#include "llist.h"
#include "lwip/sockets.h"
#include "server.h"
#include "trie.h"

#include <lwip/netdb.h>
#include <stdio.h>
#include <string.h>
#include <sys/param.h>

static const char *TAG = __FILE__;

static int
receive_packet(int sock, struct sockaddr_storage *source_addr, void *packet)
{
	ESP_LOGI(TAG, "Dest: %p", (void *) packet);
	socklen_t socklen = sizeof(*source_addr);

	int len = recvfrom(
		sock,
		packet,
		DNS_PACKET_SIZE_MAX,
		0,
		(struct sockaddr *) source_addr,
		&socklen
	);
	if (len < 0)
	{
		ESP_LOGE(
			TAG,
			"Error occurred during recvfrom: errno %d: %s",
			errno,
			strerror(errno)
		);
		return -1;
	}
	return len;
}

static void dump_mem_str(char *buf, size_t size)
{
	for (size_t i = 0; i < size; i++)
	{
		if (isprint((int) buf[i]))
		{
			putchar(buf[i]);
			buf[i] = '\0';
		}
	}
	putchar('\n');
}

int handle_request(int sock, struct sockaddr_storage *source_addr) // NOLINT
{
	int messagelen = 0;

	struct dns_packet	  packet;
	struct dns_header	 *header = (void *) &packet;
	struct dns_questions *questions =
		(void *) ((char *) &packet + DNS_HEADER_BYTE_SIZE);
	label_list *labels = NULL;

	ESP_LOGI(TAG, "Sizeof pointer: %u", sizeof(struct trie_s *));
	while (1)
	{
		messagelen = receive_packet(sock, source_addr, header);
		if (messagelen < 0)
		{
			ESP_LOGE(TAG, "Error occurred during receive: errno %d", errno);
			return -1;
		}
		dns_ntoh_header(header);

		for (int i = 0; i < header->qdcount; i++)
		{
			dns_read_labels(&labels, (char *) questions, messagelen);
			// TODO: handle labels
		}
	}
}
