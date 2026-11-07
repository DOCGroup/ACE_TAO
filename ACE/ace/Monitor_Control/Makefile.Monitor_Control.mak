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
!MESSAGE NMAKE /f "Makefile.Monitor_Control.mak" CFG="Win32 Debug"
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
INTDIR=Debug\Monitor_Control\I386

ALL : "$(INTDIR)" "$(OUTDIR)" DEPENDCHECK $(GENERATED_DIRTY) "..\..\lib\ACE_Monitor_Controld.dll"

DEPEND :
!IF "$(DEPGEN)" == ""
	@echo No suitable dependency generator could be found.
	@echo One comes with MPC, just set the MPC_ROOT environment variable
	@echo to the full path of MPC.  You can download MPC from
	@echo http://www.ociweb.com/products/mpc/down.html
!ELSE
	$(DEPGEN) -I"..\.." -D_DEBUG -DWIN32 -D_WINDOWS -D_CRT_SECURE_NO_WARNINGS -D_CRT_SECURE_NO_DEPRECATE -D_CRT_NONSTDC_NO_DEPRECATE -D_WINSOCK_DEPRECATED_NO_WARNINGS -DMONITOR_CONTROL_BUILD_DLL -f "Makefile.Monitor_Control.dep" "Auto_Update_Starter.cpp" "BSD_Network_Interface_Monitor.cpp" "Bytes_Received_Monitor.cpp" "Bytes_Sent_Monitor.cpp" "CPU_Load_Monitor.cpp" "Constraint_Interpreter.cpp" "Constraint_Visitor.cpp" "FreeBSD_Network_Interface_Monitor.cpp" "Linux_Network_Interface_Monitor.cpp" "Memory_Usage_Monitor.cpp" "Monitor_Group.cpp" "Monitor_Query.cpp" "Null_Network_Interface_Monitor.cpp" "Num_Threads_Monitor.cpp" "Packets_Received_Monitor.cpp" "Packets_Sent_Monitor.cpp" "Solaris_Network_Interface_Monitor.cpp" "Windows_Monitor.cpp" "Windows_Multi_Instance_Monitor.cpp"
!ENDIF

REALCLEAN : CLEAN
	-@del /f/q "$(OUTDIR)\ACE_Monitor_Controld.pdb"
	-@del /f/q "..\..\lib\ACE_Monitor_Controld.dll"
	-@del /f/q "$(OUTDIR)\ACE_Monitor_Controld.lib"
	-@del /f/q "$(OUTDIR)\ACE_Monitor_Controld.exp"
	-@del /f/q "$(OUTDIR)\ACE_Monitor_Controld.ilk"

"$(INTDIR)" :
	if not exist "Debug\$(NULL)" mkdir "Debug"
	if not exist "Debug\Monitor_Control\$(NULL)" mkdir "Debug\Monitor_Control"
	if not exist "$(INTDIR)\$(NULL)" mkdir "$(INTDIR)"
CPP=cl.exe
CPP_COMMON=/Zc:wchar_t /nologo /Ob0 /W3 /EHsc /Zi /MDd /GR /Gy /wd4355 /wd4456 /wd4459 /wd4458 /Fd"$(INTDIR)/" /I "..\.." /D _DEBUG /D WIN32 /D _WINDOWS /D _CRT_SECURE_NO_WARNINGS /D _CRT_SECURE_NO_DEPRECATE /D _CRT_NONSTDC_NO_DEPRECATE /D _WINSOCK_DEPRECATED_NO_WARNINGS /D MONITOR_CONTROL_BUILD_DLL /D MPC_LIB_MODIFIER=\"d\" /c

CPP_PROJ=$(CPP_COMMON) /Fo"$(INTDIR)\\"

RSC=rc.exe

