# Microsoft Developer Studio Generated NMAKE File, Format Version 4.20
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Console Application" 0x0103

!IF "$(CFG)" == ""
CFG=CPP_inserver - Win32 Debug
!MESSAGE No configuration specified.  Defaulting to CPP_inserver - Win32 Debug.
!ENDIF 

!IF "$(CFG)" != "C_inclient - Win32 Debug" && "$(CFG)" !=\
 "C_inserver - Win32 Debug" && "$(CFG)" != "CPP_inclient - Win32 Debug" &&\
 "$(CFG)" != "CPP_inserver - Win32 Debug"
!MESSAGE Invalid configuration "$(CFG)" specified.
!MESSAGE You can specify a configuration when running NMAKE on this makefile
!MESSAGE by defining the macro CFG on the command line.  For example:
!MESSAGE 
!MESSAGE NMAKE /f "SOCK_SAP.mak" CFG="CPP_inserver - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "C_inclient - Win32 Debug" (based on\
 "Win32 (x86) Console Application")
!MESSAGE "C_inserver - Win32 Debug" (based on\
 "Win32 (x86) Console Application")
!MESSAGE "CPP_inclient - Win32 Debug" (based on\
 "Win32 (x86) Console Application")
!MESSAGE "CPP_inserver - Win32 Debug" (based on\
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

!IF  "$(CFG)" == "C_inclient - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "C_inclient\Debug"
# PROP BASE Intermediate_Dir "C_inclient\Debug"
# PROP BASE Target_Dir "C_inclient"
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "."
# PROP Intermediate_Dir "debug"
# PROP Target_Dir "C_inclient"
OUTDIR=.\.
INTDIR=.\debug

ALL : "$(OUTDIR)\C_inclient.exe"

CLEAN : 
	-@erase "$(INTDIR)\C-inclient.obj"
	-@erase "$(INTDIR)\vc40.idb"
	-@erase "$(INTDIR)\vc40.pdb"
	-@erase "$(OUTDIR)\C_inclient.exe"
	-@erase "$(OUTDIR)\C_inclient.ilk"
	-@erase "$(OUTDIR)\C_inclient.pdb"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

"$(INTDIR)" :
    if not exist "$(INTDIR)/$(NULL)" mkdir "$(INTDIR)"

# ADD BASE CPP /nologo /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /YX /c
# ADD CPP /nologo /MDd /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /YX /c
CPP_OBJS=.\debug/
CPP_SBRS=.\.
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
BSC32_FLAGS=/nologo /o"$(OUTDIR)/C_inclient.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /debug /machine:I386
# ADD LINK32 ace.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /debug /machine:I386
LINK32_FLAGS=ace.lib kernel32.lib user32.lib gdi32.lib winspool.lib\
 comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib\
 odbc32.lib odbccp32.lib /nologo /subsystem:console /incremental:yes\
 /pdb:"$(OUTDIR)/C_inclient.pdb" /debug /machine:I386\
 /out:"$(OUTDIR)/C_inclient.exe" 
LINK32_OBJS= \
	"$(INTDIR)\C-inclient.obj"

"$(OUTDIR)\C_inclient.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ELSEIF  "$(CFG)" == "C_inserver - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "C_inserver\Debug"
# PROP BASE Intermediate_Dir "C_inserver\Debug"
# PROP BASE Target_Dir "C_inserver"
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "."
# PROP Intermediate_Dir "debug"
# PROP Target_Dir "C_inserver"
OUTDIR=.\.
INTDIR=.\debug

ALL : "$(OUTDIR)\C_inserver.exe"

CLEAN : 
	-@erase "$(INTDIR)\C-inserver.obj"
	-@erase "$(INTDIR)\vc40.idb"
	-@erase "$(INTDIR)\vc40.pdb"
	-@erase "$(OUTDIR)\C_inserver.exe"
	-@erase "$(OUTDIR)\C_inserver.ilk"
	-@erase "$(OUTDIR)\C_inserver.pdb"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

"$(INTDIR)" :
    if not exist "$(INTDIR)/$(NULL)" mkdir "$(INTDIR)"

# ADD BASE CPP /nologo /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /YX /c
# ADD CPP /nologo /MDd /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /YX /c
CPP_OBJS=.\debug/
CPP_SBRS=.\.
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
BSC32_FLAGS=/nologo /o"$(OUTDIR)/C_inserver.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /debug /machine:I386
# ADD LINK32 ace.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /debug /machine:I386
LINK32_FLAGS=ace.lib kernel32.lib user32.lib gdi32.lib winspool.lib\
 comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib\
 odbc32.lib odbccp32.lib /nologo /subsystem:console /incremental:yes\
 /pdb:"$(OUTDIR)/C_inserver.pdb" /debug /machine:I386\
 /out:"$(OUTDIR)/C_inserver.exe" 
LINK32_OBJS= \
	"$(INTDIR)\C-inserver.obj"

"$(OUTDIR)\C_inserver.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ELSEIF  "$(CFG)" == "CPP_inclient - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "CPP_inclient\Debug"
# PROP BASE Intermediate_Dir "CPP_inclient\Debug"
# PROP BASE Target_Dir "CPP_inclient"
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "."
# PROP Intermediate_Dir "debug"
# PROP Target_Dir "CPP_inclient"
OUTDIR=.\.
INTDIR=.\debug

ALL : "$(OUTDIR)\CPP_inclient.exe"

CLEAN : 
	-@erase "$(INTDIR)\CPP-inclient.obj"
	-@erase "$(INTDIR)\vc40.idb"
	-@erase "$(INTDIR)\vc40.pdb"
	-@erase "$(OUTDIR)\CPP_inclient.exe"
	-@erase "$(OUTDIR)\CPP_inclient.ilk"
	-@erase "$(OUTDIR)\CPP_inclient.pdb"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

"$(INTDIR)" :
    if not exist "$(INTDIR)/$(NULL)" mkdir "$(INTDIR)"

# ADD BASE CPP /nologo /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /YX /c
# ADD CPP /nologo /MDd /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /YX /c
CPP_OBJS=.\debug/
CPP_SBRS=.\.
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
BSC32_FLAGS=/nologo /o"$(OUTDIR)/CPP_inclient.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /debug /machine:I386
# ADD LINK32 ace.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /debug /machine:I386
LINK32_FLAGS=ace.lib kernel32.lib user32.lib gdi32.lib winspool.lib\
 comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib\
 odbc32.lib odbccp32.lib /nologo /subsystem:console /incremental:yes\
 /pdb:"$(OUTDIR)/CPP_inclient.pdb" /debug /machine:I386\
 /out:"$(OUTDIR)/CPP_inclient.exe" 
LINK32_OBJS= \
	"$(INTDIR)\CPP-inclient.obj"

"$(OUTDIR)\CPP_inclient.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ELSEIF  "$(CFG)" == "CPP_inserver - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "CPP_inserver\Debug"
# PROP BASE Intermediate_Dir "CPP_inserver\Debug"
# PROP BASE Target_Dir "CPP_inserver"
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "."
# PROP Intermediate_Dir "debug"
# PROP Target_Dir "CPP_inserver"
OUTDIR=.\.
INTDIR=.\debug

ALL : "$(OUTDIR)\CPP_inserver.exe"

CLEAN : 
	-@erase "$(INTDIR)\CPP-inserver.obj"
	-@erase "$(INTDIR)\vc40.idb"
	-@erase "$(INTDIR)\vc40.pdb"
	-@erase "$(OUTDIR)\CPP_inserver.exe"
	-@erase "$(OUTDIR)\CPP_inserver.ilk"
	-@erase "$(OUTDIR)\CPP_inserver.pdb"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

"$(INTDIR)" :
    if not exist "$(INTDIR)/$(NULL)" mkdir "$(INTDIR)"

# ADD BASE CPP /nologo /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /YX /c
# ADD CPP /nologo /MDd /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /YX /c
CPP_OBJS=.\debug/
CPP_SBRS=.\.
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
BSC32_FLAGS=/nologo /o"$(OUTDIR)/CPP_inserver.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /debug /machine:I386
# ADD LINK32 ace.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /debug /machine:I386
LINK32_FLAGS=ace.lib kernel32.lib user32.lib gdi32.lib winspool.lib\
 comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib\
 odbc32.lib odbccp32.lib /nologo /subsystem:console /incremental:yes\
 /pdb:"$(OUTDIR)/CPP_inserver.pdb" /debug /machine:I386\
 /out:"$(OUTDIR)/CPP_inserver.exe" 
LINK32_OBJS= \
	"$(INTDIR)\CPP-inserver.obj"

"$(OUTDIR)\CPP_inserver.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ENDIF 

CPP_PROJ=/nologo /MDd /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_CONSOLE"\
 /Fp"$(INTDIR)/C_inclient.pch" /YX /Fo"$(INTDIR)/" /Fd"$(INTDIR)/" /c 

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

# Name "C_inclient - Win32 Debug"
################################################################################
# Begin Source File

SOURCE=".\C-inclient.cpp"
DEP_CPP_C_INC=\
	{$(INCLUDE)}"\ace\ACE.h"\
	{$(INCLUDE)}"\ace\ACE.i"\
	{$(INCLUDE)}"\ace\config-win32-common.h"\
	{$(INCLUDE)}"\ace\config.h"\
	{$(INCLUDE)}"\ace\Log_Msg.h"\
	{$(INCLUDE)}"\ace\Log_Priority.h"\
	{$(INCLUDE)}"\ace\Log_Record.h"\
	{$(INCLUDE)}"\ace\Log_Record.i"\
	{$(INCLUDE)}"\ace\OS.h"\
	{$(INCLUDE)}"\ace\OS.i"\
	{$(INCLUDE)}"\ace\SString.h"\
	{$(INCLUDE)}"\ace\SString.i"\
	{$(INCLUDE)}"\ace\stdcpp.h"\
	{$(INCLUDE)}"\ace\Trace.h"\
	{$(INCLUDE)}"\ace\ws2tcpip.h"\
	

"$(INTDIR)\C-inclient.obj" : $(SOURCE) $(DEP_CPP_C_INC) "$(INTDIR)"


# End Source File
# End Target
################################################################################
# Begin Target

# Name "C_inserver - Win32 Debug"
################################################################################
# Begin Source File

SOURCE=".\C-inserver.cpp"
DEP_CPP_C_INS=\
	{$(INCLUDE)}"\ace\ACE.h"\
	{$(INCLUDE)}"\ace\ACE.i"\
	{$(INCLUDE)}"\ace\config-win32-common.h"\
	{$(INCLUDE)}"\ace\config.h"\
	{$(INCLUDE)}"\ace\Log_Msg.h"\
	{$(INCLUDE)}"\ace\Log_Priority.h"\
	{$(INCLUDE)}"\ace\Log_Record.h"\
	{$(INCLUDE)}"\ace\Log_Record.i"\
	{$(INCLUDE)}"\ace\OS.h"\
	{$(INCLUDE)}"\ace\OS.i"\
	{$(INCLUDE)}"\ace\SString.h"\
	{$(INCLUDE)}"\ace\SString.i"\
	{$(INCLUDE)}"\ace\stdcpp.h"\
	{$(INCLUDE)}"\ace\Trace.h"\
	{$(INCLUDE)}"\ace\ws2tcpip.h"\
	

"$(INTDIR)\C-inserver.obj" : $(SOURCE) $(DEP_CPP_C_INS) "$(INTDIR)"


# End Source File
# End Target
################################################################################
# Begin Target

# Name "CPP_inclient - Win32 Debug"
################################################################################
# Begin Source File

SOURCE=".\CPP-inclient.cpp"
DEP_CPP_CPP_I=\
	{$(INCLUDE)}"\ace\ACE.h"\
	{$(INCLUDE)}"\ace\ACE.i"\
	{$(INCLUDE)}"\ace\Addr.h"\
	{$(INCLUDE)}"\ace\Addr.i"\
	{$(INCLUDE)}"\ace\config-win32-common.h"\
	{$(INCLUDE)}"\ace\config.h"\
	{$(INCLUDE)}"\ace\INET_Addr.h"\
	{$(INCLUDE)}"\ace\INET_Addr.i"\
	{$(INCLUDE)}"\ace\IPC_SAP.h"\
	{$(INCLUDE)}"\ace\IPC_SAP.i"\
	{$(INCLUDE)}"\ace\Log_Msg.h"\
	{$(INCLUDE)}"\ace\Log_Priority.h"\
	{$(INCLUDE)}"\ace\Log_Record.h"\
	{$(INCLUDE)}"\ace\Log_Record.i"\
	{$(INCLUDE)}"\ace\OS.h"\
	{$(INCLUDE)}"\ace\OS.i"\
	{$(INCLUDE)}"\ace\SOCK.h"\
	{$(INCLUDE)}"\ace\SOCK.i"\
	{$(INCLUDE)}"\ace\SOCK_Connector.h"\
	{$(INCLUDE)}"\ace\SOCK_Connector.i"\
	{$(INCLUDE)}"\ace\SOCK_IO.h"\
	{$(INCLUDE)}"\ace\SOCK_IO.i"\
	{$(INCLUDE)}"\ace\SOCK_Stream.h"\
	{$(INCLUDE)}"\ace\SOCK_Stream.i"\
	{$(INCLUDE)}"\ace\SString.h"\
	{$(INCLUDE)}"\ace\SString.i"\
	{$(INCLUDE)}"\ace\stdcpp.h"\
	{$(INCLUDE)}"\ace\Time_Value.h"\
	{$(INCLUDE)}"\ace\Trace.h"\
	{$(INCLUDE)}"\ace\ws2tcpip.h"\
	

"$(INTDIR)\CPP-inclient.obj" : $(SOURCE) $(DEP_CPP_CPP_I) "$(INTDIR)"


# End Source File
# End Target
################################################################################
# Begin Target

# Name "CPP_inserver - Win32 Debug"
################################################################################
# Begin Source File

SOURCE=".\CPP-inserver.cpp"
DEP_CPP_CPP_IN=\
	{$(INCLUDE)}"\ace\ACE.h"\
	{$(INCLUDE)}"\ace\ACE.i"\
	{$(INCLUDE)}"\ace\Addr.h"\
	{$(INCLUDE)}"\ace\Addr.i"\
	{$(INCLUDE)}"\ace\config-win32-common.h"\
	{$(INCLUDE)}"\ace\config.h"\
	{$(INCLUDE)}"\ace\Handle_Set.h"\
	{$(INCLUDE)}"\ace\Handle_Set.i"\
	{$(INCLUDE)}"\ace\INET_Addr.h"\
	{$(INCLUDE)}"\ace\INET_Addr.i"\
	{$(INCLUDE)}"\ace\IPC_SAP.h"\
	{$(INCLUDE)}"\ace\IPC_SAP.i"\
	{$(INCLUDE)}"\ace\Log_Msg.h"\
	{$(INCLUDE)}"\ace\Log_Priority.h"\
	{$(INCLUDE)}"\ace\Log_Record.h"\
	{$(INCLUDE)}"\ace\Log_Record.i"\
	{$(INCLUDE)}"\ace\OS.h"\
	{$(INCLUDE)}"\ace\OS.i"\
	{$(INCLUDE)}"\ace\SOCK.h"\
	{$(INCLUDE)}"\ace\SOCK.i"\
	{$(INCLUDE)}"\ace\SOCK_Acceptor.h"\
	{$(INCLUDE)}"\ace\SOCK_Acceptor.i"\
	{$(INCLUDE)}"\ace\SOCK_IO.h"\
	{$(INCLUDE)}"\ace\SOCK_IO.i"\
	{$(INCLUDE)}"\ace\SOCK_Stream.h"\
	{$(INCLUDE)}"\ace\SOCK_Stream.i"\
	{$(INCLUDE)}"\ace\SString.h"\
	{$(INCLUDE)}"\ace\SString.i"\
	{$(INCLUDE)}"\ace\stdcpp.h"\
	{$(INCLUDE)}"\ace\Time_Value.h"\
	{$(INCLUDE)}"\ace\Trace.h"\
	{$(INCLUDE)}"\ace\ws2tcpip.h"\
	

"$(INTDIR)\CPP-inserver.obj" : $(SOURCE) $(DEP_CPP_CPP_IN) "$(INTDIR)"


# End Source File
# End Target
# End Project
################################################################################
