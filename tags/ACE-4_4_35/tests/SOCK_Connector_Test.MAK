# Microsoft Developer Studio Generated NMAKE File, Based on SOCK_Connector_Test.dsp
!IF "$(CFG)" == ""
CFG=SOCK_Connector_Test - Win32 Debug
!MESSAGE No configuration specified. Defaulting to SOCK_Connector_Test - Win32\
 Debug.
!ENDIF 

!IF "$(CFG)" != "SOCK_Connector_Test - Win32 Debug"
!MESSAGE Invalid configuration "$(CFG)" specified.
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "SOCK_Connector_Test.mak"\
 CFG="SOCK_Connector_Test - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "SOCK_Connector_Test - Win32 Debug" (based on\
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
OUTDIR=.\.
INTDIR=.\DLL\Debug
# Begin Custom Macros
OutDir=.\.
# End Custom Macros

!IF "$(RECURSE)" == "0" 

ALL : "$(OUTDIR)\SOCK_Connector_Test.exe"

!ELSE 

ALL : "$(OUTDIR)\SOCK_Connector_Test.exe"

!ENDIF 

CLEAN :
	-@erase "$(INTDIR)\SOCK_Connector_Test.obj"
	-@erase "$(INTDIR)\vc50.idb"
	-@erase "$(INTDIR)\vc50.pdb"
	-@erase "$(OUTDIR)\SOCK_Connector_Test.exe"
	-@erase "$(OUTDIR)\SOCK_Connector_Test.ilk"
	-@erase "$(OUTDIR)\SOCK_Connector_Test.pdb"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

"$(INTDIR)" :
    if not exist "$(INTDIR)/$(NULL)" mkdir "$(INTDIR)"

BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\SOCK_Connector_Test.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
LINK32_FLAGS=aced.lib /nologo /subsystem:console /incremental:yes\
 /pdb:"$(OUTDIR)\SOCK_Connector_Test.pdb" /debug /machine:I386\
 /out:"$(OUTDIR)\SOCK_Connector_Test.exe" /libpath:"..\ace" 
LINK32_OBJS= \
	"$(INTDIR)\SOCK_Connector_Test.obj"

"$(OUTDIR)\SOCK_Connector_Test.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

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


!IF "$(CFG)" == "SOCK_Connector_Test - Win32 Debug"
SOURCE=.\SOCK_Connector_Test.cpp
DEP_CPP_SOCK_=\
	"..\ace\ACE.h"\
	"..\ace\ACE.i"\
	"..\ace\Addr.h"\
	"..\ace\Addr.i"\
	"..\ace\Auto_Ptr.cpp"\
	"..\ace\Auto_Ptr.h"\
	"..\ace\Auto_Ptr.i"\
	"..\ace\Basic_Types.h"\
	"..\ace\Basic_Types.i"\
	"..\ace\config-win32-common.h"\
	"..\ace\config-win32.h"\
	"..\ace\config.h"\
	"..\ace\inc_user_config.h"\
	"..\ace\INET_Addr.h"\
	"..\ace\INET_Addr.i"\
	"..\ace\iosfwd.h"\
	"..\ace\IPC_SAP.h"\
	"..\ace\IPC_SAP.i"\
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
	"..\ace\SOCK.h"\
	"..\ace\SOCK.i"\
	"..\ace\SOCK_Connector.h"\
	"..\ace\SOCK_Connector.i"\
	"..\ace\SOCK_IO.h"\
	"..\ace\SOCK_IO.i"\
	"..\ace\SOCK_Stream.h"\
	"..\ace\SOCK_Stream.i"\
	"..\ace\SString.h"\
	"..\ace\SString.i"\
	"..\ace\streams.h"\
	"..\ace\sys_conf.h"\
	"..\ace\Time_Value.h"\
	"..\ace\Trace.h"\
	"..\ace\Version.h"\
	"..\ace\ws2tcpip.h"\
	".\test_config.h"\
	{$(INCLUDE)}"sys\stat.h"\
	{$(INCLUDE)}"sys\types.h"\
	

"$(INTDIR)\SOCK_Connector_Test.obj" : $(SOURCE) $(DEP_CPP_SOCK_) "$(INTDIR)"



!ENDIF 

