# Microsoft Developer Studio Generated NMAKE File, Format Version 4.10
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Console Application" 0x0103

!IF "$(CFG)" == ""
CFG=Webmaster - Win32 Debug
!MESSAGE No configuration specified.  Defaulting to Webmaster - Win32 Debug.
!ENDIF 

!IF "$(CFG)" != "Webmaster - Win32 Release" && "$(CFG)" !=\
 "Webmaster - Win32 Debug"
!MESSAGE Invalid configuration "$(CFG)" specified.
!MESSAGE You can specify a configuration when running NMAKE on this makefile
!MESSAGE by defining the macro CFG on the command line.  For example:
!MESSAGE 
!MESSAGE NMAKE /f "webmaster.mak" CFG="Webmaster - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "Webmaster - Win32 Release" (based on\
 "Win32 (x86) Console Application")
!MESSAGE "Webmaster - Win32 Debug" (based on "Win32 (x86) Console Application")
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
# PROP Target_Last_Scanned "Webmaster - Win32 Debug"
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "Webmaster - Win32 Release"

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

ALL : ".\WinRel\webmaster.exe" ".\WinRel\webmaster.bsc"

CLEAN : 
	-@erase ".\WinRel\bench.obj"
	-@erase ".\WinRel\bench.sbr"
	-@erase ".\WinRel\errexit.obj"
	-@erase ".\WinRel\errexit.sbr"
	-@erase ".\WinRel\getopt.obj"
	-@erase ".\WinRel\getopt.sbr"
	-@erase ".\WinRel\parse_file_list.obj"
	-@erase ".\WinRel\parse_file_list.sbr"
	-@erase ".\WinRel\rexec.obj"
	-@erase ".\WinRel\rexec.sbr"
	-@erase ".\WinRel\statistics.obj"
	-@erase ".\WinRel\statistics.sbr"
	-@erase ".\WinRel\sysdep.obj"
	-@erase ".\WinRel\sysdep.sbr"
	-@erase ".\WinRel\timefunc.obj"
	-@erase ".\WinRel\timefunc.sbr"
	-@erase ".\WinRel\webmaster.bsc"
	-@erase ".\WinRel\webmaster.exe"
	-@erase ".\WinRel\webmaster.obj"
	-@erase ".\WinRel\webmaster.sbr"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /FR /YX /c
# ADD CPP /nologo /MTd /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /FR /YX /c
CPP_PROJ=/nologo /MTd /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_CONSOLE"\
 /FR"$(INTDIR)/" /Fp"$(INTDIR)/webmaster.pch" /YX /Fo"$(INTDIR)/" /c 
CPP_OBJS=.\WinRel/
CPP_SBRS=.\WinRel/
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
BSC32_FLAGS=/nologo /o"$(OUTDIR)/webmaster.bsc" 
BSC32_SBRS= \
	".\WinRel\bench.sbr" \
	".\WinRel\errexit.sbr" \
	".\WinRel\getopt.sbr" \
	".\WinRel\parse_file_list.sbr" \
	".\WinRel\rexec.sbr" \
	".\WinRel\statistics.sbr" \
	".\WinRel\sysdep.sbr" \
	".\WinRel\timefunc.sbr" \
	".\WinRel\webmaster.sbr"

".\WinRel\webmaster.bsc" : "$(OUTDIR)" $(BSC32_SBRS)
    $(BSC32) @<<
  $(BSC32_FLAGS) $(BSC32_SBRS)
<<

LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /machine:I386
# ADD LINK32 wsock32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /machine:I386
LINK32_FLAGS=wsock32.lib kernel32.lib user32.lib gdi32.lib winspool.lib\
 comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib\
 odbc32.lib odbccp32.lib /nologo /subsystem:console /incremental:no\
 /pdb:"$(OUTDIR)/webmaster.pdb" /machine:I386 /out:"$(OUTDIR)/webmaster.exe" 
