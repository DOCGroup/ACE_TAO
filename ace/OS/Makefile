# $Id$

#----------------------------------------------------------------------------
#       Makefile for the libACE_OS
#----------------------------------------------------------------------------

MAKEFILE = Makefile
LIBOS   = libACE_OS
LIB      = $(LIBOS).a
SHLIB    = $(LIBOS).$(SOEXT)


OS_FILES = \
	Base_Thread_Adapter\
	Basic_Types\
	Date_Time\
	gethrtime\
	Handle_Ops\
	Handle_Set\
	OS\
	OS_Dirent\
	OS_Errno\
	OS_Log_Msg_Attributes\
	OS_Memory\
	OS_QoS\
	OS_String\
	OS_Thread_Adapter\
	OS_TLI\
	Sched_Params\
	Thread_Hook

#----------------------------------------------------------------------------
#       Include macros and targets
#----------------------------------------------------------------------------

include $(ACE_ROOT)/include/makeinclude/wrapper_macros.GNU

####
#### ACE_COMPONENTS support.
####
FILES += $(OS_FILES)

LSRC  = $(addsuffix .cpp,$(FILES))

include $(ACE_ROOT)/include/makeinclude/macros.GNU
include $(ACE_ROOT)/include/makeinclude/rules.common.GNU
include $(ACE_ROOT)/include/makeinclude/rules.nested.GNU
include $(ACE_ROOT)/include/makeinclude/rules.lib.GNU
include $(ACE_ROOT)/include/makeinclude/rules.local.GNU

INSTALL =

clean:
	$(RM) -f $(LIBOS).a $(LIBOS).so

#----------------------------------------------------------------------------
#       Local targets
#----------------------------------------------------------------------------



#----------------------------------------------------------------------------
#       Dependencies
#----------------------------------------------------------------------------
# DO NOT DELETE THIS LINE -- g++dep uses it.
# DO NOT PUT ANYTHING AFTER THIS LINE, IT WILL GO AWAY.


.obj/Base_Thread_Adapter.o .obj/Base_Thread_Adapter.o .obj/Base_Thread_Adapter.o .obj/Base_Thread_Adapter.o: Base_Thread_Adapter.cpp \
 Base_Thread_Adapter.h \
 $(ACE_ROOT)/ace/pre.h \
 OS_Log_Msg_Attributes.h \
 $(ACE_ROOT)/ace/post.h \
 $(ACE_ROOT)/ace/ACE_export.h \
 $(ACE_ROOT)/ace/svc_export.h \
 $(ACE_ROOT)/ace/ace_wchar.h \
 $(ACE_ROOT)/ace/ace_wchar.inl \
 OS_Errno.h \
 OS_Export.h \
 OS_Errno.inl \
 $(ACE_ROOT)/ace/streams.h \
 OS_Log_Msg_Attributes.inl \
 Base_Thread_Adapter.inl \
 OS.h \
 OS_Dirent.h \
 OS_Dirent.inl \
 OS_String.h \
 Basic_Types.h \
 Basic_Types.i \
 OS_String.inl \
 OS_Memory.h \
 OS_Memory.inl \
 OS_TLI.h \
 OS_TLI.inl \
 $(ACE_ROOT)/ace/Default_Constants.h \
 Global_Macros.h \
 $(ACE_ROOT)/ace/Min_Max.h \
 OS.i

.obj/Basic_Types.o .obj/Basic_Types.o .obj/Basic_Types.o .obj/Basic_Types.o: Basic_Types.cpp \
 OS.h \
 $(ACE_ROOT)/ace/pre.h \
 $(ACE_ROOT)/ace/post.h \
 $(ACE_ROOT)/ace/ACE_export.h \
 $(ACE_ROOT)/ace/svc_export.h \
 $(ACE_ROOT)/ace/ace_wchar.h \
 $(ACE_ROOT)/ace/ace_wchar.inl \
 OS_Errno.h \
 OS_Export.h \
 OS_Errno.inl \
 OS_Dirent.h \
 OS_Dirent.inl \
 OS_String.h \
 Basic_Types.h \
 Basic_Types.i \
 OS_String.inl \
 OS_Memory.h \
 OS_Memory.inl \
 OS_TLI.h \
 OS_TLI.inl \
 $(ACE_ROOT)/ace/Default_Constants.h \
 Global_Macros.h \
 $(ACE_ROOT)/ace/Min_Max.h \
 $(ACE_ROOT)/ace/streams.h \
 OS.i \
 $(ACE_ROOT)/ace/Template_Instantiations.cpp

