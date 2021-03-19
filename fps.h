///
//
// 

#ifndef FPS_HEADER
#define FPS_HEADER

#ifndef TI_UART
#define TI_UART
#include <ti/drivers/UART.h>
#endif

#define COMMAND_PACKET_SIZE 12
#define RESPONSE_PACKET_SIZE 12

typedef unsigned char byte;
typedef unsigned short word;
typedef unsigned long dword;

static byte command_start_code1 = 0x55;
static byte command_start_code2 = 0xaa;
static byte data_start_code1 = 0x5a;
static byte data_start_code2 = 0xa5;
static byte device_id1 = 0x01; // lsB; together, deviceID is 0x0001. the fingerprintscanner is little endian
static byte device_id2 = 0x00; // msB

// Command Codes
enum Command_Code
{
    OPEN = 0x01,
    CLOSE = 0x02,
    USB_INTERNAL_CHECK = 0X03,
    CHANGE_BAUDRATE = 0X04,
    CMOS_LED = 0X12,
    GET_ENROLL_COUNT = 0X20,
    CHECK_ENROLLED = 0X21,
    ENROLL_START = 0X22,
    ENROLL1 = 0X22,
    ENROLL2 = 0X24,
    ENROLL3 = 0X25,
    IS_PRESS_FINGER = 0X26,
    DELETE_ID = 0X40,
    DELETE_ALL = 0X41,
    VERIFY = 0X50,
    IDENTIFY = 0X51,
    VERIFY_TEMPLATE = 0X52,
    IDENTIFY_TEMPLATE = 0X53,
    CAPTURE_FINGER = 0X60,
    MAKE_TEMPLATE = 0X61,
    GET_IMAGE = 0X62,
    GET_RAW_IMAGE = 0X63,
    GET_TEMPLATE = 0X70,
    SET_TEMPLATE = 0X71,
    GET_DATABASE_START = 0X72,
    GET_DATABASE_END = 0X73,
    SET_SECURITY_LEVEL = 0XF0,
    GET_SECURITY_LEVEL = 0XF1,
    IDENTIFY_TEMPLATE2 = 0XF4,
    ENTER_STANDBY_MODE = 0XF9,
    ACK = 0X30,
    NACK = 0X31
} Command;

enum ERROR_CODES
{
    NACK_TIMEOUT = 0X1001,
    NACK_INVALID_BAUDRATE = 0X1002,
    NACK_INVALID_POS = 0X1003,
    NACK_IS_NOT_USED = 0X1004,
    NACK_IS_ALREADY_USED = 0X1005,
    NACK_COMM_ERR = 0X1006,
    NACK_VERIFY_FAILED = 0X1008,
    NACK_DB_IS_FULL = 0X1009,
    NACK_DB_IS_EMPTY = 0X100A,
    NACK_TURN_ERR = 0X100B,
    NACK_BAD_FINGER = 0X100C,
    NACK_ENROLL_FAILED = 0X100D,
    NACK_IS_NOT_SUPPORTED = 0X100E,
    NACK_DEV_ERR = 0X100F,
    NACK_CAPTURE_CANCELED = 0X1010,
    NACK_INVALID_PARAM = 0X1011,
    NACK_FINGER_IS_NOT_PRESSED = 0X1012,
    INVALID = 0XFFFF
} Error_Code;

// // Response Codes
// enum Response_Code {
//     ACK  = 0x30,
//     NACK = 0x31
// } Response;

// Packet Structures

// Command Packet
typedef struct CMD_PKT
{
    dword parameter;
    word command;
} CommandPacket;

CommandPacket new_command_packet(word command, dword parameter);
word cmd_checksum(CommandPacket *self);
CommandPacket open_cmd(dword parameter);
CommandPacket close_cmd();
CommandPacket check_usb_cmd();
CommandPacket led_cmd(dword parameter);
CommandPacket led_on_cmd();
CommandPacket led_off_cmd();
CommandPacket change_baud_cmd(dword parameter);
CommandPacket get_enroll_count_cmd();
CommandPacket check_enrollment_cmd(dword id);
CommandPacket start_enrollment_cmd(dword id);
CommandPacket enroll1_cmd();
CommandPacket enroll2_cmd();
CommandPacket enroll3_cmd();
CommandPacket check_finger_pressed_cmd();
CommandPacket delete_one_fingerprint_cmd(dword id);
CommandPacket delete_all_cmd();
CommandPacket verify_cmd(dword id);
CommandPacket identify_cmd();
CommandPacket capture_finger_cmd(dword quality);
CommandPacket get_fingerprint_img_cmd();
CommandPacket set_security_level_cmd(dword level);
CommandPacket get_security_level_cmd();
CommandPacket enter_standby_cmd();


byte* command_packet_payload(CommandPacket *self);


// Response Packet

typedef struct RES_PKT
{
    word start_code;
    word device_id;
    dword parameter;
    word response;
    word checksum;
} ResponsePacket;

ResponsePacket new_response_packet(byte* buf);
int isAck(ResponsePacket *res);
int expected_checksum(dword parameter, word response);

// Data Packets

//typedef struct DATA_PACKET
//{
//    byte start1;
//    byte start2;
//    word dev_id;
//    byte *data;
//    word checksum;
//    unsigned int size;
//} DataPacket;

//DataPacket data_packet(byte *buf, int size);
//word data_checksum(DataPacket *self);


/*
 * Struct: devinfo
 * ---------------
 * The FPS returns this data after sending an OPEN
 * command with a nonzero parameter.
 */
struct devinfo{
    word data_start_code;
    word device_code;
    dword firmwareVersion;
    dword isoAreaMaxSize;
    byte deviceSerialNumber[16];
    word checksum;
};

struct devinfo devinfo_frombytes(byte* arr);

// FPS UART commands
int fps_open_info(UART_Handle handle);
int fps_open(UART_Handle handle);
int fps_close(UART_Handle handle);
int _fps_led(UART_Handle handle, dword i);
int fps_led_on(UART_Handle handle);
int fps_led_off(UART_Handle handle);
int fps_check_usb(UART_Handle handle);

#endif

