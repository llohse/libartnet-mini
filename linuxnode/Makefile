ARTNETMINI_DIR = ../artnet-mini
ARTNETMINI_SRC = ${ARTNETMINI_DIR}/artnet_node.c \
		 ${ARTNETMINI_DIR}/artnet_net.c

INCDIR = ${ARTNETMINI_DIR}


CSRC = ${ARTNETMINI_SRC} \
       linuxnode.c 

CC=gcc

linuxnode: ${CSRC}
	${CC} -o linuxnode ${CSRC} -I ${INCDIR}