.obj/Date_Time.o .obj/Date_Time.o .obj/Date_Time.o .obj/Date_Time.o: Date_Time.cpp \
 Date_Time.h \
 $(ACE_ROOT)/ace/pre.h \
 $(ACE_ROOT)/ace/ACE.h \
 OS.h \
 $(ACE_ROOT)/ace/post.h \
 $(ACE_ROOT)/ace/ACE_export.h \
 $(ACE_ROOT)/ace/svc_export.h \
 $(ACE_ROOT)/ace/ace_wchar.h \
 $(ACE_ROOT)/ace/ace_wchar.inl \
 OS_Errno.h \
 OS_Export.h \
 OS_Errno.inl \
 OS_Dirent.h \
 OS_Dirent.inl \
 OS_String.h \
 Basic_Types.h \
 Basic_Types.i \
 OS_String.inl \
 OS_Memory.h \
 OS_Memory.inl \
 OS_TLI.h \
 OS_TLI.inl \
 $(ACE_ROOT)/ace/Default_Constants.h \
 Global_Macros.h \
 $(ACE_ROOT)/ace/Min_Max.h \
 $(ACE_ROOT)/ace/streams.h \
 OS.i \
 $(ACE_ROOT)/ace/Utils/Flag_Manip.h \
 $(ACE_ROOT)/ace/Utils/Flag_Manip.i \
 Handle_Ops.h \
 Handle_Ops.i \
 $(ACE_ROOT)/ace/Utils/Lib_Find.h \
 $(ACE_ROOT)/ace/Utils/Lib_Find.i \
 $(ACE_ROOT)/ace/Utils/Init_ACE.h \
 $(ACE_ROOT)/ace/Utils/Init_ACE.i \
 $(ACE_ROOT)/ace/Connection/Sock_Connect.h \
 $(ACE_ROOT)/ace/Connection/Sock_Connect.i \
 $(ACE_ROOT)/ace/ACE.i \
 Date_Time.i

.obj/gethrtime.o .obj/gethrtime.o .obj/gethrtime.o .obj/gethrtime.o: gethrtime.cpp \
 OS.h \
 $(ACE_ROOT)/ace/pre.h \
 $(ACE_ROOT)/ace/post.h \
 $(ACE_ROOT)/ace/ACE_export.h \
 $(ACE_ROOT)/ace/svc_export.h \
 $(ACE_ROOT)/ace/ace_wchar.h \
 $(ACE_ROOT)/ace/ace_wchar.inl \
 OS_Errno.h \
 OS_Export.h \
 OS_Errno.inl \
 OS_Dirent.h \
 OS_Dirent.inl \
 OS_String.h \
 Basic_Types.h \
 Basic_Types.i \
 OS_String.inl \
 OS_Memory.h \
 OS_Memory.inl \
 OS_TLI.h \
 OS_TLI.inl \
 $(ACE_ROOT)/ace/Default_Constants.h \
 Global_Macros.h \
 $(ACE_ROOT)/ace/Min_Max.h \
 $(ACE_ROOT)/ace/streams.h \
 OS.i

.obj/Handle_Ops.o .obj/Handle_Ops.o .obj/Handle_Ops.o .obj/Handle_Ops.o: Handle_Ops.cpp \
 Handle_Ops.h \
 $(ACE_ROOT)/ace/pre.h \
 OS.h \
 $(ACE_ROOT)/ace/post.h \
 $(ACE_ROOT)/ace/ACE_export.h \
 $(ACE_ROOT)/ace/svc_export.h \
 $(ACE_ROOT)/ace/ace_wchar.h \
 $(ACE_ROOT)/ace/ace_wchar.inl \
 OS_Errno.h \
 OS_Export.h \
 OS_Errno.inl \
 OS_Dirent.h \
 OS_Dirent.inl \
 OS_String.h \
 Basic_Types.h \
 Basic_Types.i \
 OS_String.inl \
 OS_Memory.h \
 OS_Memory.inl \
 OS_TLI.h \
 OS_TLI.inl \
 $(ACE_ROOT)/ace/Default_Constants.h \
 Global_Macros.h \
 $(ACE_ROOT)/ace/Min_Max.h \
 $(ACE_ROOT)/ace/streams.h \
 OS.i \
 Handle_Ops.i

