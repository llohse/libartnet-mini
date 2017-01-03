#include "ch.h"

#include "lwip/opt.h"
#include "lwip/arch.h"
#include "lwip/api.h"

#include "artnet.h"

/**
 * Stack area for the artnet thread.
 */
THD_WORKING_AREA(wa_artnet_handler, ARTNET_THREAD_STACK_SIZE);


/**
 * Art-Net thread.
 */
THD_FUNCTION(artnet_handler, p) {
  struct netconn *conn;
  err_t err;

  struct netbuf *buf;


  (void)p;
  chRegSetThreadName("artnet");

  // setup socket/connection 
  /* Create a new TCP connection handle */
  conn = netconn_new(NETCONN_UDP);
  LWIP_ERROR("artnet_handler: invalid conn", (conn != NULL), chThdExit(MSG_RESET););

  /* Bind to port ARTNET_PORT with default IP address */
  netconn_bind(conn, IP_ADDR_ANY, ARTNET_PORT);

  /* Goes to the final priority after initialization.*/
  chThdSetPriority(ARTNET_THREAD_PRIORITY);

	// init data structures and state machine
	artnet_init_node(&node, 0, 1);
	node.ip = ip;
	memcpy(node.mac, mac, 6);
	strcpy(node.shortname, "libartnet-mini");

	artnet_network_init(&node);

	while (1) {
		err_t recv_err, send_err;
		struct ip_addr *src_addr;

		artnet_packet_t rxpacket;
		artnet_packet_t txpacket;

		recv_err = netconn_recv(conn, &buf);

		if (recv_err != ERR_OK) {
			continue;
		}

		addr = netbuf_fromaddr(buf);
		netbuf_data(buf, &(rxpacket.data), &(rxpacket.len));

		/* supposedly this is already network byte order */
		rxpacket.ip = addr->addr;

		/* yes yes... the api could be improved */
		txpacket.len = 0;

		/* this is where all the artnet magic happens */
		artnet_handle_packet(&node, &rxpacket, &txpacket);

		/* free the receive buffer - this way it is 0-copy */
		netbuf_delete(buf);

		if (txpacket.len > 0) {
			netbuf * sendbuf;
			ip_addr_t dst_addr;

			sendbuf = netbuf_new();
			dst_addr.addr = txpacket.ip;
			netbuf_ref(sendbuf, txpacket.data, txpacket.len);

			send_err = netconn_sendto(conn, sendbuf, &dst_addr, ARTNET_PORT);

			/* delete the netbuf - does not free the ref'd data (as it is static) */
			netbuf_delete( sendbuf );
		}

	}
}
