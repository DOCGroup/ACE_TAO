#----------------------------------------------------------------------------
#	$Id$
#
#	Top-level Makefile for the TAO IDL compiler
#----------------------------------------------------------------------------

#----------------------------------------------------------------------------
#	Local macros
#----------------------------------------------------------------------------

INFO	= 
DIRS	= driver fe ast util narrow be
BIN	= tao
TAOLIBS	= -ldrv -lfe -last -lutil -lnarrow -lbe
CLONE	= 

#----------------------------------------------------------------------------
#	Include macros and targets
#----------------------------------------------------------------------------

include $(WRAPPER_ROOT)/include/makeinclude/wrapper_macros.GNU
include $(WRAPPER_ROOT)/include/makeinclude/macros.GNU
include $(WRAPPER_ROOT)/include/makeinclude/rules.common.GNU
include $(WRAPPER_ROOT)/include/makeinclude/rules.nested.GNU
include $(WRAPPER_ROOT)/include/makeinclude/rules.nolocal.GNU

#----------------------------------------------------------------------------
#	Local targets
#----------------------------------------------------------------------------

all:
	cp ./driver/libdrv.so ./fe/libfe.so ./ast/libast.so ./util/libutil.so \
	./narrow/libnarrow.so ./be/libbe.so /project/mambo/gokhale/lib
	$(CXX) -g -o tao $(TAOLIBS) $(LIBS) -lthread

tags:
	rm -f TAGS
	etags -C include/*.h be_include/*.h 
	etags -a ast/*.cpp util/*.cpp narrow/*.cpp \
		driver/*.cpp fe/*.cpp be/*.cpp
clean:
	rm -f tao

realclean:
	rm -f tao TAGS

