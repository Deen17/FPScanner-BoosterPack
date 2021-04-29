#ifndef FPS_UART
#define FPS_UART

#include "fps.h"

#ifndef STDLIB
#define STDLIB
#include <stdlib.h>
#endif

/*
 * fps_uart.c
 *
 *  Created on: Mar 16, 2021
 *      Author: Deen17
 */


//int _req_res(UART_Handle handle, word command, dword id){
//    CommandPacket pkt = new_command_packet(command,id);
//    byte* buf = command_packet_payload(&pkt);
//    int32_t status = UART_write(handle, buf, RESPONSE_PACKET_SIZE);
//    if (status != COMMAND_PACKET_SIZE) {
//        free(buf);
//
//        return -1;
//    }
//    status = UART_read(handle, buf, RESPONSE_PACKET_SIZE);
//    if (status == RESPONSE_PACKET_SIZE){
//        ResponsePacket res = new_response_packet(buf);
//        if (res.response == ACK){
//            return 0;
//        }
//        if(res.response == NACK) return res.response;
//        return -1;
//    }
//    free(buf);
//    return -1;
//}


/*
 * Function: fps_open_info
 * -------------
 * A dev function that finds the devinfo data from the
 * fingerprint scanner, in addition to initializing communication.
 *
 * handle: a UART Handle according to the TI UART.h
 *
 * returns: 0 if successful, or nonzero if unsuccessful
 */
int fps_open_info(UART_Handle handle){
    CommandPacket pkt = open_cmd(1);
    byte* buf = command_packet_payload(&pkt);
    int32_t status = UART_write(handle, buf, COMMAND_PACKET_SIZE);
    if (status != COMMAND_PACKET_SIZE){
        free(buf);
        return -1;
    }
    status = UART_read(handle, buf, RESPONSE_PACKET_SIZE);
    if (status == RESPONSE_PACKET_SIZE){
        ResponsePacket res = new_response_packet(buf);
        if (res.checksum == expected_checksum(0, ACK) ){

            byte* data = (byte*)malloc(sizeof(byte) * 30);
            status = UART_read(handle, data, 30);
            struct devinfo info = devinfo_frombytes(data);

            return 0;
        }
        return -1;
    }
    free(buf);
    return -1;
}

/*
 * Function: fps_open
 * -------------
 * Initializes communication to the fingerprint scanner.
 *
 * handle: a UART Handle according to the TI UART.h
 *
 * returns: 0 if successful, or nonzero if unsuccessful
 */
int fps_open(UART_Handle handle){
    CommandPacket pkt = open_cmd(0);
    byte* buf = command_packet_payload(&pkt);
    int32_t status = UART_write(handle, buf, COMMAND_PACKET_SIZE);
    if (status != COMMAND_PACKET_SIZE){
        free(buf);
        return -1;
    }
    status = UART_read(handle, buf, RESPONSE_PACKET_SIZE);
    if (status == RESPONSE_PACKET_SIZE){
        ResponsePacket res = new_response_packet(buf);
        if (res.checksum == expected_checksum(0, ACK) ){
            return 0;
        }
        return -1;
    }
    free(buf);
    return -1;
}

/*
 * Function: fps_close
 * -------------
 * Initializes communication to the fingerprint scanner.
 *
 * handle: a UART Handle according to the TI UART.h
 *
 * returns: 0 if successful, or nonzero if unsuccessful
 */
int fps_close(UART_Handle handle){
    CommandPacket pkt = close_cmd(0);
    byte* buf = command_packet_payload(&pkt);
    int32_t status = UART_write(handle, buf, COMMAND_PACKET_SIZE);
    if (status != COMMAND_PACKET_SIZE){
        free(buf);
        return -1;
    }
    status = UART_read(handle, buf, RESPONSE_PACKET_SIZE);
    if(status == RESPONSE_PACKET_SIZE){
        ResponsePacket res = new_response_packet(buf);
        if (res.checksum == expected_checksum(0, ACK)){
            return 0;
        }
        return -1;
    }
    free(buf);
    return -1;
}

int _fps_led(UART_Handle handle, dword i){
    CommandPacket pkt = led_cmd(i);
    byte* buf = command_packet_payload(&pkt);
    int32_t status = UART_write(handle, buf, COMMAND_PACKET_SIZE);
    if (status != COMMAND_PACKET_SIZE){
        free(buf);
        return -1;
    }
    status = UART_read(handle, buf, RESPONSE_PACKET_SIZE);
    if (status == RESPONSE_PACKET_SIZE){
        ResponsePacket res = new_response_packet(buf);
        if (res.checksum == expected_checksum(0, ACK)){
            return 0;
        }
        return -1;
    }
    free(buf);
    return -1;
}

