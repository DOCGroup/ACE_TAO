# Microsoft Developer Studio Generated NMAKE File, Format Version 4.20
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Static Library" 0x0104

!IF "$(CFG)" == ""
CFG=TAO_Baseline - Win32 Debug
!MESSAGE No configuration specified.  Defaulting to TAO_Baseline - Win32 Debug.
!ENDIF 

!IF "$(CFG)" != "TAO_Baseline - Win32 Release" && "$(CFG)" !=\
 "TAO_Baseline - Win32 Debug"
!MESSAGE Invalid configuration "$(CFG)" specified.
!MESSAGE You can specify a configuration when running NMAKE on this makefile
!MESSAGE by defining the macro CFG on the command line.  For example:
!MESSAGE 
!MESSAGE NMAKE /f "TAO_Baseline.mak" CFG="TAO_Baseline - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "TAO_Baseline - Win32 Release" (based on "Win32 (x86) Static Library")
!MESSAGE "TAO_Baseline - Win32 Debug" (based on "Win32 (x86) Static Library")
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
# PROP Target_Last_Scanned "TAO_Baseline - Win32 Debug"
CPP=cl.exe

!IF  "$(CFG)" == "TAO_Baseline - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "."
# PROP Intermediate_Dir "Release"
# PROP Target_Dir ""
OUTDIR=.\.
INTDIR=.\Release

ALL : "$(OUTDIR)\TAO_Baseline.lib"

CLEAN : 
	-@erase "$(INTDIR)\any.obj"
	-@erase "$(INTDIR)\cdr.obj"
	-@erase "$(INTDIR)\connmgr.obj"
	-@erase "$(INTDIR)\corbacom.obj"
	-@erase "$(INTDIR)\debug.obj"
	-@erase "$(INTDIR)\except.obj"
	-@erase "$(INTDIR)\giop.obj"
	-@erase "$(INTDIR)\iiopobj.obj"
	-@erase "$(INTDIR)\iioporb.obj"
	-@erase "$(INTDIR)\interp.obj"
	-@erase "$(INTDIR)\invoke.obj"
	-@erase "$(INTDIR)\marshal.obj"
	-@erase "$(INTDIR)\nvlist.obj"
	-@erase "$(INTDIR)\object.obj"
	-@erase "$(INTDIR)\orbobj.obj"
	-@erase "$(INTDIR)\principa.obj"
	-@erase "$(INTDIR)\request.obj"
	-@erase "$(INTDIR)\svrrqst.obj"
	-@erase "$(INTDIR)\tc_const.obj"
	-@erase "$(INTDIR)\tcpoa.obj"
	-@erase "$(INTDIR)\toa.obj"
	-@erase "$(INTDIR)\typecode.obj"
	-@erase "$(OUTDIR)\TAO_Baseline.lib"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

"$(INTDIR)" :
    if not exist "$(INTDIR)/$(NULL)" mkdir "$(INTDIR)"

# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /YX /c
# ADD CPP /nologo /MD /W3 /GX /O2 /I "\Ofp\Workspace\InfrastructureServices\PlatformServices\ACE_Wrappers" /I "." /D "NDEBUG" /D "WIN32" /D "_WINDOWS" /D "__IIOP_BUILD" /YX /c
CPP_PROJ=/nologo /MD /W3 /GX /O2 /I\
 "\Ofp\Workspace\InfrastructureServices\PlatformServices\ACE_Wrappers" /I "." /D\
 "NDEBUG" /D "WIN32" /D "_WINDOWS" /D "__IIOP_BUILD"\
 /Fp"$(INTDIR)/TAO_Baseline.pch" /YX /Fo"$(INTDIR)/" /c 
CPP_OBJS=.\Release/
CPP_SBRS=.\.
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
BSC32_FLAGS=/nologo /o"$(OUTDIR)/TAO_Baseline.bsc" 
BSC32_SBRS= \
	
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo
LIB32_FLAGS=/nologo /out:"$(OUTDIR)/TAO_Baseline.lib" 
LIB32_OBJS= \
	"$(INTDIR)\any.obj" \
	"$(INTDIR)\cdr.obj" \
	"$(INTDIR)\connmgr.obj" \
	"$(INTDIR)\corbacom.obj" \
	"$(INTDIR)\debug.obj" \
	"$(INTDIR)\except.obj" \
	"$(INTDIR)\giop.obj" \
	"$(INTDIR)\iiopobj.obj" \
	"$(INTDIR)\iioporb.obj" \
	"$(INTDIR)\interp.obj" \
	"$(INTDIR)\invoke.obj" \
	"$(INTDIR)\marshal.obj" \
	"$(INTDIR)\nvlist.obj" \
	"$(INTDIR)\object.obj" \
	"$(INTDIR)\orbobj.obj" \
	"$(INTDIR)\principa.obj" \
	"$(INTDIR)\request.obj" \
	"$(INTDIR)\svrrqst.obj" \
	"$(INTDIR)\tc_const.obj" \
	"$(INTDIR)\tcpoa.obj" \
	"$(INTDIR)\toa.obj" \
	"$(INTDIR)\typecode.obj" \
	"..\..\..\OFP\Workspace\libfiles\ace.lib"

"$(OUTDIR)\TAO_Baseline.lib" : "$(OUTDIR)" $(DEF_FILE) $(LIB32_OBJS)
    $(LIB32) @<<
  $(LIB32_FLAGS) $(DEF_FLAGS) $(LIB32_OBJS)
<<

!ELSEIF  "$(CFG)" == "TAO_Baseline - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "."
# PROP Intermediate_Dir "Debug"
# PROP Target_Dir ""
OUTDIR=.\.
INTDIR=.\Debug

ALL : "$(OUTDIR)\TAO_Baseline.lib"

CLEAN : 
	-@erase "$(INTDIR)\any.obj"
	-@erase "$(INTDIR)\cdr.obj"
	-@erase "$(INTDIR)\connmgr.obj"
	-@erase "$(INTDIR)\corbacom.obj"
	-@erase "$(INTDIR)\debug.obj"
	-@erase "$(INTDIR)\except.obj"
	-@erase "$(INTDIR)\giop.obj"
	-@erase "$(INTDIR)\iiopobj.obj"
	-@erase "$(INTDIR)\iioporb.obj"
	-@erase "$(INTDIR)\interp.obj"
	-@erase "$(INTDIR)\invoke.obj"
	-@erase "$(INTDIR)\marshal.obj"
	-@erase "$(INTDIR)\nvlist.obj"
	-@erase "$(INTDIR)\object.obj"
	-@erase "$(INTDIR)\orbobj.obj"
	-@erase "$(INTDIR)\principa.obj"
	-@erase "$(INTDIR)\request.obj"
	-@erase "$(INTDIR)\svrrqst.obj"
	-@erase "$(INTDIR)\tc_const.obj"
	-@erase "$(INTDIR)\tcpoa.obj"
	-@erase "$(INTDIR)\toa.obj"
	-@erase "$(INTDIR)\typecode.obj"
	-@erase "$(OUTDIR)\TAO_Baseline.lib"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

"$(INTDIR)" :
    if not exist "$(INTDIR)/$(NULL)" mkdir "$(INTDIR)"

# ADD BASE CPP /nologo /W3 /GX /Z7 /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /YX /c
# ADD CPP /nologo /MDd /W3 /GX /Z7 /Od /I "\Ofp\Workspace\InfrastructureServices\PlatformServices\ACE_Wrappers" /I "." /D "_DEBUG" /D "WIN32" /D "_WINDOWS" /D "__IIOP_BUILD" /YX /c
CPP_PROJ=/nologo /MDd /W3 /GX /Z7 /Od /I\
 "\Ofp\Workspace\InfrastructureServices\PlatformServices\ACE_Wrappers" /I "." /D\
 "_DEBUG" /D "WIN32" /D "_WINDOWS" /D "__IIOP_BUILD"\
 /Fp"$(INTDIR)/TAO_Baseline.pch" /YX /Fo"$(INTDIR)/" /c 
