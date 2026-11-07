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
!MESSAGE NMAKE /f "Makefile.ACE_XML_Utils.mak" CFG="Win32 Debug"
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
INTDIR=Debug\ACE_XML_Utils\I386

ALL : "$(INTDIR)" "$(OUTDIR)" DEPENDCHECK $(GENERATED_DIRTY) "..\..\lib\ACE_XML_Utilsd.dll"

DEPEND :
!IF "$(DEPGEN)" == ""
	@echo No suitable dependency generator could be found.
	@echo One comes with MPC, just set the MPC_ROOT environment variable
	@echo to the full path of MPC.  You can download MPC from
	@echo http://www.ociweb.com/products/mpc/down.html
!ELSE
	$(DEPGEN) -I"..\.." -I"$(XERCESCROOT)\include" -D_DEBUG -DWIN32 -D_WINDOWS -D_CRT_SECURE_NO_WARNINGS -D_CRT_SECURE_NO_DEPRECATE -D_CRT_NONSTDC_NO_DEPRECATE -D_WINSOCK_DEPRECATED_NO_WARNINGS -DXML_UTILS_BUILD_DLL -f "Makefile.ACE_XML_Utils.dep" "XML_Error_Handler.cpp" "XML_Schema_Resolver.cpp" "XML_Typedefs.cpp" "XercesString.cpp"
!ENDIF

REALCLEAN : CLEAN
	-@del /f/q "$(OUTDIR)\ACE_XML_Utilsd.pdb"
	-@del /f/q "..\..\lib\ACE_XML_Utilsd.dll"
	-@del /f/q "$(OUTDIR)\ACE_XML_Utilsd.lib"
	-@del /f/q "$(OUTDIR)\ACE_XML_Utilsd.exp"
	-@del /f/q "$(OUTDIR)\ACE_XML_Utilsd.ilk"

"$(INTDIR)" :
	if not exist "Debug\$(NULL)" mkdir "Debug"
	if not exist "Debug\ACE_XML_Utils\$(NULL)" mkdir "Debug\ACE_XML_Utils"
	if not exist "$(INTDIR)\$(NULL)" mkdir "$(INTDIR)"
CPP=cl.exe
CPP_COMMON=/Zc:wchar_t /nologo /Ob0 /W3 /EHsc /Zi /MDd /GR /Gy /wd4355 /wd4456 /wd4459 /wd4458 /Fd"$(INTDIR)/" /I "..\.." /I "$(XERCESCROOT)\include" /D _DEBUG /D WIN32 /D _WINDOWS /D _CRT_SECURE_NO_WARNINGS /D _CRT_SECURE_NO_DEPRECATE /D _CRT_NONSTDC_NO_DEPRECATE /D _WINSOCK_DEPRECATED_NO_WARNINGS /D XML_UTILS_BUILD_DLL /D MPC_LIB_MODIFIER=\"d\" /c

CPP_PROJ=$(CPP_COMMON) /Fo"$(INTDIR)\\"

RSC=rc.exe

LINK32=link.exe
LINK32_FLAGS=advapi32.lib user32.lib /INCREMENTAL:NO ACEd.lib iphlpapi.lib xerces-c_3D.lib /libpath:"." /libpath:"..\..\lib" /libpath:"$(XERCESCROOT)\lib" /nologo /subsystem:windows /dll /debug /pdb:"..\..\lib\ACE_XML_Utilsd.pdb" /machine:I386 /out:"..\..\lib\ACE_XML_Utilsd.dll" /implib:"$(OUTDIR)\ACE_XML_Utilsd.lib"
LINK32_OBJS= \
	"$(INTDIR)\XML_Error_Handler.obj" \
	"$(INTDIR)\XML_Schema_Resolver.obj" \
	"$(INTDIR)\XML_Typedefs.obj" \
	"$(INTDIR)\XercesString.obj"

