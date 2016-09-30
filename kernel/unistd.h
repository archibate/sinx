#ifndef	_SINX_UNISTD_H
#define	_SINX_UNISTD_H

#ifndef	NULL
#define	NULL	((void *) 0)
#endif
#ifndef	EOF
#define	EOF	(-1)
#endif
#ifndef	TRUE
#define	TRUE	1
#endif
#ifndef	FALSE
#define	FALSE	0
#endif

#ifndef	_SINX_TYPEDEFS
#define	_SINX_TYPEDEFS
typedef unsigned int size_t;
typedef unsigned char u8;
typedef unsigned short u16;
typedef unsigned long u32;
typedef unsigned long long u64;
typedef signed char s8;
typedef signed short s16;
typedef signed long s32;
typedef signed long long s64;
typedef u32 r_t;
typedef void *vp_t;
typedef u8 *va_list_t;
typedef u8 byte_t;
typedef u16 word_t;
typedef u32 long_t;
typedef u64 qual_t;
#endif

#endif