CPP_OBJS=.\Debug/
CPP_SBRS=.\.
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
BSC32_FLAGS=/nologo /o"$(OUTDIR)/TAO_Baseline.bsc" 
BSC32_SBRS= \
	
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo
LIB32_FLAGS=/nologo /out:"$(OUTDIR)/TAO_Baseline.lib" 
LIB32_OBJS= \
	"$(INTDIR)\any.obj" \
	"$(INTDIR)\cdr.obj" \
	"$(INTDIR)\connmgr.obj" \
	"$(INTDIR)\corbacom.obj" \
	"$(INTDIR)\debug.obj" \
	"$(INTDIR)\except.obj" \
	"$(INTDIR)\giop.obj" \
	"$(INTDIR)\iiopobj.obj" \
	"$(INTDIR)\iioporb.obj" \
	"$(INTDIR)\interp.obj" \
	"$(INTDIR)\invoke.obj" \
	"$(INTDIR)\marshal.obj" \
	"$(INTDIR)\nvlist.obj" \
	"$(INTDIR)\object.obj" \
	"$(INTDIR)\orbobj.obj" \
	"$(INTDIR)\principa.obj" \
	"$(INTDIR)\request.obj" \
	"$(INTDIR)\svrrqst.obj" \
	"$(INTDIR)\tc_const.obj" \
	"$(INTDIR)\tcpoa.obj" \
	"$(INTDIR)\toa.obj" \
	"$(INTDIR)\typecode.obj" \
	"..\..\..\OFP\Workspace\libfiles\ace.lib"

"$(OUTDIR)\TAO_Baseline.lib" : "$(OUTDIR)" $(DEF_FILE) $(LIB32_OBJS)
    $(LIB32) @<<
  $(LIB32_FLAGS) $(DEF_FLAGS) $(LIB32_OBJS)
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

# Name "TAO_Baseline - Win32 Release"
# Name "TAO_Baseline - Win32 Debug"

!IF  "$(CFG)" == "TAO_Baseline - Win32 Release"

!ELSEIF  "$(CFG)" == "TAO_Baseline - Win32 Debug"

!ENDIF 

################################################################################
# Begin Source File

SOURCE=.\cdr.cpp
DEP_CPP_CDR_C=\
	".\align.hh"\
	".\any.hh"\
	".\cdr.hh"\
	".\corbacom.hh"\
	".\debug.hh"\
	".\except.hh"\
	".\nvlist.hh"\
	".\object.hh"\
	".\orb.hh"\
	".\orbconf.hh"\
	".\orbobj.hh"\
	".\principa.hh"\
	".\request.hh"\
	".\sequence.hh"\
	".\svrrqst.hh"\
	".\typecode.hh"\
	"\Ofp\Workspace\InfrastructureServices\PlatformServices\ACE_Wrappers\ace\ACE.h"\
	"\Ofp\Workspace\InfrastructureServices\PlatformServices\ACE_Wrappers\ace\ACE.i"\
	"\Ofp\Workspace\InfrastructureServices\PlatformServices\ACE_Wrappers\ace\config.h"\
	"\Ofp\Workspace\InfrastructureServices\PlatformServices\ACE_Wrappers\ace\Log_Msg.h"\
	"\Ofp\Workspace\InfrastructureServices\PlatformServices\ACE_Wrappers\ace\Log_Priority.h"\
	"\Ofp\Workspace\InfrastructureServices\PlatformServices\ACE_Wrappers\ace\Log_Record.h"\
	"\Ofp\Workspace\InfrastructureServices\PlatformServices\ACE_Wrappers\ace\Log_Record.i"\
	"\Ofp\Workspace\InfrastructureServices\PlatformServices\ACE_Wrappers\ace\OS.h"\
	"\Ofp\Workspace\InfrastructureServices\PlatformServices\ACE_Wrappers\ace\OS.i"\
	"\Ofp\Workspace\InfrastructureServices\PlatformServices\ACE_Wrappers\ace\SString.h"\
	"\Ofp\Workspace\InfrastructureServices\PlatformServices\ACE_Wrappers\ace\SString.i"\
	"\Ofp\Workspace\InfrastructureServices\PlatformServices\ACE_Wrappers\ace\stdcpp.h"\
	"\Ofp\Workspace\InfrastructureServices\PlatformServices\ACE_Wrappers\ace\Trace.h"\
	"\Ofp\Workspace\InfrastructureServices\PlatformServices\ACE_Wrappers\ace\ws2tcpip.h"\
	

"$(INTDIR)\cdr.obj" : $(SOURCE) $(DEP_CPP_CDR_C) "$(INTDIR)"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\connmgr.cpp

!IF  "$(CFG)" == "TAO_Baseline - Win32 Release"

DEP_CPP_CONNM=\
	".\align.hh"\
	".\any.hh"\
	".\cdr.hh"\
	".\connmgr.hh"\
	".\corbacom.hh"\
	".\debug.hh"\
	".\except.hh"\
	".\nvlist.hh"\
	".\object.hh"\
	".\orb.hh"\
	".\orbconf.hh"\
	".\orbobj.hh"\
	".\principa.hh"\
	".\request.hh"\
	".\sequence.hh"\
	".\stub.hh"\
	".\svrrqst.hh"\
	".\thread.hh"\
	".\typecode.hh"\
	"\Ofp\Workspace\InfrastructureServices\PlatformServices\ACE_Wrappers\ace\ACE.h"\
	"\Ofp\Workspace\InfrastructureServices\PlatformServices\ACE_Wrappers\ace\ACE.i"\
	"\Ofp\Workspace\InfrastructureServices\PlatformServices\ACE_Wrappers\ace\config.h"\
	"\Ofp\Workspace\InfrastructureServices\PlatformServices\ACE_Wrappers\ace\Log_Msg.h"\
	"\Ofp\Workspace\InfrastructureServices\PlatformServices\ACE_Wrappers\ace\Log_Priority.h"\
	"\Ofp\Workspace\InfrastructureServices\PlatformServices\ACE_Wrappers\ace\Log_Record.h"\
	"\Ofp\Workspace\InfrastructureServices\PlatformServices\ACE_Wrappers\ace\Log_Record.i"\
	"\Ofp\Workspace\InfrastructureServices\PlatformServices\ACE_Wrappers\ace\OS.h"\
	"\Ofp\Workspace\InfrastructureServices\PlatformServices\ACE_Wrappers\ace\OS.i"\
	"\Ofp\Workspace\InfrastructureServices\PlatformServices\ACE_Wrappers\ace\SString.h"\
	"\Ofp\Workspace\InfrastructureServices\PlatformServices\ACE_Wrappers\ace\SString.i"\
	"\Ofp\Workspace\InfrastructureServices\PlatformServices\ACE_Wrappers\ace\stdcpp.h"\
	"\Ofp\Workspace\InfrastructureServices\PlatformServices\ACE_Wrappers\ace\Trace.h"\
	"\Ofp\Workspace\InfrastructureServices\PlatformServices\ACE_Wrappers\ace\ws2tcpip.h"\
	{$(INCLUDE)}"\sys\TYPES.H"\
	

"$(INTDIR)\connmgr.obj" : $(SOURCE) $(DEP_CPP_CONNM) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "TAO_Baseline - Win32 Debug"

DEP_CPP_CONNM=\
	".\align.hh"\
	".\any.hh"\
	".\cdr.hh"\
	".\connmgr.hh"\
	".\corbacom.hh"\
	".\debug.hh"\
	".\except.hh"\
	".\nvlist.hh"\
	".\object.hh"\
	".\orb.hh"\
	".\orbconf.hh"\
	".\orbobj.hh"\
	".\principa.hh"\
	".\request.hh"\
	".\sequence.hh"\
	".\stub.hh"\
	".\svrrqst.hh"\
	".\thread.hh"\
	".\typecode.hh"\
	"\Ofp\Workspace\InfrastructureServices\PlatformServices\ACE_Wrappers\ace\ACE.h"\
	"\Ofp\Workspace\InfrastructureServices\PlatformServices\ACE_Wrappers\ace\ACE.i"\
	"\Ofp\Workspace\InfrastructureServices\PlatformServices\ACE_Wrappers\ace\config.h"\
	"\Ofp\Workspace\InfrastructureServices\PlatformServices\ACE_Wrappers\ace\Log_Msg.h"\
	"\Ofp\Workspace\InfrastructureServices\PlatformServices\ACE_Wrappers\ace\Log_Priority.h"\
	"\Ofp\Workspace\InfrastructureServices\PlatformServices\ACE_Wrappers\ace\Log_Record.h"\
	"\Ofp\Workspace\InfrastructureServices\PlatformServices\ACE_Wrappers\ace\Log_Record.i"\
	"\Ofp\Workspace\InfrastructureServices\PlatformServices\ACE_Wrappers\ace\OS.h"\
	"\Ofp\Workspace\InfrastructureServices\PlatformServices\ACE_Wrappers\ace\OS.i"\
	"\Ofp\Workspace\InfrastructureServices\PlatformServices\ACE_Wrappers\ace\SString.h"\
	"\Ofp\Workspace\InfrastructureServices\PlatformServices\ACE_Wrappers\ace\SString.i"\
	"\Ofp\Workspace\InfrastructureServices\PlatformServices\ACE_Wrappers\ace\stdcpp.h"\
	"\Ofp\Workspace\InfrastructureServices\PlatformServices\ACE_Wrappers\ace\Trace.h"\
	"\Ofp\Workspace\InfrastructureServices\PlatformServices\ACE_Wrappers\ace\ws2tcpip.h"\
	{$(INCLUDE)}"\sys\TYPES.H"\
	

