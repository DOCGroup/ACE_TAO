# $Id$

#----------------------------------------------------------------------------
#       Makefile for the libACE_Sockets
#----------------------------------------------------------------------------

MAKEFILE = Makefile
LIBOS   = libACE_Sockets
LIB      = $(LIBOS).a
SHLIB    = $(LIBOS).$(SOEXT)


OS_FILES = \
	IPC_SAP\
	LSOCK_Acceptor\
	LSOCK_CODgram\
	LSOCK_Connector\
	LSOCK\
	LSOCK_Dgram\
	LSOCK_Stream\
	SOCK_Acceptor\
	SOCK_CODgram\
	SOCK_Connector\
	SOCK\
	SOCK_Dgram_Bcast\
	SOCK_Dgram\
	SOCK_Dgram_Mcast\
	SOCK_IO\
	SOCK_Stream

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


.obj/IPC_SAP.o .obj/IPC_SAP.o .obj/IPC_SAP.o .obj/IPC_SAP.o: IPC_SAP.cpp \
 IPC_SAP.h \
 $(ACE_ROOT)/ace/pre.h \
 $(ACE_ROOT)/ace/Utils/Flag_Manip.h \
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
 $(ACE_ROOT)/ace/Utils/Flag_Manip.i \
 IPC_SAP.i \
 $(ACE_ROOT)/ace/Logging/Log_Msg.h \
 $(ACE_ROOT)/ace/Logging/Log_Record.h \
 $(ACE_ROOT)/ace/Logging/Log_Priority.h \
 $(ACE_ROOT)/ace/Logging/Log_Record.i \
 $(ACE_ROOT)/ace/OS/OS_Log_Msg_Attributes.h \
 $(ACE_ROOT)/ace/OS/OS_Log_Msg_Attributes.inl

.obj/LSOCK_Acceptor.o .obj/LSOCK_Acceptor.o .obj/LSOCK_Acceptor.o .obj/LSOCK_Acceptor.o: LSOCK_Acceptor.cpp \
 LSOCK_Acceptor.h \
 $(ACE_ROOT)/ace/pre.h \
 SOCK_Acceptor.h \
 SOCK_Stream.h \
 SOCK_IO.h \
 SOCK.h \
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
 IPC_SAP.h \
 IPC_SAP.i \
 SOCK.i \
 SOCK_IO.i \
 $(ACE_ROOT)/ace/IPC/INET_Addr.h \
 $(ACE_ROOT)/ace/IPC/INET_Addr.i \
 SOCK_Stream.i \
 $(ACE_ROOT)/ace/Timer/Time_Value.h \
 SOCK_Acceptor.i \
 $(ACE_ROOT)/ace/IPC/UNIX_Addr.h \
 $(ACE_ROOT)/ace/IPC/UNIX_Addr.i \
 LSOCK_Stream.h \
 LSOCK.h \
 LSOCK.i \
 LSOCK_Stream.i \
 $(ACE_ROOT)/ace/Logging/Log_Msg.h \
 $(ACE_ROOT)/ace/Logging/Log_Record.h \
 $(ACE_ROOT)/ace/Logging/Log_Priority.h \
 $(ACE_ROOT)/ace/Logging/Log_Record.i \
 $(ACE_ROOT)/ace/OS/OS_Log_Msg_Attributes.h \
 $(ACE_ROOT)/ace/OS/OS_Log_Msg_Attributes.inl

.obj/LSOCK_CODgram.o .obj/LSOCK_CODgram.o .obj/LSOCK_CODgram.o .obj/LSOCK_CODgram.o: LSOCK_CODgram.cpp \
 LSOCK_CODgram.h \
 $(ACE_ROOT)/ace/pre.h \
 LSOCK.h \
 SOCK.h \
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
 IPC_SAP.h \
 IPC_SAP.i \
 SOCK.i \
 LSOCK.i \
 SOCK_CODgram.h \
 SOCK_IO.h \
 SOCK_IO.i \
 $(ACE_ROOT)/ace/IPC/INET_Addr.h \
 $(ACE_ROOT)/ace/IPC/INET_Addr.i \
 SOCK_CODgram.i \
 LSOCK_CODgram.i \
 $(ACE_ROOT)/ace/Logging/Log_Msg.h \
 $(ACE_ROOT)/ace/Logging/Log_Record.h \
 $(ACE_ROOT)/ace/Logging/Log_Priority.h \
 $(ACE_ROOT)/ace/Logging/Log_Record.i \
 $(ACE_ROOT)/ace/OS/OS_Log_Msg_Attributes.h \
 $(ACE_ROOT)/ace/OS/OS_Log_Msg_Attributes.inl

