# Microsoft Developer Studio Generated NMAKE File, Format Version 4.20
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Console Application" 0x0103

!IF "$(CFG)" == ""
CFG=client - Win32 Debug
!MESSAGE No configuration specified.  Defaulting to client - Win32 Debug.
!ENDIF 

!IF "$(CFG)" != "server - Win32 Release" && "$(CFG)" != "server - Win32 Debug"\
 && "$(CFG)" != "client - Win32 Release" && "$(CFG)" != "client - Win32 Debug"
!MESSAGE Invalid configuration "$(CFG)" specified.
!MESSAGE You can specify a configuration when running NMAKE on this makefile
!MESSAGE by defining the macro CFG on the command line.  For example:
!MESSAGE 
!MESSAGE NMAKE /f "Cubit.mak" CFG="client - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "server - Win32 Release" (based on "Win32 (x86) Console Application")
!MESSAGE "server - Win32 Debug" (based on "Win32 (x86) Console Application")
!MESSAGE "client - Win32 Release" (based on "Win32 (x86) Console Application")
!MESSAGE "client - Win32 Debug" (based on "Win32 (x86) Console Application")
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
# PROP Target_Last_Scanned "client - Win32 Debug"
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "server - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "server\Release"
# PROP BASE Intermediate_Dir "server\Release"
# PROP BASE Target_Dir "server"
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir ""
# PROP Intermediate_Dir "server\Release"
# PROP Target_Dir "server"
OUTDIR=.
INTDIR=.\server\Release

ALL : "$(OUTDIR)\server.exe"

CLEAN : 
	-@erase "$(INTDIR)\cubit_i.obj"
	-@erase "$(INTDIR)\cubitC.obj"
	-@erase "$(INTDIR)\cubitS.obj"
	-@erase "$(INTDIR)\svr.obj"
	-@erase "$(OUTDIR)\server.exe"
	-@erase ".\cubitC.cpp"
	-@erase ".\cubitC.h"
	-@erase ".\cubitC.i"
	-@erase ".\cubitS.cpp"
	-@erase ".\cubitS.h"
	-@erase ".\cubitS.i"

"$(INTDIR)" :
    if not exist "$(INTDIR)/$(NULL)" mkdir "$(INTDIR)"

# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /YX /c
# ADD CPP /nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /YX /c
CPP_PROJ=/nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_CONSOLE"\
 /Fp"$(INTDIR)/server.pch" /YX /Fo"$(INTDIR)/" /c 
CPP_OBJS=.\server\Release/
CPP_SBRS=.\.
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
BSC32_FLAGS=/nologo /o"$(OUTDIR)/server.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /machine:I386
# ADD LINK32 ace.lib TAO.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /machine:I386
LINK32_FLAGS=ace.lib TAO.lib kernel32.lib user32.lib gdi32.lib winspool.lib\
 comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib\
 odbc32.lib odbccp32.lib /nologo /subsystem:console /incremental:no\
 /pdb:"$(OUTDIR)/server.pdb" /machine:I386 /out:"$(OUTDIR)/server.exe" 
LINK32_OBJS= \
	"$(INTDIR)\cubit_i.obj" \
	"$(INTDIR)\cubitC.obj" \
	"$(INTDIR)\cubitS.obj" \
	"$(INTDIR)\svr.obj"

"$(OUTDIR)\server.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ELSEIF  "$(CFG)" == "server - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "server\Debug"
# PROP BASE Intermediate_Dir "server\Debug"
# PROP BASE Target_Dir "server"
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir ""
# PROP Intermediate_Dir "server\Debug"
# PROP Target_Dir "server"
OUTDIR=.
INTDIR=.\server\Debug

ALL : "$(OUTDIR)\server.exe"

CLEAN : 
	-@erase "$(INTDIR)\cubit_i.obj"
	-@erase "$(INTDIR)\cubitC.obj"
	-@erase "$(INTDIR)\cubitS.obj"
	-@erase "$(INTDIR)\svr.obj"
	-@erase "$(INTDIR)\vc40.idb"
	-@erase "$(INTDIR)\vc40.pdb"
	-@erase "$(OUTDIR)\server.exe"
	-@erase "$(OUTDIR)\server.ilk"
	-@erase "$(OUTDIR)\server.pdb"
	-@erase ".\cubitC.cpp"
	-@erase ".\cubitC.h"
	-@erase ".\cubitC.i"
	-@erase ".\cubitS.cpp"
	-@erase ".\cubitS.h"
	-@erase ".\cubitS.i"

