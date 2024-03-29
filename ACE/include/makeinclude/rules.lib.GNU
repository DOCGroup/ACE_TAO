# -*- Makefile -*-
#----------------------------------------------------------------------------
#
#
#       Build libraries (i.e., contain no binary executables)
#       GNU version
#       Requires GNU make
#----------------------------------------------------------------------------

#---------------------------------------------------------------------------
#       Library versioning
#---------------------------------------------------------------------------

ifneq ($(versioned_so),0)
  # Turn on symbol versioning. The scheme that we follow is to allow
  # applications dependent on libraries, with same version numbers (major,
  # minor and beta) to run, but applications with dependencies on libraries
  # with different minor or major or beta versions to fail.
  #
  # Version number of the libraries
  #
  ifeq ($(SOVERSION),)
    ifeq ($(versioned_so),1)
      SOVERSION = .$(GNUACE_PROJECT_VERSION)
    else ifeq ($(versioned_so),2)
      SOVERSION = -$(GNUACE_PROJECT_VERSION)
    else ifeq ($(versioned_so),3)
      # Nothing
    else
      $(error unknown value for versioned_so: $(versioned_so))
    endif
  endif # SOVERSION

  # Name that will be written into the dynamic library
  #
  ifeq ($(SONAME),)
    ifeq ($(versioned_so),1)
      SONAME = $(SHLIB)$(SOVERSION)
    else ifeq ($(versioned_so),2)
      SONAME = $(patsubst %.$(SOEXT),%,$(SHLIB))$(SOVERSION).$(SOEXT)
    else ifeq ($(versioned_so),3)
      SONAME = $(SHLIB)
    endif
  endif # SONAME
endif # versioned_so


#----------------------------------------------------------------------------
# The following targets arrange to build both unshared and shared libraries
#----------------------------------------------------------------------------

VLIB = $(ARCH_LIB:%.$(LIBEXT)=%$(VAR).$(LIBEXT))
CLEANUP_VLIB = $(CLEANUP_LIB:%.$(LIBEXT)=%$(VAR).$(LIBEXT))

# Add VLIB to VLIBS if building static_libs.
ifdef static_libs
  VLIBS += $(VLIB)
endif

SHLIB ?=
# Setup shared library variables, including versions.
VSHLIB_NO_VER = $(ARCH_SHLIB:%.$(SOEXT)=%$(VAR).$(SOEXT))
CLEANUP_VSHLIB_NO_VER = $(CLEANUP_SHLIB:%.$(SOEXT)=%$(VAR).$(SOEXT))
ifneq ($(versioned_so),2)
  VSHLIB = $(VSHLIB_NO_VER:%=%$(SOVERSION))
  CLEANUP_VSHLIB = $(CLEANUP_VSHLIB_NO_VER:%=%$(SOVERSION))
  SHLIB_WITH_VER = $(SHLIB:%=%$(SOVERSION))
else
  VSHLIB = $(if $(VSHLIB_NO_VER), $(patsubst %.$(SOEXT),%,$(VSHLIB_NO_VER))$(SOVERSION).$(SOEXT))
  CLEANUP_VSHLIB = $(if $(CLEANUP_VSHLIB_NO_VER), $(patsubst %.$(SOEXT),%,$(CLEANUP_VSHLIB_NO_VER))$(SOVERSION).$(SOEXT))
  SHLIB_WITH_VER = $(if $(SHLIB), $(patsubst %.$(SOEXT),%,$(SHLIB))$(SOVERSION).$(SOEXT))
endif

SHLIBA ?=
VSHLIBA = $(ARCH_SHLIBA:%.$(LIBEXT)=%$(VAR).$(LIBEXT))

# Add VSHLIB if we're building shared_libs.
ifdef shared_libs
  VLIBS += $(VSHLIB) $(VSHLIBA)
endif

