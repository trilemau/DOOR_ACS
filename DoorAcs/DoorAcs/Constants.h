#pragma once

// Serial port constants
#define COM_PORT "COM3"
#define BAUD_RATE 9600
#define SERIAL_PORT_TIMEOUT 700

// Simple Protocol constants
#define MAX_RESPONSE_NUMBER_OF_BYTES 0xFF
#define PACKET_CARRIEGE_RETURN '\r'

// Tag Type constants
#define NO_LF_TAG_TYPES 0
#define NO_HF_TAG_TYPES 0
#define ALL_LF_TAG_TYPES 0xFFFFFFFF
#define ALL_HF_TAG_TYPES 0xFFFFFFFF

// Search Tag constants
#define SEARCH_TAG_MAX_ID_BYTES 16

// APDU command constants
#define HCE_SERVICE_AID "FF00000000000001"
#define DEFAULT_CLA 0
#define NO_PARAMETER 0
#define P1_SELECT_FILE 04
#define NO_RESPONSE_LENGTH 0
#define INS_SELECT_FILE 0xA4
#define INS_GET_RESPONSE 0xC0
#define APDU_NO_DATA {}
#define APDU_HEADER_LENGTH 5
#define APDU_STATUS_OK "9000"
#define APDU_STATUS_FAILED "6F00"

// API command numbers
#define COMMAND_NUMBER_SEARCH_TAG "0500"
#define COMMAND_NUMBER_SET_TAG_TYPES "0502"
#define COMMAND_NUMBER_CHECK_PRESENCE "1202"
#define COMMAND_NUMBER_ISO14443_4_TDX "1203"