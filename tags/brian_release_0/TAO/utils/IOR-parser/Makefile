#----------------------------------------------------------------------------
# $Id$
#
#	Makefile for the IOR Parser
#----------------------------------------------------------------------------

#----------------------------------------------------------------------------
#	Local macros
#----------------------------------------------------------------------------

BIN   	= ior-parser

FILES  	= ior-handler

SRC   	= $(addsuffix .cpp,$(FILES))
OBJ	= $(addsuffix .o,$(FILES))

TAO_IDLFLAGS += -Ge 1

ifndef TAO_ROOT
  TAO_ROOT = $(ACE_ROOT)/TAO
endif

#----------------------------------------------------------------------------
#	Include macros and targets
#----------------------------------------------------------------------------

include	$(ACE_ROOT)/include/makeinclude/wrapper_macros.GNU
include	$(ACE_ROOT)/include/makeinclude/macros.GNU
include $(TAO_ROOT)/rules.tao.GNU
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


.obj/ior-handler.o .obj/ior-handler.so .shobj/ior-handler.o .shobj/ior-handler.so: ior-handler.cpp ior-handler.h \
  $(ACE_ROOT)/ace/OS.h \
  $(ACE_ROOT)/ace/pre.h \
  $(ACE_ROOT)/ace/config-all.h \
  $(ACE_ROOT)/ace/config.h \
  $(ACE_ROOT)/ace/$(ACE_PLATFORM_CONFIG) \
  $(ACE_ROOT)/ace/post.h \
  $(ACE_ROOT)/ace/ace_wchar.h \
  $(ACE_ROOT)/ace/ace_wchar.inl \
  $(ACE_ROOT)/ace/OS_main.h \
  $(ACE_ROOT)/ace/ACE_export.h \
  $(ACE_ROOT)/ace/Cleanup.h \
  $(ACE_ROOT)/ace/Cleanup.inl \
  $(ACE_ROOT)/ace/Object_Manager_Base.h \
  $(ACE_ROOT)/ace/os_include/sys/os_types.h \
  $(ACE_ROOT)/ace/os_include/os_stddef.h \
  $(ACE_ROOT)/ace/os_include/os_signal.h \
  $(ACE_ROOT)/ace/os_include/os_time.h \
  $(ACE_ROOT)/ace/os_include/os_ucontext.h \
  $(ACE_ROOT)/ace/Object_Manager_Base.inl \
  $(ACE_ROOT)/ace/OS_NS_arpa_inet.h \
  $(ACE_ROOT)/ace/os_include/arpa/os_inet.h \
  $(ACE_ROOT)/ace/os_include/netinet/os_in.h \
  $(ACE_ROOT)/ace/os_include/os_inttypes.h \
  $(ACE_ROOT)/ace/os_include/os_stdint.h \
  $(ACE_ROOT)/ace/os_include/sys/os_socket.h \
  $(ACE_ROOT)/ace/os_include/sys/os_uio.h \
  $(ACE_ROOT)/ace/os_include/os_limits.h \
  $(ACE_ROOT)/ace/os_include/os_unistd.h \
  $(ACE_ROOT)/ace/os_include/os_stdio.h \
  $(ACE_ROOT)/ace/os_include/os_stdarg.h \
  $(ACE_ROOT)/ace/OS_NS_arpa_inet.inl \
  $(ACE_ROOT)/ace/OS_NS_string.h \
  $(ACE_ROOT)/ace/Basic_Types.h \
  $(ACE_ROOT)/ace/os_include/os_float.h \
  $(ACE_ROOT)/ace/os_include/os_stdlib.h \
  $(ACE_ROOT)/ace/os_include/sys/os_wait.h \
  $(ACE_ROOT)/ace/os_include/sys/os_resource.h \
  $(ACE_ROOT)/ace/os_include/sys/os_time.h \
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
  $(ACE_ROOT)/ace/OS_NS_stdio.h \
  $(ACE_ROOT)/ace/os_include/os_fcntl.h \
  $(ACE_ROOT)/ace/os_include/sys/os_stat.h \
  $(ACE_ROOT)/ace/OS_NS_stdio.inl \
  $(ACE_ROOT)/ace/OS_NS_unistd.h \
  $(ACE_ROOT)/ace/Time_Value.h \
  $(ACE_ROOT)/ace/Time_Value.inl \
  $(ACE_ROOT)/ace/OS_NS_sys_time.h \
  $(ACE_ROOT)/ace/OS_NS_sys_time.inl \
  $(ACE_ROOT)/ace/OS_NS_unistd.inl \
  $(ACE_ROOT)/ace/OS_NS_sys_utsname.h \
  $(ACE_ROOT)/ace/os_include/sys/os_utsname.h \
  $(ACE_ROOT)/ace/OS_NS_sys_utsname.inl \
  $(ACE_ROOT)/ace/OS_NS_macros.h \
  $(ACE_ROOT)/ace/OS_NS_fcntl.h \
  $(ACE_ROOT)/ace/Global_Macros.h \
  $(ACE_ROOT)/ace/OS_Errno.h \
  $(ACE_ROOT)/ace/OS_Errno.inl \
  $(ACE_ROOT)/ace/OS_NS_fcntl.inl \
  $(ACE_ROOT)/ace/Default_Constants.h \
  $(ACE_ROOT)/ace/OS_Memory.h \
  $(ACE_ROOT)/ace/OS_NS_stdlib.h \
  $(ACE_ROOT)/ace/OS_NS_stdlib.inl \
  $(ACE_ROOT)/ace/os_include/os_search.h \
  $(ACE_ROOT)/ace/OS_Memory.inl \
  $(ACE_ROOT)/ace/OS_NS_pwd.h \
  $(ACE_ROOT)/ace/os_include/os_pwd.h \
  $(ACE_ROOT)/ace/OS_NS_pwd.inl \
  $(ACE_ROOT)/ace/OS_NS_ctype.h \
  $(ACE_ROOT)/ace/OS_NS_ctype.inl \
  $(ACE_ROOT)/ace/OS_NS_dirent.h \
  $(ACE_ROOT)/ace/os_include/os_dirent.h \
  $(ACE_ROOT)/ace/OS_NS_dirent.inl \
  $(ACE_ROOT)/ace/OS_NS_dlfcn.h \
  $(ACE_ROOT)/ace/os_include/os_dlfcn.h \
  $(ACE_ROOT)/ace/OS_NS_dlfcn.inl \
  $(ACE_ROOT)/ace/OS_NS_math.h \
  $(ACE_ROOT)/ace/OS_NS_math.inl \
  $(ACE_ROOT)/ace/OS_NS_netdb.h \
  $(ACE_ROOT)/ace/os_include/os_netdb.h \
  $(ACE_ROOT)/ace/OS_NS_netdb.inl \
  $(ACE_ROOT)/ace/OS_NS_poll.h \
  $(ACE_ROOT)/ace/os_include/os_poll.h \
  $(ACE_ROOT)/ace/OS_NS_poll.inl \
  $(ACE_ROOT)/ace/OS_NS_regex.h \
  $(ACE_ROOT)/ace/OS_NS_regex.inl \
  $(ACE_ROOT)/ace/os_include/os_regex.h \
  $(ACE_ROOT)/ace/OS_NS_signal.h \
  $(ACE_ROOT)/ace/OS_NS_signal.inl \
  $(ACE_ROOT)/ace/OS_NS_strings.h \
  $(ACE_ROOT)/ace/os_include/os_strings.h \
  $(ACE_ROOT)/ace/OS_NS_strings.inl \
  $(ACE_ROOT)/ace/OS_NS_stropts.h \
  $(ACE_ROOT)/ace/os_include/os_stropts.h \
  $(ACE_ROOT)/ace/OS_NS_stropts.inl \
  $(ACE_ROOT)/ace/OS_QoS.h \
  $(ACE_ROOT)/ace/OS_NS_sys_mman.h \
  $(ACE_ROOT)/ace/OS_NS_sys_mman.inl \
  $(ACE_ROOT)/ace/os_include/sys/os_mman.h \
  $(ACE_ROOT)/ace/OS_NS_sys_msg.h \
  $(ACE_ROOT)/ace/os_include/sys/os_msg.h \
  $(ACE_ROOT)/ace/os_include/sys/os_ipc.h \
  $(ACE_ROOT)/ace/OS_NS_sys_msg.inl \
  $(ACE_ROOT)/ace/OS_NS_sys_resource.h \
  $(ACE_ROOT)/ace/OS_NS_sys_resource.inl \
  $(ACE_ROOT)/ace/OS_NS_sys_select.h \
  $(ACE_ROOT)/ace/os_include/sys/os_select.h \
  $(ACE_ROOT)/ace/OS_NS_sys_select.inl \
  $(ACE_ROOT)/ace/OS_NS_sys_shm.h \
  $(ACE_ROOT)/ace/os_include/sys/os_shm.h \
  $(ACE_ROOT)/ace/OS_NS_sys_shm.inl \
  $(ACE_ROOT)/ace/OS_NS_sys_socket.h \
  $(ACE_ROOT)/ace/os_include/net/os_if.h \
  $(ACE_ROOT)/ace/OS_NS_sys_socket.inl \
  $(ACE_ROOT)/ace/OS_NS_sys_uio.h \
  $(ACE_ROOT)/ace/OS_NS_sys_uio.inl \
  $(ACE_ROOT)/ace/OS_NS_sys_stat.h \
  $(ACE_ROOT)/ace/OS_NS_sys_stat.inl \
  $(ACE_ROOT)/ace/OS_NS_sys_wait.h \
  $(ACE_ROOT)/ace/OS_NS_sys_wait.inl \
  $(ACE_ROOT)/ace/OS_NS_Thread.h \
  $(ACE_ROOT)/ace/os_include/os_pthread.h \
  $(ACE_ROOT)/ace/Base_Thread_Adapter.h \
  $(ACE_ROOT)/ace/OS_Log_Msg_Attributes.h \
  $(ACE_ROOT)/ace/iosfwd.h \
  $(ACE_ROOT)/ace/OS_Log_Msg_Attributes.inl \
  $(ACE_ROOT)/ace/Base_Thread_Adapter.inl \
  $(ACE_ROOT)/ace/os_include/sys/os_sem.h \
  $(ACE_ROOT)/ace/os_include/os_semaphore.h \
  $(ACE_ROOT)/ace/OS_NS_Thread.inl \
  $(ACE_ROOT)/ace/OS_NS_time.h \
  $(ACE_ROOT)/ace/OS_NS_time.inl \
  $(ACE_ROOT)/ace/OS_Dirent.h \
  $(ACE_ROOT)/ace/OS_Dirent.inl \
  $(ACE_ROOT)/ace/OS_String.h \
  $(ACE_ROOT)/ace/OS_String.inl \
  $(ACE_ROOT)/ace/OS_TLI.h \
  $(ACE_ROOT)/ace/OS_TLI.inl \
  $(ACE_ROOT)/ace/Min_Max.h \
  $(ACE_ROOT)/ace/os_include/netinet/os_tcp.h \
  $(ACE_ROOT)/ace/os_include/os_assert.h \
  $(ACE_ROOT)/ace/os_include/os_termios.h \
  $(ACE_ROOT)/ace/os_include/os_aio.h \
  $(ACE_ROOT)/ace/os_include/sys/os_un.h \
  $(ACE_ROOT)/ace/os_include/os_syslog.h \
  $(ACE_ROOT)/ace/OS.i \
  $(ACE_ROOT)/ace/Log_Msg.h \
  $(ACE_ROOT)/ace/Log_Priority.h