"..\..\lib\ACE_XML_Utilsd.dll" : $(DEF_FILE) $(LINK32_OBJS)
	$(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<
	if exist "..\..\lib\ACE_XML_Utilsd.dll.manifest" mt.exe -manifest "..\..\lib\ACE_XML_Utilsd.dll.manifest" -outputresource:$@;2

!ELSEIF  "$(CFG)" == "Win32 Release"

OUTDIR=..\..\lib
INTDIR=Release\ACE_XML_Utils\I386

ALL : "$(INTDIR)" "$(OUTDIR)" DEPENDCHECK $(GENERATED_DIRTY) "..\..\lib\ACE_XML_Utils.dll"

DEPEND :
!IF "$(DEPGEN)" == ""
	@echo No suitable dependency generator could be found.
	@echo One comes with MPC, just set the MPC_ROOT environment variable
	@echo to the full path of MPC.  You can download MPC from
	@echo http://www.ociweb.com/products/mpc/down.html
!ELSE
	$(DEPGEN) -I"..\.." -I"$(XERCESCROOT)\include" -DNDEBUG -DWIN32 -D_WINDOWS -D_CRT_SECURE_NO_WARNINGS -D_CRT_SECURE_NO_DEPRECATE -D_CRT_NONSTDC_NO_DEPRECATE -D_WINSOCK_DEPRECATED_NO_WARNINGS -DXML_UTILS_BUILD_DLL -f "Makefile.ACE_XML_Utils.dep" "XML_Error_Handler.cpp" "XML_Schema_Resolver.cpp" "XML_Typedefs.cpp" "XercesString.cpp"
!ENDIF

REALCLEAN : CLEAN
	-@del /f/q "..\..\lib\ACE_XML_Utils.dll"
	-@del /f/q "$(OUTDIR)\ACE_XML_Utils.lib"
	-@del /f/q "$(OUTDIR)\ACE_XML_Utils.exp"
	-@del /f/q "$(OUTDIR)\ACE_XML_Utils.ilk"

"$(INTDIR)" :
	if not exist "Release\$(NULL)" mkdir "Release"
	if not exist "Release\ACE_XML_Utils\$(NULL)" mkdir "Release\ACE_XML_Utils"
	if not exist "$(INTDIR)\$(NULL)" mkdir "$(INTDIR)"
CPP=cl.exe
CPP_COMMON=/Zc:wchar_t /nologo /O2 /W3 /EHsc /MD /GR /wd4355 /wd4456 /wd4459 /wd4458 /I "..\.." /I "$(XERCESCROOT)\include" /D NDEBUG /D WIN32 /D _WINDOWS /D _CRT_SECURE_NO_WARNINGS /D _CRT_SECURE_NO_DEPRECATE /D _CRT_NONSTDC_NO_DEPRECATE /D _WINSOCK_DEPRECATED_NO_WARNINGS /D XML_UTILS_BUILD_DLL  /c

CPP_PROJ=$(CPP_COMMON) /Fo"$(INTDIR)\\"

RSC=rc.exe

LINK32=link.exe
LINK32_FLAGS=advapi32.lib user32.lib /INCREMENTAL:NO ACE.lib iphlpapi.lib xerces-c_3.lib /libpath:"." /libpath:"..\..\lib" /libpath:"$(XERCESCROOT)\lib" /nologo /subsystem:windows /dll  /machine:I386 /out:"..\..\lib\ACE_XML_Utils.dll" /implib:"$(OUTDIR)\ACE_XML_Utils.lib"
LINK32_OBJS= \
	"$(INTDIR)\XML_Error_Handler.obj" \
	"$(INTDIR)\XML_Schema_Resolver.obj" \
	"$(INTDIR)\XML_Typedefs.obj" \
	"$(INTDIR)\XercesString.obj"

"..\..\lib\ACE_XML_Utils.dll" : $(DEF_FILE) $(LINK32_OBJS)
	$(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<
	if exist "..\..\lib\ACE_XML_Utils.dll.manifest" mt.exe -manifest "..\..\lib\ACE_XML_Utils.dll.manifest" -outputresource:$@;2

!ELSEIF  "$(CFG)" == "Win32 Static Debug"

OUTDIR=..\..\lib
INTDIR=Static_Debug\ACE_XML_Utils\I386

ALL : "$(INTDIR)" "$(OUTDIR)" DEPENDCHECK $(GENERATED_DIRTY) "$(OUTDIR)\ACE_XML_Utilssd.lib"

DEPEND :
!IF "$(DEPGEN)" == ""
	@echo No suitable dependency generator could be found.
	@echo One comes with MPC, just set the MPC_ROOT environment variable
	@echo to the full path of MPC.  You can download MPC from
	@echo http://www.ociweb.com/products/mpc/down.html
!ELSE
	$(DEPGEN) -I"..\.." -I"$(XERCESCROOT)\include" -D_DEBUG -DWIN32 -D_WINDOWS -D_CRT_SECURE_NO_WARNINGS -D_CRT_SECURE_NO_DEPRECATE -D_CRT_NONSTDC_NO_DEPRECATE -D_WINSOCK_DEPRECATED_NO_WARNINGS -DACE_AS_STATIC_LIBS -f "Makefile.ACE_XML_Utils.dep" "XML_Error_Handler.cpp" "XML_Schema_Resolver.cpp" "XML_Typedefs.cpp" "XercesString.cpp"
!ENDIF

REALCLEAN : CLEAN
	-@del /f/q "$(OUTDIR)\ACE_XML_Utilssd.lib"
	-@del /f/q "$(OUTDIR)\ACE_XML_Utilssd.exp"
	-@del /f/q "$(OUTDIR)\ACE_XML_Utilssd.ilk"
	-@del /f/q "..\..\lib\ACE_XML_Utilssd.pdb"

"$(INTDIR)" :
	if not exist "Static_Debug\$(NULL)" mkdir "Static_Debug"
	if not exist "Static_Debug\ACE_XML_Utils\$(NULL)" mkdir "Static_Debug\ACE_XML_Utils"
	if not exist "$(INTDIR)\$(NULL)" mkdir "$(INTDIR)"
CPP=cl.exe
CPP_COMMON=/Zc:wchar_t /nologo /Ob0 /W3 /EHsc /Zi /GR /Gy /MDd /wd4355 /wd4456 /wd4459 /wd4458 /Fd"..\..\lib\ACE_XML_Utilssd.pdb" /I "..\.." /I "$(XERCESCROOT)\include" /D _DEBUG /D WIN32 /D _WINDOWS /D _CRT_SECURE_NO_WARNINGS /D _CRT_SECURE_NO_DEPRECATE /D _CRT_NONSTDC_NO_DEPRECATE /D _WINSOCK_DEPRECATED_NO_WARNINGS /D ACE_AS_STATIC_LIBS /D MPC_LIB_MODIFIER=\"sd\" /c

CPP_PROJ=$(CPP_COMMON) /Fo"$(INTDIR)\\"


LINK32=link.exe -lib
LINK32_FLAGS=/nologo /machine:I386 /out:"..\..\lib\ACE_XML_Utilssd.lib"
LINK32_OBJS= \
	"$(INTDIR)\XML_Error_Handler.obj" \
	"$(INTDIR)\XML_Schema_Resolver.obj" \
	"$(INTDIR)\XML_Typedefs.obj" \
	"$(INTDIR)\XercesString.obj"

"$(OUTDIR)\ACE_XML_Utilssd.lib" : $(DEF_FILE) $(LINK32_OBJS)
	$(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<
	if exist "$(OUTDIR)\ACE_XML_Utilssd.lib.manifest" mt.exe -manifest "$(OUTDIR)\ACE_XML_Utilssd.lib.manifest" -outputresource:$@;2

!ELSEIF  "$(CFG)" == "Win32 Static Release"

OUTDIR=..\..\lib
INTDIR=Static_Release\ACE_XML_Utils\I386

ALL : "$(INTDIR)" "$(OUTDIR)" DEPENDCHECK $(GENERATED_DIRTY) "$(OUTDIR)\ACE_XML_Utilss.lib"

DEPEND :
!IF "$(DEPGEN)" == ""
	@echo No suitable dependency generator could be found.
	@echo One comes with MPC, just set the MPC_ROOT environment variable
	@echo to the full path of MPC.  You can download MPC from
	@echo http://www.ociweb.com/products/mpc/down.html
!ELSE
	$(DEPGEN) -I"..\.." -I"$(XERCESCROOT)\include" -DNDEBUG -DWIN32 -D_WINDOWS -D_CRT_SECURE_NO_WARNINGS -D_CRT_SECURE_NO_DEPRECATE -D_CRT_NONSTDC_NO_DEPRECATE -D_WINSOCK_DEPRECATED_NO_WARNINGS -DACE_AS_STATIC_LIBS -f "Makefile.ACE_XML_Utils.dep" "XML_Error_Handler.cpp" "XML_Schema_Resolver.cpp" "XML_Typedefs.cpp" "XercesString.cpp"
!ENDIF

REALCLEAN : CLEAN
	-@del /f/q "$(OUTDIR)\ACE_XML_Utilss.lib"
	-@del /f/q "$(OUTDIR)\ACE_XML_Utilss.exp"
	-@del /f/q "$(OUTDIR)\ACE_XML_Utilss.ilk"

"$(INTDIR)" :
	if not exist "Static_Release\$(NULL)" mkdir "Static_Release"
	if not exist "Static_Release\ACE_XML_Utils\$(NULL)" mkdir "Static_Release\ACE_XML_Utils"
	if not exist "$(INTDIR)\$(NULL)" mkdir "$(INTDIR)"
CPP=cl.exe
CPP_COMMON=/Zc:wchar_t /nologo /O2 /W3 /EHsc /MD /GR /wd4355 /wd4456 /wd4459 /wd4458 /I "..\.." /I "$(XERCESCROOT)\include" /D NDEBUG /D WIN32 /D _WINDOWS /D _CRT_SECURE_NO_WARNINGS /D _CRT_SECURE_NO_DEPRECATE /D _CRT_NONSTDC_NO_DEPRECATE /D _WINSOCK_DEPRECATED_NO_WARNINGS /D ACE_AS_STATIC_LIBS /D MPC_LIB_MODIFIER=\"s\" /c

CPP_PROJ=$(CPP_COMMON) /Fo"$(INTDIR)\\"


LINK32=link.exe -lib
LINK32_FLAGS=/nologo /machine:I386 /out:"..\..\lib\ACE_XML_Utilss.lib"
LINK32_OBJS= \
	"$(INTDIR)\XML_Error_Handler.obj" \
	"$(INTDIR)\XML_Schema_Resolver.obj" \
	"$(INTDIR)\XML_Typedefs.obj" \
	"$(INTDIR)\XercesString.obj"

"$(OUTDIR)\ACE_XML_Utilss.lib" : $(DEF_FILE) $(LINK32_OBJS)
	$(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<
	if exist "$(OUTDIR)\ACE_XML_Utilss.lib.manifest" mt.exe -manifest "$(OUTDIR)\ACE_XML_Utilss.lib.manifest" -outputresource:$@;2

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
!IF EXISTS("Makefile.ACE_XML_Utils.dep")
!INCLUDE "Makefile.ACE_XML_Utils.dep"
!ENDIF
!ENDIF

!IF "$(CFG)" == "Win32 Debug" || "$(CFG)" == "Win32 Release" || "$(CFG)" == "Win32 Static Debug" || "$(CFG)" == "Win32 Static Release" 
SOURCE="XML_Error_Handler.cpp"

"$(INTDIR)\XML_Error_Handler.obj" : $(SOURCE)
	$(CPP) $(CPP_COMMON) /Fo"$(INTDIR)\XML_Error_Handler.obj" $(SOURCE)

SOURCE="XML_Schema_Resolver.cpp"

"$(INTDIR)\XML_Schema_Resolver.obj" : $(SOURCE)
	$(CPP) $(CPP_COMMON) /Fo"$(INTDIR)\XML_Schema_Resolver.obj" $(SOURCE)

SOURCE="XML_Typedefs.cpp"

"$(INTDIR)\XML_Typedefs.obj" : $(SOURCE)
	$(CPP) $(CPP_COMMON) /Fo"$(INTDIR)\XML_Typedefs.obj" $(SOURCE)

SOURCE="XercesString.cpp"

"$(INTDIR)\XercesString.obj" : $(SOURCE)
	$(CPP) $(CPP_COMMON) /Fo"$(INTDIR)\XercesString.obj" $(SOURCE)

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
!IF EXISTS("Makefile.ACE_XML_Utils.dep")
	@echo Using "Makefile.ACE_XML_Utils.dep"
!ELSE
	@echo Warning: cannot find "Makefile.ACE_XML_Utils.dep"
!ENDIF
!ENDIF

