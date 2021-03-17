#include "fps.h"
#include <stdlib.h>

ResponsePacket new_response_packet(byte *buf){
    ResponsePacket res;
    res.start_code = buf[0]
                     + (buf[1] << 8);
    res.device_id = buf[2]
                        + (buf[3] << 8);
    res.parameter = buf[4]
                    + (buf[5] << 8)
                    + (buf[6] << 16)
                    + (buf[7] << 24);
    res.response =  buf[8]
                    + (buf[9] << 8);
    res.checksum = buf[10]
                       + (buf[11] << 8);
    free(buf);
    return res;
}

int isAck(ResponsePacket *res){
    return res->response == ACK;
}

int expected_checksum(dword parameter, word response){
    return 255 + parameter + response;
}
