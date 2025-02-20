/* ************************************************************************** */
/*                                                                            */
/*                                                              ++            */
/*   server.c                                                  +**+   +*  *   */
/*                                                             ##%#*###*+++   */
/*   By: aboyreau <bnzlvosnb@mozmail.com>                     +**+ -- ##+     */
/*                                                            # *   *. #*     */
/*   Created: 2025/02/19 12:37:58 by aboyreau          **+*+  * -_._-   #+    */
/*   Updated: 2025/02/20 01:15:16 by aboyreau          +#-.-*  +         *    */
/*                                                     *-.. *   ++       #    */
/* ************************************************************************** */

#include "cc.h"
#include "dns.h"
#include "esp_log.h"
#include "lwip/sockets.h"
#include "server.h"

#include <lwip/netdb.h>
#include <stdio.h>
#include <string.h>
#include <sys/param.h>

static const char *TAG = __FILE__;

static void log_header(struct dns_header *header)
{
	ESP_LOGI(
		TAG,
		"\nid: %016X\n"	 // id
		"qr: 1\n"		 // qr		- query or response
		"opcode: %04X\n" // opcode	- query type
		"aa: %01X\n"	 // aa		- authoritative answer
		"tc: %01X\n"	 // tc 		- truncated
		"rd: %01X\n"	 // rd		- recursion desired
		"ra: 0\n"		 // ra		- recursion available
		"z: %03X\n"		 // z		- reserved for future use
		"rcode: %04X\n"	 // rcode	- response code
		"qdcount: %u\n"	 // qdcount	- how many questions ?
		"ancount: %u\n"	 // ancount	- how many answers ?
		"nscount: %u\n"	 // nscount	- how many name server resource
						 // record in the authority record session
		"arcount: %u\n", // arcount	- how many resources record in
						 // the additionnal record section
		header->id,
		header->opcode,
		header->aa,
		header->tc,
		header->rd,
		header->z,
		header->rcode,
		header->qdcount,
		header->ancount,
		header->nscount,
		header->arcount
	);
}

static int
receive_header(int sock, struct sockaddr_storage *source_addr, struct dns_header *header)
{
	socklen_t socklen = sizeof(*source_addr);

	int len =
		recvfrom(sock, header, 512, 0, (struct sockaddr *) source_addr, &socklen);
	len = ntohs(len);
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
	// ntohs(header);
	header->id		= ntohs(header->id);
	header->flags	= ntohs(header->flags);
	header->qdcount = ntohs(header->qdcount);
	header->ancount = ntohs(header->ancount);
	header->nscount = ntohs(header->nscount);
	header->arcount = ntohs(header->arcount);
	log_header(header);
	return 0;
}

static int
send_header(int sock, struct sockaddr_storage *source_addr, struct dns_header *header)
{
	// Change header to match response
	header->qr	  = 0;
	header->aa	  = 0;
	header->rcode = 2;

	return sendto(
		sock,
		header,
		sizeof(*header),
		0,
		(struct sockaddr *) source_addr,
		sizeof(*source_addr)
	);
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
	int				  err		   = 0;
	char			  message[512] = {0};						   // NOLINT
	struct dns_header header	   = *(struct dns_header *) message;

	while (1)
	{
		ESP_LOGI(TAG, "Waiting for DNS query header");

		err = receive_header(sock, source_addr, (void *) message);
		if (err < 0)
		{
			ESP_LOGE(TAG, "Error occurred during receive: errno %d", errno);
			return -1;
		}

		uint8_t qsize = 0;
		size_t	index = HEADER_SIZE / 8;
		while (1)
		{
			qsize = *(uint8_t *) (message + index);
			index++;
			// recv(sock, &qsize, sizeof(qsize), MSG_WAITALL);
			// NOLINTEND
			// clang-format on

			if (qsize == 0)
				break;
			ESP_LOGI(TAG, "Size: %d", qsize);

			char *question = malloc(qsize + 1);
			memcpy(question, message + index, qsize);
			question[qsize] = 0;
			index += qsize;
			// recv(sock, question, qsize, MSG_WAITALL);
			ESP_LOGI(TAG, "Question:");
			dump_mem_str(question, (size_t) qsize);
			free(question);
		}
		// return 0;
		ESP_LOGI(TAG, "DNS query answered (kinda)");

		{
			// for (int i = 0; i < header.qdcount; i++)
			// {
			// 	while (1)
			// 	{
			// 		char size = 0;
			// 		err		  = recvfrom(sock, &size, 1, MSG_WAITALL, NULL,
			// NULL); 		if (err < 0)
			// 		{
			// 			ESP_LOGE(
			// 				TAG,
			// 				"Error while reading question: %d: %s",
			// 				errno,
			// 				strerror(errno)
			// 			);
			// 			return -1;
			// 		}
			// 		ESP_LOGI(TAG, "Expected question size: %d", size);
			// 		if (size == 0)
			// 			break;
			// 		char *question = calloc(size + 1, 1);
			// 		recvfrom(sock, question, size, MSG_WAITALL, NULL, NULL);
			// 		ESP_LOGI(TAG, "Question: ");
			// 		dump_mem_str(question, size);
			// 		free(question);
			// 	}
			// }

			// err = send_header(sock, source_addr, &header);
			// if (err < 0)
			// {
			// 	ESP_LOGE(
			// 		TAG,
			// 		"Error occurred during sending: errno %d: %s",
			// 		errno,
			// 		strerror(errno)
			// 	);
			// 	return -1;
			// }
		}
	}
}
