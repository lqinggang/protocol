/*****************************************************************************
File Name:    crc.h
Description:  generate CRC check code 
History:
Date                Author                   Description
2017-03-13         Lucien                    Creat
****************************************************************************/

#ifndef __CRC_H__
#define __CRC_H__

#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>


unsigned char cal_crc8_table(unsigned char *ptr, unsigned char len);
uint32_t crc32(uint32_t crc, const void *buf, size_t size);


#endif
