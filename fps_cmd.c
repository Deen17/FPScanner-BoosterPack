#include "fps.h"
#include <stdlib.h>

CommandPacket new_command_packet(word command, dword parameter){
    CommandPacket c;
    c.command = command;
    c.parameter = parameter;
    
    return c;
}

word cmd_checksum(CommandPacket *self){
    word sum = 0;
    sum += command_start_code1;
    sum += command_start_code2;
    sum += device_id1;
    sum += device_id2;
    sum += self->parameter & 0xff;
    sum += (self->parameter &0xff00 )>> 8;
    sum += (self->parameter & 0xff0000) >> 16;
    sum += (self->parameter & 0xff000000) >> 24;
    sum += (self->command >> 8) & 0x00ff;
    sum += (self->command) & 0x00ff;

    return sum;
}

CommandPacket open_cmd(dword parameter){
    return new_command_packet(OPEN, parameter);
}

CommandPacket close_cmd(){
    return new_command_packet(CLOSE, 0);
}

CommandPacket check_usb_cmd(){
    return new_command_packet(USB_INTERNAL_CHECK, 0);
}

CommandPacket led_cmd(dword parameter){
    return new_command_packet(CMOS_LED, parameter);
}

// Programming guide says to issue LED ON command prior to any capture. (p.15)
CommandPacket led_on_cmd(){
    return led_cmd(1);
}


CommandPacket led_off_cmd(){
    return led_cmd(0);
}

// the device is initially set to a Baud rate of 9600 bps after power on.
CommandPacket change_baud_cmd(dword parameter){
    return new_command_packet(CHANGE_BAUDRATE, parameter);
}

CommandPacket get_enroll_count_cmd(){
    return new_command_packet(GET_ENROLL_COUNT, 0);
}

// check the enrollment status of IDs from 0 to 2999
CommandPacket check_enrollment_cmd(dword id){
    return new_command_packet(CHECK_ENROLLED, id);
}

CommandPacket start_enrollment_cmd(dword id){
    return new_command_packet(ENROLL_START, id);
}

CommandPacket enroll1_cmd(){
    return new_command_packet(ENROLL1, 0);
}

CommandPacket enroll2_cmd(){
    return new_command_packet(ENROLL2, 0);
}

CommandPacket enroll3_cmd(){
    return new_command_packet(ENROLL3, 0);
}

CommandPacket check_finger_pressed_cmd(){
    return new_command_packet(IS_PRESS_FINGER,0);
}

CommandPacket delete_one_fingerprint_cmd(dword id){
    return new_command_packet(DELETE_ID, id);
}

CommandPacket delete_all_cmd(){
    return new_command_packet(DELETE_ALL, 0);
}

// 1:1 verification
CommandPacket verify_cmd(dword id){
    return new_command_packet(VERIFY, id);
}

/// 1:N identification
CommandPacket identify_cmd(){
    return new_command_packet(IDENTIFY, 0);
}

// if quality is nonzero, then device takes more time to capture image
// Use 0 for identification/verification, and 1 for enrollment 
CommandPacket capture_finger_cmd(dword quality){
    return new_command_packet(CAPTURE_FINGER,quality);
}

CommandPacket get_fingerprint_img_cmd(){
    return new_command_packet(GET_IMAGE, 0);
}

CommandPacket set_security_level_cmd(dword level){
    return new_command_packet(SET_SECURITY_LEVEL, level);
}

CommandPacket get_security_level_cmd(){
    return new_command_packet(GET_SECURITY_LEVEL, 0);
}

// to wake back up, send 0x00 first and then wait 20ms to wake up, then send the standard command
CommandPacket enter_standby_cmd(){
    return new_command_packet(ENTER_STANDBY_MODE, 0);
}



byte* command_packet_payload(CommandPacket *self){
    byte *payload = (byte*)malloc(12 * sizeof(byte));

    word checksum = cmd_checksum(self);

    payload[0] = command_start_code1; 
    payload[1] = command_start_code2;
    payload[2] = device_id1;
    payload[3] = device_id2;
    payload[4] = self->parameter & 0xff;
    payload[5] = (self->parameter &0xff00 )>> 8;
    payload[6] = (self->parameter & 0xff0000) >> 16;
    payload[7] = (self->parameter & 0xff000000) >> 24;
    payload[8] = self->command & 0xff;
    payload[9] = (self->command & 0xff00) >> 8;
    payload[10] = (checksum) & 0xff;
    payload[11] = (checksum & 0xff00) > 8;

    return payload;
}

