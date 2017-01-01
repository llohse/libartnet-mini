
#define ARTNET_PROTOCOL_VERSION 14
#define ARTNET_MAX_PORTS 4
#define ARTNET_MAC_SIZE 4
#define ARTNET_SHORTNAME_LENGTH 18 
#define ARTNET_LONGNAME_LENGTH 64
#define ARTNET_REPORT_LENGTH 64

const char[] artnet_id = { 'A', 'r', 't', '-', 'N', 'e', 't', '\0' }
const size_t artnet_id_len = 8;

const uint16_t artnet_protocol_version = 14;

enum artnet_opcode_t {
  ARTNET_OP_INVALID = 0x0000, // this is not part of the protocol
  ARTNET_OP_POLL = 0x2000,
  ARTNET_OP_POLLREPLY = 0x2100,
  ARTNET_OP_DIAGDATA = 0x2300,
  ARTNET_OP_COMMAND = 0x2400,
  ARTNET_OP_DMX = 0x5000,
  ARTNET_OP_NZS = 0x5100,
  ARTNET_OP_SYNC = 0x5200,
  ARTNET_OP_ADDRESS = 0x6000,
  ARTNET_OP_INPUT = 0x7000,
  ARTNET_OP_TODREQUEST = 0x8000,
  ARTNET_OP_TODDATA = 0x8100,
  ARTNET_OP_TODCONTROL = 0x8200,
  ARTNET_OP_RDM = 0x8300,
  ARTNET_OP_RDMSUB = 0x8400,
  ARTNET_OP_VIDEOSETUP = 0xa010,
  ARTNET_OP_VIDEOPALETTE = 0xa020,
  ARTNET_OP_VIDEODATA = 0xa040,
  ARTNET_OP_MACMASTER = 0xf000,
  ARTNET_OP_MACSLAVE = 0xf100,
  ARTNET_OP_FIRMWAREMASTER = 0xf200,
  ARTNET_OP_FIRMWAREREPLY = 0xf300,
  ARTNET_OP_FILETNMASTER = 0xf400,
  ARTNET_OP_FILEFNMASTER = 0xf500,
  ARTNET_OP_FILEFNREPLY = 0xf600,
  ARTNET_OP_IPPROG = 0xf800,
  ARTNET_OP_IPPROGREPLY = 0xf800,
  ARTNET_OP_MEDIA = 0x9000,
  ARTNET_OP_MEDIAPATCH = 0x9100,
  ARTNET_OP_MEDIACONTROL = 0x9200,
  ARTNET_OP_MEDIACONTRLREPLY = 0x9300,
  ARTNET_OP_TIMECODE = 0x9700,
  ARTNET_OP_TIMESYNC = 0x9800,
  ARTNET_OP_TRIGGER = 0x9900,
  ARTNET_OP_DIRECTORY = 0x9a00,
  ARTNET_OP_DIRECTORYREPLY = 0x9b00
};

enum artnet_rccode_t {
	ARTNET_RC_DEBUG = 0x0000,
	ARTNET_RC_POWEROK = 0x0001,
	ARTNET_RC_POWERFAIL = 0x0002,
	ARTNET_RC_SOCKETWR1 = 0x0003,
	ARTNET_RC_PARSEFAIL = 0x0004,
	ARTNET_RC_UDPFAIL = 0x0005,
	ARTNET_RC_SHNAMEOK = 0x0006,
	ARTNET_RC_LONAMEOK = 0x0007,
	ARTNET_RC_DMXERROR = 0x0008,
	ARTNET_RC_DMXUDPFULL = 0x0009,
	ARTNET_RC_DMXRXFULL = 0x000a,
	ARTNET_RC_SWITCHERR = 0x000b,
	ARTNET_RC_CONFIGERR = 0x000c,
	ARTNET_RC_DMXSHORT = 0x000d,
	ARTNET_RC_FIRMWAREFAIL = 0x000e,
	ARTNET_RC_USERFAIL = 0x000f
};

enum artnet_stcode_t { 
	ARTNET_ST_NODE = 0x00,
	ARTNET_ST_CONTROLLER = 0x01,
	ARTNET_ST_MEDIA = 0x02,
	ARTNET_ST_ROUTE = 0x03,
	ARTNET_ST_BACKUP = 0x04,
	ARTNET_ST_CONFIG = 0x05,
	ARTNET_ST_VISUAL = 0x06
};

