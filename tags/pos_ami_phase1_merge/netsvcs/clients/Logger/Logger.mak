# Microsoft Developer Studio Generated NMAKE File, Format Version 4.20
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Console Application" 0x0103

!IF "$(CFG)" == ""
CFG=Indirect - Win32 Debug
!MESSAGE No configuration specified.  Defaulting to Indirect - Win32 Debug.
!ENDIF 

!IF "$(CFG)" != "Direct - Win32 Debug" && "$(CFG)" != "Indirect - Win32 Debug"
!MESSAGE Invalid configuration "$(CFG)" specified.
!MESSAGE You can specify a configuration when running NMAKE on this makefile
!MESSAGE by defining the macro CFG on the command line.  For example:
!MESSAGE 
!MESSAGE NMAKE /f "Logger.mak" CFG="Indirect - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "Direct - Win32 Debug" (based on "Win32 (x86) Console Application")
!MESSAGE "Indirect - Win32 Debug" (based on "Win32 (x86) Console Application")
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
# PROP Target_Last_Scanned "Indirect - Win32 Debug"
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "Direct - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Direct\Debug"
# PROP BASE Intermediate_Dir "Direct\Debug"
# PROP BASE Target_Dir "Direct"
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir ""
# PROP Intermediate_Dir "Debug"
# PROP Target_Dir "Direct"
OUTDIR=.
INTDIR=.\Debug

ALL : "$(OUTDIR)\Direct.exe"

CLEAN : 
	-@erase "$(INTDIR)\direct_logging.obj"
	-@erase "$(INTDIR)\vc40.idb"
	-@erase "$(INTDIR)\vc40.pdb"
	-@erase "$(OUTDIR)\Direct.exe"
	-@erase "$(OUTDIR)\Direct.ilk"
	-@erase "$(OUTDIR)\Direct.pdb"

"$(INTDIR)" :
    if not exist "$(INTDIR)/$(NULL)" mkdir "$(INTDIR)"

# ADD BASE CPP /nologo /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /YX /c
# ADD CPP /nologo /MDd /W3 /Gm /GX /Zi /Od /I "..\..\.." /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /YX /c
CPP_OBJS=.\Debug/
CPP_SBRS=.\.
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
BSC32_FLAGS=/nologo /o"$(OUTDIR)/Direct.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /debug /machine:I386
# ADD LINK32 ..\..\..\ace\aced.lib /nologo /subsystem:console /debug /machine:I386
LINK32_FLAGS=..\..\..\ace\aced.lib /nologo /subsystem:console /incremental:yes\
 /pdb:"$(OUTDIR)/Direct.pdb" /debug /machine:I386 /out:"$(OUTDIR)/Direct.exe" 
LINK32_OBJS= \
	"$(INTDIR)\direct_logging.obj"

"$(OUTDIR)\Direct.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ELSEIF  "$(CFG)" == "Indirect - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Indirect\Debug"
# PROP BASE Intermediate_Dir "Indirect\Debug"
# PROP BASE Target_Dir "Indirect"
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "."
# PROP Intermediate_Dir "Debug"
# PROP Target_Dir "Indirect"
OUTDIR=.\.
INTDIR=.\Debug

ALL : "$(OUTDIR)\Indirect.exe"

CLEAN : 
	-@erase "$(INTDIR)\indirect_logging.obj"
	-@erase "$(INTDIR)\vc40.idb"
	-@erase "$(INTDIR)\vc40.pdb"
	-@erase "$(OUTDIR)\Indirect.exe"
	-@erase "$(OUTDIR)\Indirect.ilk"
	-@erase "$(OUTDIR)\Indirect.pdb"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

"$(INTDIR)" :
    if not exist "$(INTDIR)/$(NULL)" mkdir "$(INTDIR)"

# ADD BASE CPP /nologo /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /YX /c
# ADD CPP /nologo /MDd /W3 /Gm /GX /Zi /Od /I "..\..\.." /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /YX /c
CPP_OBJS=.\Debug/
CPP_SBRS=.\.
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
BSC32_FLAGS=/nologo /o"$(OUTDIR)/Indirect.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /debug /machine:I386
# ADD LINK32 ..\..\..\ace\aced.lib /nologo /subsystem:console /debug /machine:I386
LINK32_FLAGS=..\..\..\ace\aced.lib /nologo /subsystem:console /incremental:yes\
 /pdb:"$(OUTDIR)/Indirect.pdb" /debug /machine:I386\
 /out:"$(OUTDIR)/Indirect.exe" 
LINK32_OBJS= \
	"$(INTDIR)\indirect_logging.obj"

"$(OUTDIR)\Indirect.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ENDIF 

CPP_PROJ=/nologo /MDd /W3 /Gm /GX /Zi /Od /I "..\..\.." /D "WIN32" /D "_DEBUG"\
 /D "_CONSOLE" /Fp"$(INTDIR)/Direct.pch" /YX /Fo"$(INTDIR)/" /Fd"$(INTDIR)/" /c 

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

