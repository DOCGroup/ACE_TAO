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
!MESSAGE NMAKE /f "Makefile.SSL.mak" CFG="Win32 Debug"
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
INTDIR=Debug\SSL\I386

ALL : "$(INTDIR)" "$(OUTDIR)" DEPENDCHECK $(GENERATED_DIRTY) "..\..\lib\ACE_SSLd.dll"

DEPEND :
!IF "$(DEPGEN)" == ""
	@echo No suitable dependency generator could be found.
	@echo One comes with MPC, just set the MPC_ROOT environment variable
	@echo to the full path of MPC.  You can download MPC from
	@echo http://www.ociweb.com/products/mpc/down.html
!ELSE
	$(DEPGEN) -I"..\.." -I"$(SSL_ROOT)\include" -I"$(SSL_ROOT)\inc32" -D_DEBUG -DWIN32 -D_WINDOWS -D_CRT_SECURE_NO_WARNINGS -D_CRT_SECURE_NO_DEPRECATE -D_CRT_NONSTDC_NO_DEPRECATE -D_WINSOCK_DEPRECATED_NO_WARNINGS -DACE_SSL_BUILD_DLL -f "Makefile.SSL.dep" "SSL_Asynch_BIO.cpp" "SSL_Asynch_Stream.cpp" "SSL_Context.cpp" "SSL_Initializer.cpp" "SSL_SOCK.cpp" "SSL_SOCK_Acceptor.cpp" "SSL_SOCK_Connector.cpp" "SSL_SOCK_Stream.cpp"
!ENDIF

REALCLEAN : CLEAN
	-@del /f/q "$(OUTDIR)\ACE_SSLd.pdb"
	-@del /f/q "..\..\lib\ACE_SSLd.dll"
	-@del /f/q "$(OUTDIR)\ACE_SSLd.lib"
	-@del /f/q "$(OUTDIR)\ACE_SSLd.exp"
	-@del /f/q "$(OUTDIR)\ACE_SSLd.ilk"

"$(INTDIR)" :
	if not exist "Debug\$(NULL)" mkdir "Debug"
	if not exist "Debug\SSL\$(NULL)" mkdir "Debug\SSL"
	if not exist "$(INTDIR)\$(NULL)" mkdir "$(INTDIR)"
CPP=cl.exe
CPP_COMMON=/Zc:wchar_t /nologo /Ob0 /W3 /EHsc /Zi /MDd /GR /Gy /wd4355 /wd4456 /wd4459 /wd4458 /Fd"$(INTDIR)/" /I "..\.." /I "$(SSL_ROOT)\include" /I "$(SSL_ROOT)\inc32" /D _DEBUG /D WIN32 /D _WINDOWS /D _CRT_SECURE_NO_WARNINGS /D _CRT_SECURE_NO_DEPRECATE /D _CRT_NONSTDC_NO_DEPRECATE /D _WINSOCK_DEPRECATED_NO_WARNINGS /D ACE_SSL_BUILD_DLL /D MPC_LIB_MODIFIER=\"d\" /c

CPP_PROJ=$(CPP_COMMON) /Fo"$(INTDIR)\\"

RSC=rc.exe

LINK32=link.exe
LINK32_FLAGS=advapi32.lib user32.lib /INCREMENTAL:NO ACEd.lib iphlpapi.lib ssleay32.lib libeay32.lib /libpath:"." /libpath:"..\..\lib" /libpath:"$(SSL_ROOT)\lib" /libpath:"$(SSL_ROOT)\out32dll" /libpath:"$(SSL_ROOT)\out32" /libpath:"$(SSL_ROOT)\lib\VC" /nologo /subsystem:windows /dll /debug /pdb:"..\..\lib\ACE_SSLd.pdb" /machine:I386 /out:"..\..\lib\ACE_SSLd.dll" /implib:"$(OUTDIR)\ACE_SSLd.lib"
LINK32_OBJS= \
	"$(INTDIR)\SSL_Asynch_BIO.obj" \
	"$(INTDIR)\SSL_Asynch_Stream.obj" \
	"$(INTDIR)\SSL_Context.obj" \
	"$(INTDIR)\SSL_Initializer.obj" \
	"$(INTDIR)\SSL_SOCK.obj" \
	"$(INTDIR)\SSL_SOCK_Acceptor.obj" \
	"$(INTDIR)\SSL_SOCK_Connector.obj" \
	"$(INTDIR)\SSL_SOCK_Stream.obj"

