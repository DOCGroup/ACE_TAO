# -*- Makefile -*-
#----------------------------------------------------------------------------
#       $Id$
#
#       Local targets
#       GNU version
#       Requires GNU make
#----------------------------------------------------------------------------

#### TEMPLATE_REPOSITORY can be set in individual platform files
#### to create template repository directories.  See
#### platform_sunos5_sunc++.GNU for an example.
OBJDIRS += $(TEMPLATE_REPOSITORY)

all.local: build.objdirs build.local install.local

#----------------------------------------------------------------------------
#       C/C++ compilation targets
#----------------------------------------------------------------------------

build.local: $(BUILD)

# Set up the suffixes for C++ and IDL.
.SUFFIXES:
.SUFFIXES: .cpp .cc .C .idl $(SUFFIXES)

ifndef TAO_ROOT
# and here's how to compile C++ files from the IDL file.
# only ONE of these rules will be run at make-time,

# The rules are only used if TAO_ROOT is not defined to avoid
# conflicts with a similar rule in TAO.

%S.cpp: %.idl
	$(IDL) --version $(IDLFLAGS) $<

%C.cpp: %.idl
	$(IDL) --version $(IDLFLAGS) $<

%.hh: %.idl
	$(IDL) --version $(IDLFLAGS) $<
endif # TAO_ROOT

# C++ related targets

ifeq ($(OS),"Windows_NT")
$(VDIR)%.obj: %.c
	$(COMPILE.c) $(CC_OUTPUT_FLAG) $@ $<
	${MVCMD}

$(VDIR)%.obj: %.cpp
	$(COMPILE.cc) $(CC_OUTPUT_FLAG) $@ $<
	${MVCMD}
else
$(VDIR)%.o: %.c
	$(COMPILE.c) $(CC_OUTPUT_FLAG) $@ $<
	${MVCMD}

$(VDIR)%.o: %.cpp
	$(COMPILE.cc) $(CC_OUTPUT_FLAG) $@ $<
	${MVCMD}
endif

$(VDIR)%.o: %.C
	$(COMPILE.cc) $(CC_OUTPUT_FLAG) $@ $<
	${MVCMD}

$(VDIR)%.o: %.cc
	$(COMPILE.cc) $(CC_OUTPUT_FLAG) $@ $<
	${MVCMD}

# If SOLINK is defined, then the .$(SOEXT) file is built from the .$(OBJEXT) file via
# separate rules in the same directory.  Otherwise, the .$(SOEXT) and .$(OBJEXT) are
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
$(VSHDIR)%.$(OBJEXT): %.cpp
	$(COMPILE.cc) $(PIC) $(CC_OUTPUT_FLAG) $@ $<
$(VSHDIR)%.$(OBJEXT): %.cc
	$(COMPILE.cc) $(PIC) $(CC_OUTPUT_FLAG) $@ $<
$(VSHDIR)%.$(SOEXT): $(VSHDIR)%.$(OBJEXT)
	$(SOLINK)
endif

#----------------------------------------------------------------------------
#       Library generation targets
#----------------------------------------------------------------------------

.PRECIOUS: $(VLIB)

#### show_statics shows static objects in locally-created object files.
#### It assumes that the object files were built using g++.
#### TOOLENV selects the proper nm in VxWorks host environments.
#### TOOLDIR allows specification of the full path to nm via definition
#### in the platform_macros.GNU file.
show_statics:
	-@$(TOOLDIR)nm$(TOOLENV) -Co $(VSHDIR)*o | \
	  egrep ' global destructors '; true

#### show_uninit shows uninitialized data in locally-created object files.
#### TOOLENV selects the proper nm in VxWorks host environments.
show_uninit:
	-@$(TOOLDIR)nm$(TOOLENV) -Co $(VSHDIR)*o | egrep ' b '

#----------------------------------------------------------------------------
#       Installation targets
#----------------------------------------------------------------------------

install.local: $(INSTALL)

deinstall.local:
	$(RM) $(INSTALL) $(LIB:%.$(LIBEXT)=$(INSLIB)/%*.$(LIBEXT)) \
$(SHLIB:%.$(SOEXT)=$(INSLIB)/%*.$(SOEXT))


#### To disable installs, just add "INSTALL=" to your "make" invocation.

ifeq (,$(LN_S))
  ifndef COMSPEC
    ifdef ComSpec
      #### ACE+TAO use COMSPEC, but ComSpec is defined.
      COMSPEC = $(ComSpec)
    endif # ComSpec
  endif # ! COMPSPEC

  ifdef COMSPEC
    #### Assume we're on a WIN32 host.
    LN_S = cp -p
  else  # ! WIN32
    LN_S = ln -s
  endif # ! WIN32
endif # ! LN_S

$(INSBIN)/%$(VAR) \
$(INSINC)/ace% \
$(INSLIB)/% \
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
	@if test $(shell pwd) != $(@D) $(CC_OUTPUT_FLAG) -n "$(ACE_LDSO_Version_Number)" ; then \
		if test -s $@ ; then \
			echo "$(@F) already installed" ; \
		else \
			echo "Installing $(@F) -> $(@D)" ; \
			$(RM) $(@); $(LN_S) $(shell pwd)/$(@F:%.$(SOEXT)$(ACE_LDSO_Version_Number)=%.$(SOEXT)) $(@) ; \
		fi ; \
	fi
