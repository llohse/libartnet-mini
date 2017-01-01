#include "artnet_ll.h"

// compliance: Art-Net 4 (protocol 14)
/* Features:
 *  - receive OpPoll and send OpPollReply
 *  - receive OpIpProg and send OpIpProgReply
 *  - receive OpDmx
 *  - receive OpAddress
 *  - receive OpInput
*/

artnet_controller_t controller;

artnet_nodestack_t nodestack;

// TODO: adapt for big endian systems

/* dirty little helpers */
inline void write_uint16_lsb(uint8_t *dst, uint16_t n) {
       dst[0] = (uint8_t) (n);
       dst[1] = (uint8_t) (n >> 8);
}

inline void write_uint16_hsb(uint8_t *dst, uint16_t n) {
       dst[0] = (uint8_t) (n >> 8 );
       dst[1] = (uint8_t) (n);
}

inline void write_uint32_hsb(uint8_t *dst, uint32_t n) {
       dst[0] = (uint8_t) (n >> 24);
       dst[1] = (uint8_t) (n >> 16);
       dst[2] = (uint8_t) (n >> 8);
       dst[3] = (uint8_t) (n);
}

artnet_opcode_t artnet_rx_packet(uint8_t *buf, size_t len, ipv4_addr_t srcip) {
	uint16_t opcode_be, opcode;

	/* check artnet header */
	if ( len <= artnet_id_len ) {
		// no artnet package
		return 0;
	}

	if (strncmp(buf, artnet_id, artnet_id_len) != 0) {
		// no artnet package
		return 0;
	}

	/* this *should* work for big and little endian hosts */
        opcode_be = buf[8] + (buf[9] << 8);
	opcode = (artnet_opcode_t) ntohs(opcode_be);

	switch ( opcode ) {
		case ARTNET_OP_POLL:
			artnet_parse_netpoll(buf, len, srcip);
			return ARTNET_OP_POLL;
		case ARTNET_OP_DMX:
			artnet_parse_dmx(buf, len, srcip);
			return ARTNET_OP_DMX;
		default:
			return ARTNET_OP_INVALID;
	}
}

void artnet_rx_netpoll(uint8_t *buf, size_t len, ipv4_addr_t srcip) {

	// TODO: check length
	controller.ip = srcip;

	//  00 id (1)
	//  08 opcode (2)
	//  10 protver (3-4)
	//  12 talktome (5)
	controller.talktome = buf[12];
	//  13 priority (6)
	controller.priority = buf[13];
}

void artnet_rx_address(uint8_t *buf, size_t len) {
	
	// TODO: check length
	// TODO: implement
}

void artnet_rx_dmx(uint8_t *buf, size_t len) {
	// extract header
	//
	// find corresponding nodes and outputs
	//
	// copy data
}

void artnet_tx_pollreply(uint8_t *buf, artnet_node_t *n) {
	const size_t packet_size = 239;
	size_t i;

	// copy buffered packet
	memcpy(buf, n->poll_reply_template, packet_size);

	// only port statuses need to be updated
	// 178 goodinput (21)
	
	for (i = 0; i<n->numports_in) {
          buf[178+i] = n->dmx_in.status;
	}
	// 182 goodoutput (22)
	for (i = 0; i<n->numports_out) {
          buf[182+i] = n->dmx_out.status;
	}
}

void artnet_tx_pollreply_tpl(uint8_t *buf, artnet_node_t *n, uint32_t ip, uint8_t mac[6]) {
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
	write_uint32_hsb(buf+10, ip);
	//  14 port (4)
	write_uint16_lsb(buf+14, artnet_port);
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
	memcpy(buf+26, n->shortname, ARTNET_SHORT_NAME_LENGTH);
	//  44 longname (16)
	memcpy(buf+44, n->longname, ARTNET_LONG_NAME_LENGTH);
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
		buf[186+i] = (uint8_t) n->dmx_in.uni;
	}
	// 190 swout (24)
	for ( i=0; i < n->numports_out; i++ ) {
		buf[190+i] = (uint8_t) n->dmx_out.uni;
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
	memcpy(buf+200, mac, 6);
	// 207 bindip (38)
	/* bind ip is always this device */
	write_uint32_hsb(buf+207, ip);
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
}