enum artnet_status2_t {
	ARTNET_STATUS2_WEB = 0x01, 
	ARTNET_STATUS2_DHCPCONF = 0x02,
	ARTNET_STATUS2_DHCP = 0x04,
	ARTNET_STATUS2_LONGPORT = 0x08,
	ARTNET_STATUS2_SACN = 0x10
};

enum artnet_porttype_t {
	ARTNET_PORTTYPE_OUTPUT = 0x80,
	ARTNET_PORTTYPE_INPUT = 0x40,
	ARTNET_PORTTYPE_DMX512 = 0x00,
	ARTNET_PORTTYPE_MIDI = 0x01,
	ARTNET_PORTTYPE_AVAB = 0x02,
	ARTNET_PORTTYPE_COLORTRAN = 0x03,
	ARTNET_PORTTYPE_ADB = 0x04,
	ARTNET_PORTTYPE_ARTNET = 0x05
}



/// artnet/packets.h
enum { ARTNET_MAX_RDM_ADCOUNT = 32 };

enum { ARTNET_MAX_UID_COUNT = 200 };

// according to the rdm spec, this should be 278 bytes
// we'll set to 512 here, the firmware datagram is still bigger
enum { ARTNET_MAX_RDM_DATA = 512 };

enum { ARTNET_FIRMWARE_SIZE = 512 };

enum artnet_packet_type_e {
  ARTNET_POLL = 0x2000,
  ARTNET_REPLY = 0x2100,
  ARTNET_DMX = 0x5000,
  ARTNET_ADDRESS = 0x6000,
  ARTNET_INPUT = 0x7000,
  ARTNET_TODREQUEST = 0x8000,
  ARTNET_TODDATA = 0x8100,
  ARTNET_TODCONTROL = 0x8200,
  ARTNET_RDM = 0x8300,
  ARTNET_VIDEOSTEUP = 0xa010,
  ARTNET_VIDEOPALETTE = 0xa020,
  ARTNET_VIDEODATA = 0xa040,
  ARTNET_MACMASTER = 0xf000,
  ARTNET_MACSLAVE = 0xf100,
  ARTNET_FIRMWAREMASTER = 0xf200,
  ARTNET_FIRMWAREREPLY = 0xf300,
  ARTNET_IPPROG = 0xf800,
  ARTNET_IPREPLY = 0xf900,
  ARTNET_MEDIA = 0x9000,
  ARTNET_MEDIAPATCH = 0x9200,
  ARTNET_MEDIACONTROLREPLY = 0x9300
} PACKED;

typedef enum artnet_packet_type_e artnet_packet_type_t;

struct	artnet_poll_s {
  uint8_t  id[8];
  uint16_t opCode;
  uint8_t  verH;
  uint8_t  ver;
  uint8_t  ttm;
  uint8_t  pad;
} PACKED;

typedef struct artnet_poll_s artnet_poll_t;

struct artnet_reply_s {
  uint8_t  id[8];
  uint16_t opCode;
  uint8_t  ip[4];
  uint16_t port;
  uint8_t  verH;
  uint8_t  ver;
  uint8_t  subH;
  uint8_t  sub;
  uint8_t  oemH;
  uint8_t  oem;
  uint8_t  ubea;
  uint8_t  status;
  uint8_t  etsaman[2];
  uint8_t  shortname[ARTNET_SHORT_NAME_LENGTH];
  uint8_t  longname[ARTNET_LONG_NAME_LENGTH];
  uint8_t  nodereport[ARTNET_REPORT_LENGTH];
  uint8_t  numbportsH;
  uint8_t  numbports;
  uint8_t  porttypes[ARTNET_MAX_PORTS];
  uint8_t  goodinput[ARTNET_MAX_PORTS];
  uint8_t  goodoutput[ARTNET_MAX_PORTS];
  uint8_t  swin[ARTNET_MAX_PORTS];
  uint8_t  swout[ARTNET_MAX_PORTS];
  uint8_t  swvideo;
  uint8_t  swmacro;
  uint8_t  swremote;
  uint8_t  sp1;
  uint8_t  sp2;
  uint8_t  sp3;
  uint8_t  style;
  uint8_t  mac[ARTNET_MAC_SIZE];
  uint8_t  filler[32];
} PACKED;