.obj/Handle_Set.o .obj/Handle_Set.o .obj/Handle_Set.o .obj/Handle_Set.o: Handle_Set.cpp \
 Handle_Set.h \
 $(ACE_ROOT)/ace/pre.h \
 OS.h \
 $(ACE_ROOT)/ace/post.h \
 $(ACE_ROOT)/ace/ACE_export.h \
 $(ACE_ROOT)/ace/svc_export.h \
 $(ACE_ROOT)/ace/ace_wchar.h \
 $(ACE_ROOT)/ace/ace_wchar.inl \
 OS_Errno.h \
 OS_Export.h \
 OS_Errno.inl \
 OS_Dirent.h \
 OS_Dirent.inl \
 OS_String.h \
 Basic_Types.h \
 Basic_Types.i \
 OS_String.inl \
 OS_Memory.h \
 OS_Memory.inl \
 OS_TLI.h \
 OS_TLI.inl \
 $(ACE_ROOT)/ace/Default_Constants.h \
 Global_Macros.h \
 $(ACE_ROOT)/ace/Min_Max.h \
 $(ACE_ROOT)/ace/streams.h \
 OS.i \
 Handle_Set.i

.obj/OS.o .obj/OS.o .obj/OS.o .obj/OS.o: OS.cpp OS.h \
 $(ACE_ROOT)/ace/pre.h \
 $(ACE_ROOT)/ace/post.h \
 $(ACE_ROOT)/ace/ACE_export.h \
 $(ACE_ROOT)/ace/svc_export.h \
 $(ACE_ROOT)/ace/ace_wchar.h \
 $(ACE_ROOT)/ace/ace_wchar.inl \
 OS_Errno.h \
 OS_Export.h \
 OS_Errno.inl \
 OS_Dirent.h \
 OS_Dirent.inl \
 OS_String.h \
 Basic_Types.h \
 Basic_Types.i \
 OS_String.inl \
 OS_Memory.h \
 OS_Memory.inl \
 OS_TLI.h \
 OS_TLI.inl \
 $(ACE_ROOT)/ace/Default_Constants.h \
 Global_Macros.h \
 $(ACE_ROOT)/ace/Min_Max.h \
 $(ACE_ROOT)/ace/streams.h \
 OS.i \
 Sched_Params.h \
 Sched_Params.i \
 OS_Thread_Adapter.h \
 Base_Thread_Adapter.h \
 OS_Log_Msg_Attributes.h \
 OS_Log_Msg_Attributes.inl \
 Base_Thread_Adapter.inl \
 OS_Thread_Adapter.inl \
 OS_QoS.h

.obj/OS_Dirent.o .obj/OS_Dirent.o .obj/OS_Dirent.o .obj/OS_Dirent.o: OS_Dirent.cpp \
 OS.h \
 $(ACE_ROOT)/ace/pre.h \
 $(ACE_ROOT)/ace/post.h \
 $(ACE_ROOT)/ace/ACE_export.h \
 $(ACE_ROOT)/ace/svc_export.h \
 $(ACE_ROOT)/ace/ace_wchar.h \
 $(ACE_ROOT)/ace/ace_wchar.inl \
 OS_Errno.h \
 OS_Export.h \
 OS_Errno.inl \
 OS_Dirent.h \
 OS_Dirent.inl \
 OS_String.h \
 Basic_Types.h \
 Basic_Types.i \
 OS_String.inl \
 OS_Memory.h \
 OS_Memory.inl \
 OS_TLI.h \
 OS_TLI.inl \
 $(ACE_ROOT)/ace/Default_Constants.h \
 Global_Macros.h \
 $(ACE_ROOT)/ace/Min_Max.h \
 $(ACE_ROOT)/ace/streams.h \
 OS.i

