#----------------------------------------------------------------------------
#	$Id$
#
#	Build binaries
#	GNU version
#	Requires GNU make
#----------------------------------------------------------------------------

VBIN    = $(BIN:%=%$(VAR))

ifndef OBJEXT
OBJEXT=o
endif

VOBJS   = $(subst .cpp,.$(OBJEXT),$(foreach file,$(SRC),$(VDIR)$(notdir $(file))))

$(BIN): %: $(VDIR)%.o $(VOBJS)
	$(LINK.cc) -o $@ $^ $(LDFLAGS) $(VLDLIBS) $(POSTLINK)
