
// compliance: Art-Net 4 (protocol 14)
/* Features:
 *  - receive OpPoll and send OpPollReply
 *  - receive OpDmx
 *  - receive OpAddress
*/

#include <string.h>
#include <stddef.h> // size_t
#include "artnet.h"
#include "artnet_net.h"


/* holds information about the remote controller */
artnet_controller_t controller;

/* transmit buffer filled by the artnet system */
uint8_t artnet_txbuf[1024];

/* Only works on little endian systems */

/* dirty little helpers */
static inline void write_uint16_lsb(uint8_t *dst, uint16_t n) {
       dst[0] = (uint8_t) (n);
       dst[1] = (uint8_t) (n >> 8);
}

static inline void write_uint16_hsb(uint8_t *dst, uint16_t n) {
       dst[0] = (uint8_t) (n >> 8 );
       dst[1] = (uint8_t) (n);
}

static inline void write_uint32_hsb(uint8_t *dst, uint32_t n) {
       dst[0] = (uint8_t) (n >> 24);
       dst[1] = (uint8_t) (n >> 16);
       dst[2] = (uint8_t) (n >> 8);
       dst[3] = (uint8_t) (n);
}

static inline void write_uint32_lsb(uint8_t *dst, uint32_t n) {
       dst[0] = (uint8_t) (n);
       dst[1] = (uint8_t) (n >> 8);
       dst[2] = (uint8_t) (n >> 16);
       dst[3] = (uint8_t) (n >> 24);
}

static inline uint16_t read_uint16_hsb(uint8_t *src) {
	return ( src[0] << 8 )  + ( src[0] );
}

static inline uint16_t read_uint16_lsb(uint8_t *src) {
	return ( src[0] ) + ( src[1] << 8 );
}

void artnet_network_init(artnet_node_t *n) {
	artnet_tx_pollreply_tpl(n->poll_reply_template, n);
}

void artnet_handle_packet(artnet_node_t *n, const artnet_packet_t *rxpacket, artnet_packet_t *txpacket) {
	uint16_t opcode;

	size_t txlen;

	size_t len = rxpacket->len;
	uint8_t *buf = rxpacket->data;

	/* check artnet header */
	if ( len <= artnet_id_len ) {
		// no artnet package
		return;
	}

	if (memcmp(buf, artnet_id, artnet_id_len) != 0) {
		// no artnet package
		return;
	}

	opcode = read_uint16_lsb(buf+8);

	ARTNET_DEBUG("Opcode: %#04x\n", opcode);

	switch ( opcode ) {
		case ARTNET_OP_POLL:
			/* keep track of controllers */
			// TODO: more sophisticated tracking of controllers
			if (0 == artnet_rx_netpoll(buf, len)) {
				controller.ip = rxpacket->ip;

				artnet_tx_pollreply_node(artnet_txbuf, &txlen, n);
				txpacket->data = artnet_txbuf;
				txpacket->ip = controller.ip;
				txpacket->len = txlen;
			}
			break;
		case ARTNET_OP_DMX:
			if (0 == artnet_rx_dmx(buf, len, n)) {
				// does not require a reply
			}
			break;
		case ARTNET_OP_ADDRESS:
			if (0 == artnet_rx_address(buf, len, n)) {
				artnet_tx_pollreply_node(artnet_txbuf, &txlen, n);
				txpacket->data = artnet_txbuf;
				txpacket->ip = controller.ip;
				txpacket->len = txlen;
			}
			break;
		default:
			break;
	}
}

int artnet_rx_netpoll(uint8_t *buf, size_t len) {

	if ( len < artnet_len_netpoll )
		return 1;

	//  00 id (1)
	//  08 opcode (2)
	//  10 protver (3-4)
	//  12 talktome (5)
	controller.talktome = buf[12];
	//  13 priority (6)
	controller.priority = buf[13];

	return 0;
}

static inline void artnet_program_address(uint8_t new, uint8_t *cur, uint8_t def) {
	if (new == 0x00) {
		// reset to default
		*cur = def;
	}
	else if((new & 0x80) == 0x80) {
		// bit 7 is set
		*cur = new & 0x0a;
	}

}

int artnet_rx_address(uint8_t *buf, size_t len, artnet_node_t *n) {

	uint8_t i;

	if ( len < artnet_len_address )
		return 1;

	/* skip if bindindex does not match */
	if (buf[13] != n->bindindex) {
		// TODO make nicer
		return 1;
	}
	
	//  00 id (1)
	//  08 opcode (2)
	//  10 protver (3-4)
	//  12 netswitch (5)
	artnet_program_address(buf[12], &(n->net), n->net_default);
	//  13 bindindex (6)
	//  14 shortname (7)
	if ( buf[14] != 0 ) {
		strncpy(n->shortname, (const char *) (buf+14), 18);
		// TODO log status
	}
	//  32 longname (8)
	if ( buf[32] != 0 ) {
		strncpy(n->longname, (const char *) (buf+32), 64);
		// TODO log status
	}
	//  96 swin (9)
	for (i = 0; i < n->numports_in; i++ ) {
		artnet_program_address(buf[96+i], &(n->dmx_in[i].uni), n->dmx_in[i].uni_default);
	}
	// 100 swout (10)
	for (i = 0; i < n->numports_out; i++ ) {
		artnet_program_address(buf[100+i], &(n->dmx_out[i].uni), n->dmx_out[i].uni_default);
	}
	// 104 subswitch (11)
	artnet_program_address(buf[104], &(n->sub), n->sub_default);
	// 105 swvideo (12)
	/* we ignore this */
	// 106 swcommand (13)
	/* we ignore this */
	
	return 0;
}