# Warn if no libs found
ifeq ($(VLIBS),)
  LIB_WARNING = 0
  ifndef LIB_UNCHECKED
    LIB_WARNING = 1
  endif # !LIB_CHECKED
  ifndef SHLIB_UNCHECKED
    LIB_WARNING = 1
  endif # !SHLIB_UNCHECKED
  ifeq ($(LIB_WARNING),1)
    @echo Warning: rules.lib.GNU included, but no library targets detected.
  endif # LIB_WARNING
else # VLIBS
  # Libraries always depend on idl_stubs, if they exist.
  IDL_SRC ?=
  ifeq ($(IDL_SRC),)
    $(VLIBS): $(IDL_SRC)
  endif # IDL_SRC
endif # !VLIBS

VLOBJS = $(addprefix $(VDIR),$(foreach var, $(addsuffix .$(OBJEXT), $(basename $(LSRC)) $(RESOURCES)), $(patsubst ../%,$(notdir $(var)),$(var))))
ifdef LSRC2
  LSRC += $(LSRC2)
endif

VSHOBJS = $(addprefix $(VSHDIR),$(foreach var, $(addsuffix .$(OBJEXT), $(basename $(LSRC)) $(RESOURCES)), $(patsubst ../%,$(notdir $(var)),$(var))))

ifneq ($(IDL_SRC),)
  $(VLOBJS) $(VSHOBJS): $(IDL_SRC)
endif

# Since INSLIB already has the ARCH dir appended, if appropriate, ARCH
# needs to be stripped from V[SH]LIB on the way over to INSLIB. To avoid
# an extra '/' which confuses the relative link calculation, ensure the
# '/' after ARCH is stripped if it's an ARCH build.
ARCHDIR ?=
ifdef ARCH
  ARCHDIR = $(ARCH)/
endif

ifneq ($(INSLIB),)
  ifdef static_libs
    LIB_INSTALL += $(VLIB:$(ARCHDIR)%=$(INSLIB)/%)
    CLEANUP_INSTALL += $(CLEANUP_VLIB:$(ARCHDIR)%=$(INSLIB)/%)
  endif # static_libs

  ifdef shared_libs
    LIB_INSTALL += $(VSHLIB:$(ARCHDIR)%=$(INSLIB)/%)
    CLEANUP_INSTALL += $(CLEANUP_VSHLIB:$(ARCHDIR)%=$(INSLIB)/%)
    CLEANUP_INSTALL += $(CLEANUP_VSHLIB_NO_VER:$(ARCHDIR)%=$(INSLIB)/%*)
    ifdef SHLIBA
      LIB_INSTALL += $(VSHLIBA:$(ARCHDIR)%=$(INSLIB)/%)
      CLEANUP_INSTALL += $(CLEANUP_SHLIBA:$(ARCHDIR)%=$(INSLIB)/%)
    endif
  endif # shared_libs
endif # INSLIB

ace_lib_prelink ?= 0

ifeq (1,$(VXWORKS))

  ifeq (1,$(ace_lib_prelink))
.PHONY: ace-templ_inst

ace-templ_inst:
	-$(LINK.cc) $(LINK_OUTPUT_FLAG) ace-templ_inst $(VLOBJS) $(LDFLAGS) $(ACE_SHLIBS)
	-$(RM) ace-templ_inst

$(VLIB): $(VLOBJS) ace-templ_inst
  else