"$(INTDIR)" :
    if not exist "$(INTDIR)/$(NULL)" mkdir "$(INTDIR)"

# ADD BASE CPP /nologo /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /YX /c
# ADD CPP /nologo /MDd /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /YX /c
CPP_PROJ=/nologo /MDd /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_CONSOLE"\
 /Fp"$(INTDIR)/server.pch" /YX /Fo"$(INTDIR)/" /Fd"$(INTDIR)/" /c 
CPP_OBJS=.\server\Debug/
CPP_SBRS=.\.
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
BSC32_FLAGS=/nologo /o"$(OUTDIR)/server.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /debug /machine:I386
# ADD LINK32 aced.lib TAOd.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /debug /machine:I386
LINK32_FLAGS=aced.lib TAOd.lib kernel32.lib user32.lib gdi32.lib winspool.lib\
 comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib\
 odbc32.lib odbccp32.lib /nologo /subsystem:console /incremental:yes\
 /pdb:"$(OUTDIR)/server.pdb" /debug /machine:I386 /out:"$(OUTDIR)/server.exe" 
LINK32_OBJS= \
	"$(INTDIR)\cubit_i.obj" \
	"$(INTDIR)\cubitC.obj" \
	"$(INTDIR)\cubitS.obj" \
	"$(INTDIR)\svr.obj"

"$(OUTDIR)\server.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ELSEIF  "$(CFG)" == "client - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "client\Release"
# PROP BASE Intermediate_Dir "client\Release"
# PROP BASE Target_Dir "client"
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir ""
# PROP Intermediate_Dir "client\Release"
# PROP Target_Dir "client"
OUTDIR=.
INTDIR=.\client\Release

ALL : "$(OUTDIR)\client.exe" "$(OUTDIR)\cubitS.h" "$(OUTDIR)\cubitS.i"\
 "$(OUTDIR)\cubitS.cpp"

CLEAN : 
	-@erase "$(INTDIR)\clnt.obj"
	-@erase "$(INTDIR)\cubitC.obj"
	-@erase "$(OUTDIR)\client.exe"
	-@erase ".\cubitC.cpp"
	-@erase ".\cubitC.h"
	-@erase ".\cubitC.i"
	-@erase ".\cubitS.cpp"
	-@erase ".\cubitS.h"
	-@erase ".\cubitS.i"

"$(INTDIR)" :
    if not exist "$(INTDIR)/$(NULL)" mkdir "$(INTDIR)"

# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /YX /c
# ADD CPP /nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /YX /c
CPP_PROJ=/nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_CONSOLE"\
 /Fp"$(INTDIR)/client.pch" /YX /Fo"$(INTDIR)/" /c 
CPP_OBJS=.\client\Release/
CPP_SBRS=.\.
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
BSC32_FLAGS=/nologo /o"$(OUTDIR)/client.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /machine:I386
# ADD LINK32 ace.lib TAO.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /machine:I386
LINK32_FLAGS=ace.lib TAO.lib kernel32.lib user32.lib gdi32.lib winspool.lib\
 comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib\
 odbc32.lib odbccp32.lib /nologo /subsystem:console /incremental:no\
 /pdb:"$(OUTDIR)/client.pdb" /machine:I386 /out:"$(OUTDIR)/client.exe" 
LINK32_OBJS= \
	"$(INTDIR)\clnt.obj" \
	"$(INTDIR)\cubitC.obj"

"$(OUTDIR)\client.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ELSEIF  "$(CFG)" == "client - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "client\Debug"
# PROP BASE Intermediate_Dir "client\Debug"
# PROP BASE Target_Dir "client"
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir ""
# PROP Intermediate_Dir "client\Debug"
# PROP Target_Dir "client"
OUTDIR=.
INTDIR=.\client\Debug

ALL : "$(OUTDIR)\client.exe" "$(OUTDIR)\cubitS.h" "$(OUTDIR)\cubitS.i"\
 "$(OUTDIR)\cubitS.cpp"

CLEAN : 
	-@erase "$(INTDIR)\clnt.obj"
	-@erase "$(INTDIR)\cubitC.obj"
	-@erase "$(INTDIR)\vc40.idb"
	-@erase "$(INTDIR)\vc40.pdb"
	-@erase "$(OUTDIR)\client.exe"
	-@erase "$(OUTDIR)\client.ilk"
	-@erase "$(OUTDIR)\client.pdb"
	-@erase ".\cubitC.cpp"
	-@erase ".\cubitC.h"
	-@erase ".\cubitC.i"
	-@erase ".\cubitS.cpp"
	-@erase ".\cubitS.h"
	-@erase ".\cubitS.i"