"$(INTDIR)\connmgr.obj" : $(SOURCE) $(DEP_CPP_CONNM) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\corbacom.cpp
DEP_CPP_CORBA=\
	".\align.hh"\
	".\any.hh"\
	".\cdr.hh"\
	".\corbacom.hh"\
	".\except.hh"\
	".\nvlist.hh"\
	".\object.hh"\
	".\orb.hh"\
	".\orbconf.hh"\
	".\orbobj.hh"\
	".\principa.hh"\
	".\request.hh"\
	".\sequence.hh"\
	".\svrrqst.hh"\
	".\typecode.hh"\
	"\Ofp\Workspace\InfrastructureServices\PlatformServices\ACE_Wrappers\ace\ACE.h"\
	"\Ofp\Workspace\InfrastructureServices\PlatformServices\ACE_Wrappers\ace\ACE.i"\
	"\Ofp\Workspace\InfrastructureServices\PlatformServices\ACE_Wrappers\ace\config.h"\
	"\Ofp\Workspace\InfrastructureServices\PlatformServices\ACE_Wrappers\ace\Log_Msg.h"\
	"\Ofp\Workspace\InfrastructureServices\PlatformServices\ACE_Wrappers\ace\Log_Priority.h"\
	"\Ofp\Workspace\InfrastructureServices\PlatformServices\ACE_Wrappers\ace\Log_Record.h"\
	"\Ofp\Workspace\InfrastructureServices\PlatformServices\ACE_Wrappers\ace\Log_Record.i"\
	"\Ofp\Workspace\InfrastructureServices\PlatformServices\ACE_Wrappers\ace\OS.h"\
	"\Ofp\Workspace\InfrastructureServices\PlatformServices\ACE_Wrappers\ace\OS.i"\
	"\Ofp\Workspace\InfrastructureServices\PlatformServices\ACE_Wrappers\ace\SString.h"\
	"\Ofp\Workspace\InfrastructureServices\PlatformServices\ACE_Wrappers\ace\SString.i"\
	"\Ofp\Workspace\InfrastructureServices\PlatformServices\ACE_Wrappers\ace\stdcpp.h"\
	"\Ofp\Workspace\InfrastructureServices\PlatformServices\ACE_Wrappers\ace\Trace.h"\
	"\Ofp\Workspace\InfrastructureServices\PlatformServices\ACE_Wrappers\ace\ws2tcpip.h"\
	

"$(INTDIR)\corbacom.obj" : $(SOURCE) $(DEP_CPP_CORBA) "$(INTDIR)"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\debug.cpp
DEP_CPP_DEBUG=\
	".\align.hh"\
	".\any.hh"\
	".\cdr.hh"\
	".\corbacom.hh"\
	".\debug.hh"\
	".\except.hh"\
	".\nvlist.hh"\
	".\object.hh"\
	".\orb.hh"\
	".\orbconf.hh"\
	".\orbobj.hh"\
	".\principa.hh"\
	".\request.hh"\
	".\sequence.hh"\
	".\svrrqst.hh"\
	".\thread.hh"\
	".\typecode.hh"\
	"\Ofp\Workspace\InfrastructureServices\PlatformServices\ACE_Wrappers\ace\ACE.h"\
	"\Ofp\Workspace\InfrastructureServices\PlatformServices\ACE_Wrappers\ace\ACE.i"\
	"\Ofp\Workspace\InfrastructureServices\PlatformServices\ACE_Wrappers\ace\config.h"\
	"\Ofp\Workspace\InfrastructureServices\PlatformServices\ACE_Wrappers\ace\Log_Msg.h"\
	"\Ofp\Workspace\InfrastructureServices\PlatformServices\ACE_Wrappers\ace\Log_Priority.h"\
	"\Ofp\Workspace\InfrastructureServices\PlatformServices\ACE_Wrappers\ace\Log_Record.h"\
	"\Ofp\Workspace\InfrastructureServices\PlatformServices\ACE_Wrappers\ace\Log_Record.i"\
	"\Ofp\Workspace\InfrastructureServices\PlatformServices\ACE_Wrappers\ace\OS.h"\
	"\Ofp\Workspace\InfrastructureServices\PlatformServices\ACE_Wrappers\ace\OS.i"\
	"\Ofp\Workspace\InfrastructureServices\PlatformServices\ACE_Wrappers\ace\SString.h"\
	"\Ofp\Workspace\InfrastructureServices\PlatformServices\ACE_Wrappers\ace\SString.i"\
	"\Ofp\Workspace\InfrastructureServices\PlatformServices\ACE_Wrappers\ace\stdcpp.h"\
	"\Ofp\Workspace\InfrastructureServices\PlatformServices\ACE_Wrappers\ace\Trace.h"\
	"\Ofp\Workspace\InfrastructureServices\PlatformServices\ACE_Wrappers\ace\ws2tcpip.h"\
	{$(INCLUDE)}"\sys\TYPES.H"\
	

"$(INTDIR)\debug.obj" : $(SOURCE) $(DEP_CPP_DEBUG) "$(INTDIR)"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\except.cpp
DEP_CPP_EXCEP=\
	".\align.hh"\
	".\any.hh"\
	".\cdr.hh"\
	".\corbacom.hh"\
	".\debug.hh"\
	".\except.hh"\
	".\nvlist.hh"\
	".\object.hh"\
	".\orb.hh"\
	".\orbconf.hh"\
	".\orbobj.hh"\
	".\principa.hh"\
	".\request.hh"\
	".\sequence.hh"\
	".\svrrqst.hh"\
	".\thread.hh"\
	".\typecode.hh"\
	"\Ofp\Workspace\InfrastructureServices\PlatformServices\ACE_Wrappers\ace\ACE.h"\
	"\Ofp\Workspace\InfrastructureServices\PlatformServices\ACE_Wrappers\ace\ACE.i"\
	"\Ofp\Workspace\InfrastructureServices\PlatformServices\ACE_Wrappers\ace\config.h"\
	"\Ofp\Workspace\InfrastructureServices\PlatformServices\ACE_Wrappers\ace\Log_Msg.h"\
	"\Ofp\Workspace\InfrastructureServices\PlatformServices\ACE_Wrappers\ace\Log_Priority.h"\
	"\Ofp\Workspace\InfrastructureServices\PlatformServices\ACE_Wrappers\ace\Log_Record.h"\
	"\Ofp\Workspace\InfrastructureServices\PlatformServices\ACE_Wrappers\ace\Log_Record.i"\
	"\Ofp\Workspace\InfrastructureServices\PlatformServices\ACE_Wrappers\ace\OS.h"\
	"\Ofp\Workspace\InfrastructureServices\PlatformServices\ACE_Wrappers\ace\OS.i"\
	"\Ofp\Workspace\InfrastructureServices\PlatformServices\ACE_Wrappers\ace\SString.h"\
	"\Ofp\Workspace\InfrastructureServices\PlatformServices\ACE_Wrappers\ace\SString.i"\
	"\Ofp\Workspace\InfrastructureServices\PlatformServices\ACE_Wrappers\ace\stdcpp.h"\
	"\Ofp\Workspace\InfrastructureServices\PlatformServices\ACE_Wrappers\ace\Trace.h"\
	"\Ofp\Workspace\InfrastructureServices\PlatformServices\ACE_Wrappers\ace\ws2tcpip.h"\
	

