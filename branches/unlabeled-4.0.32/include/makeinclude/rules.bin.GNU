#----------------------------------------------------------------------------
#	@(#)rules.bin.GNU	1.1	10/18/96
#
#	Build binaries
#	GNU version
#	Requires GNU make
#----------------------------------------------------------------------------

VBIN    = $(BIN:%=%$(VAR))

$(BIN): %: $(VDIR)%.o $(VSHOBJS)
	$(LINK.cc) -o $@ $< $(LDFLAGS) $(VLDLIBS)
