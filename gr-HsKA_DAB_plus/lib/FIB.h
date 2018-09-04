
#ifndef _FIB_H
#define _FIB_H 

#include <stdint.h>
#include <vector>
#include <algorithm>

#define FIB_DATA_FIELD_LENGTH 30
#define FIB_ENDMARKER 0xFF
#define FIB_PADDING 0x00

#define FIB_FIG_TYPE_MCI    0
#define FIB_FIG_TYPE_LABEL1 1
#define FIB_FIG_TYPE_LABEL2 2
#define FIB_FIG_TYPE_FIDC   5
#define FIB_FIG_TYPE_CA     6

#define FIB_MCI_EXTENSION_ENSEMBLE_INFO 0
#define FIB_MCI_EXTENSION_SUBCHANNEL_ORGA 1
#define FIB_MCI_EXTENSION_SERVICE_ORGA 2
#define FIB_MCI_EXTENSION_SERVICE_ORGA_PACKET_MODE 3
#define FIB_MCI_EXTENSION_SERVICE_ORGA_CA 4
#define FIB_SI_EXTENSION_SERVICE_COMP_LANGUAGE 5
#define FIB_SI_EXTENSION_SERVICE_LINKING_INFO 6
#define FIB_MCI_EXTENSION_CONFIG_INFO 7
#define FIB_MCI_EXTENSION_SERVICE_COMP_GLOBAL_DEFINITION 8
#define FIB_SI_EXTENSION_COUNTRY_LTO 9
#define FIB_SI_EXTENSION_TIME_AND_COUNTRYID 10
#define FIB_SI_EXTENSION_USER_APPLICATION_INFO 13
#define FIB_MCI_EXTENSION_SUBCHANNEL_PACKET_MODE_FEC 14
#define FIB_SI_EXTENSION_PROGRAMME_TYPE 17
#define FIB_SI_EXTENSION_ANNOUNCEMENT_SUPPORT 18
#define FIB_SI_EXTENSION_ANNOUNCEMENT_SWITCHING 19
#define FIB_SI_EXTENSION_SERVICE_COMP_INFO 20
#define FIB_SI_EXTENSION_FREQUENCY_INFO 21
#define FIB_SI_EXTENSION_OE_SERVICES 24
#define FIB_SI_EXTENSION_OE_ANNOUNCEMENT_SUPPORT 25
#define FIB_SI_EXTENSION_OE_ANNOUNCEMENT_SWITCHING 26

#define FIB_SI_EXTENSION_ENSEMBLE_LABEL 0
#define FIB_SI_EXTENSION_PROGRAMME_SERVICE_LABEL 1
#define FIB_SI_EXTENSION_SERVICE_COMP_LABEL 4
#define FIB_SI_EXTENSION_DATA_SERVICE_LABEL 5
#define FIB_SI_EXTENSION_XPAD_USER_APPLICATION_LABEL 6

#define FIB_FIDC_EXTENSION_PAGING 0
#define FIB_FIDC_EXTENSION_TMC    1
#define FIB_FIDC_EXTENSION_EWS    2

// Database IDs
#define DB_SUBCHANNEL_ORGA_ID_BASE 0
#define DB_SUBCHANNEL_ORGA_ID_LENGTH 256
#define DB_SUBCHANNEL_ORGA_ID(subchid) ((subchid)+DB_SUBCHANNEL_ORGA_ID_BASE)
#define DB_SUBCHANNEL_ORGA_ID_MIN (DB_SUBCHANNEL_ORGA_ID_BASE)
#define DB_SUBCHANNEL_ORGA_ID_MAX (DB_SUBCHANNEL_ORGA_ID_BASE+DB_SUBCHANNEL_ORGA_ID_LENGTH-1)

#define DB_DATE_AND_TIME_ID 256

#define DB_ENSEMBLE_LABEL_ID 257

#define DB_CONFIG_INFO_ID 258

#define DB_SERVICE_ENUMERATION_PTR_ID 259

#define DB_SELECTED_SUBCHANNEL_ID 260

#define DB_PROGRAM_SERVICE_LABEL_ID_BASE 65536
#define DB_PROGRAM_SERVICE_LABEL_ID_LENGTH 65536
#define DB_PROGRAM_SERVICE_LABEL_ID(service_id) ((service_id)+DB_PROGRAM_SERVICE_LABEL_ID_BASE)
#define DB_PROGRAM_SERVICE_LABEL_ID_MIN (DB_PROGRAM_SERVICE_LABEL_ID_BASE)
#define DB_PROGRAM_SERVICE_LABEL_ID_MAX (DB_PROGRAM_SERVICE_LABEL_ID_BASE+DB_PROGRAM_SERVICE_LABEL_ID_LENGTH-1)