typedef struct artnet_reply_s artnet_reply_t;

struct artnet_ipprog_s {
  uint8_t  id[8];
  uint16_t OpCode;
  uint8_t  ProVerH;
  uint8_t  ProVer;
  uint8_t  Filler1;
  uint8_t  Filler2;
  uint8_t  Command;
  uint8_t  Filler4;
  uint8_t  ProgIpHi;
  uint8_t  ProgIp2;
  uint8_t  ProgIp1;
  uint8_t  ProgIpLo;
  uint8_t  ProgSmHi;
  uint8_t  ProgSm2;
  uint8_t  ProgSm1;
  uint8_t  ProgSmLo;
  uint8_t  ProgPortHi;
  uint8_t  ProgPortLo;
  uint8_t  Spare1;
  uint8_t  Spare2;
  uint8_t  Spare3;
  uint8_t  Spare4;
  uint8_t  Spare5;
  uint8_t  Spare6;
  uint8_t  Spare7;
  uint8_t  Spare8;

} PACKED;

typedef struct artnet_ipprog_s artnet_ipprog_t;

struct artnet_ipprog_reply_s {
  uint8_t id[8];
  uint16_t  OpCode;
  uint8_t  ProVerH;
  uint8_t  ProVer;
  uint8_t  Filler1;
  uint8_t  Filler2;
  uint8_t  Filler3;
  uint8_t  Filler4;
  uint8_t  ProgIpHi;
  uint8_t  ProgIp2;
  uint8_t  ProgIp1;
  uint8_t  ProgIpLo;
  uint8_t  ProgSmHi;
  uint8_t  ProgSm2;
  uint8_t  ProgSm1;
  uint8_t  ProgSmLo;
  uint8_t  ProgPortHi;
  uint8_t  ProgPortLo;
  uint8_t  Spare1;
  uint8_t  Spare2;
  uint8_t  Spare3;
  uint8_t  Spare4;
  uint8_t  Spare5;
  uint8_t  Spare6;
  uint8_t  Spare7;
  uint8_t  Spare8;
} PACKED;

typedef struct artnet_ipprog_reply_s artnet_ipprog_reply_t;


struct artnet_address_s {
  uint8_t  id[8];
  uint16_t opCode;
  uint8_t  verH;
  uint8_t  ver;
  uint8_t  filler1;
  uint8_t  filler2;
  uint8_t  shortname[ARTNET_SHORT_NAME_LENGTH];
  uint8_t  longname[ARTNET_LONG_NAME_LENGTH];
  uint8_t  swin[ARTNET_MAX_PORTS];
  uint8_t  swout[ARTNET_MAX_PORTS];
  uint8_t  subnet;
  uint8_t  swvideo;
  uint8_t  command;
} PACKED;

typedef struct artnet_address_s artnet_address_t;


struct artnet_dmx_s {
  uint8_t  id[8];
  uint16_t opCode;
  uint8_t  verH;
  uint8_t  ver;
  uint8_t  sequence;
  uint8_t  physical;
  uint16_t  universe;
  uint8_t  lengthHi;
  uint8_t  length;
  uint8_t  data[ARTNET_DMX_LENGTH];
} PACKED;

typedef struct artnet_dmx_s artnet_dmx_t;


struct artnet_input_s {
  uint8_t id[8];
  uint16_t  opCode;
  uint8_t  verH;
  uint8_t  ver;
  uint8_t  filler1;
  uint8_t  filler2;
  uint8_t  numbportsH;
  uint8_t  numbports;
  uint8_t  input[ARTNET_MAX_PORTS];
} PACKED;

typedef struct artnet_input_s artnet_input_t;


struct artnet_todrequest_s {
  uint8_t  id[8];
  uint16_t opCode;
  uint8_t  verH;
  uint8_t  ver;
  uint8_t  filler1;
  uint8_t  filler2;
  uint8_t  spare1;
  uint8_t  spare2;
  uint8_t  spare3;
  uint8_t  spare4;
  uint8_t  spare5;
  uint8_t  spare6;
  uint8_t  spare7;
  uint8_t  spare8;
  uint8_t  command;
  uint8_t  adCount;
  uint8_t  address[ARTNET_MAX_RDM_ADCOUNT];
} PACKED;