"$(INTDIR)\except.obj" : $(SOURCE) $(DEP_CPP_EXCEP) "$(INTDIR)"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\giop.cpp
DEP_CPP_GIOP_=\
	".\align.hh"\
	".\any.hh"\
	".\cdr.hh"\
	".\connmgr.hh"\
	".\corbacom.hh"\
	".\debug.hh"\
	".\except.hh"\
	".\giop.hh"\
	".\iiopobj.hh"\
	".\nvlist.hh"\
	".\object.hh"\
	".\orb.hh"\
	".\orbconf.hh"\
	".\orbobj.hh"\
	".\principa.hh"\
	".\request.hh"\
	".\sequence.hh"\
	".\stub.hh"\
	".\svrrqst.hh"\
	".\thread.hh"\
	".\typecode.hh"\
	"\Ofp\Workspace\InfrastructureServices\PlatformServices\ACE_Wrappers\ace\ACE.h"\
	"\Ofp\Workspace\InfrastructureServices\PlatformServices\ACE_Wrappers\ace\ACE.i"\
	"\Ofp\Workspace\InfrastructureServices\PlatformServices\ACE_Wrappers\ace\config.h"\
	"\Ofp\Workspace\InfrastructureServices\PlatformServices\ACE_Wrappers\ace\Log_Msg.h"\
	"\Ofp\Workspace\InfrastructureServices\PlatformServices\ACE_Wrappers\ace\Log_Priority.h"\
	"\Ofp\Workspace\InfrastructureServices\PlatformServices\ACE_Wrappers\ace\Log_Record.h"\
	"\Ofp\Workspace\InfrastructureServices\PlatformServices\ACE_Wrappers\ace\Log_Record.i"\
	"\Ofp\Workspace\InfrastructureServices\PlatformServices\ACE_Wrappers\ace\OS.h"\
	"\Ofp\Workspace\InfrastructureServices\PlatformServices\ACE_Wrappers\ace\OS.i"\
	"\Ofp\Workspace\InfrastructureServices\PlatformServices\ACE_Wrappers\ace\SString.h"\
	"\Ofp\Workspace\InfrastructureServices\PlatformServices\ACE_Wrappers\ace\SString.i"\
	"\Ofp\Workspace\InfrastructureServices\PlatformServices\ACE_Wrappers\ace\stdcpp.h"\
	"\Ofp\Workspace\InfrastructureServices\PlatformServices\ACE_Wrappers\ace\Trace.h"\
	"\Ofp\Workspace\InfrastructureServices\PlatformServices\ACE_Wrappers\ace\ws2tcpip.h"\
	{$(INCLUDE)}"\sys\TYPES.H"\
	

"$(INTDIR)\giop.obj" : $(SOURCE) $(DEP_CPP_GIOP_) "$(INTDIR)"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\iiopobj.cpp
DEP_CPP_IIOPO=\
	".\align.hh"\
	".\any.hh"\
	".\cdr.hh"\
	".\corbacom.hh"\
	".\except.hh"\
	".\iiopobj.hh"\
	".\nvlist.hh"\
	".\object.hh"\
	".\orb.hh"\
	".\orbconf.hh"\
	".\orbobj.hh"\
	".\principa.hh"\
	".\request.hh"\
	".\sequence.hh"\
	".\stub.hh"\
	".\svrrqst.hh"\
	".\thread.hh"\
	".\typecode.hh"\
	"\Ofp\Workspace\InfrastructureServices\PlatformServices\ACE_Wrappers\ace\ACE.h"\
	"\Ofp\Workspace\InfrastructureServices\PlatformServices\ACE_Wrappers\ace\ACE.i"\
	"\Ofp\Workspace\InfrastructureServices\PlatformServices\ACE_Wrappers\ace\config.h"\
	"\Ofp\Workspace\InfrastructureServices\PlatformServices\ACE_Wrappers\ace\Log_Msg.h"\
	"\Ofp\Workspace\InfrastructureServices\PlatformServices\ACE_Wrappers\ace\Log_Priority.h"\
	"\Ofp\Workspace\InfrastructureServices\PlatformServices\ACE_Wrappers\ace\Log_Record.h"\
	"\Ofp\Workspace\InfrastructureServices\PlatformServices\ACE_Wrappers\ace\Log_Record.i"\
	"\Ofp\Workspace\InfrastructureServices\PlatformServices\ACE_Wrappers\ace\OS.h"\
	"\Ofp\Workspace\InfrastructureServices\PlatformServices\ACE_Wrappers\ace\OS.i"\
	"\Ofp\Workspace\InfrastructureServices\PlatformServices\ACE_Wrappers\ace\SString.h"\
	"\Ofp\Workspace\InfrastructureServices\PlatformServices\ACE_Wrappers\ace\SString.i"\
	"\Ofp\Workspace\InfrastructureServices\PlatformServices\ACE_Wrappers\ace\stdcpp.h"\
	"\Ofp\Workspace\InfrastructureServices\PlatformServices\ACE_Wrappers\ace\Trace.h"\
	"\Ofp\Workspace\InfrastructureServices\PlatformServices\ACE_Wrappers\ace\ws2tcpip.h"\
	

"$(INTDIR)\iiopobj.obj" : $(SOURCE) $(DEP_CPP_IIOPO) "$(INTDIR)"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\iioporb.cpp
DEP_CPP_IIOPOR=\
	".\align.hh"\
	".\any.hh"\
	".\cdr.hh"\
	".\corbacom.hh"\
	".\except.hh"\
	".\iiopobj.hh"\
	".\iioporb.hh"\
	".\nvlist.hh"\
	".\object.hh"\
	".\orb.hh"\
	".\orbconf.hh"\
	".\orbobj.hh"\
	".\principa.hh"\
	".\request.hh"\
	".\sequence.hh"\
	".\stub.hh"\
	".\svrrqst.hh"\
	".\typecode.hh"\
	"\Ofp\Workspace\InfrastructureServices\PlatformServices\ACE_Wrappers\ace\ACE.h"\
	"\Ofp\Workspace\InfrastructureServices\PlatformServices\ACE_Wrappers\ace\ACE.i"\
	"\Ofp\Workspace\InfrastructureServices\PlatformServices\ACE_Wrappers\ace\config.h"\
	"\Ofp\Workspace\InfrastructureServices\PlatformServices\ACE_Wrappers\ace\Log_Msg.h"\
	"\Ofp\Workspace\InfrastructureServices\PlatformServices\ACE_Wrappers\ace\Log_Priority.h"\
	"\Ofp\Workspace\InfrastructureServices\PlatformServices\ACE_Wrappers\ace\Log_Record.h"\
	"\Ofp\Workspace\InfrastructureServices\PlatformServices\ACE_Wrappers\ace\Log_Record.i"\
	"\Ofp\Workspace\InfrastructureServices\PlatformServices\ACE_Wrappers\ace\OS.h"\
	"\Ofp\Workspace\InfrastructureServices\PlatformServices\ACE_Wrappers\ace\OS.i"\
	"\Ofp\Workspace\InfrastructureServices\PlatformServices\ACE_Wrappers\ace\SString.h"\
	"\Ofp\Workspace\InfrastructureServices\PlatformServices\ACE_Wrappers\ace\SString.i"\
	"\Ofp\Workspace\InfrastructureServices\PlatformServices\ACE_Wrappers\ace\stdcpp.h"\
	"\Ofp\Workspace\InfrastructureServices\PlatformServices\ACE_Wrappers\ace\Trace.h"\
	"\Ofp\Workspace\InfrastructureServices\PlatformServices\ACE_Wrappers\ace\ws2tcpip.h"\
	

"$(INTDIR)\iioporb.obj" : $(SOURCE) $(DEP_CPP_IIOPOR) "$(INTDIR)"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\interp.cpp
DEP_CPP_INTER=\
	".\align.hh"\
	".\any.hh"\
	".\cdr.hh"\
	".\corbacom.hh"\
	".\debug.hh"\
	".\except.hh"\
	".\nvlist.hh"\
	".\object.hh"\
	".\orb.hh"\
	".\orbconf.hh"\
	".\orbobj.hh"\
	".\principa.hh"\
	".\request.hh"\
	".\sequence.hh"\
	".\svrrqst.hh"\
	".\typecode.hh"\
	"\Ofp\Workspace\InfrastructureServices\PlatformServices\ACE_Wrappers\ace\ACE.h"\
	"\Ofp\Workspace\InfrastructureServices\PlatformServices\ACE_Wrappers\ace\ACE.i"\
	"\Ofp\Workspace\InfrastructureServices\PlatformServices\ACE_Wrappers\ace\config.h"\
	"\Ofp\Workspace\InfrastructureServices\PlatformServices\ACE_Wrappers\ace\Log_Msg.h"\
	"\Ofp\Workspace\InfrastructureServices\PlatformServices\ACE_Wrappers\ace\Log_Priority.h"\
	"\Ofp\Workspace\InfrastructureServices\PlatformServices\ACE_Wrappers\ace\Log_Record.h"\
	"\Ofp\Workspace\InfrastructureServices\PlatformServices\ACE_Wrappers\ace\Log_Record.i"\
	"\Ofp\Workspace\InfrastructureServices\PlatformServices\ACE_Wrappers\ace\OS.h"\
	"\Ofp\Workspace\InfrastructureServices\PlatformServices\ACE_Wrappers\ace\OS.i"\
	"\Ofp\Workspace\InfrastructureServices\PlatformServices\ACE_Wrappers\ace\SString.h"\
	"\Ofp\Workspace\InfrastructureServices\PlatformServices\ACE_Wrappers\ace\SString.i"\
	"\Ofp\Workspace\InfrastructureServices\PlatformServices\ACE_Wrappers\ace\stdcpp.h"\
	"\Ofp\Workspace\InfrastructureServices\PlatformServices\ACE_Wrappers\ace\Trace.h"\
	"\Ofp\Workspace\InfrastructureServices\PlatformServices\ACE_Wrappers\ace\ws2tcpip.h"\
	

