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
!MESSAGE NMAKE /f "Makefile.ACE_ETCL_Parser.mak" CFG="Win32 Debug"
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

OUTDIR=..\..\lib
INTDIR=Debug\ACE_ETCL_Parser\I386

ALL : "$(INTDIR)" "$(OUTDIR)" DEPENDCHECK $(GENERATED_DIRTY) "..\..\lib\ACE_ETCL_Parserd.dll"

DEPEND :
!IF "$(DEPGEN)" == ""
	@echo No suitable dependency generator could be found.
	@echo One comes with MPC, just set the MPC_ROOT environment variable
	@echo to the full path of MPC.  You can download MPC from
	@echo http://www.ociweb.com/products/mpc/down.html
!ELSE
	$(DEPGEN) -I"..\.." -D_DEBUG -DWIN32 -D_WINDOWS -D_CRT_SECURE_NO_WARNINGS -D_CRT_SECURE_NO_DEPRECATE -D_CRT_NONSTDC_NO_DEPRECATE -D_WINSOCK_DEPRECATED_NO_WARNINGS -DETCL_PARSER_BUILD_DLL -f "Makefile.ACE_ETCL_Parser.dep" "ETCL_Interpreter.cpp" "ETCL_l.cpp" "ETCL_y.cpp"
!ENDIF

REALCLEAN : CLEAN
	-@del /f/q "$(OUTDIR)\ACE_ETCL_Parserd.pdb"
	-@del /f/q "..\..\lib\ACE_ETCL_Parserd.dll"
	-@del /f/q "$(OUTDIR)\ACE_ETCL_Parserd.lib"
	-@del /f/q "$(OUTDIR)\ACE_ETCL_Parserd.exp"
	-@del /f/q "$(OUTDIR)\ACE_ETCL_Parserd.ilk"

"$(INTDIR)" :
	if not exist "Debug\$(NULL)" mkdir "Debug"
	if not exist "Debug\ACE_ETCL_Parser\$(NULL)" mkdir "Debug\ACE_ETCL_Parser"
	if not exist "$(INTDIR)\$(NULL)" mkdir "$(INTDIR)"
CPP=cl.exe
CPP_COMMON=/Zc:wchar_t /nologo /Ob0 /W3 /EHsc /Zi /MDd /GR /Gy /wd4355 /wd4456 /wd4459 /wd4458 /Fd"$(INTDIR)/" /I "..\.." /D _DEBUG /D WIN32 /D _WINDOWS /D _CRT_SECURE_NO_WARNINGS /D _CRT_SECURE_NO_DEPRECATE /D _CRT_NONSTDC_NO_DEPRECATE /D _WINSOCK_DEPRECATED_NO_WARNINGS /D ETCL_PARSER_BUILD_DLL /D MPC_LIB_MODIFIER=\"d\" /c

CPP_PROJ=$(CPP_COMMON) /Fo"$(INTDIR)\\"

RSC=rc.exe

LINK32=link.exe
LINK32_FLAGS=advapi32.lib user32.lib /INCREMENTAL:NO ACEd.lib ACE_ETCLd.lib iphlpapi.lib /libpath:"." /libpath:"..\..\lib" /nologo /subsystem:windows /dll /debug /pdb:"..\..\lib\ACE_ETCL_Parserd.pdb" /machine:I386 /out:"..\..\lib\ACE_ETCL_Parserd.dll" /implib:"$(OUTDIR)\ACE_ETCL_Parserd.lib"
LINK32_OBJS= \
	"$(INTDIR)\ETCL_Interpreter.obj" \
	"$(INTDIR)\ETCL_l.obj" \
	"$(INTDIR)\ETCL_y.obj"

