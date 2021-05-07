#include "fps.h"
#include <stdlib.h>
#include <string.h>

/*
 * Function: devinfo_frombytes
 * ---------------------------
 * Constructs a `devinfo` struct from a series of bytes
 *
 * arr: a pointer to a 30-byte array
 *
 * returns: a devinfo packet
 */
struct devinfo devinfo_frombytes(byte* arr){
    struct devinfo packet;
    packet.data_start_code = arr[0] + (arr[1] << 8);
    packet.device_code     = arr[2] + (arr[3] << 8);
    packet.firmwareVersion = arr[4]
                                 + (arr[5] << 8)
                                 + (arr[6] << 16)
                                 + (arr[7] << 24);
    packet.isoAreaMaxSize  = arr[8]
                                 + (arr[9]  << 8)
                                 + (arr[10] << 16)
                                 + (arr[11] << 24);
    memcpy(packet.deviceSerialNumber, arr + 12, 16);
    packet.checksum        = arr[28] + (arr[29] << 8);
    free(arr);
    return packet;
}
