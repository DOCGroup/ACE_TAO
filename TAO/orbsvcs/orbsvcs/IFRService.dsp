# Microsoft Developer Studio Project File - Name="IFRService" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Dynamic-Link Library" 0x0102

CFG=IFRService - Win32 mfc Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "IFRService.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "IFRService.mak" CFG="IFRService - Win32 mfc Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "IFRService - Win32 Release" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "IFRService - Win32 Debug" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "IFRService - Win32 mfc Release" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "IFRService - Win32 mfc Debug" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "IFRService - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir ""
# PROP Intermediate_Dir "DLL\Release\IFRService"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "IFRService_EXPORTS" /YX /FD /c
# ADD CPP /nologo /MD /W3 /GX /O2 /I "../" /I "../../" /I "../../../" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "TAO_IFRSERVICE_BUILD_DLL" /FD /c
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
# ADD LINK32 TAO_TypeCodeFactory.lib TAO_IFR_Client.lib TAO_Svc_Utils.lib TAO_IORTable.lib TAO_PortableServer.lib TAO.lib ace.lib /nologo /dll /machine:I386 /out:"..\..\..\bin\TAO_IFRSERVICE.dll" /libpath:"..\..\tao\TypeCodeFactory" /libpath:"..\..\tao\IFR_Client" /libpath:"..\..\tao\IORTable" /libpath:"..\..\tao\PortableServer" /libpath:"..\..\tao" /libpath:"..\..\..\ace"

!ELSEIF  "$(CFG)" == "IFRService - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir ""
# PROP Intermediate_Dir "DLL\Debug\IFRService"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MTd /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "IFRService_EXPORTS" /YX /FD /c
# ADD CPP /nologo /MDd /W3 /Gm /GX /Zi /Od /I "../" /I "../../" /I "../../../" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "TAO_IFRSERVICE_BUILD_DLL" /D "TAO_ORBSVCS_HAS_IFRSERVICE" /FD /c
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
# ADD LINK32 TAO_TypeCodeFactoryd.lib TAO_Svc_Utilsd.lib TAO_IFR_Clientd.lib TAO_IORTabled.lib TAO_PortableServerd.lib TAOd.lib aced.lib /nologo /dll /debug /machine:I386 /out:"..\..\..\bin\TAO_IFRServiced.dll" /pdbtype:sept /libpath:"..\..\tao\TypeCodeFactory" /libpath:"..\..\tao\IFR_Client" /libpath:"..\..\tao\IORTable" /libpath:"..\..\tao\PortableServer" /libpath:"..\..\tao" /libpath:"..\..\..\ace"
# SUBTRACT LINK32 /profile

!ELSEIF  "$(CFG)" == "IFRService - Win32 mfc Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "IFRService___Win32_mfc_Release"
# PROP BASE Intermediate_Dir "IFRService___Win32_mfc_Release"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir ""
# PROP Intermediate_Dir "DLL\Release\IFRServiceMFC"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MD /W3 /GX /O2 /I "../" /I "../../" /I "../../../" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "TAO_IFRSERVICE_BUILD_DLL" /FD /c
# SUBTRACT BASE CPP /YX
# ADD CPP /nologo /MD /W3 /GX /O2 /I "../" /I "../../" /I "../../../" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "TAO_IFRSERVICE_BUILD_DLL" /D ACE_HAS_MFC=1 /FD /c
# SUBTRACT CPP /YX
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 ace.lib TAO.lib TAO_Svc_Utils.lib /nologo /dll /machine:I386 /out:"..\..\..\bin\TAO_IFRService.dll" /libpath:"..\..\tao" /libpath:"..\..\..\ace"
# ADD LINK32 acemfc.lib TAOmfc.lib TAO_Svc_Utilsmfc.lib TAO_PortableServermfc.lib /nologo /dll /machine:I386 /out:"..\..\..\bin\TAO_IFRServicemfc.dll" /libpath:"..\..\tao\PortableServer" /libpath:"..\..\tao" /libpath:"..\..\..\ace"

