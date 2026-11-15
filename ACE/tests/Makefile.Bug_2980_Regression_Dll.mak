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
!MESSAGE NMAKE /f "Makefile.Bug_2980_Regression_Dll.mak" CFG="Win32 Debug"
!MESSAGE
!MESSAGE Possible choices for configuration are:
!MESSAGE
!MESSAGE "Win32 Debug" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "Win32 Release" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "Win32 Static Debug" (based on "Win32 (x86) Static Library")
!MESSAGE "Win32 Static Release" (based on "Win32 (x86) Static Library")
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
INTDIR=Debug\Bug_2980_Regression_Dll\I386

ALL : "$(INTDIR)" "$(OUTDIR)" DEPENDCHECK $(GENERATED_DIRTY) ".\Bug_2980_Regressiond.dll"

DEPEND :
!IF "$(DEPGEN)" == ""
	@echo No suitable dependency generator could be found.
	@echo One comes with MPC, just set the MPC_ROOT environment variable
	@echo to the full path of MPC.  You can download MPC from
	@echo https://github.com/DOCGroup/MPC
!ELSE
	$(DEPGEN) -I".." -D_DEBUG -DWIN32 -D_WINDOWS -DACE_HAS_IPV6 -D_CRT_SECURE_NO_WARNINGS -D_CRT_SECURE_NO_DEPRECATE -D_CRT_NONSTDC_NO_DEPRECATE -D_WINSOCK_DEPRECATED_NO_WARNINGS -DTEST_OUTPUT_BUILD_DLL -f "Makefile.Bug_2980_Regression_Dll.dep" "Bug_2980_Regression_Dll.cpp" "Test_Output.cpp"
!ENDIF

REALCLEAN : CLEAN
	-@del /f/q "$(OUTDIR)\Bug_2980_Regressiond.pdb"
	-@del /f/q ".\Bug_2980_Regressiond.dll"
	-@del /f/q "$(OUTDIR)\Bug_2980_Regressiond.lib"
	-@del /f/q "$(OUTDIR)\Bug_2980_Regressiond.exp"
	-@del /f/q "$(OUTDIR)\Bug_2980_Regressiond.ilk"

"$(INTDIR)" :
	if not exist "Debug\$(NULL)" mkdir "Debug"
	if not exist "Debug\Bug_2980_Regression_Dll\$(NULL)" mkdir "Debug\Bug_2980_Regression_Dll"
	if not exist "$(INTDIR)\$(NULL)" mkdir "$(INTDIR)"
CPP=cl.exe
CPP_COMMON=/Zc:wchar_t /nologo /Ob0 /W3 /EHsc /Zi /MDd /GR /Gy /std:c++17 /wd4355 /wd4456 /wd4459 /wd4458 /Fd"$(INTDIR)/" /I ".." /D _DEBUG /D WIN32 /D _WINDOWS /D ACE_HAS_IPV6 /D _CRT_SECURE_NO_WARNINGS /D _CRT_SECURE_NO_DEPRECATE /D _CRT_NONSTDC_NO_DEPRECATE /D _WINSOCK_DEPRECATED_NO_WARNINGS /D TEST_OUTPUT_BUILD_DLL /D MPC_LIB_MODIFIER=\"d\" /c

CPP_PROJ=$(CPP_COMMON) /Fo"$(INTDIR)\\"

RSC=rc.exe

LINK32=link.exe
LINK32_FLAGS=advapi32.lib user32.lib /INCREMENTAL:NO ACEd.lib iphlpapi.lib /libpath:"." /libpath:"..\lib" /nologo /subsystem:windows /dll /debug /pdb:".\Bug_2980_Regressiond.pdb" /machine:I386 /out:".\Bug_2980_Regressiond.dll" /implib:"$(OUTDIR)\Bug_2980_Regressiond.lib"
LINK32_OBJS= \
	"$(INTDIR)\Bug_2980_Regression_Dll.obj" \
	"$(INTDIR)\Test_Output.obj"

