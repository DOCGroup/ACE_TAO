# Microsoft Developer Studio Generated NMAKE File, Format Version 4.20
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Console Application" 0x0103

!IF "$(CFG)" == ""
CFG=jaws - Win32 Debug
!MESSAGE No configuration specified.  Defaulting to jaws - Win32 Debug.
!ENDIF 

!IF "$(CFG)" != "jaws - Win32 Release" && "$(CFG)" != "jaws - Win32 Debug"
!MESSAGE Invalid configuration "$(CFG)" specified.
!MESSAGE You can specify a configuration when running NMAKE on this makefile
!MESSAGE by defining the macro CFG on the command line.  For example:
!MESSAGE 
!MESSAGE NMAKE /f "jaws.mak" CFG="jaws - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "jaws - Win32 Release" (based on "Win32 (x86) Console Application")
!MESSAGE "jaws - Win32 Debug" (based on "Win32 (x86) Console Application")
!MESSAGE 
!ERROR An invalid configuration is specified.
!ENDIF 

!IF "$(OS)" == "Windows_NT"
NULL=
!ELSE 
NULL=nul
!ENDIF 
################################################################################
# Begin Project
# PROP Target_Last_Scanned "jaws - Win32 Debug"
RSC=rc.exe
CPP=cl.exe

!IF  "$(CFG)" == "jaws - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir ""
# PROP Intermediate_Dir "Release"
# PROP Target_Dir ""
OUTDIR=.
INTDIR=.\Release

ALL : "$(OUTDIR)\jaws-r.exe"

CLEAN : 
	-@erase "$(INTDIR)\HTTP_Config.obj"
	-@erase "$(INTDIR)\HTTP_Handler.obj"
	-@erase "$(INTDIR)\HTTP_Helpers.obj"
	-@erase "$(INTDIR)\HTTP_Request.obj"
	-@erase "$(INTDIR)\HTTP_Response.obj"
	-@erase "$(INTDIR)\HTTP_Server.obj"
	-@erase "$(INTDIR)\IO.obj"
	-@erase "$(INTDIR)\main.obj"
	-@erase "$(INTDIR)\Parse_Headers.obj"
	-@erase "$(OUTDIR)\jaws-r.exe"

"$(INTDIR)" :
    if not exist "$(INTDIR)/$(NULL)" mkdir "$(INTDIR)"

# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /YX /c
# ADD CPP /nologo /MDd /W3 /GX /O2 /I "..\.." /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /D "ACE_NLOGGING" /YX /c
CPP_PROJ=/nologo /MDd /W3 /GX /O2 /I "..\.." /D "WIN32" /D "NDEBUG" /D\
 "_CONSOLE" /D "ACE_NLOGGING" /Fp"$(INTDIR)/jaws.pch" /YX /Fo"$(INTDIR)/" /c 
CPP_OBJS=.\Release/
CPP_SBRS=.\.
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
BSC32_FLAGS=/nologo /o"$(OUTDIR)/jaws.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /machine:I386
# ADD LINK32 wsock32.lib ace-r.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /machine:I386 /out:"jaws-r.exe"
LINK32_FLAGS=wsock32.lib ace-r.lib kernel32.lib user32.lib gdi32.lib\
 winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib\
 uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /incremental:no\
 /pdb:"$(OUTDIR)/jaws-r.pdb" /machine:I386 /out:"$(OUTDIR)/jaws-r.exe" 
LINK32_OBJS= \
	"$(INTDIR)\HTTP_Config.obj" \
	"$(INTDIR)\HTTP_Handler.obj" \
	"$(INTDIR)\HTTP_Helpers.obj" \
	"$(INTDIR)\HTTP_Request.obj" \
	"$(INTDIR)\HTTP_Response.obj" \
	"$(INTDIR)\HTTP_Server.obj" \
	"$(INTDIR)\IO.obj" \
	"$(INTDIR)\main.obj" \
	"$(INTDIR)\Parse_Headers.obj"

"$(OUTDIR)\jaws-r.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ELSEIF  "$(CFG)" == "jaws - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir ""
# PROP Intermediate_Dir "Debug"
# PROP Target_Dir ""
OUTDIR=.
INTDIR=.\Debug

ALL : "$(OUTDIR)\jaws.exe"

CLEAN : 
	-@erase "$(INTDIR)\HTTP_Config.obj"
	-@erase "$(INTDIR)\HTTP_Handler.obj"
	-@erase "$(INTDIR)\HTTP_Helpers.obj"
	-@erase "$(INTDIR)\HTTP_Request.obj"
	-@erase "$(INTDIR)\HTTP_Response.obj"
	-@erase "$(INTDIR)\HTTP_Server.obj"
	-@erase "$(INTDIR)\IO.obj"
	-@erase "$(INTDIR)\main.obj"
	-@erase "$(INTDIR)\Parse_Headers.obj"
	-@erase "$(INTDIR)\vc40.idb"
	-@erase "$(INTDIR)\vc40.pdb"
	-@erase "$(OUTDIR)\jaws.exe"
	-@erase "$(OUTDIR)\jaws.ilk"
	-@erase "$(OUTDIR)\jaws.pdb"

"$(INTDIR)" :
    if not exist "$(INTDIR)/$(NULL)" mkdir "$(INTDIR)"

# ADD BASE CPP /nologo /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /YX /c
# ADD CPP /nologo /MDd /W3 /Gm /GX /Zi /Od /I "..\.." /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /YX /c
CPP_PROJ=/nologo /MDd /W3 /Gm /GX /Zi /Od /I "..\.." /D "WIN32" /D "_DEBUG" /D\
 "_CONSOLE" /Fp"$(INTDIR)/jaws.pch" /YX /Fo"$(INTDIR)/" /Fd"$(INTDIR)/" /c 
CPP_OBJS=.\Debug/
CPP_SBRS=.\.
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
BSC32_FLAGS=/nologo /o"$(OUTDIR)/jaws.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /debug /machine:I386
# ADD LINK32 wsock32.lib ace.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /debug /machine:I386
LINK32_FLAGS=wsock32.lib ace.lib kernel32.lib user32.lib gdi32.lib winspool.lib\
 comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib\
 odbc32.lib odbccp32.lib /nologo /subsystem:console /incremental:yes\
 /pdb:"$(OUTDIR)/jaws.pdb" /debug /machine:I386 /out:"$(OUTDIR)/jaws.exe" 
