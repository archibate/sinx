include Makefile.header
include Makefile.build

default : target

.PHONY : target clean $(PHONYS)

target : $(IMAGE)

clean:
	#$(SH) << __eof__
	#for mod_dir in #$(ALL_MOD_DIRS)
	#do $(MAKE) -r clean -C $(mod_dir) -I $(SRC)
	#done
	$(foreach mod_dir, $(ALL_MOD_DIRS), \
		$(MAKE) -r clean -C $(mod_dir) -I $(SRC);)

$(IMAGE) : $(ALL_MODS) $(ALL_MOD_DIRS)
	@echo 'LD $@'
	$(Q)$(LD) $(LD_FLAGS) -o $@ $(ALL_MODS)

$(SRC)/%/target.a : $(SRC)/%/
	@echo 'MAKE $<'
	$(Q)$(MAKE) -r target -C $< -I $(SRC)