/*
 * Function: fps_led_on
 * -------------
 * Turns the LED backlight on.
 *
 * handle: a UART Handle according to the TI UART.h
 *
 * returns: 0 if successful, or nonzero if unsuccessful
 */
int fps_led_on(UART_Handle handle){
    return _fps_led(handle, 1);
}

/*
 * Function: fps_led_off
 * -------------
 * Turns the LED backlight off.
 *
 * handle: a UART Handle according to the TI UART.h
 *
 * returns: 0 if successful, or nonzero if unsuccessful
 */
int fps_led_off(UART_Handle handle){
    return _fps_led(handle, 0);
}

int fps_check_usb(UART_Handle handle){
    CommandPacket pkt = check_usb_cmd();
    byte* buf = command_packet_payload(&pkt);
    int32_t status = UART_write(handle, buf, RESPONSE_PACKET_SIZE);
    if (status != COMMAND_PACKET_SIZE) {
        free(buf);
        return -1;
    }
    status = UART_read(handle, buf, RESPONSE_PACKET_SIZE);
    if (status == RESPONSE_PACKET_SIZE){
        ResponsePacket res = new_response_packet(buf);
        if (res.checksum == expected_checksum(0x55, ACK)){
            return 0;
        }
        return -1;
    }
    free(buf);
    return -1;
}

/*
 * Returns the number of enrolled fingerprints, or -1 if an error occurred.
 */
int fps_enroll_count(UART_Handle handle){
    CommandPacket pkt = new_command_packet(GET_ENROLL_COUNT, 0);
    byte* buf = command_packet_payload(&pkt);
    int32_t status = UART_write(handle, buf, RESPONSE_PACKET_SIZE);
    if (status != COMMAND_PACKET_SIZE) {
        free(buf);
        return -1;
    }
    status = UART_read(handle, buf, RESPONSE_PACKET_SIZE);
    if (status == RESPONSE_PACKET_SIZE){
        ResponsePacket res = new_response_packet(buf);
        if (res.response == ACK){
            return res.parameter;
        }
        return -1;
    }
    free(buf);
    return -1;
}

result _req_res(UART_Handle handle, word command, dword id){
    CommandPacket pkt = new_command_packet(command,id);
    byte* buf = command_packet_payload(&pkt);
    result r;
    int32_t status = UART_write(handle, buf, RESPONSE_PACKET_SIZE);
    if (status != COMMAND_PACKET_SIZE) {
        free(buf);
        r = (result) {0,-1};
        return r;
    }
    status = UART_read(handle, buf, RESPONSE_PACKET_SIZE);
    if (status == RESPONSE_PACKET_SIZE){
        ResponsePacket res = new_response_packet(buf);
        if (res.response == ACK){
            r = (result) {res.parameter, 0};
            return r;
        }
        if(res.response == NACK) return (result) {res.parameter,-1};
        return (result) {0,-2};
    }
    free(buf);
    return (result) {0,-3};
}

result fps_check_enrolled(UART_Handle handle, dword id){
    return _req_res(handle, CHECK_ENROLLED, id);
}

result fps_enroll_start(UART_Handle handle, dword id){
    return _req_res(handle, ENROLL_START, id);
}

result fps_enrolln(UART_Handle handle, int n){
    switch(n){
    case 1:
        return _req_res(handle, ENROLL1, 0);
    case 2:
        return _req_res(handle, ENROLL2, 0);
    case 3:
        return _req_res(handle, ENROLL3, 0);
    default:
        return (result) {0,-4};
    }

}

// returns 1 if finger is pressed, and 0 if not.
int fps_is_finger_pressed_uart(UART_Handle handle){
    result r = _req_res(handle, IS_PRESS_FINGER, 0);
    return r.res == 0 && !r.parameter;
}


result fps_delete_id(UART_Handle handle, dword id){
    return _req_res(handle, DELETE_ID, id);
}


result fps_delete_all(UART_Handle handle){
    return _req_res(handle, DELETE_ALL, 0);
}

result fps_verify(UART_Handle handle, dword id){
    return _req_res(handle, VERIFY, id);
}

result fps_identify(UART_Handle handle){
    return _req_res(handle, IDENTIFY, 0);
}

// quality 0 = not best image, but fast. use for identification/verification
// quality nonzero = best image quality, but slow. use for enrollment
result fps_capture_finger(UART_Handle handle, dword quality){
    return _req_res(handle, CAPTURE_FINGER, quality);
}

int fps_standby(UART_Handle);

/* returns whether the ICPCK pin is high or not. If the fingerprint scanner
 * senses a finger is touching the scanner, it returns 1. Otherwise, 0 is returned
 */
int icpck(){
    return GPIO_read(ICPCK);
}

#endif
