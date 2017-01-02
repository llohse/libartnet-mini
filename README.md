

## Code design:

network_ll.c contains functions to setup a udp socket, read and write to it and read out the interface ip and mac address.

artnet_node.h contains data structures for the state machine
artnet_node.c contains ways to manipulate these structures

artnet_net.c contains the protocol.