.obj/OS_Errno.o .obj/OS_Errno.o .obj/OS_Errno.o .obj/OS_Errno.o: OS_Errno.cpp \
 OS_Errno.h \
 $(ACE_ROOT)/ace/pre.h \
 $(ACE_ROOT)/ace/post.h \
 OS_Export.h \
 $(ACE_ROOT)/ace/ACE_export.h \
 $(ACE_ROOT)/ace/svc_export.h \
 $(ACE_ROOT)/ace/ace_wchar.h \
 $(ACE_ROOT)/ace/ace_wchar.inl \
 OS_Errno.inl

.obj/OS_Log_Msg_Attributes.o .obj/OS_Log_Msg_Attributes.o .obj/OS_Log_Msg_Attributes.o .obj/OS_Log_Msg_Attributes.o: OS_Log_Msg_Attributes.cpp \
 OS_Log_Msg_Attributes.h \
 $(ACE_ROOT)/ace/pre.h \
 $(ACE_ROOT)/ace/post.h \
 $(ACE_ROOT)/ace/ACE_export.h \
 $(ACE_ROOT)/ace/svc_export.h \
 $(ACE_ROOT)/ace/ace_wchar.h \
 $(ACE_ROOT)/ace/ace_wchar.inl \
 OS_Errno.h \
 OS_Export.h \
 OS_Errno.inl \
 $(ACE_ROOT)/ace/streams.h \
 OS_Log_Msg_Attributes.inl

.obj/OS_Memory.o .obj/OS_Memory.o .obj/OS_Memory.o .obj/OS_Memory.o: OS_Memory.cpp \
 OS_Memory.h \
 $(ACE_ROOT)/ace/pre.h \
 $(ACE_ROOT)/ace/post.h \
 $(ACE_ROOT)/ace/ACE_export.h \
 $(ACE_ROOT)/ace/svc_export.h \
 $(ACE_ROOT)/ace/ace_wchar.h \
 $(ACE_ROOT)/ace/ace_wchar.inl \
 OS_Errno.h \
 OS_Export.h \
 OS_Errno.inl \
 OS_Memory.inl

.obj/OS_QoS.o .obj/OS_QoS.o .obj/OS_QoS.o .obj/OS_QoS.o: OS_QoS.cpp \
 OS_QoS.h \
 $(ACE_ROOT)/ace/pre.h \
 OS.h \
 $(ACE_ROOT)/ace/post.h \
 $(ACE_ROOT)/ace/ACE_export.h \
 $(ACE_ROOT)/ace/svc_export.h \
 $(ACE_ROOT)/ace/ace_wchar.h \
 $(ACE_ROOT)/ace/ace_wchar.inl \
 OS_Errno.h \
 OS_Export.h \
 OS_Errno.inl \
 OS_Dirent.h \
 OS_Dirent.inl \
 OS_String.h \
 Basic_Types.h \
 Basic_Types.i \
 OS_String.inl \
 OS_Memory.h \
 OS_Memory.inl \
 OS_TLI.h \
 OS_TLI.inl \
 $(ACE_ROOT)/ace/Default_Constants.h \
 Global_Macros.h \
 $(ACE_ROOT)/ace/Min_Max.h \
 $(ACE_ROOT)/ace/streams.h \
 OS.i

.obj/OS_String.o .obj/OS_String.o .obj/OS_String.o .obj/OS_String.o: OS_String.cpp \
 OS_String.h \
 $(ACE_ROOT)/ace/pre.h \
 $(ACE_ROOT)/ace/post.h \
 $(ACE_ROOT)/ace/ACE_export.h \
 $(ACE_ROOT)/ace/svc_export.h \
 $(ACE_ROOT)/ace/ace_wchar.h \
 $(ACE_ROOT)/ace/ace_wchar.inl \
 OS_Errno.h \
 OS_Export.h \
 OS_Errno.inl \
 Basic_Types.h \
 Basic_Types.i \
 OS_String.inl \
 OS_Memory.h \
 OS_Memory.inl

