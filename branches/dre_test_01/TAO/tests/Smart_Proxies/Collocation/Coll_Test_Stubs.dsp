# Microsoft Developer Studio Project File - Name="Coll_Test_Stubs Library" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Dynamic-Link Library" 0x0102

CFG=Coll_Test_Stubs Library - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "Coll_Test_Stubs.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "Coll_Test_Stubs.mak" CFG="Coll_Test_Stubs Library - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "Coll_Test_Stubs Library - Win32 Release" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "Coll_Test_Stubs Library - Win32 Debug" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "Coll_Test_Stubs Library - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir ""
# PROP Intermediate_Dir "Release"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "COLL_TEST_STUBS_EXPORTS" /YX /FD /c
# ADD CPP /nologo /MD /W3 /GX /O2 /I "../../../../" /I "../../../" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "MY_STUB_BUILD_DLL" /FD /c
# SUBTRACT CPP /YX
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /machine:I386
# ADD LINK32 TAO_SmartProxies.lib TAO.lib ace.lib /nologo /dll /machine:I386 /libpath:"..\..\..\..\ace" /libpath:"..\..\..\tao" /libpath:"..\..\..\tao\SmartProxies"

!ELSEIF  "$(CFG)" == "Coll_Test_Stubs Library - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Coll_Test_Stubs___Win32_Debug"
# PROP BASE Intermediate_Dir "Coll_Test_Stubs___Win32_Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir ""
# PROP Intermediate_Dir "Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MTd /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "COLL_TEST_STUBS_EXPORTS" /YX /FD /c
# ADD CPP /nologo /MDd /W3 /Gm /GX /Zi /Od /I "../../../../" /I "../../../" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "MY_STUB_BUILD_DLL" /FD /c
# SUBTRACT CPP /YX
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /debug /machine:I386 /pdbtype:sept
# ADD LINK32 TAO_SmartProxiesd.lib TAOd.lib aced.lib /nologo /dll /debug /machine:I386 /out:"Coll_Test_Stubsd.dll" /pdbtype:sept /libpath:"..\..\..\..\ace" /libpath:"..\..\..\tao" /libpath:"..\..\..\tao\SmartProxies"

!ENDIF 

# Begin Target

# Name "Coll_Test_Stubs Library - Win32 Release"
# Name "Coll_Test_Stubs Library - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\DiamondC.cpp
# ADD CPP /GR
# End Source File
# Begin Source File

SOURCE=.\Smart_Proxy_Impl.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\DiamondC.h
# End Source File
# Begin Source File

SOURCE=.\Smart_Proxy_impl.h
# End Source File
# Begin Source File

SOURCE=.\stub_export.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# End Group
# Begin Group "IDL Files"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\Diamond.idl

!IF  "$(CFG)" == "Coll_Test_Stubs Library - Win32 Release"

# PROP Ignore_Default_Tool 1
USERDEP__DIAMO="..\..\..\..\bin\release\tao_idl.exe"	
# Begin Custom Build - Invoking tao_idl on $(InputName).idl
InputPath=.\Diamond.idl
InputName=Diamond

BuildCmds= \
	..\..\..\..\bin\release\tao_idl.exe -Ge 1 -Gsp -Gd  -Wb,skel_export_macro=Diamond_Export -Wb,stub_export_macro=MY_Stub_Export  -Wb,skel_export_include=diamond_export.h -Wb,stub_export_include=stub_export.h  $(InputName).idl

"$(InputName)C.h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)C.i" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)C.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)S.h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)S.i" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)S.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)S_T.h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)S_T.i" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)S_T.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)
# End Custom Build

!ELSEIF  "$(CFG)" == "Coll_Test_Stubs Library - Win32 Debug"

# PROP Ignore_Default_Tool 1
USERDEP__DIAMO="..\..\..\..\bin\tao_idl.exe"	
# Begin Custom Build - Invoking tao_idl on $(InputName).idl
InputPath=.\Diamond.idl
InputName=Diamond

BuildCmds= \
	..\..\..\..\bin\tao_idl.exe -Ge 1 -Gsp -Gd -Wb,skel_export_macro=Diamond_Export  -Wb,stub_export_macro=MY_Stub_Export -Wb,skel_export_include=diamond_export.h  -Wb,stub_export_include=stub_export.h $(InputName).idl

"$(InputName)C.h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)C.i" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)C.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)S.h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)S.i" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)S.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)S_T.h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)S_T.i" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)S_T.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)
# End Custom Build

!ENDIF 

# End Source File
# End Group
# End Target
# End Project
