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
VLOBJS   = $(subst .cpp,.$(OBJEXT),$(foreach file,$(LSRC),$(VDIR)$(notdir $(file))))

ifdef TEMPINCDIR
VSHOBJS1 =
else
LSRC    += $(LSRC2)
ifdef PRELIB
ifdef PRELIB_USES_OBJ_ONLY
VSHOBJS = $(subst .cpp,.o,$(foreach file,$(LSRC),$(VSHDIR)$(notdir $(file))))
else
VSHOBJS = $(subst .cpp,.o,$(foreach file,$(LSRC),$(VSHDIR)$(notdir $(file))))
endif
VSHOBJS1 = $(VSHOBJS)
else
VSHOBJS = $(subst .cpp,.o,$(foreach file,$(LSRC),$(VSHDIR)$(notdir $(file))))
VSHOBJS1 = $(subst .cpp,.o,$(foreach file,$(LSRC),$(VSHDIR)$(notdir $(file))))
endif
endif

# Comment out for now...
# $(DEFS:%=$(INSINC)/%) \

ifdef shared_libs_only
  INSTALL += \
    $(VSHLIB:%.$(SOEXT)=$(INSLIB)/%.$(SOEXT)$(ACE_LDSO_Version_Number))
else
ifdef static_libs_only
  INSTALL += $(VLIB:%.a=$(INSLIB)/%.a)
else
  INSTALL += $(VLIB:%.a=$(INSLIB)/%.a) \
    $(VSHLIB:%.$(SOEXT)=$(INSLIB)/%.$(SOEXT)$(ACE_LDSO_Version_Number))
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
#$(VSHLIB): $(VSHOBJS) $(STATLIB)
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
# This is required for AIX!!!
	if test -e xxx; \
	then  \
		rm xxx; \
	fi
	(if test -s ./$(TEMPINCDIR)/*.C; \
	then  \
		while ! test -e xxx; \
		do  \
			touch xxx; \
			cd ./$(TEMPINCDIR); \
			for temp in *.C; \
	 		do  \
				if ! test -d ./$(TEMPINCDIR); \
				then  \
					cd ..; \
				fi; \
				if $(COMPILE.cc) -o $(VDIR)$$temp.o ./$(TEMPINCDIR)/$$temp; \
				then  \
					$(AR) $(ARFLAGS) $@ $(VDIR)$$temp.o;\
				else  \
					rm $@; \
					rm xxx; \
					exit; \
				fi \
	 		done;  \
			for temp in ./$(TEMPINCDIR)/*.C; \
	 		do  \
				if test $$temp -nt xxx; \
				then \
					rm xxx; \
					break; \
				fi \
	 		done  \
	 	done;  \
	fi)
	rm xxx
endif
	-chmod a+r $@
	-$(RANLIB) $@

# Note that if you don't want to build shared libraries, just remove the $(VSHOBJS)
ifdef SHLIBBUILD
$(VSHLIB): $(VSHOBJS) $(STATLIB)
else
$(VSHLIB): $(VSHOBJS1) $(STATLIB)
endif
ifdef PRELIB
	$(PRELIB)
endif
ifdef SHLIBBUILD
	$(SHLIBBUILD)
else
	$(SOLINK.cc) -o $@ $(VSHOBJS1) $(LDFLAGS) $(LIBS) $(STATLIB)
	-chmod a+rx $@
endif

ifdef SHLIBA
$(SHLIBA): $(VSHLIB)
	$(AR) $(ARFLAGS) $@ $?
	-chmod a+rx $@
	-$(RANLIB) $@
endif
