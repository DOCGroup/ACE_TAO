# Microsoft Developer Studio Generated NMAKE File
!IF "$(CFG)" == ""
CFG=Win32 Debug
!MESSAGE No configuration specified. Defaulting to Win32 Debug.
!ENDIF

!IF "$(CFG)" == "Win32 Debug" || "$(CFG)" == "Win32 Release" || "$(CFG)" == "Win32 Static Debug" || "$(CFG)" == "Win32 Static Release"
!ELSE
!MESSAGE Invalid configuration "$(CFG)" specified.
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE
!MESSAGE NMAKE /f "Makefile.Bug_2980_Regression_Test.mak" CFG="Win32 Debug"
!MESSAGE
!MESSAGE Possible choices for configuration are:
!MESSAGE
!MESSAGE "Win32 Debug" (based on "Win32 (x86) Console Application")
!MESSAGE "Win32 Release" (based on "Win32 (x86) Console Application")
!MESSAGE "Win32 Static Debug" (based on "Win32 (x86) Console Application")
!MESSAGE "Win32 Static Release" (based on "Win32 (x86) Console Application")
!MESSAGE
!ERROR An invalid configuration was specified.
!ENDIF

!IF "$(OS)" == "Windows_NT"
NULL=
!ELSE
NULL=nul
!ENDIF

!IF "$(DEPGEN)" == ""
!IF EXISTS("$(MPC_ROOT)/depgen.pl")
DEPGEN=perl $(MPC_ROOT)/depgen.pl -i -t nmake
!ELSEIF EXISTS("$(DEPGEN_ROOT)/depgen.pl")
DEPGEN=perl $(DEPGEN_ROOT)/depgen.pl -i -t nmake
!ELSEIF EXISTS("$(ACE_ROOT)/bin/depgen.pl")
DEPGEN=perl $(ACE_ROOT)/bin/depgen.pl -i -t nmake
!ENDIF
!ENDIF

GENERATED_DIRTY =

!IF  "$(CFG)" == "Win32 Debug"

OUTDIR=.
INSTALLDIR=.
INTDIR=Debug\Bug_2980_Regression_Test\I386

ALL : "$(INTDIR)" "$(OUTDIR)" DEPENDCHECK $(GENERATED_DIRTY) "$(INSTALLDIR)\Bug_2980_Regression_Test.exe"

DEPEND :
!IF "$(DEPGEN)" == ""
	@echo No suitable dependency generator could be found.
	@echo One comes with MPC, just set the MPC_ROOT environment variable
	@echo to the full path of MPC.  You can download MPC from
	@echo https://github.com/DOCGroup/MPC
!ELSE
	$(DEPGEN) -I".." -D_DEBUG -DWIN32 -D_CONSOLE -f "Makefile.Bug_2980_Regression_Test.dep" "Bug_2980_Regression_Test.cpp"
!ENDIF

REALCLEAN : CLEAN
	-@del /f/q "$(INSTALLDIR)\Bug_2980_Regression_Test.pdb"
	-@del /f/q "$(INSTALLDIR)\Bug_2980_Regression_Test.exe"
	-@del /f/q "$(INSTALLDIR)\Bug_2980_Regression_Test.ilk"

"$(INTDIR)" :
	if not exist "Debug\$(NULL)" mkdir "Debug"
	if not exist "Debug\Bug_2980_Regression_Test\$(NULL)" mkdir "Debug\Bug_2980_Regression_Test"
	if not exist "$(INTDIR)\$(NULL)" mkdir "$(INTDIR)"
CPP=cl.exe
CPP_COMMON=/Zc:wchar_t /nologo /Ob0 /W3 /EHsc /Zi /MDd /GR /Gy /std:c++17 /wd4456 /wd4459 /wd4458 /Fd"$(INTDIR)/" /I ".." /D _DEBUG /D WIN32 /D _CONSOLE  /c

