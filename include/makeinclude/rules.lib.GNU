#----------------------------------------------------------------------------
#	@(#)rules.lib.GNU	1.1	10/18/96
#
#	Build libraries (i.e., contain no binary executables)
#	GNU version
#	Requires GNU make
#----------------------------------------------------------------------------

#----------------------------------------------------------------------------
# The following targets arrange to build both unshared and shared libraries
#----------------------------------------------------------------------------

VSHLIB  = $(SHLIB:%.so=%$(VAR).so)
VOBJS   = $(LSRC:%.cpp=$(VDIR)%.o)

ifdef TEMPINCDIR
VSHOBJS1 = 
else
LSRC    += $(LSRC2)
ifdef PRELIB
VSHOBJS	= $(LSRC:%.cpp=$(VSHDIR)%.so)
VSHOBJS1 = $(VSHOBJS)
else
VSHOBJS = $(LSRC:%.cpp=$(VSHDIR)%.so)
VSHOBJS1 = $(LSRC:%.cpp=$(VSHDIR)%.o)
endif
endif

# Comment out for now... 
# $(DEFS:%=$(INSINC)/%) \

INSTALL = $(VLIB:%.a=$(INSLIB)/%.a) \
	  $(VSHLIB:%.so=$(INSLIB)/%.so)

ifdef PRELIB
# Unfortunately, gcc has to do a link of all of the objects (during which
# it may decide to recompile some of the objects), before we can safely build
# any libraries or shared objects.  Comment this line if no shared
# libraries or objects are used.
$(VSHOBJS): $(VSHLIB)
endif

# Comment these lines out if you want to build both *.a and *.so libraries...
#$(VSHLIB): $(VSHOBJS) $(STATLIB)
#ifdef PRELIB
#	$(PRELIB)
#endif
#	$(SOLINK.cc) -o $@ $(LDFLAGS) $(VSHDIR)*.o

# Uncomment the remaining lines if you want to build both *.a and *.so libraries...
VLIB	= $(LIB:%.a=%$(VAR).a)

$(VLIB): $(VOBJS)
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
$(VSHLIB): $(VSHOBJS) $(STATLIB)
ifdef PRELIB
	$(PRELIB)
endif
	$(SOLINK.cc) -o $@ $(VSHOBJS1) $(LDFLAGS) $(LIBS) $(STATLIB)
	-chmod a+rx $@

ifdef SHLIBA
$(SHLIBA): $(VSHLIB)
	$(AR) $(ARFLAGS) $@ $?
	-chmod a+rx $@
	-$(RANLIB) $@
endif
