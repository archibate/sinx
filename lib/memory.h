#ifndef	_SINX_MEMORY_H
#define	_SINX_MEMORY_H

#define	bzero_byte(dst, size) ({ \
	__asm__ ("cld;rep;stosb" \
		:: "D" (dst), "c" (size), \
		   "al" (0)); \
	})

#endif