LINK32_OBJS= \
	".\WinRel\bench.obj" \
	".\WinRel\errexit.obj" \
	".\WinRel\getopt.obj" \
	".\WinRel\parse_file_list.obj" \
	".\WinRel\rexec.obj" \
	".\WinRel\statistics.obj" \
	".\WinRel\sysdep.obj" \
	".\WinRel\timefunc.obj" \
	".\WinRel\webmaster.obj"

".\WinRel\webmaster.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ELSEIF  "$(CFG)" == "Webmaster - Win32 Debug"

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

ALL : ".\WinDebug\webmaster.exe" ".\WinDebug\webmaster.bsc"

CLEAN : 
	-@erase ".\WinDebug\bench.obj"
	-@erase ".\WinDebug\bench.sbr"
	-@erase ".\WinDebug\errexit.obj"
	-@erase ".\WinDebug\errexit.sbr"
	-@erase ".\WinDebug\getopt.obj"
	-@erase ".\WinDebug\getopt.sbr"
	-@erase ".\WinDebug\parse_file_list.obj"
	-@erase ".\WinDebug\parse_file_list.sbr"
	-@erase ".\WinDebug\rexec.obj"
	-@erase ".\WinDebug\rexec.sbr"
	-@erase ".\WinDebug\statistics.obj"
	-@erase ".\WinDebug\statistics.sbr"
	-@erase ".\WinDebug\sysdep.obj"
	-@erase ".\WinDebug\sysdep.sbr"
	-@erase ".\WinDebug\timefunc.obj"
	-@erase ".\WinDebug\timefunc.sbr"
	-@erase ".\WinDebug\vc40.idb"
	-@erase ".\WinDebug\vc40.pdb"
	-@erase ".\WinDebug\webmaster.bsc"
	-@erase ".\WinDebug\webmaster.exe"
	-@erase ".\WinDebug\webmaster.ilk"
	-@erase ".\WinDebug\webmaster.obj"
	-@erase ".\WinDebug\webmaster.pdb"
	-@erase ".\WinDebug\webmaster.sbr"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

# ADD BASE CPP /nologo /W3 /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /FR /YX /c
# ADD CPP /nologo /MTd /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /FR /YX /c
CPP_PROJ=/nologo /MTd /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_CONSOLE"\
 /FR"$(INTDIR)/" /Fp"$(INTDIR)/webmaster.pch" /YX /Fo"$(INTDIR)/"\
 /Fd"$(INTDIR)/" /c 
CPP_OBJS=.\WinDebug/
CPP_SBRS=.\WinDebug/
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
BSC32_FLAGS=/nologo /o"$(OUTDIR)/webmaster.bsc" 
BSC32_SBRS= \
	".\WinDebug\bench.sbr" \
	".\WinDebug\errexit.sbr" \
	".\WinDebug\getopt.sbr" \
	".\WinDebug\parse_file_list.sbr" \
	".\WinDebug\rexec.sbr" \
	".\WinDebug\statistics.sbr" \
	".\WinDebug\sysdep.sbr" \
	".\WinDebug\timefunc.sbr" \
	".\WinDebug\webmaster.sbr"

".\WinDebug\webmaster.bsc" : "$(OUTDIR)" $(BSC32_SBRS)
    $(BSC32) @<<
  $(BSC32_FLAGS) $(BSC32_SBRS)
<<

LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /debug /machine:I386
# ADD LINK32 wsock32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /debug /machine:I386
# SUBTRACT LINK32 /pdb:none
LINK32_FLAGS=wsock32.lib kernel32.lib user32.lib gdi32.lib winspool.lib\
 comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib\
 odbc32.lib odbccp32.lib /nologo /subsystem:console /incremental:yes\
 /pdb:"$(OUTDIR)/webmaster.pdb" /debug /machine:I386\
 /out:"$(OUTDIR)/webmaster.exe" 