.obj/LSOCK_Connector.o .obj/LSOCK_Connector.o .obj/LSOCK_Connector.o .obj/LSOCK_Connector.o: LSOCK_Connector.cpp \
 LSOCK_Connector.h \
 $(ACE_ROOT)/ace/pre.h \
 SOCK_Connector.h \
 SOCK_Stream.h \
 SOCK_IO.h \
 SOCK.h \
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
 IPC_SAP.h \
 IPC_SAP.i \
 SOCK.i \
 SOCK_IO.i \
 $(ACE_ROOT)/ace/IPC/INET_Addr.h \
 $(ACE_ROOT)/ace/IPC/INET_Addr.i \
 SOCK_Stream.i \
 $(ACE_ROOT)/ace/Timer/Time_Value.h \
 SOCK_Connector.i \
 LSOCK_Stream.h \
 $(ACE_ROOT)/ace/IPC/UNIX_Addr.h \
 $(ACE_ROOT)/ace/IPC/UNIX_Addr.i \
 LSOCK.h \
 LSOCK.i \
 LSOCK_Stream.i \
 LSOCK_Connector.i \
 $(ACE_ROOT)/ace/Logging/Log_Msg.h \
 $(ACE_ROOT)/ace/Logging/Log_Record.h \
 $(ACE_ROOT)/ace/Logging/Log_Priority.h \
 $(ACE_ROOT)/ace/Logging/Log_Record.i \
 $(ACE_ROOT)/ace/OS/OS_Log_Msg_Attributes.h \
 $(ACE_ROOT)/ace/OS/OS_Log_Msg_Attributes.inl

.obj/LSOCK.o .obj/LSOCK.o .obj/LSOCK.o .obj/LSOCK.o: LSOCK.cpp \
 LSOCK.h \
 $(ACE_ROOT)/ace/pre.h \
 SOCK.h \
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
 IPC_SAP.h \
 IPC_SAP.i \
 SOCK.i \
 LSOCK.i \
 $(ACE_ROOT)/ace/Logging/Log_Msg.h \
 $(ACE_ROOT)/ace/Logging/Log_Record.h \
 $(ACE_ROOT)/ace/Logging/Log_Priority.h \
 $(ACE_ROOT)/ace/Logging/Log_Record.i \
 $(ACE_ROOT)/ace/OS/OS_Log_Msg_Attributes.h \
 $(ACE_ROOT)/ace/OS/OS_Log_Msg_Attributes.inl

.obj/LSOCK_Dgram.o .obj/LSOCK_Dgram.o .obj/LSOCK_Dgram.o .obj/LSOCK_Dgram.o: LSOCK_Dgram.cpp \
 LSOCK_Dgram.h \
 $(ACE_ROOT)/ace/pre.h \
 SOCK_Dgram.h \
 SOCK.h \
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
 IPC_SAP.h \
 IPC_SAP.i \
 SOCK.i \
 $(ACE_ROOT)/ace/IPC/INET_Addr.h \
 $(ACE_ROOT)/ace/IPC/INET_Addr.i \
 SOCK_Dgram.i \
 LSOCK.h \
 LSOCK.i \
 LSOCK_Dgram.i \
 $(ACE_ROOT)/ace/Logging/Log_Msg.h \
 $(ACE_ROOT)/ace/Logging/Log_Record.h \
 $(ACE_ROOT)/ace/Logging/Log_Priority.h \
 $(ACE_ROOT)/ace/Logging/Log_Record.i \
 $(ACE_ROOT)/ace/OS/OS_Log_Msg_Attributes.h \
 $(ACE_ROOT)/ace/OS/OS_Log_Msg_Attributes.inl

