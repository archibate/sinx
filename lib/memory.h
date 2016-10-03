#ifndef	_SINX_LIB_MEMORY_H
#define	_SINX_LIB_MEMORY_H

#define	bzero_byte(dst, size) ({ \
	__asm__ ("movb $0, %%al;cld;rep;stosb" \
		:: "D" (dst), "c" (size)); \
	})
#define	bzero_long(dst, size) ({ \
	__asm__ ("xorl %%eax, %%eax;cld;rep;stosl" \
		:: "D" (dst), "c" (size)); \
	})
#define	memcpy_byte(dst, src, size) ({ \
	__asm__ ("cld;rep;movsb" \
		:: "D" (dst), "S" (src), \
		   "c" (size)); \
	})

#endif

