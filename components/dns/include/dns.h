/* ************************************************************************** */
/*                                                                            */
/*                                                              ++            */
/*   dns.h                                                     +**+   +*  *   */
/*                                                             ##%#*###*+++   */
/*   By: aboyreau <bnzlvosnb@mozmail.com>                     +**+ -- ##+     */
/*                                                            # *   *. #*     */
/*   Created: 2025/02/17 22:57:37 by aboyreau          **+*+  * -_._-   #+    */
/*   Updated: 2025/03/01 20:10:05 by aboyreau          +#-.-*  +         *    */
/*                                                     *-.. *   ++       #    */
/* ************************************************************************** */

#ifndef DNS_H
#define DNS_H

#include "pstring.h"

#include <stdint.h>
#include <stdio.h>

#ifndef TAG
#define TAG "DNS"
#endif

#define DNS_HEADER_SIZE		 96
#define DNS_HEADER_BYTE_SIZE 12

#define DNS_PACKET_SIZE_MAX 512

extern const char *dns_classes[];
extern const char *dns_types[];

#undef MR

enum dns_type_e
{
	A = 1,
	NS,
	MD,
	MF,
	CNAME,
	SOA,
	MB,
	MG,
	MR,
	NuLL,
	WKS,
	PTR,
	HINFO,
	MINFO,
	MX,
	TXT
};

#define MR 32

enum dns_class_e
{
	IN = 1,
	CS,
	CH,
	HS
};

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
};

struct dns_question
{
		pstr8_t	 qname;
		uint16_t qtype;
		uint16_t qclass;
};

struct dns_rr
{
		uint16_t type;
		uint16_t class;
		uint32_t ttl;
		uint16_t rdlength;
		uint32_t rddata;
};

struct dns_packet
{
		union
		{
				struct
				{
						struct dns_header header;
						pstr8_t			  qlabels;
				};

				uint8_t raw[DNS_PACKET_SIZE_MAX];
		};
};

int	 dns_read_header(struct dns_header *header, FILE *inputstream);
void dns_ntoh_header(struct dns_header *header);
void dns_log_header(struct dns_header *header);
int	 dns_write_header(struct dns_header *header, char *response);

pstr8_t dns_read_label(char *question);
pstr8_t dns_read_labels(char *message, size_t messagelen);

struct dns_question dns_parse_question(pstr8_t question, size_t packetlength);
void				dns_log_question(struct dns_question question);

#endif