LINK32_OBJS= \
	"$(INTDIR)\HTTP_Config.obj" \
	"$(INTDIR)\HTTP_Handler.obj" \
	"$(INTDIR)\HTTP_Helpers.obj" \
	"$(INTDIR)\HTTP_Request.obj" \
	"$(INTDIR)\HTTP_Response.obj" \
	"$(INTDIR)\HTTP_Server.obj" \
	"$(INTDIR)\IO.obj" \
	"$(INTDIR)\main.obj" \
	"$(INTDIR)\Parse_Headers.obj"

"$(OUTDIR)\jaws.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ENDIF 

.c{$(CPP_OBJS)}.obj:
   $(CPP) $(CPP_PROJ) $<  

.cpp{$(CPP_OBJS)}.obj:
   $(CPP) $(CPP_PROJ) $<  

.cxx{$(CPP_OBJS)}.obj:
   $(CPP) $(CPP_PROJ) $<  

.c{$(CPP_SBRS)}.sbr:
   $(CPP) $(CPP_PROJ) $<  

.cpp{$(CPP_SBRS)}.sbr:
   $(CPP) $(CPP_PROJ) $<  

.cxx{$(CPP_SBRS)}.sbr:
   $(CPP) $(CPP_PROJ) $<  

################################################################################
# Begin Target

# Name "jaws - Win32 Release"
# Name "jaws - Win32 Debug"

!IF  "$(CFG)" == "jaws - Win32 Release"

!ELSEIF  "$(CFG)" == "jaws - Win32 Debug"

!ENDIF 

################################################################################
# Begin Source File

SOURCE=.\main.cpp

!IF  "$(CFG)" == "jaws - Win32 Release"

DEP_CPP_MAIN_=\
	"..\..\JAWS/server/HTTP_Handler.h"\
	"..\..\JAWS/server/HTTP_Server.h"\
	".\ace\Acceptor.h"\
	".\ace\Filecache.h"\
	".\ace\Reactor.h"\
	".\ace\Service_Config.h"\
	".\ace\Service_Object.h"\
	".\ace\Task_T.h"\
	".\ace\Thread_Manager.h"\
	
NODEP_CPP_MAIN_=\
	".\ace\Asynch_IO.h"\
	".\ace\SOCK_Acceptor.h"\
	

"$(INTDIR)\main.obj" : $(SOURCE) $(DEP_CPP_MAIN_) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "jaws - Win32 Debug"

DEP_CPP_MAIN_=\
	"..\..\JAWS/server/HTTP_Handler.h"\
	"..\..\JAWS/server/HTTP_Request.h"\
	"..\..\JAWS/server/HTTP_Server.h"\
	"..\..\JAWS/server/HTTP_Server_T.cpp"\
	"..\..\JAWS/server/HTTP_Server_T.h"\
	"..\..\JAWS/server/IO.h"\
	".\ace\Acceptor.h"\
	".\ace\ACE.h"\
	".\ace\Asynch_IO.h"\
	".\ace\Filecache.h"\
	".\ace\OS.h"\
	".\ace\Reactor.h"\
	".\ace\Service_Config.h"\
	".\ace\Service_Object.h"\
	".\ace\SOCK_Acceptor.h"\
	".\ace\Task_T.h"\
	".\ace\Thread_Manager.h"\
	".\HTTP_Response.h"\
	".\Parse_Headers.h"\
	

"$(INTDIR)\main.obj" : $(SOURCE) $(DEP_CPP_MAIN_) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\HTTP_Helpers.cpp

!IF  "$(CFG)" == "jaws - Win32 Release"

DEP_CPP_HTTP_=\
	"..\..\JAWS/server/HTTP_Helpers.h"\
	".\ace\ACE.h"\
	".\ace\Atomic_Op.i"\
	".\ace\config-win32-common.h"\
	".\ace\config-win32.h"\
	".\ace\config.h"\
	".\ace\Event_Handler.h"\
	".\ace\Event_Handler.i"\
	".\ace\Log_Msg.h"\
	".\ace\Log_Record.h"\
	".\ace\OS.h"\
	".\ace\OS.i"\
	".\ace\stdcpp.h"\
	".\ace\SV_Semaphore_Complex.h"\
	".\ace\SV_Semaphore_Complex.i"\
	".\ace\SV_Semaphore_Simple.h"\
	".\ace\SV_Semaphore_Simple.i"\
	".\ace\Synch.h"\
	".\ace\Synch.i"\
	".\ace\Synch_T.cpp"\
	".\ace\Synch_T.h"\
	".\ace\Synch_T.i"\
	".\ace\Thread.h"\
	".\ace\Thread.i"\
	".\ace\Trace.h"\
	".\ace\ws2tcpip.h"\
	

"$(INTDIR)\HTTP_Helpers.obj" : $(SOURCE) $(DEP_CPP_HTTP_) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "jaws - Win32 Debug"

DEP_CPP_HTTP_=\
	"..\..\JAWS/server/HTTP_Helpers.h"\
	".\ace\ACE.h"\
	".\ace\ACE.i"\
	".\ace\Atomic_Op.i"\
	".\ace\Auto_Ptr.cpp"\
	".\ace\Auto_Ptr.h"\
	".\ace\Auto_Ptr.i"\
	".\ace\config-win32-common.h"\
	".\ace\config-win32.h"\
	".\ace\config.h"\
	".\ace\Event_Handler.h"\
	".\ace\Event_Handler.i"\
	".\ace\Log_Msg.h"\
	".\ace\Log_Priority.h"\
	".\ace\Log_Record.h"\
	".\ace\Log_Record.i"\
	".\ace\OS.h"\
	".\ace\OS.i"\
	".\ace\SString.h"\
	".\ace\SString.i"\
	".\ace\stdcpp.h"\
	".\ace\SV_Semaphore_Complex.h"\
	".\ace\SV_Semaphore_Complex.i"\
	".\ace\SV_Semaphore_Simple.h"\
	".\ace\SV_Semaphore_Simple.i"\
	".\ace\Synch.h"\
	".\ace\Synch.i"\
	".\ace\Synch_T.cpp"\
	".\ace\Synch_T.h"\
	".\ace\Synch_T.i"\
	".\ace\Thread.h"\
	".\ace\Thread.i"\
	".\ace\Trace.h"\
	".\ace\ws2tcpip.h"\
	

