ARTNETMINI_DIR = ../artnet-mini
ARTNETMINI_SRC = ${ARTNETMINI_DIR}/artnet_node.c \
		 ${ARTNETMINI_DIR}/artnet_net.c
ARTNETMINI_HDR = ${ARTNETMINI_DIR}/artnet.h \
		 ${ARTNETMINI_DIR}/artnet_node.h \
		 ${ARTNETMINI_DIR}/artnet_net.h

INCDIR = ${ARTNETMINI_DIR}


CSRC = ${ARTNETMINI_SRC} \
       linuxnode.c 

CC=gcc

CFLAGS=-Wall -D'ARTNET_DEBUG(...)=do {fprintf(stderr, \#\#__VA_ARGS__);} while(0)'

linuxnode: ${CSRC} ${ARTNETMINI_HDR}
	${CC} -o linuxnode ${CSRC} -I ${INCDIR} ${CFLAGS}
