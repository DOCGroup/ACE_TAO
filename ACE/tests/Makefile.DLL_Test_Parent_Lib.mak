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
!MESSAGE NMAKE /f "Makefile.DLL_Test_Parent_Lib.mak" CFG="Win32 Debug"
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
INTDIR=Debug\DLL_Test_Parent_Lib\I386

ALL : "$(INTDIR)" "$(OUTDIR)" DEPENDCHECK $(GENERATED_DIRTY) ".\DLL_Test_Parentd.dll"

DEPEND :
!IF "$(DEPGEN)" == ""
	@echo No suitable dependency generator could be found.
	@echo One comes with MPC, just set the MPC_ROOT environment variable
	@echo to the full path of MPC.  You can download MPC from
	@echo https://github.com/DOCGroup/MPC
!ELSE
	$(DEPGEN) -I".." -D_DEBUG -DWIN32 -D_WINDOWS -DACE_HAS_IPV6 -D_CRT_SECURE_NO_WARNINGS -D_CRT_SECURE_NO_DEPRECATE -D_CRT_NONSTDC_NO_DEPRECATE -D_WINSOCK_DEPRECATED_NO_WARNINGS -DDLL_TEST_PARENT_BUILD_DLL -f "Makefile.DLL_Test_Parent_Lib.dep" "DLL_Test_Parent.cpp"
!ENDIF

REALCLEAN : CLEAN
	-@del /f/q "$(OUTDIR)\DLL_Test_Parentd.pdb"
	-@del /f/q ".\DLL_Test_Parentd.dll"
	-@del /f/q "$(OUTDIR)\DLL_Test_Parentd.lib"
	-@del /f/q "$(OUTDIR)\DLL_Test_Parentd.exp"
	-@del /f/q "$(OUTDIR)\DLL_Test_Parentd.ilk"

"$(INTDIR)" :
	if not exist "Debug\$(NULL)" mkdir "Debug"
	if not exist "Debug\DLL_Test_Parent_Lib\$(NULL)" mkdir "Debug\DLL_Test_Parent_Lib"
	if not exist "$(INTDIR)\$(NULL)" mkdir "$(INTDIR)"
CPP=cl.exe
CPP_COMMON=/Zc:wchar_t /nologo /Ob0 /W3 /EHsc /Zi /MDd /GR /Gy /std:c++17 /wd4355 /wd4456 /wd4459 /wd4458 /Fd"$(INTDIR)/" /I ".." /D _DEBUG /D WIN32 /D _WINDOWS /D ACE_HAS_IPV6 /D _CRT_SECURE_NO_WARNINGS /D _CRT_SECURE_NO_DEPRECATE /D _CRT_NONSTDC_NO_DEPRECATE /D _WINSOCK_DEPRECATED_NO_WARNINGS /D DLL_TEST_PARENT_BUILD_DLL /D MPC_LIB_MODIFIER=\"d\" /c

CPP_PROJ=$(CPP_COMMON) /Fo"$(INTDIR)\\"

RSC=rc.exe

LINK32=link.exe
LINK32_FLAGS=advapi32.lib user32.lib /INCREMENTAL:NO ACEd.lib iphlpapi.lib /libpath:"." /libpath:"..\lib" /nologo /subsystem:windows /dll /debug /pdb:".\DLL_Test_Parentd.pdb" /machine:I386 /out:".\DLL_Test_Parentd.dll" /implib:"$(OUTDIR)\DLL_Test_Parentd.lib"
LINK32_OBJS= \
	"$(INTDIR)\DLL_Test_Parent.obj"

