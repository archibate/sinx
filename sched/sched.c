#include <kernel.h>
#include <sched/sched.h>
#include <init/init.h>
#include <lib/memory.h>

struct task _task_a;
struct task *task_a = &_task_a;
struct task _task_b;
struct task *task_b = &_task_b;

void sched_modinit()
{
	extern void task_a_main();
	extern void task_b_main();
	bzero_byte(task_a, sizeof(*task_a));
	bzero_byte(task_b, sizeof(*task_b));
	task_a->ctx.cs = CODE_SELECTOR;
	task_a->ctx.eip = (r_t) task_a_main;
	task_a->ctx.ss = task_a->ctx.ds = task_a->ctx.es
	       = task_a->ctx.fs = task_a->ctx.gs = DATA_SELECTOR;
	task_a->ctx.esp = task_a->ctx.ebp = 0x100400;
	task_a->ctx.eax = 0x13372C6E;
	task_a->ctx.ecx = 1024;
	task_a->ctx.efl = 0x1002;
	memcpy_byte(&task_b->ctx, &task_a->ctx, sizeof(task_b->ctx));
	task_b->ctx.eip = (r_t) task_b_main;
	task_a->next = task_b;
	task_b->next = task_a;	/* 建立单向循环链表 */
	curr_task = task_a;
	switch_to_task(curr_task);
}

void schedule()
{
	switch_to_task(curr_task = curr_task->next);
}

