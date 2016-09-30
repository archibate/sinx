
.text
.code16
.global	_start

_start:
	jmp	entry

entry:
	movw	$0x07C0, %ax
	movw	%ax, %ds
	movw	$0x9000, %ax
	movw	%ax, %es
	movw	$256, %cx
	subw	%si, %si
	movw	%si, %di
	rep
	movsw
	ljmp	$0x9000, $go	# the low address (0x00000~0x80000) will be
				# used for kernel. so let us copy our boot.bin
				# to high address, to protect boot.bin from
				# covered by loaded kernel. same as linux-0.11

go:
	movw	%ax, %ds
	movw	%ax, %ss
	movw	$0x7DFC, %sp
	call	clear_screen
	movw	$msg_boot, %si
	call	print

read_setup:			# only the boot sector is not enough
	call	read_sects	# we need setup sects for more functions
	#cmpw	$0, setup
	#jnz	setup
	jmp	setup
	pushw	%cs
	popw	%ds
	movw	$msg_error, %si
	call	print
	jmp	die

read_sects:
	movw	$0x7E00, %bx	# the sector buffer
	xorw	%ax, %ax
	movw	%ax, %di
	movw	%ax, %es	# In some (much) version of BIOS, they
	movw	%ax, %ds	# read sector to 0:%bx instead of %es:%bx
	cld			# So first, we read the data in sector
				# to a low address buffer (0x7E00) templately,
				# and then move it to the correct address
				# which caller given (%es:%bx)
read_next:
#delay_down:
#	movw	$0x07FF, %ax
#delay_down0:
#	movw	$0x07FF, %cx
#delay_down1:
#	loop	delay_down1
#	dec	%ax
#	jnz	delay_down0
#delay_down_end:
	movw	%cs:sect_nr, %ax	# %ax is LBA
	incw	%cs:sect_nr
	call	read_sect
	movw	$256, %cx
	pushw	%es
	movw	%cs:dest_sel, %ax
	movw	%ax, %es
	movw	%bx, %si		# %si=%bx=0x7E00
	rep			# copy from buffer to correct address
	movsw			# and %di will be increased to next address
	cmpw	$0xFE00, %di	# but, if %di is bigger than segment size,
	jb	di_not_too_much	# increase %di will cause %di back to 0 again
	addw	$0xFE00, %cs:dest_sel	# to avoid this, increase dest_sel,
	subw	$0xFE00, %di		# and decrease %di, make dest_sel:%di
					# back to old address, but %di is
					# avaliable for use again
	jmp	halt
di_not_too_much:
	popw	%es
	movw	$0x0E2E, %ax
	int	$0x10			# print an progress dot every sector
	movw	%cs:final_sect_nr, %ax	# read from sect_nr to final_sect_nr,
	cmpw	%ax, %cs:sect_nr	# and NOT contains final_sect_nr
	jb	read_next
	movw	$0x0E0D, %ax
	int	$0x10
	movb	$0x0A, %al
	int	$0x10
	ret

read_sect:			# LBA is given in %ax
	movb	$36, %dl
	divb	%dl
	movb	%al, %ch
	shrw	$8, %ax		# = movb %al, %ah + movb $0, %al
	movb	$18, %dl
	divb	%dl
	movb	%al, %dh
	movb	%ah, %cl
	incb	%cl		# compute CHS from the given LBA
	movb	$0x00, %dl	# our boot device is floppya
	movw	$0x0201, %ax
read_again:
	#cmpw	$0x1000, %cs:dest_sel
	#je	err
	#call	print_hex
	int	$0x13
	jc	read_again	# if read boot device failed, try again
	ret			# but while it do not work any way, still all
				# right ... it is the same as jmp die

/*
print_hex:
	movb	%cl, %al
	pushw	$print_hex_high_of_cl
	andb	$0x0F, %al
print_a_hex_char:
	cmpb	$9, %al
	jb	hex_number_below_9
	addb	$'A' - 10 - '0', %al
hex_number_below_9:
	addb	$'0', %al
	movb	$0x0E, %ah
	int	$0x10
	ret
print_hex_high_of_cl:
	jmp	die
	testw	$0xFFF0, %cx
	jz	print_hex_done
	movb	%cl, %al
	shrb	$4, %al
	call	print_a_hex_char
	shrw	$8, %cx
	#testw	%cx, %cx
	jnz	print_hex
print_hex_done:
	ret
*/

print:
	movb	$0x0E, %ah
next_char:
	lodsb
	testb	%al, %al
	jz	print_done
	int	$0x10
	jmp	next_char
print_done:
	ret

clear_screen:
	movw	$0x0600, %ax
	xorw	%cx, %cx
	movw	$0x184F, %dx
	movb	$0x07, %bh
	int	$0x10
	movb	$0x02, %ah
	movb	$0x00, %bh
	xorw	%dx, %dx
	int	$0x10
	ret

die:	jmp	die

boot_datas:
msg_boot:	.ascii	"Booting\0"
msg_error:	.ascii	"ERROR in boot\0"

	.align	2
sect_nr:	# These three, they are args for read_sects
	.word	1
final_sect_nr:
	.word	4
