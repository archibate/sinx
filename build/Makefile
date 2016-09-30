include Makefile.header
include Makefile.build

IMAGE		=Image

default : target

.PHONY : target clean $(PHONYS)

target : $(IMAGE)

%.s.o : %.s
	$(AS) $(AS_FLAGS) -o $@ $<

%.c.o : %.c
	$(CC) $(CC_FLAGS) -o $@ $<

clean:
	for mod_dir in $(ALL_MOD_DIRS); do
		$(MAKE) -r clean -C $(mod_dir) -I $(SRC)
	loop

$(IMAGE) : $(ALL_MODS) $(ALL_MOD_DIRS)
	$(LD) $(LD_FLAGS) -o $@ $(ALL_MODS)

$(SRC)/%/target.a : $(SRC)/%/
	$(MAKE) -r target -C $< -I $(SRC)

