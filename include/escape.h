/****************************************
   > File Name: escape.h
   > Author: lqinggang
   > Email: 1944058861@qq.com
   > Create Time: Sat 08 Dec 2018 05:24:23 PM CST
****************************************/



#ifndef ESCAPE_H
#define ESCAPE_H

#include <stdio.h>
#include <string.h>
#include "protocol.h"



static unsigned char header = 0xAA;
static unsigned char tail = 0x55;
static unsigned char escapeByte = 0xCC;



void escape(const char *rscptr, char *dstptr, size_t *length);
void reescape(const char *rscptr, char *dstptr, size_t *length);


#endif