.obj/OS_Thread_Adapter.o .obj/OS_Thread_Adapter.o .obj/OS_Thread_Adapter.o .obj/OS_Thread_Adapter.o: OS_Thread_Adapter.cpp \
 OS_Thread_Adapter.h \
 $(ACE_ROOT)/ace/pre.h \
 Base_Thread_Adapter.h \
 OS_Log_Msg_Attributes.h \
 $(ACE_ROOT)/ace/post.h \
 $(ACE_ROOT)/ace/ACE_export.h \
 $(ACE_ROOT)/ace/svc_export.h \
 $(ACE_ROOT)/ace/ace_wchar.h \
 $(ACE_ROOT)/ace/ace_wchar.inl \
 OS_Errno.h \
 OS_Export.h \
 OS_Errno.inl \
 $(ACE_ROOT)/ace/streams.h \
 OS_Log_Msg_Attributes.inl \
 Base_Thread_Adapter.inl \
 OS_Thread_Adapter.inl \
 Thread_Hook.h \
 OS.h \
 OS_Dirent.h \
 OS_Dirent.inl \
 OS_String.h \
 Basic_Types.h \
 Basic_Types.i \
 OS_String.inl \
 OS_Memory.h \
 OS_Memory.inl \
 OS_TLI.h \
 OS_TLI.inl \
 $(ACE_ROOT)/ace/Default_Constants.h \
 Global_Macros.h \
 $(ACE_ROOT)/ace/Min_Max.h \
 OS.i

.obj/OS_TLI.o .obj/OS_TLI.o .obj/OS_TLI.o .obj/OS_TLI.o: OS_TLI.cpp \
 OS_TLI.h \
 $(ACE_ROOT)/ace/pre.h \
 $(ACE_ROOT)/ace/post.h \
 $(ACE_ROOT)/ace/ACE_export.h \
 $(ACE_ROOT)/ace/svc_export.h \
 $(ACE_ROOT)/ace/ace_wchar.h \
 $(ACE_ROOT)/ace/ace_wchar.inl \
 OS_Errno.h \
 OS_Export.h \
 OS_Errno.inl \
 OS_TLI.inl

.obj/Sched_Params.o .obj/Sched_Params.o .obj/Sched_Params.o .obj/Sched_Params.o: Sched_Params.cpp \
 Sched_Params.h \
 $(ACE_ROOT)/ace/pre.h \
 OS.h \
 $(ACE_ROOT)/ace/post.h \
 $(ACE_ROOT)/ace/ACE_export.h \
 $(ACE_ROOT)/ace/svc_export.h \
 $(ACE_ROOT)/ace/ace_wchar.h \
 $(ACE_ROOT)/ace/ace_wchar.inl \
 OS_Errno.h \
 OS_Export.h \
 OS_Errno.inl \
 OS_Dirent.h \
 OS_Dirent.inl \
 OS_String.h \
 Basic_Types.h \
 Basic_Types.i \
 OS_String.inl \
 OS_Memory.h \
 OS_Memory.inl \
 OS_TLI.h \
 OS_TLI.inl \
 $(ACE_ROOT)/ace/Default_Constants.h \
 Global_Macros.h \
 $(ACE_ROOT)/ace/Min_Max.h \
 $(ACE_ROOT)/ace/streams.h \
 OS.i \
 Sched_Params.i

.obj/Thread_Hook.o .obj/Thread_Hook.o .obj/Thread_Hook.o .obj/Thread_Hook.o: Thread_Hook.cpp \
 Thread_Hook.h \
 $(ACE_ROOT)/ace/pre.h \
 $(ACE_ROOT)/ace/post.h \
 $(ACE_ROOT)/ace/ACE_export.h \
 $(ACE_ROOT)/ace/svc_export.h \
 $(ACE_ROOT)/ace/ace_wchar.h \
 $(ACE_ROOT)/ace/ace_wchar.inl \
 OS_Errno.h \
 OS_Export.h \
 OS_Errno.inl \
 OS.h \
 OS_Dirent.h \
 OS_Dirent.inl \
 OS_String.h \
 Basic_Types.h \
 Basic_Types.i \
 OS_String.inl \
 OS_Memory.h \
 OS_Memory.inl \
 OS_TLI.h \
 OS_TLI.inl \
 $(ACE_ROOT)/ace/Default_Constants.h \
 Global_Macros.h \
 $(ACE_ROOT)/ace/Min_Max.h \
 $(ACE_ROOT)/ace/streams.h \
 OS.i

# IF YOU PUT ANYTHING HERE IT WILL GO AWAY
