#----------------------------------------------------------------------------
#       $Id$
#
#       Build libraries (i.e., contain no binary executables)
#       GNU version
#       Requires GNU make
#----------------------------------------------------------------------------

#----------------------------------------------------------------------------
# The following targets arrange to build both unshared and shared libraries
#----------------------------------------------------------------------------

VSHLIB  = $(SHLIB:%.$(SOEXT)=%$(VAR).$(SOEXT))
ifdef SHLIBA
VSHLIBA = $(SHLIBA:%.$(LIBEXT)=%$(VAR).$(LIBEXT))
endif

VLOBJS   += $(addsuffix .$(OBJEXT),$(addprefix $(VDIR),$(basename $(notdir $(LSRC)))))
LSRC    += $(LSRC2)


VSHOBJS = $(addsuffix .$(OBJEXT),$(addprefix $(VSHDIR),$(basename $(notdir $(LSRC)))))
VSHOBJS1 = $(addsuffix .$(OBJEXT),$(addprefix $(VSHDIR),$(basename $(notdir $(LSRC)))))

ifdef static_libs
  INSTALL += $(VLIB:%.$(LIBEXT)=$(INSLIB)/%.$(LIBEXT))
endif # static_libs

ifdef shared_libs
  INSTALL += \
    $(VSHLIB:%.$(SOEXT)=$(INSLIB)/%.$(SOEXT)$(SOVERSION))
ifdef SHLIBA
  INSTALL += $(VSHLIBA:%=$(INSLIB)/%)
endif
endif # shared_libs

# Uncomment the remaining lines if you want to build both *.a and *.so
# libraries...
VLIB    = $(LIB:%.$(LIBEXT)=%$(VAR).$(LIBEXT))

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
	echo $(TMPINCDIR:%=%/*.o) $? | xargs $(AR) $(ARFLAGS) $@ $(AREXTRA);
ifdef ibmcxx_build
# This is required to get AIX IBM C/C++ to instantiate and compile the needed
# templates.
	if test -s ./$(TEMPINCDIR)/*.C; \
	then \
		$(LINK.cc) $(LINK_OUTPUT_FLAG) dummy $(LDFLAGS) $(ACE_ROOT)/etc/xlc_dummy.cpp $^ $(ACE_SHLIBS) $(LIBS); \
		$(RM) dummy; \
		$(AR) $(ARFLAGS) $@ tempinc/*.$(OBJEXT); \
	fi
endif
	-chmod a+r $@
ifneq (,$(RANLIB))
	-$(RANLIB) $@
endif # RANLIB

# Note that if you don't want to build shared libraries, just remove the
# $(VSHOBJS)
ifdef SHLIBBUILD
$(VSHLIB): $(VSHOBJS)
else
ifeq (1,$(aix_build))
$(VSHLIB): shr.o
	$(AR) $(ARFLAGS) $@ $?
	-chmod a+rx $@
ifneq (,$(RANLIB))
	-$(RANLIB) $@
endif # RANLIB

.INTERMEDIATE: shr.o

shr.o: $(VSHOBJS1)
else
$(VSHLIB): $(VSHOBJS1)
endif  # aix_build
endif  # SHLIBBUILD
ifdef PRELIB
	$(PRELIB)
endif
ifdef ibmcxx_build
# This is required to get AIX IBM C/C++ to instantiate and compile the needed
# templates before building the library.
	if test -s ./$(TEMPINCDIR)/*.C; \
	then \
		$(LINK.cc) $(LINK_OUTPUT_FLAG) dummy $(LDFLAGS) $(ACE_ROOT)/etc/xlc_dummy.cpp $^ $(ACE_SHLIBS) $(LIBS); \
		$(RM) dummy; \
	fi
endif
ifdef SHLIBBUILD
	$(SHLIBBUILD)
else
ifdef ibmcxx_build
	if test -s ./$(TEMPINCDIR)/*.$(OBJEXT); \
        then \
		$(SOLINK.cc) $(SO_OUTPUT_FLAG) $@ $(VSHOBJS1) $(TEMPINCDIR)/*.$(OBJEXT) $(LDFLAGS) $(ACE_SHLIBS) $(LIBS); \
        else \
		$(SOLINK.cc) $(SO_OUTPUT_FLAG) $@ $(VSHOBJS1) $(LDFLAGS) $(ACE_SHLIBS) $(LIBS); \
	fi
else
ifdef kylix
	$(SOLINK.cc) $(LDFLAGS) $(VSHOBJS1), $@,, $(ACE_SHLIBS) $(LIBS)
else
ifeq (1,$(ace_lib_prelink))
        #### Attempt a link (which will fail), to add template instantiations
        #### to lib files, for example.
	-$(LINK.cc) $(LINK_OUTPUT_FLAG) ace-templ_inst $(VSHOBJS1) $(LDFLAGS) $(ACE_NUL_STDERR)
endif # ace_lib_prelink
ifeq ($(split),1)
	$(SHR_FILTER) $(SOLINK.cc) $(SO_OUTPUT_FLAG) $@$(SOVERSION) $(VSHDIR)/*.$(OBJEXT) $(LDFLAGS) $(ACE_SHLIBS) $(LIBS)
else
	$(SHR_FILTER) $(SOLINK.cc) $(SO_OUTPUT_FLAG) $@$(SOVERSION) $(VSHOBJS1) $(LDFLAGS) $(ACE_SHLIBS) $(LIBS)
endif
endif
endif
ifneq ($(SOVERSION),)
	-$(RM) $@
	-$(LN_S) $@$(SOVERSION) $@
endif
	-chmod a+rx $@$(SOVERSION)
endif
