#ifndef _ARTNET_H
#define _ARTNET_H 1

#include "artnet_node.h"

#define ARTNET_PORT 6454

#define DEBUG 1

#ifdef DEBUG
#include <stdio.h>
#endif

#ifndef ARTNET_DEBUG
#define ARTNET_DEBUG(...) do {; } while (0); 
#endif

typedef struct artnet_packet_s {
	ipv4_addr_t ip;
	uint16_t len;
	uint8_t *data;
} artnet_packet_t;

void artnet_handle_packet(artnet_node_t *n, const artnet_packet_t *rxpacket, artnet_packet_t *txpacket);
void artnet_network_init(artnet_node_t *n);

#endif
