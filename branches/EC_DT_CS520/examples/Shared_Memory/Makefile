#----------------------------------------------------------------------------
#	$Id$
#
#	Makefile for tests of the Shared_Malloc wrappers
#----------------------------------------------------------------------------

#----------------------------------------------------------------------------
#	Local macros
#----------------------------------------------------------------------------

BIN     = test_MM \
	  test_SV 

LSRC    = test_MM.cpp  \
	  test_SV.cpp 

LDLIBS  = 

VLDLIBS = $(LDLIBS:%=%$(VAR))

BUILD   = $(VBIN)

INSTALL =

#----------------------------------------------------------------------------
#	Include macros and targets
#----------------------------------------------------------------------------

include	$(ACE_ROOT)/include/makeinclude/wrapper_macros.GNU
include	$(ACE_ROOT)/include/makeinclude/macros.GNU
include	$(ACE_ROOT)/include/makeinclude/rules.common.GNU
include	$(ACE_ROOT)/include/makeinclude/rules.nonested.GNU
include	$(ACE_ROOT)/include/makeinclude/rules.bin.GNU
include	$(ACE_ROOT)/include/makeinclude/rules.local.GNU

#----------------------------------------------------------------------------
#	Local targets
#----------------------------------------------------------------------------

#----------------------------------------------------------------------------
#	Dependencies
#----------------------------------------------------------------------------

# DO NOT DELETE THIS LINE -- g++dep uses it.
# DO NOT PUT ANYTHING AFTER THIS LINE, IT WILL GO AWAY.


.obj/test_MM.o .obj/test_MM.so .shobj/test_MM.o .shobj/test_MM.so: test_MM.cpp \
  $(ACE_ROOT)/ace/Shared_Memory_MM.h \
  $(ACE_ROOT)/ace/pre.h \
  $(ACE_ROOT)/ace/Shared_Memory.h \
  $(ACE_ROOT)/ace/ACE_export.h \
  $(ACE_ROOT)/ace/post.h \
  $(ACE_ROOT)/ace/ace_wchar.h \
  $(ACE_ROOT)/ace/ace_wchar.inl \
  $(ACE_ROOT)/ace/Mem_Map.h \
  $(ACE_ROOT)/ace/Global_Macros.h \
  $(ACE_ROOT)/ace/OS_Export.h \
  $(ACE_ROOT)/ace/os_include/sys/os_mman.h \
  $(ACE_ROOT)/ace/os_include/sys/os_types.h \
  $(ACE_ROOT)/ace/os_include/os_stddef.h \
  $(ACE_ROOT)/ace/os_include/os_limits.h \
  $(ACE_ROOT)/ace/os_include/os_unistd.h \
  $(ACE_ROOT)/ace/os_include/os_inttypes.h \
  $(ACE_ROOT)/ace/os_include/os_stdint.h \
  $(ACE_ROOT)/ace/os_include/os_stdio.h \
  $(ACE_ROOT)/ace/os_include/os_stdarg.h \
  $(ACE_ROOT)/ace/os_include/os_fcntl.h \
  $(ACE_ROOT)/ace/os_include/sys/os_stat.h \
  $(ACE_ROOT)/ace/Default_Constants.h \
  $(ACE_ROOT)/ace/Mem_Map.i \
  $(ACE_ROOT)/ace/OS_NS_unistd.h \
  $(ACE_ROOT)/ace/Time_Value.h \
  $(ACE_ROOT)/ace/Time_Value.inl \
  $(ACE_ROOT)/ace/OS_NS_sys_time.h \
  $(ACE_ROOT)/ace/OS_NS_sys_time.inl \
  $(ACE_ROOT)/ace/os_include/sys/os_time.h \
  $(ACE_ROOT)/ace/os_include/sys/os_select.h \
  $(ACE_ROOT)/ace/os_include/os_signal.h \
  $(ACE_ROOT)/ace/os_include/os_time.h \
  $(ACE_ROOT)/ace/os_include/os_ucontext.h \
  $(ACE_ROOT)/ace/OS_NS_unistd.inl \
  $(ACE_ROOT)/ace/OS_NS_sys_utsname.h \
  $(ACE_ROOT)/ace/os_include/sys/os_utsname.h \
  $(ACE_ROOT)/ace/OS_NS_sys_utsname.inl \
  $(ACE_ROOT)/ace/OS_NS_string.h \
  $(ACE_ROOT)/ace/Basic_Types.h \
  $(ACE_ROOT)/ace/os_include/os_float.h \
  $(ACE_ROOT)/ace/os_include/os_stdlib.h \
  $(ACE_ROOT)/ace/os_include/sys/os_wait.h \
  $(ACE_ROOT)/ace/os_include/sys/os_resource.h \
  $(ACE_ROOT)/ace/Basic_Types.i \
  $(ACE_ROOT)/ace/OS_NS_string.inl \
  $(ACE_ROOT)/ace/OS_NS_wchar.h \
  $(ACE_ROOT)/ace/os_include/os_wchar.h \
  $(ACE_ROOT)/ace/os_include/os_string.h \
  $(ACE_ROOT)/ace/os_include/os_ctype.h \
  $(ACE_ROOT)/ace/OS_NS_wchar.inl \
  $(ACE_ROOT)/ace/OS_NS_errno.h \
  $(ACE_ROOT)/ace/os_include/os_errno.h \
  $(ACE_ROOT)/ace/OS_NS_errno.inl \
  $(ACE_ROOT)/ace/OS_NS_sys_mman.h \
  $(ACE_ROOT)/ace/OS_NS_sys_mman.inl \
  $(ACE_ROOT)/ace/OS_NS_fcntl.h \
  $(ACE_ROOT)/ace/OS_NS_fcntl.inl \
  $(ACE_ROOT)/ace/Shared_Memory_MM.i \
  $(ACE_ROOT)/ace/Log_Msg.h \
  $(ACE_ROOT)/ace/Log_Priority.h \
  $(ACE_ROOT)/ace/OS_Log_Msg_Attributes.h \
  $(ACE_ROOT)/ace/iosfwd.h \
  $(ACE_ROOT)/ace/OS_Log_Msg_Attributes.inl \
  $(ACE_ROOT)/ace/OS_NS_stdlib.h \
  $(ACE_ROOT)/ace/OS_NS_stdlib.inl \
  $(ACE_ROOT)/ace/Object_Manager_Base.h \
  $(ACE_ROOT)/ace/Cleanup.h \
  $(ACE_ROOT)/ace/Cleanup.inl \
  $(ACE_ROOT)/ace/Object_Manager_Base.inl

