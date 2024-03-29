# -*- Makefile -*-

#----------------------------------------------------------------------------
#
#
#       Build binaries
#       GNU version
#       Requires GNU make
#----------------------------------------------------------------------------

ifdef BIN
  VBIN = $(ARCH_BIN:%=%$(VAR))
  # VBIN always depends on idl_stubs and libraries, if they exist.
  ifdef IDL_SRC
    $(VBIN): $(IDL_SRC)
  endif
  ifdef VLIBS
    $(VBIN): $(LIB_INSTALL) $(VLIBS)
  endif
else # !BIN
  ifndef BIN_UNCHECKED
    @echo Warning: rules.bin.GNU included, but no BIN targets detected.
  endif # !BIN_UNCHECKED
endif # BIN

VOBJS = $(addsuffix .$(OBJEXT),$(addprefix $(VDIR),$(basename $(SRC))))

ifneq ($(IDL_SRC),)
  $(VBIN): $(IDL_SRC)
endif

# Needed for depend.
PSRC += $(addsuffix .cpp, $(CLEANUP_BIN))

CLEANUP_INSTALL += $(CLEANUP_BIN:%=$(INSBIN)/%$(VAR)$(EXEEXT))

VXWORKSLINK ?=

ifeq ($(VXWORKSLINK),1)
$(ARCH_BIN): $(DEFAULTRULE)
	mv $(DEFAULTRULE) $@
else # !VXWORKSLINK
# Since we add in the vdir, don't use arch_bin.  use bin instead!
$(ARCH_BIN): %: $(VDIR)$(BIN).$(OBJEXT) $(VOBJS)
	$(LINK.cc) $(LDFLAGS) $(CC_OUTPUT_FLAG) $@ $(sort $(VDIR)$(BIN).$(OBJEXT) $(VOBJS)) $(VLDLIBS) $(POSTLINK)
endif
