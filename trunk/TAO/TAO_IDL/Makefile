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
ifndef VXWORKS
  include $(ACE_ROOT)/include/makeinclude/macros.GNU
  include $(ACE_ROOT)/include/makeinclude/rules.lib.GNU
  include $(ACE_ROOT)/include/makeinclude/rules.common.GNU
  include $(ACE_ROOT)/include/makeinclude/rules.nested.GNU
  include $(ACE_ROOT)/include/makeinclude/rules.nolocal.GNU
endif # VXWORKS

INFO	=
# LIBS	= -lACE
DIRS	= driver fe ast util narrow be
BIN	= tao_idl
TAOLIBS	= -L$(ACE_ROOT)/ace -ldrv -lfe -last -ltao_idl_util -lnarrow -lbe -lACE
CLONE	=
BUILD	= $(VBIN)

#----------------------------------------------------------------------------
#	Local targets
#----------------------------------------------------------------------------

ifdef VXWORKS
all clean realclean:
	@echo The IDL compiler won\'t run or build for VxWorks.
else
all:
	$(PRELINK) $(CXX) $(CFLAGS) -o $(BIN) $(TAOLIBS) $(LIBS)

clean:
	rm -f tao_idl

realclean:
	rm -f tao_idl TAGS
endif # VXWORKS

tags:
	rm -f TAGS
	etags -C include/*.h be_include/*.h
	etags -a ast/*.cpp util/*.cpp narrow/*.cpp \
		driver/*.cpp fe/*.cpp be/*.cpp
