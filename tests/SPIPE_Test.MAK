# Microsoft Developer Studio Generated NMAKE File, Based on SPIPE_Test.dsp
!IF "$(CFG)" == ""
CFG=SPIPE_Test - Win32 Debug
!MESSAGE No configuration specified. Defaulting to SPIPE_Test - Win32 Debug.
!ENDIF 

!IF "$(CFG)" != "SPIPE_Test - Win32 Debug" && "$(CFG)" !=\
 "SPIPE_Test - Win32 Alpha Debug"
!MESSAGE Invalid configuration "$(CFG)" specified.
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "SPIPE_Test.mak" CFG="SPIPE_Test - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "SPIPE_Test - Win32 Debug" (based on\
 "Win32 (x86) Console Application")
!MESSAGE "SPIPE_Test - Win32 Alpha Debug" (based on\
 "Win32 (ALPHA) Console Application")
!MESSAGE 
!ERROR An invalid configuration is specified.
!ENDIF 

!IF "$(OS)" == "Windows_NT"
NULL=
!ELSE 
NULL=nul
!ENDIF 

RSC=rc.exe

!IF  "$(CFG)" == "SPIPE_Test - Win32 Debug"

OUTDIR=.\.
INTDIR=.\DLL\Debug
# Begin Custom Macros
OutDir=.\.
# End Custom Macros

!IF "$(RECURSE)" == "0" 

ALL : "$(OUTDIR)\SPIPE_Test.exe"

!ELSE 

ALL : "$(OUTDIR)\SPIPE_Test.exe"

!ENDIF 

CLEAN :
	-@erase "$(INTDIR)\SPIPE_Test.obj"
	-@erase "$(INTDIR)\vc50.idb"
	-@erase "$(INTDIR)\vc50.pdb"
	-@erase "$(OUTDIR)\SPIPE_Test.exe"
	-@erase "$(OUTDIR)\SPIPE_Test.ilk"
	-@erase "$(OUTDIR)\SPIPE_Test.pdb"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

"$(INTDIR)" :
    if not exist "$(INTDIR)/$(NULL)" mkdir "$(INTDIR)"

CPP=cl.exe
CPP_PROJ=/nologo /MDd /W3 /Gm /GX /Zi /Od /I "..\\" /D "WIN32" /D "_DEBUG" /D\
 "_CONSOLE" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 
CPP_OBJS=.\DLL\Debug/
CPP_SBRS=.

