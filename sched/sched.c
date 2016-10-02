#include <kernel.h>
#include <sched/sched.h>
#include <init/init.h>
#include <lib/memory.h>

void sched_modinit()
{
	extern void my_thread_main();
	struct context _ctx;
	struct context *ctx = &_ctx;
	bzero_byte(ctx, sizeof(*ctx));
	ctx->cs = CODE_SELECTOR;
	ctx->eip = (r_t) my_thread_main;
	ctx->ss = ctx->ds = ctx->es = ctx->fs
	       = ctx->gs = DATA_SELECTOR;
	ctx->esp = ctx->ebp = 0x100400;
	ctx->eax = 0x13372C6E;
	ctx->ecx = 1024;
	ctx->efl = 0x1002;
	switch_to_context(ctx);
}

