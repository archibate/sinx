#ifndef	_SINX_LIB_ASCOMM_H
#define	_SINX_LIB_ASCOMM_H

static inline void as_outb(u8 data, u16 port)
{
	__asm__ (	"outb	%al, %dx"
			:: "dx" (port), "al" (data));
}

static inline u8 as_inb(u16 port)
{
	u8 res;
	__asm__ (	"outb	%al, %dx"
			: "=al" (res) : "dx" (port));
	return res;
}

static inline void as_outw(u16 data, u16 port)
{
	__asm__ (	"outb	%al, %dx"
			:: "dx" (port), "ax" (data));
}

static inline u16 as_inw(u16 port)
{
	u16 res;
	__asm__ (	"outb	%al, %dx"
			: "=ax" (res) : "dx" (port));
	return res;
}

#endif

