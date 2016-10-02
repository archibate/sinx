include Makefile.header
include Makefile.build

default : target

.PHONY : target clean $(PHONYS)

target : $(IMAGE)

clean:
	$(foreach mod_dir, $(ALL_MOD_DIR), \
		$(MAKE) -r clean -C $(mod_dir) -I $(SRC);)

$(IMAGE) : $(ALL_MOD_STATIC) $(ALL_MOD_STATIC_DIR)
	@echo 'LD $@'
	$(Q)$(LD) $(LD_FLAGS) -o $@ $(ALL_MOD_STATIC)

$(SRC)/%/target.a : $(SRC)/%/
	@echo 'MAKE $<'
	$(Q)$(MAKE) -r target -C $< -I $(SRC)

