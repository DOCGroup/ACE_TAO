#----------------------------------------------------------------------------
#	$Id$
#
#	Build binaries
#	GNU version
#	Requires GNU make
#----------------------------------------------------------------------------

ifndef OBJEXT
OBJEXT=o
endif

VOBJS   = $(subst .cpp,.$(OBJEXT),$(foreach file,$(SRC),$(VDIR)$(notdir $(file))))

ifeq ($(CHORUSLINK),true)
$(BIN): %: $(VDIR)%.o $(VOBJS)
	$(LINK.cc) -o $@.xp.o $^ -u _main -u _premain  $(VLDLIBS) $(LDFLAGS)$(POSTLINK)
	$(MERGEDIR)/dtool/mkctors $@.xp.o > $@.ct.s
	$(CXX) -c $@.ct.s
	$(RM) $@.ct.s
	$(LD) $(LDOPTIONS) -e _start -o $@ $(MERGEDIR)/lib/crt/crth.u.o $(MERGEDIR)/lib/crt/crt0.o $(MERGEDIR)/lib/crt/crti.o $@.xp.o $@.ct.o $(MERGEDIR)/lib/crt/crtn.o -dn -T $(MERGEDIR)/src/act/slots/act_u.ld
	$(RM) $@.xp.o
	$(RM) $@.ct.o
else
$(BIN): %: $(VDIR)%.o $(VOBJS)
	$(LINK.cc) -o $@ $^ $(LDFLAGS) $(VLDLIBS) $(POSTLINK)
endif
