/*
 *
 * Author Ouzw
 * Mail ouzw.mail@gmail.com
 * Date 2024-09-17 18:03:17
 *
 */
#ifndef _XDEF_H__
#define _XDEF_H__

#ifdef __cplusplus
extern "C" {
#endif

#include <stddef.h>
#include <stdlib.h>

typedef int xt_err;
#define XOK 0
#define XERROR -1
#define XERROR_MEMORY -2
#define XERROR_INVALID_PARAM -3

#define XTRUE 1
#define XFALSE 0

typedef char xt_bool;
typedef unsigned char xt_u8;
typedef unsigned short xt_u16;
typedef unsigned int xt_u32;
typedef unsigned long long xt_u64;
typedef char xt_s8;
typedef short xt_s16;
typedef int xt_s32;
typedef long long xt_s64;
typedef float xt_f32;
typedef double xt_f64;
typedef char *xt_string;
typedef const char *xt_cstring;

#define XMAX(x, y) ((x) > (y) ? (x) : (y))
#define XMIN(x, y) ((x) < (y) ? (x) : (y))

#define xt_malloc(s) malloc(s)
#define xt_realloc(p, s) realloc(p, s)
#define xt_free(p) free(p)

#ifdef __cplusplus
}
#endif

#endif /* _XDEF_H__ */

