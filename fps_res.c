#include "fps.h"
#include <stdlib.h>

// not fully implemented to check all bytes
ResponsePacket new_response_packet(byte* buf){
    ResponsePacket res;
    res.parameter = buf[4] + (buf[5] << 4) + (buf[6] << 8 )+ (buf[7] << 12);
    res.response  = buf[8] + (buf[9] << 4);
    return res;
}

int isAck(ResponsePacket *res){
    return res->response == ACK;
}