".\DLL_Test_Parentd.dll" : $(DEF_FILE) $(LINK32_OBJS)
	$(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<
	if exist ".\DLL_Test_Parentd.dll.manifest" mt.exe -manifest ".\DLL_Test_Parentd.dll.manifest" -outputresource:$@;2

!ELSEIF  "$(CFG)" == "Win32 Release"

OUTDIR=.
INTDIR=Release\DLL_Test_Parent_Lib\I386

ALL : "$(INTDIR)" "$(OUTDIR)" DEPENDCHECK $(GENERATED_DIRTY) ".\DLL_Test_Parent.dll"

DEPEND :
!IF "$(DEPGEN)" == ""
	@echo No suitable dependency generator could be found.
	@echo One comes with MPC, just set the MPC_ROOT environment variable
	@echo to the full path of MPC.  You can download MPC from
	@echo https://github.com/DOCGroup/MPC
!ELSE
	$(DEPGEN) -I".." -DNDEBUG -DWIN32 -D_WINDOWS -DACE_HAS_IPV6 -D_CRT_SECURE_NO_WARNINGS -D_CRT_SECURE_NO_DEPRECATE -D_CRT_NONSTDC_NO_DEPRECATE -D_WINSOCK_DEPRECATED_NO_WARNINGS -DDLL_TEST_PARENT_BUILD_DLL -f "Makefile.DLL_Test_Parent_Lib.dep" "DLL_Test_Parent.cpp"
!ENDIF

REALCLEAN : CLEAN
	-@del /f/q ".\DLL_Test_Parent.dll"
	-@del /f/q "$(OUTDIR)\DLL_Test_Parent.lib"
	-@del /f/q "$(OUTDIR)\DLL_Test_Parent.exp"
	-@del /f/q "$(OUTDIR)\DLL_Test_Parent.ilk"

"$(INTDIR)" :
	if not exist "Release\$(NULL)" mkdir "Release"
	if not exist "Release\DLL_Test_Parent_Lib\$(NULL)" mkdir "Release\DLL_Test_Parent_Lib"
	if not exist "$(INTDIR)\$(NULL)" mkdir "$(INTDIR)"
CPP=cl.exe
CPP_COMMON=/Zc:wchar_t /nologo /O2 /W3 /EHsc /MD /GR /std:c++17 /wd4355 /wd4456 /wd4459 /wd4458 /I ".." /D NDEBUG /D WIN32 /D _WINDOWS /D ACE_HAS_IPV6 /D _CRT_SECURE_NO_WARNINGS /D _CRT_SECURE_NO_DEPRECATE /D _CRT_NONSTDC_NO_DEPRECATE /D _WINSOCK_DEPRECATED_NO_WARNINGS /D DLL_TEST_PARENT_BUILD_DLL  /c

CPP_PROJ=$(CPP_COMMON) /Fo"$(INTDIR)\\"

RSC=rc.exe

LINK32=link.exe
LINK32_FLAGS=advapi32.lib user32.lib /INCREMENTAL:NO ACE.lib iphlpapi.lib /libpath:"." /libpath:"..\lib" /nologo /subsystem:windows /dll  /machine:I386 /out:".\DLL_Test_Parent.dll" /implib:"$(OUTDIR)\DLL_Test_Parent.lib"
LINK32_OBJS= \
	"$(INTDIR)\DLL_Test_Parent.obj"

".\DLL_Test_Parent.dll" : $(DEF_FILE) $(LINK32_OBJS)
	$(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<
	if exist ".\DLL_Test_Parent.dll.manifest" mt.exe -manifest ".\DLL_Test_Parent.dll.manifest" -outputresource:$@;2

!ELSEIF  "$(CFG)" == "Win32 Static Debug"

OUTDIR=.
INTDIR=Static_Debug\DLL_Test_Parent_Lib\I386

ALL : "$(INTDIR)" "$(OUTDIR)" DEPENDCHECK $(GENERATED_DIRTY) "$(OUTDIR)\DLL_Test_Parentsd.lib"

DEPEND :
!IF "$(DEPGEN)" == ""
	@echo No suitable dependency generator could be found.
	@echo One comes with MPC, just set the MPC_ROOT environment variable
	@echo to the full path of MPC.  You can download MPC from
	@echo https://github.com/DOCGroup/MPC
!ELSE
	$(DEPGEN) -I".." -D_DEBUG -DWIN32 -D_WINDOWS -DACE_HAS_IPV6 -D_CRT_SECURE_NO_WARNINGS -D_CRT_SECURE_NO_DEPRECATE -D_CRT_NONSTDC_NO_DEPRECATE -D_WINSOCK_DEPRECATED_NO_WARNINGS -DACE_AS_STATIC_LIBS -f "Makefile.DLL_Test_Parent_Lib.dep" "DLL_Test_Parent.cpp"
!ENDIF

REALCLEAN : CLEAN
	-@del /f/q "$(OUTDIR)\DLL_Test_Parentsd.lib"
	-@del /f/q "$(OUTDIR)\DLL_Test_Parentsd.exp"
	-@del /f/q "$(OUTDIR)\DLL_Test_Parentsd.ilk"
	-@del /f/q ".\DLL_Test_Parentsd.pdb"

"$(INTDIR)" :
	if not exist "Static_Debug\$(NULL)" mkdir "Static_Debug"
	if not exist "Static_Debug\DLL_Test_Parent_Lib\$(NULL)" mkdir "Static_Debug\DLL_Test_Parent_Lib"
	if not exist "$(INTDIR)\$(NULL)" mkdir "$(INTDIR)"
CPP=cl.exe
CPP_COMMON=/Zc:wchar_t /nologo /Ob0 /W3 /EHsc /Zi /GR /Gy /MDd /std:c++17 /wd4355 /wd4456 /wd4459 /wd4458 /Fd".\DLL_Test_Parentsd.pdb" /I ".." /D _DEBUG /D WIN32 /D _WINDOWS /D ACE_HAS_IPV6 /D _CRT_SECURE_NO_WARNINGS /D _CRT_SECURE_NO_DEPRECATE /D _CRT_NONSTDC_NO_DEPRECATE /D _WINSOCK_DEPRECATED_NO_WARNINGS /D ACE_AS_STATIC_LIBS /D MPC_LIB_MODIFIER=\"sd\" /c

CPP_PROJ=$(CPP_COMMON) /Fo"$(INTDIR)\\"


LINK32=link.exe -lib
LINK32_FLAGS=/nologo /machine:I386 /out:".\DLL_Test_Parentsd.lib"
LINK32_OBJS= \
	"$(INTDIR)\DLL_Test_Parent.obj"

"$(OUTDIR)\DLL_Test_Parentsd.lib" : $(DEF_FILE) $(LINK32_OBJS)
	$(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<
	if exist "$(OUTDIR)\DLL_Test_Parentsd.lib.manifest" mt.exe -manifest "$(OUTDIR)\DLL_Test_Parentsd.lib.manifest" -outputresource:$@;2

!ELSEIF  "$(CFG)" == "Win32 Static Release"

OUTDIR=.
INTDIR=Static_Release\DLL_Test_Parent_Lib\I386

ALL : "$(INTDIR)" "$(OUTDIR)" DEPENDCHECK $(GENERATED_DIRTY) "$(OUTDIR)\DLL_Test_Parents.lib"

DEPEND :
!IF "$(DEPGEN)" == ""
	@echo No suitable dependency generator could be found.
	@echo One comes with MPC, just set the MPC_ROOT environment variable
	@echo to the full path of MPC.  You can download MPC from
	@echo https://github.com/DOCGroup/MPC
!ELSE
	$(DEPGEN) -I".." -DNDEBUG -DWIN32 -D_WINDOWS -DACE_HAS_IPV6 -D_CRT_SECURE_NO_WARNINGS -D_CRT_SECURE_NO_DEPRECATE -D_CRT_NONSTDC_NO_DEPRECATE -D_WINSOCK_DEPRECATED_NO_WARNINGS -DACE_AS_STATIC_LIBS -f "Makefile.DLL_Test_Parent_Lib.dep" "DLL_Test_Parent.cpp"
!ENDIF

REALCLEAN : CLEAN
	-@del /f/q "$(OUTDIR)\DLL_Test_Parents.lib"
	-@del /f/q "$(OUTDIR)\DLL_Test_Parents.exp"
	-@del /f/q "$(OUTDIR)\DLL_Test_Parents.ilk"

"$(INTDIR)" :
	if not exist "Static_Release\$(NULL)" mkdir "Static_Release"
	if not exist "Static_Release\DLL_Test_Parent_Lib\$(NULL)" mkdir "Static_Release\DLL_Test_Parent_Lib"
	if not exist "$(INTDIR)\$(NULL)" mkdir "$(INTDIR)"
CPP=cl.exe
CPP_COMMON=/Zc:wchar_t /nologo /O2 /W3 /EHsc /MD /GR /std:c++17 /wd4355 /wd4456 /wd4459 /wd4458 /I ".." /D NDEBUG /D WIN32 /D _WINDOWS /D ACE_HAS_IPV6 /D _CRT_SECURE_NO_WARNINGS /D _CRT_SECURE_NO_DEPRECATE /D _CRT_NONSTDC_NO_DEPRECATE /D _WINSOCK_DEPRECATED_NO_WARNINGS /D ACE_AS_STATIC_LIBS /D MPC_LIB_MODIFIER=\"s\" /c

CPP_PROJ=$(CPP_COMMON) /Fo"$(INTDIR)\\"


LINK32=link.exe -lib
LINK32_FLAGS=/nologo /machine:I386 /out:".\DLL_Test_Parents.lib"
LINK32_OBJS= \
	"$(INTDIR)\DLL_Test_Parent.obj"

"$(OUTDIR)\DLL_Test_Parents.lib" : $(DEF_FILE) $(LINK32_OBJS)
	$(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<
	if exist "$(OUTDIR)\DLL_Test_Parents.lib.manifest" mt.exe -manifest "$(OUTDIR)\DLL_Test_Parents.lib.manifest" -outputresource:$@;2

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
!IF EXISTS("Makefile.DLL_Test_Parent_Lib.dep")
!INCLUDE "Makefile.DLL_Test_Parent_Lib.dep"
!ENDIF
!ENDIF

!IF "$(CFG)" == "Win32 Debug" || "$(CFG)" == "Win32 Release" || "$(CFG)" == "Win32 Static Debug" || "$(CFG)" == "Win32 Static Release" 
SOURCE="DLL_Test_Parent.cpp"

"$(INTDIR)\DLL_Test_Parent.obj" : $(SOURCE)
	$(CPP) $(CPP_COMMON) /Fo"$(INTDIR)\DLL_Test_Parent.obj" $(SOURCE)

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
!IF EXISTS("Makefile.DLL_Test_Parent_Lib.dep")
	@echo Using "Makefile.DLL_Test_Parent_Lib.dep"
!ELSE
	@echo Warning: cannot find "Makefile.DLL_Test_Parent_Lib.dep"
!ENDIF
!ENDIF

