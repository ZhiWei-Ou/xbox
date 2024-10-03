/*
 * © 2024 [ZhiWei-Ou]. All rights reserved.
 * @file xvector.c
 * @author Ouzw
 * @mail ouzw.mail@gmail.com
 * @date 2024-10-03 09:10:44

 * @brief 
 */
#include "xvector.h"
#include "xdef.h"

void xvec_expand(char **data, int *length, int *capacity, int memsz) {
  if (*length + 1 > *capacity) {
    if (*capacity == 0) {
      *capacity = 1;
    } else {
      *capacity <<= 1;
    }
    *data = xt_realloc(*data, *capacity * memsz);
  }
}

void xvec_splice(char **data,
                        int *length,
                        int *capacity,
                        int memsz,
                        int start,
                        int count) {
  (void) capacity;
  memmove(*data + start * memsz,
          *data + (start + count) * memsz,
          (*length - start - count) * memsz);
}