"$(INTDIR)\HTTP_Helpers.obj" : $(SOURCE) $(DEP_CPP_HTTP_) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\HTTP_Handler.cpp

!IF  "$(CFG)" == "jaws - Win32 Release"

DEP_CPP_HTTP_H=\
	"..\..\JAWS/server/HTTP_Handler.h"\
	"..\..\JAWS/server/HTTP_Helpers.h"\
	"..\..\JAWS/server/HTTP_Request.h"\
	"..\..\JAWS/server/IO.h"\
	".\ace\ACE.h"\
	".\ace\Asynch_IO.h"\
	".\ace\Asynch_IO.i"\
	".\ace\Atomic_Op.i"\
	".\ace\config-win32-common.h"\
	".\ace\config-win32.h"\
	".\ace\config.h"\
	".\ace\Containers.cpp"\
	".\ace\Containers.h"\
	".\ace\Containers.i"\
	".\ace\Event_Handler.h"\
	".\ace\Event_Handler.i"\
	".\ace\Filecache.h"\
	".\ace\Free_List.cpp"\
	".\ace\Free_List.h"\
	".\ace\Free_List.i"\
	".\ace\Hash_Map_Manager.cpp"\
	".\ace\Hash_Map_Manager.h"\
	".\ace\Log_Msg.h"\
	".\ace\Log_Record.h"\
	".\ace\Malloc.h"\
	".\ace\Malloc.i"\
	".\ace\Malloc_T.cpp"\
	".\ace\Malloc_T.h"\
	".\ace\Malloc_T.i"\
	".\ace\Mem_Map.h"\
	".\ace\Mem_Map.i"\
	".\ace\Memory_Pool.h"\
	".\ace\Memory_Pool.i"\
	".\ace\Message_Block.h"\
	".\ace\Message_Block.i"\
	".\ace\OS.h"\
	".\ace\OS.i"\
	".\ace\Service_Config.h"\
	".\ace\Signal.h"\
	".\ace\Signal.i"\
	".\ace\stdcpp.h"\
	".\ace\SV_Semaphore_Complex.h"\
	".\ace\SV_Semaphore_Complex.i"\
	".\ace\SV_Semaphore_Simple.h"\
	".\ace\SV_Semaphore_Simple.i"\
	".\ace\Synch.h"\
	".\ace\Synch.i"\
	".\ace\Synch_T.cpp"\
	".\ace\Synch_T.h"\
	".\ace\Synch_T.i"\
	".\ace\Thread.h"\
	".\ace\Thread.i"\
	".\ace\Trace.h"\
	".\ace\ws2tcpip.h"\
	".\HTTP_Response.h"\
	".\Parse_Headers.h"\
	

"$(INTDIR)\HTTP_Handler.obj" : $(SOURCE) $(DEP_CPP_HTTP_H) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "jaws - Win32 Debug"

DEP_CPP_HTTP_H=\
	"..\..\JAWS/server/HTTP_Handler.h"\
	"..\..\JAWS/server/HTTP_Helpers.h"\
	"..\..\JAWS/server/HTTP_Request.h"\
	"..\..\JAWS/server/IO.h"\
	".\ace\ACE.h"\
	".\ace\ACE.i"\
	".\ace\Addr.h"\
	".\ace\Addr.i"\
	".\ace\Asynch_IO.h"\
	".\ace\Asynch_IO.i"\
	".\ace\Atomic_Op.i"\
	".\ace\Auto_Ptr.cpp"\
	".\ace\Auto_Ptr.h"\
	".\ace\Auto_Ptr.i"\
	".\ace\config-win32-common.h"\
	".\ace\config-win32.h"\
	".\ace\config.h"\
	".\ace\Containers.cpp"\
	".\ace\Containers.h"\
	".\ace\Containers.i"\
	".\ace\Event_Handler.h"\
	".\ace\Event_Handler.i"\
	".\ace\Filecache.h"\
	".\ace\Free_List.cpp"\
	".\ace\Free_List.h"\
	".\ace\Free_List.i"\
	".\ace\Handle_Set.h"\
	".\ace\Handle_Set.i"\
	".\ace\Hash_Map_Manager.cpp"\
	".\ace\Hash_Map_Manager.h"\
	".\ace\High_Res_Timer.h"\
	".\ace\High_Res_Timer.i"\
	".\ace\INET_Addr.h"\
	".\ace\INET_Addr.i"\
	".\ace\IPC_SAP.h"\
	".\ace\IPC_SAP.i"\
	".\ace\Local_Tokens.h"\
	".\ace\Local_Tokens.i"\
	".\ace\Log_Msg.h"\
	".\ace\Log_Priority.h"\
	".\ace\Log_Record.h"\
	".\ace\Log_Record.i"\
	".\ace\Malloc.h"\
	".\ace\Malloc.i"\
	".\ace\Malloc_T.cpp"\
	".\ace\Malloc_T.h"\
	".\ace\Malloc_T.i"\
	".\ace\Map_Manager.cpp"\
	".\ace\Map_Manager.h"\
	".\ace\Map_Manager.i"\
	".\ace\Mem_Map.h"\
	".\ace\Mem_Map.i"\
	".\ace\Memory_Pool.h"\
	".\ace\Memory_Pool.i"\
	".\ace\Message_Block.h"\
	".\ace\Message_Queue.h"\
	".\ace\OS.h"\
	".\ace\OS.i"\
	".\ace\Pipe.h"\
	".\ace\Pipe.i"\
	".\ace\Proactor.h"\
	".\ace\Proactor.i"\
	".\ace\Reactor.h"\
	".\ace\Reactor.i"\
	".\ace\ReactorEx.h"\
	".\ace\Service_Config.h"\
	".\ace\Service_Config.i"\
	".\ace\Service_Object.h"\
	".\ace\Service_Object.i"\
	".\ace\Shared_Object.h"\
	".\ace\Shared_Object.i"\
	".\ace\Signal.h"\
	".\ace\Signal.i"\
	".\ace\SOCK.h"\
	".\ace\SOCK.i"\
	".\ace\SOCK_IO.h"\
	".\ace\SOCK_IO.i"\
	".\ace\SOCK_Stream.h"\
	".\ace\SOCK_Stream.i"\
	".\ace\SString.h"\
	".\ace\SString.i"\
	".\ace\stdcpp.h"\
	".\ace\SV_Semaphore_Complex.h"\
	".\ace\SV_Semaphore_Complex.i"\
	".\ace\SV_Semaphore_Simple.h"\
	".\ace\SV_Semaphore_Simple.i"\
	".\ace\Synch.h"\
	".\ace\Synch.i"\
	".\ace\Synch_Options.h"\
	".\ace\Synch_T.cpp"\
	".\ace\Synch_T.h"\
	".\ace\Synch_T.i"\
	".\ace\Thread.h"\
	".\ace\Thread.i"\
	".\ace\Thread_Manager.h"\
	".\ace\Thread_Manager.i"\
	".\ace\Time_Value.h"\
	".\ace\Timer_Heap.h"\
	".\ace\Timer_Heap_T.cpp"\
	".\ace\Timer_Heap_T.h"\
	".\ace\Timer_List.h"\
	".\ace\Timer_List_T.cpp"\
	".\ace\Timer_List_T.h"\
	".\ace\Timer_Queue.h"\
	".\ace\Timer_Queue_T.cpp"\
	".\ace\Timer_Queue_T.h"\
	".\ace\Timer_Queue_T.i"\
	".\ace\Timer_Wheel.h"\
	".\ace\Timer_Wheel_T.cpp"\
	".\ace\Timer_Wheel_T.h"\
	".\ace\Token.h"\
	".\ace\Token.i"\
	".\ace\Trace.h"\
	".\ace\ws2tcpip.h"\
	".\HTTP_Response.h"\
	".\Parse_Headers.h"\
	

