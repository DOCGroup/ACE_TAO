# Microsoft Developer Studio Generated NMAKE File, Based on Env_Value_Test.dsp
!IF "$(CFG)" == ""
CFG=Env_Value_Test - Win32 Debug
!MESSAGE No configuration specified. Defaulting to Env_Value_Test - Win32\
 Debug.
!ENDIF 

!IF "$(CFG)" != "Env_Value_Test - Win32 Debug"
!MESSAGE Invalid configuration "$(CFG)" specified.
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "Env_Value_Test.mak" CFG="Env_Value_Test - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "Env_Value_Test - Win32 Debug" (based on\
 "Win32 (x86) Console Application")
!MESSAGE 
!ERROR An invalid configuration is specified.
!ENDIF 

!IF "$(OS)" == "Windows_NT"
NULL=
!ELSE 
NULL=nul
!ENDIF 

CPP=cl.exe
RSC=rc.exe
OUTDIR=.
INTDIR=.\DLL\Debug
# Begin Custom Macros
OutDir=.
# End Custom Macros

!IF "$(RECURSE)" == "0" 

ALL : "$(OUTDIR)\Env_Value_Test.exe"

!ELSE 

ALL : "$(OUTDIR)\Env_Value_Test.exe"

!ENDIF 

CLEAN :
	-@erase "$(INTDIR)\Env_Value_Test.obj"
	-@erase "$(INTDIR)\vc50.idb"
	-@erase "$(INTDIR)\vc50.pdb"
	-@erase "$(OUTDIR)\Env_Value_Test.exe"
	-@erase "$(OUTDIR)\Env_Value_Test.ilk"
	-@erase "$(OUTDIR)\Env_Value_Test.pdb"

"$(INTDIR)" :
    if not exist "$(INTDIR)/$(NULL)" mkdir "$(INTDIR)"

BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\Env_Value_Test.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
LINK32_FLAGS=aced.lib /nologo /subsystem:console /incremental:yes\
 /pdb:"$(OUTDIR)\Env_Value_Test.pdb" /debug /machine:I386\
 /out:"$(OUTDIR)\Env_Value_Test.exe" /pdbtype:sept /libpath:"..\ace" 
LINK32_OBJS= \
	"$(INTDIR)\Env_Value_Test.obj"

"$(OUTDIR)\Env_Value_Test.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

CPP_PROJ=/nologo /MDd /W3 /Gm /GX /Zi /Od /I "..\\" /D "WIN32" /D "_DEBUG" /D\
 "_CONSOLE" /Fp"$(INTDIR)\Env_Value_Test.pch" /YX /Fo"$(INTDIR)\\"\
 /Fd"$(INTDIR)\\" /FD /c 
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


!IF "$(CFG)" == "Env_Value_Test - Win32 Debug"
SOURCE=.\Env_Value_Test.cpp
DEP_CPP_ENV_V=\
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
	"..\ace\Env_Value_T.cpp"\
	"..\ace\Env_Value_T.h"\
	"..\ace\Env_Value_T.i"\
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
	

"$(INTDIR)\Env_Value_Test.obj" : $(SOURCE) $(DEP_CPP_ENV_V) "$(INTDIR)"



!ENDIF 

