# Microsoft Developer Studio Generated NMAKE File, Format Version 4.20
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Console Application" 0x0103

!IF "$(CFG)" == ""
CFG=webclient - Win32 Debug
!MESSAGE No configuration specified.  Defaulting to webclient - Win32 Debug.
!ENDIF 

!IF "$(CFG)" != "webclient - Win32 Release" && "$(CFG)" !=\
 "webclient - Win32 Debug"
!MESSAGE Invalid configuration "$(CFG)" specified.
!MESSAGE You can specify a configuration when running NMAKE on this makefile
!MESSAGE by defining the macro CFG on the command line.  For example:
!MESSAGE 
!MESSAGE NMAKE /f "webclient.mak" CFG="webclient - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "webclient - Win32 Release" (based on\
 "Win32 (x86) Console Application")
!MESSAGE "webclient - Win32 Debug" (based on "Win32 (x86) Console Application")
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
# PROP Target_Last_Scanned "webclient - Win32 Debug"
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "webclient - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "WinRel"
# PROP BASE Intermediate_Dir "WinRel"
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "WinRel"
# PROP Intermediate_Dir "WinRel"
OUTDIR=.\WinRel
INTDIR=.\WinRel

ALL : "$(OUTDIR)\webclient.exe" "$(OUTDIR)\webclient.bsc"

CLEAN : 
	-@erase "$(INTDIR)\bench.obj"
	-@erase "$(INTDIR)\bench.sbr"
	-@erase "$(INTDIR)\errexit.obj"
	-@erase "$(INTDIR)\errexit.sbr"
	-@erase "$(INTDIR)\get.obj"
	-@erase "$(INTDIR)\get.sbr"
	-@erase "$(INTDIR)\getopt.obj"
	-@erase "$(INTDIR)\getopt.sbr"
	-@erase "$(INTDIR)\gettimeofday.obj"
	-@erase "$(INTDIR)\gettimeofday.sbr"
	-@erase "$(INTDIR)\parse_file_list.obj"
	-@erase "$(INTDIR)\parse_file_list.sbr"
	-@erase "$(INTDIR)\statistics.obj"
	-@erase "$(INTDIR)\statistics.sbr"
	-@erase "$(INTDIR)\sysdep.obj"
	-@erase "$(INTDIR)\sysdep.sbr"
	-@erase "$(INTDIR)\timefunc.obj"
	-@erase "$(INTDIR)\timefunc.sbr"
	-@erase "$(INTDIR)\webclient.obj"
	-@erase "$(INTDIR)\webclient.sbr"
	-@erase "$(OUTDIR)\webclient.bsc"
	-@erase "$(OUTDIR)\webclient.exe"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /FR /YX /c
# ADD CPP /nologo /MTd /W3 /GX /O2 /D "NDEBUG" /D "WIN32" /D "_CONSOLE" /FR /YX /c
CPP_PROJ=/nologo /MTd /W3 /GX /O2 /D "NDEBUG" /D "WIN32" /D "_CONSOLE"\
 /FR"$(INTDIR)/" /Fp"$(INTDIR)/webclient.pch" /YX /Fo"$(INTDIR)/" /c 
CPP_OBJS=.\WinRel/
CPP_SBRS=.\WinRel/
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
BSC32_FLAGS=/nologo /o"$(OUTDIR)/webclient.bsc" 
BSC32_SBRS= \
	"$(INTDIR)\bench.sbr" \
	"$(INTDIR)\errexit.sbr" \
	"$(INTDIR)\get.sbr" \
	"$(INTDIR)\getopt.sbr" \
	"$(INTDIR)\gettimeofday.sbr" \
	"$(INTDIR)\parse_file_list.sbr" \
	"$(INTDIR)\statistics.sbr" \
	"$(INTDIR)\sysdep.sbr" \
	"$(INTDIR)\timefunc.sbr" \
	"$(INTDIR)\webclient.sbr"

"$(OUTDIR)\webclient.bsc" : "$(OUTDIR)" $(BSC32_SBRS)
    $(BSC32) @<<
  $(BSC32_FLAGS) $(BSC32_SBRS)
<<

LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /machine:I386
# ADD LINK32 wsock32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /machine:I386
# SUBTRACT LINK32 /profile
LINK32_FLAGS=wsock32.lib kernel32.lib user32.lib gdi32.lib winspool.lib\
 comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib\
 odbc32.lib odbccp32.lib /nologo /subsystem:console /incremental:no\
 /pdb:"$(OUTDIR)/webclient.pdb" /machine:I386 /out:"$(OUTDIR)/webclient.exe" 
LINK32_OBJS= \
	"$(INTDIR)\bench.obj" \
	"$(INTDIR)\errexit.obj" \
	"$(INTDIR)\get.obj" \
	"$(INTDIR)\getopt.obj" \
	"$(INTDIR)\gettimeofday.obj" \
	"$(INTDIR)\parse_file_list.obj" \
	"$(INTDIR)\statistics.obj" \
	"$(INTDIR)\sysdep.obj" \
	"$(INTDIR)\timefunc.obj" \
	"$(INTDIR)\webclient.obj"

"$(OUTDIR)\webclient.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ELSEIF  "$(CFG)" == "webclient - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "WinDebug"
# PROP BASE Intermediate_Dir "WinDebug"
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "WinDebug"
# PROP Intermediate_Dir "WinDebug"
OUTDIR=.\WinDebug
INTDIR=.\WinDebug

ALL : "$(OUTDIR)\webclient.exe" "$(OUTDIR)\webclient.bsc"

CLEAN : 
	-@erase "$(INTDIR)\bench.obj"
	-@erase "$(INTDIR)\bench.sbr"
	-@erase "$(INTDIR)\errexit.obj"
	-@erase "$(INTDIR)\errexit.sbr"
	-@erase "$(INTDIR)\get.obj"
	-@erase "$(INTDIR)\get.sbr"
	-@erase "$(INTDIR)\getopt.obj"
	-@erase "$(INTDIR)\getopt.sbr"
	-@erase "$(INTDIR)\gettimeofday.obj"
	-@erase "$(INTDIR)\gettimeofday.sbr"
	-@erase "$(INTDIR)\parse_file_list.obj"
	-@erase "$(INTDIR)\parse_file_list.sbr"
	-@erase "$(INTDIR)\statistics.obj"
	-@erase "$(INTDIR)\statistics.sbr"
	-@erase "$(INTDIR)\sysdep.obj"
	-@erase "$(INTDIR)\sysdep.sbr"
	-@erase "$(INTDIR)\timefunc.obj"
	-@erase "$(INTDIR)\timefunc.sbr"
	-@erase "$(INTDIR)\vc40.idb"
	-@erase "$(INTDIR)\vc40.pdb"
	-@erase "$(INTDIR)\webclient.obj"
	-@erase "$(INTDIR)\webclient.sbr"
	-@erase "$(OUTDIR)\webclient.bsc"
	-@erase "$(OUTDIR)\webclient.exe"
	-@erase "$(OUTDIR)\webclient.ilk"
	-@erase "$(OUTDIR)\webclient.pdb"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

# ADD BASE CPP /nologo /W3 /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /FR /YX /c
# ADD CPP /nologo /MTd /W3 /Gm /GX /Zi /Od /D "_DEBUG" /D "WIN32" /D "_CONSOLE" /FR /YX /c
CPP_PROJ=/nologo /MTd /W3 /Gm /GX /Zi /Od /D "_DEBUG" /D "WIN32" /D "_CONSOLE"\
 /FR"$(INTDIR)/" /Fp"$(INTDIR)/webclient.pch" /YX /Fo"$(INTDIR)/"\
 /Fd"$(INTDIR)/" /c 
CPP_OBJS=.\WinDebug/
CPP_SBRS=.\WinDebug/
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
BSC32_FLAGS=/nologo /o"$(OUTDIR)/webclient.bsc" 
BSC32_SBRS= \
	"$(INTDIR)\bench.sbr" \
	"$(INTDIR)\errexit.sbr" \
	"$(INTDIR)\get.sbr" \
	"$(INTDIR)\getopt.sbr" \
	"$(INTDIR)\gettimeofday.sbr" \
	"$(INTDIR)\parse_file_list.sbr" \
	"$(INTDIR)\statistics.sbr" \
	"$(INTDIR)\sysdep.sbr" \
	"$(INTDIR)\timefunc.sbr" \
	"$(INTDIR)\webclient.sbr"