"$(INTDIR)\HTTP_Handler.obj" : $(SOURCE) $(DEP_CPP_HTTP_H) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\IO.cpp

!IF  "$(CFG)" == "jaws - Win32 Release"

DEP_CPP_IO_CP=\
	"..\..\JAWS/server/HTTP_Helpers.h"\
	"..\..\JAWS/server/IO.h"\
	".\ace\ACE.h"\
	".\ace\Addr.h"\
	".\ace\Addr.i"\
	".\ace\Asynch_IO.h"\
	".\ace\Asynch_IO.i"\
	".\ace\Atomic_Op.i"\
	".\ace\config-win32-common.h"\
	".\ace\config-win32.h"\
	".\ace\config.h"\
	".\ace\Containers.cpp"\
	".\ace\Containers.h"\
	".\ace\Containers.i"\
	".\ace\Event_Handler.h"\
	".\ace\Event_Handler.i"\
	".\ace\Filecache.h"\
	".\ace\Free_List.cpp"\
	".\ace\Free_List.h"\
	".\ace\Free_List.i"\
	".\ace\Hash_Map_Manager.cpp"\
	".\ace\Hash_Map_Manager.h"\
	".\ace\INET_Addr.h"\
	".\ace\INET_Addr.i"\
	".\ace\IPC_SAP.h"\
	".\ace\IPC_SAP.i"\
	".\ace\Log_Msg.h"\
	".\ace\Log_Record.h"\
	".\ace\Malloc.h"\
	".\ace\Malloc.i"\
	".\ace\Malloc_T.cpp"\
	".\ace\Malloc_T.h"\
	".\ace\Malloc_T.i"\
	".\ace\Mem_Map.h"\
	".\ace\Mem_Map.i"\
	".\ace\Memory_Pool.h"\
	".\ace\Memory_Pool.i"\
	".\ace\Message_Block.h"\
	".\ace\Message_Block.i"\
	".\ace\OS.h"\
	".\ace\OS.i"\
	".\ace\Service_Config.h"\
	".\ace\Signal.h"\
	".\ace\Signal.i"\
	".\ace\SOCK.h"\
	".\ace\SOCK.i"\
	".\ace\SOCK_IO.h"\
	".\ace\SOCK_IO.i"\
	".\ace\SOCK_Stream.h"\
	".\ace\SOCK_Stream.i"\
	".\ace\stdcpp.h"\
	".\ace\SV_Semaphore_Complex.h"\
	".\ace\SV_Semaphore_Complex.i"\
	".\ace\SV_Semaphore_Simple.h"\
	".\ace\SV_Semaphore_Simple.i"\
	".\ace\Synch.h"\
	".\ace\Synch.i"\
	".\ace\Synch_T.cpp"\
	".\ace\Synch_T.h"\
	".\ace\Synch_T.i"\
	".\ace\Thread.h"\
	".\ace\Thread.i"\
	".\ace\Trace.h"\
	".\ace\ws2tcpip.h"\
	

"$(INTDIR)\IO.obj" : $(SOURCE) $(DEP_CPP_IO_CP) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "jaws - Win32 Debug"

