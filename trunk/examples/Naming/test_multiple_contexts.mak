# Microsoft Developer Studio Generated NMAKE File, Format Version 4.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Console Application" 0x0103

!IF "$(CFG)" == ""
CFG=test_multiple_contexts - Win32 Debug
!MESSAGE No configuration specified.  Defaulting to test_multiple_contexts -\
 Win32 Debug.
!ENDIF 

!IF "$(CFG)" != "test_multiple_contexts - Win32 Release" && "$(CFG)" !=\
 "test_multiple_contexts - Win32 Debug"
!MESSAGE Invalid configuration "$(CFG)" specified.
!MESSAGE You can specify a configuration when running NMAKE on this makefile
!MESSAGE by defining the macro CFG on the command line.  For example:
!MESSAGE 
!MESSAGE NMAKE /f "test_multiple_contexts.mak"\
 CFG="test_multiple_contexts - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "test_multiple_contexts - Win32 Release" (based on\
 "Win32 (x86) Console Application")
!MESSAGE "test_multiple_contexts - Win32 Debug" (based on\
 "Win32 (x86) Console Application")
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
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "test_multiple_contexts - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
OUTDIR=.
INTDIR=.

ALL : "$(OUTDIR)\test_multiple_contexts.exe"

CLEAN : 
	-@erase ".\test_multiple_contexts.exe"
	-@erase ".\test_multiple_contexts.obj"

# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /YX /c
# ADD CPP /nologo /MDd /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /YX /c
CPP_PROJ=/nologo /MDd /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_CONSOLE"\
 /Fp"test_multiple_contexts.pch" /YX /c 
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
BSC32_FLAGS=/nologo /o"$(OUTDIR)/test_multiple_contexts.bsc" 
BSC32_SBRS=
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /machine:I386
# ADD LINK32 ace.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /machine:I386
LINK32_FLAGS=ace.lib kernel32.lib user32.lib gdi32.lib winspool.lib\
 comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib\
 odbc32.lib odbccp32.lib /nologo /subsystem:console /incremental:no\
 /pdb:"$(OUTDIR)/test_multiple_contexts.pdb" /machine:I386\
 /out:"$(OUTDIR)/test_multiple_contexts.exe" 
LINK32_OBJS= \
	"$(INTDIR)/test_multiple_contexts.obj"

"$(OUTDIR)\test_multiple_contexts.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ELSEIF  "$(CFG)" == "test_multiple_contexts - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
OUTDIR=.
INTDIR=.

ALL : "$(OUTDIR)\test_multiple_contexts.exe"

CLEAN : 
	-@erase ".\test_multiple_contexts.exe"
	-@erase ".\test_multiple_contexts.obj"
	-@erase ".\test_multiple_contexts.ilk"
	-@erase ".\test_multiple_contexts.pdb"
	-@erase ".\vc40.pdb"
	-@erase ".\vc40.idb"

# ADD BASE CPP /nologo /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /YX /c
# ADD CPP /nologo /MDd /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /YX /c
CPP_PROJ=/nologo /MDd /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_CONSOLE"\
 /Fp"test_multiple_contexts.pch" /YX /c 
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
BSC32_FLAGS=/nologo /o"$(OUTDIR)/test_multiple_contexts.bsc" 
BSC32_SBRS=
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /debug /machine:I386
# ADD LINK32 ace.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /debug /machine:I386
LINK32_FLAGS=ace.lib kernel32.lib user32.lib gdi32.lib winspool.lib\
 comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib\
 odbc32.lib odbccp32.lib /nologo /subsystem:console /incremental:yes\
 /pdb:"$(OUTDIR)/test_multiple_contexts.pdb" /debug /machine:I386\
 /out:"$(OUTDIR)/test_multiple_contexts.exe" 
LINK32_OBJS= \
	"$(INTDIR)/test_multiple_contexts.obj"

"$(OUTDIR)\test_multiple_contexts.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ENDIF 

