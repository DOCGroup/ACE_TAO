#----------------------------------------------------------------------------
#	$Id$
#
#	Build binaries
#	GNU version
#	Requires GNU make
#----------------------------------------------------------------------------

VBIN    = $(BIN:%=%$(VAR))

$(BIN): %: $(VDIR)%.o $(VOBJS)
	$(LINK.cc) -o $@ $^ $(LDFLAGS) $(VLDLIBS) $(POSTLINK)
