
#define ARTNET_PROTOCOL_VERSION 14
#define ARTNET_MAX_PORTS 4
#define ARTNET_MAC_SIZE 4
#define ARTNET_SHORTNAME_LENGTH 18 
#define ARTNET_LONGNAME_LENGTH 64
#define ARTNET_REPORT_LENGTH 64

const uint8_t[] artnet_id = { 'A', 'r', 't', '-', 'N', 'e', 't', '\0' };
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
};

/* packet lengths */
const size_t artnet_len_netpoll = 14;
const size_t artnet_len_address = 14;
const size_t artnet_len_dmx_header = 18;
const size_t artnet_len_pollreply = 239;
