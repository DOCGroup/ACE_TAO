# Microsoft Developer Studio Generated NMAKE File, Based on Pipe_Test.dsp
!IF "$(CFG)" == ""
CFG=Pipe_Test - Win32 Debug
!MESSAGE No configuration specified. Defaulting to Pipe_Test - Win32 Debug.
!ENDIF 

!IF "$(CFG)" != "Pipe_Test - Win32 Debug" && "$(CFG)" !=\
 "Pipe_Test - Win32 Alpha Debug"
!MESSAGE Invalid configuration "$(CFG)" specified.
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "Pipe_Test.mak" CFG="Pipe_Test - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "Pipe_Test - Win32 Debug" (based on "Win32 (x86) Console Application")
!MESSAGE "Pipe_Test - Win32 Alpha Debug" (based on\
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

!IF  "$(CFG)" == "Pipe_Test - Win32 Debug"

OUTDIR=.\.
INTDIR=.\DLL\Debug
# Begin Custom Macros
OutDir=.\.
# End Custom Macros

!IF "$(RECURSE)" == "0" 

ALL : "$(OUTDIR)\Pipe_Test.exe"

!ELSE 

ALL : "$(OUTDIR)\Pipe_Test.exe"

!ENDIF 

CLEAN :
	-@erase "$(INTDIR)\Pipe_Test.obj"
	-@erase "$(INTDIR)\vc50.idb"
	-@erase "$(INTDIR)\vc50.pdb"
	-@erase "$(OUTDIR)\Pipe_Test.exe"
	-@erase "$(OUTDIR)\Pipe_Test.ilk"
	-@erase "$(OUTDIR)\Pipe_Test.pdb"

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
BSC32_FLAGS=/nologo /o"$(OUTDIR)\Pipe_Test.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
LINK32_FLAGS=aced.lib /nologo /subsystem:console /incremental:yes\
 /pdb:"$(OUTDIR)\Pipe_Test.pdb" /debug /machine:I386\
 /out:"$(OUTDIR)\Pipe_Test.exe" /libpath:"..\ace" 
LINK32_OBJS= \
	"$(INTDIR)\Pipe_Test.obj"

"$(OUTDIR)\Pipe_Test.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ELSEIF  "$(CFG)" == "Pipe_Test - Win32 Alpha Debug"

OUTDIR=.
INTDIR=.\Debug
# Begin Custom Macros
OutDir=.
# End Custom Macros

!IF "$(RECURSE)" == "0" 

ALL : "$(OUTDIR)\Pipe_Test.exe"

!ELSE 

ALL : "$(OUTDIR)\Pipe_Test.exe"

!ENDIF 

CLEAN :
	-@erase "$(INTDIR)\Pipe_Test.obj"
	-@erase "$(INTDIR)\vc50.idb"
	-@erase "$(INTDIR)\vc50.pdb"
	-@erase "$(OUTDIR)\Pipe_Test.exe"
	-@erase "$(OUTDIR)\Pipe_Test.pdb"

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
BSC32_FLAGS=/nologo /o"$(OUTDIR)\Pipe_Test.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
LINK32_FLAGS=aced.lib /nologo /subsystem:console /pdb:"$(OUTDIR)\Pipe_Test.pdb"\
 /debug /machine:ALPHA /out:"$(OUTDIR)\Pipe_Test.exe" /libpath:"..\ace" 
LINK32_OBJS= \
	"$(INTDIR)\Pipe_Test.obj"

"$(OUTDIR)\Pipe_Test.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ENDIF 


!IF "$(CFG)" == "Pipe_Test - Win32 Debug" || "$(CFG)" ==\
 "Pipe_Test - Win32 Alpha Debug"
SOURCE=.\Pipe_Test.cpp

!IF  "$(CFG)" == "Pipe_Test - Win32 Debug"

DEP_CPP_PIPE_=\
	"..\ace\ACE.h"\
	"..\ace\ACE.i"\
	"..\ace\Auto_Ptr.cpp"\
	"..\ace\Auto_Ptr.h"\
	"..\ace\Auto_Ptr.i"\
	"..\ace\Basic_Types.h"\
	"..\ace\Basic_Types.i"\
	"..\ace\config-win32-common.h"\
	"..\ace\config-win32.h"\
	"..\ace\config.h"\
	"..\ace\Get_Opt.h"\
	"..\ace\Get_Opt.i"\
	"..\ace\inc_user_config.h"\
	"..\ace\iosfwd.h"\
	"..\ace\Log_Msg.h"\
	"..\ace\Log_Priority.h"\
	"..\ace\Log_Record.h"\
	"..\ace\Log_Record.i"\
	"..\ace\Managed_Object.cpp"\
	"..\ace\Managed_Object.h"\
	"..\ace\Managed_Object.i"\
	"..\ace\Object_Manager.h"\
	"..\ace\Object_Manager.i"\
	"..\ace\OS.h"\
	"..\ace\OS.i"\
	"..\ace\Pipe.h"\
	"..\ace\Pipe.i"\
	"..\ace\Process.h"\
	"..\ace\Process.i"\
	"..\ace\SString.h"\
	"..\ace\SString.i"\
	"..\ace\streams.h"\
	"..\ace\sys_conf.h"\
	"..\ace\Trace.h"\
	"..\ace\Version.h"\
	"..\ace\ws2tcpip.h"\
	".\test_config.h"\
	{$(INCLUDE)}"sys\stat.h"\
	{$(INCLUDE)}"sys\types.h"\
	

"$(INTDIR)\Pipe_Test.obj" : $(SOURCE) $(DEP_CPP_PIPE_) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "Pipe_Test - Win32 Alpha Debug"

DEP_CPP_PIPE_=\
	"..\ace\ACE.h"\
	"..\ace\ACE.i"\
	"..\ace\Auto_Ptr.cpp"\
	"..\ace\Auto_Ptr.h"\
	"..\ace\Auto_Ptr.i"\
	"..\ace\Basic_Types.h"\
	"..\ace\Basic_Types.i"\
	"..\ace\config-win32-common.h"\
	"..\ace\config-win32.h"\
	"..\ace\config.h"\
	"..\ace\Get_Opt.h"\
	"..\ace\Get_Opt.i"\
	"..\ace\inc_user_config.h"\
	"..\ace\iosfwd.h"\
	"..\ace\Log_Msg.h"\
	"..\ace\Log_Priority.h"\
	"..\ace\Log_Record.h"\
	"..\ace\Log_Record.i"\
	"..\ace\Managed_Object.cpp"\
	"..\ace\Managed_Object.h"\
	"..\ace\Managed_Object.i"\
	"..\ace\Object_Manager.h"\
	"..\ace\Object_Manager.i"\
	"..\ace\OS.h"\
	"..\ace\OS.i"\
	"..\ace\Pipe.h"\
	"..\ace\Pipe.i"\
	"..\ace\Process.h"\
	"..\ace\Process.i"\
	"..\ace\SString.h"\
	"..\ace\SString.i"\
	"..\ace\streams.h"\
	"..\ace\sys_conf.h"\
	"..\ace\Trace.h"\
	"..\ace\Version.h"\
	"..\ace\ws2tcpip.h"\
	".\test_config.h"\
	{$(INCLUDE)}"sys\stat.h"\
	{$(INCLUDE)}"sys\types.h"\
	

"$(INTDIR)\Pipe_Test.obj" : $(SOURCE) $(DEP_CPP_PIPE_) "$(INTDIR)"


!ENDIF 


!ENDIF 

