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

all.local: $(split_target) build.objdirs build.local install.local

#----------------------------------------------------------------------------
#       C/C++ compilation targets
#----------------------------------------------------------------------------

build.local: $(BUILD)

# Set up the suffixes for C++ and IDL.
.SUFFIXES:
.SUFFIXES: .cpp .cc .C .cxx .idl $(SUFFIXES)

# C++ related targets

ifeq ($(OS),"Windows_NT")
$(VDIR)%.obj: %.c
	$(COMPILE.c) $(CC_OUTPUT_FLAG) $@ $<
	${MVCMD}

$(VDIR)%.obj: %.cpp
	$(COMPILE.cc) $(CC_OUTPUT_FLAG) $@ $<
	${MVCMD}
else
$(VDIR)%.o: %.c $(VDIR).creation_stamp
	$(COMPILE.c) $(CC_OUTPUT_FLAG) $@ $<
	${MVCMD}

$(VDIR)%.o: %.cpp $(VDIR).creation_stamp
	$(COMPILE.cc) $(CC_OUTPUT_FLAG) $@ $<
	${MVCMD}
endif

$(VDIR)%.o: %.C $(VDIR).creation_stamp
	$(COMPILE.cc) $(CC_OUTPUT_FLAG) $@ $<
	${MVCMD}

$(VDIR)%.o: %.cc $(VDIR).creation_stamp
	$(COMPILE.cc) $(CC_OUTPUT_FLAG) $@ $<
	${MVCMD}

$(VDIR)%.o: %.cxx $(VDIR).creation_stamp
	$(COMPILE.cc) $(CC_OUTPUT_FLAG) $@ $<
	${MVCMD}

# If SOLINK is defined, then the .$(SOEXT) file is built from the
# .$(OBJEXT) file via separate rules in the same directory.
# Otherwise, the .$(SOEXT) and .$(OBJEXT) are built via the same rule.
# SOLINK is required for the repository under gcc.
ifndef SOLINK
# I added the "Executable Shared Object (ESO)" define to separate between
# normal shared object files and executable shared object files (the kind
# that the service configurator needs to be able to function).
# 970104 Marius Kjeldahl <mariusk@sn.no, marius@funcom.com>
ifdef (ESOBUILD)
$(VSHDIR)%.$(SOEXT): %.cpp $(VSHDIR).creation_stamp
	$(ESOBUILD)
else
$(VSHDIR)%.$(SOEXT): %.cpp $(VSHDIR).creation_stamp
	$(SOBUILD)
endif

ifeq ($(split), 1)
$(VSHDIR)%.$(OBJEXT): %.cpp $(VSHDIR).creation_stamp
	$(COMPILE.cc) $(PIC) SPLIT/`echo $< | sed "s/.cpp/_S\*/"`
	mv *.$(OBJEXT) $(VSHDIR)
else

$(VSHDIR)%.$(OBJEXT): %.cpp $(VSHDIR).creation_stamp
	$(COMPILE.cc) $(PIC) $(CC_OUTPUT_FLAG) $@ $<
endif

$(VSHDIR)%.$(OBJEXT): %.cc $(VSHDIR).creation_stamp
	$(COMPILE.cc) $(PIC) $(CC_OUTPUT_FLAG) $@ $<

$(VSHDIR)%.$(OBJEXT): %.C $(VSHDIR).creation_stamp
	$(COMPILE.cc) $(PIC) $(CC_OUTPUT_FLAG) $@ $<

$(VSHDIR)%.$(OBJEXT): %.cxx $(VSHDIR).creation_stamp
	$(COMPILE.cc) $(PIC) $(CC_OUTPUT_FLAG) $@ $<

$(VSHDIR)%.$(OBJEXT): %.c $(VSHDIR).creation_stamp
	$(COMPILE.c) $(PIC) $(CC_OUTPUT_FLAG) $@ $<

$(VSHDIR)%.$(SOEXT): $(VSHDIR)%.$(OBJEXT)
	$(SOLINK)
endif

ifeq ($(OS),"Windows_NT")
$(VDIR).creation_stamp:
	@if not exist $(VDIR) mkdir $(VDIR)
	@echo > $@
else
$(VDIR).creation_stamp:
	@-test -d $(VDIR) || mkdir $(VDIR) $(ACE_NUL_STDERR)
	@touch $@
endif

ifneq ($(VDIR),$(VSHDIR))
  ifeq ($(OS),"Windows_NT")
$(VSHDIR).creation_stamp:
	@if not exist $(VSHDIR) mkdir $(VSHDIR)
	@echo > $@
  else
$(VSHDIR).creation_stamp:
	@-test -d $(VSHDIR) || mkdir $(VSHDIR) $(ACE_NUL_STDERR)
	@touch $@
  endif
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
	$(RM) $(INSTALL) $(LIB:%.$(LIBEXT)=$(INSLIB)/%.$(LIBEXT)) \
$(SHLIB:%.$(SOEXT)=$(INSLIB)/%.$(SOEXT))


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