typedef struct artnet_todrequest_s artnet_todrequest_t;



struct artnet_toddata_s {
  uint8_t  id[8];
  uint16_t opCode;
  uint8_t  verH;
  uint8_t  ver;
  uint8_t  rdmVer;
  uint8_t  port;
  uint8_t  spare1;
  uint8_t  spare2;
  uint8_t  spare3;
  uint8_t  spare4;
  uint8_t  spare5;
  uint8_t  spare6;
  uint8_t  spare7;
  uint8_t  spare8;
  uint8_t  cmdRes;
  uint8_t  address;
  uint8_t  uidTotalHi;
  uint8_t  uidTotal;
  uint8_t  blockCount;
  uint8_t  uidCount;
  uint8_t  tod[ARTNET_MAX_UID_COUNT][ARTNET_RDM_UID_WIDTH];
} PACKED;

typedef struct artnet_toddata_s artnet_toddata_t;

struct artnet_firmware_s {
  uint8_t  id[8];
  uint16_t opCode;
  uint8_t  verH;
  uint8_t  ver;
  uint8_t  filler1;
  uint8_t  filler2;
  uint8_t  type;
  uint8_t  blockId;
  uint8_t  length[4];
  uint8_t  spare[20];
  uint16_t  data[ARTNET_FIRMWARE_SIZE ];
} PACKED;

typedef struct artnet_firmware_s artnet_firmware_t;

struct artnet_todcontrol_s {
  uint8_t  id[8];
  uint16_t opCode;
  uint8_t  verH;
  uint8_t  ver;
  uint8_t  filler1;
  uint8_t  filler2;
  uint8_t  spare1;
  uint8_t  spare2;
  uint8_t  spare3;
  uint8_t  spare4;
  uint8_t  spare5;
  uint8_t  spare6;
  uint8_t  spare7;
  uint8_t  spare8;
  uint8_t  cmd;
  uint8_t  address;
} PACKED;


typedef struct artnet_todcontrol_s artnet_todcontrol_t;



struct artnet_rdm_s {
  uint8_t id[8];
  uint16_t  opCode;
  uint8_t  verH;
  uint8_t  ver;
  uint8_t  rdmVer;
  uint8_t  filler2;
  uint8_t  spare1;
  uint8_t  spare2;
  uint8_t  spare3;
  uint8_t  spare4;
  uint8_t  spare5;
  uint8_t  spare6;
  uint8_t  spare7;
  uint8_t  spare8;
  uint8_t  cmd;
  uint8_t  address;
  uint8_t  data[ARTNET_MAX_RDM_DATA];
} PACKED;


typedef struct artnet_rdm_s artnet_rdm_t;


struct artnet_firmware_reply_s {
  uint8_t  id[8];
  uint16_t opCode;
  uint8_t  verH;
  uint8_t  ver;
  uint8_t  filler1;
  uint8_t  filler2;
  uint8_t  type;
  uint8_t  spare[21];
} PACKED;

typedef struct artnet_firmware_reply_s artnet_firmware_reply_t;



// union of all artnet packets
typedef union {
  artnet_poll_t ap;
  artnet_reply_t ar;
  artnet_ipprog_t aip;
  artnet_address_t addr;
  artnet_dmx_t admx;
  artnet_input_t ainput;
  artnet_todrequest_t todreq;
  artnet_toddata_t toddata;
  artnet_firmware_t firmware;
  artnet_firmware_reply_t firmwarer;
  artnet_todcontrol_t todcontrol;
  artnet_rdm_t rdm;
} artnet_packet_union_t;


// a packet, containing data, length, type and a src/dst address
typedef struct {
  int length;
  struct in_addr from;
  struct in_addr to;
  artnet_packet_type_t type;
  artnet_packet_union_t data;
} artnet_packet_t;

typedef artnet_packet_t *artnet_packet;


#ifdef _MSC_VER
	#pragma pack(pop)
#endif

#undef PACKED
