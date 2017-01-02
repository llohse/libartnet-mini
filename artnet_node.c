/* compliance: Art-Net 4 (protocol 14)
 * Features:
 *  - receive OpPoll and send OpPollReply
 *  - receive OpDmx
 *  - receive OpAddress
 *
 *  This implementation is designed to work with a single controller. Merging and keeping track of all the controllers present is not supported.
 */

#include "artnet_node.h"
#include <stdlib.h> // calloc


void artnet_init ...

void artnet_node_main(artnet_nodestack_t *s) {
	// recv (blocking)
	ssize_t len;
	uint8_t *recvbuf;
	uint8_t *sendbuf;

	while(1) {
	    size_t i;
	    artnet_opcode_t opcode;
	    len = network_recv(recvbuf, len);
	    opcode = artnet_rx_packet(recvbuf, len, srcip);

	    // certain Packets require a reply
	    for (i=0; i<s->num; i++) {
	    }
	    

	    

	}
}


void artnet_init_node(artnet_node_t *n) {
	size_t i;

	static uint8_t curbind = 1; // current bind index

	n->bindindex = curbind;

	for(i=0; i<4; i++) {
		artnet_init_port(dmx_out[i]);
	}

	curbind++;

}

void artnet_init_nodestack(artnet_nodestack_t *s, size_t num) {
	size_t i;

	s->num = num;
	s->nodes = (artnet_node_t *) calloc(num, sizeof(artnet_node_t));
	artnet_get_ip(&(s->ip));
	artnet_get_mac(s->mac);

	for(i=0; i<num; i++) {
		artnet_init_node(s->nodes[i]);
	}
}

void artnet_init_port(artnet_dmx_port_t *p) {

}

int artnet_init_node(artnet_node *an, size_t num_universes) {
	int i;

	if ( num_universes > 4 ) {
		return -1;
	}

	// TODO: test if legal number of universes
	an->num_universes = num_universes;
	an->universes = calloc(num_universes, sizeof(dmx_universe));

	return 0;
}