.c{$(CPP_OBJS)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cpp{$(CPP_OBJS)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cxx{$(CPP_OBJS)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.c{$(CPP_SBRS)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cpp{$(CPP_SBRS)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cxx{$(CPP_SBRS)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\SPIPE_Test.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
LINK32_FLAGS=aced.lib /nologo /subsystem:console /incremental:yes\
 /pdb:"$(OUTDIR)\SPIPE_Test.pdb" /debug /machine:I386\
 /out:"$(OUTDIR)\SPIPE_Test.exe" /libpath:"..\ace" 
LINK32_OBJS= \
	"$(INTDIR)\SPIPE_Test.obj"

"$(OUTDIR)\SPIPE_Test.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ELSEIF  "$(CFG)" == "SPIPE_Test - Win32 Alpha Debug"

OUTDIR=.
INTDIR=.\Debug
# Begin Custom Macros
OutDir=.
# End Custom Macros

!IF "$(RECURSE)" == "0" 

ALL : "$(OUTDIR)\SPIPE_Test.exe"

!ELSE 

ALL : "$(OUTDIR)\SPIPE_Test.exe"

!ENDIF 

CLEAN :
	-@erase "$(INTDIR)\SPIPE_Test.obj"
	-@erase "$(INTDIR)\vc50.idb"
	-@erase "$(INTDIR)\vc50.pdb"
	-@erase "$(OUTDIR)\SPIPE_Test.exe"
	-@erase "$(OUTDIR)\SPIPE_Test.pdb"

"$(INTDIR)" :
    if not exist "$(INTDIR)/$(NULL)" mkdir "$(INTDIR)"

CPP=cl.exe
CPP_PROJ=/nologo /Gt0 /W3 /GX /Zi /Od /I "..\\" /D "WIN32" /D "_DEBUG" /D\
 "_CONSOLE" /D "_MBCS" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /MDd /c 
CPP_OBJS=.\Debug/
CPP_SBRS=.

.c{$(CPP_OBJS)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cpp{$(CPP_OBJS)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cxx{$(CPP_OBJS)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.c{$(CPP_SBRS)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cpp{$(CPP_SBRS)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cxx{$(CPP_SBRS)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\SPIPE_Test.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
LINK32_FLAGS=aced.lib /nologo /subsystem:console\
 /pdb:"$(OUTDIR)\SPIPE_Test.pdb" /debug /machine:ALPHA\
 /out:"$(OUTDIR)\SPIPE_Test.exe" /libpath:"..\ace" 
LINK32_OBJS= \
	"$(INTDIR)\SPIPE_Test.obj"

"$(OUTDIR)\SPIPE_Test.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ENDIF 


!IF "$(CFG)" == "SPIPE_Test - Win32 Debug" || "$(CFG)" ==\
 "SPIPE_Test - Win32 Alpha Debug"
SOURCE=.\SPIPE_Test.cpp

!IF  "$(CFG)" == "SPIPE_Test - Win32 Debug"

DEP_CPP_SPIPE=\
	"..\ace\ACE.h"\
	"..\ace\ACE.i"\
	"..\ace\Addr.h"\
	"..\ace\Addr.i"\
	"..\ace\Atomic_Op.i"\
	"..\ace\Auto_Ptr.cpp"\
	"..\ace\Auto_Ptr.h"\
	"..\ace\Auto_Ptr.i"\
	"..\ace\Basic_Types.h"\
	"..\ace\Basic_Types.i"\
	"..\ace\config-win32-common.h"\
	"..\ace\config-win32.h"\
	"..\ace\config.h"\
	"..\ace\Containers.cpp"\
	"..\ace\Containers.h"\
	"..\ace\Containers.i"\
	"..\ace\Event_Handler.h"\
	"..\ace\Event_Handler.i"\
	"..\ace\Free_List.cpp"\
	"..\ace\Free_List.h"\
	"..\ace\Free_List.i"\
	"..\ace\inc_user_config.h"\
	"..\ace\iosfwd.h"\
	"..\ace\IPC_SAP.h"\
	"..\ace\IPC_SAP.i"\
	"..\ace\Log_Msg.h"\
	"..\ace\Log_Priority.h"\
	"..\ace\Log_Record.h"\
	"..\ace\Log_Record.i"\
	"..\ace\Malloc.h"\
	"..\ace\Malloc.i"\
	"..\ace\Malloc_T.cpp"\
	"..\ace\Malloc_T.h"\
	"..\ace\Malloc_T.i"\
	"..\ace\Managed_Object.cpp"\
	"..\ace\Managed_Object.h"\
	"..\ace\Managed_Object.i"\
	"..\ace\Mem_Map.h"\
	"..\ace\Mem_Map.i"\
	"..\ace\Memory_Pool.h"\
	"..\ace\Memory_Pool.i"\
	"..\ace\Object_Manager.h"\
	"..\ace\Object_Manager.i"\
	"..\ace\OS.h"\
	"..\ace\OS.i"\
	"..\ace\Signal.h"\
	"..\ace\Signal.i"\
	"..\ace\SPIPE.h"\
	"..\ace\SPIPE.i"\
	"..\ace\SPIPE_Acceptor.h"\
	"..\ace\SPIPE_Addr.h"\
	"..\ace\SPIPE_Addr.i"\
	"..\ace\SPIPE_Connector.h"\
	"..\ace\SPIPE_Connector.i"\
	"..\ace\SPIPE_Stream.h"\
	"..\ace\SPIPE_Stream.i"\
	"..\ace\SString.h"\
	"..\ace\SString.i"\
	"..\ace\streams.h"\
	"..\ace\SV_Semaphore_Complex.h"\
	"..\ace\SV_Semaphore_Complex.i"\
	"..\ace\SV_Semaphore_Simple.h"\
	"..\ace\SV_Semaphore_Simple.i"\
	"..\ace\Synch.h"\
	"..\ace\Synch.i"\
	"..\ace\Synch_T.cpp"\
	"..\ace\Synch_T.h"\
	"..\ace\Synch_T.i"\
	"..\ace\sys_conf.h"\
	"..\ace\Thread.h"\
	"..\ace\Thread.i"\
	"..\ace\Thread_Manager.h"\
	"..\ace\Thread_Manager.i"\
	"..\ace\Trace.h"\
	"..\ace\Version.h"\
	"..\ace\ws2tcpip.h"\
	".\test_config.h"\
	{$(INCLUDE)}"sys\stat.h"\
	{$(INCLUDE)}"sys\types.h"\
	

"$(INTDIR)\SPIPE_Test.obj" : $(SOURCE) $(DEP_CPP_SPIPE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "SPIPE_Test - Win32 Alpha Debug"

DEP_CPP_SPIPE=\
	"..\ace\ACE.h"\
	"..\ace\ACE.i"\
	"..\ace\Addr.h"\
	"..\ace\Addr.i"\
	"..\ace\Atomic_Op.i"\
	"..\ace\Auto_Ptr.cpp"\
	"..\ace\Auto_Ptr.h"\
	"..\ace\Auto_Ptr.i"\
	"..\ace\Basic_Types.h"\
	"..\ace\Basic_Types.i"\
	"..\ace\config-win32-common.h"\
	"..\ace\config-win32.h"\
	"..\ace\config.h"\
	"..\ace\Containers.cpp"\
	"..\ace\Containers.h"\
	"..\ace\Containers.i"\
	"..\ace\Event_Handler.h"\
	"..\ace\Event_Handler.i"\
	"..\ace\Free_List.cpp"\
	"..\ace\Free_List.h"\
	"..\ace\Free_List.i"\
	"..\ace\inc_user_config.h"\
	"..\ace\iosfwd.h"\
	"..\ace\IPC_SAP.h"\
	"..\ace\IPC_SAP.i"\
	"..\ace\Log_Msg.h"\
	"..\ace\Log_Priority.h"\
	"..\ace\Log_Record.h"\
	"..\ace\Log_Record.i"\
	"..\ace\Malloc.h"\
	"..\ace\Malloc.i"\
	"..\ace\Malloc_T.cpp"\
	"..\ace\Malloc_T.h"\
	"..\ace\Malloc_T.i"\
	"..\ace\Managed_Object.cpp"\
	"..\ace\Managed_Object.h"\
	"..\ace\Managed_Object.i"\
	"..\ace\Mem_Map.h"\
	"..\ace\Mem_Map.i"\
	"..\ace\Memory_Pool.h"\
	"..\ace\Memory_Pool.i"\
	"..\ace\Object_Manager.h"\
	"..\ace\Object_Manager.i"\
	"..\ace\OS.h"\
	"..\ace\OS.i"\
	"..\ace\Signal.h"\
	"..\ace\Signal.i"\
	"..\ace\SPIPE.h"\
	"..\ace\SPIPE.i"\
	"..\ace\SPIPE_Acceptor.h"\
	"..\ace\SPIPE_Addr.h"\
	"..\ace\SPIPE_Addr.i"\
	"..\ace\SPIPE_Connector.h"\
	"..\ace\SPIPE_Connector.i"\
	"..\ace\SPIPE_Stream.h"\
	"..\ace\SPIPE_Stream.i"\
	"..\ace\SString.h"\
	"..\ace\SString.i"\
	"..\ace\streams.h"\
	"..\ace\SV_Semaphore_Complex.h"\
	"..\ace\SV_Semaphore_Complex.i"\
	"..\ace\SV_Semaphore_Simple.h"\
	"..\ace\SV_Semaphore_Simple.i"\
	"..\ace\Synch.h"\
	"..\ace\Synch.i"\
	"..\ace\Synch_T.cpp"\
	"..\ace\Synch_T.h"\
	"..\ace\Synch_T.i"\
	"..\ace\sys_conf.h"\
	"..\ace\Thread.h"\
	"..\ace\Thread.i"\
	"..\ace\Thread_Manager.h"\
	"..\ace\Thread_Manager.i"\
	"..\ace\Trace.h"\
	"..\ace\Version.h"\
	"..\ace\ws2tcpip.h"\
	".\test_config.h"\
	{$(INCLUDE)}"sys\stat.h"\
	{$(INCLUDE)}"sys\types.h"\
	

"$(INTDIR)\SPIPE_Test.obj" : $(SOURCE) $(DEP_CPP_SPIPE) "$(INTDIR)"


!ENDIF 


!ENDIF 

