#ifndef PROTOCOL_INCLUDE_CRC_H
#define PROTOCOL_INCLUDE_CRC_H

#include <stdio.h>
#include <stdint.h>

extern  char cal_crc8_table( char *ptr,  char len);
extern uint32_t crc32(uint32_t crc, const void *buf, size_t size);


#endif  /* PROTOCOL_INCLUDE_CRC_H */