!ELSEIF  "$(CFG)" == "IFRService - Win32 mfc Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "IFRService___Win32_mfc_Debug"
# PROP BASE Intermediate_Dir "IFRService___Win32_mfc_Debug"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir ""
# PROP Intermediate_Dir "DLL\Debug\IFRServiceMFC"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MDd /W3 /Gm /GX /Zi /Od /I "../" /I "../../" /I "../../../" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "TAO_IFRSERVICE_BUILD_DLL" /D "TAO_ORBSVCS_HAS_IFRSERVICE" /FD /c
# SUBTRACT BASE CPP /YX
# ADD CPP /nologo /MDd /W3 /Gm /GX /Zi /Od /I "../" /I "../../" /I "../../../" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "TAO_IFRSERVICE_BUILD_DLL" /D "TAO_ORBSVCS_HAS_IFRSERVICE" /D ACE_HAS_MFC=1 /FD /c
# SUBTRACT CPP /YX
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 TAOd.lib aced.lib TAO_Svc_Utilsd.lib /nologo /dll /debug /machine:I386 /out:"..\..\..\bin\TAO_IFRServiced.dll" /pdbtype:sept /libpath:"..\..\tao" /libpath:"..\..\..\ace"
# SUBTRACT BASE LINK32 /profile
# ADD LINK32 TAOmfcd.lib acemfcd.lib TAO_Svc_Utilsmfcd.lib TAO_PortableServermfcd.lib /nologo /dll /debug /machine:I386 /out:"..\..\..\bin\TAO_IFRServicemfcd.dll" /pdbtype:sept /libpath:"..\..\tao\PortableServer" /libpath:"..\..\tao" /libpath:"..\..\..\ace"
# SUBTRACT LINK32 /profile

!ENDIF 

# Begin Target

# Name "IFRService - Win32 Release"
# Name "IFRService - Win32 Debug"
# Name "IFRService - Win32 mfc Release"
# Name "IFRService - Win32 mfc Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\IFRService\AbstractInterfaceDef_i.cpp
# End Source File
# Begin Source File

SOURCE=.\IFRService\AliasDef_i.cpp
# End Source File
# Begin Source File

SOURCE=.\IFRService\ArrayDef_i.cpp
# End Source File
# Begin Source File

SOURCE=.\IFRService\AttributeDef_i.cpp
# End Source File
# Begin Source File

SOURCE=.\IFRService\ComponentContainer_i.cpp
# End Source File
# Begin Source File

SOURCE=.\IFRService\ComponentDef_i.cpp
# End Source File
# Begin Source File

SOURCE=.\IFRService\ComponentModuleDef_i.cpp
# End Source File
# Begin Source File

SOURCE=.\IFRService\ComponentRepository_i.cpp
# End Source File
# Begin Source File

SOURCE=.\IFRService\ConstantDef_i.cpp
# End Source File
# Begin Source File

SOURCE=.\IFRService\ConsumesDef_i.cpp
# End Source File
# Begin Source File

SOURCE=.\IFRService\Contained_i.cpp
# End Source File
# Begin Source File

SOURCE=.\IFRService\Container_i.cpp
# End Source File
# Begin Source File

SOURCE=.\IFRService\EmitsDef_i.cpp
# End Source File
# Begin Source File

SOURCE=.\IFRService\EnumDef_i.cpp
# End Source File
# Begin Source File

SOURCE=.\IFRService\EventDef_i.cpp
# End Source File
# Begin Source File

SOURCE=.\IFRService\EventPortDef_i.cpp
# End Source File
# Begin Source File

SOURCE=.\IFRService\ExceptionDef_i.cpp
# End Source File
# Begin Source File

SOURCE=.\IFRService\ExtAbstractInterfaceDef_i.cpp
# End Source File
# Begin Source File

SOURCE=.\IFRService\ExtAttributeDef_i.cpp
# End Source File
# Begin Source File

SOURCE=.\IFRService\ExtInterfaceDef_i.cpp
# End Source File
# Begin Source File

SOURCE=.\IFRService\ExtLocalInterfaceDef_i.cpp
# End Source File
# Begin Source File