DEP_CPP_IO_CP=\
	"..\..\JAWS/server/HTTP_Helpers.h"\
	"..\..\JAWS/server/IO.h"\
	".\ace\ACE.h"\
	".\ace\ACE.i"\
	".\ace\Addr.h"\
	".\ace\Addr.i"\
	".\ace\Asynch_IO.h"\
	".\ace\Asynch_IO.i"\
	".\ace\Atomic_Op.i"\
	".\ace\Auto_Ptr.cpp"\
	".\ace\Auto_Ptr.h"\
	".\ace\Auto_Ptr.i"\
	".\ace\config-win32-common.h"\
	".\ace\config-win32.h"\
	".\ace\config.h"\
	".\ace\Containers.cpp"\
	".\ace\Containers.h"\
	".\ace\Containers.i"\
	".\ace\Event_Handler.h"\
	".\ace\Event_Handler.i"\
	".\ace\Filecache.h"\
	".\ace\Free_List.cpp"\
	".\ace\Free_List.h"\
	".\ace\Free_List.i"\
	".\ace\Handle_Set.h"\
	".\ace\Handle_Set.i"\
	".\ace\Hash_Map_Manager.cpp"\
	".\ace\Hash_Map_Manager.h"\
	".\ace\High_Res_Timer.h"\
	".\ace\High_Res_Timer.i"\
	".\ace\INET_Addr.h"\
	".\ace\INET_Addr.i"\
	".\ace\IPC_SAP.h"\
	".\ace\IPC_SAP.i"\
	".\ace\Local_Tokens.h"\
	".\ace\Local_Tokens.i"\
	".\ace\Log_Msg.h"\
	".\ace\Log_Priority.h"\
	".\ace\Log_Record.h"\
	".\ace\Log_Record.i"\
	".\ace\Malloc.h"\
	".\ace\Malloc.i"\
	".\ace\Malloc_T.cpp"\
	".\ace\Malloc_T.h"\
	".\ace\Malloc_T.i"\
	".\ace\Map_Manager.cpp"\
	".\ace\Map_Manager.h"\
	".\ace\Map_Manager.i"\
	".\ace\Mem_Map.h"\
	".\ace\Mem_Map.i"\
	".\ace\Memory_Pool.h"\
	".\ace\Memory_Pool.i"\
	".\ace\Message_Block.h"\
	".\ace\Message_Queue.h"\
	".\ace\OS.h"\
	".\ace\OS.i"\
	".\ace\Pipe.h"\
	".\ace\Pipe.i"\
	".\ace\Proactor.h"\
	".\ace\Proactor.i"\
	".\ace\Reactor.h"\
	".\ace\Reactor.i"\
	".\ace\ReactorEx.h"\
	".\ace\Service_Config.h"\
	".\ace\Service_Config.i"\
	".\ace\Service_Object.h"\
	".\ace\Service_Object.i"\
	".\ace\Shared_Object.h"\
	".\ace\Shared_Object.i"\
	".\ace\Signal.h"\
	".\ace\Signal.i"\
	".\ace\SOCK.h"\
	".\ace\SOCK.i"\
	".\ace\SOCK_IO.h"\
	".\ace\SOCK_IO.i"\
	".\ace\SOCK_Stream.h"\
	".\ace\SOCK_Stream.i"\
	".\ace\SString.h"\
	".\ace\SString.i"\
	".\ace\stdcpp.h"\
	".\ace\SV_Semaphore_Complex.h"\
	".\ace\SV_Semaphore_Complex.i"\
	".\ace\SV_Semaphore_Simple.h"\
	".\ace\SV_Semaphore_Simple.i"\
	".\ace\Synch.h"\
	".\ace\Synch.i"\
	".\ace\Synch_Options.h"\
	".\ace\Synch_T.cpp"\
	".\ace\Synch_T.h"\
	".\ace\Synch_T.i"\
	".\ace\Thread.h"\
	".\ace\Thread.i"\
	".\ace\Thread_Manager.h"\
	".\ace\Thread_Manager.i"\
	".\ace\Time_Value.h"\
	".\ace\Timer_Heap.h"\
	".\ace\Timer_Heap_T.cpp"\
	".\ace\Timer_Heap_T.h"\
	".\ace\Timer_List.h"\
	".\ace\Timer_List_T.cpp"\
	".\ace\Timer_List_T.h"\
	".\ace\Timer_Queue.h"\
	".\ace\Timer_Queue_T.cpp"\
	".\ace\Timer_Queue_T.h"\
	".\ace\Timer_Queue_T.i"\
	".\ace\Timer_Wheel.h"\
	".\ace\Timer_Wheel_T.cpp"\
	".\ace\Timer_Wheel_T.h"\
	".\ace\Token.h"\
	".\ace\Token.i"\
	".\ace\Trace.h"\
	".\ace\ws2tcpip.h"\
	

"$(INTDIR)\IO.obj" : $(SOURCE) $(DEP_CPP_IO_CP) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\HTTP_Request.cpp

!IF  "$(CFG)" == "jaws - Win32 Release"

DEP_CPP_HTTP_R=\
	"..\..\JAWS/server/HTTP_Helpers.h"\
	"..\..\JAWS/server/HTTP_Request.h"\
	".\ace\ACE.h"\
	".\ace\Atomic_Op.i"\
	".\ace\config-win32-common.h"\
	".\ace\config-win32.h"\
	".\ace\config.h"\
	".\ace\Containers.cpp"\
	".\ace\Containers.h"\
	".\ace\Containers.i"\
	".\ace\Event_Handler.h"\
	".\ace\Event_Handler.i"\
	".\ace\Free_List.cpp"\
	".\ace\Free_List.h"\
	".\ace\Free_List.i"\
	".\ace\Log_Msg.h"\
	".\ace\Log_Record.h"\
	".\ace\Malloc.h"\
	".\ace\Malloc.i"\
	".\ace\Malloc_T.cpp"\
	".\ace\Malloc_T.h"\
	".\ace\Malloc_T.i"\
	".\ace\Mem_Map.h"\
	".\ace\Mem_Map.i"\
	".\ace\Memory_Pool.h"\
	".\ace\Memory_Pool.i"\
	".\ace\Message_Block.h"\
	".\ace\Message_Block.i"\
	".\ace\OS.h"\
	".\ace\OS.i"\
	".\ace\Signal.h"\
	".\ace\Signal.i"\
	".\ace\stdcpp.h"\
	".\ace\SV_Semaphore_Complex.h"\
	".\ace\SV_Semaphore_Complex.i"\
	".\ace\SV_Semaphore_Simple.h"\
	".\ace\SV_Semaphore_Simple.i"\
	".\ace\Synch.h"\
	".\ace\Synch.i"\
	".\ace\Synch_T.cpp"\
	".\ace\Synch_T.h"\
	".\ace\Synch_T.i"\
	".\ace\Thread.h"\
	".\ace\Thread.i"\
	".\ace\Trace.h"\
	".\ace\ws2tcpip.h"\
	".\HTTP_Config.h"\
	".\Parse_Headers.h"\
	

"$(INTDIR)\HTTP_Request.obj" : $(SOURCE) $(DEP_CPP_HTTP_R) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "jaws - Win32 Debug"