.c.obj:
   $(CPP) $(CPP_PROJ) $<  

.cpp.obj:
   $(CPP) $(CPP_PROJ) $<  

.cxx.obj:
   $(CPP) $(CPP_PROJ) $<  

.c.sbr:
   $(CPP) $(CPP_PROJ) $<  

.cpp.sbr:
   $(CPP) $(CPP_PROJ) $<  

.cxx.sbr:
   $(CPP) $(CPP_PROJ) $<  

################################################################################
# Begin Target

# Name "test_multiple_contexts - Win32 Release"
# Name "test_multiple_contexts - Win32 Debug"

!IF  "$(CFG)" == "test_multiple_contexts - Win32 Release"

!ELSEIF  "$(CFG)" == "test_multiple_contexts - Win32 Debug"

!ENDIF 

################################################################################
# Begin Source File

SOURCE=.\test_multiple_contexts.cpp
DEP_CPP_TEST_=\
	{$(INCLUDE)}"\ace\Naming_Context.h"\
	{$(INCLUDE)}"\ace\ACE.h"\
	{$(INCLUDE)}"\ace\SString.h"\
	{$(INCLUDE)}"\ace\Set.h"\
	{$(INCLUDE)}"\ace\Service_Object.h"\
	{$(INCLUDE)}"\ace\Name_Proxy.h"\
	{$(INCLUDE)}"\ace\Name_Space.h"\
	{$(INCLUDE)}"\ace\OS.h"\
	{$(INCLUDE)}"\ace\ACE.i"\
	{$(INCLUDE)}"\ace\Time_Value.h"\
	{$(INCLUDE)}"\sys\types.h"\
	{$(INCLUDE)}"\sys\stat.h"\
	{$(INCLUDE)}"\sys\timeb.h"\
	{$(INCLUDE)}"\bstring.h"\
	{$(INCLUDE)}"\ace\Trace.h"\
	{$(INCLUDE)}"\ace\OS.i"\
	{$(INCLUDE)}"\ace\config.h"\
	{$(INCLUDE)}"\ace\Time_Value.i"\
	{$(INCLUDE)}"\ace\Log_Msg.h"\
	{$(INCLUDE)}"\ace\Log_Record.h"\
	{$(INCLUDE)}"\ace\Log_Priority.h"\
	{$(INCLUDE)}"\ace\Log_Record.i"\
	{$(INCLUDE)}"\ace\SString.i"\
	{$(INCLUDE)}"\ace\Set.i"\
	{$(INCLUDE)}"\ace\Set.cpp"\
	{$(INCLUDE)}"\ace\Shared_Object.h"\
	{$(INCLUDE)}"\ace\Event_Handler.h"\
	{$(INCLUDE)}"\ace\Service_Object.i"\
	{$(INCLUDE)}"\ace\Shared_Object.i"\
	{$(INCLUDE)}"\ace\Event_Handler.i"\
	{$(INCLUDE)}"\ace\INET_Addr.h"\
	{$(INCLUDE)}"\ace\SOCK_Connector.h"\
	{$(INCLUDE)}"\ace\SOCK_Stream.h"\
	{$(INCLUDE)}"\ace\Service_Config.h"\
	{$(INCLUDE)}"\ace\Synch_Options.h"\
	{$(INCLUDE)}"\ace\Name_Request_Reply.h"\
	{$(INCLUDE)}"\ace\Addr.h"\
	{$(INCLUDE)}"\ace\INET_Addr.i"\
	{$(INCLUDE)}"\ace\Addr.i"\
	{$(INCLUDE)}"\ace\SOCK_Connector.i"\
	{$(INCLUDE)}"\ace\SOCK_IO.h"\
	{$(INCLUDE)}"\ace\SOCK_Stream.i"\
	{$(INCLUDE)}"\ace\SOCK.h"\
	{$(INCLUDE)}"\ace\SOCK_IO.i"\
	{$(INCLUDE)}"\ace\IPC_SAP.h"\
	{$(INCLUDE)}"\ace\SOCK.i"\
	{$(INCLUDE)}"\ace\IPC_SAP.i"\
	{$(INCLUDE)}"\ace\Thread_Manager.h"\
	{$(INCLUDE)}"\ace\Proactor.h"\
	{$(INCLUDE)}"\ace\ReactorEx.h"\
	{$(INCLUDE)}"\ace\Service_Config.i"\
	{$(INCLUDE)}"\ace\Reactor.h"\
	{$(INCLUDE)}"\ace\Svc_Conf_Tokens.h"\
	{$(INCLUDE)}"\ace\Thread.h"\
	{$(INCLUDE)}"\ace\Synch.h"\
	{$(INCLUDE)}"\ace\Thread_Manager.i"\
	{$(INCLUDE)}"\ace\Thread.i"\
	{$(INCLUDE)}"\ace\SV_Semaphore_Complex.h"\
	{$(INCLUDE)}"\ace\Synch.i"\
	{$(INCLUDE)}"\ace\Synch_T.h"\
	{$(INCLUDE)}"\ace\SV_Semaphore_Simple.h"\
	{$(INCLUDE)}"\ace\SV_Semaphore_Complex.i"\
	{$(INCLUDE)}"\ace\SV_Semaphore_Simple.i"\
	{$(INCLUDE)}"\ace\Synch_T.i"\
	{$(INCLUDE)}"\ace\Synch_T.cpp"\
	{$(INCLUDE)}"\ace\Message_Block.h"\
	{$(INCLUDE)}"\ace\Timer_Queue.h"\
	{$(INCLUDE)}"\ace\Proactor.i"\
	{$(INCLUDE)}"\ace\Malloc.h"\
	{$(INCLUDE)}"\ace\Message_Block.i"\
	{$(INCLUDE)}"\ace\Malloc.i"\
	{$(INCLUDE)}"\ace\Malloc_T.h"\
	{$(INCLUDE)}"\ace\Memory_Pool.h"\
	{$(INCLUDE)}"\ace\Malloc_T.i"\
	{$(INCLUDE)}"\ace\Malloc_T.cpp"\
	{$(INCLUDE)}"\ace\Signal.h"\
	{$(INCLUDE)}"\ace\Mem_Map.h"\
	{$(INCLUDE)}"\ace\Memory_Pool.i"\
	{$(INCLUDE)}"\ace\Signal.i"\
	{$(INCLUDE)}"\ace\Mem_Map.i"\
	{$(INCLUDE)}"\ace\Timer_Queue.i"\
	{$(INCLUDE)}"\ace\Token.h"\
	{$(INCLUDE)}"\ace\Local_Tokens.h"\
	{$(INCLUDE)}"\ace\ReactorEx.i"\
	{$(INCLUDE)}"\ace\Token.i"\
	{$(INCLUDE)}"\ace\Stack.h"\
	{$(INCLUDE)}"\ace\Map_Manager.h"\
	{$(INCLUDE)}"\ace\Local_Tokens.i"\
	{$(INCLUDE)}"\ace\Stack.i"\
	{$(INCLUDE)}"\ace\Stack.cpp"\
	{$(INCLUDE)}"\ace\Map_Manager.i"\
	{$(INCLUDE)}"\ace\Map_Manager.cpp"\
	{$(INCLUDE)}"\ace\Handle_Set.h"\
	{$(INCLUDE)}"\ace\Pipe.h"\
	{$(INCLUDE)}"\ace\Reactor.i"\
	{$(INCLUDE)}"\ace\Handle_Set.i"\
	{$(INCLUDE)}"\ace\Pipe.i"\
	
NODEP_CPP_TEST_=\
	"..\..\STL\mutex.h"\
	"..\..\ace\ace\Sync_T.h"\
	

"$(INTDIR)\test_multiple_contexts.obj" : $(SOURCE) $(DEP_CPP_TEST_) "$(INTDIR)"


# End Source File
# End Target
# End Project
################################################################################
