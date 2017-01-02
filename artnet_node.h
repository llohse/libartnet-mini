# define FIRMWARE_VERSION 1

typedef struct artnet_controller_s {
	in_addr_t ip;
	uint8_t talktome;
	uint8_t priority;
	// TODO: keep track of last contact 
} artnet_controller_t;

typedef struct { uint8_t data[512]; } dmx_t;

typedef struct artnet_dmx_port_s {
	uint8_t uni;
	uint8_t seq;
	uint8_t status;

	*dmx_t dmx;
} artnet_dmx_port_t;

typedef struct artnet_node_s {
	artnet_dmx_port_t dmx_out[4];
	artnet_dmx_port_t dmx_in[4];

	size_t numports_in;
	size_t numports_out;

	uint8_t net; // The top 7 bits of the 15 bit Port-Address
	uint8_t sub; // Bits 7-4 of the 15 bit Port-Address

	uint8_t bindindex;

	/* status */
	uint8_t status1;
	uint8_t porttypes[4];
	uint8_t goodinput[4];
	uint8_t goodoutput[4];

	/* various information included in PollReply */
        uint8_t shortname[18];
        uint8_t longname[64];

	uint16_t oem;

	uint8_t ubea_ver;
	uint8_t esta_code[2];

	uint8_t *poll_reply_template;
} artnet_node_t;

typedef struct artnet_nodestack_s {
	size_t num;
	artnet_node_t *nodes;

	uint32_t ip;
	uint8_t mac[6];
} artnet_nodestack_t;

void artnet_init_node(artnet_node_t *n);
void artnet_init_port(artnet_dmx_port_t *p);
void artnet_init_nodestack(artnet_nodestack_t, size_t num);