CPP_PROJ=$(CPP_COMMON) /Fo"$(INTDIR)\\"

RSC=rc.exe

LINK32=link.exe
LINK32_FLAGS=advapi32.lib user32.lib /INCREMENTAL:NO /libpath:"." /nologo /subsystem:console /debug /pdb:"$(INSTALLDIR)\Bug_2980_Regression_Test.pdb" /machine:I386 /out:"$(INSTALLDIR)\Bug_2980_Regression_Test.exe"
LINK32_OBJS= \
	"$(INTDIR)\Bug_2980_Regression_Test.obj"

"$(INSTALLDIR)\Bug_2980_Regression_Test.exe" : $(DEF_FILE) $(LINK32_OBJS)
	$(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<
	if exist "$(INSTALLDIR)\Bug_2980_Regression_Test.exe.manifest" mt.exe -manifest "$(INSTALLDIR)\Bug_2980_Regression_Test.exe.manifest" -outputresource:$@;1

!ELSEIF  "$(CFG)" == "Win32 Release"

OUTDIR=Release
INSTALLDIR=Release
INTDIR=Release\Bug_2980_Regression_Test\I386

ALL : "$(INTDIR)" "$(OUTDIR)" DEPENDCHECK $(GENERATED_DIRTY) "$(INSTALLDIR)\Bug_2980_Regression_Test.exe"

DEPEND :
!IF "$(DEPGEN)" == ""
	@echo No suitable dependency generator could be found.
	@echo One comes with MPC, just set the MPC_ROOT environment variable
	@echo to the full path of MPC.  You can download MPC from
	@echo https://github.com/DOCGroup/MPC
!ELSE
	$(DEPGEN) -I".." -DNDEBUG -DWIN32 -D_CONSOLE -f "Makefile.Bug_2980_Regression_Test.dep" "Bug_2980_Regression_Test.cpp"
!ENDIF

REALCLEAN : CLEAN
	-@del /f/q "$(INSTALLDIR)\Bug_2980_Regression_Test.exe"
	-@del /f/q "$(INSTALLDIR)\Bug_2980_Regression_Test.ilk"

"$(INTDIR)" :
	if not exist "Release\$(NULL)" mkdir "Release"
	if not exist "Release\Bug_2980_Regression_Test\$(NULL)" mkdir "Release\Bug_2980_Regression_Test"
	if not exist "$(INTDIR)\$(NULL)" mkdir "$(INTDIR)"
CPP=cl.exe
CPP_COMMON=/Zc:wchar_t /nologo /O2 /W3 /EHsc /MD /GR /std:c++17 /wd4456 /wd4459 /wd4458 /I ".." /D NDEBUG /D WIN32 /D _CONSOLE  /c

CPP_PROJ=$(CPP_COMMON) /Fo"$(INTDIR)\\"

RSC=rc.exe

LINK32=link.exe
LINK32_FLAGS=advapi32.lib user32.lib /INCREMENTAL:NO /libpath:"." /nologo /subsystem:console  /machine:I386 /out:"$(INSTALLDIR)\Bug_2980_Regression_Test.exe"
LINK32_OBJS= \
	"$(INTDIR)\Bug_2980_Regression_Test.obj"

"$(INSTALLDIR)\Bug_2980_Regression_Test.exe" : $(DEF_FILE) $(LINK32_OBJS)
	$(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<
	if exist "$(INSTALLDIR)\Bug_2980_Regression_Test.exe.manifest" mt.exe -manifest "$(INSTALLDIR)\Bug_2980_Regression_Test.exe.manifest" -outputresource:$@;1

!ELSEIF  "$(CFG)" == "Win32 Static Debug"

OUTDIR=Static_Debug
INSTALLDIR=Static_Debug
INTDIR=Static_Debug\Bug_2980_Regression_Test\I386

ALL : "$(INTDIR)" "$(OUTDIR)" DEPENDCHECK $(GENERATED_DIRTY) "$(INSTALLDIR)\Bug_2980_Regression_Test.exe"

DEPEND :
!IF "$(DEPGEN)" == ""
	@echo No suitable dependency generator could be found.
	@echo One comes with MPC, just set the MPC_ROOT environment variable
	@echo to the full path of MPC.  You can download MPC from
	@echo https://github.com/DOCGroup/MPC
!ELSE
	$(DEPGEN) -I".." -D_DEBUG -DWIN32 -D_CONSOLE -DACE_AS_STATIC_LIBS -f "Makefile.Bug_2980_Regression_Test.dep" "Bug_2980_Regression_Test.cpp"
!ENDIF

REALCLEAN : CLEAN
	-@del /f/q "$(INSTALLDIR)\Bug_2980_Regression_Test.pdb"
	-@del /f/q "$(INSTALLDIR)\Bug_2980_Regression_Test.exe"
	-@del /f/q "$(INSTALLDIR)\Bug_2980_Regression_Test.ilk"

"$(INTDIR)" :
	if not exist "Static_Debug\$(NULL)" mkdir "Static_Debug"
	if not exist "Static_Debug\Bug_2980_Regression_Test\$(NULL)" mkdir "Static_Debug\Bug_2980_Regression_Test"
	if not exist "$(INTDIR)\$(NULL)" mkdir "$(INTDIR)"
CPP=cl.exe
CPP_COMMON=/Zc:wchar_t /nologo /Ob0 /W3 /EHsc /Zi /MDd /GR /Gy /std:c++17 /wd4456 /wd4459 /wd4458 /Fd"$(INTDIR)/" /I ".." /D _DEBUG /D WIN32 /D _CONSOLE /D ACE_AS_STATIC_LIBS  /c

CPP_PROJ=$(CPP_COMMON) /Fo"$(INTDIR)\\"

RSC=rc.exe

LINK32=link.exe
LINK32_FLAGS=advapi32.lib user32.lib /INCREMENTAL:NO /libpath:"." /nologo /subsystem:console /debug /pdb:"$(INSTALLDIR)\Bug_2980_Regression_Test.pdb" /machine:I386 /out:"$(INSTALLDIR)\Bug_2980_Regression_Test.exe"
LINK32_OBJS= \
	"$(INTDIR)\Bug_2980_Regression_Test.obj"

"$(INSTALLDIR)\Bug_2980_Regression_Test.exe" : $(DEF_FILE) $(LINK32_OBJS)
	$(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<
	if exist "$(INSTALLDIR)\Bug_2980_Regression_Test.exe.manifest" mt.exe -manifest "$(INSTALLDIR)\Bug_2980_Regression_Test.exe.manifest" -outputresource:$@;1

!ELSEIF  "$(CFG)" == "Win32 Static Release"

OUTDIR=Static_Release
INSTALLDIR=Static_Release
INTDIR=Static_Release\Bug_2980_Regression_Test\I386

ALL : "$(INTDIR)" "$(OUTDIR)" DEPENDCHECK $(GENERATED_DIRTY) "$(INSTALLDIR)\Bug_2980_Regression_Test.exe"

DEPEND :
!IF "$(DEPGEN)" == ""
	@echo No suitable dependency generator could be found.
	@echo One comes with MPC, just set the MPC_ROOT environment variable
	@echo to the full path of MPC.  You can download MPC from
	@echo https://github.com/DOCGroup/MPC
!ELSE
	$(DEPGEN) -I".." -DNDEBUG -DWIN32 -D_CONSOLE -DACE_AS_STATIC_LIBS -f "Makefile.Bug_2980_Regression_Test.dep" "Bug_2980_Regression_Test.cpp"
!ENDIF

REALCLEAN : CLEAN
	-@del /f/q "$(INSTALLDIR)\Bug_2980_Regression_Test.exe"
	-@del /f/q "$(INSTALLDIR)\Bug_2980_Regression_Test.ilk"

"$(INTDIR)" :
	if not exist "Static_Release\$(NULL)" mkdir "Static_Release"
	if not exist "Static_Release\Bug_2980_Regression_Test\$(NULL)" mkdir "Static_Release\Bug_2980_Regression_Test"
	if not exist "$(INTDIR)\$(NULL)" mkdir "$(INTDIR)"
CPP=cl.exe
CPP_COMMON=/Zc:wchar_t /nologo /O2 /W3 /EHsc /MD /GR /std:c++17 /wd4456 /wd4459 /wd4458 /I ".." /D NDEBUG /D WIN32 /D _CONSOLE /D ACE_AS_STATIC_LIBS  /c

CPP_PROJ=$(CPP_COMMON) /Fo"$(INTDIR)\\"

RSC=rc.exe

LINK32=link.exe
LINK32_FLAGS=advapi32.lib user32.lib /INCREMENTAL:NO /libpath:"." /nologo /subsystem:console  /machine:I386 /out:"$(INSTALLDIR)\Bug_2980_Regression_Test.exe"
LINK32_OBJS= \
	"$(INTDIR)\Bug_2980_Regression_Test.obj"

"$(INSTALLDIR)\Bug_2980_Regression_Test.exe" : $(DEF_FILE) $(LINK32_OBJS)
	$(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<
	if exist "$(INSTALLDIR)\Bug_2980_Regression_Test.exe.manifest" mt.exe -manifest "$(INSTALLDIR)\Bug_2980_Regression_Test.exe.manifest" -outputresource:$@;1

!ENDIF

CLEAN :
	-@del /f/s/q "$(INTDIR)"


"$(INSTALLDIR)" ::
	if not exist "$(INSTALLDIR)\$(NULL)" mkdir "$(INSTALLDIR)"

"$(OUTDIR)" ::
	if not exist "$(OUTDIR)\$(NULL)" mkdir "$(OUTDIR)"

.c{$(INTDIR)}.obj::
	$(CPP) @<<
   $(CPP_PROJ) $<
<<

.cpp{$(INTDIR)}.obj::
	$(CPP) @<<
   $(CPP_PROJ) $<
<<

.cxx{$(INTDIR)}.obj::
	$(CPP) @<<
   $(CPP_PROJ) $<
<<

.c{$(INTDIR)}.sbr::
	$(CPP) @<<
   $(CPP_PROJ) $<
<<

.cpp{$(INTDIR)}.sbr::
	$(CPP) @<<
   $(CPP_PROJ) $<
<<

.cxx{$(INTDIR)}.sbr::
	$(CPP) @<<
   $(CPP_PROJ) $<
<<

!IF "$(NO_EXTERNAL_DEPS)" != "1"
!IF EXISTS("Makefile.Bug_2980_Regression_Test.dep")
!INCLUDE "Makefile.Bug_2980_Regression_Test.dep"
!ENDIF
!ENDIF

!IF "$(CFG)" == "Win32 Debug" || "$(CFG)" == "Win32 Release" || "$(CFG)" == "Win32 Static Debug" || "$(CFG)" == "Win32 Static Release" 
SOURCE="Bug_2980_Regression_Test.cpp"

"$(INTDIR)\Bug_2980_Regression_Test.obj" : $(SOURCE)
	$(CPP) $(CPP_COMMON) /Fo"$(INTDIR)\Bug_2980_Regression_Test.obj" $(SOURCE)


!ENDIF

GENERATED : "$(INTDIR)" "$(OUTDIR)" $(GENERATED_DIRTY)
	-@rem

DEPENDCHECK :
!IF "$(NO_EXTERNAL_DEPS)" != "1"
!IF EXISTS("Makefile.Bug_2980_Regression_Test.dep")
	@echo Using "Makefile.Bug_2980_Regression_Test.dep"
!ELSE
	@echo Warning: cannot find "Makefile.Bug_2980_Regression_Test.dep"
!ENDIF
!ENDIF

