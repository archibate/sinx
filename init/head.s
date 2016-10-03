.section .tempload.text
.global	___begin
.global _start
.global ___bootloader_info
.extern _xmain

.equ	MBOOT_HEADER_MAGIC,	0x1BADB002
.equ	MBOOT_HEADER_FLAGS,	3	# 0011b: PAGE_ALIGN | MEM_INFO
.equ	MBOOT_CHECKSUM,		-(MBOOT_HEADER_MAGIC + MBOOT_HEADER_FLAGS)

__begin:
	.long	MBOOT_HEADER_MAGIC
	.long	MBOOT_HEADER_FLAGS
	.long	MBOOT_CHECKSUM

_start:
	ljmp	$0x0008, $_head

_head:
	cli
	cmpl	$0x10000000 + MBOOT_HEADER_MAGIC, %eax
	jne	not_mboot
	movl	$stack_top0, %esp
	movl	%esp, %ebp

load_gdt:
	movl	$gdt0, %esi
	movl	$gdt0_pa, %edi
	movl	$gdt0_len >> 2, %ecx
	rep
	movsl
	lgdt	gdtr0

	pushl	$0x0008
	call	to_retf
	movw	$0x0010, %ax
	movw	%ax, %ds
	movw	%ax, %es
	movw	%ax, %ss
	movw	%ax, %fs
	movw	%ax, %gs

load_idt:
	movl	$idt0, %esi
	movl	$idt0_pa, %edi
	movl	$idt0_len >> 2, %ecx
	rep
	movsl
	lidt	idtr0

/*
load_tss:
	movl	$tss0, %esi
	movl	$tss0_pa, %edi
	movl	$tss0_len >> 2, %ecx
	rep
	movsl
	movw	$0x0018, %ax
	ltr	%ax
*/

set_pg:
set_l2pt:
	movl	$l1pt0_pa + 3, %eax
	movl	$l2pt_pa, %edi
	stosl
	addl	$0x1000, %eax
	movl	$l2pt_pa + (0xC0000 >> 8), %edi
	stosl

set_l1pt:
	movl	$1024, %ecx	# 映射物理地址前 4MB
	movl	$3, %eax
	movl	$l1pt0_pa, %edi
.loop1:	stosl
	addl	$0x1000, %eax
	loop	.loop1
	movl	$1024, %ecx	# 我们把 0x00000000-0x00400000
	movl	$3, %eax	# 映射到 0xC0000000-0xC0400000
.loop2:	stosl
	addl	$0x1000, %eax
	loop	.loop2

set_cr3:
	movl	$cr3_value, %eax
	movl	%eax, %cr3
	movl	%cr0, %eax
	orl	$0x80000000, %eax
	movl	%eax, %cr0

	jmp	goto_xmain

to_retf:
	retf

	.align	8
gdt0:
	.quad	0x0000000000000000	# NULL
	.quad	0x00CF9A000000FFFF	# SYS_CODE
	.quad	0x00CF92000000FFFF	# SYS_DATA
	.quad	0x00CFFA000000FFFF	# USR_CODE
	.quad	0x00CFF2000000FFFF	# USR_DATA
	#.quad	0x00008B083800006C	# TSS at 0x83800
	.quad	0x0000000000000000	# reserved for TSS
	.quad	0x0000000000000000	# reserved for LDT

.equ	gdt0_len,	. - gdt0

gdtr0:
	.word	gdt0_len - 1
	.long	gdt0_pa

	.align	8
idt0:
	#.quad	0x0000000000000000

.equ	idt0_len,	. - idt0

idtr0:
.if	idt0_len
	.word	idt0_len - 1
.else
	.word	0
.endif
	.long	idt0_pa

	.align	8
tss0:
	.long	0		# BACK
	.long	stack_top	# ESP0
	.long	0x0010		# SS0
	.long	0		# ESP1
	.long	0		# SS1
	.long	0		# ESP2
	.long	0		# SS2
	.long	cr3_value	# CR3
	.long	0		# EIP
	.long	0		# EFL
	.long	0		# EAX
	.long	0		# ECX
	.long	0		# EDX
	.long	0		# EBX
	.long	0		# ESP
	.long	0		# EBP
	.long	0		# ESI
	.long	0		# EDI
	.long	0		# ES
	.long	0		# CS
	.long	0		# SS
	.long	0		# DS
	.long	0		# FS
	.long	0		# GS
	.long	0		# LDTR
	.word	0		# TRAP_SYM
	.word	tss0_sub_len	# IOMAP_OFF
				# IOMAP
	.byte	0xFF		# END_SYMBOL

.equ	tss0_sub_len,	. - tss0

	.align	4

.equ	tss0_len,	. - tss0

.equ	gdt0_pa,	0x80800
.equ	idt0_pa,	0x80000
.equ	cr3_value,	l2pt_pa
.equ	tss0_pa,	0x83800
.equ	l2pt_pa,	0
.equ	l1pt0_pa,	0x1000
/*.equ	l1pt0_max,	0x200
.equ	l1pt1_pa,	l1pt0_pa + l1pt0_max * 4
.equ	l1pt1_max,	0x10000*/

goto_xmain:
	movl	$stack_top, %esp
	movl	%esp, %ebp
	call	_xmain

not_mboot:
	cli
	hlt

	.org	0x400, 0
stack_top0:

.equ	stack_top,	stack_top0 + 0xC0000000


