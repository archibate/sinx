#ifndef	_SINX_SCHED_TASK_H
#define	_SINX_SCHED_TASK_H

#include <sched/context.h>

typedef u16 pid_t;

struct task {
	struct context ctx;
	struct task *next;
	pid_t pid;
	u16 flags;
};

extern struct task *curr_task;

void switch_to_task(struct task *task);

#endif

