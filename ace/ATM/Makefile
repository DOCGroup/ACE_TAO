# $Id$

#----------------------------------------------------------------------------
#       Makefile for the libACE_ATM
#----------------------------------------------------------------------------

MAKEFILE = Makefile
LIBNAME   = libACE_ATM
LIB      = $(LIBNAME).a
SHLIB    = $(LIBNAME).$(SOEXT)


LIB_FILES = \
	ATM_Acceptor\
	ATM_Addr\
	ATM_Connector\
	ATM_Params\
	ATM_QoS\
	ATM_Stream

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


.obj/ATM_Acceptor.o .obj/ATM_Acceptor.o .obj/ATM_Acceptor.o .obj/ATM_Acceptor.o: ATM_Acceptor.cpp ATM_Acceptor.h \
 $(ACE_ROOT)/ace/pre.h \
 $(ACE_ROOT)/ace/Timer/Time_Value.h \
 $(ACE_ROOT)/ace/OS/OS.h \
 $(ACE_ROOT)/ace/post.h \
 $(ACE_ROOT)/ace/ACE_export.h \
 $(ACE_ROOT)/ace/svc_export.h \
 $(ACE_ROOT)/ace/ace_wchar.h \
 $(ACE_ROOT)/ace/ace_wchar.inl \
 $(ACE_ROOT)/ace/OS/OS_Errno.h \
 $(ACE_ROOT)/ace/OS/OS_Export.h \
 $(ACE_ROOT)/ace/OS/OS_Errno.inl \
 $(ACE_ROOT)/ace/OS/OS_Dirent.h \
 $(ACE_ROOT)/ace/OS/OS_Dirent.inl \
 $(ACE_ROOT)/ace/OS/OS_String.h \
 $(ACE_ROOT)/ace/OS/Basic_Types.h \
 $(ACE_ROOT)/ace/OS/Basic_Types.i \
 $(ACE_ROOT)/ace/OS/OS_String.inl \
 $(ACE_ROOT)/ace/OS/OS_Memory.h \
 $(ACE_ROOT)/ace/OS/OS_Memory.inl \
 $(ACE_ROOT)/ace/OS/OS_TLI.h \
 $(ACE_ROOT)/ace/OS/OS_TLI.inl \
 $(ACE_ROOT)/ace/Default_Constants.h \
 $(ACE_ROOT)/ace/OS/Global_Macros.h \
 $(ACE_ROOT)/ace/Min_Max.h \
 $(ACE_ROOT)/ace/streams.h \
 $(ACE_ROOT)/ace/OS/OS.i \
 ATM_Stream.h \
 ATM_Addr.h \
 $(ACE_ROOT)/ace/ACE.h \
 $(ACE_ROOT)/ace/Utils/Flag_Manip.h \
 $(ACE_ROOT)/ace/Utils/Flag_Manip.i \
 $(ACE_ROOT)/ace/OS/Handle_Ops.h \
 $(ACE_ROOT)/ace/OS/Handle_Ops.i \
 $(ACE_ROOT)/ace/Utils/Lib_Find.h \
 $(ACE_ROOT)/ace/Utils/Lib_Find.i \
 $(ACE_ROOT)/ace/Utils/Init_ACE.h \
 $(ACE_ROOT)/ace/Utils/Init_ACE.i \
 $(ACE_ROOT)/ace/Connection/Sock_Connect.h \
 $(ACE_ROOT)/ace/Connection/Sock_Connect.i \
 $(ACE_ROOT)/ace/ACE.i \
 $(ACE_ROOT)/ace/IPC/Addr.h \
 $(ACE_ROOT)/ace/IPC/Addr.i \
 ATM_Addr.i \
 ATM_Params.h \
 ATM_QoS.h