"..\..\lib\ACE_ETCL_Parserd.dll" : $(DEF_FILE) $(LINK32_OBJS)
	$(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<
	if exist "..\..\lib\ACE_ETCL_Parserd.dll.manifest" mt.exe -manifest "..\..\lib\ACE_ETCL_Parserd.dll.manifest" -outputresource:$@;2

!ELSEIF  "$(CFG)" == "Win32 Release"

OUTDIR=..\..\lib
INTDIR=Release\ACE_ETCL_Parser\I386

ALL : "$(INTDIR)" "$(OUTDIR)" DEPENDCHECK $(GENERATED_DIRTY) "..\..\lib\ACE_ETCL_Parser.dll"

DEPEND :
!IF "$(DEPGEN)" == ""
	@echo No suitable dependency generator could be found.
	@echo One comes with MPC, just set the MPC_ROOT environment variable
	@echo to the full path of MPC.  You can download MPC from
	@echo http://www.ociweb.com/products/mpc/down.html
!ELSE
	$(DEPGEN) -I"..\.." -DNDEBUG -DWIN32 -D_WINDOWS -D_CRT_SECURE_NO_WARNINGS -D_CRT_SECURE_NO_DEPRECATE -D_CRT_NONSTDC_NO_DEPRECATE -D_WINSOCK_DEPRECATED_NO_WARNINGS -DETCL_PARSER_BUILD_DLL -f "Makefile.ACE_ETCL_Parser.dep" "ETCL_Interpreter.cpp" "ETCL_l.cpp" "ETCL_y.cpp"
!ENDIF

REALCLEAN : CLEAN
	-@del /f/q "..\..\lib\ACE_ETCL_Parser.dll"
	-@del /f/q "$(OUTDIR)\ACE_ETCL_Parser.lib"
	-@del /f/q "$(OUTDIR)\ACE_ETCL_Parser.exp"
	-@del /f/q "$(OUTDIR)\ACE_ETCL_Parser.ilk"

"$(INTDIR)" :
	if not exist "Release\$(NULL)" mkdir "Release"
	if not exist "Release\ACE_ETCL_Parser\$(NULL)" mkdir "Release\ACE_ETCL_Parser"
	if not exist "$(INTDIR)\$(NULL)" mkdir "$(INTDIR)"
CPP=cl.exe
CPP_COMMON=/Zc:wchar_t /nologo /O2 /W3 /EHsc /MD /GR /wd4355 /wd4456 /wd4459 /wd4458 /I "..\.." /D NDEBUG /D WIN32 /D _WINDOWS /D _CRT_SECURE_NO_WARNINGS /D _CRT_SECURE_NO_DEPRECATE /D _CRT_NONSTDC_NO_DEPRECATE /D _WINSOCK_DEPRECATED_NO_WARNINGS /D ETCL_PARSER_BUILD_DLL  /c

CPP_PROJ=$(CPP_COMMON) /Fo"$(INTDIR)\\"

RSC=rc.exe

LINK32=link.exe
LINK32_FLAGS=advapi32.lib user32.lib /INCREMENTAL:NO ACE.lib ACE_ETCL.lib iphlpapi.lib /libpath:"." /libpath:"..\..\lib" /nologo /subsystem:windows /dll  /machine:I386 /out:"..\..\lib\ACE_ETCL_Parser.dll" /implib:"$(OUTDIR)\ACE_ETCL_Parser.lib"
LINK32_OBJS= \
	"$(INTDIR)\ETCL_Interpreter.obj" \
	"$(INTDIR)\ETCL_l.obj" \
	"$(INTDIR)\ETCL_y.obj"

"..\..\lib\ACE_ETCL_Parser.dll" : $(DEF_FILE) $(LINK32_OBJS)
	$(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<
	if exist "..\..\lib\ACE_ETCL_Parser.dll.manifest" mt.exe -manifest "..\..\lib\ACE_ETCL_Parser.dll.manifest" -outputresource:$@;2

!ELSEIF  "$(CFG)" == "Win32 Static Debug"

OUTDIR=..\..\lib
INTDIR=Static_Debug\ACE_ETCL_Parser\I386

ALL : "$(INTDIR)" "$(OUTDIR)" DEPENDCHECK $(GENERATED_DIRTY) "$(OUTDIR)\ACE_ETCL_Parsersd.lib"

DEPEND :
!IF "$(DEPGEN)" == ""
	@echo No suitable dependency generator could be found.
	@echo One comes with MPC, just set the MPC_ROOT environment variable
	@echo to the full path of MPC.  You can download MPC from
	@echo http://www.ociweb.com/products/mpc/down.html
!ELSE
	$(DEPGEN) -I"..\.." -D_DEBUG -DWIN32 -D_WINDOWS -D_CRT_SECURE_NO_WARNINGS -D_CRT_SECURE_NO_DEPRECATE -D_CRT_NONSTDC_NO_DEPRECATE -D_WINSOCK_DEPRECATED_NO_WARNINGS -DACE_AS_STATIC_LIBS -f "Makefile.ACE_ETCL_Parser.dep" "ETCL_Interpreter.cpp" "ETCL_l.cpp" "ETCL_y.cpp"
!ENDIF

REALCLEAN : CLEAN
	-@del /f/q "$(OUTDIR)\ACE_ETCL_Parsersd.lib"
	-@del /f/q "$(OUTDIR)\ACE_ETCL_Parsersd.exp"
	-@del /f/q "$(OUTDIR)\ACE_ETCL_Parsersd.ilk"
	-@del /f/q "..\..\lib\ACE_ETCL_Parsersd.pdb"

"$(INTDIR)" :
	if not exist "Static_Debug\$(NULL)" mkdir "Static_Debug"
	if not exist "Static_Debug\ACE_ETCL_Parser\$(NULL)" mkdir "Static_Debug\ACE_ETCL_Parser"
	if not exist "$(INTDIR)\$(NULL)" mkdir "$(INTDIR)"
CPP=cl.exe
CPP_COMMON=/Zc:wchar_t /nologo /Ob0 /W3 /EHsc /Zi /GR /Gy /MDd /wd4355 /wd4456 /wd4459 /wd4458 /Fd"..\..\lib\ACE_ETCL_Parsersd.pdb" /I "..\.." /D _DEBUG /D WIN32 /D _WINDOWS /D _CRT_SECURE_NO_WARNINGS /D _CRT_SECURE_NO_DEPRECATE /D _CRT_NONSTDC_NO_DEPRECATE /D _WINSOCK_DEPRECATED_NO_WARNINGS /D ACE_AS_STATIC_LIBS /D MPC_LIB_MODIFIER=\"sd\" /c

CPP_PROJ=$(CPP_COMMON) /Fo"$(INTDIR)\\"


LINK32=link.exe -lib
LINK32_FLAGS=/nologo /machine:I386 /out:"..\..\lib\ACE_ETCL_Parsersd.lib"
LINK32_OBJS= \
	"$(INTDIR)\ETCL_Interpreter.obj" \
	"$(INTDIR)\ETCL_l.obj" \
	"$(INTDIR)\ETCL_y.obj"

"$(OUTDIR)\ACE_ETCL_Parsersd.lib" : $(DEF_FILE) $(LINK32_OBJS)
	$(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<
	if exist "$(OUTDIR)\ACE_ETCL_Parsersd.lib.manifest" mt.exe -manifest "$(OUTDIR)\ACE_ETCL_Parsersd.lib.manifest" -outputresource:$@;2

!ELSEIF  "$(CFG)" == "Win32 Static Release"

OUTDIR=..\..\lib
INTDIR=Static_Release\ACE_ETCL_Parser\I386

ALL : "$(INTDIR)" "$(OUTDIR)" DEPENDCHECK $(GENERATED_DIRTY) "$(OUTDIR)\ACE_ETCL_Parsers.lib"

DEPEND :
!IF "$(DEPGEN)" == ""
	@echo No suitable dependency generator could be found.
	@echo One comes with MPC, just set the MPC_ROOT environment variable
	@echo to the full path of MPC.  You can download MPC from
	@echo http://www.ociweb.com/products/mpc/down.html
!ELSE
	$(DEPGEN) -I"..\.." -DNDEBUG -DWIN32 -D_WINDOWS -D_CRT_SECURE_NO_WARNINGS -D_CRT_SECURE_NO_DEPRECATE -D_CRT_NONSTDC_NO_DEPRECATE -D_WINSOCK_DEPRECATED_NO_WARNINGS -DACE_AS_STATIC_LIBS -f "Makefile.ACE_ETCL_Parser.dep" "ETCL_Interpreter.cpp" "ETCL_l.cpp" "ETCL_y.cpp"
!ENDIF

REALCLEAN : CLEAN
	-@del /f/q "$(OUTDIR)\ACE_ETCL_Parsers.lib"
	-@del /f/q "$(OUTDIR)\ACE_ETCL_Parsers.exp"
	-@del /f/q "$(OUTDIR)\ACE_ETCL_Parsers.ilk"

"$(INTDIR)" :
	if not exist "Static_Release\$(NULL)" mkdir "Static_Release"
	if not exist "Static_Release\ACE_ETCL_Parser\$(NULL)" mkdir "Static_Release\ACE_ETCL_Parser"
	if not exist "$(INTDIR)\$(NULL)" mkdir "$(INTDIR)"
CPP=cl.exe
CPP_COMMON=/Zc:wchar_t /nologo /O2 /W3 /EHsc /MD /GR /wd4355 /wd4456 /wd4459 /wd4458 /I "..\.." /D NDEBUG /D WIN32 /D _WINDOWS /D _CRT_SECURE_NO_WARNINGS /D _CRT_SECURE_NO_DEPRECATE /D _CRT_NONSTDC_NO_DEPRECATE /D _WINSOCK_DEPRECATED_NO_WARNINGS /D ACE_AS_STATIC_LIBS /D MPC_LIB_MODIFIER=\"s\" /c

CPP_PROJ=$(CPP_COMMON) /Fo"$(INTDIR)\\"


LINK32=link.exe -lib
LINK32_FLAGS=/nologo /machine:I386 /out:"..\..\lib\ACE_ETCL_Parsers.lib"
LINK32_OBJS= \
	"$(INTDIR)\ETCL_Interpreter.obj" \
	"$(INTDIR)\ETCL_l.obj" \
	"$(INTDIR)\ETCL_y.obj"

"$(OUTDIR)\ACE_ETCL_Parsers.lib" : $(DEF_FILE) $(LINK32_OBJS)
	$(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<
	if exist "$(OUTDIR)\ACE_ETCL_Parsers.lib.manifest" mt.exe -manifest "$(OUTDIR)\ACE_ETCL_Parsers.lib.manifest" -outputresource:$@;2

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
!IF EXISTS("Makefile.ACE_ETCL_Parser.dep")
!INCLUDE "Makefile.ACE_ETCL_Parser.dep"
!ENDIF
!ENDIF

!IF "$(CFG)" == "Win32 Debug" || "$(CFG)" == "Win32 Release" || "$(CFG)" == "Win32 Static Debug" || "$(CFG)" == "Win32 Static Release" 
SOURCE="ETCL_Interpreter.cpp"

"$(INTDIR)\ETCL_Interpreter.obj" : $(SOURCE)
	$(CPP) $(CPP_COMMON) /Fo"$(INTDIR)\ETCL_Interpreter.obj" $(SOURCE)

SOURCE="ETCL_l.cpp"

"$(INTDIR)\ETCL_l.obj" : $(SOURCE)
	$(CPP) $(CPP_COMMON) /Fo"$(INTDIR)\ETCL_l.obj" $(SOURCE)

SOURCE="ETCL_y.cpp"

"$(INTDIR)\ETCL_y.obj" : $(SOURCE)
	$(CPP) $(CPP_COMMON) /Fo"$(INTDIR)\ETCL_y.obj" $(SOURCE)

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
!IF EXISTS("Makefile.ACE_ETCL_Parser.dep")
	@echo Using "Makefile.ACE_ETCL_Parser.dep"
!ELSE
	@echo Warning: cannot find "Makefile.ACE_ETCL_Parser.dep"
!ENDIF
!ENDIF

