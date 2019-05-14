#ifndef PROTOCOL_INCLUDE_ESCAPE_H
#define PROTOCOL_INCLUDE_ESCAPE_H

#include <stddef.h>


extern void escape(const  char *rscptr,  char *dstptr, size_t *length);
extern void reescape(const  char *rscptr,  char *dstptr, size_t *length);


#endif  /* PROTOCOL_INCLUDE_ESCAPE_H */
