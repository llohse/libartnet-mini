#include <sys/types.h>
#include "artnet.h"

#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/socket.h>
#include <string.h> // memset


uint8_t mac[6] = { 0x02, 0x00, 0x00, 0x00, 0x00 };
struct in_addr ip;

int socket_fd = 0;

artnet_node_t node;

#define RXBUF_SIZE 1024
uint8_t rxbuf[RXBUF_SIZE];

void network_init_artnet(void) {
	struct sockaddr_in local_sa;
	memset(&local_sa, 0, sizeof(struct sockaddr_in));
	local_sa.sin_family = AF_INET;
	local_sa.sin_addr.s_addr = INADDR_ANY;
	local_sa.sin_port = htons(ARTNET_PORT);

	socket_fd = socket(PF_INET, SOCK_DGRAM, 0);

	if (bind(socket_fd, (struct sockaddr *) &local_sa, sizeof(struct sockaddr_in)) != 0 ) {
		// ERROR
	}
}

void network_send_artnet(artnet_packet_t *packet) {
	struct sockaddr_in remote_sa;
	ssize_t send_len;

	memset(&remote_sa, 0, sizeof(struct sockaddr_in));
	remote_sa.sin_family = AF_INET;
	remote_sa.sin_addr.s_addr = packet->ip;
	remote_sa.sin_port = htons(ARTNET_PORT);

	send_len = sendto(socket_fd, packet->data, packet->len, 0, (struct sockaddr *) &remote_sa, sizeof(struct sockaddr_in));

	if (send_len < packet->len) {
		// something went wrong
		return;
	} 

	return;
}


void network_recv_artnet(artnet_packet_t *packet) {
	struct sockaddr_in remote_sa;
	ssize_t len;

	memset(&remote_sa, 0, sizeof(struct sockaddr_in));
	remote_sa.sin_family = AF_INET;
	socklen_t addrlen = sizeof(struct sockaddr_in);

	packet->data = rxbuf;

	len = recvfrom(socket_fd, rxbuf, RXBUF_SIZE, 0, (struct sockaddr *) &remote_sa, &addrlen);

	packet->ip = remote_sa.sin_addr.s_addr;
	packet->data = rxbuf;
	packet->len = (uint16_t) len;
}

/* main thread */
int main(int argc, char **argv) {
	// read mac and ip
	/*
	if (argc < 3) {
		printf("Not enough arguments!\n");
		return 1;
	}
	*/
	inet_aton("10.10.4.127", &ip);
	mac[0] = 0xc8;
	mac[1] = 0x5b;
	mac[2] = 0x76;
	mac[3] = 0x45;
	mac[4] = 0x56;
	mac[5] = 0x4b;

	// init systen
	
	// init udp socket
	network_init_artnet();
	
	// init data structures and state machine
	artnet_init_node(&node, 0, 1);
	node.ip = ip.s_addr;
	memcpy(node.mac, mac, 6);
	strcpy((char *) node.shortname, "libartnet-mini");

	artnet_network_init(&node);

	while (1) {
		artnet_packet_t rxpacket;
		artnet_packet_t txpacket;

		struct in_addr dstip;

		txpacket.len = 0;

		network_recv_artnet(&rxpacket);

		artnet_handle_packet(&node, &rxpacket, &txpacket);

		dstip.s_addr = txpacket.ip;

		ARTNET_DEBUG("Sending %d bytes to %s\n", txpacket.len, inet_ntoa(dstip));

		if (txpacket.len > 0) {
			network_send_artnet(&txpacket);
		}
	}
}
