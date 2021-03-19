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
 * TODO: add files to fps.h
 *
 *  Created on: Mar 16, 2021
 *      Author: Deen17
 */

int fps_open_info(UART_Handle handle){
    CommandPacket pkt = open_cmd(1);
    byte* buf = command_packet_payload(&pkt);
    int32_t status = UART_write(handle, buf, COMMAND_PACKET_SIZE);
    if (status != COMMAND_PACKET_SIZE){
        free(buf);
        return -1;
    }
    status = UART_read(handle, buf, RESPONSE_PACKET_SIZE); // reuse buf
    ResponsePacket res = new_response_packet(buf);
    if (res.checksum == expected_checksum(1, ACK) ){ // 0x55 + 0xaa + 0x1 + 0x30 = 304
        struct devinfo _info;
        byte data[30];
        status = UART_read(handle, data, 30);
        _info = devinfo_frombytes(&data);

        return 0;
    }
    return -1;
}

int fps_open(UART_Handle handle){
    CommandPacket pkt = open_cmd(0);
    byte* buf = command_packet_payload(&pkt);
    int32_t status = UART_write(handle, buf, COMMAND_PACKET_SIZE);
    if (status != COMMAND_PACKET_SIZE){
        free(buf);
        return -1;
    }
    status = UART_read(handle, buf, RESPONSE_PACKET_SIZE); // reuse buf
    ResponsePacket res = new_response_packet(buf);
    if (res.checksum == expected_checksum(0, ACK) ){ // 0x55 + 0xaa + 0x1 + 0x30 = 304
        return 0;
    }
    return -1;
}

int fps_close(UART_Handle handle){
    CommandPacket pkt = close_cmd(0);
    byte* buf = command_packet_payload(&pkt);
    int32_t status = UART_write(handle, buf, COMMAND_PACKET_SIZE);
    if (status != COMMAND_PACKET_SIZE){
        free(buf);
        return -1;
    }
    status = UART_read(handle, buf, RESPONSE_PACKET_SIZE); // reuse buf
    ResponsePacket res = new_response_packet(buf);
    if (res.checksum == expected_checksum(0, ACK)){
        return 0;
    }
    return -1;
}

int _fps_led(UART_Handle handle, dword i){
    CommandPacket pkt = led_cmd(i);
    byte* buf = command_packet_payload(&pkt);
    int32_t status = UART_write(handle, buf, COMMAND_PACKET_SIZE);
    free(buf);
    if (status != COMMAND_PACKET_SIZE){
        return -1;
    }
    int size;
    if (i == 1) size = RESPONSE_PACKET_SIZE;
    else size = 20;
    buf = (byte*)malloc(sizeof(byte) * RESPONSE_PACKET_SIZE);
    status = UART_read(handle, buf, RESPONSE_PACKET_SIZE); // reuse buf
    ResponsePacket res = new_response_packet(buf);
    if (res.checksum == expected_checksum(0, ACK)){
        return 0;
    }
    return -1;
}

int fps_led_on(UART_Handle handle){
    return _fps_led(handle, 1);
}

// TODO: why is the device sending back a devinfo data packet????
int fps_led_off(UART_Handle handle){
    return _fps_led(handle, 0);
}

int fps_check_usb(UART_Handle handle){
    CommandPacket pkt = check_usb_cmd();
    byte* buf = command_packet_payload(&pkt);
    int32_t status = UART_write(handle, buf, RESPONSE_PACKET_SIZE);
    free(buf);
    if (status != COMMAND_PACKET_SIZE) return -1;
    buf = (byte*)malloc(sizeof(byte) * RESPONSE_PACKET_SIZE);
    status = UART_read(handle, buf, RESPONSE_PACKET_SIZE); // reuse buf
    ResponsePacket res = new_response_packet(buf);
    if (res.checksum == expected_checksum(0x55, ACK)){
        return 0;
    }
    return -1;
}

#endif
