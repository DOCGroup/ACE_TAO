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

ifndef OBJEXT
  OBJEXT=o
endif

ifndef SOEXT
  SOEXT=so
endif

VSHLIB  = $(SHLIB:%.$(SOEXT)=%$(VAR).$(SOEXT))
ifdef SHLIBA
VSHLIBA = $(SHLIBA:%.a=%$(VAR).a)
endif

VLOBJS   = $(subst .cpp,.$(OBJEXT),$(foreach file,$(LSRC),$(VDIR)$(notdir $(file))))

LSRC    += $(LSRC2)
ifdef PRELIB
ifdef PRELIB_USES_OBJ_ONLY
  VSHOBJS = $(subst .cpp,.o,$(foreach file,$(LSRC),$(VSHDIR)$(notdir $(file))))
else  # ! PRELIB_USES_OBJ_ONLY
  VSHOBJS = $(subst .cpp,.o,$(foreach file,$(LSRC),$(VSHDIR)$(notdir $(file))))
endif # ! PRELIB_USES_OBJ_ONLY
  VSHOBJS1 = $(VSHOBJS)
else  # ! PRELIB
  VSHOBJS = $(subst .cpp,.o,$(foreach file,$(LSRC),$(VSHDIR)$(notdir $(file))))
  VSHOBJS1 = \
    $(subst .cpp,.o,$(foreach file,$(LSRC),$(VSHDIR)$(notdir $(file))))
endif # ! PRELIB

# Comment out for now...
# $(DEFS:%=$(INSINC)/%) \

ifdef shared_libs_only
  INSTALL += \
    $(VSHLIB:%.$(SOEXT)=$(INSLIB)/%.$(SOEXT)$(ACE_LDSO_Version_Number))
ifdef SHLIBA
  INSTALL += $(VSHLIBA:%=$(INSLIB)/%)
endif
else
ifdef static_libs_only
  INSTALL += $(VLIB:%.a=$(INSLIB)/%.a)
else
  INSTALL += $(VLIB:%.a=$(INSLIB)/%.a) \
    $(VSHLIB:%.$(SOEXT)=$(INSLIB)/%.$(SOEXT)$(ACE_LDSO_Version_Number))
ifdef SHLIBA
  INSTALL += $(VSHLIBA:%=$(INSLIB)/%)
endif
endif # static_libs_only
endif # shared_libs_only

ifdef PRELIB
# Unfortunately, gcc has to do a link of all of the objects (during which
# it may decide to recompile some of the objects), before we can safely build
# any libraries or shared objects.  Comment this line if no shared
# libraries or objects are used.
#
# I have no idea why this dependency has any effect similar to what is
# described above, so I will comment it out.  This eliminates the VERY
# annoying messages from make such as:
# make: Circular .shobj/interp.so <- libcorba.so dependency dropped.
#$(VSHOBJS): $(VSHLIB)
endif

# Comment these lines out if you want to build both *.a and *.so libraries...
#$(VSHLIB): $(VSHOBJS)
#ifdef PRELIB
#	$(PRELIB)
#endif
#	$(SOLINK.cc) -o $@ $(LDFLAGS) $(VSHDIR)*.o

# Uncomment the remaining lines if you want to build both *.a and *.so libraries...
VLIB    = $(LIB:%.a=%$(VAR).a)

$(VLIB): $(VLOBJS)
ifdef PRELIB
	$(PRELIB)
endif
	$(AR) $(ARFLAGS) $@ $?
ifdef TEMPINCDIR
# This is required to get AIX xlC to instantiate and compile the needed
# templates.
	if test -s ./$(TEMPINCDIR)/*.C; \
	then \
		$(LINK.cc) -o dummy $(LDFLAGS) $(ACE_ROOT)/etc/xlc_dummy.cpp $^ $(LIBS); \
		$(RM) dummy; \
		$(AR) $(ARFLAGS) $@ tempinc/*.o; \
	fi
endif
	-chmod a+r $@
ifneq (,$(RANLIB))
	-$(RANLIB) $@
endif # RANLIB

# Note that if you don't want to build shared libraries, just remove the $(VSHOBJS)
ifdef SHLIBBUILD
$(VSHLIB): $(VSHOBJS)
else
$(VSHLIB): $(VSHOBJS1)
endif
ifdef PRELIB
	$(PRELIB)
endif
ifdef TEMPINCDIR
# This is required to get AIX xlC to instantiate and compile the needed
# templates before building the library.
	if test -s ./$(TEMPINCDIR)/*.C; \
	then \
		$(LINK.cc) -o dummy $(LDFLAGS) $(ACE_ROOT)/etc/xlc_dummy.cpp $^ $(LIBS); \
		$(RM) dummy; \
	fi
endif
ifdef SHLIBBUILD
	$(SHLIBBUILD)
else
ifdef TEMPINCDIR
	if test -s ./$(TEMPINCDIR)/*.o; \
        then \
       	        $(SOLINK.cc) -o $@ $(VSHOBJS1) $(TEMPINCDIR)/*.o $(LDFLAGS) $(LIBS); \
        else \
		$(SOLINK.cc) -o $@ $(VSHOBJS1) $(LDFLAGS) $(LIBS); \
	fi
else
	$(SOLINK.cc) -o $@ $(VSHOBJS1) $(LDFLAGS) $(LIBS)
endif
	-chmod a+rx $@
endif

ifdef SHLIBA
$(SHLIBA): $(VSHLIB)
	$(AR) $(ARFLAGS) $@ $?
	-chmod a+rx $@
ifneq (,$(RANLIB))
	-$(RANLIB) $@
endif # RANLIB
endif