SOURCE=.\IFRService\ExtValueDef_i.cpp
# End Source File
# Begin Source File

SOURCE=.\IFRService\FactoryDef_i.cpp
# End Source File
# Begin Source File

SOURCE=.\IFRService\FinderDef_i.cpp
# End Source File
# Begin Source File

SOURCE=.\IFRService\FixedDef_i.cpp
# End Source File
# Begin Source File

SOURCE=.\IFRService\HomeDef_i.cpp
# End Source File
# Begin Source File

SOURCE=.\IFRService\IDLType_i.cpp
# End Source File
# Begin Source File

SOURCE=.\IFRService\IFR_BaseS.cpp
# End Source File
# Begin Source File

SOURCE=.\IFRService\IFR_BasicS.cpp
# End Source File
# Begin Source File

SOURCE=.\IFRService\IFR_ComponentsS.cpp
# End Source File
# Begin Source File

SOURCE=.\IFRService\IFR_ExtendedS.cpp
# End Source File
# Begin Source File

SOURCE=.\IFRService\IFR_Service_Loader.cpp
# End Source File
# Begin Source File

SOURCE=.\IFRService\IFR_Service_Utils.cpp
# End Source File
# Begin Source File

SOURCE=.\IFRService\InterfaceAttrExtension_i.cpp
# End Source File
# Begin Source File

SOURCE=.\IFRService\InterfaceDef_i.cpp
# End Source File
# Begin Source File

SOURCE=.\IFRService\IRObject_i.cpp
# End Source File
# Begin Source File

SOURCE=.\IFRService\LocalInterfaceDef_i.cpp
# End Source File
# Begin Source File

SOURCE=.\IFRService\ModuleDef_i.cpp
# End Source File
# Begin Source File

SOURCE=.\IFRService\NativeDef_i.cpp
# End Source File
# Begin Source File

SOURCE=.\IFRService\OperationDef_i.cpp
# End Source File
# Begin Source File

SOURCE=.\IFRService\Options.cpp
# End Source File
# Begin Source File

SOURCE=.\IFRService\PrimitiveDef_i.cpp
# End Source File
# Begin Source File

SOURCE=.\IFRService\ProvidesDef_i.cpp
# End Source File
# Begin Source File

SOURCE=.\IFRService\PublishesDef_i.cpp
# End Source File
# Begin Source File

SOURCE=.\IFRService\Repository_i.cpp
# End Source File
# Begin Source File

SOURCE=.\IFRService\SequenceDef_i.cpp
# End Source File
# Begin Source File

SOURCE=.\IFRService\StringDef_i.cpp
# End Source File
# Begin Source File

SOURCE=.\IFRService\StructDef_i.cpp
# End Source File
# Begin Source File

SOURCE=.\IFRService\tmplinst.cpp
# End Source File
# Begin Source File

SOURCE=.\IFRService\TypedefDef_i.cpp
# End Source File
# Begin Source File

SOURCE=.\IFRService\UnionDef_i.cpp
# End Source File
# Begin Source File

SOURCE=.\IFRService\UsesDef_i.cpp
# End Source File
# Begin Source File

SOURCE=.\IFRService\ValueBoxDef_i.cpp
# End Source File
# Begin Source File

SOURCE=.\IFRService\ValueDef_i.cpp
# End Source File
# Begin Source File

SOURCE=.\IFRService\ValueMemberDef_i.cpp
# End Source File
# Begin Source File

SOURCE=.\IFRService\WstringDef_i.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\IFRService\AbstractInterfaceDef_i.h
# End Source File
# Begin Source File

SOURCE=.\IFRService\AliasDef_i.h
# End Source File
# Begin Source File

SOURCE=.\IFRService\ArrayDef_i.h
# End Source File
# Begin Source File

SOURCE=.\IFRService\AttributeDef_i.h
# End Source File
# Begin Source File

SOURCE=.\IFRService\ComponentContainer_i.h
# End Source File
# Begin Source File