# Name "Direct - Win32 Debug"
################################################################################
# Begin Source File

SOURCE=.\direct_logging.cpp
DEP_CPP_DIREC=\
	"..\..\..\ace/ACE.h"\
	"..\..\..\ace/ACE.i"\
	"..\..\..\ace/Addr.h"\
	"..\..\..\ace/Addr.i"\
	"..\..\..\ace/Auto_Ptr.cpp"\
	"..\..\..\ace/Auto_Ptr.h"\
	"..\..\..\ace/Auto_Ptr.i"\
	"..\..\..\ace/Basic_Types.h"\
	"..\..\..\ace/Basic_Types.i"\
	"..\..\..\ace/config-win32-borland.h"\
	"..\..\..\ace/config-win32-common.h"\
	"..\..\..\ace/config-win32.h"\
	"..\..\..\ace/config-WinCE.h"\
	"..\..\..\ace/config.h"\
	"..\..\..\ace/inc_user_config.h"\
	"..\..\..\ace/INET_Addr.h"\
	"..\..\..\ace/INET_Addr.i"\
	"..\..\..\ace/iosfwd.h"\
	"..\..\..\ace/IPC_SAP.h"\
	"..\..\..\ace/IPC_SAP.i"\
	"..\..\..\ace/Log_Msg.h"\
	"..\..\..\ace/Log_Priority.h"\
	"..\..\..\ace/Log_Record.h"\
	"..\..\..\ace/Log_Record.i"\
	"..\..\..\ace/Malloc_Base.h"\
	"..\..\..\ace/Managed_Object.cpp"\
	"..\..\..\ace/Managed_Object.h"\
	"..\..\..\ace/Managed_Object.i"\
	"..\..\..\ace/Object_Manager.h"\
	"..\..\..\ace/Object_Manager.i"\
	"..\..\..\ace/OS.h"\
	"..\..\..\ace/OS.i"\
	"..\..\..\ace/SOCK.h"\
	"..\..\..\ace/SOCK.i"\
	"..\..\..\ace/SOCK_Connector.h"\
	"..\..\..\ace/SOCK_Connector.i"\
	"..\..\..\ace/SOCK_IO.h"\
	"..\..\..\ace/SOCK_IO.i"\
	"..\..\..\ace/SOCK_Stream.h"\
	"..\..\..\ace/SOCK_Stream.i"\
	"..\..\..\ace/SString.h"\
	"..\..\..\ace/SString.i"\
	"..\..\..\ace/streams.h"\
	"..\..\..\ace/sys_conf.h"\
	"..\..\..\ace/Time_Value.h"\
	"..\..\..\ace/Trace.h"\
	"..\..\..\ace/ws2tcpip.h"\
	

"$(INTDIR)\direct_logging.obj" : $(SOURCE) $(DEP_CPP_DIREC) "$(INTDIR)"


# End Source File
# End Target
################################################################################
# Begin Target

# Name "Indirect - Win32 Debug"
################################################################################
# Begin Source File

SOURCE=.\indirect_logging.cpp
DEP_CPP_INDIR=\
	"..\..\..\ace/ACE.h"\
	"..\..\..\ace/ACE.i"\
	"..\..\..\ace/Auto_Ptr.cpp"\
	"..\..\..\ace/Auto_Ptr.h"\
	"..\..\..\ace/Auto_Ptr.i"\
	"..\..\..\ace/Basic_Types.h"\
	"..\..\..\ace/Basic_Types.i"\
	"..\..\..\ace/config-win32-borland.h"\
	"..\..\..\ace/config-win32-common.h"\
	"..\..\..\ace/config-win32.h"\
	"..\..\..\ace/config-WinCE.h"\
	"..\..\..\ace/config.h"\
	"..\..\..\ace/inc_user_config.h"\
	"..\..\..\ace/iosfwd.h"\
	"..\..\..\ace/Log_Msg.h"\
	"..\..\..\ace/Log_Priority.h"\
	"..\..\..\ace/Log_Record.h"\
	"..\..\..\ace/Log_Record.i"\
	"..\..\..\ace/Malloc_Base.h"\
	"..\..\..\ace/Managed_Object.cpp"\
	"..\..\..\ace/Managed_Object.h"\
	"..\..\..\ace/Managed_Object.i"\
	"..\..\..\ace/Object_Manager.h"\
	"..\..\..\ace/Object_Manager.i"\
	"..\..\..\ace/OS.h"\
	"..\..\..\ace/OS.i"\
	"..\..\..\ace/SString.h"\
	"..\..\..\ace/SString.i"\
	"..\..\..\ace/streams.h"\
	"..\..\..\ace/sys_conf.h"\
	"..\..\..\ace/Trace.h"\
	"..\..\..\ace/ws2tcpip.h"\
	

"$(INTDIR)\indirect_logging.obj" : $(SOURCE) $(DEP_CPP_INDIR) "$(INTDIR)"


# End Source File
# End Target
# End Project
################################################################################