DEP_CPP_HTTP_R=\
	"..\..\JAWS/server/HTTP_Helpers.h"\
	"..\..\JAWS/server/HTTP_Request.h"\
	".\ace\ACE.h"\
	".\ace\ACE.i"\
	".\ace\Atomic_Op.i"\
	".\ace\Auto_Ptr.cpp"\
	".\ace\Auto_Ptr.h"\
	".\ace\Auto_Ptr.i"\
	".\ace\config-win32-common.h"\
	".\ace\config-win32.h"\
	".\ace\config.h"\
	".\ace\Event_Handler.h"\
	".\ace\Event_Handler.i"\
	".\ace\Log_Msg.h"\
	".\ace\Log_Priority.h"\
	".\ace\Log_Record.h"\
	".\ace\Log_Record.i"\
	".\ace\Message_Block.h"\
	".\ace\OS.h"\
	".\ace\OS.i"\
	".\ace\SString.h"\
	".\ace\SString.i"\
	".\ace\stdcpp.h"\
	".\ace\SV_Semaphore_Complex.h"\
	".\ace\SV_Semaphore_Complex.i"\
	".\ace\SV_Semaphore_Simple.h"\
	".\ace\SV_Semaphore_Simple.i"\
	".\ace\Synch.h"\
	".\ace\Synch.i"\
	".\ace\Synch_T.cpp"\
	".\ace\Synch_T.h"\
	".\ace\Synch_T.i"\
	".\ace\Thread.h"\
	".\ace\Thread.i"\
	".\ace\Trace.h"\
	".\ace\ws2tcpip.h"\
	".\HTTP_Config.h"\
	".\Parse_Headers.h"\
	

"$(INTDIR)\HTTP_Request.obj" : $(SOURCE) $(DEP_CPP_HTTP_R) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\HTTP_Server.cpp

!IF  "$(CFG)" == "jaws - Win32 Release"

DEP_CPP_HTTP_S=\
	"..\..\JAWS/server/HTTP_Server.h"\
	"..\..\JAWS/server/IO.h"\
	".\ace\Acceptor.h"\
	".\ace\ACE.h"\
	".\ace\Addr.h"\
	".\ace\Addr.i"\
	".\ace\Asynch_Acceptor.cpp"\
	".\ace\Asynch_Acceptor.h"\
	".\ace\Asynch_Acceptor.i"\
	".\ace\Asynch_IO.h"\
	".\ace\Asynch_IO.i"\
	".\ace\Atomic_Op.i"\
	".\ace\config-win32-common.h"\
	".\ace\config-win32.h"\
	".\ace\config.h"\
	".\ace\Containers.cpp"\
	".\ace\Containers.h"\
	".\ace\Containers.i"\
	".\ace\Event_Handler.h"\
	".\ace\Event_Handler.i"\
	".\ace\Free_List.cpp"\
	".\ace\Free_List.h"\
	".\ace\Free_List.i"\
	".\ace\Get_Opt.h"\
	".\ace\Get_Opt.i"\
	".\ace\High_Res_Timer.h"\
	".\ace\High_Res_Timer.i"\
	".\ace\INET_Addr.h"\
	".\ace\INET_Addr.i"\
	".\ace\Log_Msg.h"\
	".\ace\Log_Record.h"\
	".\ace\Malloc.h"\
	".\ace\Malloc.i"\
	".\ace\Malloc_T.cpp"\
	".\ace\Malloc_T.h"\
	".\ace\Malloc_T.i"\
	".\ace\Mem_Map.h"\
	".\ace\Mem_Map.i"\
	".\ace\Memory_Pool.h"\
	".\ace\Memory_Pool.i"\
	".\ace\Message_Block.h"\
	".\ace\Message_Block.i"\
	".\ace\OS.h"\
	".\ace\OS.i"\
	".\ace\Proactor.h"\
	".\ace\Proactor.i"\
	".\ace\Service_Config.h"\
	".\ace\Service_Object.h"\
	".\ace\Service_Object.i"\
	".\ace\Shared_Object.h"\
	".\ace\Shared_Object.i"\
	".\ace\Signal.h"\
	".\ace\Signal.i"\
	".\ace\stdcpp.h"\
	".\ace\SV_Semaphore_Complex.h"\
	".\ace\SV_Semaphore_Complex.i"\
	".\ace\SV_Semaphore_Simple.h"\
	".\ace\SV_Semaphore_Simple.i"\
	".\ace\Svc_Handler.h"\
	".\ace\Synch.h"\
	".\ace\Synch.i"\
	".\ace\Synch_Options.h"\
	".\ace\Synch_T.cpp"\
	".\ace\Synch_T.h"\
	".\ace\Synch_T.i"\
	".\ace\Task.h"\
	".\ace\Task.i"\
	".\ace\Task_T.h"\
	".\ace\Thread.h"\
	".\ace\Thread.i"\
	".\ace\Thread_Manager.h"\
	".\ace\Thread_Manager.i"\
	".\ace\Time_Value.h"\
	".\ace\Timer_Heap.h"\
	".\ace\Timer_Heap_T.cpp"\
	".\ace\Timer_Heap_T.h"\
	".\ace\Timer_List.h"\
	".\ace\Timer_List_T.cpp"\
	".\ace\Timer_List_T.h"\
	".\ace\Timer_Queue.h"\
	".\ace\Timer_Queue_T.cpp"\
	".\ace\Timer_Queue_T.h"\
	".\ace\Timer_Queue_T.i"\
	".\ace\Timer_Wheel.h"\
	".\ace\Timer_Wheel_T.cpp"\
	".\ace\Timer_Wheel_T.h"\
	".\ace\Trace.h"\
	".\ace\ws2tcpip.h"\
	

"$(INTDIR)\HTTP_Server.obj" : $(SOURCE) $(DEP_CPP_HTTP_S) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "jaws - Win32 Debug"