LINK32=link.exe
LINK32_FLAGS=advapi32.lib user32.lib /INCREMENTAL:NO ACEd.lib ACE_ETCLd.lib ACE_ETCL_Parserd.lib iphlpapi.lib pdh.lib /libpath:"." /libpath:"..\..\lib" /nologo /subsystem:windows /dll /debug /pdb:"..\..\lib\ACE_Monitor_Controld.pdb" /machine:I386 /out:"..\..\lib\ACE_Monitor_Controld.dll" /implib:"$(OUTDIR)\ACE_Monitor_Controld.lib"
LINK32_OBJS= \
	"$(INTDIR)\Auto_Update_Starter.obj" \
	"$(INTDIR)\BSD_Network_Interface_Monitor.obj" \
	"$(INTDIR)\Bytes_Received_Monitor.obj" \
	"$(INTDIR)\Bytes_Sent_Monitor.obj" \
	"$(INTDIR)\CPU_Load_Monitor.obj" \
	"$(INTDIR)\Constraint_Interpreter.obj" \
	"$(INTDIR)\Constraint_Visitor.obj" \
	"$(INTDIR)\FreeBSD_Network_Interface_Monitor.obj" \
	"$(INTDIR)\Linux_Network_Interface_Monitor.obj" \
	"$(INTDIR)\Memory_Usage_Monitor.obj" \
	"$(INTDIR)\Monitor_Group.obj" \
	"$(INTDIR)\Monitor_Query.obj" \
	"$(INTDIR)\Null_Network_Interface_Monitor.obj" \
	"$(INTDIR)\Num_Threads_Monitor.obj" \
	"$(INTDIR)\Packets_Received_Monitor.obj" \
	"$(INTDIR)\Packets_Sent_Monitor.obj" \
	"$(INTDIR)\Solaris_Network_Interface_Monitor.obj" \
	"$(INTDIR)\Windows_Monitor.obj" \
	"$(INTDIR)\Windows_Multi_Instance_Monitor.obj"