LINK32_OBJS= \
	".\WinDebug\bench.obj" \
	".\WinDebug\errexit.obj" \
	".\WinDebug\getopt.obj" \
	".\WinDebug\parse_file_list.obj" \
	".\WinDebug\rexec.obj" \
	".\WinDebug\statistics.obj" \
	".\WinDebug\sysdep.obj" \
	".\WinDebug\timefunc.obj" \
	".\WinDebug\webmaster.obj"

".\WinDebug\webmaster.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
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

# Name "Webmaster - Win32 Release"
# Name "Webmaster - Win32 Debug"

!IF  "$(CFG)" == "Webmaster - Win32 Release"

!ELSEIF  "$(CFG)" == "Webmaster - Win32 Debug"

!ENDIF 

################################################################################
# Begin Source File

SOURCE=..\bench.c
DEP_CPP_BENCH=\
	"..\bench.h"\
	"..\sysdep.h"\
	{$(INCLUDE)}"\sys\TYPES.H"\
	
NODEP_CPP_BENCH=\
	"..\config.h"\
	

!IF  "$(CFG)" == "Webmaster - Win32 Release"


BuildCmds= \
	$(CPP) $(CPP_PROJ) $(SOURCE) \
	

".\WinRel\bench.obj" : $(SOURCE) $(DEP_CPP_BENCH) "$(INTDIR)"
   $(BuildCmds)

".\WinRel\bench.sbr" : $(SOURCE) $(DEP_CPP_BENCH) "$(INTDIR)"
   $(BuildCmds)

!ELSEIF  "$(CFG)" == "Webmaster - Win32 Debug"


BuildCmds= \
	$(CPP) $(CPP_PROJ) $(SOURCE) \
	

".\WinDebug\bench.obj" : $(SOURCE) $(DEP_CPP_BENCH) "$(INTDIR)"
   $(BuildCmds)

".\WinDebug\bench.sbr" : $(SOURCE) $(DEP_CPP_BENCH) "$(INTDIR)"
   $(BuildCmds)

!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=..\errexit.c
DEP_CPP_ERREX=\
	"..\bench.h"\
	"..\sysdep.h"\
	{$(INCLUDE)}"\sys\TYPES.H"\
	
NODEP_CPP_ERREX=\
	"..\config.h"\
	

!IF  "$(CFG)" == "Webmaster - Win32 Release"


BuildCmds= \
	$(CPP) $(CPP_PROJ) $(SOURCE) \
	

".\WinRel\errexit.obj" : $(SOURCE) $(DEP_CPP_ERREX) "$(INTDIR)"
   $(BuildCmds)

".\WinRel\errexit.sbr" : $(SOURCE) $(DEP_CPP_ERREX) "$(INTDIR)"
   $(BuildCmds)

!ELSEIF  "$(CFG)" == "Webmaster - Win32 Debug"


BuildCmds= \
	$(CPP) $(CPP_PROJ) $(SOURCE) \
	

".\WinDebug\errexit.obj" : $(SOURCE) $(DEP_CPP_ERREX) "$(INTDIR)"
   $(BuildCmds)

".\WinDebug\errexit.sbr" : $(SOURCE) $(DEP_CPP_ERREX) "$(INTDIR)"
   $(BuildCmds)

!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=..\webmaster.c
DEP_CPP_WEBMA=\
	"..\bench.h"\
	"..\sysdep.h"\
	{$(INCLUDE)}"\sys\TYPES.H"\
	
NODEP_CPP_WEBMA=\
	"..\config.h"\
	

!IF  "$(CFG)" == "Webmaster - Win32 Release"


BuildCmds= \
	$(CPP) $(CPP_PROJ) $(SOURCE) \
	

".\WinRel\webmaster.obj" : $(SOURCE) $(DEP_CPP_WEBMA) "$(INTDIR)"
   $(BuildCmds)