SOURCE=.\IFRService\ComponentDef_i.h
# End Source File
# Begin Source File

SOURCE=.\IFRService\ComponentModuleDef_i.h
# End Source File
# Begin Source File

SOURCE=.\IFRService\ComponentRepository_i.h
# End Source File
# Begin Source File

SOURCE=.\IFRService\concrete_classes.h
# End Source File
# Begin Source File

SOURCE=.\IFRService\ConstantDef_i.h
# End Source File
# Begin Source File

SOURCE=.\IFRService\ConsumesDef_i.h
# End Source File
# Begin Source File

SOURCE=.\IFRService\Contained_i.h
# End Source File
# Begin Source File

SOURCE=.\IFRService\Container_i.h
# End Source File
# Begin Source File

SOURCE=.\IFRService\EmitsDef_i.h
# End Source File
# Begin Source File

SOURCE=.\IFRService\EnumDef_i.h
# End Source File
# Begin Source File

SOURCE=.\IFRService\EventDef_i.h
# End Source File
# Begin Source File

SOURCE=.\IFRService\EventPortDef_i.h
# End Source File
# Begin Source File

SOURCE=.\IFRService\ExceptionDef_i.h
# End Source File
# Begin Source File

SOURCE=.\IFRService\ExtAbstractInterfaceDef_i.h
# End Source File
# Begin Source File

SOURCE=.\IFRService\ExtAttributeDef_i.h
# End Source File
# Begin Source File

SOURCE=.\IFRService\ExtInterfaceDef_i.h
# End Source File
# Begin Source File

SOURCE=.\IFRService\ExtLocalInterfaceDef_i.h
# End Source File
# Begin Source File

SOURCE=.\IFRService\ExtValueDef_i.h
# End Source File
# Begin Source File

SOURCE=.\IFRService\FactoryDef_i.h
# End Source File
# Begin Source File

SOURCE=.\IFRService\FinderDef_i.h
# End Source File
# Begin Source File

SOURCE=.\IFRService\FixedDef_i.h
# End Source File
# Begin Source File

SOURCE=.\IFRService\HomeDef_i.h
# End Source File
# Begin Source File

SOURCE=.\IFRService\IDLType_i.h
# End Source File
# Begin Source File

SOURCE=.\IFRService\IFR_BaseS.h
# End Source File
# Begin Source File

SOURCE=.\IFRService\IFR_BaseS_T.h
# End Source File
# Begin Source File

SOURCE=.\IFRService\IFR_BasicS.h
# End Source File
# Begin Source File

SOURCE=.\IFRService\IFR_BasicS_T.h
# End Source File
# Begin Source File

SOURCE=.\IFRService\IFR_ComponentsS.h
# End Source File
# Begin Source File

SOURCE=.\IFRService\IFR_ComponentsS_T.h
# End Source File
# Begin Source File

SOURCE=.\IFRService\IFR_ExtendedS.h
# End Source File
# Begin Source File

SOURCE=.\IFRService\IFR_ExtendedS_T.h
# End Source File
# Begin Source File

SOURCE=.\IFRService\IFR_macro.h
# End Source File
# Begin Source File

SOURCE=.\IFRService\ifr_service_export.h
# End Source File
# Begin Source File

SOURCE=.\IFRService\IFR_Service_Loader.h
# End Source File
# Begin Source File

SOURCE=.\IFRService\IFR_Service_Utils.h
# End Source File
# Begin Source File

SOURCE=.\IFRService\IFR_Service_Utils_T.h
# End Source File
# Begin Source File

SOURCE=.\IFRService\InterfaceAttrExtension_i.h
# End Source File
# Begin Source File

SOURCE=.\IFRService\InterfaceDef_i.h
# End Source File
# Begin Source File

SOURCE=.\IFRService\IRObject_i.h
# End Source File
# Begin Source File

SOURCE=.\IFRService\LocalInterfaceDef_i.h
# End Source File
# Begin Source File

SOURCE=.\IFRService\ModuleDef_i.h
# End Source File
# Begin Source File

SOURCE=.\IFRService\NativeDef_i.h
# End Source File
# Begin Source File