".\Bug_2980_Regressiond.dll" : $(DEF_FILE) $(LINK32_OBJS)
	$(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<
	if exist ".\Bug_2980_Regressiond.dll.manifest" mt.exe -manifest ".\Bug_2980_Regressiond.dll.manifest" -outputresource:$@;2

!ELSEIF  "$(CFG)" == "Win32 Release"

OUTDIR=.
INTDIR=Release\Bug_2980_Regression_Dll\I386

ALL : "$(INTDIR)" "$(OUTDIR)" DEPENDCHECK $(GENERATED_DIRTY) ".\Bug_2980_Regression.dll"

DEPEND :
!IF "$(DEPGEN)" == ""
	@echo No suitable dependency generator could be found.
	@echo One comes with MPC, just set the MPC_ROOT environment variable
	@echo to the full path of MPC.  You can download MPC from
	@echo https://github.com/DOCGroup/MPC
!ELSE
	$(DEPGEN) -I".." -DNDEBUG -DWIN32 -D_WINDOWS -DACE_HAS_IPV6 -D_CRT_SECURE_NO_WARNINGS -D_CRT_SECURE_NO_DEPRECATE -D_CRT_NONSTDC_NO_DEPRECATE -D_WINSOCK_DEPRECATED_NO_WARNINGS -DTEST_OUTPUT_BUILD_DLL -f "Makefile.Bug_2980_Regression_Dll.dep" "Bug_2980_Regression_Dll.cpp" "Test_Output.cpp"
!ENDIF

REALCLEAN : CLEAN
	-@del /f/q ".\Bug_2980_Regression.dll"
	-@del /f/q "$(OUTDIR)\Bug_2980_Regression.lib"
	-@del /f/q "$(OUTDIR)\Bug_2980_Regression.exp"
	-@del /f/q "$(OUTDIR)\Bug_2980_Regression.ilk"

"$(INTDIR)" :
	if not exist "Release\$(NULL)" mkdir "Release"
	if not exist "Release\Bug_2980_Regression_Dll\$(NULL)" mkdir "Release\Bug_2980_Regression_Dll"
	if not exist "$(INTDIR)\$(NULL)" mkdir "$(INTDIR)"
CPP=cl.exe
CPP_COMMON=/Zc:wchar_t /nologo /O2 /W3 /EHsc /MD /GR /std:c++17 /wd4355 /wd4456 /wd4459 /wd4458 /I ".." /D NDEBUG /D WIN32 /D _WINDOWS /D ACE_HAS_IPV6 /D _CRT_SECURE_NO_WARNINGS /D _CRT_SECURE_NO_DEPRECATE /D _CRT_NONSTDC_NO_DEPRECATE /D _WINSOCK_DEPRECATED_NO_WARNINGS /D TEST_OUTPUT_BUILD_DLL  /c

CPP_PROJ=$(CPP_COMMON) /Fo"$(INTDIR)\\"

RSC=rc.exe

LINK32=link.exe
LINK32_FLAGS=advapi32.lib user32.lib /INCREMENTAL:NO ACE.lib iphlpapi.lib /libpath:"." /libpath:"..\lib" /nologo /subsystem:windows /dll  /machine:I386 /out:".\Bug_2980_Regression.dll" /implib:"$(OUTDIR)\Bug_2980_Regression.lib"
LINK32_OBJS= \
	"$(INTDIR)\Bug_2980_Regression_Dll.obj" \
	"$(INTDIR)\Test_Output.obj"

".\Bug_2980_Regression.dll" : $(DEF_FILE) $(LINK32_OBJS)
	$(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<
	if exist ".\Bug_2980_Regression.dll.manifest" mt.exe -manifest ".\Bug_2980_Regression.dll.manifest" -outputresource:$@;2

!ELSEIF  "$(CFG)" == "Win32 Static Debug"

OUTDIR=.
INTDIR=Static_Debug\Bug_2980_Regression_Dll\I386

ALL : "$(INTDIR)" "$(OUTDIR)" DEPENDCHECK $(GENERATED_DIRTY) "$(OUTDIR)\Bug_2980_Regressionsd.lib"

DEPEND :
!IF "$(DEPGEN)" == ""
	@echo No suitable dependency generator could be found.
	@echo One comes with MPC, just set the MPC_ROOT environment variable
	@echo to the full path of MPC.  You can download MPC from
	@echo https://github.com/DOCGroup/MPC
!ELSE
	$(DEPGEN) -I".." -D_DEBUG -DWIN32 -D_WINDOWS -DACE_HAS_IPV6 -D_CRT_SECURE_NO_WARNINGS -D_CRT_SECURE_NO_DEPRECATE -D_CRT_NONSTDC_NO_DEPRECATE -D_WINSOCK_DEPRECATED_NO_WARNINGS -DACE_AS_STATIC_LIBS -f "Makefile.Bug_2980_Regression_Dll.dep" "Bug_2980_Regression_Dll.cpp" "Test_Output.cpp"
!ENDIF

REALCLEAN : CLEAN
	-@del /f/q "$(OUTDIR)\Bug_2980_Regressionsd.lib"
	-@del /f/q "$(OUTDIR)\Bug_2980_Regressionsd.exp"
	-@del /f/q "$(OUTDIR)\Bug_2980_Regressionsd.ilk"
	-@del /f/q ".\Bug_2980_Regressionsd.pdb"

"$(INTDIR)" :
	if not exist "Static_Debug\$(NULL)" mkdir "Static_Debug"
	if not exist "Static_Debug\Bug_2980_Regression_Dll\$(NULL)" mkdir "Static_Debug\Bug_2980_Regression_Dll"
	if not exist "$(INTDIR)\$(NULL)" mkdir "$(INTDIR)"
CPP=cl.exe
CPP_COMMON=/Zc:wchar_t /nologo /Ob0 /W3 /EHsc /Zi /GR /Gy /MDd /std:c++17 /wd4355 /wd4456 /wd4459 /wd4458 /Fd".\Bug_2980_Regressionsd.pdb" /I ".." /D _DEBUG /D WIN32 /D _WINDOWS /D ACE_HAS_IPV6 /D _CRT_SECURE_NO_WARNINGS /D _CRT_SECURE_NO_DEPRECATE /D _CRT_NONSTDC_NO_DEPRECATE /D _WINSOCK_DEPRECATED_NO_WARNINGS /D ACE_AS_STATIC_LIBS /D MPC_LIB_MODIFIER=\"sd\" /c

CPP_PROJ=$(CPP_COMMON) /Fo"$(INTDIR)\\"


LINK32=link.exe -lib
LINK32_FLAGS=/nologo /machine:I386 /out:".\Bug_2980_Regressionsd.lib"
LINK32_OBJS= \
	"$(INTDIR)\Bug_2980_Regression_Dll.obj" \
	"$(INTDIR)\Test_Output.obj"

"$(OUTDIR)\Bug_2980_Regressionsd.lib" : $(DEF_FILE) $(LINK32_OBJS)
	$(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<
	if exist "$(OUTDIR)\Bug_2980_Regressionsd.lib.manifest" mt.exe -manifest "$(OUTDIR)\Bug_2980_Regressionsd.lib.manifest" -outputresource:$@;2

!ELSEIF  "$(CFG)" == "Win32 Static Release"

OUTDIR=.
INTDIR=Static_Release\Bug_2980_Regression_Dll\I386

ALL : "$(INTDIR)" "$(OUTDIR)" DEPENDCHECK $(GENERATED_DIRTY) "$(OUTDIR)\Bug_2980_Regressions.lib"

DEPEND :
!IF "$(DEPGEN)" == ""
	@echo No suitable dependency generator could be found.
	@echo One comes with MPC, just set the MPC_ROOT environment variable
	@echo to the full path of MPC.  You can download MPC from
	@echo https://github.com/DOCGroup/MPC
!ELSE
	$(DEPGEN) -I".." -DNDEBUG -DWIN32 -D_WINDOWS -DACE_HAS_IPV6 -D_CRT_SECURE_NO_WARNINGS -D_CRT_SECURE_NO_DEPRECATE -D_CRT_NONSTDC_NO_DEPRECATE -D_WINSOCK_DEPRECATED_NO_WARNINGS -DACE_AS_STATIC_LIBS -f "Makefile.Bug_2980_Regression_Dll.dep" "Bug_2980_Regression_Dll.cpp" "Test_Output.cpp"
!ENDIF

REALCLEAN : CLEAN
	-@del /f/q "$(OUTDIR)\Bug_2980_Regressions.lib"
	-@del /f/q "$(OUTDIR)\Bug_2980_Regressions.exp"
	-@del /f/q "$(OUTDIR)\Bug_2980_Regressions.ilk"

"$(INTDIR)" :
	if not exist "Static_Release\$(NULL)" mkdir "Static_Release"
	if not exist "Static_Release\Bug_2980_Regression_Dll\$(NULL)" mkdir "Static_Release\Bug_2980_Regression_Dll"
	if not exist "$(INTDIR)\$(NULL)" mkdir "$(INTDIR)"
CPP=cl.exe
CPP_COMMON=/Zc:wchar_t /nologo /O2 /W3 /EHsc /MD /GR /std:c++17 /wd4355 /wd4456 /wd4459 /wd4458 /I ".." /D NDEBUG /D WIN32 /D _WINDOWS /D ACE_HAS_IPV6 /D _CRT_SECURE_NO_WARNINGS /D _CRT_SECURE_NO_DEPRECATE /D _CRT_NONSTDC_NO_DEPRECATE /D _WINSOCK_DEPRECATED_NO_WARNINGS /D ACE_AS_STATIC_LIBS /D MPC_LIB_MODIFIER=\"s\" /c

CPP_PROJ=$(CPP_COMMON) /Fo"$(INTDIR)\\"


LINK32=link.exe -lib
LINK32_FLAGS=/nologo /machine:I386 /out:".\Bug_2980_Regressions.lib"
LINK32_OBJS= \
	"$(INTDIR)\Bug_2980_Regression_Dll.obj" \
	"$(INTDIR)\Test_Output.obj"

"$(OUTDIR)\Bug_2980_Regressions.lib" : $(DEF_FILE) $(LINK32_OBJS)
	$(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<
	if exist "$(OUTDIR)\Bug_2980_Regressions.lib.manifest" mt.exe -manifest "$(OUTDIR)\Bug_2980_Regressions.lib.manifest" -outputresource:$@;2

!ENDIF

CLEAN :
	-@del /f/s/q "$(INTDIR)"

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
!IF EXISTS("Makefile.Bug_2980_Regression_Dll.dep")
!INCLUDE "Makefile.Bug_2980_Regression_Dll.dep"
!ENDIF
!ENDIF

!IF "$(CFG)" == "Win32 Debug" || "$(CFG)" == "Win32 Release" || "$(CFG)" == "Win32 Static Debug" || "$(CFG)" == "Win32 Static Release" 
SOURCE="Bug_2980_Regression_Dll.cpp"

"$(INTDIR)\Bug_2980_Regression_Dll.obj" : $(SOURCE)
	$(CPP) $(CPP_COMMON) /Fo"$(INTDIR)\Bug_2980_Regression_Dll.obj" $(SOURCE)

SOURCE="Test_Output.cpp"

"$(INTDIR)\Test_Output.obj" : $(SOURCE)
	$(CPP) $(CPP_COMMON) /Fo"$(INTDIR)\Test_Output.obj" $(SOURCE)

!IF  "$(CFG)" == "Win32 Debug"
!ELSEIF  "$(CFG)" == "Win32 Release"
!ELSEIF  "$(CFG)" == "Win32 Static Debug"
!ELSEIF  "$(CFG)" == "Win32 Static Release"
!ENDIF


!ENDIF

GENERATED : "$(INTDIR)" "$(OUTDIR)" $(GENERATED_DIRTY)
	-@rem

DEPENDCHECK :
!IF "$(NO_EXTERNAL_DEPS)" != "1"
!IF EXISTS("Makefile.Bug_2980_Regression_Dll.dep")
	@echo Using "Makefile.Bug_2980_Regression_Dll.dep"
!ELSE
	@echo Warning: cannot find "Makefile.Bug_2980_Regression_Dll.dep"
!ENDIF
!ENDIF