"..\..\lib\ACE_SSLd.dll" : $(DEF_FILE) $(LINK32_OBJS)
	$(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<
	if exist "..\..\lib\ACE_SSLd.dll.manifest" mt.exe -manifest "..\..\lib\ACE_SSLd.dll.manifest" -outputresource:$@;2

!ELSEIF  "$(CFG)" == "Win32 Release"

OUTDIR=..\..\lib
INTDIR=Release\SSL\I386

ALL : "$(INTDIR)" "$(OUTDIR)" DEPENDCHECK $(GENERATED_DIRTY) "..\..\lib\ACE_SSL.dll"

DEPEND :
!IF "$(DEPGEN)" == ""
	@echo No suitable dependency generator could be found.
	@echo One comes with MPC, just set the MPC_ROOT environment variable
	@echo to the full path of MPC.  You can download MPC from
	@echo http://www.ociweb.com/products/mpc/down.html
!ELSE
	$(DEPGEN) -I"..\.." -I"$(SSL_ROOT)\include" -I"$(SSL_ROOT)\inc32" -DNDEBUG -DWIN32 -D_WINDOWS -D_CRT_SECURE_NO_WARNINGS -D_CRT_SECURE_NO_DEPRECATE -D_CRT_NONSTDC_NO_DEPRECATE -D_WINSOCK_DEPRECATED_NO_WARNINGS -DACE_SSL_BUILD_DLL -f "Makefile.SSL.dep" "SSL_Asynch_BIO.cpp" "SSL_Asynch_Stream.cpp" "SSL_Context.cpp" "SSL_Initializer.cpp" "SSL_SOCK.cpp" "SSL_SOCK_Acceptor.cpp" "SSL_SOCK_Connector.cpp" "SSL_SOCK_Stream.cpp"
!ENDIF

REALCLEAN : CLEAN
	-@del /f/q "..\..\lib\ACE_SSL.dll"
	-@del /f/q "$(OUTDIR)\ACE_SSL.lib"
	-@del /f/q "$(OUTDIR)\ACE_SSL.exp"
	-@del /f/q "$(OUTDIR)\ACE_SSL.ilk"

"$(INTDIR)" :
	if not exist "Release\$(NULL)" mkdir "Release"
	if not exist "Release\SSL\$(NULL)" mkdir "Release\SSL"
	if not exist "$(INTDIR)\$(NULL)" mkdir "$(INTDIR)"
CPP=cl.exe
CPP_COMMON=/Zc:wchar_t /nologo /O2 /W3 /EHsc /MD /GR /wd4355 /wd4456 /wd4459 /wd4458 /I "..\.." /I "$(SSL_ROOT)\include" /I "$(SSL_ROOT)\inc32" /D NDEBUG /D WIN32 /D _WINDOWS /D _CRT_SECURE_NO_WARNINGS /D _CRT_SECURE_NO_DEPRECATE /D _CRT_NONSTDC_NO_DEPRECATE /D _WINSOCK_DEPRECATED_NO_WARNINGS /D ACE_SSL_BUILD_DLL  /c

CPP_PROJ=$(CPP_COMMON) /Fo"$(INTDIR)\\"

RSC=rc.exe

LINK32=link.exe
LINK32_FLAGS=advapi32.lib user32.lib /INCREMENTAL:NO ACE.lib iphlpapi.lib ssleay32.lib libeay32.lib /libpath:"." /libpath:"..\..\lib" /libpath:"$(SSL_ROOT)\lib" /libpath:"$(SSL_ROOT)\out32dll" /libpath:"$(SSL_ROOT)\out32" /libpath:"$(SSL_ROOT)\lib\VC" /nologo /subsystem:windows /dll  /machine:I386 /out:"..\..\lib\ACE_SSL.dll" /implib:"$(OUTDIR)\ACE_SSL.lib"
LINK32_OBJS= \
	"$(INTDIR)\SSL_Asynch_BIO.obj" \
	"$(INTDIR)\SSL_Asynch_Stream.obj" \
	"$(INTDIR)\SSL_Context.obj" \
	"$(INTDIR)\SSL_Initializer.obj" \
	"$(INTDIR)\SSL_SOCK.obj" \
	"$(INTDIR)\SSL_SOCK_Acceptor.obj" \
	"$(INTDIR)\SSL_SOCK_Connector.obj" \
	"$(INTDIR)\SSL_SOCK_Stream.obj"

"..\..\lib\ACE_SSL.dll" : $(DEF_FILE) $(LINK32_OBJS)
	$(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<
	if exist "..\..\lib\ACE_SSL.dll.manifest" mt.exe -manifest "..\..\lib\ACE_SSL.dll.manifest" -outputresource:$@;2

!ELSEIF  "$(CFG)" == "Win32 Static Debug"

OUTDIR=..\..\lib
INTDIR=Static_Debug\SSL\I386

ALL : "$(INTDIR)" "$(OUTDIR)" DEPENDCHECK $(GENERATED_DIRTY) "$(OUTDIR)\ACE_SSLsd.lib"

DEPEND :
!IF "$(DEPGEN)" == ""
	@echo No suitable dependency generator could be found.
	@echo One comes with MPC, just set the MPC_ROOT environment variable
	@echo to the full path of MPC.  You can download MPC from
	@echo http://www.ociweb.com/products/mpc/down.html
!ELSE
	$(DEPGEN) -I"..\.." -I"$(SSL_ROOT)\include" -I"$(SSL_ROOT)\inc32" -D_DEBUG -DWIN32 -D_WINDOWS -D_CRT_SECURE_NO_WARNINGS -D_CRT_SECURE_NO_DEPRECATE -D_CRT_NONSTDC_NO_DEPRECATE -D_WINSOCK_DEPRECATED_NO_WARNINGS -DACE_AS_STATIC_LIBS -f "Makefile.SSL.dep" "SSL_Asynch_BIO.cpp" "SSL_Asynch_Stream.cpp" "SSL_Context.cpp" "SSL_Initializer.cpp" "SSL_SOCK.cpp" "SSL_SOCK_Acceptor.cpp" "SSL_SOCK_Connector.cpp" "SSL_SOCK_Stream.cpp"
!ENDIF

REALCLEAN : CLEAN
	-@del /f/q "$(OUTDIR)\ACE_SSLsd.lib"
	-@del /f/q "$(OUTDIR)\ACE_SSLsd.exp"
	-@del /f/q "$(OUTDIR)\ACE_SSLsd.ilk"
	-@del /f/q "..\..\lib\ACE_SSLsd.pdb"

"$(INTDIR)" :
	if not exist "Static_Debug\$(NULL)" mkdir "Static_Debug"
	if not exist "Static_Debug\SSL\$(NULL)" mkdir "Static_Debug\SSL"
	if not exist "$(INTDIR)\$(NULL)" mkdir "$(INTDIR)"
CPP=cl.exe
CPP_COMMON=/Zc:wchar_t /nologo /Ob0 /W3 /EHsc /Zi /GR /Gy /MDd /wd4355 /wd4456 /wd4459 /wd4458 /Fd"..\..\lib\ACE_SSLsd.pdb" /I "..\.." /I "$(SSL_ROOT)\include" /I "$(SSL_ROOT)\inc32" /D _DEBUG /D WIN32 /D _WINDOWS /D _CRT_SECURE_NO_WARNINGS /D _CRT_SECURE_NO_DEPRECATE /D _CRT_NONSTDC_NO_DEPRECATE /D _WINSOCK_DEPRECATED_NO_WARNINGS /D ACE_AS_STATIC_LIBS /D MPC_LIB_MODIFIER=\"sd\" /c

CPP_PROJ=$(CPP_COMMON) /Fo"$(INTDIR)\\"


LINK32=link.exe -lib
LINK32_FLAGS=/nologo /machine:I386 /out:"..\..\lib\ACE_SSLsd.lib"
LINK32_OBJS= \
	"$(INTDIR)\SSL_Asynch_BIO.obj" \
	"$(INTDIR)\SSL_Asynch_Stream.obj" \
	"$(INTDIR)\SSL_Context.obj" \
	"$(INTDIR)\SSL_Initializer.obj" \
	"$(INTDIR)\SSL_SOCK.obj" \
	"$(INTDIR)\SSL_SOCK_Acceptor.obj" \
	"$(INTDIR)\SSL_SOCK_Connector.obj" \
	"$(INTDIR)\SSL_SOCK_Stream.obj"

"$(OUTDIR)\ACE_SSLsd.lib" : $(DEF_FILE) $(LINK32_OBJS)
	$(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<
	if exist "$(OUTDIR)\ACE_SSLsd.lib.manifest" mt.exe -manifest "$(OUTDIR)\ACE_SSLsd.lib.manifest" -outputresource:$@;2

!ELSEIF  "$(CFG)" == "Win32 Static Release"

OUTDIR=..\..\lib
INTDIR=Static_Release\SSL\I386

ALL : "$(INTDIR)" "$(OUTDIR)" DEPENDCHECK $(GENERATED_DIRTY) "$(OUTDIR)\ACE_SSLs.lib"

DEPEND :
!IF "$(DEPGEN)" == ""
	@echo No suitable dependency generator could be found.
	@echo One comes with MPC, just set the MPC_ROOT environment variable
	@echo to the full path of MPC.  You can download MPC from
	@echo http://www.ociweb.com/products/mpc/down.html
!ELSE
	$(DEPGEN) -I"..\.." -I"$(SSL_ROOT)\include" -I"$(SSL_ROOT)\inc32" -DNDEBUG -DWIN32 -D_WINDOWS -D_CRT_SECURE_NO_WARNINGS -D_CRT_SECURE_NO_DEPRECATE -D_CRT_NONSTDC_NO_DEPRECATE -D_WINSOCK_DEPRECATED_NO_WARNINGS -DACE_AS_STATIC_LIBS -f "Makefile.SSL.dep" "SSL_Asynch_BIO.cpp" "SSL_Asynch_Stream.cpp" "SSL_Context.cpp" "SSL_Initializer.cpp" "SSL_SOCK.cpp" "SSL_SOCK_Acceptor.cpp" "SSL_SOCK_Connector.cpp" "SSL_SOCK_Stream.cpp"
!ENDIF

REALCLEAN : CLEAN
	-@del /f/q "$(OUTDIR)\ACE_SSLs.lib"
	-@del /f/q "$(OUTDIR)\ACE_SSLs.exp"
	-@del /f/q "$(OUTDIR)\ACE_SSLs.ilk"

"$(INTDIR)" :
	if not exist "Static_Release\$(NULL)" mkdir "Static_Release"
	if not exist "Static_Release\SSL\$(NULL)" mkdir "Static_Release\SSL"
	if not exist "$(INTDIR)\$(NULL)" mkdir "$(INTDIR)"
CPP=cl.exe
CPP_COMMON=/Zc:wchar_t /nologo /O2 /W3 /EHsc /MD /GR /wd4355 /wd4456 /wd4459 /wd4458 /I "..\.." /I "$(SSL_ROOT)\include" /I "$(SSL_ROOT)\inc32" /D NDEBUG /D WIN32 /D _WINDOWS /D _CRT_SECURE_NO_WARNINGS /D _CRT_SECURE_NO_DEPRECATE /D _CRT_NONSTDC_NO_DEPRECATE /D _WINSOCK_DEPRECATED_NO_WARNINGS /D ACE_AS_STATIC_LIBS /D MPC_LIB_MODIFIER=\"s\" /c

CPP_PROJ=$(CPP_COMMON) /Fo"$(INTDIR)\\"


LINK32=link.exe -lib
LINK32_FLAGS=/nologo /machine:I386 /out:"..\..\lib\ACE_SSLs.lib"
LINK32_OBJS= \
	"$(INTDIR)\SSL_Asynch_BIO.obj" \
	"$(INTDIR)\SSL_Asynch_Stream.obj" \
	"$(INTDIR)\SSL_Context.obj" \
	"$(INTDIR)\SSL_Initializer.obj" \
	"$(INTDIR)\SSL_SOCK.obj" \
	"$(INTDIR)\SSL_SOCK_Acceptor.obj" \
	"$(INTDIR)\SSL_SOCK_Connector.obj" \
	"$(INTDIR)\SSL_SOCK_Stream.obj"

"$(OUTDIR)\ACE_SSLs.lib" : $(DEF_FILE) $(LINK32_OBJS)
	$(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<
	if exist "$(OUTDIR)\ACE_SSLs.lib.manifest" mt.exe -manifest "$(OUTDIR)\ACE_SSLs.lib.manifest" -outputresource:$@;2

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
!IF EXISTS("Makefile.SSL.dep")
!INCLUDE "Makefile.SSL.dep"
!ENDIF
!ENDIF

!IF "$(CFG)" == "Win32 Debug" || "$(CFG)" == "Win32 Release" || "$(CFG)" == "Win32 Static Debug" || "$(CFG)" == "Win32 Static Release" 
SOURCE="SSL_Asynch_BIO.cpp"

"$(INTDIR)\SSL_Asynch_BIO.obj" : $(SOURCE)
	$(CPP) $(CPP_COMMON) /Fo"$(INTDIR)\SSL_Asynch_BIO.obj" $(SOURCE)

SOURCE="SSL_Asynch_Stream.cpp"

"$(INTDIR)\SSL_Asynch_Stream.obj" : $(SOURCE)
	$(CPP) $(CPP_COMMON) /Fo"$(INTDIR)\SSL_Asynch_Stream.obj" $(SOURCE)

SOURCE="SSL_Context.cpp"

"$(INTDIR)\SSL_Context.obj" : $(SOURCE)
	$(CPP) $(CPP_COMMON) /Fo"$(INTDIR)\SSL_Context.obj" $(SOURCE)

SOURCE="SSL_Initializer.cpp"

"$(INTDIR)\SSL_Initializer.obj" : $(SOURCE)
	$(CPP) $(CPP_COMMON) /Fo"$(INTDIR)\SSL_Initializer.obj" $(SOURCE)

SOURCE="SSL_SOCK.cpp"

"$(INTDIR)\SSL_SOCK.obj" : $(SOURCE)
	$(CPP) $(CPP_COMMON) /Fo"$(INTDIR)\SSL_SOCK.obj" $(SOURCE)

SOURCE="SSL_SOCK_Acceptor.cpp"

"$(INTDIR)\SSL_SOCK_Acceptor.obj" : $(SOURCE)
	$(CPP) $(CPP_COMMON) /Fo"$(INTDIR)\SSL_SOCK_Acceptor.obj" $(SOURCE)

SOURCE="SSL_SOCK_Connector.cpp"

"$(INTDIR)\SSL_SOCK_Connector.obj" : $(SOURCE)
	$(CPP) $(CPP_COMMON) /Fo"$(INTDIR)\SSL_SOCK_Connector.obj" $(SOURCE)

SOURCE="SSL_SOCK_Stream.cpp"

"$(INTDIR)\SSL_SOCK_Stream.obj" : $(SOURCE)
	$(CPP) $(CPP_COMMON) /Fo"$(INTDIR)\SSL_SOCK_Stream.obj" $(SOURCE)

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
!IF EXISTS("Makefile.SSL.dep")
	@echo Using "Makefile.SSL.dep"
!ELSE
	@echo Warning: cannot find "Makefile.SSL.dep"
!ENDIF
!ENDIF