.obj/test_SV.o .obj/test_SV.so .shobj/test_SV.o .shobj/test_SV.so: test_SV.cpp \
  $(ACE_ROOT)/ace/Shared_Memory_SV.h \
  $(ACE_ROOT)/ace/pre.h \
  $(ACE_ROOT)/ace/Shared_Memory.h \
  $(ACE_ROOT)/ace/ACE_export.h \
  $(ACE_ROOT)/ace/post.h \
  $(ACE_ROOT)/ace/ace_wchar.h \
  $(ACE_ROOT)/ace/ace_wchar.inl \
  $(ACE_ROOT)/ace/SV_Shared_Memory.h \
  $(ACE_ROOT)/ace/os_include/sys/os_ipc.h \
  $(ACE_ROOT)/ace/os_include/sys/os_types.h \
  $(ACE_ROOT)/ace/os_include/os_stddef.h \
  $(ACE_ROOT)/ace/Default_Constants.h \
  $(ACE_ROOT)/ace/SV_Shared_Memory.i \
  $(ACE_ROOT)/ace/OS_NS_sys_shm.h \
  $(ACE_ROOT)/ace/os_include/sys/os_shm.h \
  $(ACE_ROOT)/ace/OS_NS_sys_shm.inl \
  $(ACE_ROOT)/ace/Global_Macros.h \
  $(ACE_ROOT)/ace/OS_Export.h \
  $(ACE_ROOT)/ace/OS_NS_errno.h \
  $(ACE_ROOT)/ace/os_include/os_errno.h \
  $(ACE_ROOT)/ace/OS_NS_errno.inl \
  $(ACE_ROOT)/ace/Shared_Memory_SV.i \
  $(ACE_ROOT)/ace/Log_Msg.h \
  $(ACE_ROOT)/ace/Log_Priority.h \
  $(ACE_ROOT)/ace/os_include/os_limits.h \
  $(ACE_ROOT)/ace/os_include/os_unistd.h \
  $(ACE_ROOT)/ace/os_include/os_inttypes.h \
  $(ACE_ROOT)/ace/os_include/os_stdint.h \
  $(ACE_ROOT)/ace/os_include/os_stdio.h \
  $(ACE_ROOT)/ace/os_include/os_stdarg.h \
  $(ACE_ROOT)/ace/OS_Log_Msg_Attributes.h \
  $(ACE_ROOT)/ace/iosfwd.h \
  $(ACE_ROOT)/ace/OS_Log_Msg_Attributes.inl \
  $(ACE_ROOT)/ace/OS_NS_unistd.h \
  $(ACE_ROOT)/ace/Time_Value.h \
  $(ACE_ROOT)/ace/Time_Value.inl \
  $(ACE_ROOT)/ace/OS_NS_sys_time.h \
  $(ACE_ROOT)/ace/OS_NS_sys_time.inl \
  $(ACE_ROOT)/ace/os_include/sys/os_time.h \
  $(ACE_ROOT)/ace/os_include/sys/os_select.h \
  $(ACE_ROOT)/ace/os_include/os_signal.h \
  $(ACE_ROOT)/ace/os_include/os_time.h \
  $(ACE_ROOT)/ace/os_include/os_ucontext.h \
  $(ACE_ROOT)/ace/OS_NS_unistd.inl \
  $(ACE_ROOT)/ace/OS_NS_sys_utsname.h \
  $(ACE_ROOT)/ace/os_include/sys/os_utsname.h \
  $(ACE_ROOT)/ace/OS_NS_sys_utsname.inl \
  $(ACE_ROOT)/ace/OS_NS_string.h \
  $(ACE_ROOT)/ace/Basic_Types.h \
  $(ACE_ROOT)/ace/os_include/os_float.h \
  $(ACE_ROOT)/ace/os_include/os_stdlib.h \
  $(ACE_ROOT)/ace/os_include/sys/os_wait.h \
  $(ACE_ROOT)/ace/os_include/sys/os_resource.h \
  $(ACE_ROOT)/ace/Basic_Types.i \
  $(ACE_ROOT)/ace/OS_NS_string.inl \
  $(ACE_ROOT)/ace/OS_NS_wchar.h \
  $(ACE_ROOT)/ace/os_include/os_wchar.h \
  $(ACE_ROOT)/ace/os_include/os_string.h \
  $(ACE_ROOT)/ace/os_include/os_ctype.h \
  $(ACE_ROOT)/ace/OS_NS_wchar.inl

# IF YOU PUT ANYTHING HERE IT WILL GO AWAY
