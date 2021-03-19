#include "fps.h"
#include <stdlib.h>
#include <string.h>

//DataPacket data_packet(byte *buf, int size){
//    DataPacket packet;
//    packet.data = (byte*)malloc(sizeof(byte) * size);
//    memcpy(packet.data, buf + 4, size);
//    free(buf);
//    return packet;
//}
//

struct devinfo devinfo_frombytes(byte* arr){
    struct devinfo packet;
    packet.data_start_code = arr[0] + (arr[1] << 8);
    packet.device_code = arr[2] + (arr[3] << 8);
    packet.firmwareVersion = arr[4]
                                 + (arr[5] << 8)
                                 + (arr[6] << 16)
                                 + (arr[7] << 24);
    packet.isoAreaMaxSize = arr[8]
                                 + (arr[9] << 8)
                                 + (arr[10] << 16)
                                 + (arr[11] << 24);
    memcpy(packet.deviceSerialNumber, arr + 12, 16);
//    free(arr);
    return packet;
}