".\WinRel\webmaster.sbr" : $(SOURCE) $(DEP_CPP_WEBMA) "$(INTDIR)"
   $(BuildCmds)

!ELSEIF  "$(CFG)" == "Webmaster - Win32 Debug"


BuildCmds= \
	$(CPP) $(CPP_PROJ) $(SOURCE) \
	

".\WinDebug\webmaster.obj" : $(SOURCE) $(DEP_CPP_WEBMA) "$(INTDIR)"
   $(BuildCmds)

".\WinDebug\webmaster.sbr" : $(SOURCE) $(DEP_CPP_WEBMA) "$(INTDIR)"
   $(BuildCmds)

!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=..\timefunc.c

!IF  "$(CFG)" == "Webmaster - Win32 Release"

DEP_CPP_TIMEF=\
	"..\bench.h"\
	"..\sysdep.h"\
	

BuildCmds= \
	$(CPP) $(CPP_PROJ) $(SOURCE) \
	

".\WinRel\timefunc.obj" : $(SOURCE) $(DEP_CPP_TIMEF) "$(INTDIR)"
   $(BuildCmds)

".\WinRel\timefunc.sbr" : $(SOURCE) $(DEP_CPP_TIMEF) "$(INTDIR)"
   $(BuildCmds)

!ELSEIF  "$(CFG)" == "Webmaster - Win32 Debug"

DEP_CPP_TIMEF=\
	"..\bench.h"\
	"..\sysdep.h"\
	
NODEP_CPP_TIMEF=\
	"..\config.h"\
	

BuildCmds= \
	$(CPP) $(CPP_PROJ) $(SOURCE) \
	

".\WinDebug\timefunc.obj" : $(SOURCE) $(DEP_CPP_TIMEF) "$(INTDIR)"
   $(BuildCmds)

".\WinDebug\timefunc.sbr" : $(SOURCE) $(DEP_CPP_TIMEF) "$(INTDIR)"
   $(BuildCmds)

!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=..\sysdep.c
DEP_CPP_SYSDE=\
	"..\sysdep.h"\
	
NODEP_CPP_SYSDE=\
	"..\config.h"\
	

!IF  "$(CFG)" == "Webmaster - Win32 Release"


BuildCmds= \
	$(CPP) $(CPP_PROJ) $(SOURCE) \
	

".\WinRel\sysdep.obj" : $(SOURCE) $(DEP_CPP_SYSDE) "$(INTDIR)"
   $(BuildCmds)

".\WinRel\sysdep.sbr" : $(SOURCE) $(DEP_CPP_SYSDE) "$(INTDIR)"
   $(BuildCmds)

!ELSEIF  "$(CFG)" == "Webmaster - Win32 Debug"


BuildCmds= \
	$(CPP) $(CPP_PROJ) $(SOURCE) \
	

".\WinDebug\sysdep.obj" : $(SOURCE) $(DEP_CPP_SYSDE) "$(INTDIR)"
   $(BuildCmds)

".\WinDebug\sysdep.sbr" : $(SOURCE) $(DEP_CPP_SYSDE) "$(INTDIR)"
   $(BuildCmds)

!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=..\statistics.c

!IF  "$(CFG)" == "Webmaster - Win32 Release"

DEP_CPP_STATI=\
	"..\bench.h"\
	"..\sysdep.h"\
	

BuildCmds= \
	$(CPP) $(CPP_PROJ) $(SOURCE) \
	

".\WinRel\statistics.obj" : $(SOURCE) $(DEP_CPP_STATI) "$(INTDIR)"
   $(BuildCmds)

".\WinRel\statistics.sbr" : $(SOURCE) $(DEP_CPP_STATI) "$(INTDIR)"
   $(BuildCmds)

!ELSEIF  "$(CFG)" == "Webmaster - Win32 Debug"

DEP_CPP_STATI=\
	"..\bench.h"\
	"..\sysdep.h"\
	