.obj/LSOCK_Stream.o .obj/LSOCK_Stream.o .obj/LSOCK_Stream.o .obj/LSOCK_Stream.o: LSOCK_Stream.cpp \
 LSOCK_Stream.h \
 $(ACE_ROOT)/ace/pre.h \
 SOCK_Stream.h \
 SOCK_IO.h \
 SOCK.h \
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
 IPC_SAP.h \
 IPC_SAP.i \
 SOCK.i \
 SOCK_IO.i \
 $(ACE_ROOT)/ace/IPC/INET_Addr.h \
 $(ACE_ROOT)/ace/IPC/INET_Addr.i \
 SOCK_Stream.i \
 $(ACE_ROOT)/ace/IPC/UNIX_Addr.h \
 $(ACE_ROOT)/ace/IPC/UNIX_Addr.i \
 LSOCK.h \
 LSOCK.i \
 LSOCK_Stream.i \
 $(ACE_ROOT)/ace/Logging/Log_Msg.h \
 $(ACE_ROOT)/ace/Logging/Log_Record.h \
 $(ACE_ROOT)/ace/Logging/Log_Priority.h \
 $(ACE_ROOT)/ace/Logging/Log_Record.i \
 $(ACE_ROOT)/ace/OS/OS_Log_Msg_Attributes.h \
 $(ACE_ROOT)/ace/OS/OS_Log_Msg_Attributes.inl

.obj/SOCK_Acceptor.o .obj/SOCK_Acceptor.o .obj/SOCK_Acceptor.o .obj/SOCK_Acceptor.o: SOCK_Acceptor.cpp \
 SOCK_Acceptor.h \
 $(ACE_ROOT)/ace/pre.h \
 SOCK_Stream.h \
 SOCK_IO.h \
 SOCK.h \
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
 IPC_SAP.h \
 IPC_SAP.i \
 SOCK.i \
 SOCK_IO.i \
 $(ACE_ROOT)/ace/IPC/INET_Addr.h \
 $(ACE_ROOT)/ace/IPC/INET_Addr.i \
 SOCK_Stream.i \
 $(ACE_ROOT)/ace/Timer/Time_Value.h \
 SOCK_Acceptor.i \
 $(ACE_ROOT)/ace/Logging/Log_Msg.h \
 $(ACE_ROOT)/ace/Logging/Log_Record.h \
 $(ACE_ROOT)/ace/Logging/Log_Priority.h \
 $(ACE_ROOT)/ace/Logging/Log_Record.i \
 $(ACE_ROOT)/ace/OS/OS_Log_Msg_Attributes.h \
 $(ACE_ROOT)/ace/OS/OS_Log_Msg_Attributes.inl \
 $(ACE_ROOT)/ace/OS/OS_QoS.h \
 $(ACE_ROOT)/ace/Threads/Synch.h \
 $(ACE_ROOT)/ace/Threads/Synch.i \
 $(ACE_ROOT)/ace/Threads/Synch_T.h \
 $(ACE_ROOT)/ace/Threads/Synch_T.i \
 $(ACE_ROOT)/ace/Threads/Thread.h \
 $(ACE_ROOT)/ace/Threads/Thread_Adapter.h \
 $(ACE_ROOT)/ace/OS/Base_Thread_Adapter.h \
 $(ACE_ROOT)/ace/OS/Base_Thread_Adapter.inl \
 $(ACE_ROOT)/ace/Threads/Thread_Adapter.inl \
 $(ACE_ROOT)/ace/Threads/Thread.i \
 $(ACE_ROOT)/ace/Threads/Synch_T.cpp

