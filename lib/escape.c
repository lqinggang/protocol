/****************************************
  > File Name: escape.c
  > Author: lqinggang
  > Email: 1944058861@qq.com
  > Create Time: Sat 08 Dec 2018 05:27:09 PM CST
  > Description: Data escaping and reescaping
 ****************************************/


#include "escape.h"

/*
 * rscptr: the data need to be escape
 * dstptr: data after escape
 * length: length that need to be escape
 *         and return the length after escaping
 */
void escape(const char *rscptr, char *dstptr, size_t *length)
{
	volatile unsigned char c;
	if(*length > 0) {
		size_t i = 0;
		int len = 0;
		c = *rscptr; //get current character
		while(i < *length ) {
			len++;

			/* need to be escape */
			if(*rscptr == header || *rscptr == tail || *rscptr == escapeByte) {

				/* characters that need to be escaped plus 1
				 * and the escape character is inserted before it
				 */
				(int)c++;
				*dstptr++ = escapeByte;
				len++;
			}
			*dstptr = c;
			
			/* get the address of the next character
			 * and next save character
			 */
			rscptr++; 
			dstptr++;

			i++;
			c = *rscptr;
		}
		*length = len;
	}
}

/*
 * rscptr: the data need to be reescape
 * dstptr: data after reescape
 * length: length that need to be reescape 
 *         and return the length after reescaping
 */
void reescape(const char *rscptr, char *dstptr, size_t *length)
{
	if(*length > 0) {
		size_t i = 0;
		size_t len = 0;
		unsigned char c;
		c = *rscptr; //get current character
		while(i < *length) {
			len++;
			if(escapeByte == c) { // the next character need to be reescape

				/*
				 *  get the character that need to be reescape 
				 *  after escape character 
				 */
				c = *(++rscptr);

				/*
				 * restore back to the original character
				 */
				(int)c--;
				len--;
			}
			*dstptr = c;
	
			/* get the address of the next character
			 * and next save character
			 */
			rscptr++;
			dstptr++;

			i++;
			c = *rscptr; //get current character
		}
		*length = len;
	} 
}