NODEP_CPP_STATI=\
	"..\config.h"\
	

BuildCmds= \
	$(CPP) $(CPP_PROJ) $(SOURCE) \
	

".\WinDebug\statistics.obj" : $(SOURCE) $(DEP_CPP_STATI) "$(INTDIR)"
   $(BuildCmds)

".\WinDebug\statistics.sbr" : $(SOURCE) $(DEP_CPP_STATI) "$(INTDIR)"
   $(BuildCmds)

!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=..\rexec.c
DEP_CPP_REXEC=\
	"..\bench.h"\
	"..\sysdep.h"\
	
NODEP_CPP_REXEC=\
	"..\config.h"\
	

!IF  "$(CFG)" == "Webmaster - Win32 Release"


BuildCmds= \
	$(CPP) $(CPP_PROJ) $(SOURCE) \
	

".\WinRel\rexec.obj" : $(SOURCE) $(DEP_CPP_REXEC) "$(INTDIR)"
   $(BuildCmds)

".\WinRel\rexec.sbr" : $(SOURCE) $(DEP_CPP_REXEC) "$(INTDIR)"
   $(BuildCmds)

!ELSEIF  "$(CFG)" == "Webmaster - Win32 Debug"


BuildCmds= \
	$(CPP) $(CPP_PROJ) $(SOURCE) \
	

".\WinDebug\rexec.obj" : $(SOURCE) $(DEP_CPP_REXEC) "$(INTDIR)"
   $(BuildCmds)

".\WinDebug\rexec.sbr" : $(SOURCE) $(DEP_CPP_REXEC) "$(INTDIR)"
   $(BuildCmds)

!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=..\getopt.c

!IF  "$(CFG)" == "Webmaster - Win32 Release"


BuildCmds= \
	$(CPP) $(CPP_PROJ) $(SOURCE) \
	

".\WinRel\getopt.obj" : $(SOURCE) "$(INTDIR)"
   $(BuildCmds)

".\WinRel\getopt.sbr" : $(SOURCE) "$(INTDIR)"
   $(BuildCmds)

!ELSEIF  "$(CFG)" == "Webmaster - Win32 Debug"


BuildCmds= \
	$(CPP) $(CPP_PROJ) $(SOURCE) \
	

".\WinDebug\getopt.obj" : $(SOURCE) "$(INTDIR)"
   $(BuildCmds)

".\WinDebug\getopt.sbr" : $(SOURCE) "$(INTDIR)"
   $(BuildCmds)

!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=\stone.jim\webstone.src\src\parse_file_list.c
DEP_CPP_PARSE=\
	"..\bench.h"\
	"..\sysdep.h"\
	{$(INCLUDE)}"\sys\TYPES.H"\
	
NODEP_CPP_PARSE=\
	"..\config.h"\
	

!IF  "$(CFG)" == "Webmaster - Win32 Release"


BuildCmds= \
	$(CPP) $(CPP_PROJ) $(SOURCE) \
	

".\WinRel\parse_file_list.obj" : $(SOURCE) $(DEP_CPP_PARSE) "$(INTDIR)"
   $(BuildCmds)

".\WinRel\parse_file_list.sbr" : $(SOURCE) $(DEP_CPP_PARSE) "$(INTDIR)"
   $(BuildCmds)

!ELSEIF  "$(CFG)" == "Webmaster - Win32 Debug"


BuildCmds= \
	$(CPP) $(CPP_PROJ) $(SOURCE) \
	

".\WinDebug\parse_file_list.obj" : $(SOURCE) $(DEP_CPP_PARSE) "$(INTDIR)"
   $(BuildCmds)

".\WinDebug\parse_file_list.sbr" : $(SOURCE) $(DEP_CPP_PARSE) "$(INTDIR)"
   $(BuildCmds)

!ENDIF 

# End Source File
# End Target
# End Project
################################################################################
