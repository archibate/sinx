.section .text
.global	switch_to_context
.global	my_thread_main

switch_to_context:
	movl	4(%esp), %esp
	popl	%gs
	popl	%fs
	popl	%es
	popl	%ds
	popal
	addl	$8, %esp
	iretl

my_thread_main:
	incw	0xC00B8000
	loop	my_thread_main
	cli
	hlt

