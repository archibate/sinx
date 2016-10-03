#include <kernel.h>
#include <init/init.h>
#include <init/entry.h>
#include <sched/sched.h>
#include <mm/pmm.h>
#include <tty/tty.h>

void kernel_modinit(void)
{
	tty_modinit();
	pmm_modinit();
	//sched_modinit();
	__asm__ ("sti");
	__asm__ ("int $0x30");
	__asm__ ("cli;hlt");
}

