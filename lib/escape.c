/****************************************
  > File Name: escape.c
  > Author: lqinggang
  > Email: 1944058861@qq.com
  > Create Time: Sat 08 Dec 2018 05:27:09 PM CST
  > Description: Data escaping and inversion
 ****************************************/


#include "escape.h"


void escape(const char *rscptr, char *dstptr, size_t *length)
{
	size_t i = 0;
	int len = 0;
	volatile unsigned char c;
	if(*length > 0) {
		c = *rscptr;
		while(i < *length ) {
			len++;
			if(*rscptr == header || *rscptr == tail || *rscptr == escapeByte) {
				(int)c++;
				*dstptr++ = escapeByte;
				len++;
			}
			*dstptr = c;
			rscptr++;
			dstptr++;
			i++;
			c = *rscptr;
		}
	}
	*length = len;
}

void reescape(const char *rscptr, char *dstptr, size_t *length)
{
	size_t i = 0;
	size_t len = 0;
	unsigned char c;
	if(*length > 0) {
		c = *rscptr;
		while(i < *length) {
			len++;
			if(escapeByte == c) {
				c = *(++rscptr);
				(int)c--;
				len--;
			}
			*dstptr = c;
			rscptr++;
			dstptr++;
			i++;
			c = *rscptr;
		}
	}
}