"$(INTDIR)" :
    if not exist "$(INTDIR)/$(NULL)" mkdir "$(INTDIR)"

# ADD BASE CPP /nologo /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /YX /c
# ADD CPP /nologo /MDd /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /YX /c
CPP_PROJ=/nologo /MDd /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_CONSOLE"\
 /Fp"$(INTDIR)/client.pch" /YX /Fo"$(INTDIR)/" /Fd"$(INTDIR)/" /c 
CPP_OBJS=.\client\Debug/
CPP_SBRS=.\.
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
BSC32_FLAGS=/nologo /o"$(OUTDIR)/client.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /debug /machine:I386
# ADD LINK32 aced.lib TAOd.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /debug /machine:I386
LINK32_FLAGS=aced.lib TAOd.lib kernel32.lib user32.lib gdi32.lib winspool.lib\
 comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib\
 odbc32.lib odbccp32.lib /nologo /subsystem:console /incremental:yes\
 /pdb:"$(OUTDIR)/client.pdb" /debug /machine:I386 /out:"$(OUTDIR)/client.exe" 
LINK32_OBJS= \
	"$(INTDIR)\clnt.obj" \
	"$(INTDIR)\cubitC.obj"

"$(OUTDIR)\client.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
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

# Name "server - Win32 Release"
# Name "server - Win32 Debug"

!IF  "$(CFG)" == "server - Win32 Release"

!ELSEIF  "$(CFG)" == "server - Win32 Debug"

!ENDIF 

################################################################################
# Begin Source File

SOURCE=.\svr.cpp
DEP_CPP_SVR_C=\
	".\cubit_i.h"\
	".\cubitC.h"\
	".\cubitC.i"\
	".\cubitS.h"\
	".\cubitS.i"\
	".\tao\corba.h"\
	{$(INCLUDE)}"\ace\ACE.h"\
	{$(INCLUDE)}"\ace\ACE.i"\
	{$(INCLUDE)}"\ace\Auto_Ptr.cpp"\
	{$(INCLUDE)}"\ace\Auto_Ptr.h"\
	{$(INCLUDE)}"\ace\Auto_Ptr.i"\
	{$(INCLUDE)}"\ace\config-win32-common.h"\
	{$(INCLUDE)}"\ace\config-win32.h"\
	{$(INCLUDE)}"\ace\config.h"\
	{$(INCLUDE)}"\ace\Get_Opt.h"\
	{$(INCLUDE)}"\ace\Get_Opt.i"\
	{$(INCLUDE)}"\ace\Log_Msg.h"\
	{$(INCLUDE)}"\ace\Log_Priority.h"\
	{$(INCLUDE)}"\ace\Log_Record.h"\
	{$(INCLUDE)}"\ace\Log_Record.i"\
	{$(INCLUDE)}"\ace\Managed_Object.cpp"\
	{$(INCLUDE)}"\ace\Managed_Object.h"\
	{$(INCLUDE)}"\ace\Managed_Object.i"\
	{$(INCLUDE)}"\ace\Object_Manager.h"\
	{$(INCLUDE)}"\ace\Object_Manager.i"\
	{$(INCLUDE)}"\ace\OS.h"\
	{$(INCLUDE)}"\ace\OS.i"\
	{$(INCLUDE)}"\ace\SString.h"\
	{$(INCLUDE)}"\ace\SString.i"\
	{$(INCLUDE)}"\ace\stdcpp.h"\
	{$(INCLUDE)}"\ace\Trace.h"\
	{$(INCLUDE)}"\ace\Version.h"\
	{$(INCLUDE)}"\ace\ws2tcpip.h"\
	

"$(INTDIR)\svr.obj" : $(SOURCE) $(DEP_CPP_SVR_C) "$(INTDIR)" ".\cubitC.h"\
 ".\cubitC.i" ".\cubitS.h" ".\cubitS.i"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\cubitS.cpp
DEP_CPP_CUBIT=\
	".\cubitC.h"\
	".\cubitC.i"\
	".\cubitS.h"\
	".\cubitS.i"\
	".\tao\corba.h"\
	

"$(INTDIR)\cubitS.obj" : $(SOURCE) $(DEP_CPP_CUBIT) "$(INTDIR)" ".\cubitC.h"\
 ".\cubitC.i" ".\cubitS.h" ".\cubitS.i"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\cubit_i.cpp
