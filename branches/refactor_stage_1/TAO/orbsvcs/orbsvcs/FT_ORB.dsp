# Microsoft Developer Studio Project File - Name="FT ORB" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Dynamic-Link Library" 0x0102

CFG=FT ORB - Win32 MFC Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "FT_ORB.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "FT_ORB.mak" CFG="FT ORB - Win32 MFC Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "FT ORB - Win32 Release" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "FT ORB - Win32 Debug" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "FT ORB - Win32 mfc Release" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "FT ORB - Win32 MFC Debug" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "FT ORB - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir ""
# PROP Intermediate_Dir "DLL\Release\FT_ORB"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "TAO_FT_EXPORTS" /YX /FD /c
# ADD CPP /nologo /MD /W3 /GX /O2 /I "../" /I "../../" /I "../../../" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "TAO_FT_BUILD_DLL" /D "TAO_ORBSVCS_HAS_FT" /FD /c
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
# ADD LINK32 ace.lib TAO.lib TAO_IORManip.lib TAO_PortableServer.lib /nologo /dll /machine:I386 /out:"..\..\..\bin\TAO_FT_ORB.dll" /libpath:"..\..\tao\PortableServer" /libpath:"..\..\tao" /libpath:"..\..\tao\IORManipulation" /libpath:"..\..\..\ace"

!ELSEIF  "$(CFG)" == "FT ORB - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir ""
# PROP Intermediate_Dir "DLL\Debug\FT_ORB"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MTd /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "TAO_FT_EXPORTS" /YX /FD /c
# ADD CPP /nologo /MDd /W3 /Gm /GR- /GX /Zi /Od /I "../" /I "../../" /I "../../../" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "TAO_FT_BUILD_DLL" /D "TAO_ORBSVCS_HAS_FT" /FD /c
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
# ADD LINK32 TAO_PortableServerd.lib TAOd.lib TAO_IORManipd.lib aced.lib /nologo /dll /debug /machine:I386 /out:"..\..\..\bin\TAO_FT_ORBd.dll" /pdbtype:sept /libpath:"..\..\tao\PortableServer" /libpath:"..\..\tao" /libpath:"..\..\tao\IORManipulation" /libpath:"..\..\..\ace"
# SUBTRACT LINK32 /pdb:none

!ELSEIF  "$(CFG)" == "FT ORB - Win32 mfc Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "FT_ORB___Win32_mfc_Release"
# PROP BASE Intermediate_Dir "FT_ORB___Win32_mfc_Release"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir ""
# PROP Intermediate_Dir "DLL\Release\FT_ORBMFC"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MD /W3 /GX /O2 /I "../" /I "../../" /I "../../../" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "TAO_FT_BUILD_DLL" /D "TAO_ORBSVCS_HAS_FT" /FD /c
# SUBTRACT BASE CPP /YX
# ADD CPP /nologo /MD /W3 /GX /O2 /I "../" /I "../../" /I "../../../" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "TAO_FT_BUILD_DLL" /D "TAO_ORBSVCS_HAS_FT" /D ACE_HAS_MFC=1 /FD /c
# SUBTRACT CPP /YX
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 ace.lib TAO.lib /nologo /dll /machine:I386 /out:"..\..\..\bin\TAO_FT_ORB.dll" /libpath:"..\..\tao\IORManipulation" /libpath:"..\..\tao" /libpath:"..\..\..\ace"
# ADD LINK32 acemfc.lib TAOmfc.lib TAO_IORManipmfc.lib TAO_PortableServermfc.lib /nologo /dll /machine:I386 /out:"..\..\..\bin\TAO_FT_ORBmfc.dll" /libpath:"..\..\tao\PortableServer" /libpath:"..\..\tao" /libpath:"..\..\tao\IORManipulation" /libpath:"..\..\..\ace"

