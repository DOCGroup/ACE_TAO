# $Id$

#----------------------------------------------------------------------------
#       Makefile for the libACE_OS
#----------------------------------------------------------------------------

MAKEFILE = Makefile
LIBNAME   = libACE_Utils
LIB      = $(LIBNAME).a
SHLIB    = $(LIBNAME).$(SOEXT)


FILES = \
	ARGV\
	Active_Map_Manager\
	Active_Map_Manager_T\
	Arg_Shifter\
	Capabilities\
	Configuration\
	Configuration_Import_Export\
	Connection_Recycling_Strategy\
	Containers\
	Dirent\
	Dirent_Selector\
	Dynamic\
	Filecache\
	Flag_Manip\
	Functor\
	Functor_T\
	Future\
	Future_Set\
	Get_Opt\
	Hash_Map_Manager\
	Hashable\
	IO_Cntl_Msg\
	Init_ACE\
	Lib_Find\
	Map\
	Message_Block\
	Method_Request\
	Multiplexor\
	NT_Service\
	Object_Manager\
	Pair\
	Recyclable\
	Refcountable\
	Registry\
	SString\
	Sample_History\
	Stats\
	String_Base_Const\
	Test_and_Set\
	Unbounded_Queue\
	Unbounded_Set

#----------------------------------------------------------------------------
#       Include macros and targets
#----------------------------------------------------------------------------

include $(ACE_ROOT)/include/makeinclude/wrapper_macros.GNU

####
#### ACE_COMPONENTS support.
####

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
#       Local targets
#----------------------------------------------------------------------------



#----------------------------------------------------------------------------
#       Dependencies
#----------------------------------------------------------------------------
# DO NOT DELETE THIS LINE -- g++dep uses it.
# DO NOT PUT ANYTHING AFTER THIS LINE, IT WILL GO AWAY.