DEP_CPP_CUBIT_=\
	".\cubit_i.h"\
	".\cubitC.h"\
	".\cubitC.i"\
	".\cubitS.h"\
	".\cubitS.i"\
	".\tao\corba.h"\
	

"$(INTDIR)\cubit_i.obj" : $(SOURCE) $(DEP_CPP_CUBIT_) "$(INTDIR)" ".\cubitC.h"\
 ".\cubitC.i" ".\cubitS.h" ".\cubitS.i"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\cubitC.cpp
DEP_CPP_CUBITC=\
	".\cubitC.h"\
	".\cubitC.i"\
	".\tao\corba.h"\
	

"$(INTDIR)\cubitC.obj" : $(SOURCE) $(DEP_CPP_CUBITC) "$(INTDIR)" ".\cubitC.h"\
 ".\cubitC.i"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\cubit.idl

!IF  "$(CFG)" == "server - Win32 Release"

# PROP Exclude_From_Build 0
# Begin Custom Build - Compiling IDL file(s):
InputPath=.\cubit.idl
InputName=cubit

BuildCmds= \
	..\..\..\..\tao_idl\tao_idl.exe $(InputName).idl \
	

"$(InputName)C.h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)C.i" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)C.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)S.h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)S.i" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)S.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)
# End Custom Build

!ELSEIF  "$(CFG)" == "server - Win32 Debug"

# PROP Exclude_From_Build 0
# Begin Custom Build - Compiling IDL file(s):
InputPath=.\cubit.idl
InputName=cubit

BuildCmds= \
	..\..\..\..\tao_idl\tao_idl.exe $(InputName).idl \
	

"$(InputName)C.h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)C.i" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)C.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)S.h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)S.i" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)S.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)
# End Custom Build

!ENDIF 

# End Source File
# End Target
################################################################################
# Begin Target

# Name "client - Win32 Release"
# Name "client - Win32 Debug"

!IF  "$(CFG)" == "client - Win32 Release"

!ELSEIF  "$(CFG)" == "client - Win32 Debug"

!ENDIF 

################################################################################
# Begin Source File

SOURCE=.\cubitC.cpp

!IF  "$(CFG)" == "client - Win32 Release"

DEP_CPP_CUBITC=\
	".\cubitC.h"\
	".\cubitC.i"\
	".\tao\corba.h"\
	

"$(INTDIR)\cubitC.obj" : $(SOURCE) $(DEP_CPP_CUBITC) "$(INTDIR)" ".\cubitC.h"\
 ".\cubitC.i"


!ELSEIF  "$(CFG)" == "client - Win32 Debug"

DEP_CPP_CUBITC=\
	".\cubitC.h"\
	".\cubitC.i"\
	".\tao\corba.h"\
	

"$(INTDIR)\cubitC.obj" : $(SOURCE) $(DEP_CPP_CUBITC) "$(INTDIR)" ".\cubitC.h"\
 ".\cubitC.i"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\clnt.cpp

!IF  "$(CFG)" == "client - Win32 Release"

DEP_CPP_CLNT_=\
	".\clnt.h"\
	".\cubitC.h"\
	".\cubitC.i"\
	".\tao\corba.h"\
	{$(INCLUDE)}"\ace\ACE.h"\
	{$(INCLUDE)}"\ace\ACE.i"\
	{$(INCLUDE)}"\ace\Auto_Ptr.cpp"\
	{$(INCLUDE)}"\ace\Auto_Ptr.h"\
	{$(INCLUDE)}"\ace\Auto_Ptr.i"\
	{$(INCLUDE)}"\ace\config-win32-common.h"\
	{$(INCLUDE)}"\ace\config-win32.h"\
	{$(INCLUDE)}"\ace\config.h"\
	{$(INCLUDE)}"\ace\Get_Opt.h"\
	{$(INCLUDE)}"\ace\Get_Opt.i"\
	{$(INCLUDE)}"\ace\High_Res_Timer.h"\
	{$(INCLUDE)}"\ace\High_Res_Timer.i"\
	{$(INCLUDE)}"\ace\Log_Msg.h"\
	{$(INCLUDE)}"\ace\Log_Priority.h"\
	{$(INCLUDE)}"\ace\Log_Record.h"\
	{$(INCLUDE)}"\ace\Log_Record.i"\
	{$(INCLUDE)}"\ace\Managed_Object.cpp"\
	{$(INCLUDE)}"\ace\Managed_Object.h"\
	{$(INCLUDE)}"\ace\Managed_Object.i"\
	{$(INCLUDE)}"\ace\Object_Manager.h"\
	{$(INCLUDE)}"\ace\Object_Manager.i"\
	{$(INCLUDE)}"\ace\OS.h"\
	{$(INCLUDE)}"\ace\OS.i"\
	{$(INCLUDE)}"\ace\Profile_Timer.h"\
	{$(INCLUDE)}"\ace\Profile_Timer.i"\
	{$(INCLUDE)}"\ace\SString.h"\
	{$(INCLUDE)}"\ace\SString.i"\
	{$(INCLUDE)}"\ace\stdcpp.h"\
	{$(INCLUDE)}"\ace\Time_Value.h"\
	{$(INCLUDE)}"\ace\Trace.h"\
	{$(INCLUDE)}"\ace\Version.h"\
	{$(INCLUDE)}"\ace\ws2tcpip.h"\
	

