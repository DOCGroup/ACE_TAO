# Microsoft Developer Studio Project File - Name="Diamond Library" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Dynamic-Link Library" 0x0102

CFG=Diamond Library - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "Diamond.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "Diamond.mak" CFG="Diamond Library - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "Diamond Library - Win32 Release" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "Diamond Library - Win32 Debug" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "Diamond Library - Win32 Release"

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
# ADD BASE CPP /nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "Diamond_EXPORTS" /YX /FD /c
# ADD CPP /nologo /MD /W3 /GX /O2 /I "../../../../" /I "../../../" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "DIAMOND_BUILD_DLL" /FD /c
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
# ADD LINK32 TAO_PortableServer.lib ace.lib TAO.lib coll_test_stubs.lib /nologo /dll /machine:I386 /libpath:"..\..\..\..\ace" /libpath:"..\..\..\tao" /libpath:"..\..\..\tao\PortableServer" /libpath:"..\..\..\tao\SmartProxies"

!ELSEIF  "$(CFG)" == "Diamond Library - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Diamond___Win32_Debug"
# PROP BASE Intermediate_Dir "Diamond___Win32_Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir ""
# PROP Intermediate_Dir "Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MTd /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "Diamond_EXPORTS" /YX /FD /c
# ADD CPP /nologo /MDd /W3 /Gm /GX /Zi /Od /I "../../../../" /I "../../../" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "DIAMOND_BUILD_DLL" /FD /c
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
# ADD LINK32 coll_test_stubsd.lib TAO_SmartProxiesd.lib TAO_PortableServerd.lib TAOd.lib aced.lib coll_test_stubsd.lib /nologo /dll /debug /machine:I386 /out:"Diamondd.dll" /pdbtype:sept /libpath:"..\..\..\..\ace" /libpath:"..\..\..\tao" /libpath:"..\..\..\tao\PortableServer" /libpath:"..\..\..\tao\SmartProxies"

!ENDIF 

# Begin Target

# Name "Diamond Library - Win32 Release"
# Name "Diamond Library - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\Diamond_i.cpp
# End Source File
# Begin Source File

SOURCE=.\DiamondS.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\diamond_export.h
# End Source File
# Begin Source File

SOURCE=.\Diamond_i.h
# End Source File
# Begin Source File

SOURCE=.\DiamondC.h
# End Source File
# Begin Source File

SOURCE=.\DiamondS.h
# End Source File
# Begin Source File

SOURCE=.\DiamondS_T.h
# End Source File
# Begin Source File

SOURCE=.\stub_export.h
# End Source File
# End Group
# Begin Group "IDL Files"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\Diamond.idl

!IF  "$(CFG)" == "Diamond Library - Win32 Release"

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

!ELSEIF  "$(CFG)" == "Diamond Library - Win32 Debug"

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
# Begin Group "Template Files"

# PROP Default_Filter "_T.cpp"
# Begin Source File

SOURCE=.\DiamondS_T.cpp
# PROP Exclude_From_Build 1
# End Source File
# End Group
# End Target
# End Project
