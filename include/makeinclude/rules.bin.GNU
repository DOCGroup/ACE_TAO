#----------------------------------------------------------------------------
#	$Id$
#
#	Build binaries
#	GNU version
#	Requires GNU make
#----------------------------------------------------------------------------

VBIN    = $(BIN:%=%$(VAR))

$(BIN): %: $(VDIR)%.o $(VSHOBJS)
	$(LINK.cc) -o $@ $< $(LDFLAGS) $(VLDLIBS) $(POSTLINK)
