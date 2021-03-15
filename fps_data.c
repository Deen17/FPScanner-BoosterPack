#include "fps.h"
#include <stdlib.h>
#include <string.h>

DataPacket data_packet(byte *buf, int size){
    DataPacket packet;
    packet.data = (byte*)malloc(sizeof(byte) * size);
    memcpy(packet.data, buf + 4, size);
    free(buf);
    return packet;
}

