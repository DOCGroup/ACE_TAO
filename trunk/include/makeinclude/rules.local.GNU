#----------------------------------------------------------------------------
#	$Id$
#
#	Local targets 
#	GNU version
#	Requires GNU make
#----------------------------------------------------------------------------

ifndef  OBJDIRS
OBJDIRS = .obj .shobj # .obj_debug .obj_profile .obj_optimize
endif # OBJDIRS

all.local: build.objdirs build.local install.local

#----------------------------------------------------------------------------
#	C/C++ compilation targets
#----------------------------------------------------------------------------

build.local: $(BUILD)

# Set up the suffixes for C++ and IDL.
.SUFFIXES:
.SUFFIXES: .cpp .cc .C .idl $(SUFFIXES)

# and here's how to compile C++ files from the IDL file.
# only ONE of these rules will be run at make-time,

%S.cpp: %.idl
	$(IDL) $(IDLFLAGS) $<

%C.cpp: %.idl
	$(IDL) $(IDLFLAGS) $<

%.hh: %.idl
	$(IDL) $(IDLFLAGS) $<

# C++ related targets

$(VDIR)%.o:	%.c
	$(COMPILE.c) -o $@ $<
	${MVCMD}

$(VDIR)%.o:	%.cpp
	$(COMPILE.cc) -o $@ $<
	${MVCMD}

#$(VDIR)%.o:	%.C
#	$(COMPILE.cc) -o $@ $<

#$(VDIR)%.o:	%.cc
#	$(COMPILE.cc) -o $@ $<

# If SOLINK is defined, then the .so file is built from the .o file via
# separate rules in the same directory.  Otherwise, the .so and .o are
# built via the same rule.  SOLINK is required for the repository under
# gcc.
ifndef SOLINK
# I added the "Executable Shared Object (ESO)" define to separate between
# normal shared object files and executable shared object files (the kind
# that the service configurator needs to be able to function).
# 970104 Marius Kjeldahl <mariusk@sn.no, marius@funcom.com>
ifdef (ESOBUILD)
$(VSHDIR)%.$(SOEXT): %.cpp
	$(ESOBUILD)
else
$(VSHDIR)%.$(SOEXT): %.cpp
	$(SOBUILD)
endif
$(VSHDIR)%.o:	%.cpp
	$(COMPILE.cc) $(PIC) -o $@ $<
$(VSHDIR)%.o:	%.cc
	$(COMPILE.cc) $(PIC) -o $@ $<
$(VSHDIR)%.$(SOEXT):	$(VSHDIR)%.o
	$(SOLINK)
endif

#----------------------------------------------------------------------------
#	Library generation targets
#----------------------------------------------------------------------------

.PRECIOUS:	$(VLIB)

#----------------------------------------------------------------------------
#	Installation targets
#----------------------------------------------------------------------------

install.local: 	$(INSTALL)

deinstall.local:
	$(RM) $(INSTALL) $(LIB:%.a=$(INSLIB)/%*.a) $(SHLIB:%.$(SOEXT)=$(INSLIB)/%*.$(SOEXT))


#### To disable installs, just remove write permission from $ACE_ROOT/ace.
#### Or, add "INSTALL=" to your "make" invocation.

$(INSBIN)/%$(VAR) \
$(INSINC)/ace% \
$(INSLIB)/%$(VAR).a \
$(INSLIB)/%$(VAR).$(SOEXT) \
$(INSMAN)/man1/% \
$(INSMAN)/man2/% \
$(INSMAN)/man3/% \
$(INSMAN)/man4/% \
$(INSMAN)/man5/% \
$(INSMAN)/man6/% \
$(INSMAN)/man7/% \
$(INSMAN)/man8/% \
$(INSMAN)/manl/% \
$(INSMAN)/mann/% :
	@if test -w $(@D) ; then \
		if test -s $@ ; then \
			echo "$(@F) already installed" ; \
		else \
			echo "Installing $(@F) -> $(@D)" ; \
			ln -s $(shell pwd)/$(@F) $@ ; \
		fi \
	fi
ifdef MVSLIB
#special for MVS in order to use the .x files
	@if test -w $(@D) ; then \
		if test -s $(@:.$(SOEXT)=.x) ; then \
			echo "$(@F:.$(SOEXT)=.x) already installed" ; \
		else \
			echo "Installing $(@F:.$(SOEXT)=.x) -> $(@D)" ; \
			ln -s $(shell pwd)/$(@F:.$(SOEXT)=.x) $(@:.$(SOEXT)=.x) ; \
			fi \
	fi
endif

#----------------------------------------------------------------------------
#	Cleanup targets
#----------------------------------------------------------------------------

clean.local:
	-$(RM) -f *.o *~ *.bak *.rpo Makefile.old core
	-$(RM) -rf $(OBJDIRS) $(TEMPINCDIR) 
	-$(RM) -rf ptrepository Templates.DB gcctemp.c gcctemp so_locations

realclean.local: clean.local
	-$(RM) -f $(BIN:%=%) $(BIN:%=%_debug) $(BIN:%=%_profile) $(BIN:%=%_optimize) $(LIB:%=%) $(LIB:%=%_debug) $(LIB:%=%_profile) $(LIB:%=%_optimize) $(SHLIB:%=%) $(SHLIB:%=%_debug) $(SHLIB:%=%_profile) $(SHLIB:%=%_optimize)

#----------------------------------------------------------------------------
#	Dependency generation target
#----------------------------------------------------------------------------

ifndef MAKEFILE
MAKEFILE=Makefile
endif

depend.local: $(MAKEFILE)
	@$(RM) -f $(MAKEFILE).old
	@cp $(MAKEFILE) $(MAKEFILE).old
	$(ACE_ROOT)/bin/g++dep -f $(MAKEFILE) $(CPPFLAGS) -DMAKEDEPEND $(LSRC) $(SRC)
	@cat $(MAKEFILE) | \
		sed -e "s;$(TAO_ROOT);\$$(TAO_ROOT);g" \
		    -e "s;$(ACE_ROOT);\$$(ACE_ROOT);g" \
		    -e "s; /[-a-zA-Z0-9_./]*\.h;;g" \
		    -e "/:$$/d" \
		    -e "s;\([-a-zA-Z0-9._]*\)\.o:;.obj/\1.o $(VSHDIR)\1.$(SOEXT):;" \
	> $(MAKEFILE).new
	@mv $(MAKEFILE).new $(MAKEFILE) 
	@if	cmp -s $(MAKEFILE) $(MAKEFILE).old ;\
	then	echo "Makefile dependencies unchanged." ;\
	else \
		echo "Makefile dependencies updated." ;\
	fi ;\
	$(RM) -f $(MAKEFILE).old ;
#----------------------------------------------------------------------------
#	RCS info target
#----------------------------------------------------------------------------

rcs_info.local:
	@rcs info

#----------------------------------------------------------------------------
#	Variant targets and conditional macros
#----------------------------------------------------------------------------

build.objdirs: $(OBJDIRS)

$(OBJDIRS):
	test -d $@ || mkdir $@