"$(INTDIR)\interp.obj" : $(SOURCE) $(DEP_CPP_INTER) "$(INTDIR)"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\invoke.cpp
DEP_CPP_INVOK=\
	".\align.hh"\
	".\any.hh"\
	".\cdr.hh"\
	".\connmgr.hh"\
	".\corbacom.hh"\
	".\debug.hh"\
	".\except.hh"\
	".\giop.hh"\
	".\iiopobj.hh"\
	".\nvlist.hh"\
	".\object.hh"\
	".\orb.hh"\
	".\orbconf.hh"\
	".\orbobj.hh"\
	".\principa.hh"\
	".\request.hh"\
	".\sequence.hh"\
	".\stub.hh"\
	".\svrrqst.hh"\
	".\thread.hh"\
	".\typecode.hh"\
	"\Ofp\Workspace\InfrastructureServices\PlatformServices\ACE_Wrappers\ace\ACE.h"\
	"\Ofp\Workspace\InfrastructureServices\PlatformServices\ACE_Wrappers\ace\ACE.i"\
	"\Ofp\Workspace\InfrastructureServices\PlatformServices\ACE_Wrappers\ace\config.h"\
	"\Ofp\Workspace\InfrastructureServices\PlatformServices\ACE_Wrappers\ace\Log_Msg.h"\
	"\Ofp\Workspace\InfrastructureServices\PlatformServices\ACE_Wrappers\ace\Log_Priority.h"\
	"\Ofp\Workspace\InfrastructureServices\PlatformServices\ACE_Wrappers\ace\Log_Record.h"\
	"\Ofp\Workspace\InfrastructureServices\PlatformServices\ACE_Wrappers\ace\Log_Record.i"\
	"\Ofp\Workspace\InfrastructureServices\PlatformServices\ACE_Wrappers\ace\OS.h"\
	"\Ofp\Workspace\InfrastructureServices\PlatformServices\ACE_Wrappers\ace\OS.i"\
	"\Ofp\Workspace\InfrastructureServices\PlatformServices\ACE_Wrappers\ace\SString.h"\
	"\Ofp\Workspace\InfrastructureServices\PlatformServices\ACE_Wrappers\ace\SString.i"\
	"\Ofp\Workspace\InfrastructureServices\PlatformServices\ACE_Wrappers\ace\stdcpp.h"\
	"\Ofp\Workspace\InfrastructureServices\PlatformServices\ACE_Wrappers\ace\Trace.h"\
	"\Ofp\Workspace\InfrastructureServices\PlatformServices\ACE_Wrappers\ace\ws2tcpip.h"\
	

"$(INTDIR)\invoke.obj" : $(SOURCE) $(DEP_CPP_INVOK) "$(INTDIR)"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\marshal.cpp
DEP_CPP_MARSH=\
	".\align.hh"\
	".\any.hh"\
	".\cdr.hh"\
	".\connmgr.hh"\
	".\corbacom.hh"\
	".\debug.hh"\
	".\except.hh"\
	".\giop.hh"\
	".\iiopobj.hh"\
	".\nvlist.hh"\
	".\object.hh"\
	".\orb.hh"\
	".\orbconf.hh"\
	".\orbobj.hh"\
	".\principa.hh"\
	".\request.hh"\
	".\sequence.hh"\
	".\stub.hh"\
	".\svrrqst.hh"\
	".\typecode.hh"\
	"\Ofp\Workspace\InfrastructureServices\PlatformServices\ACE_Wrappers\ace\ACE.h"\
	"\Ofp\Workspace\InfrastructureServices\PlatformServices\ACE_Wrappers\ace\ACE.i"\
	"\Ofp\Workspace\InfrastructureServices\PlatformServices\ACE_Wrappers\ace\config.h"\
	"\Ofp\Workspace\InfrastructureServices\PlatformServices\ACE_Wrappers\ace\Log_Msg.h"\
	"\Ofp\Workspace\InfrastructureServices\PlatformServices\ACE_Wrappers\ace\Log_Priority.h"\
	"\Ofp\Workspace\InfrastructureServices\PlatformServices\ACE_Wrappers\ace\Log_Record.h"\
	"\Ofp\Workspace\InfrastructureServices\PlatformServices\ACE_Wrappers\ace\Log_Record.i"\
	"\Ofp\Workspace\InfrastructureServices\PlatformServices\ACE_Wrappers\ace\OS.h"\
	"\Ofp\Workspace\InfrastructureServices\PlatformServices\ACE_Wrappers\ace\OS.i"\
	"\Ofp\Workspace\InfrastructureServices\PlatformServices\ACE_Wrappers\ace\SString.h"\
	"\Ofp\Workspace\InfrastructureServices\PlatformServices\ACE_Wrappers\ace\SString.i"\
	"\Ofp\Workspace\InfrastructureServices\PlatformServices\ACE_Wrappers\ace\stdcpp.h"\
	"\Ofp\Workspace\InfrastructureServices\PlatformServices\ACE_Wrappers\ace\Trace.h"\
	"\Ofp\Workspace\InfrastructureServices\PlatformServices\ACE_Wrappers\ace\ws2tcpip.h"\
	

"$(INTDIR)\marshal.obj" : $(SOURCE) $(DEP_CPP_MARSH) "$(INTDIR)"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\nvlist.cpp
DEP_CPP_NVLIS=\
	".\align.hh"\
	".\any.hh"\
	".\cdr.hh"\
	".\corbacom.hh"\
	".\debug.hh"\
	".\except.hh"\
	".\nvlist.hh"\
	".\object.hh"\
	".\orb.hh"\
	".\orbconf.hh"\
	".\orbobj.hh"\
	".\principa.hh"\
	".\request.hh"\
	".\sequence.hh"\
	".\svrrqst.hh"\
	".\thread.hh"\
	".\typecode.hh"\
	"\Ofp\Workspace\InfrastructureServices\PlatformServices\ACE_Wrappers\ace\ACE.h"\
	"\Ofp\Workspace\InfrastructureServices\PlatformServices\ACE_Wrappers\ace\ACE.i"\
	"\Ofp\Workspace\InfrastructureServices\PlatformServices\ACE_Wrappers\ace\config.h"\
	"\Ofp\Workspace\InfrastructureServices\PlatformServices\ACE_Wrappers\ace\Log_Msg.h"\
	"\Ofp\Workspace\InfrastructureServices\PlatformServices\ACE_Wrappers\ace\Log_Priority.h"\
	"\Ofp\Workspace\InfrastructureServices\PlatformServices\ACE_Wrappers\ace\Log_Record.h"\
	"\Ofp\Workspace\InfrastructureServices\PlatformServices\ACE_Wrappers\ace\Log_Record.i"\
	"\Ofp\Workspace\InfrastructureServices\PlatformServices\ACE_Wrappers\ace\OS.h"\
	"\Ofp\Workspace\InfrastructureServices\PlatformServices\ACE_Wrappers\ace\OS.i"\
	"\Ofp\Workspace\InfrastructureServices\PlatformServices\ACE_Wrappers\ace\SString.h"\
	"\Ofp\Workspace\InfrastructureServices\PlatformServices\ACE_Wrappers\ace\SString.i"\
	"\Ofp\Workspace\InfrastructureServices\PlatformServices\ACE_Wrappers\ace\stdcpp.h"\
	"\Ofp\Workspace\InfrastructureServices\PlatformServices\ACE_Wrappers\ace\Trace.h"\
	"\Ofp\Workspace\InfrastructureServices\PlatformServices\ACE_Wrappers\ace\ws2tcpip.h"\
	