ifdef MVSLIB
#special for MVS in order to use the .x files
	@if test -w $(@D) ; then \
		if test -s $(@:.$(SOEXT)=.x) ; then \
			echo "$(@F:.$(SOEXT)=.x) already installed" ; \
		else \
			echo "Installing $(@F:.$(SOEXT)=.x) -> $(@D)" ; \
			$(RM) $(@); $(LN_S) $(shell pwd)/$(@F:.$(SOEXT)=.x) $(@:.$(SOEXT)=.x) ; \
		fi \
	fi
endif

#----------------------------------------------------------------------------
#       Cleanup targets
#----------------------------------------------------------------------------

clean.local:
	-$(RM) *.$(OBJEXT) *~ *.bak *.rpo *.sym lib*.*_pure_* Makefile.old core
	-$(RM) -r $(OBJDIRS) $(TEMPINCDIR) cxx_repository ptrepository \
Templates.DB gcctemp.c gcctemp so_locations *.ics

realclean.local: clean.local
	-$(RM) $(BIN) $(BIN:%=%_debug) $(BIN:%=%_profile) $(BIN:%=%_optimize) $(LIB:%=%) $(LIB:%=%_debug) $(LIB:%=%_profile) $(LIB:%=%_optimize) $(SHLIB:%=%) $(SHLIB:%=%_debug) $(SHLIB:%=%_profile) $(SHLIB:%=%_optimize) $(SHLIBA:%=%) $(BIN2) $(BIN2:%=%_debug) $(BIN2:%=%_profile) $(BIN2:%=%_optimize) $(LIB2:%=%) $(LIB2:%=%_debug) $(LIB2:%=%_profile) $(LIB2:%=%_optimize) $(SHLIB2:%=%) $(SHLIB2:%=%_debug) $(SHLIB2:%=%_profile) $(SHLIB2:%=%_optimize) $(SHLIBA2:%=%)

#----------------------------------------------------------------------------
#       Dependency generation target
#----------------------------------------------------------------------------

ifeq ($(MAKEFILE),)
  MAKEFILE = Makefile
endif # ! MAKEFILE

ifndef TAO_ROOT
  TAO_ROOT = $(ACE_ROOT)/TAO
endif # TAO_ROOT

# If no libraries are build in the current directory (actually, if
# rules.lib.GNU isn't included), then SOEXT might not be set.
ifndef SOEXT
  SOEXT=so
endif # SOEXT

ORIGdepend.local: $(MAKEFILE)
	@$(RM) $(MAKEFILE).old
	@cp $(MAKEFILE) $(MAKEFILE).old
	$(ACE_ROOT)/bin/g++dep -f $(MAKEFILE) $(CPPFLAGS) -DMAKEDEPEND $(LSRC) $(SRC) $(PSRC)
	@cat $(MAKEFILE) | \
	sed -e "s;$(TAO_ROOT);\$$(TAO_ROOT);g" \
	    -e "s;$(ACE_ROOT);\$$(ACE_ROOT);g" \
	    $(ACE_DEPEND_SED_CMD) \
	    -e '/$$(ACE_ROOT)\/ace\/config[^\.]*\.h/d' \
	    -e "s; /[-a-zA-Z0-9_./]*\.h;;g" \
	    -e "s;\([-a-zA-Z0-9._]*\)\.$(OBJEXT):;.obj/\1.$(OBJEXT) .obj/\1.$(SOEXT) $(VSHDIR)\1.$(OBJEXT) $(VSHDIR)\1.$(SOEXT):;" \
	> $(MAKEFILE).new
	@cat $(MAKEFILE).new >$(MAKEFILE)
	@$(RM) $(MAKEFILE).new
	@if	cmp -s $(MAKEFILE) $(MAKEFILE).old ;\
	then	echo "Makefile dependencies unchanged." ;\
	else \
		echo "Makefile dependencies updated." ;\
	fi ;\
	$(RM) $(MAKEFILE).old ;

depend.local: $(MAKEFILE)
	@$(RM) $(MAKEFILE).old
	@cp $(MAKEFILE) $(MAKEFILE).old
	SOEXT=$(SOEXT) VSHDIR=$(VSHDIR) ACE_DEPEND_SED_CMD="$(ACE_DEPEND_SED_CMD)" $(ACE_ROOT)/bin/g++dep -A -f $(MAKEFILE) $(CPPFLAGS) -DMAKEDEPEND $(LSRC) $(SRC) $(PSRC)
	@if cmp -s $(MAKEFILE) $(MAKEFILE).old ;\
	then echo "Makefile dependencies unchanged." ;\
	else \
		echo "Makefile dependencies updated." ;\
	fi ;\
	$(RM) $(MAKEFILE).old ;

#----------------------------------------------------------------------------
#       RCS info target
#----------------------------------------------------------------------------

rcs_info.local:
	@rcs info

#----------------------------------------------------------------------------
#       Variant targets and conditional macros
#----------------------------------------------------------------------------

build.objdirs: $(OBJDIRS)

ifeq ($(OS),"Windows_NT")
$(OBJDIRS):
	if not exist $@ mkdir $@
else
$(OBJDIRS):
	test -d $@ || mkdir $@
endif
