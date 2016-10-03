#include <kernel.h>
#include <init/init.h>
#include <sched/sched.h>
#include <sched/context.h>
#include <tty/tty.h>
#include <mm/pmm.h>

void isr_handler_0x30(void)
{
	tty_putstr(curr_tty, "ISR handler 0x30 entered");
}