"$(INTDIR)\nvlist.obj" : $(SOURCE) $(DEP_CPP_NVLIS) "$(INTDIR)"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\object.cpp
DEP_CPP_OBJEC=\
	".\align.hh"\
	".\any.hh"\
	".\cdr.hh"\
	".\corbacom.hh"\
	".\debug.hh"\
	".\except.hh"\
	".\nvlist.hh"\
	".\object.hh"\
	".\orb.hh"\
	".\orbconf.hh"\
	".\orbobj.hh"\
	".\principa.hh"\
	".\request.hh"\
	".\sequence.hh"\
	".\stub.hh"\
	".\svrrqst.hh"\
	".\typecode.hh"\
	"\Ofp\Workspace\InfrastructureServices\PlatformServices\ACE_Wrappers\ace\ACE.h"\
	"\Ofp\Workspace\InfrastructureServices\PlatformServices\ACE_Wrappers\ace\ACE.i"\
	"\Ofp\Workspace\InfrastructureServices\PlatformServices\ACE_Wrappers\ace\config.h"\
	"\Ofp\Workspace\InfrastructureServices\PlatformServices\ACE_Wrappers\ace\Log_Msg.h"\
	"\Ofp\Workspace\InfrastructureServices\PlatformServices\ACE_Wrappers\ace\Log_Priority.h"\
	"\Ofp\Workspace\InfrastructureServices\PlatformServices\ACE_Wrappers\ace\Log_Record.h"\
	"\Ofp\Workspace\InfrastructureServices\PlatformServices\ACE_Wrappers\ace\Log_Record.i"\
	"\Ofp\Workspace\InfrastructureServices\PlatformServices\ACE_Wrappers\ace\OS.h"\
	"\Ofp\Workspace\InfrastructureServices\PlatformServices\ACE_Wrappers\ace\OS.i"\
	"\Ofp\Workspace\InfrastructureServices\PlatformServices\ACE_Wrappers\ace\SString.h"\
	"\Ofp\Workspace\InfrastructureServices\PlatformServices\ACE_Wrappers\ace\SString.i"\
	"\Ofp\Workspace\InfrastructureServices\PlatformServices\ACE_Wrappers\ace\stdcpp.h"\
	"\Ofp\Workspace\InfrastructureServices\PlatformServices\ACE_Wrappers\ace\Trace.h"\
	"\Ofp\Workspace\InfrastructureServices\PlatformServices\ACE_Wrappers\ace\ws2tcpip.h"\
	

"$(INTDIR)\object.obj" : $(SOURCE) $(DEP_CPP_OBJEC) "$(INTDIR)"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\orbobj.cpp
DEP_CPP_ORBOB=\
	".\align.hh"\
	".\any.hh"\
	".\cdr.hh"\
	".\corbacom.hh"\
	".\debug.hh"\
	".\except.hh"\
	".\iioporb.hh"\
	".\nvlist.hh"\
	".\object.hh"\
	".\orb.hh"\
	".\orbconf.hh"\
	".\orbobj.hh"\
	".\principa.hh"\
	".\request.hh"\
	".\sequence.hh"\
	".\stub.hh"\
	".\svrrqst.hh"\
	".\thread.hh"\
	".\typecode.hh"\
	"\Ofp\Workspace\InfrastructureServices\PlatformServices\ACE_Wrappers\ace\ACE.h"\
	"\Ofp\Workspace\InfrastructureServices\PlatformServices\ACE_Wrappers\ace\ACE.i"\
	"\Ofp\Workspace\InfrastructureServices\PlatformServices\ACE_Wrappers\ace\config.h"\
	"\Ofp\Workspace\InfrastructureServices\PlatformServices\ACE_Wrappers\ace\Log_Msg.h"\
	"\Ofp\Workspace\InfrastructureServices\PlatformServices\ACE_Wrappers\ace\Log_Priority.h"\
	"\Ofp\Workspace\InfrastructureServices\PlatformServices\ACE_Wrappers\ace\Log_Record.h"\
	"\Ofp\Workspace\InfrastructureServices\PlatformServices\ACE_Wrappers\ace\Log_Record.i"\
	"\Ofp\Workspace\InfrastructureServices\PlatformServices\ACE_Wrappers\ace\OS.h"\
	"\Ofp\Workspace\InfrastructureServices\PlatformServices\ACE_Wrappers\ace\OS.i"\
	"\Ofp\Workspace\InfrastructureServices\PlatformServices\ACE_Wrappers\ace\SString.h"\
	"\Ofp\Workspace\InfrastructureServices\PlatformServices\ACE_Wrappers\ace\SString.i"\
	"\Ofp\Workspace\InfrastructureServices\PlatformServices\ACE_Wrappers\ace\stdcpp.h"\
	"\Ofp\Workspace\InfrastructureServices\PlatformServices\ACE_Wrappers\ace\Trace.h"\
	"\Ofp\Workspace\InfrastructureServices\PlatformServices\ACE_Wrappers\ace\ws2tcpip.h"\
	

"$(INTDIR)\orbobj.obj" : $(SOURCE) $(DEP_CPP_ORBOB) "$(INTDIR)"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\principa.cpp
DEP_CPP_PRINC=\
	".\align.hh"\
	".\any.hh"\
	".\cdr.hh"\
	".\corbacom.hh"\
	".\except.hh"\
	".\nvlist.hh"\
	".\object.hh"\
	".\orb.hh"\
	".\orbconf.hh"\
	".\orbobj.hh"\
	".\principa.hh"\
	".\request.hh"\
	".\sequence.hh"\
	".\svrrqst.hh"\
	".\thread.hh"\
	".\typecode.hh"\
	"\Ofp\Workspace\InfrastructureServices\PlatformServices\ACE_Wrappers\ace\ACE.h"\
	"\Ofp\Workspace\InfrastructureServices\PlatformServices\ACE_Wrappers\ace\ACE.i"\
	"\Ofp\Workspace\InfrastructureServices\PlatformServices\ACE_Wrappers\ace\config.h"\
	"\Ofp\Workspace\InfrastructureServices\PlatformServices\ACE_Wrappers\ace\Log_Msg.h"\
	"\Ofp\Workspace\InfrastructureServices\PlatformServices\ACE_Wrappers\ace\Log_Priority.h"\
	"\Ofp\Workspace\InfrastructureServices\PlatformServices\ACE_Wrappers\ace\Log_Record.h"\
	"\Ofp\Workspace\InfrastructureServices\PlatformServices\ACE_Wrappers\ace\Log_Record.i"\
	"\Ofp\Workspace\InfrastructureServices\PlatformServices\ACE_Wrappers\ace\OS.h"\
	"\Ofp\Workspace\InfrastructureServices\PlatformServices\ACE_Wrappers\ace\OS.i"\
	"\Ofp\Workspace\InfrastructureServices\PlatformServices\ACE_Wrappers\ace\SString.h"\
	"\Ofp\Workspace\InfrastructureServices\PlatformServices\ACE_Wrappers\ace\SString.i"\
	"\Ofp\Workspace\InfrastructureServices\PlatformServices\ACE_Wrappers\ace\stdcpp.h"\
	"\Ofp\Workspace\InfrastructureServices\PlatformServices\ACE_Wrappers\ace\Trace.h"\
	"\Ofp\Workspace\InfrastructureServices\PlatformServices\ACE_Wrappers\ace\ws2tcpip.h"\
	

