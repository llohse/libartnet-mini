typedef uint32_t ipv4_addr_t;

void artnet_init();

void artnet_send(size_t len, char *payload, ipaddress dst);

void artnet_recv(size_t len, char *payload, ipaddress src);