!ELSEIF  "$(CFG)" == "FT ORB - Win32 MFC Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "FT_ORB___Win32_MFC_Debug"
# PROP BASE Intermediate_Dir "FT_ORB___Win32_MFC_Debug"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir ""
# PROP Intermediate_Dir "DLL\Debug\FT_ORB"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MDd /W3 /Gm /GX /Zi /Od /I "../" /I "../../" /I "../../../" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "TAO_FT_BUILD_DLL" /D "TAO_ORBSVCS_HAS_FT" /FD /c
# SUBTRACT BASE CPP /YX
# ADD CPP /nologo /MDd /W3 /Gm /GX /Zi /Od /I "../" /I "../../" /I "../../../" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "TAO_FT_BUILD_DLL" /D "TAO_ORBSVCS_HAS_FT" /D ACE_HAS_MFC=1 /FD /c
# SUBTRACT CPP /YX
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 TAOd.lib aced.lib /nologo /dll /debug /machine:I386 /out:"..\..\..\bin\TAO_FT_ORBd.dll" /pdbtype:sept /libpath:"..\..\tao" /libpath:"..\..\tao\IORManipulation" /libpath:"..\..\..\ace"
# ADD LINK32 TAO_PortableServermfcd.lib TAO_IORManipmfcd.lib TAOmfcd.lib acemfcd.lib /nologo /dll /debug /machine:I386 /out:"..\..\..\bin\TAO_FT_ORBmfcd.dll" /pdbtype:sept /libpath:"..\..\tao\PortableServer" /libpath:"..\..\tao\IORManipulation" /libpath:"..\..\tao" /libpath:"..\..\..\ace"

!ENDIF 

# Begin Target

# Name "FT ORB - Win32 Release"
# Name "FT ORB - Win32 Debug"
# Name "FT ORB - Win32 mfc Release"
# Name "FT ORB - Win32 MFC Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\FT_CORBA_ORBC.cpp
# ADD CPP /GR
# End Source File
# Begin Source File

SOURCE=.\FT_CORBA_ORBS.cpp
# End Source File
# Begin Source File

SOURCE=.\FaultTolerance\FT_Endpoint_Selector_Factory.cpp
# End Source File
# Begin Source File

SOURCE=.\FaultTolerance\FT_Invocation_Endpoint_Selectors.cpp
# End Source File
# Begin Source File

SOURCE=.\FaultTolerance\FT_IOGR_Property.cpp
# End Source File
# Begin Source File

SOURCE=.\FaultTolerance\FT_ORBInitializer.cpp
# End Source File
# Begin Source File

SOURCE=.\FaultTolerance\FT_Policy_i.cpp
# End Source File
# Begin Source File

SOURCE=.\FaultTolerance\FT_PolicyFactory.cpp
# End Source File
# Begin Source File

SOURCE=.\FaultTolerance\FT_Service_Activate.cpp
# End Source File
# Begin Source File

SOURCE=.\FaultTolerance\FT_Service_Callbacks.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\FT_CORBA_ORBC.h
# End Source File
# Begin Source File

SOURCE=.\FT_CORBA_ORBS.h
# End Source File
# Begin Source File

SOURCE=.\FaultTolerance\FT_Endpoint_Selector_Factory.h
# End Source File
# Begin Source File

SOURCE=.\FaultTolerance\FT_Invocation_Endpoint_Selectors.h
# End Source File
# Begin Source File

SOURCE=.\FaultTolerance\FT_IOGR_Property.h
# End Source File
# Begin Source File

SOURCE=.\FaultTolerance\FT_ORBInitializer.h
# End Source File
# Begin Source File

SOURCE=.\FaultTolerance\FT_Policy_i.h
# End Source File
# Begin Source File

SOURCE=.\FaultTolerance\FT_PolicyFactory.h
# End Source File
# Begin Source File

SOURCE=.\FaultTolerance\FT_Service_Activate.h
# End Source File
# Begin Source File

SOURCE=.\FaultTolerance\FT_Service_Callbacks.h
# End Source File
# Begin Source File

SOURCE=.\FaultTolerance\ftconf.h
# End Source File
# End Group
# Begin Group "Inline Files"

# PROP Default_Filter ".i"
# Begin Source File

SOURCE=.\FT_CORBA_ORBC.i
# End Source File
# Begin Source File

