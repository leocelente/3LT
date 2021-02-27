#ifndef BSP_H_
#define BSP_H_

#include "ipc.h"
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

extern comm_t gcomm;

static inline int bsp_transmit(int address, char *data, int len, char* buffer, int sz) {
    uint8_t response[COMM_BUFF_SZ] = {0};
    uint8_t packet[COMM_BUFF_SZ] = { (uint8_t)address, (uint8_t)len };
    memcpy(&packet[2], data, len);
    comm_write(&gcomm, (uint8_t*)packet, COMM_BUFF_SZ);
    comm_read(&gcomm, response, COMM_BUFF_SZ);
    memcpy(buffer, response, sz);
    return 1;
}

#endif
