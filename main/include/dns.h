/* ************************************************************************** */
/*                                                                            */
/*                                                              ++            */
/*   dns.h                                                     +**+   +*  *   */
/*                                                             ##%#*###*+++   */
/*   By: aboyreau <bnzlvosnb@mozmail.com>                     +**+ -- ##+     */
/*                                                            # *   *. #*     */
/*   Created: 2025/02/17 22:57:37 by aboyreau          **+*+  * -_._-   #+    */
/*   Updated: 2025/02/20 00:17:43 by aboyreau          +#-.-*  +         *    */
/*                                                     *-.. *   ++       #    */
/* ************************************************************************** */

#ifndef DNS_H
#define DNS_H

#include <stdint.h>
#include <stdio.h>

#define HEADER_SIZE 96

struct dns_header
{
		// header fields
		uint16_t id : 16;

		union
		{
				uint16_t flags;

				struct
				{
						uint16_t qr		: 1;
						uint16_t opcode : 4;
						uint16_t aa		: 1;
						uint16_t tc		: 1;
						uint16_t rd		: 1;
						uint16_t ra		: 1;
						uint16_t z		: 3;
						uint16_t rcode	: 4;
				};
		};

		uint16_t qdcount : 16;
		uint16_t ancount : 16;
		uint16_t nscount : 16;
		uint16_t arcount : 16;
} __attribute__((packed));

struct dns_question
{
		// question fields
		uint8_t length; // length of the name
		char   *qname;	// name
		uint8_t qtype  : 2;
		uint8_t qclass : 2;
};

struct dns_answer
{
		uint16_t line;
};

struct dns_authority
{
		uint16_t line;
};

struct dns_additionnal
{
		uint16_t line;
};

int dns_write_header(struct dns_header *header, char *response);
int dns_read_header(struct dns_header *header, FILE *inputstream);

#endif
