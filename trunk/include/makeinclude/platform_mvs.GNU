# For MVS OpenEdition platform
 
CXX           = cxx
CFLAGS        += $(DCFLAGS)
CCFLAGS       += $(CFLAGS)
DCFLAGS	      += -g
CPPFLAGS      = -+
DEFFLAGS      = -D_ALL_SOURCE -DNDEBUG
CC            = $(CXX)
LD            = $(CXX)
DLD           = $(CXX)
AR            = ar
ARFLAGS       = ruv
RANLIB        = echo
#LDFLAGS      = -W l,p,map
 
# MVCMD needed because cxx does not use the -o option to place objects
MVCMD         = @test ! -s $(@F) || mv $(@F) $(@D)/$(@F)
 
# Used to build static executables
MVSLIB_STATIC = -lACE
 
# Used to build shared executables (much smaller in size)
MVSLIB_DLL    = $(WRAPPER_ROOT)/ace/libACE.x
 
# set accordingly to build either static or shared executables
MVSLIB        = $(MVSLIB_DLL)
 
SOFLAGS       = -W l,dll
SOBUILD       = @echo $(COMPILE.cc) -W c,exportall -o $(VSHDIR)$*.o $<; \
                $(COMPILE.cc) -W c,exportall $<; \
                test ! -s $*.o || mv $*.o $(VSHDIR)/$*.o; \
                test -s $@ || ln -s $*.o $@