$(VLIB): $(VLOBJS)
  endif
  ifeq (1,$(repo))
	@echo $(LINK.cc) $(LINK_OUTPUT_FLAG) > .prelink.spec
	@echo xargs $(AR) $(ARFLAGS) $(notdir $@) $(AREXTRA) >> .prelink.spec
	@if not [""]==["$(ACE_SHLIBS)"] @echo $(subst -l,,$(ACE_SHLIBS)) >> .prelink.spec
	@echo Add .rpo files
	echo .prelink.spec $(foreach rpo,$(patsubst %.$(OBJEXT),%.rpo,$(filter %.$(OBJEXT), $?)),$(shell if exist $(rpo) echo $(rpo))) | xargs $(AR) $(ARFLAGS) $@ $(AREXTRA)
  endif
	@echo Add .o files
	echo $(TMPINCDIR:%=%/*.$(OBJEXT)) $(filter %.$(OBJEXT), $?) | xargs $(AR) $(ARFLAGS) $@ $(AREXTRA)
	-chmod a+r $@
  ifneq (,$(RANLIB))
	-$(RANLIB) $@
  endif # RANLIB

else # !VXWORKS

$(VLIB): $(VLOBJS)
  ifeq (1,$(repo))
    ifneq (,$(ACELIB))
    #### With repo, link in all of the ace object files to the lib.
    #### This avoids multiple instantiations.  We haven't found it to
    #### be necessary with shared libs, so just do it for static libs.
    vpath %.$(OBJEXT) $(ACE_ROOT)/ace/$(VDIR)

    $(VLIB): $(ACE_ROOT)/ace/$(VDIR)*.$(OBJEXT)
    endif # ! ACELIB
  endif # repo
  ifdef PRELIB
	$(PRELIB)
  endif
  ifeq (1,$(ace_lib_prelink))
        #### Attempt a link (which will fail), to add template instantiations
        #### to lib files, for example.
	-$(LINK.cc) $(LINK_OUTPUT_FLAG) ace-templ_inst $? $(LDFLAGS) $(ACE_NUL_STDERR)
  endif # ace_lib_prelink
  ifneq (,$(AR.cc.override))
	$(AR.cc.override)
    else
	([ "`echo $(TMPINCDIR:%=%/*.$(OBJEXT))`" != "$(TMPINCDIR:%=%/*.$(OBJEXT))" ] && echo $(TMPINCDIR:%=%/*.$(OBJEXT)); \
	echo $(filter %.$(OBJEXT), $^)) | xargs $(AR) $(ARFLAGS) $@ $(AREXTRA)
  endif
	-chmod a+r $@
  ifneq (,$(RANLIB))
	-$(RANLIB) $@
  endif # RANLIB

endif # !VXWORKS

ifeq ($(VXWORKSLINK),1)
$(VSHLIB): $(basename $(notdir $(PRJ_FILE))).out
	-$(RM) $@
	-$(LN_S)  $(PROJECT_OUT) $@
	-$(POST_BUILD_RULE)
else # !VXWORKSLINK
$(VSHLIB): $(VSHOBJS)
  ifdef PRELIB
	$(PRELIB)
  endif # PRELIB
  ifdef SHLIBBUILD
	$(SHLIBBUILD)
    else # ! SHLIBBUILD
          ifeq (1,$(ace_lib_prelink))
          #### Attempt a link (which will fail), to add template instantiations
          #### to lib files, for example.
	-$(LINK.cc) $(LINK_OUTPUT_FLAG) ace-templ_inst $^ $(LDFLAGS) $(ACE_NUL_STDERR)
          endif # ace_lib_prelink
          ifneq ($(SOLINK.cc.override),)
	$(SOLINK.cc.override)
          else
	$(SHR_FILTER) $(SOLINK.cc) $(SO_OUTPUT_FLAG) $@ $^ $(LDFLAGS) $(ACE_SHLIBS) $(LIBS)
          endif #override
  endif  # SHLIBBUILD
endif # VXWORKSLINK
ifneq ($(SOVERSION),)
# This carefully works with both ARCH (where VSHLIB_* refer to the ARCH
# subdir, but SHLIB does not) and non-ARCH builds (where VSHLIB_* and SHLIB
# refer to the same place). SHLIB_WITH_VER should be the same as $@ but
# without the ARCH subdir.
	-$(RM) $(VSHLIB_NO_VER)
	-$(LN_S) $(if $(findstring cp,$(firstword $(LN_S))),$(@),$(SHLIB_WITH_VER)) $(VSHLIB_NO_VER)
endif
	-chmod a+rx $@
