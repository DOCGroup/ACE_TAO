#----------------------------------------------------------------------------
#	$Id$
#
#	Top-level Makefile for the TAO IDL compiler
#----------------------------------------------------------------------------

#----------------------------------------------------------------------------
#	Local macros
#----------------------------------------------------------------------------

INFO	= 
LIBS	= -lACE
DIRS	= driver fe ast util narrow be
BIN	= tao
TAOLIBS	= -ldrv -lfe -last -lutil -lnarrow -lbe
CLONE	= 
BUILD	= $(VBIN)
#----------------------------------------------------------------------------
#	Include macros and targets
#----------------------------------------------------------------------------

include $(ACE_ROOT)/include/makeinclude/wrapper_macros.GNU
include $(ACE_ROOT)/include/makeinclude/macros.GNU
include $(ACE_ROOT)/include/makeinclude/rules.lib.GNU
include $(ACE_ROOT)/include/makeinclude/rules.common.GNU
include $(ACE_ROOT)/include/makeinclude/rules.nested.GNU
include $(ACE_ROOT)/include/makeinclude/rules.nolocal.GNU

#----------------------------------------------------------------------------
#	Local targets
#----------------------------------------------------------------------------

all:
	$(PRELINK) $(CXX) -g -o tao $(TAOLIBS) $(LIBS) -lthread

tags:
	rm -f TAGS
	etags -C include/*.h be_include/*.h 
	etags -a ast/*.cpp util/*.cpp narrow/*.cpp \
		driver/*.cpp fe/*.cpp be/*.cpp
clean:
	rm -f tao

realclean:
	rm -f tao TAGS