.obj/SOCK_CODgram.o .obj/SOCK_CODgram.o .obj/SOCK_CODgram.o .obj/SOCK_CODgram.o: SOCK_CODgram.cpp \
 SOCK_CODgram.h \
 $(ACE_ROOT)/ace/pre.h \
 SOCK_IO.h \
 SOCK.h \
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
 IPC_SAP.h \
 IPC_SAP.i \
 SOCK.i \
 SOCK_IO.i \
 $(ACE_ROOT)/ace/IPC/INET_Addr.h \
 $(ACE_ROOT)/ace/IPC/INET_Addr.i \
 SOCK_CODgram.i \
 $(ACE_ROOT)/ace/Logging/Log_Msg.h \
 $(ACE_ROOT)/ace/Logging/Log_Record.h \
 $(ACE_ROOT)/ace/Logging/Log_Priority.h \
 $(ACE_ROOT)/ace/Logging/Log_Record.i \
 $(ACE_ROOT)/ace/OS/OS_Log_Msg_Attributes.h \
 $(ACE_ROOT)/ace/OS/OS_Log_Msg_Attributes.inl

.obj/SOCK_Connector.o .obj/SOCK_Connector.o .obj/SOCK_Connector.o .obj/SOCK_Connector.o: SOCK_Connector.cpp \
 SOCK_Connector.h \
 $(ACE_ROOT)/ace/pre.h \
 SOCK_Stream.h \
 SOCK_IO.h \
 SOCK.h \
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
 IPC_SAP.h \
 IPC_SAP.i \
 SOCK.i \
 SOCK_IO.i \
 $(ACE_ROOT)/ace/IPC/INET_Addr.h \
 $(ACE_ROOT)/ace/IPC/INET_Addr.i \
 SOCK_Stream.i \
 $(ACE_ROOT)/ace/Timer/Time_Value.h \
 SOCK_Connector.i \
 $(ACE_ROOT)/ace/Logging/Log_Msg.h \
 $(ACE_ROOT)/ace/Logging/Log_Record.h \
 $(ACE_ROOT)/ace/Logging/Log_Priority.h \
 $(ACE_ROOT)/ace/Logging/Log_Record.i \
 $(ACE_ROOT)/ace/OS/OS_Log_Msg_Attributes.h \
 $(ACE_ROOT)/ace/OS/OS_Log_Msg_Attributes.inl \
 $(ACE_ROOT)/ace/OS/OS_QoS.h

.obj/SOCK.o .obj/SOCK.o .obj/SOCK.o .obj/SOCK.o: SOCK.cpp \
 SOCK.h \
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
 IPC_SAP.h \
 IPC_SAP.i \
 SOCK.i \
 $(ACE_ROOT)/ace/Logging/Log_Msg.h \
 $(ACE_ROOT)/ace/Logging/Log_Record.h \
 $(ACE_ROOT)/ace/Logging/Log_Priority.h \
 $(ACE_ROOT)/ace/Logging/Log_Record.i \
 $(ACE_ROOT)/ace/OS/OS_Log_Msg_Attributes.h \
 $(ACE_ROOT)/ace/OS/OS_Log_Msg_Attributes.inl

.obj/SOCK_Dgram_Bcast.o .obj/SOCK_Dgram_Bcast.o .obj/SOCK_Dgram_Bcast.o .obj/SOCK_Dgram_Bcast.o: SOCK_Dgram_Bcast.cpp \
 SOCK_Dgram_Bcast.h \
 $(ACE_ROOT)/ace/pre.h \
 $(ACE_ROOT)/ace/IPC/INET_Addr.h \
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
 $(ACE_ROOT)/ace/IPC/INET_Addr.i \
 SOCK_Dgram.h \
 SOCK.h \
 IPC_SAP.h \
 IPC_SAP.i \
 SOCK.i \
 SOCK_Dgram.i \
 SOCK_Dgram_Bcast.i \
 $(ACE_ROOT)/ace/Logging/Log_Msg.h \
 $(ACE_ROOT)/ace/Logging/Log_Record.h \
 $(ACE_ROOT)/ace/Logging/Log_Priority.h \
 $(ACE_ROOT)/ace/Logging/Log_Record.i \
 $(ACE_ROOT)/ace/OS/OS_Log_Msg_Attributes.h \
 $(ACE_ROOT)/ace/OS/OS_Log_Msg_Attributes.inl