"$(INTDIR)\principa.obj" : $(SOURCE) $(DEP_CPP_PRINC) "$(INTDIR)"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\request.cpp
DEP_CPP_REQUE=\
	".\align.hh"\
	".\any.hh"\
	".\cdr.hh"\
	".\corbacom.hh"\
	".\debug.hh"\
	".\except.hh"\
	".\nvlist.hh"\
	".\object.hh"\
	".\orb.hh"\
	".\orbconf.hh"\
	".\orbobj.hh"\
	".\principa.hh"\
	".\request.hh"\
	".\sequence.hh"\
	".\stub.hh"\
	".\svrrqst.hh"\
	".\thread.hh"\
	".\typecode.hh"\
	"\Ofp\Workspace\InfrastructureServices\PlatformServices\ACE_Wrappers\ace\ACE.h"\
	"\Ofp\Workspace\InfrastructureServices\PlatformServices\ACE_Wrappers\ace\ACE.i"\
	"\Ofp\Workspace\InfrastructureServices\PlatformServices\ACE_Wrappers\ace\config.h"\
	"\Ofp\Workspace\InfrastructureServices\PlatformServices\ACE_Wrappers\ace\Log_Msg.h"\
	"\Ofp\Workspace\InfrastructureServices\PlatformServices\ACE_Wrappers\ace\Log_Priority.h"\
	"\Ofp\Workspace\InfrastructureServices\PlatformServices\ACE_Wrappers\ace\Log_Record.h"\
	"\Ofp\Workspace\InfrastructureServices\PlatformServices\ACE_Wrappers\ace\Log_Record.i"\
	"\Ofp\Workspace\InfrastructureServices\PlatformServices\ACE_Wrappers\ace\OS.h"\
	"\Ofp\Workspace\InfrastructureServices\PlatformServices\ACE_Wrappers\ace\OS.i"\
	"\Ofp\Workspace\InfrastructureServices\PlatformServices\ACE_Wrappers\ace\SString.h"\
	"\Ofp\Workspace\InfrastructureServices\PlatformServices\ACE_Wrappers\ace\SString.i"\
	"\Ofp\Workspace\InfrastructureServices\PlatformServices\ACE_Wrappers\ace\stdcpp.h"\
	"\Ofp\Workspace\InfrastructureServices\PlatformServices\ACE_Wrappers\ace\Trace.h"\
	"\Ofp\Workspace\InfrastructureServices\PlatformServices\ACE_Wrappers\ace\ws2tcpip.h"\
	

"$(INTDIR)\request.obj" : $(SOURCE) $(DEP_CPP_REQUE) "$(INTDIR)"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\svrrqst.cpp
DEP_CPP_SVRRQ=\
	".\align.hh"\
	".\any.hh"\
	".\cdr.hh"\
	".\corbacom.hh"\
	".\debug.hh"\
	".\except.hh"\
	".\nvlist.hh"\
	".\object.hh"\
	".\orb.hh"\
	".\orbconf.hh"\
	".\orbobj.hh"\
	".\principa.hh"\
	".\request.hh"\
	".\sequence.hh"\
	".\svrrqst.hh"\
	".\thread.hh"\
	".\typecode.hh"\
	"\Ofp\Workspace\InfrastructureServices\PlatformServices\ACE_Wrappers\ace\ACE.h"\
	"\Ofp\Workspace\InfrastructureServices\PlatformServices\ACE_Wrappers\ace\ACE.i"\
	"\Ofp\Workspace\InfrastructureServices\PlatformServices\ACE_Wrappers\ace\config.h"\
	"\Ofp\Workspace\InfrastructureServices\PlatformServices\ACE_Wrappers\ace\Log_Msg.h"\
	"\Ofp\Workspace\InfrastructureServices\PlatformServices\ACE_Wrappers\ace\Log_Priority.h"\
	"\Ofp\Workspace\InfrastructureServices\PlatformServices\ACE_Wrappers\ace\Log_Record.h"\
	"\Ofp\Workspace\InfrastructureServices\PlatformServices\ACE_Wrappers\ace\Log_Record.i"\
	"\Ofp\Workspace\InfrastructureServices\PlatformServices\ACE_Wrappers\ace\OS.h"\
	"\Ofp\Workspace\InfrastructureServices\PlatformServices\ACE_Wrappers\ace\OS.i"\
	"\Ofp\Workspace\InfrastructureServices\PlatformServices\ACE_Wrappers\ace\SString.h"\
	"\Ofp\Workspace\InfrastructureServices\PlatformServices\ACE_Wrappers\ace\SString.i"\
	"\Ofp\Workspace\InfrastructureServices\PlatformServices\ACE_Wrappers\ace\stdcpp.h"\
	"\Ofp\Workspace\InfrastructureServices\PlatformServices\ACE_Wrappers\ace\Trace.h"\
	"\Ofp\Workspace\InfrastructureServices\PlatformServices\ACE_Wrappers\ace\ws2tcpip.h"\
	

"$(INTDIR)\svrrqst.obj" : $(SOURCE) $(DEP_CPP_SVRRQ) "$(INTDIR)"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\tc_const.cpp
DEP_CPP_TC_CO=\
	".\align.hh"\
	".\any.hh"\
	".\cdr.hh"\
	".\corbacom.hh"\
	".\except.hh"\
	".\nvlist.hh"\
	".\object.hh"\
	".\orb.hh"\
	".\orbconf.hh"\
	".\orbobj.hh"\
	".\principa.hh"\
	".\request.hh"\
	".\sequence.hh"\
	".\svrrqst.hh"\
	".\typecode.hh"\
	"\Ofp\Workspace\InfrastructureServices\PlatformServices\ACE_Wrappers\ace\ACE.h"\
	"\Ofp\Workspace\InfrastructureServices\PlatformServices\ACE_Wrappers\ace\ACE.i"\
	"\Ofp\Workspace\InfrastructureServices\PlatformServices\ACE_Wrappers\ace\config.h"\
	"\Ofp\Workspace\InfrastructureServices\PlatformServices\ACE_Wrappers\ace\Log_Msg.h"\
	"\Ofp\Workspace\InfrastructureServices\PlatformServices\ACE_Wrappers\ace\Log_Priority.h"\
	"\Ofp\Workspace\InfrastructureServices\PlatformServices\ACE_Wrappers\ace\Log_Record.h"\
	"\Ofp\Workspace\InfrastructureServices\PlatformServices\ACE_Wrappers\ace\Log_Record.i"\
	"\Ofp\Workspace\InfrastructureServices\PlatformServices\ACE_Wrappers\ace\OS.h"\
	"\Ofp\Workspace\InfrastructureServices\PlatformServices\ACE_Wrappers\ace\OS.i"\
	"\Ofp\Workspace\InfrastructureServices\PlatformServices\ACE_Wrappers\ace\SString.h"\
	"\Ofp\Workspace\InfrastructureServices\PlatformServices\ACE_Wrappers\ace\SString.i"\
	"\Ofp\Workspace\InfrastructureServices\PlatformServices\ACE_Wrappers\ace\stdcpp.h"\
	"\Ofp\Workspace\InfrastructureServices\PlatformServices\ACE_Wrappers\ace\Trace.h"\
	"\Ofp\Workspace\InfrastructureServices\PlatformServices\ACE_Wrappers\ace\ws2tcpip.h"\
	

"$(INTDIR)\tc_const.obj" : $(SOURCE) $(DEP_CPP_TC_CO) "$(INTDIR)"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\tcpoa.cpp
DEP_CPP_TCPOA=\
	".\align.hh"\
	".\any.hh"\
	".\cdr.hh"\
	".\connmgr.hh"\
	".\corbacom.hh"\
	".\debug.hh"\
	".\except.hh"\
	".\giop.hh"\
	".\iiopobj.hh"\
	".\nvlist.hh"\
	".\object.hh"\
	".\orb.hh"\
	".\orbconf.hh"\
	".\orbobj.hh"\
	".\principa.hh"\
	".\request.hh"\
	".\sequence.hh"\
	".\stub.hh"\
	".\svrrqst.hh"\
	".\tcpoa.hh"\
	".\thread.hh"\
	".\toa.hh"\
	".\typecode.hh"\
	"\Ofp\Workspace\InfrastructureServices\PlatformServices\ACE_Wrappers\ace\ACE.h"\
	"\Ofp\Workspace\InfrastructureServices\PlatformServices\ACE_Wrappers\ace\ACE.i"\
	"\Ofp\Workspace\InfrastructureServices\PlatformServices\ACE_Wrappers\ace\config.h"\
	"\Ofp\Workspace\InfrastructureServices\PlatformServices\ACE_Wrappers\ace\Log_Msg.h"\
	"\Ofp\Workspace\InfrastructureServices\PlatformServices\ACE_Wrappers\ace\Log_Priority.h"\
	"\Ofp\Workspace\InfrastructureServices\PlatformServices\ACE_Wrappers\ace\Log_Record.h"\
	"\Ofp\Workspace\InfrastructureServices\PlatformServices\ACE_Wrappers\ace\Log_Record.i"\
	"\Ofp\Workspace\InfrastructureServices\PlatformServices\ACE_Wrappers\ace\OS.h"\
	"\Ofp\Workspace\InfrastructureServices\PlatformServices\ACE_Wrappers\ace\OS.i"\
	"\Ofp\Workspace\InfrastructureServices\PlatformServices\ACE_Wrappers\ace\SString.h"\
	"\Ofp\Workspace\InfrastructureServices\PlatformServices\ACE_Wrappers\ace\SString.i"\
	"\Ofp\Workspace\InfrastructureServices\PlatformServices\ACE_Wrappers\ace\stdcpp.h"\
	"\Ofp\Workspace\InfrastructureServices\PlatformServices\ACE_Wrappers\ace\Trace.h"\
	"\Ofp\Workspace\InfrastructureServices\PlatformServices\ACE_Wrappers\ace\ws2tcpip.h"\
	

