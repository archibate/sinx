#include <kernel.h>
#include <init/init.h>
#include <init/entry.h>
#include <sched/sched.h>
#include <tty/tty.h>

void kernel_modinit(void)
{
	sched_modinit();
	tty_modinit();
}

