#ifndef _SINX_SCHED_SCHED_H
#define _SINX_SCHED_SCHED_H

struct context {
	r_t	gs, fs, es, ds, edi, esi, ebp, new_esp,
		ebx, edx, ecx, eax, ret_addr, unused,
		eip, cs, efl, esp, ss;
};	/* 结构体的大小是 76 字节 */

void sched_modinit();
void save_my_context(struct context *ctx);
void switch_to_context(struct context *ctx);

#endif