"$(INTDIR)\tcpoa.obj" : $(SOURCE) $(DEP_CPP_TCPOA) "$(INTDIR)"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\toa.cpp
DEP_CPP_TOA_C=\
	".\align.hh"\
	".\any.hh"\
	".\cdr.hh"\
	".\connmgr.hh"\
	".\corbacom.hh"\
	".\debug.hh"\
	".\except.hh"\
	".\iioporb.hh"\
	".\nvlist.hh"\
	".\object.hh"\
	".\orb.hh"\
	".\orbconf.hh"\
	".\orbobj.hh"\
	".\principa.hh"\
	".\request.hh"\
	".\sequence.hh"\
	".\svrrqst.hh"\
	".\tcpoa.hh"\
	".\thread.hh"\
	".\toa.hh"\
	".\typecode.hh"\
	"\Ofp\Workspace\InfrastructureServices\PlatformServices\ACE_Wrappers\ace\ACE.h"\
	"\Ofp\Workspace\InfrastructureServices\PlatformServices\ACE_Wrappers\ace\ACE.i"\
	"\Ofp\Workspace\InfrastructureServices\PlatformServices\ACE_Wrappers\ace\config.h"\
	"\Ofp\Workspace\InfrastructureServices\PlatformServices\ACE_Wrappers\ace\Log_Msg.h"\
	"\Ofp\Workspace\InfrastructureServices\PlatformServices\ACE_Wrappers\ace\Log_Priority.h"\
	"\Ofp\Workspace\InfrastructureServices\PlatformServices\ACE_Wrappers\ace\Log_Record.h"\
	"\Ofp\Workspace\InfrastructureServices\PlatformServices\ACE_Wrappers\ace\Log_Record.i"\
	"\Ofp\Workspace\InfrastructureServices\PlatformServices\ACE_Wrappers\ace\OS.h"\
	"\Ofp\Workspace\InfrastructureServices\PlatformServices\ACE_Wrappers\ace\OS.i"\
	"\Ofp\Workspace\InfrastructureServices\PlatformServices\ACE_Wrappers\ace\SString.h"\
	"\Ofp\Workspace\InfrastructureServices\PlatformServices\ACE_Wrappers\ace\SString.i"\
	"\Ofp\Workspace\InfrastructureServices\PlatformServices\ACE_Wrappers\ace\stdcpp.h"\
	"\Ofp\Workspace\InfrastructureServices\PlatformServices\ACE_Wrappers\ace\Trace.h"\
	"\Ofp\Workspace\InfrastructureServices\PlatformServices\ACE_Wrappers\ace\ws2tcpip.h"\
	

"$(INTDIR)\toa.obj" : $(SOURCE) $(DEP_CPP_TOA_C) "$(INTDIR)"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\typecode.cpp
DEP_CPP_TYPEC=\
	".\align.hh"\
	".\any.hh"\
	".\cdr.hh"\
	".\corbacom.hh"\
	".\debug.hh"\
	".\except.hh"\
	".\nvlist.hh"\
	".\object.hh"\
	".\orb.hh"\
	".\orbconf.hh"\
	".\orbobj.hh"\
	".\principa.hh"\
	".\request.hh"\
	".\sequence.hh"\
	".\svrrqst.hh"\
	".\thread.hh"\
	".\typecode.hh"\
	"\Ofp\Workspace\InfrastructureServices\PlatformServices\ACE_Wrappers\ace\ACE.h"\
	"\Ofp\Workspace\InfrastructureServices\PlatformServices\ACE_Wrappers\ace\ACE.i"\
	"\Ofp\Workspace\InfrastructureServices\PlatformServices\ACE_Wrappers\ace\config.h"\
	"\Ofp\Workspace\InfrastructureServices\PlatformServices\ACE_Wrappers\ace\Log_Msg.h"\
	"\Ofp\Workspace\InfrastructureServices\PlatformServices\ACE_Wrappers\ace\Log_Priority.h"\
	"\Ofp\Workspace\InfrastructureServices\PlatformServices\ACE_Wrappers\ace\Log_Record.h"\
	"\Ofp\Workspace\InfrastructureServices\PlatformServices\ACE_Wrappers\ace\Log_Record.i"\
	"\Ofp\Workspace\InfrastructureServices\PlatformServices\ACE_Wrappers\ace\OS.h"\
	"\Ofp\Workspace\InfrastructureServices\PlatformServices\ACE_Wrappers\ace\OS.i"\
	"\Ofp\Workspace\InfrastructureServices\PlatformServices\ACE_Wrappers\ace\SString.h"\
	"\Ofp\Workspace\InfrastructureServices\PlatformServices\ACE_Wrappers\ace\SString.i"\
	"\Ofp\Workspace\InfrastructureServices\PlatformServices\ACE_Wrappers\ace\stdcpp.h"\
	"\Ofp\Workspace\InfrastructureServices\PlatformServices\ACE_Wrappers\ace\Trace.h"\
	"\Ofp\Workspace\InfrastructureServices\PlatformServices\ACE_Wrappers\ace\ws2tcpip.h"\
	{$(INCLUDE)}"\sys\TYPES.H"\
	

"$(INTDIR)\typecode.obj" : $(SOURCE) $(DEP_CPP_TYPEC) "$(INTDIR)"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\any.cpp
DEP_CPP_ANY_C=\
	".\align.hh"\
	".\any.hh"\
	".\cdr.hh"\
	".\corbacom.hh"\
	".\debug.hh"\
	".\except.hh"\
	".\nvlist.hh"\
	".\object.hh"\
	".\orb.hh"\
	".\orbconf.hh"\
	".\orbobj.hh"\
	".\principa.hh"\
	".\request.hh"\
	".\sequence.hh"\
	".\svrrqst.hh"\
	".\thread.hh"\
	".\typecode.hh"\
	"\Ofp\Workspace\InfrastructureServices\PlatformServices\ACE_Wrappers\ace\ACE.h"\
	"\Ofp\Workspace\InfrastructureServices\PlatformServices\ACE_Wrappers\ace\ACE.i"\
	"\Ofp\Workspace\InfrastructureServices\PlatformServices\ACE_Wrappers\ace\config.h"\
	"\Ofp\Workspace\InfrastructureServices\PlatformServices\ACE_Wrappers\ace\Log_Msg.h"\
	"\Ofp\Workspace\InfrastructureServices\PlatformServices\ACE_Wrappers\ace\Log_Priority.h"\
	"\Ofp\Workspace\InfrastructureServices\PlatformServices\ACE_Wrappers\ace\Log_Record.h"\
	"\Ofp\Workspace\InfrastructureServices\PlatformServices\ACE_Wrappers\ace\Log_Record.i"\
	"\Ofp\Workspace\InfrastructureServices\PlatformServices\ACE_Wrappers\ace\OS.h"\
	"\Ofp\Workspace\InfrastructureServices\PlatformServices\ACE_Wrappers\ace\OS.i"\
	"\Ofp\Workspace\InfrastructureServices\PlatformServices\ACE_Wrappers\ace\SString.h"\
	"\Ofp\Workspace\InfrastructureServices\PlatformServices\ACE_Wrappers\ace\SString.i"\
	"\Ofp\Workspace\InfrastructureServices\PlatformServices\ACE_Wrappers\ace\stdcpp.h"\
	"\Ofp\Workspace\InfrastructureServices\PlatformServices\ACE_Wrappers\ace\Trace.h"\
	"\Ofp\Workspace\InfrastructureServices\PlatformServices\ACE_Wrappers\ace\ws2tcpip.h"\
	

"$(INTDIR)\any.obj" : $(SOURCE) $(DEP_CPP_ANY_C) "$(INTDIR)"


# End Source File
################################################################################
# Begin Source File

SOURCE=\OFP\Workspace\libfiles\ace.lib

!IF  "$(CFG)" == "TAO_Baseline - Win32 Release"

!ELSEIF  "$(CFG)" == "TAO_Baseline - Win32 Debug"

!ENDIF 

# End Source File
# End Target
# End Project
################################################################################
