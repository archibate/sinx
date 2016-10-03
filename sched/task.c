#include <kernel.h>
#include <sched/task.h>
#include <sched/context.h>
#include <sched/sched.h>
#include <init/init.h>

struct task _curr_task;
struct task *curr_task = &_curr_task;

void switch_to_task(struct task *task)
{
	if (task)
		switch_to_context(&task->ctx);
	//else
	//	__asm__ ("cli;hlt");
}

void schedule_task()
{
	switch_to_task(curr_task = curr_task->next);
}

