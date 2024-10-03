/*
 * © 2024 [ZhiWei-Ou]. All rights reserved.
 * @file xvector.h
 * @author Ouzw
 * @mail ouzw.mail@gmail.com
 * @date 2024-10-03 09:08:51
 *
 * @brief 
 */
#ifndef __XVECTOR_H__
#define __XVECTOR_H__

#ifdef __cplusplus
extern "C" {
#endif

#include "xdef.h"
#include <string.h>


#define XVector(T)\
  struct { T *data; int length, capacity; }

#define XVectorUnpack(v)\
  (char**)&(v)->data, &(v)->length, &(v)->capacity, sizeof(*(v)->data)

#define XVectorInit(v)\
  memset((v), 0, sizeof(*(v)))

#define XVectorFini(v)\
    do { \
        xt_free((v)->data); \
        (v)->length = (v)->capacity = 0; \
    } while(0)

#define XVectorClear(v)\
  ((v)->length = 0)

#define XVectorPush(v, val)\
  ( xvec_expand(XVectorUnpack(v)),\
    (v)->data[(v)->length++] = (val) )

#define XVectorSplice(v, start, count)\
  ( xvec_splice(vec_unpack(v), start, count),\
    (v)->length -= (count) )

#define XVectorLen(v)\
  ((v)->length)

#define XVectorCap(v)\
  ((v)->capacity)
    
#define XVectorAt(v, index)\
  ((v)->data[index])

#define XVectorForEach(v, index) \
    for (int index = 0; index < (v)->length; ++index)



void xvec_splice(char **data,
                        int *length,
                        int *capacity,
                        int memsz,
                        int start,
                        int count);

void xvec_expand(char **data, int *length, int *capacity, int memsz);

#ifdef __cplusplus
}
#endif
#endif /* __XVECTOR_H__ */