int artnet_rx_dmx(uint8_t *buf, size_t len, artnet_node_t *n) {

	if ( len < artnet_len_dmx_header)
		return 1;

	uint8_t sequence;
//	uint8_t physical;
	uint8_t subuni;
	uint8_t net;
	uint16_t length;

	size_t i;

	/* extract header */

	//  00 id (1)
	//  08 opcode (2)
	//  10 protver (3-4)
	//  12 sequence (5)
	sequence = buf[12];
	//  13 physical (6)
//	physical = buf[13];
	//  14 subuni (7)
	subuni = buf[14];
	//  15 net (8)
	net = buf[15];

	/* check if we listen for this address */
	if (net != n->net)
		return 1;

	//  16 length (9-10)
	length = read_uint16_hsb(buf+16);

	if(length > 512) {
		// malicious packet
		return 1;
	}
	if(len < (size_t) (length+16) ) {
		// incomplete packet
		return 1;
	}
	
	//  18 data (11)
	// find subscribed ports
	for (i=0; i<n->numports_out; i++) {
		artnet_dmx_port_t *p;

		p = &(n->dmx_out[i]);

		if ( subuni == ((n->sub << 4) + p->uni) && sequence >= p->seq) {
			// copy data
			memcpy(p->dmx->data, buf+16, length);
			p->seq = sequence;
		}	
	}

	return 0;
}

int artnet_tx_pollreply_node(uint8_t *buf, size_t *buflen, artnet_node_t *n) {
	const size_t packet_size = 239;
	size_t i;

	// copy buffered packet
	memcpy(buf, n->poll_reply_template, packet_size);

	// only port statuses need to be updated
	// 178 goodinput (21)
	
	for (i = 0; i < n->numports_in; i++) {
          buf[178+i] = n->dmx_in[i].status;
	}
	// 182 goodoutput (22)
	for (i = 0; i < n->numports_out; i++) {
          buf[182+i] = n->dmx_out[i].status;
	}

	*buflen = artnet_len_pollreply;
	return 0;
}

int artnet_tx_pollreply_tpl(uint8_t *buf, artnet_node_t *n) {
	const size_t packet_size = 239;
	size_t numports;
	size_t i;

	memset(buf, 0, packet_size);

	// packet size 239 bytes

	//  00 id (1)
	memcpy(buf+0, artnet_id, artnet_id_len);
	//  08 opcode (2)
	write_uint16_lsb(buf+8, (uint16_t) ARTNET_OP_POLLREPLY);
	//  10 ipaddress (3)
	/* stored internally as big endian, transmission as little endian */
	write_uint32_lsb(buf+10, n->ip);
	//  14 port (4)
	write_uint16_lsb(buf+14, ARTNET_PORT);
	//  16 versinfo (5-6)
	write_uint16_hsb(buf+16, (uint16_t) FIRMWARE_VERSION);
	//  18 netswitch (7)
        buf[18] = n->net;
	//  19 subswitch (8)
        buf[19] = n->sub;
	//  20 oem (9-10)
	write_uint16_hsb(buf+20, n->oem);
	//  22 ubea (11)
	write_uint16_hsb(buf+22, n->ubea_ver);
	//  23 status1 (12)
	buf[23] = n->status1;
	//  24 esta (13-14)
	memcpy(buf+24, n->esta_code, 2);
	//  26 shortname (15)
	memcpy(buf+26, n->shortname, 18);
	//  44 longname (16)
	memcpy(buf+44, n->longname, 64);
	// 108 nodereport (17)
	/* not implemented */
	// 172 numports (18-19)
	if (n->numports_out > n->numports_in)
		numports = n->numports_out;
	else
		numports = n->numports_in;
	write_uint16_hsb(buf+172, (uint16_t) numports);
	// 174 porttypes (20)
	for ( i=0; i < 4; i++ ) {
		if(n->numports_in > i)
			buf[174+i] |= (uint8_t) ARTNET_PORTTYPE_INPUT;
		if(n->numports_out > i)
			buf[174+i] |= (uint8_t) ARTNET_PORTTYPE_OUTPUT;
	}
	/* goodinput and goodoutput are generated with each packet */
	// 178 goodinput (21)
	// 182 goodoutput (22)
	// 186 swin (23)
	for ( i=0; i < n->numports_in; i++ ) {
		buf[186+i] = (uint8_t) n->dmx_in[i].uni;
	}
	// 190 swout (24)
	for ( i=0; i < n->numports_out; i++ ) {
		buf[190+i] = (uint8_t) n->dmx_out[i].uni;
	}
	// 194 swvideo (25)
	/* we do not support this */
	// 195 swmacro (26)
	/* we do not support this */
	// 196 swremote (27)
	/* we do not support this */
	// 197 Spare (28-30)
	// 200 style (31)
	buf[200] = (uint8_t) ARTNET_ST_NODE;
	// 201 mac (32-37)
	memcpy(buf+201, n->mac, 6);
	// 207 bindip (38)
	/* bind ip is always this device */
	/* stored internally as big endian, transmission as little endian */
	write_uint32_lsb(buf+207, n->ip);
	// 211 bindindex (39)
	buf[211] = n->bindindex;
	// 212 status2 (40)
	/* This field should be called capabilities.
	 * We support 
	 * - Art-Net 4 (15 bit Port Addresses)
	 * - no DHCP
	 * - no Webbrowser configuration
	 * - no sACN
	 */
	buf[212] = (uint8_t) ARTNET_STATUS2_LONGPORT;
	// 213 filler (26 bytes) (41)
	
	return 0;
}


