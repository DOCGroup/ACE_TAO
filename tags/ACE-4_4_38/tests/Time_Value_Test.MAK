# Microsoft Developer Studio Generated NMAKE File, Based on Time_Value_Test.dsp
!IF "$(CFG)" == ""
CFG=Time_Value_Test - Win32 Debug
!MESSAGE No configuration specified. Defaulting to Time_Value_Test - Win32\
 Debug.
!ENDIF 

!IF "$(CFG)" != "Time_Value_Test - Win32 Debug" && "$(CFG)" !=\
 "Time_Value_Test - Win32 Alpha Debug"
!MESSAGE Invalid configuration "$(CFG)" specified.
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "Time_Value_Test.mak" CFG="Time_Value_Test - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "Time_Value_Test - Win32 Debug" (based on\
 "Win32 (x86) Console Application")
!MESSAGE "Time_Value_Test - Win32 Alpha Debug" (based on\
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

!IF  "$(CFG)" == "Time_Value_Test - Win32 Debug"

OUTDIR=.\.
INTDIR=.\DLL\Debug
# Begin Custom Macros
OutDir=.\.
# End Custom Macros

!IF "$(RECURSE)" == "0" 

ALL : "$(OUTDIR)\Time_Value_Test.exe"

!ELSE 

ALL : "$(OUTDIR)\Time_Value_Test.exe"

!ENDIF 

CLEAN :
	-@erase "$(INTDIR)\Time_Value_Test.obj"
	-@erase "$(INTDIR)\vc50.idb"
	-@erase "$(INTDIR)\vc50.pdb"
	-@erase "$(OUTDIR)\Time_Value_Test.exe"
	-@erase "$(OUTDIR)\Time_Value_Test.ilk"
	-@erase "$(OUTDIR)\Time_Value_Test.pdb"

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
BSC32_FLAGS=/nologo /o"$(OUTDIR)\Time_Value_Test.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
LINK32_FLAGS=aced.lib /nologo /subsystem:console /incremental:yes\
 /pdb:"$(OUTDIR)\Time_Value_Test.pdb" /debug /machine:I386\
 /out:"$(OUTDIR)\Time_Value_Test.exe" /libpath:"..\ace" 
LINK32_OBJS= \
	"$(INTDIR)\Time_Value_Test.obj"

"$(OUTDIR)\Time_Value_Test.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ELSEIF  "$(CFG)" == "Time_Value_Test - Win32 Alpha Debug"

OUTDIR=.
INTDIR=.\Debug
# Begin Custom Macros
OutDir=.
# End Custom Macros

!IF "$(RECURSE)" == "0" 

ALL : "$(OUTDIR)\Time_Value_Test.exe"

!ELSE 

ALL : "$(OUTDIR)\Time_Value_Test.exe"

!ENDIF 

CLEAN :
	-@erase "$(INTDIR)\Time_Value_Test.obj"
	-@erase "$(INTDIR)\vc50.idb"
	-@erase "$(INTDIR)\vc50.pdb"
	-@erase "$(OUTDIR)\Time_Value_Test.exe"
	-@erase "$(OUTDIR)\Time_Value_Test.pdb"

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
BSC32_FLAGS=/nologo /o"$(OUTDIR)\Time_Value_Test.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
LINK32_FLAGS=aced.lib /nologo /subsystem:console\
 /pdb:"$(OUTDIR)\Time_Value_Test.pdb" /debug /machine:ALPHA\
 /out:"$(OUTDIR)\Time_Value_Test.exe" /libpath:"..\ace" 
LINK32_OBJS= \
	"$(INTDIR)\Time_Value_Test.obj"

"$(OUTDIR)\Time_Value_Test.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ENDIF 


!IF "$(CFG)" == "Time_Value_Test - Win32 Debug" || "$(CFG)" ==\
 "Time_Value_Test - Win32 Alpha Debug"
SOURCE=.\Time_Value_Test.cpp

!IF  "$(CFG)" == "Time_Value_Test - Win32 Debug"

DEP_CPP_TIME_=\
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
	

"$(INTDIR)\Time_Value_Test.obj" : $(SOURCE) $(DEP_CPP_TIME_) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "Time_Value_Test - Win32 Alpha Debug"

DEP_CPP_TIME_=\
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
	

"$(INTDIR)\Time_Value_Test.obj" : $(SOURCE) $(DEP_CPP_TIME_) "$(INTDIR)"


!ENDIF 


!ENDIF 