"$(OUTDIR)\webclient.bsc" : "$(OUTDIR)" $(BSC32_SBRS)
    $(BSC32) @<<
  $(BSC32_FLAGS) $(BSC32_SBRS)
<<

LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /debug /machine:I386
# ADD LINK32 wsock32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /debug /machine:I386
# SUBTRACT LINK32 /profile
LINK32_FLAGS=wsock32.lib kernel32.lib user32.lib gdi32.lib winspool.lib\
 comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib\
 odbc32.lib odbccp32.lib /nologo /subsystem:console /incremental:yes\
 /pdb:"$(OUTDIR)/webclient.pdb" /debug /machine:I386\
 /out:"$(OUTDIR)/webclient.exe" 
LINK32_OBJS= \
	"$(INTDIR)\bench.obj" \
	"$(INTDIR)\errexit.obj" \
	"$(INTDIR)\get.obj" \
	"$(INTDIR)\getopt.obj" \
	"$(INTDIR)\gettimeofday.obj" \
	"$(INTDIR)\parse_file_list.obj" \
	"$(INTDIR)\statistics.obj" \
	"$(INTDIR)\sysdep.obj" \
	"$(INTDIR)\timefunc.obj" \
	"$(INTDIR)\webclient.obj"

"$(OUTDIR)\webclient.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
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

# Name "webclient - Win32 Release"
# Name "webclient - Win32 Debug"

!IF  "$(CFG)" == "webclient - Win32 Release"

!ELSEIF  "$(CFG)" == "webclient - Win32 Debug"

!ENDIF 

################################################################################
# Begin Source File

SOURCE=..\webclient.c
DEP_CPP_WEBCL=\
	"..\bench.h"\
	"..\config.h"\
	"..\sysdep.h"\
	{$(INCLUDE)}"\sys\TYPES.H"\
	

BuildCmds= \
	$(CPP) $(CPP_PROJ) $(SOURCE) \
	

"$(INTDIR)\webclient.obj" : $(SOURCE) $(DEP_CPP_WEBCL) "$(INTDIR)"
   $(BuildCmds)

"$(INTDIR)\webclient.sbr" : $(SOURCE) $(DEP_CPP_WEBCL) "$(INTDIR)"
   $(BuildCmds)

# End Source File
################################################################################
# Begin Source File

SOURCE=..\timefunc.c
DEP_CPP_TIMEF=\
	"..\bench.h"\
	"..\config.h"\
	"..\sysdep.h"\
	

BuildCmds= \
	$(CPP) $(CPP_PROJ) $(SOURCE) \
	

"$(INTDIR)\timefunc.obj" : $(SOURCE) $(DEP_CPP_TIMEF) "$(INTDIR)"
   $(BuildCmds)

"$(INTDIR)\timefunc.sbr" : $(SOURCE) $(DEP_CPP_TIMEF) "$(INTDIR)"
   $(BuildCmds)

# End Source File
################################################################################
# Begin Source File

SOURCE=..\sysdep.c
DEP_CPP_SYSDE=\
	"..\config.h"\
	"..\sysdep.h"\
	

BuildCmds= \
	$(CPP) $(CPP_PROJ) $(SOURCE) \
	

"$(INTDIR)\sysdep.obj" : $(SOURCE) $(DEP_CPP_SYSDE) "$(INTDIR)"
   $(BuildCmds)

"$(INTDIR)\sysdep.sbr" : $(SOURCE) $(DEP_CPP_SYSDE) "$(INTDIR)"
   $(BuildCmds)

# End Source File
################################################################################
# Begin Source File

SOURCE=..\statistics.c
DEP_CPP_STATI=\
	"..\bench.h"\
	"..\config.h"\
	"..\sysdep.h"\
	

BuildCmds= \
	$(CPP) $(CPP_PROJ) $(SOURCE) \
	

"$(INTDIR)\statistics.obj" : $(SOURCE) $(DEP_CPP_STATI) "$(INTDIR)"
   $(BuildCmds)