DEP_CPP_HTTP_S=\
	"..\..\JAWS/server/HTTP_Handler.h"\
	"..\..\JAWS/server/HTTP_Request.h"\
	"..\..\JAWS/server/HTTP_Server.h"\
	"..\..\JAWS/server/HTTP_Server_T.cpp"\
	"..\..\JAWS/server/HTTP_Server_T.h"\
	"..\..\JAWS/server/IO.h"\
	".\ace\Acceptor.cpp"\
	".\ace\Acceptor.h"\
	".\ace\Acceptor.i"\
	".\ace\ACE.h"\
	".\ace\ACE.i"\
	".\ace\Addr.h"\
	".\ace\Addr.i"\
	".\ace\Asynch_Acceptor.cpp"\
	".\ace\Asynch_Acceptor.h"\
	".\ace\Asynch_Acceptor.i"\
	".\ace\Asynch_IO.h"\
	".\ace\Asynch_IO.i"\
	".\ace\Atomic_Op.i"\
	".\ace\Auto_Ptr.cpp"\
	".\ace\Auto_Ptr.h"\
	".\ace\Auto_Ptr.i"\
	".\ace\config-win32-common.h"\
	".\ace\config-win32.h"\
	".\ace\config.h"\
	".\ace\Containers.cpp"\
	".\ace\Containers.h"\
	".\ace\Containers.i"\
	".\ace\Dynamic.h"\
	".\ace\Dynamic.i"\
	".\ace\Event_Handler.h"\
	".\ace\Event_Handler.i"\
	".\ace\Free_List.cpp"\
	".\ace\Free_List.h"\
	".\ace\Free_List.i"\
	".\ace\Get_Opt.h"\
	".\ace\Get_Opt.i"\
	".\ace\Handle_Set.h"\
	".\ace\Handle_Set.i"\
	".\ace\Hash_Map_Manager.cpp"\
	".\ace\Hash_Map_Manager.h"\
	".\ace\High_Res_Timer.h"\
	".\ace\High_Res_Timer.i"\
	".\ace\INET_Addr.h"\
	".\ace\INET_Addr.i"\
	".\ace\IPC_SAP.h"\
	".\ace\IPC_SAP.i"\
	".\ace\Local_Tokens.h"\
	".\ace\Local_Tokens.i"\
	".\ace\Log_Msg.h"\
	".\ace\Log_Priority.h"\
	".\ace\Log_Record.h"\
	".\ace\Log_Record.i"\
	".\ace\Malloc.h"\
	".\ace\Malloc.i"\
	".\ace\Malloc_T.cpp"\
	".\ace\Malloc_T.h"\
	".\ace\Malloc_T.i"\
	".\ace\Map_Manager.cpp"\
	".\ace\Map_Manager.h"\
	".\ace\Map_Manager.i"\
	".\ace\Mem_Map.h"\
	".\ace\Mem_Map.i"\
	".\ace\Memory_Pool.h"\
	".\ace\Memory_Pool.i"\
	".\ace\Message_Block.h"\
	".\ace\Message_Queue.h"\
	".\ace\Module.cpp"\
	".\ace\Module.h"\
	".\ace\Module.i"\
	".\ace\OS.h"\
	".\ace\OS.i"\
	".\ace\Pipe.h"\
	".\ace\Pipe.i"\
	".\ace\Proactor.h"\
	".\ace\Proactor.i"\
	".\ace\Reactor.h"\
	".\ace\Reactor.i"\
	".\ace\ReactorEx.h"\
	".\ace\Service_Config.h"\
	".\ace\Service_Config.i"\
	".\ace\Service_Object.h"\
	".\ace\Service_Object.i"\
	".\ace\Shared_Object.h"\
	".\ace\Shared_Object.i"\
	".\ace\Signal.h"\
	".\ace\Signal.i"\
	".\ace\SOCK.h"\
	".\ace\SOCK.i"\
	".\ace\SOCK_Acceptor.h"\
	".\ace\SOCK_Acceptor.i"\
	".\ace\SOCK_IO.h"\
	".\ace\SOCK_IO.i"\
	".\ace\SOCK_Stream.h"\
	".\ace\SOCK_Stream.i"\
	".\ace\SString.h"\
	".\ace\SString.i"\
	".\ace\stdcpp.h"\
	".\ace\Strategies.h"\
	".\ace\Strategies_T.cpp"\
	".\ace\Strategies_T.h"\
	".\ace\Stream_Modules.cpp"\
	".\ace\Stream_Modules.h"\
	".\ace\Stream_Modules.i"\
	".\ace\SV_Semaphore_Complex.h"\
	".\ace\SV_Semaphore_Complex.i"\
	".\ace\SV_Semaphore_Simple.h"\
	".\ace\SV_Semaphore_Simple.i"\
	".\ace\Svc_Handler.cpp"\
	".\ace\Svc_Handler.h"\
	".\ace\Svc_Handler.i"\
	".\ace\Synch.h"\
	".\ace\Synch.i"\
	".\ace\Synch_Options.h"\
	".\ace\Synch_T.cpp"\
	".\ace\Synch_T.h"\
	".\ace\Synch_T.i"\
	".\ace\Task.h"\
	".\ace\Task.i"\
	".\ace\Task_T.cpp"\
	".\ace\Task_T.h"\
	".\ace\Task_T.i"\
	".\ace\Thread.h"\
	".\ace\Thread.i"\
	".\ace\Thread_Manager.h"\
	".\ace\Thread_Manager.i"\
	".\ace\Time_Value.h"\
	".\ace\Timer_Heap.h"\
	".\ace\Timer_Heap_T.cpp"\
	".\ace\Timer_Heap_T.h"\
	".\ace\Timer_List.h"\
	".\ace\Timer_List_T.cpp"\
	".\ace\Timer_List_T.h"\
	".\ace\Timer_Queue.h"\
	".\ace\Timer_Queue_T.cpp"\
	".\ace\Timer_Queue_T.h"\
	".\ace\Timer_Queue_T.i"\
	".\ace\Timer_Wheel.h"\
	".\ace\Timer_Wheel_T.cpp"\
	".\ace\Timer_Wheel_T.h"\
	".\ace\Token.h"\
	".\ace\Token.i"\
	".\ace\Trace.h"\
	".\ace\ws2tcpip.h"\
	".\HTTP_Response.h"\
	".\Parse_Headers.h"\
	

"$(INTDIR)\HTTP_Server.obj" : $(SOURCE) $(DEP_CPP_HTTP_S) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\HTTP_Config.cpp

!IF  "$(CFG)" == "jaws - Win32 Release"

DEP_CPP_HTTP_C=\
	".\ace\config-win32-common.h"\
	".\ace\config-win32.h"\
	".\ace\config.h"\
	".\ace\Log_Msg.h"\
	".\ace\Log_Record.h"\
	".\ace\OS.h"\
	".\ace\OS.i"\
	".\ace\stdcpp.h"\
	".\ace\Trace.h"\
	".\ace\ws2tcpip.h"\
	".\HTTP_Config.h"\
	

"$(INTDIR)\HTTP_Config.obj" : $(SOURCE) $(DEP_CPP_HTTP_C) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "jaws - Win32 Debug"

