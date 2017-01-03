/* compliance: Art-Net 4 (protocol 14)
 * Features:
 *  - receive OpPoll and send OpPollReply
 *  - receive OpDmx
 *  - receive OpAddress
 *
 *  This implementation is designed to work with a single controller. Merging and keeping track of all the controllers present is not supported.
 */

#include "artnet_node.h"
#include <stdlib.h> // malloc

void artnet_init_node(artnet_node_t *n, uint8_t n_in, uint8_t n_out) {
	uint8_t i;
	static uint8_t curbind = 1; // current bind index

	n->bindindex = curbind++;
	n->numports_in = n_in;
	n->numports_out = n_out;

	for(i=0; i<n_in; i++) {
		artnet_init_port(&(n->dmx_in[i]));
	}
	for(i=0; i<n_out; i++) {
		artnet_init_port(&(n->dmx_out[i]));
	}
}

void artnet_init_port(artnet_dmx_port_t *p) {
 	p->dmx = malloc(sizeof(dmx_t));
}