"..\..\lib\ACE_Monitor_Controld.dll" : $(DEF_FILE) $(LINK32_OBJS)
	$(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<
	if exist "..\..\lib\ACE_Monitor_Controld.dll.manifest" mt.exe -manifest "..\..\lib\ACE_Monitor_Controld.dll.manifest" -outputresource:$@;2

!ELSEIF  "$(CFG)" == "Win32 Release"

OUTDIR=..\..\lib
INTDIR=Release\Monitor_Control\I386

ALL : "$(INTDIR)" "$(OUTDIR)" DEPENDCHECK $(GENERATED_DIRTY) "..\..\lib\ACE_Monitor_Control.dll"

DEPEND :
!IF "$(DEPGEN)" == ""
	@echo No suitable dependency generator could be found.
	@echo One comes with MPC, just set the MPC_ROOT environment variable
	@echo to the full path of MPC.  You can download MPC from
	@echo http://www.ociweb.com/products/mpc/down.html
!ELSE
	$(DEPGEN) -I"..\.." -DNDEBUG -DWIN32 -D_WINDOWS -D_CRT_SECURE_NO_WARNINGS -D_CRT_SECURE_NO_DEPRECATE -D_CRT_NONSTDC_NO_DEPRECATE -D_WINSOCK_DEPRECATED_NO_WARNINGS -DMONITOR_CONTROL_BUILD_DLL -f "Makefile.Monitor_Control.dep" "Auto_Update_Starter.cpp" "BSD_Network_Interface_Monitor.cpp" "Bytes_Received_Monitor.cpp" "Bytes_Sent_Monitor.cpp" "CPU_Load_Monitor.cpp" "Constraint_Interpreter.cpp" "Constraint_Visitor.cpp" "FreeBSD_Network_Interface_Monitor.cpp" "Linux_Network_Interface_Monitor.cpp" "Memory_Usage_Monitor.cpp" "Monitor_Group.cpp" "Monitor_Query.cpp" "Null_Network_Interface_Monitor.cpp" "Num_Threads_Monitor.cpp" "Packets_Received_Monitor.cpp" "Packets_Sent_Monitor.cpp" "Solaris_Network_Interface_Monitor.cpp" "Windows_Monitor.cpp" "Windows_Multi_Instance_Monitor.cpp"
!ENDIF

REALCLEAN : CLEAN
	-@del /f/q "..\..\lib\ACE_Monitor_Control.dll"
	-@del /f/q "$(OUTDIR)\ACE_Monitor_Control.lib"
	-@del /f/q "$(OUTDIR)\ACE_Monitor_Control.exp"
	-@del /f/q "$(OUTDIR)\ACE_Monitor_Control.ilk"

"$(INTDIR)" :
	if not exist "Release\$(NULL)" mkdir "Release"
	if not exist "Release\Monitor_Control\$(NULL)" mkdir "Release\Monitor_Control"
	if not exist "$(INTDIR)\$(NULL)" mkdir "$(INTDIR)"
CPP=cl.exe
CPP_COMMON=/Zc:wchar_t /nologo /O2 /W3 /EHsc /MD /GR /wd4355 /wd4456 /wd4459 /wd4458 /I "..\.." /D NDEBUG /D WIN32 /D _WINDOWS /D _CRT_SECURE_NO_WARNINGS /D _CRT_SECURE_NO_DEPRECATE /D _CRT_NONSTDC_NO_DEPRECATE /D _WINSOCK_DEPRECATED_NO_WARNINGS /D MONITOR_CONTROL_BUILD_DLL  /c

CPP_PROJ=$(CPP_COMMON) /Fo"$(INTDIR)\\"

RSC=rc.exe

LINK32=link.exe
LINK32_FLAGS=advapi32.lib user32.lib /INCREMENTAL:NO ACE.lib ACE_ETCL.lib ACE_ETCL_Parser.lib iphlpapi.lib pdh.lib /libpath:"." /libpath:"..\..\lib" /nologo /subsystem:windows /dll  /machine:I386 /out:"..\..\lib\ACE_Monitor_Control.dll" /implib:"$(OUTDIR)\ACE_Monitor_Control.lib"
LINK32_OBJS= \
	"$(INTDIR)\Auto_Update_Starter.obj" \
	"$(INTDIR)\BSD_Network_Interface_Monitor.obj" \
	"$(INTDIR)\Bytes_Received_Monitor.obj" \
	"$(INTDIR)\Bytes_Sent_Monitor.obj" \
	"$(INTDIR)\CPU_Load_Monitor.obj" \
	"$(INTDIR)\Constraint_Interpreter.obj" \
	"$(INTDIR)\Constraint_Visitor.obj" \
	"$(INTDIR)\FreeBSD_Network_Interface_Monitor.obj" \
	"$(INTDIR)\Linux_Network_Interface_Monitor.obj" \
	"$(INTDIR)\Memory_Usage_Monitor.obj" \
	"$(INTDIR)\Monitor_Group.obj" \
	"$(INTDIR)\Monitor_Query.obj" \
	"$(INTDIR)\Null_Network_Interface_Monitor.obj" \
	"$(INTDIR)\Num_Threads_Monitor.obj" \
	"$(INTDIR)\Packets_Received_Monitor.obj" \
	"$(INTDIR)\Packets_Sent_Monitor.obj" \
	"$(INTDIR)\Solaris_Network_Interface_Monitor.obj" \
	"$(INTDIR)\Windows_Monitor.obj" \
	"$(INTDIR)\Windows_Multi_Instance_Monitor.obj"

"..\..\lib\ACE_Monitor_Control.dll" : $(DEF_FILE) $(LINK32_OBJS)
	$(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<
	if exist "..\..\lib\ACE_Monitor_Control.dll.manifest" mt.exe -manifest "..\..\lib\ACE_Monitor_Control.dll.manifest" -outputresource:$@;2

!ELSEIF  "$(CFG)" == "Win32 Static Debug"

OUTDIR=..\..\lib
INTDIR=Static_Debug\Monitor_Control\I386

ALL : "$(INTDIR)" "$(OUTDIR)" DEPENDCHECK $(GENERATED_DIRTY) "$(OUTDIR)\ACE_Monitor_Controlsd.lib"

DEPEND :
!IF "$(DEPGEN)" == ""
	@echo No suitable dependency generator could be found.
	@echo One comes with MPC, just set the MPC_ROOT environment variable
	@echo to the full path of MPC.  You can download MPC from
	@echo http://www.ociweb.com/products/mpc/down.html
!ELSE
	$(DEPGEN) -I"..\.." -D_DEBUG -DWIN32 -D_WINDOWS -D_CRT_SECURE_NO_WARNINGS -D_CRT_SECURE_NO_DEPRECATE -D_CRT_NONSTDC_NO_DEPRECATE -D_WINSOCK_DEPRECATED_NO_WARNINGS -DACE_AS_STATIC_LIBS -f "Makefile.Monitor_Control.dep" "Auto_Update_Starter.cpp" "BSD_Network_Interface_Monitor.cpp" "Bytes_Received_Monitor.cpp" "Bytes_Sent_Monitor.cpp" "CPU_Load_Monitor.cpp" "Constraint_Interpreter.cpp" "Constraint_Visitor.cpp" "FreeBSD_Network_Interface_Monitor.cpp" "Linux_Network_Interface_Monitor.cpp" "Memory_Usage_Monitor.cpp" "Monitor_Group.cpp" "Monitor_Query.cpp" "Null_Network_Interface_Monitor.cpp" "Num_Threads_Monitor.cpp" "Packets_Received_Monitor.cpp" "Packets_Sent_Monitor.cpp" "Solaris_Network_Interface_Monitor.cpp" "Windows_Monitor.cpp" "Windows_Multi_Instance_Monitor.cpp"
!ENDIF

REALCLEAN : CLEAN
	-@del /f/q "$(OUTDIR)\ACE_Monitor_Controlsd.lib"
	-@del /f/q "$(OUTDIR)\ACE_Monitor_Controlsd.exp"
	-@del /f/q "$(OUTDIR)\ACE_Monitor_Controlsd.ilk"
	-@del /f/q "..\..\lib\ACE_Monitor_Controlsd.pdb"

"$(INTDIR)" :
	if not exist "Static_Debug\$(NULL)" mkdir "Static_Debug"
	if not exist "Static_Debug\Monitor_Control\$(NULL)" mkdir "Static_Debug\Monitor_Control"
	if not exist "$(INTDIR)\$(NULL)" mkdir "$(INTDIR)"
CPP=cl.exe
CPP_COMMON=/Zc:wchar_t /nologo /Ob0 /W3 /EHsc /Zi /GR /Gy /MDd /wd4355 /wd4456 /wd4459 /wd4458 /Fd"..\..\lib\ACE_Monitor_Controlsd.pdb" /I "..\.." /D _DEBUG /D WIN32 /D _WINDOWS /D _CRT_SECURE_NO_WARNINGS /D _CRT_SECURE_NO_DEPRECATE /D _CRT_NONSTDC_NO_DEPRECATE /D _WINSOCK_DEPRECATED_NO_WARNINGS /D ACE_AS_STATIC_LIBS /D MPC_LIB_MODIFIER=\"sd\" /c

CPP_PROJ=$(CPP_COMMON) /Fo"$(INTDIR)\\"


LINK32=link.exe -lib
LINK32_FLAGS=/nologo /machine:I386 /out:"..\..\lib\ACE_Monitor_Controlsd.lib"
LINK32_OBJS= \
	"$(INTDIR)\Auto_Update_Starter.obj" \
	"$(INTDIR)\BSD_Network_Interface_Monitor.obj" \
	"$(INTDIR)\Bytes_Received_Monitor.obj" \
	"$(INTDIR)\Bytes_Sent_Monitor.obj" \
	"$(INTDIR)\CPU_Load_Monitor.obj" \
	"$(INTDIR)\Constraint_Interpreter.obj" \
	"$(INTDIR)\Constraint_Visitor.obj" \
	"$(INTDIR)\FreeBSD_Network_Interface_Monitor.obj" \
	"$(INTDIR)\Linux_Network_Interface_Monitor.obj" \
	"$(INTDIR)\Memory_Usage_Monitor.obj" \
	"$(INTDIR)\Monitor_Group.obj" \
	"$(INTDIR)\Monitor_Query.obj" \
	"$(INTDIR)\Null_Network_Interface_Monitor.obj" \
	"$(INTDIR)\Num_Threads_Monitor.obj" \
	"$(INTDIR)\Packets_Received_Monitor.obj" \
	"$(INTDIR)\Packets_Sent_Monitor.obj" \
	"$(INTDIR)\Solaris_Network_Interface_Monitor.obj" \
	"$(INTDIR)\Windows_Monitor.obj" \
	"$(INTDIR)\Windows_Multi_Instance_Monitor.obj"

"$(OUTDIR)\ACE_Monitor_Controlsd.lib" : $(DEF_FILE) $(LINK32_OBJS)
	$(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<
	if exist "$(OUTDIR)\ACE_Monitor_Controlsd.lib.manifest" mt.exe -manifest "$(OUTDIR)\ACE_Monitor_Controlsd.lib.manifest" -outputresource:$@;2

!ELSEIF  "$(CFG)" == "Win32 Static Release"

OUTDIR=..\..\lib
INTDIR=Static_Release\Monitor_Control\I386

ALL : "$(INTDIR)" "$(OUTDIR)" DEPENDCHECK $(GENERATED_DIRTY) "$(OUTDIR)\ACE_Monitor_Controls.lib"

DEPEND :
!IF "$(DEPGEN)" == ""
	@echo No suitable dependency generator could be found.
	@echo One comes with MPC, just set the MPC_ROOT environment variable
	@echo to the full path of MPC.  You can download MPC from
	@echo http://www.ociweb.com/products/mpc/down.html
!ELSE
	$(DEPGEN) -I"..\.." -DNDEBUG -DWIN32 -D_WINDOWS -D_CRT_SECURE_NO_WARNINGS -D_CRT_SECURE_NO_DEPRECATE -D_CRT_NONSTDC_NO_DEPRECATE -D_WINSOCK_DEPRECATED_NO_WARNINGS -DACE_AS_STATIC_LIBS -f "Makefile.Monitor_Control.dep" "Auto_Update_Starter.cpp" "BSD_Network_Interface_Monitor.cpp" "Bytes_Received_Monitor.cpp" "Bytes_Sent_Monitor.cpp" "CPU_Load_Monitor.cpp" "Constraint_Interpreter.cpp" "Constraint_Visitor.cpp" "FreeBSD_Network_Interface_Monitor.cpp" "Linux_Network_Interface_Monitor.cpp" "Memory_Usage_Monitor.cpp" "Monitor_Group.cpp" "Monitor_Query.cpp" "Null_Network_Interface_Monitor.cpp" "Num_Threads_Monitor.cpp" "Packets_Received_Monitor.cpp" "Packets_Sent_Monitor.cpp" "Solaris_Network_Interface_Monitor.cpp" "Windows_Monitor.cpp" "Windows_Multi_Instance_Monitor.cpp"
!ENDIF

REALCLEAN : CLEAN
	-@del /f/q "$(OUTDIR)\ACE_Monitor_Controls.lib"
	-@del /f/q "$(OUTDIR)\ACE_Monitor_Controls.exp"
	-@del /f/q "$(OUTDIR)\ACE_Monitor_Controls.ilk"

"$(INTDIR)" :
	if not exist "Static_Release\$(NULL)" mkdir "Static_Release"
	if not exist "Static_Release\Monitor_Control\$(NULL)" mkdir "Static_Release\Monitor_Control"
	if not exist "$(INTDIR)\$(NULL)" mkdir "$(INTDIR)"
CPP=cl.exe
CPP_COMMON=/Zc:wchar_t /nologo /O2 /W3 /EHsc /MD /GR /wd4355 /wd4456 /wd4459 /wd4458 /I "..\.." /D NDEBUG /D WIN32 /D _WINDOWS /D _CRT_SECURE_NO_WARNINGS /D _CRT_SECURE_NO_DEPRECATE /D _CRT_NONSTDC_NO_DEPRECATE /D _WINSOCK_DEPRECATED_NO_WARNINGS /D ACE_AS_STATIC_LIBS /D MPC_LIB_MODIFIER=\"s\" /c

CPP_PROJ=$(CPP_COMMON) /Fo"$(INTDIR)\\"


LINK32=link.exe -lib
LINK32_FLAGS=/nologo /machine:I386 /out:"..\..\lib\ACE_Monitor_Controls.lib"
LINK32_OBJS= \
	"$(INTDIR)\Auto_Update_Starter.obj" \
	"$(INTDIR)\BSD_Network_Interface_Monitor.obj" \
	"$(INTDIR)\Bytes_Received_Monitor.obj" \
	"$(INTDIR)\Bytes_Sent_Monitor.obj" \
	"$(INTDIR)\CPU_Load_Monitor.obj" \
	"$(INTDIR)\Constraint_Interpreter.obj" \
	"$(INTDIR)\Constraint_Visitor.obj" \
	"$(INTDIR)\FreeBSD_Network_Interface_Monitor.obj" \
	"$(INTDIR)\Linux_Network_Interface_Monitor.obj" \
	"$(INTDIR)\Memory_Usage_Monitor.obj" \
	"$(INTDIR)\Monitor_Group.obj" \
	"$(INTDIR)\Monitor_Query.obj" \
	"$(INTDIR)\Null_Network_Interface_Monitor.obj" \
	"$(INTDIR)\Num_Threads_Monitor.obj" \
	"$(INTDIR)\Packets_Received_Monitor.obj" \
	"$(INTDIR)\Packets_Sent_Monitor.obj" \
	"$(INTDIR)\Solaris_Network_Interface_Monitor.obj" \
	"$(INTDIR)\Windows_Monitor.obj" \
	"$(INTDIR)\Windows_Multi_Instance_Monitor.obj"

"$(OUTDIR)\ACE_Monitor_Controls.lib" : $(DEF_FILE) $(LINK32_OBJS)
	$(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<
	if exist "$(OUTDIR)\ACE_Monitor_Controls.lib.manifest" mt.exe -manifest "$(OUTDIR)\ACE_Monitor_Controls.lib.manifest" -outputresource:$@;2

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
!IF EXISTS("Makefile.Monitor_Control.dep")
!INCLUDE "Makefile.Monitor_Control.dep"
!ENDIF
!ENDIF

!IF "$(CFG)" == "Win32 Debug" || "$(CFG)" == "Win32 Release" || "$(CFG)" == "Win32 Static Debug" || "$(CFG)" == "Win32 Static Release" 
SOURCE="Auto_Update_Starter.cpp"

"$(INTDIR)\Auto_Update_Starter.obj" : $(SOURCE)
	$(CPP) $(CPP_COMMON) /Fo"$(INTDIR)\Auto_Update_Starter.obj" $(SOURCE)

SOURCE="BSD_Network_Interface_Monitor.cpp"

"$(INTDIR)\BSD_Network_Interface_Monitor.obj" : $(SOURCE)
	$(CPP) $(CPP_COMMON) /Fo"$(INTDIR)\BSD_Network_Interface_Monitor.obj" $(SOURCE)

SOURCE="Bytes_Received_Monitor.cpp"

"$(INTDIR)\Bytes_Received_Monitor.obj" : $(SOURCE)
	$(CPP) $(CPP_COMMON) /Fo"$(INTDIR)\Bytes_Received_Monitor.obj" $(SOURCE)

SOURCE="Bytes_Sent_Monitor.cpp"

"$(INTDIR)\Bytes_Sent_Monitor.obj" : $(SOURCE)
	$(CPP) $(CPP_COMMON) /Fo"$(INTDIR)\Bytes_Sent_Monitor.obj" $(SOURCE)

SOURCE="CPU_Load_Monitor.cpp"

"$(INTDIR)\CPU_Load_Monitor.obj" : $(SOURCE)
	$(CPP) $(CPP_COMMON) /Fo"$(INTDIR)\CPU_Load_Monitor.obj" $(SOURCE)

SOURCE="Constraint_Interpreter.cpp"

"$(INTDIR)\Constraint_Interpreter.obj" : $(SOURCE)
	$(CPP) $(CPP_COMMON) /Fo"$(INTDIR)\Constraint_Interpreter.obj" $(SOURCE)

SOURCE="Constraint_Visitor.cpp"

"$(INTDIR)\Constraint_Visitor.obj" : $(SOURCE)
	$(CPP) $(CPP_COMMON) /Fo"$(INTDIR)\Constraint_Visitor.obj" $(SOURCE)

SOURCE="FreeBSD_Network_Interface_Monitor.cpp"

"$(INTDIR)\FreeBSD_Network_Interface_Monitor.obj" : $(SOURCE)
	$(CPP) $(CPP_COMMON) /Fo"$(INTDIR)\FreeBSD_Network_Interface_Monitor.obj" $(SOURCE)

SOURCE="Linux_Network_Interface_Monitor.cpp"

"$(INTDIR)\Linux_Network_Interface_Monitor.obj" : $(SOURCE)
	$(CPP) $(CPP_COMMON) /Fo"$(INTDIR)\Linux_Network_Interface_Monitor.obj" $(SOURCE)

SOURCE="Memory_Usage_Monitor.cpp"

"$(INTDIR)\Memory_Usage_Monitor.obj" : $(SOURCE)
	$(CPP) $(CPP_COMMON) /Fo"$(INTDIR)\Memory_Usage_Monitor.obj" $(SOURCE)

SOURCE="Monitor_Group.cpp"

"$(INTDIR)\Monitor_Group.obj" : $(SOURCE)
	$(CPP) $(CPP_COMMON) /Fo"$(INTDIR)\Monitor_Group.obj" $(SOURCE)

SOURCE="Monitor_Query.cpp"

"$(INTDIR)\Monitor_Query.obj" : $(SOURCE)
	$(CPP) $(CPP_COMMON) /Fo"$(INTDIR)\Monitor_Query.obj" $(SOURCE)

SOURCE="Null_Network_Interface_Monitor.cpp"

"$(INTDIR)\Null_Network_Interface_Monitor.obj" : $(SOURCE)
	$(CPP) $(CPP_COMMON) /Fo"$(INTDIR)\Null_Network_Interface_Monitor.obj" $(SOURCE)

SOURCE="Num_Threads_Monitor.cpp"

"$(INTDIR)\Num_Threads_Monitor.obj" : $(SOURCE)
	$(CPP) $(CPP_COMMON) /Fo"$(INTDIR)\Num_Threads_Monitor.obj" $(SOURCE)

SOURCE="Packets_Received_Monitor.cpp"

"$(INTDIR)\Packets_Received_Monitor.obj" : $(SOURCE)
	$(CPP) $(CPP_COMMON) /Fo"$(INTDIR)\Packets_Received_Monitor.obj" $(SOURCE)

SOURCE="Packets_Sent_Monitor.cpp"

"$(INTDIR)\Packets_Sent_Monitor.obj" : $(SOURCE)
	$(CPP) $(CPP_COMMON) /Fo"$(INTDIR)\Packets_Sent_Monitor.obj" $(SOURCE)

SOURCE="Solaris_Network_Interface_Monitor.cpp"

"$(INTDIR)\Solaris_Network_Interface_Monitor.obj" : $(SOURCE)
	$(CPP) $(CPP_COMMON) /Fo"$(INTDIR)\Solaris_Network_Interface_Monitor.obj" $(SOURCE)

SOURCE="Windows_Monitor.cpp"

"$(INTDIR)\Windows_Monitor.obj" : $(SOURCE)
	$(CPP) $(CPP_COMMON) /Fo"$(INTDIR)\Windows_Monitor.obj" $(SOURCE)

SOURCE="Windows_Multi_Instance_Monitor.cpp"

"$(INTDIR)\Windows_Multi_Instance_Monitor.obj" : $(SOURCE)
	$(CPP) $(CPP_COMMON) /Fo"$(INTDIR)\Windows_Multi_Instance_Monitor.obj" $(SOURCE)

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
!IF EXISTS("Makefile.Monitor_Control.dep")
	@echo Using "Makefile.Monitor_Control.dep"
!ELSE
	@echo Warning: cannot find "Makefile.Monitor_Control.dep"
!ENDIF
!ENDIF

