# Microsoft Developer Studio Generated NMAKE File, Based on Barrier_Test.dsp
!IF "$(CFG)" == ""
CFG=Barrier_Test - Win32 Debug
!MESSAGE No configuration specified. Defaulting to Barrier_Test - Win32 Debug.
!ENDIF 

!IF "$(CFG)" != "Barrier_Test - Win32 Debug" && "$(CFG)" !=\
 "Barrier_Test - Win32 Alpha Debug"
!MESSAGE Invalid configuration "$(CFG)" specified.
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "Barrier_Test.mak" CFG="Barrier_Test - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "Barrier_Test - Win32 Debug" (based on\
 "Win32 (x86) Console Application")
!MESSAGE "Barrier_Test - Win32 Alpha Debug" (based on\
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

!IF  "$(CFG)" == "Barrier_Test - Win32 Debug"

OUTDIR=.\.
INTDIR=.\DLL\Debug
# Begin Custom Macros
OutDir=.\.
# End Custom Macros

!IF "$(RECURSE)" == "0" 

ALL : "$(OUTDIR)\Barrier_Test.exe"

!ELSE 

ALL : "$(OUTDIR)\Barrier_Test.exe"

!ENDIF 

CLEAN :
	-@erase "$(INTDIR)\Barrier_Test.obj"
	-@erase "$(INTDIR)\vc50.idb"
	-@erase "$(INTDIR)\vc50.pdb"
	-@erase "$(OUTDIR)\Barrier_Test.exe"
	-@erase "$(OUTDIR)\Barrier_Test.ilk"
	-@erase "$(OUTDIR)\Barrier_Test.pdb"

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
BSC32_FLAGS=/nologo /o"$(OUTDIR)\Barrier_Test.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
LINK32_FLAGS=aced.lib /nologo /subsystem:console /incremental:yes\
 /pdb:"$(OUTDIR)\Barrier_Test.pdb" /debug /machine:I386\
 /out:"$(OUTDIR)\Barrier_Test.exe" /libpath:"..\ace" 
LINK32_OBJS= \
	"$(INTDIR)\Barrier_Test.obj"

"$(OUTDIR)\Barrier_Test.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ELSEIF  "$(CFG)" == "Barrier_Test - Win32 Alpha Debug"

OUTDIR=.
INTDIR=.\Debug
# Begin Custom Macros
OutDir=.
# End Custom Macros

!IF "$(RECURSE)" == "0" 

ALL : "$(OUTDIR)\Barrier_Test.exe"

!ELSE 

ALL : "$(OUTDIR)\Barrier_Test.exe"

!ENDIF 

CLEAN :
	-@erase "$(INTDIR)\Barrier_Test.obj"
	-@erase "$(INTDIR)\vc50.idb"
	-@erase "$(INTDIR)\vc50.pdb"
	-@erase "$(OUTDIR)\Barrier_Test.exe"
	-@erase "$(OUTDIR)\Barrier_Test.pdb"

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
BSC32_FLAGS=/nologo /o"$(OUTDIR)\Barrier_Test.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
LINK32_FLAGS=aced.lib /nologo /subsystem:console\
 /pdb:"$(OUTDIR)\Barrier_Test.pdb" /debug /machine:ALPHA\
 /out:"$(OUTDIR)\Barrier_Test.exe" /libpath:"..\ace" 
LINK32_OBJS= \
	"$(INTDIR)\Barrier_Test.obj"

"$(OUTDIR)\Barrier_Test.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ENDIF 


!IF "$(CFG)" == "Barrier_Test - Win32 Debug" || "$(CFG)" ==\
 "Barrier_Test - Win32 Alpha Debug"
SOURCE=.\Barrier_Test.cpp

!IF  "$(CFG)" == "Barrier_Test - Win32 Debug"

DEP_CPP_BARRI=\
	"..\ace\ACE.h"\
	"..\ace\ACE.i"\
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
	

"$(INTDIR)\Barrier_Test.obj" : $(SOURCE) $(DEP_CPP_BARRI) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "Barrier_Test - Win32 Alpha Debug"

DEP_CPP_BARRI=\
	"..\ace\ACE.h"\
	"..\ace\ACE.i"\
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
	

"$(INTDIR)\Barrier_Test.obj" : $(SOURCE) $(DEP_CPP_BARRI) "$(INTDIR)"


!ENDIF 


!ENDIF 