dest_sel:
	.word	0x9020		# behind boot sector

.org	0x1FE, 0x00
.byte	0x55, 0xAA

setup:
	#int	$0x19
	movw	%cs, %ax
	movw	%ax, %ds
	movw	%ax, %es
	#movl	$0, %eax
	#cpuid
	#movl	%ebx, msg_cpumaker_sz
	#movl	%edx, msg_cpumaker_sz + 4
	#movl	%ecx, msg_cpumaker_sz + 8
	#movl	%eax, cpuid_a_0
	#movl	$1, %eax
	#cpuid
	#movl	%eax, cpuid_a_1
	#movw	$msg_cpumaker, %si
	#call	print
	#jmp	halt
#	movw	$msg_setup, %si
#	call	print
	movw	$msg_read, %si
	call	print
read_kern:
	movw	$kern_tmp_addr >> 4, dest_sel
	movw	$64, final_sect_nr
	pushw	%ds
	pushw	%es
	call	read_sects
	popw	%es
	popw	%ds
	#jmp	die
setup_envirs:
	call	setup_vga
	cli

move_kern:
	pushw	$kern_tmp_addr >> 4
	popw	%ds
	pushw	$kern_addr >> 4
	popw	%es
	cld
	movw	$30, %dx
move_kern_loop:
	subw	%si, %si
	movw	%si, %di
	movw	$0x8000, %cx
	rep
	movsw
	movw	%ds, %ax
	addw	$0x1000, %ax
	movw	%ax, %ds
	movw	%es, %ax
	addw	$0x1000, %ax
	movw	%ax, %es
	decw	%dx
	jnz	move_kern_loop

jump_into_kern:
	#movw	$msg_jump, %si
	#call	print
	#jmp	halt
	#ljmp	$kern_addr >> 4, $0
	ljmp	$0, $kern_addr

setup_vga:
setup_vga_0118:
	xorw	%ax, %ax
	movw	%ax, %es
	movw	%ax, %ds
	movw	$0x8000, %di
	movw	$0x4F00, %ax
	int	$0x10
	cmpw	$0x004F, %ax
	jne	err_vga_0118
	cmpw	$0x0200, 0x04(%di)
	jb	err_vga_0118
	movw	$0x0118, %cx
	movw	$0x4F01, %ax
	int	$0x10
	cmpw	$0x004F, %ax
	jne	err_vga_0118
	testb	$0x80, (%di)
	jz	err_vga_0118
	movw	$0x0118, %cs:vmode
	movw	0x12(%di), %ax
	movw	%ax, %cs:scrn_x
	movw	0x14(%di), %ax
	movw	%ax, %cs:scrn_y
	movb	0x19(%di), %al
	movb	%al, %cs:bits_per_pixel
	movb	0x1B(%di), %al
	movb	%al, %cs:memory_model
	movl	0x28(%di), %eax
	movl	%eax, %cs:vram
	movw	$0x4118, %bx
	movw	$0x4F02, %ax
	int	$0x10
	#movw	$0x4F02, %ax
	#movw	$0x4118, %bx
	#int	$0x10
	#cmpb	$0x4F, %ah
	#jne	err_vga_0118
	#movw	$0x0118, vmode
	#movw	$1024, scrn_x
	#movw	$768, scrn_y
	#movl	$0x000A0000, vram
	ret

poff:	movw	$0x5301, %ax
	xorw	%bx, %bx
	int	$0x15
	movw	$0x530E, %ax
	movw	$0x0102, %cx
	int	$0x15
	movw	$0x5307, %ax
	movb	$0x01, %bl
	movw	$0x0003, %cx
	int	$0x15
err_vga_0118:
err:	pushw	%cs
	popw	%ds
	movw	$msg_err, %si
	call	print
	movb	$0x00, %ah
	int	$0x16
reset:	int	$0x19
coreb:	ljmp	$0xF000, $0xFFF0
halt:	pushw	%cs
	popw	%ds
	movw	$msg_halt, %si
	call	print
	cli
fin:	hlt
	jmp	fin
setup_datas:
#msg_setup:	.ascii	"Succeed in enter setup stage"
msg_read:	.ascii	"Loading kernel\0"
msg_ucomp:	.ascii	"Uncompressing kernel\0"
msg_jump:	.ascii	"Now jump into the kernel"
msg_crlf:	.ascii	"\r\n\0"
msg_halt:	.ascii	"System halted\0"
msg_err:	.ascii	"An error occurred during the setup stage\r\n"
		.ascii	"Press any key to reset\0"
msg_cpumaker:	.ascii	"CPU : "
msg_cpumaker_sz:.ascii	"                \r\n\0"

		.align	4
cpuid_a_0:	.long	0
cpuid_a_1:	.long	0
vga_info:
vram:		.long	0
scrn_x:		.word	0
scrn_y:		.word	0
vmode:		.word	0
memory_model:	.byte	0
bits_per_pixel:	.byte	0

.equ	kern_tmp_addr,	0x10000
.equ	kern_addr,	0x8000

.org	0x7F4, 0x00

special_sym:
	.ascii	"MATRIXMBR"
	.byte	0xC3
	.byte	0x28, 0xA6

