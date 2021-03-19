#ifndef FPS_UART
#define FPS_UART
#ifndef TI_UART
#define TI_UART
#include <ti/drivers/UART.h>
#endif
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
        if (res.checksum == expected_checksum(1, ACK) ){
            struct devinfo _info;
            byte* data = (byte*)malloc(sizeof(byte) * 30);
            status = UART_read(handle, data, 30);
            _info = devinfo_frombytes(&data);

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

#endif