.obj/SOCK_Dgram.o .obj/SOCK_Dgram.o .obj/SOCK_Dgram.o .obj/SOCK_Dgram.o: SOCK_Dgram.cpp \
 SOCK_Dgram.h \
 $(ACE_ROOT)/ace/pre.h \
 SOCK.h \
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
 IPC_SAP.h \
 IPC_SAP.i \
 SOCK.i \
 $(ACE_ROOT)/ace/IPC/INET_Addr.h \
 $(ACE_ROOT)/ace/IPC/INET_Addr.i \
 SOCK_Dgram.i \
 $(ACE_ROOT)/ace/OS/Handle_Set.h \
 $(ACE_ROOT)/ace/OS/Handle_Set.i \
 $(ACE_ROOT)/ace/Threads/Synch.h \
 $(ACE_ROOT)/ace/Threads/Synch.i \
 $(ACE_ROOT)/ace/Threads/Synch_T.h \
 $(ACE_ROOT)/ace/Threads/Synch_T.i \
 $(ACE_ROOT)/ace/Threads/Thread.h \
 $(ACE_ROOT)/ace/Threads/Thread_Adapter.h \
 $(ACE_ROOT)/ace/OS/Base_Thread_Adapter.h \
 $(ACE_ROOT)/ace/OS/OS_Log_Msg_Attributes.h \
 $(ACE_ROOT)/ace/OS/OS_Log_Msg_Attributes.inl \
 $(ACE_ROOT)/ace/OS/Base_Thread_Adapter.inl \
 $(ACE_ROOT)/ace/Threads/Thread_Adapter.inl \
 $(ACE_ROOT)/ace/Threads/Thread.i \
 $(ACE_ROOT)/ace/Threads/Synch_T.cpp \
 $(ACE_ROOT)/ace/Logging/Log_Msg.h \
 $(ACE_ROOT)/ace/Logging/Log_Record.h \
 $(ACE_ROOT)/ace/Logging/Log_Priority.h \
 $(ACE_ROOT)/ace/Logging/Log_Record.i

.obj/SOCK_Dgram_Mcast.o .obj/SOCK_Dgram_Mcast.o .obj/SOCK_Dgram_Mcast.o .obj/SOCK_Dgram_Mcast.o: SOCK_Dgram_Mcast.cpp \
 SOCK_Dgram_Mcast.h \
 $(ACE_ROOT)/ace/pre.h \
 SOCK_Dgram.h \
 SOCK.h \
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
 IPC_SAP.h \
 IPC_SAP.i \
 SOCK.i \
 $(ACE_ROOT)/ace/IPC/INET_Addr.h \
 $(ACE_ROOT)/ace/IPC/INET_Addr.i \
 SOCK_Dgram.i \
 SOCK_Dgram_Mcast.i

.obj/SOCK_IO.o .obj/SOCK_IO.o .obj/SOCK_IO.o .obj/SOCK_IO.o: SOCK_IO.cpp \
 SOCK_IO.h \
 $(ACE_ROOT)/ace/pre.h \
 SOCK.h \
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
 IPC_SAP.h \
 IPC_SAP.i \
 SOCK.i \
 SOCK_IO.i \
 $(ACE_ROOT)/ace/OS/Handle_Set.h \
 $(ACE_ROOT)/ace/OS/Handle_Set.i

.obj/SOCK_Stream.o .obj/SOCK_Stream.o .obj/SOCK_Stream.o .obj/SOCK_Stream.o: SOCK_Stream.cpp \
 SOCK_Stream.h \
 $(ACE_ROOT)/ace/pre.h \
 SOCK_IO.h \
 SOCK.h \
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
 IPC_SAP.h \
 IPC_SAP.i \
 SOCK.i \
 SOCK_IO.i \
 $(ACE_ROOT)/ace/IPC/INET_Addr.h \
 $(ACE_ROOT)/ace/IPC/INET_Addr.i \
 SOCK_Stream.i

# IF YOU PUT ANYTHING HERE IT WILL GO AWAY
