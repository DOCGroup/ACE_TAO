#----------------------------------------------------------------------------
#       $Id$
#
#       Build binaries
#       GNU version
#       Requires GNU make
#----------------------------------------------------------------------------

VOBJS = \
  $(subst .cpp,.$(OBJEXT),$(foreach file,$(SRC),$(VDIR)$(notdir $(file))))

ifeq ($(CHORUSLINK),true)
$(BIN): %: $(VDIR)%.$(OBJEXT) $(VOBJS)
	$(LINK.cc) $(CC_OUTPUT_FLAG) $@ $^   $(VLDLIBS) $(LDFLAGS) $(POSTLINK)
#	$(MERGEDIR)/dtool/mkctors $@.xp.o > $@.ct.s
#	$(CXX) -c $@.ct.s
#	$(RM) $@.ct.s
#	$(LD) $(LDOPTIONS) -e _start -o $@ $(MERGEDIR)/lib/crt/crth.u.o $(MERGEDIR)/lib/crt/crt0.o $(MERGEDIR)/lib/crt/crti.o $@.xp.o $@.ct.o $(MERGEDIR)/lib/crt/crtn.o -dn -T $(MERGEDIR)/src/act/slots/act_u.ld
#	$(RM) $@.xp.o
#	$(RM) $@.ct.o
else
ifeq (1,$(using_aix_vacpp))
$(BIN): %: %.icc $(SRC)
	vacbld_r $< -severitylevel=warning
else
$(BIN): %: $(VDIR)%.$(OBJEXT) $(VOBJS)
	$(LINK.cc) $(LDFLAGS) $(CC_OUTPUT_FLAG) $@ $^ $(VLDLIBS) $(POSTLINK)
endif
endif