DEP_CPP_HTTP_C=\
	".\ace\ACE.h"\
	".\ace\ACE.i"\
	".\ace\Auto_Ptr.cpp"\
	".\ace\Auto_Ptr.h"\
	".\ace\Auto_Ptr.i"\
	".\ace\config-win32-common.h"\
	".\ace\config-win32.h"\
	".\ace\config.h"\
	".\ace\Log_Msg.h"\
	".\ace\Log_Priority.h"\
	".\ace\Log_Record.h"\
	".\ace\Log_Record.i"\
	".\ace\OS.h"\
	".\ace\OS.i"\
	".\ace\SString.h"\
	".\ace\SString.i"\
	".\ace\stdcpp.h"\
	".\ace\Trace.h"\
	".\ace\ws2tcpip.h"\
	".\HTTP_Config.h"\
	

"$(INTDIR)\HTTP_Config.obj" : $(SOURCE) $(DEP_CPP_HTTP_C) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\HTTP_Response.cpp

!IF  "$(CFG)" == "jaws - Win32 Release"

DEP_CPP_HTTP_RE=\
	"..\..\JAWS/server/HTTP_Helpers.h"\
	"..\..\JAWS/server/HTTP_Request.h"\
	"..\..\JAWS/server/IO.h"\
	".\ace\ACE.h"\
	".\ace\Asynch_IO.h"\
	".\ace\Asynch_IO.i"\
	".\ace\Atomic_Op.i"\
	".\ace\config-win32-common.h"\
	".\ace\config-win32.h"\
	".\ace\config.h"\
	".\ace\Event_Handler.h"\
	".\ace\Event_Handler.i"\
	".\ace\Log_Msg.h"\
	".\ace\Log_Record.h"\
	".\ace\Mem_Map.h"\
	".\ace\Mem_Map.i"\
	".\ace\OS.h"\
	".\ace\OS.i"\
	".\ace\Process.h"\
	".\ace\Process.i"\
	".\ace\stdcpp.h"\
	".\ace\SV_Semaphore_Complex.h"\
	".\ace\SV_Semaphore_Complex.i"\
	".\ace\SV_Semaphore_Simple.h"\
	".\ace\SV_Semaphore_Simple.i"\
	".\ace\Synch.h"\
	".\ace\Synch.i"\
	".\ace\Synch_T.cpp"\
	".\ace\Synch_T.h"\
	".\ace\Synch_T.i"\
	".\ace\Thread.h"\
	".\ace\Thread.i"\
	".\ace\Trace.h"\
	".\ace\ws2tcpip.h"\
	".\HTTP_Config.h"\
	".\HTTP_Response.h"\
	".\Parse_Headers.h"\
	

"$(INTDIR)\HTTP_Response.obj" : $(SOURCE) $(DEP_CPP_HTTP_RE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "jaws - Win32 Debug"

DEP_CPP_HTTP_RE=\
	"..\..\JAWS/server/HTTP_Helpers.h"\
	"..\..\JAWS/server/HTTP_Request.h"\
	"..\..\JAWS/server/IO.h"\
	".\ace\ACE.h"\
	".\ace\ACE.i"\
	".\ace\Asynch_IO.h"\
	".\ace\Asynch_IO.i"\
	".\ace\Atomic_Op.i"\
	".\ace\Auto_Ptr.cpp"\
	".\ace\Auto_Ptr.h"\
	".\ace\Auto_Ptr.i"\
	".\ace\config-win32-common.h"\
	".\ace\config-win32.h"\
	".\ace\config.h"\
	".\ace\Event_Handler.h"\
	".\ace\Event_Handler.i"\
	".\ace\Log_Msg.h"\
	".\ace\Log_Priority.h"\
	".\ace\Log_Record.h"\
	".\ace\Log_Record.i"\
	".\ace\Mem_Map.h"\
	".\ace\Mem_Map.i"\
	".\ace\OS.h"\
	".\ace\OS.i"\
	".\ace\Process.h"\
	".\ace\Process.i"\
	".\ace\SString.h"\
	".\ace\SString.i"\
	".\ace\stdcpp.h"\
	".\ace\SV_Semaphore_Complex.h"\
	".\ace\SV_Semaphore_Complex.i"\
	".\ace\SV_Semaphore_Simple.h"\
	".\ace\SV_Semaphore_Simple.i"\
	".\ace\Synch.h"\
	".\ace\Synch.i"\
	".\ace\Synch_T.cpp"\
	".\ace\Synch_T.h"\
	".\ace\Synch_T.i"\
	".\ace\Thread.h"\
	".\ace\Thread.i"\
	".\ace\Trace.h"\
	".\ace\ws2tcpip.h"\
	".\HTTP_Config.h"\
	".\HTTP_Response.h"\
	".\Parse_Headers.h"\
	

"$(INTDIR)\HTTP_Response.obj" : $(SOURCE) $(DEP_CPP_HTTP_RE) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\Parse_Headers.cpp

!IF  "$(CFG)" == "jaws - Win32 Release"

DEP_CPP_PARSE=\
	".\ace\config-win32-common.h"\
	".\ace\config-win32.h"\
	".\ace\config.h"\
	".\ace\Log_Msg.h"\
	".\ace\Log_Record.h"\
	".\ace\OS.h"\
	".\ace\OS.i"\
	".\ace\stdcpp.h"\
	".\ace\Trace.h"\
	".\ace\ws2tcpip.h"\
	".\Parse_Headers.h"\
	

"$(INTDIR)\Parse_Headers.obj" : $(SOURCE) $(DEP_CPP_PARSE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "jaws - Win32 Debug"

DEP_CPP_PARSE=\
	".\ace\ACE.h"\
	".\ace\ACE.i"\
	".\ace\Auto_Ptr.cpp"\
	".\ace\Auto_Ptr.h"\
	".\ace\Auto_Ptr.i"\
	".\ace\config-win32-common.h"\
	".\ace\config-win32.h"\
	".\ace\config.h"\
	".\ace\Log_Msg.h"\
	".\ace\Log_Priority.h"\
	".\ace\Log_Record.h"\
	".\ace\Log_Record.i"\
	".\ace\OS.h"\
	".\ace\OS.i"\
	".\ace\SString.h"\
	".\ace\SString.i"\
	".\ace\stdcpp.h"\
	".\ace\Trace.h"\
	".\ace\ws2tcpip.h"\
	".\Parse_Headers.h"\
	

"$(INTDIR)\Parse_Headers.obj" : $(SOURCE) $(DEP_CPP_PARSE) "$(INTDIR)"


!ENDIF 

# End Source File
# End Target
# End Project
################################################################################
