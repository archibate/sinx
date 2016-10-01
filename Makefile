include Makefile.header
include Makefile.build

default : target

.PHONY : target clean all run

all : target

target : $(FLOPPY_IMG)

$(FLOPPY_IMG) : $(IMAGE)
	mkdir -p /mnt/img
	sudo mount $@ /mnt/img
	sudo cp $< /mnt/img/boot/Image
	sudo umount /mnt/img

isodir/ : isodir/boot/Image

isodir/boot/Image : $(IMAGE)
	cp $< $@

$(IMAGE) : $(SRC)/build/ $(ALL_MOD_DIRS)
	$(MAKE) -r target -C build/ -I $(SRC)

clean :
	#$(MAKE) -r $@ -C boot/ -I $(SRC)
	$(MAKE) -r $@ -C build/ -I $(SRC)

#$(HDA_IMG) : $(SRC)/boot/ $(SRC)/build/ $(ALL_MOD_DIRS)
	#$(MAKE) -r target -C boot/ -I $(SRC)
	#$(MAKE) -r target -C build/ -I $(SRC)
	#$(EDIMG) $@ 10485760 boot/boot 0x1000 0 build/Image 0x4000 0x4000

run : target
	$(QEMU) -m 64 -parallel stdio -fda $(FLOPPY_IMG) -boot a
	#$(QEMU) -m 64 -parallel stdio -hda $(HDA_IMG) -boot c