.obj/ior-parser.o .obj/ior-parser.so .shobj/ior-parser.o .shobj/ior-parser.so: ior-parser.cpp ior-handler.h \
  $(ACE_ROOT)/ace/OS.h \
  $(ACE_ROOT)/ace/pre.h \
  $(ACE_ROOT)/ace/config-all.h \
  $(ACE_ROOT)/ace/config.h \
  $(ACE_ROOT)/ace/$(ACE_PLATFORM_CONFIG) \
  $(ACE_ROOT)/ace/post.h \
  $(ACE_ROOT)/ace/ace_wchar.h \
  $(ACE_ROOT)/ace/ace_wchar.inl \
  $(ACE_ROOT)/ace/OS_main.h \
  $(ACE_ROOT)/ace/ACE_export.h \
  $(ACE_ROOT)/ace/Cleanup.h \
  $(ACE_ROOT)/ace/Cleanup.inl \
  $(ACE_ROOT)/ace/Object_Manager_Base.h \
  $(ACE_ROOT)/ace/os_include/sys/os_types.h \
  $(ACE_ROOT)/ace/os_include/os_stddef.h \
  $(ACE_ROOT)/ace/os_include/os_signal.h \
  $(ACE_ROOT)/ace/os_include/os_time.h \
  $(ACE_ROOT)/ace/os_include/os_ucontext.h \
  $(ACE_ROOT)/ace/Object_Manager_Base.inl \
  $(ACE_ROOT)/ace/OS_NS_arpa_inet.h \
  $(ACE_ROOT)/ace/os_include/arpa/os_inet.h \
  $(ACE_ROOT)/ace/os_include/netinet/os_in.h \
  $(ACE_ROOT)/ace/os_include/os_inttypes.h \
  $(ACE_ROOT)/ace/os_include/os_stdint.h \
  $(ACE_ROOT)/ace/os_include/sys/os_socket.h \
  $(ACE_ROOT)/ace/os_include/sys/os_uio.h \
  $(ACE_ROOT)/ace/os_include/os_limits.h \
  $(ACE_ROOT)/ace/os_include/os_unistd.h \
  $(ACE_ROOT)/ace/os_include/os_stdio.h \
  $(ACE_ROOT)/ace/os_include/os_stdarg.h \
  $(ACE_ROOT)/ace/OS_NS_arpa_inet.inl \
  $(ACE_ROOT)/ace/OS_NS_string.h \
  $(ACE_ROOT)/ace/Basic_Types.h \
  $(ACE_ROOT)/ace/os_include/os_float.h \
  $(ACE_ROOT)/ace/os_include/os_stdlib.h \
  $(ACE_ROOT)/ace/os_include/sys/os_wait.h \
  $(ACE_ROOT)/ace/os_include/sys/os_resource.h \
  $(ACE_ROOT)/ace/os_include/sys/os_time.h \
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
  $(ACE_ROOT)/ace/OS_NS_stdio.h \
  $(ACE_ROOT)/ace/os_include/os_fcntl.h \
  $(ACE_ROOT)/ace/os_include/sys/os_stat.h \
  $(ACE_ROOT)/ace/OS_NS_stdio.inl \
  $(ACE_ROOT)/ace/OS_NS_unistd.h \
  $(ACE_ROOT)/ace/Time_Value.h \
  $(ACE_ROOT)/ace/Time_Value.inl \
  $(ACE_ROOT)/ace/OS_NS_sys_time.h \
  $(ACE_ROOT)/ace/OS_NS_sys_time.inl \
  $(ACE_ROOT)/ace/OS_NS_unistd.inl \
  $(ACE_ROOT)/ace/OS_NS_sys_utsname.h \
  $(ACE_ROOT)/ace/os_include/sys/os_utsname.h \
  $(ACE_ROOT)/ace/OS_NS_sys_utsname.inl \
  $(ACE_ROOT)/ace/OS_NS_macros.h \
  $(ACE_ROOT)/ace/OS_NS_fcntl.h \
  $(ACE_ROOT)/ace/Global_Macros.h \
  $(ACE_ROOT)/ace/OS_Errno.h \
  $(ACE_ROOT)/ace/OS_Errno.inl \
  $(ACE_ROOT)/ace/OS_NS_fcntl.inl \
  $(ACE_ROOT)/ace/Default_Constants.h \
  $(ACE_ROOT)/ace/OS_Memory.h \
  $(ACE_ROOT)/ace/OS_NS_stdlib.h \
  $(ACE_ROOT)/ace/OS_NS_stdlib.inl \
  $(ACE_ROOT)/ace/os_include/os_search.h \
  $(ACE_ROOT)/ace/OS_Memory.inl \
  $(ACE_ROOT)/ace/OS_NS_pwd.h \
  $(ACE_ROOT)/ace/os_include/os_pwd.h \
  $(ACE_ROOT)/ace/OS_NS_pwd.inl \
  $(ACE_ROOT)/ace/OS_NS_ctype.h \
  $(ACE_ROOT)/ace/OS_NS_ctype.inl \
  $(ACE_ROOT)/ace/OS_NS_dirent.h \
  $(ACE_ROOT)/ace/os_include/os_dirent.h \
  $(ACE_ROOT)/ace/OS_NS_dirent.inl \
  $(ACE_ROOT)/ace/OS_NS_dlfcn.h \
  $(ACE_ROOT)/ace/os_include/os_dlfcn.h \
  $(ACE_ROOT)/ace/OS_NS_dlfcn.inl \
  $(ACE_ROOT)/ace/OS_NS_math.h \
  $(ACE_ROOT)/ace/OS_NS_math.inl \
  $(ACE_ROOT)/ace/OS_NS_netdb.h \
  $(ACE_ROOT)/ace/os_include/os_netdb.h \
  $(ACE_ROOT)/ace/OS_NS_netdb.inl \
  $(ACE_ROOT)/ace/OS_NS_poll.h \
  $(ACE_ROOT)/ace/os_include/os_poll.h \
  $(ACE_ROOT)/ace/OS_NS_poll.inl \
  $(ACE_ROOT)/ace/OS_NS_regex.h \
  $(ACE_ROOT)/ace/OS_NS_regex.inl \
  $(ACE_ROOT)/ace/os_include/os_regex.h \
  $(ACE_ROOT)/ace/OS_NS_signal.h \
  $(ACE_ROOT)/ace/OS_NS_signal.inl \
  $(ACE_ROOT)/ace/OS_NS_strings.h \
  $(ACE_ROOT)/ace/os_include/os_strings.h \
  $(ACE_ROOT)/ace/OS_NS_strings.inl \
  $(ACE_ROOT)/ace/OS_NS_stropts.h \
  $(ACE_ROOT)/ace/os_include/os_stropts.h \
  $(ACE_ROOT)/ace/OS_NS_stropts.inl \
  $(ACE_ROOT)/ace/OS_QoS.h \
  $(ACE_ROOT)/ace/OS_NS_sys_mman.h \
  $(ACE_ROOT)/ace/OS_NS_sys_mman.inl \
  $(ACE_ROOT)/ace/os_include/sys/os_mman.h \
  $(ACE_ROOT)/ace/OS_NS_sys_msg.h \
  $(ACE_ROOT)/ace/os_include/sys/os_msg.h \
  $(ACE_ROOT)/ace/os_include/sys/os_ipc.h \
  $(ACE_ROOT)/ace/OS_NS_sys_msg.inl \
  $(ACE_ROOT)/ace/OS_NS_sys_resource.h \
  $(ACE_ROOT)/ace/OS_NS_sys_resource.inl \
  $(ACE_ROOT)/ace/OS_NS_sys_select.h \
  $(ACE_ROOT)/ace/os_include/sys/os_select.h \
  $(ACE_ROOT)/ace/OS_NS_sys_select.inl \
  $(ACE_ROOT)/ace/OS_NS_sys_shm.h \
  $(ACE_ROOT)/ace/os_include/sys/os_shm.h \
  $(ACE_ROOT)/ace/OS_NS_sys_shm.inl \
  $(ACE_ROOT)/ace/OS_NS_sys_socket.h \
  $(ACE_ROOT)/ace/os_include/net/os_if.h \
  $(ACE_ROOT)/ace/OS_NS_sys_socket.inl \
  $(ACE_ROOT)/ace/OS_NS_sys_uio.h \
  $(ACE_ROOT)/ace/OS_NS_sys_uio.inl \
  $(ACE_ROOT)/ace/OS_NS_sys_stat.h \
  $(ACE_ROOT)/ace/OS_NS_sys_stat.inl \
  $(ACE_ROOT)/ace/OS_NS_sys_wait.h \
  $(ACE_ROOT)/ace/OS_NS_sys_wait.inl \
  $(ACE_ROOT)/ace/OS_NS_Thread.h \
  $(ACE_ROOT)/ace/os_include/os_pthread.h \
  $(ACE_ROOT)/ace/Base_Thread_Adapter.h \
  $(ACE_ROOT)/ace/OS_Log_Msg_Attributes.h \
  $(ACE_ROOT)/ace/iosfwd.h \
  $(ACE_ROOT)/ace/OS_Log_Msg_Attributes.inl \
  $(ACE_ROOT)/ace/Base_Thread_Adapter.inl \
  $(ACE_ROOT)/ace/os_include/sys/os_sem.h \
  $(ACE_ROOT)/ace/os_include/os_semaphore.h \
  $(ACE_ROOT)/ace/OS_NS_Thread.inl \
  $(ACE_ROOT)/ace/OS_NS_time.h \
  $(ACE_ROOT)/ace/OS_NS_time.inl \
  $(ACE_ROOT)/ace/OS_Dirent.h \
  $(ACE_ROOT)/ace/OS_Dirent.inl \
  $(ACE_ROOT)/ace/OS_String.h \
  $(ACE_ROOT)/ace/OS_String.inl \
  $(ACE_ROOT)/ace/OS_TLI.h \
  $(ACE_ROOT)/ace/OS_TLI.inl \
  $(ACE_ROOT)/ace/Min_Max.h \
  $(ACE_ROOT)/ace/os_include/netinet/os_tcp.h \
  $(ACE_ROOT)/ace/os_include/os_assert.h \
  $(ACE_ROOT)/ace/os_include/os_termios.h \
  $(ACE_ROOT)/ace/os_include/os_aio.h \
  $(ACE_ROOT)/ace/os_include/sys/os_un.h \
  $(ACE_ROOT)/ace/os_include/os_syslog.h \
  $(ACE_ROOT)/ace/OS.i \
  $(ACE_ROOT)/ace/Log_Msg.h \
  $(ACE_ROOT)/ace/Log_Priority.h

# IF YOU PUT ANYTHING HERE IT WILL GO AWAY