"$(INTDIR)\clnt.obj" : $(SOURCE) $(DEP_CPP_CLNT_) "$(INTDIR)" ".\cubitC.h"\
 ".\cubitC.i"


!ELSEIF  "$(CFG)" == "client - Win32 Debug"

DEP_CPP_CLNT_=\
	".\clnt.h"\
	".\cubitC.h"\
	".\cubitC.i"\
	".\tao\corba.h"\
	{$(INCLUDE)}"\ace\ACE.h"\
	{$(INCLUDE)}"\ace\ACE.i"\
	{$(INCLUDE)}"\ace\Auto_Ptr.cpp"\
	{$(INCLUDE)}"\ace\Auto_Ptr.h"\
	{$(INCLUDE)}"\ace\Auto_Ptr.i"\
	{$(INCLUDE)}"\ace\config-win32-common.h"\
	{$(INCLUDE)}"\ace\config-win32.h"\
	{$(INCLUDE)}"\ace\config.h"\
	{$(INCLUDE)}"\ace\Get_Opt.h"\
	{$(INCLUDE)}"\ace\Get_Opt.i"\
	{$(INCLUDE)}"\ace\High_Res_Timer.h"\
	{$(INCLUDE)}"\ace\High_Res_Timer.i"\
	{$(INCLUDE)}"\ace\Log_Msg.h"\
	{$(INCLUDE)}"\ace\Log_Priority.h"\
	{$(INCLUDE)}"\ace\Log_Record.h"\
	{$(INCLUDE)}"\ace\Log_Record.i"\
	{$(INCLUDE)}"\ace\Managed_Object.cpp"\
	{$(INCLUDE)}"\ace\Managed_Object.h"\
	{$(INCLUDE)}"\ace\Managed_Object.i"\
	{$(INCLUDE)}"\ace\Object_Manager.h"\
	{$(INCLUDE)}"\ace\Object_Manager.i"\
	{$(INCLUDE)}"\ace\OS.h"\
	{$(INCLUDE)}"\ace\OS.i"\
	{$(INCLUDE)}"\ace\Profile_Timer.h"\
	{$(INCLUDE)}"\ace\Profile_Timer.i"\
	{$(INCLUDE)}"\ace\SString.h"\
	{$(INCLUDE)}"\ace\SString.i"\
	{$(INCLUDE)}"\ace\stdcpp.h"\
	{$(INCLUDE)}"\ace\Time_Value.h"\
	{$(INCLUDE)}"\ace\Trace.h"\
	{$(INCLUDE)}"\ace\Version.h"\
	{$(INCLUDE)}"\ace\ws2tcpip.h"\
	

"$(INTDIR)\clnt.obj" : $(SOURCE) $(DEP_CPP_CLNT_) "$(INTDIR)" ".\cubitC.h"\
 ".\cubitC.i"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\cubit.idl

!IF  "$(CFG)" == "client - Win32 Release"

# PROP Exclude_From_Build 0
# Begin Custom Build - Compiling IDL file(s):
InputPath=.\cubit.idl
InputName=cubit

BuildCmds= \
	..\..\..\..\tao_idl\tao_idl.exe $(InputName).idl \
	

"$(InputName)C.h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)C.i" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)C.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)S.h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)S.i" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)S.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)
# End Custom Build

!ELSEIF  "$(CFG)" == "client - Win32 Debug"

# PROP Exclude_From_Build 0
# Begin Custom Build - Compiling IDL file(s):
InputPath=.\cubit.idl
InputName=cubit

BuildCmds= \
	..\..\..\..\tao_idl\tao_idl.exe $(InputName).idl \
	

"$(InputName)C.h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)C.i" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)C.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)S.h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)S.i" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)S.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)
# End Custom Build

!ENDIF 

# End Source File
# End Target
# End Project
################################################################################
