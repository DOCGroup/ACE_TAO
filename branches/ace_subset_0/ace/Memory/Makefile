# $Id$

#----------------------------------------------------------------------------
#       Makefile for the libACE_Memory
#----------------------------------------------------------------------------

MAKEFILE = Makefile
LIBNAME   = libACE_Memory
LIB      = $(LIBNAME).a
SHLIB    = $(LIBNAME).$(SOEXT)


LIB_FILES = \
	Based_Pointer_Repository\
	Based_Pointer_T\
	MEM_Acceptor\
	MEM_Addr\
	MEM_Connector\
	MEM_IO\
	MEM_SAP\
	MEM_Stream\
	Malloc\
	Malloc_Allocator\
	Malloc_Instantiations\
	Malloc_T\
	Mem_Map\
	Memory_Pool\
	Obchunk\
	Obstack\
	Obstack_T\
	PI_Malloc\
	Read_Buffer\
	Shared_Memory\
	Shared_Memory_MM\
	Shared_Memory_SV

#----------------------------------------------------------------------------
#       Include macros and targets
#----------------------------------------------------------------------------

include $(ACE_ROOT)/include/makeinclude/wrapper_macros.GNU

####
#### ACE_COMPONENTS support.
####
FILES += $(LIB_FILES)

LSRC  = $(addsuffix .cpp,$(FILES))

include $(ACE_ROOT)/include/makeinclude/macros.GNU
include $(ACE_ROOT)/include/makeinclude/rules.common.GNU
include $(ACE_ROOT)/include/makeinclude/rules.nested.GNU
include $(ACE_ROOT)/include/makeinclude/rules.lib.GNU
include $(ACE_ROOT)/include/makeinclude/rules.local.GNU

INSTALL =

clean:
	$(RM) -f $(LIBNAME).a $(LIBNAME).so

#----------------------------------------------------------------------------
#       Dependencies
#----------------------------------------------------------------------------
# DO NOT DELETE THIS LINE -- g++dep uses it.
# DO NOT PUT ANYTHING AFTER THIS LINE, IT WILL GO AWAY.

