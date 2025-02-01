include $(PROJECT_MAKEFILE)

POTENTIAL_FILES = $(BIN_UNCHECKED) $(SHLIB_UNCHECKED) $(LIB_UNCHECKED)
ACTUAL_FILES := $(wildcard $(POTENTIAL_FILES))

.PHONY: checked-install
ifeq ($(ACTUAL_FILES),)
checked-install:
	@echo Skipping $(PROJECT_MAKEFILE:GNUmakefile.%=%), not built
else
.PHONY: checked-install-message
checked-install-message:
	@echo Installing $(ACTUAL_FILES)
checked-install: checked-install-message install
endif
