#include "artnet_net.h"
#include <string.h> // strncmp


uint8_t mac[6] = { 0x02, 0x00, 0x00, 0x00, 0x00 };
uint32_t ip;

int socket_fd = 0;
const uint16_t artnet_port = 6454;

/* main thread */
static int main ... {

	// init systen
	
	
	// init socket
	artnet_network_init();
	
	// init data structures and state machine

	while (1) {
		uint8_t *rxbuf;
		size_t len;
		ip_addr_t src_ip;
		artnet_network_rx_recv(&rxbuf, &len, &src_ip);
		artnet_handle_packet(rxbuf, len, src_ip);
		artnet_network_rx_free(uint8_t *buf);
	}
}