"$(INTDIR)\statistics.sbr" : $(SOURCE) $(DEP_CPP_STATI) "$(INTDIR)"
   $(BuildCmds)

# End Source File
################################################################################
# Begin Source File

SOURCE=..\parse_file_list.c
DEP_CPP_PARSE=\
	"..\bench.h"\
	"..\config.h"\
	"..\sysdep.h"\
	{$(INCLUDE)}"\sys\TYPES.H"\
	

BuildCmds= \
	$(CPP) $(CPP_PROJ) $(SOURCE) \
	

"$(INTDIR)\parse_file_list.obj" : $(SOURCE) $(DEP_CPP_PARSE) "$(INTDIR)"
   $(BuildCmds)

"$(INTDIR)\parse_file_list.sbr" : $(SOURCE) $(DEP_CPP_PARSE) "$(INTDIR)"
   $(BuildCmds)

# End Source File
################################################################################
# Begin Source File

SOURCE=..\gettimeofday.c
DEP_CPP_GETTI=\
	{$(INCLUDE)}"\sys\TIMEB.H"\
	

BuildCmds= \
	$(CPP) $(CPP_PROJ) $(SOURCE) \
	

"$(INTDIR)\gettimeofday.obj" : $(SOURCE) $(DEP_CPP_GETTI) "$(INTDIR)"
   $(BuildCmds)

"$(INTDIR)\gettimeofday.sbr" : $(SOURCE) $(DEP_CPP_GETTI) "$(INTDIR)"
   $(BuildCmds)

# End Source File
################################################################################
# Begin Source File

SOURCE=..\getopt.c

BuildCmds= \
	$(CPP) $(CPP_PROJ) $(SOURCE) \
	

"$(INTDIR)\getopt.obj" : $(SOURCE) "$(INTDIR)"
   $(BuildCmds)

"$(INTDIR)\getopt.sbr" : $(SOURCE) "$(INTDIR)"
   $(BuildCmds)

# End Source File
################################################################################
# Begin Source File

SOURCE=..\get.c
DEP_CPP_GET_C=\
	"..\bench.h"\
	"..\config.h"\
	"..\sysdep.h"\
	{$(INCLUDE)}"\sys\STAT.H"\
	{$(INCLUDE)}"\sys\TYPES.H"\
	

BuildCmds= \
	$(CPP) $(CPP_PROJ) $(SOURCE) \
	

"$(INTDIR)\get.obj" : $(SOURCE) $(DEP_CPP_GET_C) "$(INTDIR)"
   $(BuildCmds)

"$(INTDIR)\get.sbr" : $(SOURCE) $(DEP_CPP_GET_C) "$(INTDIR)"
   $(BuildCmds)

# End Source File
################################################################################
# Begin Source File

SOURCE=..\errexit.c
DEP_CPP_ERREX=\
	"..\bench.h"\
	"..\config.h"\
	"..\sysdep.h"\
	{$(INCLUDE)}"\sys\TYPES.H"\
	

BuildCmds= \
	$(CPP) $(CPP_PROJ) $(SOURCE) \
	

"$(INTDIR)\errexit.obj" : $(SOURCE) $(DEP_CPP_ERREX) "$(INTDIR)"
   $(BuildCmds)

"$(INTDIR)\errexit.sbr" : $(SOURCE) $(DEP_CPP_ERREX) "$(INTDIR)"
   $(BuildCmds)

# End Source File
################################################################################
# Begin Source File

SOURCE=..\bench.c
DEP_CPP_BENCH=\
	"..\bench.h"\
	"..\config.h"\
	"..\sysdep.h"\
	{$(INCLUDE)}"\sys\TYPES.H"\
	

BuildCmds= \
	$(CPP) $(CPP_PROJ) $(SOURCE) \
	

"$(INTDIR)\bench.obj" : $(SOURCE) $(DEP_CPP_BENCH) "$(INTDIR)"
   $(BuildCmds)

"$(INTDIR)\bench.sbr" : $(SOURCE) $(DEP_CPP_BENCH) "$(INTDIR)"
   $(BuildCmds)

# End Source File
# End Target
# End Project
################################################################################