.obj/ATM_Addr.o .obj/ATM_Addr.o .obj/ATM_Addr.o .obj/ATM_Addr.o: ATM_Addr.cpp \
 ATM_Addr.h \
 $(ACE_ROOT)/ace/pre.h \
 $(ACE_ROOT)/ace/ACE.h \
 $(ACE_ROOT)/ace/OS/OS.h \
 $(ACE_ROOT)/ace/post.h \
 $(ACE_ROOT)/ace/ACE_export.h \
 $(ACE_ROOT)/ace/svc_export.h \
 $(ACE_ROOT)/ace/ace_wchar.h \
 $(ACE_ROOT)/ace/ace_wchar.inl \
 $(ACE_ROOT)/ace/OS/OS_Errno.h \
 $(ACE_ROOT)/ace/OS/OS_Export.h \
 $(ACE_ROOT)/ace/OS/OS_Errno.inl \
 $(ACE_ROOT)/ace/OS/OS_Dirent.h \
 $(ACE_ROOT)/ace/OS/OS_Dirent.inl \
 $(ACE_ROOT)/ace/OS/OS_String.h \
 $(ACE_ROOT)/ace/OS/Basic_Types.h \
 $(ACE_ROOT)/ace/OS/Basic_Types.i \
 $(ACE_ROOT)/ace/OS/OS_String.inl \
 $(ACE_ROOT)/ace/OS/OS_Memory.h \
 $(ACE_ROOT)/ace/OS/OS_Memory.inl \
 $(ACE_ROOT)/ace/OS/OS_TLI.h \
 $(ACE_ROOT)/ace/OS/OS_TLI.inl \
 $(ACE_ROOT)/ace/Default_Constants.h \
 $(ACE_ROOT)/ace/OS/Global_Macros.h \
 $(ACE_ROOT)/ace/Min_Max.h \
 $(ACE_ROOT)/ace/streams.h \
 $(ACE_ROOT)/ace/OS/OS.i \
 $(ACE_ROOT)/ace/Utils/Flag_Manip.h \
 $(ACE_ROOT)/ace/Utils/Flag_Manip.i \
 $(ACE_ROOT)/ace/OS/Handle_Ops.h \
 $(ACE_ROOT)/ace/OS/Handle_Ops.i \
 $(ACE_ROOT)/ace/Utils/Lib_Find.h \
 $(ACE_ROOT)/ace/Utils/Lib_Find.i \
 $(ACE_ROOT)/ace/Utils/Init_ACE.h \
 $(ACE_ROOT)/ace/Utils/Init_ACE.i \
 $(ACE_ROOT)/ace/Connection/Sock_Connect.h \
 $(ACE_ROOT)/ace/Connection/Sock_Connect.i \
 $(ACE_ROOT)/ace/ACE.i \
 $(ACE_ROOT)/ace/IPC/Addr.h \
 $(ACE_ROOT)/ace/IPC/Addr.i \
 ATM_Addr.i

.obj/ATM_Connector.o .obj/ATM_Connector.o .obj/ATM_Connector.o .obj/ATM_Connector.o: ATM_Connector.cpp \
 $(ACE_ROOT)/ace/OS/Handle_Set.h \
 $(ACE_ROOT)/ace/pre.h \
 $(ACE_ROOT)/ace/OS/OS.h \
 $(ACE_ROOT)/ace/post.h \
 $(ACE_ROOT)/ace/ACE_export.h \
 $(ACE_ROOT)/ace/svc_export.h \
 $(ACE_ROOT)/ace/ace_wchar.h \
 $(ACE_ROOT)/ace/ace_wchar.inl \
 $(ACE_ROOT)/ace/OS/OS_Errno.h \
 $(ACE_ROOT)/ace/OS/OS_Export.h \
 $(ACE_ROOT)/ace/OS/OS_Errno.inl \
 $(ACE_ROOT)/ace/OS/OS_Dirent.h \
 $(ACE_ROOT)/ace/OS/OS_Dirent.inl \
 $(ACE_ROOT)/ace/OS/OS_String.h \
 $(ACE_ROOT)/ace/OS/Basic_Types.h \
 $(ACE_ROOT)/ace/OS/Basic_Types.i \
 $(ACE_ROOT)/ace/OS/OS_String.inl \
 $(ACE_ROOT)/ace/OS/OS_Memory.h \
 $(ACE_ROOT)/ace/OS/OS_Memory.inl \
 $(ACE_ROOT)/ace/OS/OS_TLI.h \
 $(ACE_ROOT)/ace/OS/OS_TLI.inl \
 $(ACE_ROOT)/ace/Default_Constants.h \
 $(ACE_ROOT)/ace/OS/Global_Macros.h \
 $(ACE_ROOT)/ace/Min_Max.h \
 $(ACE_ROOT)/ace/streams.h \
 $(ACE_ROOT)/ace/OS/OS.i \
 $(ACE_ROOT)/ace/OS/Handle_Set.i \
 ATM_Connector.h \
 ATM_Stream.h \
 ATM_Addr.h \
 $(ACE_ROOT)/ace/ACE.h \
 $(ACE_ROOT)/ace/Utils/Flag_Manip.h \
 $(ACE_ROOT)/ace/Utils/Flag_Manip.i \
 $(ACE_ROOT)/ace/OS/Handle_Ops.h \
 $(ACE_ROOT)/ace/OS/Handle_Ops.i \
 $(ACE_ROOT)/ace/Utils/Lib_Find.h \
 $(ACE_ROOT)/ace/Utils/Lib_Find.i \
 $(ACE_ROOT)/ace/Utils/Init_ACE.h \
 $(ACE_ROOT)/ace/Utils/Init_ACE.i \
 $(ACE_ROOT)/ace/Connection/Sock_Connect.h \
 $(ACE_ROOT)/ace/Connection/Sock_Connect.i \
 $(ACE_ROOT)/ace/ACE.i \
 $(ACE_ROOT)/ace/IPC/Addr.h \
 $(ACE_ROOT)/ace/IPC/Addr.i \
 ATM_Addr.i \
 ATM_Params.h \
 ATM_QoS.h