SOURCE=.\IFRService\OperationDef_i.h
# End Source File
# Begin Source File

SOURCE=.\IFRService\Options.h
# End Source File
# Begin Source File

SOURCE=.\IFRService\PrimitiveDef_i.h
# End Source File
# Begin Source File

SOURCE=.\IFRService\ProvidesDef_i.h
# End Source File
# Begin Source File

SOURCE=.\IFRService\PublishesDef_i.h
# End Source File
# Begin Source File

SOURCE=.\IFRService\Repository_i.h
# End Source File
# Begin Source File

SOURCE=.\IFRService\SequenceDef_i.h
# End Source File
# Begin Source File

SOURCE=.\IFRService\StringDef_i.h
# End Source File
# Begin Source File

SOURCE=.\IFRService\StructDef_i.h
# End Source File
# Begin Source File

SOURCE=.\IFRService\TypedefDef_i.h
# End Source File
# Begin Source File

SOURCE=.\IFRService\UnionDef_i.h
# End Source File
# Begin Source File

SOURCE=.\IFRService\UsesDef_i.h
# End Source File
# Begin Source File

SOURCE=.\IFRService\ValueBoxDef_i.h
# End Source File
# Begin Source File

SOURCE=.\IFRService\ValueDef_i.h
# End Source File
# Begin Source File

SOURCE=.\IFRService\ValueMemberDef_i.h
# End Source File
# Begin Source File

SOURCE=.\IFRService\WstringDef_i.h
# End Source File
# End Group
# Begin Group "IDL Files"

# PROP Default_Filter ".idl"
# End Group
# Begin Group "Inline Files"

# PROP Default_Filter ".i"
# Begin Source File

SOURCE=.\IFRService\IFR_BaseS.i
# End Source File
# Begin Source File

SOURCE=.\IFRService\IFR_BaseS_T.i
# End Source File
# Begin Source File

SOURCE=.\IFRService\IFR_BasicS.i
# End Source File
# Begin Source File

SOURCE=.\IFRService\IFR_BasicS_T.i
# End Source File
# Begin Source File

SOURCE=.\IFRService\IFR_ComponentsS.i
# End Source File
# Begin Source File

SOURCE=.\IFRService\IFR_ComponentsS_T.i
# End Source File
# Begin Source File

SOURCE=.\IFRService\IFR_ExtendedS.i
# End Source File
# Begin Source File

SOURCE=.\IFRService\IFR_ExtendedS_T.i
# End Source File
# End Group
# Begin Group "Template Files"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\IFRService\IFR_BaseS_T.cpp

!IF  "$(CFG)" == "IFRService - Win32 Release"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "IFRService - Win32 Debug"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "IFRService - Win32 mfc Release"

!ELSEIF  "$(CFG)" == "IFRService - Win32 mfc Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\IFRService\IFR_BasicS_T.cpp

!IF  "$(CFG)" == "IFRService - Win32 Release"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "IFRService - Win32 Debug"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "IFRService - Win32 mfc Release"

!ELSEIF  "$(CFG)" == "IFRService - Win32 mfc Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\IFRService\IFR_ComponentsS_T.cpp

!IF  "$(CFG)" == "IFRService - Win32 Release"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "IFRService - Win32 Debug"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "IFRService - Win32 mfc Release"

!ELSEIF  "$(CFG)" == "IFRService - Win32 mfc Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\IFRService\IFR_ExtendedS_T.cpp

!IF  "$(CFG)" == "IFRService - Win32 Release"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "IFRService - Win32 Debug"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "IFRService - Win32 mfc Release"

!ELSEIF  "$(CFG)" == "IFRService - Win32 mfc Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\IFRService\IFR_Service_Utils_T.cpp

!IF  "$(CFG)" == "IFRService - Win32 Release"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "IFRService - Win32 Debug"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "IFRService - Win32 mfc Release"

!ELSEIF  "$(CFG)" == "IFRService - Win32 mfc Debug"

!ENDIF 

# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "rc"
# End Group
# End Target
# End Project
