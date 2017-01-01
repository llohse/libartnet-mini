#include "artnet_net.h"
#include <string.h> // strncmp

# if linux
#include <arpa/inet.h>
#include <sys/socket.h>
#endif
# if chibios 
#endif

int socket_fd = 0;
const uint16_t artnet_port = 6454;

// send buffer
uint8_t txbuf[1000];

// receiver buffer
uint8_t rxbuf[1000];

void artnet_init(ipv4addr loc_ip) {
	sockaddr_in local_sa;
	memset(&local_sa, 0, sizeof(struct sockaddr_in));
	local_sa.sin_family = AF_INET;
	local_sa.sin_addr.s_addr = loc_ip;
	local_sa.sin_port = htons(artnet_port);

	socket_fd = socket(PF_INET, SOCK_DGRAM, 0);

	if (bind(socket_fd, (struct sockaddr *) &local_sa, sizeof(struct sockaddr_in)) != 0 ) {
		// ERROR
	}
}

void artnet_send(char *buf, size_t len, ipv4addr dst_ip) {
	ssize_t send_len;

	sockaddr_in remote_sa;
	memset(&remote_sa, 0, sizeof(struct sockaddr_in));
	remote_sa.sin_family = AF_INET;
	remote_sa.sin_addr.s_addr = dst_ip;
	remote_sa.sin_port = htons(artnet_port);

	send_len = sendto(socket_fd, buf, len, 0, (struct sockaddr *) &remote_sa, sizeof(struct sockaddr_in));

	if (send_len < len) {
		// ERROR
	} 
}

ssize_t artnet_recv(char *buf, size_t len, ipv4addr *src_ip) {
	sockaddr_in remote_sa;
	ssize_t recvlen;
	memset(&remote_sa, 0, sizeof(struct sockaddr_in));
	remote_sa.sin_family = AF_INET;
	socklen_t addrlen = sizeof(struct sockaddr_in);

	recvlen = recvfrom(socket_fd, buf, *len, 0, (struct sockaddr *) &remote_sa, &addrlen);
	*ipv4addr = remote_sa.sin_addr.s_addr;

	return recvlen;
}

void artnet_get_ip(uint32_t *dst) {
}

void artnet_get_mac(uint8_t *dst) {
}

static int main ... {

	// init systen
	
	// init socket
	
	// init data structures and state machine

	while (1) {
	// recv
	// opcode = parse into
	// send replies
	}
}
