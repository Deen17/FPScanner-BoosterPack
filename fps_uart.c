#ifndef FPS_UART
#define FPS_UART
#ifndef TI_UART
#define TI_UART
#include <ti/drivers/UART.h>
#endif
#include "fps.h"

/*
 * fps_uart.c
 * TODO: add files to fps.h
 *
 *  Created on: Mar 16, 2021
 *      Author: Deen17
 */


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

int _fps_led(UART_Handle handle, unsigned int i){
    CommandPacket pkt = led_cmd(i);
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

int fps_led_on(UART_Handle handle){
    return _fps_led(handle, 1);
}

int fps_led_off(UART_Handle handle){
    return _fps_led(handle, 0);
}

#endif