$(INSBIN)/%$(VAR)$(EXEEXT) \
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
	@if test $(shell pwd) != $(@D) -o -n "$(SOVERSION)" ; then \
		if test -s $@ ; then \
			echo "$(@F) already installed" ; \
		else \
			echo "Installing $(@F:%.$(SOEXT)$(SOVERSION)=%.$(SOEXT)) -> $(@D)" ; \
			$(RM) $(@:%.$(SOEXT)$(SOVERSION)=%.$(SOEXT)); \
			$(LN_S) $(shell pwd)/$(@F) $(@:%.$(SOEXT)$(SOVERSION)=%.$(SOEXT)); \
			echo "Installing $(@F) -> $(@D)" ; \
			$(RM) $@; \
			$(LN_S) $(shell pwd)/$(@F) $@; \
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
	-$(RM) -r $(OBJDIRS) $(TEMPINCDIR) cxx_repository ptrepository ti_files
	-$(RM) -r Templates.DB gcctemp.c gcctemp so_locations *.ics
	-$(RM) -r templateregistry SunWS_cache ir.out core.*

clean_idl_stubs.local:
ifneq ($(IDL_FILES),)
	-$(RM) $(addsuffix $(IDL_CLIENT_HDR_EXT), $(IDL_FILES)) $(addsuffix $(IDL_CLIENT_INL_EXT), $(IDL_FILES)) $(addsuffix $(IDL_CLIENT_SRC_EXT),$(IDL_FILES))
	-$(RM) $(addsuffix $(IDL_SERVER_HDR_EXT), $(IDL_FILES)) $(addsuffix $(IDL_SERVER_INL_EXT), $(IDL_FILES)) $(addsuffix $(IDL_SERVER_SRC_EXT),$(IDL_FILES))
	-$(RM) $(addsuffix $(IDL_SERVER_THDR_EXT), $(IDL_FILES)) $(addsuffix $(IDL_SERVER_TINL_EXT), $(IDL_FILES)) $(addsuffix $(IDL_SERVER_TSRC_EXT),$(IDL_FILES))
endif

realclean.local: clean.local deinstall.local clean_idl_stubs.local
	-$(RM) $(BIN:%=%$(EXEEXT)) $(BIN:%=%_debug$(EXEEXT)) $(BIN:%=%_profile$(EXEEXT)) $(BIN:%=%_optimize$(EXEEXT)) $(LIB:%=%) $(LIB:%=%_debug) $(LIB:%=%_profile) $(LIB:%=%_optimize) $(SHLIB:%=%) $(SHLIB:%=%.*) $(SHLIB:%.so=%.a) $(SHLIB:%=%_debug) $(SHLIB:%=%_profile) $(SHLIB:%=%_optimize) $(SHLIBA:%=%) $(BIN2:%=%$(EXEEXT)) $(BIN2:%=%_debug$(EXEEXT)) $(BIN2:%=%_profile$(EXEEXT)) $(BIN2:%=%_optimize$(EXEEXT)) $(LIB2:%=%) $(LIB2:%=%_debug) $(LIB2:%=%_profile) $(LIB2:%=%_optimize) $(SHLIB2:%=%) $(SHLIB2:%=%_debug) $(SHLIB2:%=%_profile) $(SHLIB2:%=%_optimize) $(SHLIBA2:%=%)

#----------------------------------------------------------------------------
#       Dependency generation target
#----------------------------------------------------------------------------

ifeq ($(MAKEFILE),)
  MAKEFILE = Makefile
endif # ! MAKEFILE

ifeq ($(DEPENDENCY_FILE),)
  DEPENDENCY_FILE = $(MAKEFILE)
endif

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

ifneq ($(MAKEFILE),$(DEPENDENCY_FILE))
$(DEPENDENCY_FILE):
	touch $(DEPENDENCY_FILE)
depend.local: $(DEPENDENCY_FILE)
include $(DEPENDENCY_FILE)
endif

depend.local: $(MAKEFILE)
	@$(RM) $(DEPENDENCY_FILE).old
	@cp $(DEPENDENCY_FILE) $(DEPENDENCY_FILE).old
	SOEXT=$(SOEXT) VSHDIR=$(VSHDIR) ACE_DEPEND_SED_CMD="$(ACE_DEPEND_SED_CMD)" $(ACE_ROOT)/bin/g++dep -A $(DEPEND_CMD_ARGS) -f $(DEPENDENCY_FILE) $(CPPFLAGS) -DMAKEDEPEND $(LSRC) $(SRC) $(PSRC)
	@if cmp -s $(DEPENDENCY_FILE) $(DEPENDENCY_FILE).old ;\
	then echo "Makefile dependencies unchanged." ;\
	else \
		echo "Makefile dependencies updated." ;\
	fi ;\
	$(RM) $(DEPENDENCY_FILE).old ;

#----------------------------------------------------------------------------
#       RCS info target
#----------------------------------------------------------------------------

idl_stubs.local: $(foreach file, $(IDL_FILES), $(file)$(IDL_CLIENT_HDR_EXT))

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
	@if not exist $@ mkdir $@
else
$(OBJDIRS):
	@test -d $@ || mkdir $@ $(ACE_NUL_STDERR)
endif

#---------------------------------------------------------------------------
#       Source file splitting
#---------------------------------------------------------------------------
SPLIT:
	@echo "Splitting source files..."
	@$(ACE_ROOT)/bin/split-cpp -s Svc_Conf_l.cpp -s Svc_Conf_y.cpp $(LSRC)
	@echo "done."