.obj/ATM_Params.o .obj/ATM_Params.o .obj/ATM_Params.o .obj/ATM_Params.o: ATM_Params.cpp \
 ATM_Params.h \
 $(ACE_ROOT)/ace/pre.h \
 $(ACE_ROOT)/ace/ACE.h \
 $(ACE_ROOT)/ace/OS/OS.h \
 $(ACE_ROOT)/ace/post.h \
 $(ACE_ROOT)/ace/ACE_export.h \
 $(ACE_ROOT)/ace/svc_export.h \
 $(ACE_ROOT)/ace/ace_wchar.h \
 $(ACE_ROOT)/ace/ace_wchar.inl \
 $(ACE_ROOT)/ace/OS/OS_Errno.h \
 $(ACE_ROOT)/ace/OS/OS_Export.h \
 $(ACE_ROOT)/ace/OS/OS_Errno.inl \
 $(ACE_ROOT)/ace/OS/OS_Dirent.h \
 $(ACE_ROOT)/ace/OS/OS_Dirent.inl \
 $(ACE_ROOT)/ace/OS/OS_String.h \
 $(ACE_ROOT)/ace/OS/Basic_Types.h \
 $(ACE_ROOT)/ace/OS/Basic_Types.i \
 $(ACE_ROOT)/ace/OS/OS_String.inl \
 $(ACE_ROOT)/ace/OS/OS_Memory.h \
 $(ACE_ROOT)/ace/OS/OS_Memory.inl \
 $(ACE_ROOT)/ace/OS/OS_TLI.h \
 $(ACE_ROOT)/ace/OS/OS_TLI.inl \
 $(ACE_ROOT)/ace/Default_Constants.h \
 $(ACE_ROOT)/ace/OS/Global_Macros.h \
 $(ACE_ROOT)/ace/Min_Max.h \
 $(ACE_ROOT)/ace/streams.h \
 $(ACE_ROOT)/ace/OS/OS.i \
 $(ACE_ROOT)/ace/Utils/Flag_Manip.h \
 $(ACE_ROOT)/ace/Utils/Flag_Manip.i \
 $(ACE_ROOT)/ace/OS/Handle_Ops.h \
 $(ACE_ROOT)/ace/OS/Handle_Ops.i \
 $(ACE_ROOT)/ace/Utils/Lib_Find.h \
 $(ACE_ROOT)/ace/Utils/Lib_Find.i \
 $(ACE_ROOT)/ace/Utils/Init_ACE.h \
 $(ACE_ROOT)/ace/Utils/Init_ACE.i \
 $(ACE_ROOT)/ace/Connection/Sock_Connect.h \
 $(ACE_ROOT)/ace/Connection/Sock_Connect.i \
 $(ACE_ROOT)/ace/ACE.i

