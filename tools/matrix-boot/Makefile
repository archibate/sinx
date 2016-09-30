 include	../Makefile.header

LD_FLAGS	+=-Ttext 0

target : boot.bin

boot.bin : boot.s.o
	$(LD) $(LD_FLAGS) -o /tmp/tmp.out.$$ $<
	$(OBJCOPY) -O binary -R .note -R .commit -S /tmp/tmp.out.$$ $@

clean :
	$(RM) boot.s.o boot.bin

