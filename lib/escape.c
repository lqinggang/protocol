#include "escape.h"
#include <stdio.h>
#include <string.h>
#include "protocol.h"

/*
 * rscptr: the data need to be escape
 * dstptr: data after escape
 * length: length that need to be escape
 *         and return the length after escaping
 */
void
escape(const  char *rscptr,  char *dstptr, size_t *length)
{
	volatile  char c;
	if(*length > 0)
    {
		size_t i = 0;
		int len = 0;
		c = *rscptr; //get current character
		while(i < *length ) 
        {
			len++;

			/* need to be escape */
			if(*(unsigned char *)rscptr == HEADER || *(unsigned char *)rscptr == TAIL || *(unsigned char *)rscptr == ESCAPEBYTE) 
            {

				/* characters that need to be escaped plus 1
				 * and the escape character is inserted before it
				 */
				(int)c++;
				*dstptr++ = ESCAPEBYTE;
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
void
reescape(const  char *rscptr,  char *dstptr, size_t *length)
{
	if(*length > 0)
    {
		size_t i = 0;
		size_t len = 0;
		unsigned char c;
		c = *(unsigned char *)rscptr; //get current character
		while(i < *length) 
        {
			len++;
			if(ESCAPEBYTE == c)  // the next character need to be reescape
            {

				/*
				 *  get the character that need to be reescape 
				 *  after escape character 
				 */
				c = *(unsigned char *)(++rscptr);

				/*
				 * restore back to the original character
				 */
				(int)c--;
				len--;
			}
			*(unsigned char *)dstptr = c;
	
			/* get the address of the next character
			 * and next save character
			 */
			rscptr++;
			dstptr++;

			i++;
			c = *(unsigned char *)rscptr; //get current character
		}
		*length = len;
	} 
}