.obj/ATM_QoS.o .obj/ATM_QoS.o .obj/ATM_QoS.o .obj/ATM_QoS.o: ATM_QoS.cpp \
 ATM_QoS.h \
 $(ACE_ROOT)/ace/pre.h \
 $(ACE_ROOT)/ace/ACE.h \
 $(ACE_ROOT)/ace/OS/OS.h \
 $(ACE_ROOT)/ace/post.h \
 $(ACE_ROOT)/ace/ACE_export.h \
 $(ACE_ROOT)/ace/svc_export.h \
 $(ACE_ROOT)/ace/ace_wchar.h \
 $(ACE_ROOT)/ace/ace_wchar.inl \
 $(ACE_ROOT)/ace/OS/OS_Errno.h \
 $(ACE_ROOT)/ace/OS/OS_Export.h \
 $(ACE_ROOT)/ace/OS/OS_Errno.inl \
 $(ACE_ROOT)/ace/OS/OS_Dirent.h \
 $(ACE_ROOT)/ace/OS/OS_Dirent.inl \
 $(ACE_ROOT)/ace/OS/OS_String.h \
 $(ACE_ROOT)/ace/OS/Basic_Types.h \
 $(ACE_ROOT)/ace/OS/Basic_Types.i \
 $(ACE_ROOT)/ace/OS/OS_String.inl \
 $(ACE_ROOT)/ace/OS/OS_Memory.h \
 $(ACE_ROOT)/ace/OS/OS_Memory.inl \
 $(ACE_ROOT)/ace/OS/OS_TLI.h \
 $(ACE_ROOT)/ace/OS/OS_TLI.inl \
 $(ACE_ROOT)/ace/Default_Constants.h \
 $(ACE_ROOT)/ace/OS/Global_Macros.h \
 $(ACE_ROOT)/ace/Min_Max.h \
 $(ACE_ROOT)/ace/streams.h \
 $(ACE_ROOT)/ace/OS/OS.i \
 $(ACE_ROOT)/ace/Utils/Flag_Manip.h \
 $(ACE_ROOT)/ace/Utils/Flag_Manip.i \
 $(ACE_ROOT)/ace/OS/Handle_Ops.h \
 $(ACE_ROOT)/ace/OS/Handle_Ops.i \
 $(ACE_ROOT)/ace/Utils/Lib_Find.h \
 $(ACE_ROOT)/ace/Utils/Lib_Find.i \
 $(ACE_ROOT)/ace/Utils/Init_ACE.h \
 $(ACE_ROOT)/ace/Utils/Init_ACE.i \
 $(ACE_ROOT)/ace/Connection/Sock_Connect.h \
 $(ACE_ROOT)/ace/Connection/Sock_Connect.i \
 $(ACE_ROOT)/ace/ACE.i

.obj/ATM_Stream.o .obj/ATM_Stream.o .obj/ATM_Stream.o .obj/ATM_Stream.o: ATM_Stream.cpp \
 ATM_Stream.h \
 $(ACE_ROOT)/ace/pre.h \
 ATM_Addr.h \
 $(ACE_ROOT)/ace/ACE.h \
 $(ACE_ROOT)/ace/OS/OS.h \
 $(ACE_ROOT)/ace/post.h \
 $(ACE_ROOT)/ace/ACE_export.h \
 $(ACE_ROOT)/ace/svc_export.h \
 $(ACE_ROOT)/ace/ace_wchar.h \
 $(ACE_ROOT)/ace/ace_wchar.inl \
 $(ACE_ROOT)/ace/OS/OS_Errno.h \
 $(ACE_ROOT)/ace/OS/OS_Export.h \
 $(ACE_ROOT)/ace/OS/OS_Errno.inl \
 $(ACE_ROOT)/ace/OS/OS_Dirent.h \
 $(ACE_ROOT)/ace/OS/OS_Dirent.inl \
 $(ACE_ROOT)/ace/OS/OS_String.h \
 $(ACE_ROOT)/ace/OS/Basic_Types.h \
 $(ACE_ROOT)/ace/OS/Basic_Types.i \
 $(ACE_ROOT)/ace/OS/OS_String.inl \
 $(ACE_ROOT)/ace/OS/OS_Memory.h \
 $(ACE_ROOT)/ace/OS/OS_Memory.inl \
 $(ACE_ROOT)/ace/OS/OS_TLI.h \
 $(ACE_ROOT)/ace/OS/OS_TLI.inl \
 $(ACE_ROOT)/ace/Default_Constants.h \
 $(ACE_ROOT)/ace/OS/Global_Macros.h \
 $(ACE_ROOT)/ace/Min_Max.h \
 $(ACE_ROOT)/ace/streams.h \
 $(ACE_ROOT)/ace/OS/OS.i \
 $(ACE_ROOT)/ace/Utils/Flag_Manip.h \
 $(ACE_ROOT)/ace/Utils/Flag_Manip.i \
 $(ACE_ROOT)/ace/OS/Handle_Ops.h \
 $(ACE_ROOT)/ace/OS/Handle_Ops.i \
 $(ACE_ROOT)/ace/Utils/Lib_Find.h \
 $(ACE_ROOT)/ace/Utils/Lib_Find.i \
 $(ACE_ROOT)/ace/Utils/Init_ACE.h \
 $(ACE_ROOT)/ace/Utils/Init_ACE.i \
 $(ACE_ROOT)/ace/Connection/Sock_Connect.h \
 $(ACE_ROOT)/ace/Connection/Sock_Connect.i \
 $(ACE_ROOT)/ace/ACE.i \
 $(ACE_ROOT)/ace/IPC/Addr.h \
 $(ACE_ROOT)/ace/IPC/Addr.i \
 ATM_Addr.i \
 ATM_Params.h

# IF YOU PUT ANYTHING HERE IT WILL GO AWAY
