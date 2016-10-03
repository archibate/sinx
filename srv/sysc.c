#include <kernel.h>
#include <init/init.h>
#include <sched/sched.h>
#include <sched/context.h>
#include <tty/tty.h>
#include <mm/vmm.h>

void isr_handler_0x30(void)
{
	tty_cputstr(curr_tty, "ISR handler 0x30 entered\n");
}

