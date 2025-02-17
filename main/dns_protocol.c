// SOURCE : https://github.com/wfelipe/simple-dns/
/*
 * Implementation of RFC 1035
 * DOMAIN NAMES - IMPLEMENTATION AND SPECIFICATION
 * http://www.ietf.org/rfc/rfc1034.txt
 * http://www.ietf.org/rfc/rfc1035.txt
 */

#include <arpa/inet.h>

#include <assert.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "dns_protocol.h"

#include <stdio.h>

void dns_print_header(struct dns_header *header, char *buf) {
  sprintf(buf, "ID: %d\n", header->id);
  sprintf(buf, "qr: %d\n", header->qr);
  sprintf(buf, "opcode: %d\n", header->opcode);
  sprintf(buf, "aa: %d\n", header->aa);
  sprintf(buf, "tc: %d\n", header->tc);
  sprintf(buf, "rd: %d\n", header->rd);
  sprintf(buf, "ra: %d\n", header->ra);
  sprintf(buf, "z: %d\n", header->z);
  sprintf(buf, "rcode: %d\n", header->rcode);

  sprintf(buf, "qdcount: %d\n", header->qdcount);
  sprintf(buf, "ancount: %d\n", header->ancount);
  sprintf(buf, "nscount: %d\n", header->nscount);
  sprintf(buf, "arcount: %d\n", header->arcount);
}

void dns_print_packet(struct dns_packet *packet, char *buf) {
  dns_print_header(&packet->header, buf);
  sprintf(buf, "data_size: %d\n", packet->data_size);
  sprintf(buf, "data: %s\n", packet->data);
}

int dns_request_parse(struct dns_packet *pkt, void *data, u_int16_t size) {
  int i;

  dns_header_parse(&pkt->header, data);

  pkt->data = malloc(size - 12);
  memcpy(pkt->data, data + 12, size - 12);
  pkt->data_size = size - 12;

  i = 0;
  while (i < pkt->header.qdcount) {
    dns_question_parse(pkt);
    i++;
  }

  return 1;
}

int dns_header_parse(struct dns_header *header, void *data) {
  memcpy(header, data, 12);

  header->id = ntohs(header->id);
  header->qdcount = ntohs(header->qdcount);
  header->ancount = ntohs(header->ancount);
  header->nscount = ntohs(header->nscount);
  header->arcount = ntohs(header->arcount);

  return 1;
}

int dns_question_parse(struct dns_packet *pkt) {
  u_int16_t i, length, j;
  char *question = pkt->data;

  i = 0;
  length = question[i++];

  do {
    printf("SIZE: %d\n", length);
    for (j = 0; j < length; j++) {
      printf("%c", question[i + j]);
    }
    printf("\n");
    i += length;
    length = question[i++];
  } while (length != 0 && i < pkt->data_size);

  return 1;
}
