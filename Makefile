include Makefile.header
include Makefile.build

HDA_IMG		=hda.img

default : target

.PHONY : target clean all run

all : target

target : $(HDA_IMG)

clean :
	$(RM) $(HDA_IMG)
	$(MAKE) -r $@ -C boot/ -I $(SRC)
	$(MAKE) -r $@ -C build/ -I $(SRC)

#boot/boot : $(wildcard boot/)
#	$(MAKE) -r target -C $<
#
#kernel/kernel : $(wildcard kernel/)
#	$(MAKE) -r target -C $<
#
#hda.img : boot/boot kernel/kernel
#	$(MAKE) -r target -C $<
#	$(CAT) boot/boot kernel/kernel > $@

#$(HDA_IMG) : $(shell ls boot/ -R|cut -d: -f1) \
#	     $(shell ls kernel/ -R|cut -d: -f1)
$(HDA_IMG) : boot/ build/ $(ALL_MOD_DIRS)
	$(MAKE) -r target -C boot/ -I $(SRC)
	$(MAKE) -r target -C build/ -I $(SRC)
	$(EDIMG) $@ 10485760 boot/boot 0x1000 0 build/Image 0x4000 0x4000

run : target
	$(QEMU) -m 64 -parallel stdio -hda $(HDA_IMG) -boot c