#define DB_PROGRAM_TYPE_ID_BASE 131072
#define DB_PROGRAM_TYPE_ID_LENGTH 65536
#define DB_PROGRAM_TYPE_ID(service_id) ((service_id)+DB_PROGRAM_TYPE_ID_BASE)
#define DB_PROGRAM_TYPE_ID_MIN (DB_PROGRAM_TYPE_ID_BASE)
#define DB_PROGRAM_TYPE_ID_MAX (DB_PROGRAM_TYPE_ID_BASE+DB_PROGRAM_TYPE_ID_LENGTH-1)

#define DB_SERVICE_ORGA_ID_BASE 0x100000000ULL
#define DB_SERVICE_ORGA_ID_LENGTH 0x100000000ULL
#define DB_SERVICE_ORGA_ID(service_id) ((service_id)+DB_SERVICE_ORGA_ID_BASE)
#define DB_SERVICE_ORGA_ID_MIN (DB_SERVICE_ORGA_ID_BASE)
#define DB_SERVICE_ORGA_ID_MAX (DB_SERVICE_ORGA_ID_BASE+DB_SERVICE_ORGA_ID_LENGTH-1)

struct ServiceEnumeration
{
	std::vector<uint32_t> services;
	
	void add_service(uint32_t id)
	{
		if(std::find(services.begin(), services.end(), id) == services.end())
			services.push_back(id);
	};
} service_enum;

struct EnsembleInfo
{
	uint8_t country_id;
	uint16_t ensemble_reference;
	
	uint8_t change_flags;
	bool alarm_flag;
	
	uint8_t cif_count_high_part;
	uint8_t cif_count_low_part;
	
	uint8_t occurrence_change;
}ensemble_info;

struct SubChannelOrga
{
	uint8_t subchid;
	uint16_t start_address;
	bool form;
	
	bool table_switch;
	uint8_t table_index;
	
	uint8_t option;
	uint8_t protection_level;
	uint16_t sub_channel_size;
}subchannelorga[20];

struct ServiceOrga
{
	uint8_t ecc;
	uint8_t country_id;
	uint32_t service_reference;	
	uint32_t service_identifier;
	
	uint8_t caid;
	uint8_t ncomponents;
	
	uint8_t tmid[12];
	uint8_t scty[12];
	uint8_t subchid[12];
	uint16_t scid[12];
	bool ps[12];
	bool ca_flag[12];	
}serviceorga[6];

struct ServiceOrgaPacketMode
{
	uint16_t scid;
	bool caorg_flag;
	bool dg_flag;
	uint8_t dscty;
	uint8_t subchid;
	uint16_t packet_address;
	uint16_t caorg;
}serviceorgapacketmode[6];

struct ServiceOrgaStreamMode
{
	uint8_t subchid;
	uint16_t caorg;
}serviceorgastreammode[10];

struct ConfigInfo
{
	uint8_t services_field;
	uint16_t count_field;
}configinfo;

struct FECSubChannelOrga
{
	uint8_t subchid;
	uint8_t fec_scheme;
}fecsubchannelorga[15];


struct ServiceGlobalDefinition
{
	uint32_t sid;
	bool ext_flag;
	uint8_t scids;
	bool ls_flag;
	uint8_t subchid;
	uint8_t scid;
}serviceglobaldefinition[20];

struct EnsembleLabel
{
	uint16_t ensemble_identifier;
	uint8_t character_field[16];
	uint16_t character_flag_field;
}ensemblelabel[2];

struct ProgrammeServiceLabel
{
	uint16_t service_identifier;
	uint8_t character_field[16];
	uint16_t character_flag_field;
}programmeservicelabel[2];

struct DataServiceLabel
{
	uint32_t service_identifier;
	uint8_t character_field[32];
	uint16_t character_flag_field;
}dataservicelabel[2];

struct ServiceComponentLabel
{
	bool pd;
	uint8_t scids;
	uint32_t service_identifier;
}servicecomponentlabel[2];

struct XPadUserApplicationLabel
{
	bool pd;
	uint8_t scids;
	uint32_t service_identifier;
	uint8_t xpad_application_type;
}xpaduserapplicationlabel[2];

struct ProgrammeType
{
	uint16_t sid;
	bool sd;
	uint8_t international_code;
}programmetype[10];

struct DateandTime
{
	uint32_t mjd;
	bool lsi;
	bool utc_flag;
	uint8_t hours;
	uint8_t minutes;
	uint8_t seconds;
	uint16_t milliseconds;
}dateandtime;


const char * programmetypecode[32] = 
{
"No programme type",
"News",
"Current Affairs",
"Information",
"Sport",
"Education",
"Drama",
"Culture",
"Science",
"Varied",
"Pop Music",
"Rock Music",
"Easy Listening Music",
"Light Classical",
"Serious Classical",
"Other Music",
"Weather/meteorology",
"Finance/Business",
"Children‘s programmes",
"Social Affairs",
"Religion",
"Phone In",
"Travel",
"Leisure",
"Jazz Music",
"Country Music",
"National Music",
"Oldies Music",
"Folk Music",
"Documentary",
"Not used",
"Not used"
};

#endif /* _FIB_H */