SOURCE=.\FT_CORBA_ORBS.i
# End Source File
# Begin Source File

SOURCE=.\FaultTolerance\FT_Invocation_Endpoint_Selectors.inl
# End Source File
# Begin Source File

SOURCE=.\FaultTolerance\FT_IOGR_Property.i
# End Source File
# Begin Source File

SOURCE=.\FaultTolerance\FT_Policy_i.i
# End Source File
# Begin Source File

SOURCE=.\FaultTolerance\FT_Service_Callbacks.i
# End Source File
# End Group
# Begin Group "IDL Files"

# PROP Default_Filter ".idl"
# Begin Source File

SOURCE=.\FT_CORBA_ORB.idl

!IF  "$(CFG)" == "FT ORB - Win32 Release"

# PROP Ignore_Default_Tool 1
USERDEP__FT_CO="..\..\..\bin\Release\tao_idl.exe"	
# Begin Custom Build - Invoking TAO_IDL Compiler on $(InputName)
InputPath=.\FT_CORBA_ORB.idl
InputName=FT_CORBA_ORB

BuildCmds= \
	..\..\..\bin\Release\tao_idl -Ge 1 -I../../ -I../ -Wb,pre_include=ace/pre.h -Wb,post_include=ace/post.h -Wb,export_macro=TAO_FT_Export -Wb,export_include=FaultTolerance\fault_tol_export.h -Wb,skel_export_include=tao/PortableServer/PolicyS.h $(InputName).idl

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

!ELSEIF  "$(CFG)" == "FT ORB - Win32 Debug"

# PROP Ignore_Default_Tool 1
USERDEP__FT_CO="..\..\..\bin\tao_idl.exe"	
# Begin Custom Build - Invoking TAO_IDL Compiler on $(InputName)
InputPath=.\FT_CORBA_ORB.idl
InputName=FT_CORBA_ORB

BuildCmds= \
	..\..\..\bin\tao_idl -Ge 1 -I../../ -I../ -Wb,pre_include=ace/pre.h -Wb,post_include=ace/post.h -Wb,export_macro=TAO_FT_Export -Wb,export_include=FaultTolerance/fault_tol_export.h -Wb,skel_export_include=tao/PortableServer/PolicyS.h $(InputName).idl

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

!ELSEIF  "$(CFG)" == "FT ORB - Win32 mfc Release"

# PROP BASE Ignore_Default_Tool 1
# PROP Ignore_Default_Tool 1
USERDEP__FT_CO="..\..\..\bin\Release\tao_idl.exe"	
# Begin Custom Build - Invoking TAO_IDL Compiler on $(InputName)
InputPath=.\FT_CORBA_ORB.idl
InputName=FT_CORBA_ORB

BuildCmds= \
	..\..\..\bin\Release\tao_idl -Ge 1 -I../../ -I../../tao -I../ -Wb,pre_include=ace/pre.h -Wb,post_include=ace/post.h -Wb,export_macro=TAO_FT_Export -Wb,export_include=FaultTolerance/fault_tol_export.h -Wb,skel_export_include=tao/PortableServer/PolicyS.h $(InputName).idl

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

!ELSEIF  "$(CFG)" == "FT ORB - Win32 MFC Debug"

# PROP BASE Ignore_Default_Tool 1
# PROP Ignore_Default_Tool 1
# Begin Custom Build - Invoking TAO_IDL Compiler on $(InputName)
InputPath=.\FT_CORBA_ORB.idl
InputName=FT_CORBA_ORB

BuildCmds= \
	..\..\..\bin\tao_idl -Ge 1 -I../../ -I../ -Wb,pre_include=ace/pre.h -Wb,post_include=ace/post.h -Wb,export_macro=TAO_FT_Export -Wb,export_include=FaultTolerance/fault_tol_export.h -Wb,skel_export_include=tao/PortableServer/PolicyS.h $(InputName).idl

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
# Begin Group "Resource Files"

# PROP Default_Filter "rc"
# End Group
# End Target
# End Project
