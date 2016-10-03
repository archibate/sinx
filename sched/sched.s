.section .text
.global	switch_to_context
.global	task_a_main
.global	task_b_main

switch_to_context:	# void switch_to_context(struct context *ctx);
	movl	%esp, old_esp
	movl	4(%esp), %esp	# 4(%esp) 是传入的参数 ctx
	popl	%gs
	popl	%fs
	popl	%es
	popl	%ds
	popal
	#addl	$8, %esp
	movl	%esp, %eax
	movl	%cs:old_esp, %esp
	#pushl	24(%eax)	# SS
	#pushl	20(%eax)	# ESP
	pushl	16(%eax)	# EFL
	pushl	12(%eax)	# CS
	pushl	8(%eax)		# EIP
	movl	-4(%eax), %eax	# 恢复原来的 %eax
	iretl
	#retf

	.align	4
old_esp:
	.long	0

task_a_main:
	incw	0xC00B8000
	loop	task_a_main
	call	schedule_task
	movl	$1024, %ecx
	jmp	task_a_main
	cli
	hlt

task_b_main:
	incw	0xC00B8002
	loop	task_b_main
	call	schedule_task
	cli
	hlt
	movl	$1024, %ecx
	jmp	task_b_main
	cli
	hlt


