#----------------------------------------------------------------------------
#	$Id$
#
#	Top-level Makefile for the TAO IDL compiler
#----------------------------------------------------------------------------

#----------------------------------------------------------------------------
#	Local macros
#----------------------------------------------------------------------------

#----------------------------------------------------------------------------
#	Include macros and targets
#----------------------------------------------------------------------------

include $(ACE_ROOT)/include/makeinclude/wrapper_macros.GNU
include $(ACE_ROOT)/include/makeinclude/macros.GNU

ifdef VXWORKS
  all.nested: any
  debug.nested: any
  profile.nested: any
  install.nested: any
  deinstall.nested: any
  clean.nested: any
  realclean.nested: any
  clobber.nested: any
  depend.nested: any
  rcs_info.nested: any
  any:
	@echo The IDL compiler won\'t run or build for VxWorks.
else
  include $(ACE_ROOT)/include/makeinclude/rules.lib.GNU
  include $(ACE_ROOT)/include/makeinclude/rules.common.GNU
  include $(ACE_ROOT)/include/makeinclude/rules.nested.GNU
  include $(ACE_ROOT)/include/makeinclude/rules.nolocal.GNU
endif # VXWORKS

INFO	=
# LIBS	= -lACE
DIRS	= driver fe ast util narrow be
BIN	= tao_idl
TAOLIBS	= -L$(ACE_ROOT)/ace -ldrv -lfe -last -lutil -lnarrow -lbe -lACE
CLONE	=
BUILD	= $(VBIN)

#----------------------------------------------------------------------------
#	Local targets
#----------------------------------------------------------------------------

all:
ifdef VXWORKS
	@echo The IDL compiler won\'t run or build for VxWorks.
else
	$(PRELINK) $(CXX) $(CFLAGS) -o $(BIN) $(TAOLIBS) $(LIBS)
endif # VXWORKS

tags:
	rm -f TAGS
	etags -C include/*.h be_include/*.h
	etags -a ast/*.cpp util/*.cpp narrow/*.cpp \
		driver/*.cpp fe/*.cpp be/*.cpp
clean:
	rm -f tao_idl

realclean:
	rm -f tao_idl TAGS
