# Microsoft Developer Studio Project File - Name="PortableGroup" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Dynamic-Link Library" 0x0102

CFG=PortableGroup - Win32 MFC Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "PortableGroup.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "PortableGroup.mak" CFG="PortableGroup - Win32 MFC Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "PortableGroup - Win32 Release" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "PortableGroup - Win32 Debug" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "PortableGroup - Win32 MFC Debug" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "PortableGroup - Win32 MFC Release" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "PortableGroup - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir ""
# PROP Intermediate_Dir "DLL\Release\PortableGroup"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "PortableGroup_EXPORTS" /YX /FD /c
# ADD CPP /nologo /MD /W3 /GX /O2 /I "../" /I "../../" /I "../../../" /D "NDEBUG" /D "TAO_PORTABLEGROUP_BUILD_DLL" /D "WIN32" /D "_WINDOWS" /FD /c
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
# ADD LINK32 TAO_CosNaming.lib TAO_Messaging.lib TAO_PortableServer.lib TAO.lib ace.lib /nologo /dll /machine:I386 /out:"..\..\..\bin\TAO_PortableGroup.dll" /libpath:"..\..\orbsvcs\orbsvcs" /libpath:"..\..\tao\Messaging" /libpath:"..\..\tao\PortableServer" /libpath:"..\..\tao" /libpath:"..\..\..\ace"

!ELSEIF  "$(CFG)" == "PortableGroup - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir ""
# PROP Intermediate_Dir "DLL\Debug\PortableGroup"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MTd /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "PortableGroup_EXPORTS" /YX /FD /c
# ADD CPP /nologo /MDd /W3 /Gm /GR- /GX /Zi /Od /I ".." /I "../.." /I "../../.." /D "_DEBUG" /D "TAO_PORTABLEGROUP_BUILD_DLL" /D "WIN32" /D "_WINDOWS" /FD /c
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
# ADD LINK32 TAO_Messagingd.lib TAO_PortableServerd.lib TAOd.lib aced.lib TAO_CosNamingd.lib /nologo /dll /debug /machine:I386 /out:"..\..\..\bin\TAO_PortableGroupd.dll" /pdbtype:sept /libpath:"..\..\orbsvcs\orbsvcs" /libpath:"..\..\tao\Messaging" /libpath:"..\..\tao\PortableServer" /libpath:"..\..\tao" /libpath:"..\..\..\ace"

!ELSEIF  "$(CFG)" == "PortableGroup - Win32 MFC Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "PortableGroup___Win32_MFC_Debug"
# PROP BASE Intermediate_Dir "PortableGroup___Win32_MFC_Debug"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir ""
# PROP Intermediate_Dir "DLL\Debug\PortableGroupMFC"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MDd /W3 /Gm /GX /Zi /Od /I "../" /I "../../" /I "../../../" /D "_DEBUG" /D "WIN32" /D "_WINDOWS" /D "TAO_PORTABLEGROUP_BUILD_DLL" /FD /c
# SUBTRACT BASE CPP /YX
# ADD CPP /nologo /MDd /W3 /Gm /GX /Zi /Od /I "../" /I "../../" /I "../../../" /D "_DEBUG" /D ACE_HAS_MFC=1 /D "TAO_PORTABLEGROUP_BUILD_DLL" /D "WIN32" /D "_WINDOWS" /FD /c
# SUBTRACT CPP /YX
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 TAOd.lib aced.lib TAO_Svc_Utilsd.lib TAO_RTEventd.lib TAO_RTSchedd.lib /nologo /dll /debug /machine:I386 /out:"..\..\..\bin\TAO_PortableGroupd.dll" /pdbtype:sept /libpath:"..\..\tao" /libpath:"..\..\..\ace"
# ADD LINK32 TAO_Messagingmfcd.lib TAO_PortableServermfcd.lib TAOmfcd.lib acemfcd.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /debug /machine:I386 /out:"..\..\..\bin\TAO_PortableGroupmfcd.dll" /pdbtype:sept /libpath:"..\..\tao\Messaging" /libpath:"..\..\tao\PortableServer" /libpath:"..\..\tao" /libpath:"..\..\..\ace"

!ELSEIF  "$(CFG)" == "PortableGroup - Win32 MFC Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "PortableGroup___Win32_MFC_Release"
# PROP BASE Intermediate_Dir "PortableGroup___Win32_MFC_Release"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir ""
# PROP Intermediate_Dir "DLL\Release\PortableGroupMFC"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MD /W3 /GX /O2 /I "../" /I "../../" /I "../../../" /D "NDEBUG" /D "WIN32" /D "_WINDOWS" /D "TAO_PORTABLEGROUP_BUILD_DLL" /FD /c
# SUBTRACT BASE CPP /YX
# ADD CPP /nologo /MD /W3 /GX /O2 /I "../" /I "../../" /I "../../../" /D "NDEBUG" /D ACE_HAS_MFC=1 /D "TAO_PORTABLEGROUP_BUILD_DLL" /D "WIN32" /D "_WINDOWS" /FD /c
# SUBTRACT CPP /YX
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 ace.lib TAO.lib TAO_Svc_Utils.lib TAO_RTEvent.lib TAO_RTSched.lib /nologo /dll /machine:I386 /out:"..\..\..\bin\TAO_PortableGroup.dll" /libpath:"..\..\tao" /libpath:"..\..\..\ace"
# ADD LINK32 TAO_Messagingmfc.lib TAO_PortableServermfc.lib TAOmfc.lib acemfc.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /machine:I386 /out:"..\..\..\bin\TAO_PortableGroupmfc.dll" /libpath:"..\..\tao\Messaging" /libpath:"..\..\tao\PortableServer" /libpath:"..\..\tao" /libpath:"..\..\..\ace"

!ENDIF 

# Begin Target

# Name "PortableGroup - Win32 Release"
# Name "PortableGroup - Win32 Debug"
# Name "PortableGroup - Win32 MFC Debug"
# Name "PortableGroup - Win32 MFC Release"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\miopC.cpp

!IF  "$(CFG)" == "PortableGroup - Win32 Release"

# ADD CPP /GR

!ELSEIF  "$(CFG)" == "PortableGroup - Win32 Debug"

# ADD CPP /GR /I "../../tao"

!ELSEIF  "$(CFG)" == "PortableGroup - Win32 MFC Debug"

# ADD CPP /GR

!ELSEIF  "$(CFG)" == "PortableGroup - Win32 MFC Release"

# ADD CPP /GR

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\PortableGroup\PG_Default_Property_Validator.cpp
# End Source File
# Begin Source File

SOURCE=.\PortableGroup\PG_Factory_Map.cpp
# End Source File
# Begin Source File

SOURCE=.\PortableGroup\PG_Factory_Set.cpp
# End Source File
# Begin Source File

SOURCE=.\PortableGroup\PG_GenericFactory.cpp
# End Source File
# Begin Source File

SOURCE=.\PortableGroup\PG_Location_Equal_To.cpp
# End Source File
# Begin Source File

SOURCE=.\PortableGroup\PG_Location_Hash.cpp
# End Source File
# Begin Source File

SOURCE=.\PortableGroup\PG_Location_Map.cpp
# End Source File
# Begin Source File

SOURCE=.\PortableGroup\PG_MemberInfo.cpp
# End Source File
# Begin Source File

SOURCE=.\PortableGroup\PG_Null_Property_Validator.cpp
# End Source File
# Begin Source File

SOURCE=.\PortableGroup\PG_ObjectGroup_Map.cpp
# End Source File
# Begin Source File

SOURCE=.\PortableGroup\PG_ObjectGroupManager.cpp
# End Source File
# Begin Source File

SOURCE=.\PortableGroup\PG_Operators.cpp
# End Source File
# Begin Source File

SOURCE=.\PortableGroup\PG_Property_Utils.cpp
# End Source File
# Begin Source File

SOURCE=.\PortableGroup\PG_PropertyManager.cpp
# End Source File
# Begin Source File

SOURCE=.\PortableGroup\POA_Hooks.cpp

!IF  "$(CFG)" == "PortableGroup - Win32 Release"

!ELSEIF  "$(CFG)" == "PortableGroup - Win32 Debug"

# ADD CPP /I "../../tao"

!ELSEIF  "$(CFG)" == "PortableGroup - Win32 MFC Debug"

!ELSEIF  "$(CFG)" == "PortableGroup - Win32 MFC Release"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\PortableGroup\Portable_Group_Map.cpp

!IF  "$(CFG)" == "PortableGroup - Win32 Release"

!ELSEIF  "$(CFG)" == "PortableGroup - Win32 Debug"

# ADD CPP /I "../../tao"

!ELSEIF  "$(CFG)" == "PortableGroup - Win32 MFC Debug"

!ELSEIF  "$(CFG)" == "PortableGroup - Win32 MFC Release"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\PortableGroup\PortableGroup_Acceptor_Registry.cpp

!IF  "$(CFG)" == "PortableGroup - Win32 Release"

!ELSEIF  "$(CFG)" == "PortableGroup - Win32 Debug"

# ADD CPP /I "../../tao"

!ELSEIF  "$(CFG)" == "PortableGroup - Win32 MFC Debug"

!ELSEIF  "$(CFG)" == "PortableGroup - Win32 MFC Release"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\PortableGroup\PortableGroup_Loader.cpp

!IF  "$(CFG)" == "PortableGroup - Win32 Release"

!ELSEIF  "$(CFG)" == "PortableGroup - Win32 Debug"

# ADD CPP /I "../../tao"

!ELSEIF  "$(CFG)" == "PortableGroup - Win32 MFC Debug"

!ELSEIF  "$(CFG)" == "PortableGroup - Win32 MFC Release"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\PortableGroup\PortableGroup_ORBInitializer.cpp
# End Source File
# Begin Source File

SOURCE=.\PortableGroup\PortableGroup_Request_Dispatcher.cpp

!IF  "$(CFG)" == "PortableGroup - Win32 Release"

!ELSEIF  "$(CFG)" == "PortableGroup - Win32 Debug"

# ADD CPP /I "../../tao"

!ELSEIF  "$(CFG)" == "PortableGroup - Win32 MFC Debug"

!ELSEIF  "$(CFG)" == "PortableGroup - Win32 MFC Release"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\PortableGroupC.cpp

!IF  "$(CFG)" == "PortableGroup - Win32 Release"

# ADD CPP /GR

!ELSEIF  "$(CFG)" == "PortableGroup - Win32 Debug"

# ADD CPP /GR /I "../../tao"

!ELSEIF  "$(CFG)" == "PortableGroup - Win32 MFC Debug"

# ADD CPP /GR

!ELSEIF  "$(CFG)" == "PortableGroup - Win32 MFC Release"

# ADD CPP /GR

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\PortableGroupS.cpp
# End Source File
# Begin Source File

SOURCE=.\PortableGroupS_T.cpp
# End Source File
# Begin Source File

SOURCE=.\PortableGroup\UIPMC_Acceptor.cpp

!IF  "$(CFG)" == "PortableGroup - Win32 Release"

!ELSEIF  "$(CFG)" == "PortableGroup - Win32 Debug"

# ADD CPP /I "../../tao"

!ELSEIF  "$(CFG)" == "PortableGroup - Win32 MFC Debug"

!ELSEIF  "$(CFG)" == "PortableGroup - Win32 MFC Release"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\PortableGroup\UIPMC_Connection_Handler.cpp

!IF  "$(CFG)" == "PortableGroup - Win32 Release"

!ELSEIF  "$(CFG)" == "PortableGroup - Win32 Debug"

# ADD CPP /I "../../tao"

!ELSEIF  "$(CFG)" == "PortableGroup - Win32 MFC Debug"

!ELSEIF  "$(CFG)" == "PortableGroup - Win32 MFC Release"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\PortableGroup\UIPMC_Connector.cpp

!IF  "$(CFG)" == "PortableGroup - Win32 Release"

!ELSEIF  "$(CFG)" == "PortableGroup - Win32 Debug"

# ADD CPP /I "../../tao"

!ELSEIF  "$(CFG)" == "PortableGroup - Win32 MFC Debug"

!ELSEIF  "$(CFG)" == "PortableGroup - Win32 MFC Release"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\PortableGroup\UIPMC_Endpoint.cpp

!IF  "$(CFG)" == "PortableGroup - Win32 Release"

!ELSEIF  "$(CFG)" == "PortableGroup - Win32 Debug"

# ADD CPP /I "../../tao"

!ELSEIF  "$(CFG)" == "PortableGroup - Win32 MFC Debug"

!ELSEIF  "$(CFG)" == "PortableGroup - Win32 MFC Release"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\PortableGroup\UIPMC_Factory.cpp

!IF  "$(CFG)" == "PortableGroup - Win32 Release"

!ELSEIF  "$(CFG)" == "PortableGroup - Win32 Debug"

# ADD CPP /I "../../tao"

!ELSEIF  "$(CFG)" == "PortableGroup - Win32 MFC Debug"

!ELSEIF  "$(CFG)" == "PortableGroup - Win32 MFC Release"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\PortableGroup\UIPMC_Profile.cpp

!IF  "$(CFG)" == "PortableGroup - Win32 Release"

!ELSEIF  "$(CFG)" == "PortableGroup - Win32 Debug"

# ADD CPP /I "../../tao"

!ELSEIF  "$(CFG)" == "PortableGroup - Win32 MFC Debug"

!ELSEIF  "$(CFG)" == "PortableGroup - Win32 MFC Release"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\PortableGroup\UIPMC_Transport.cpp

!IF  "$(CFG)" == "PortableGroup - Win32 Release"

!ELSEIF  "$(CFG)" == "PortableGroup - Win32 Debug"

# ADD CPP /I "../../tao"

!ELSEIF  "$(CFG)" == "PortableGroup - Win32 MFC Debug"

!ELSEIF  "$(CFG)" == "PortableGroup - Win32 MFC Release"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\PortableGroup\UIPMC_Wait_Never.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm"
# Begin Source File

SOURCE=.\miopC.h
# End Source File
# Begin Source File

SOURCE=.\PortableGroup\miopconf.h
# End Source File
# Begin Source File

SOURCE=.\PortableGroup\PG_conf.h
# End Source File
# Begin Source File

SOURCE=.\PortableGroup\PG_Default_Property_Validator.h
# End Source File
# Begin Source File

SOURCE=.\PortableGroup\PG_Factory_Map.h
# End Source File
# Begin Source File

SOURCE=.\PortableGroup\PG_Factory_Set.h
# End Source File
# Begin Source File

SOURCE=.\PortableGroup\PG_GenericFactory.h
# End Source File
# Begin Source File

SOURCE=.\PortableGroup\PG_Location_Equal_To.h
# End Source File
# Begin Source File

SOURCE=.\PortableGroup\PG_Location_Hash.h
# End Source File
# Begin Source File

SOURCE=.\PortableGroup\PG_Location_Map.h
# End Source File
# Begin Source File

SOURCE=.\PortableGroup\PG_MemberInfo.h
# End Source File
# Begin Source File

SOURCE=.\PortableGroup\PG_Null_Property_Validator.h
# End Source File
# Begin Source File

SOURCE=.\PortableGroup\PG_ObjectGroup_Map.h
# End Source File
# Begin Source File

SOURCE=.\PortableGroup\PG_ObjectGroupManager.h
# End Source File
# Begin Source File

SOURCE=.\PortableGroup\PG_Operators.h
# End Source File
# Begin Source File

SOURCE=.\PortableGroup\PG_Property_Utils.h
# End Source File
# Begin Source File

SOURCE=.\PortableGroup\PG_PropertyManager.h
# End Source File
# Begin Source File

SOURCE=.\PortableGroup\POA_Hooks.h
# End Source File
# Begin Source File

SOURCE=.\PortableGroup\Portable_Group_Map.h
# End Source File
# Begin Source File

SOURCE=.\PortableGroup\PortableGroup_Acceptor_Registry.h
# End Source File
# Begin Source File

SOURCE=.\PortableGroup\portablegroup_export.h
# End Source File
# Begin Source File

SOURCE=.\PortableGroup\PortableGroup_Loader.h
# End Source File
# Begin Source File

SOURCE=.\PortableGroup\PortableGroup_ORBInitializer.h
# End Source File
# Begin Source File

SOURCE=.\PortableGroup\PortableGroup_Request_Dispatcher.h
# End Source File
# Begin Source File

SOURCE=.\PortableGroupC.h
# End Source File
# Begin Source File

SOURCE=.\PortableGroupS.h
# End Source File
# Begin Source File

SOURCE=.\PortableGroupS_T.h
# End Source File
# Begin Source File

SOURCE=.\PortableGroup\UIPMC_Acceptor.h
# End Source File
# Begin Source File

SOURCE=.\PortableGroup\UIPMC_Connection_Handler.h
# End Source File
# Begin Source File

SOURCE=.\PortableGroup\UIPMC_Connector.h
# End Source File
# Begin Source File

SOURCE=.\PortableGroup\UIPMC_Endpoint.h
# End Source File
# Begin Source File

SOURCE=.\PortableGroup\UIPMC_Factory.h
# End Source File
# Begin Source File

SOURCE=.\PortableGroup\UIPMC_Profile.h
# End Source File
# Begin Source File

SOURCE=.\PortableGroup\UIPMC_Transport.h
# End Source File
# Begin Source File

SOURCE=.\PortableGroup\UIPMC_Wait_Never.h
# End Source File
# End Group
# Begin Group "Inline Files"

# PROP Default_Filter "i;inl"
# Begin Source File

SOURCE=.\miopC.i
# End Source File
# Begin Source File

SOURCE=.\PortableGroup\PG_Location_Equal_To.inl
# End Source File
# Begin Source File

SOURCE=.\PortableGroup\PG_Location_Hash.inl
# End Source File
# Begin Source File

SOURCE=.\PortableGroupC.i
# End Source File
# Begin Source File

SOURCE=.\PortableGroupS.i
# End Source File
# Begin Source File

SOURCE=.\PortableGroupS_T.i
# End Source File
# Begin Source File

SOURCE=.\PortableGroup\UIPMC_Acceptor.i
# End Source File
# Begin Source File

SOURCE=.\PortableGroup\UIPMC_Connection_Handler.i
# End Source File
# Begin Source File

SOURCE=.\PortableGroup\UIPMC_Endpoint.i
# End Source File
# Begin Source File

SOURCE=.\PortableGroup\UIPMC_Profile.i
# End Source File
# Begin Source File

SOURCE=.\PortableGroup\UIPMC_Transport.i
# End Source File
# End Group
# Begin Group "IDL Files"

# PROP Default_Filter ".idl"
# Begin Source File

SOURCE=.\miop.idl

!IF  "$(CFG)" == "PortableGroup - Win32 Release"

# PROP Ignore_Default_Tool 1
USERDEP__MIOP_="..\..\..\bin\Release\tao_idl.exe "	
# Begin Custom Build - Invoking TAO_IDL Compiler on $(InputName)
InputPath=.\miop.idl
InputName=miop

BuildCmds= \
	..\..\..\bin\Release\tao_idl -Ge 1 -I../.. -Wb,pre_include=ace/pre.h -Wb,post_include=ace/post.h -Wb,export_macro=TAO_PortableGroup_Export -Wb,export_include=PortableGroup/portablegroup_export.h $(InputName).idl

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

!ELSEIF  "$(CFG)" == "PortableGroup - Win32 Debug"

# PROP Ignore_Default_Tool 1
USERDEP__MIOP_="..\..\..\bin\tao_idl.exe"	
# Begin Custom Build - Invoking TAO_IDL Compiler on $(InputName)
InputPath=.\miop.idl
InputName=miop

BuildCmds= \
	..\..\..\bin\tao_idl -Ge 1 -I../.. -Wb,pre_include=ace/pre.h -Wb,post_include=ace/post.h -Wb,export_macro=TAO_PortableGroup_Export -Wb,export_include=PortableGroup/portablegroup_export.h $(InputName).idl

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

!ELSEIF  "$(CFG)" == "PortableGroup - Win32 MFC Debug"

# PROP Ignore_Default_Tool 1
# Begin Custom Build - Invoking TAO_IDL Compiler on $(InputName)
InputPath=.\miop.idl
InputName=miop

BuildCmds= \
	..\..\..\bin\tao_idl -Ge 1 -I../.. -Wb,pre_include=ace/pre.h -Wb,post_include=ace/post.h -Wb,export_macro=TAO_PortableGroup_Export -Wb,export_include=PortableGroup/portablegroup_export.h $(InputName).idl

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

!ELSEIF  "$(CFG)" == "PortableGroup - Win32 MFC Release"

# PROP Ignore_Default_Tool 1
# Begin Custom Build - Invoking TAO_IDL Compiler on $(InputName)
InputPath=.\miop.idl
InputName=miop

BuildCmds= \
	..\..\..\bin\Release\tao_idl -Ge 1 -I../.. -Wb,pre_include=ace/pre.h -Wb,post_include=ace/post.h -Wb,export_macro=TAO_PortableGroup_Export -Wb,export_include=PortableGroup/portablegroup_export.h $(InputName).idl

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
# Begin Source File

SOURCE=.\PortableGroup.idl

!IF  "$(CFG)" == "PortableGroup - Win32 Release"

# PROP Ignore_Default_Tool 1
USERDEP__PORTA="..\..\..\bin\Release\tao_idl.exe "	
# Begin Custom Build - Invoking TAO_IDL Compiler on $(InputName)
InputPath=.\PortableGroup.idl
InputName=PortableGroup

BuildCmds= \
	..\..\..\bin\Release\tao_idl -Ge 1 -GC -I../.. -Wb,pre_include=ace/pre.h -Wb,post_include=ace/post.h -Wb,export_macro=TAO_PortableGroup_Export -Wb,export_include=PortableGroup/portablegroup_export.h $(InputName).idl

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

!ELSEIF  "$(CFG)" == "PortableGroup - Win32 Debug"

# PROP Ignore_Default_Tool 1
USERDEP__PORTA="..\..\..\bin\tao_idl.exe"	
# Begin Custom Build - Invoking TAO_IDL Compiler on $(InputName)
InputPath=.\PortableGroup.idl
InputName=PortableGroup

BuildCmds= \
	..\..\..\bin\tao_idl -Ge 1 -GC -I../.. -Wb,pre_include=ace/pre.h -Wb,post_include=ace/post.h -Wb,export_macro=TAO_PortableGroup_Export -Wb,export_include=PortableGroup/portablegroup_export.h $(InputName).idl

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

!ELSEIF  "$(CFG)" == "PortableGroup - Win32 MFC Debug"

# PROP Ignore_Default_Tool 1
# Begin Custom Build - Invoking TAO_IDL Compiler on $(InputName)
InputPath=.\PortableGroup.idl
InputName=PortableGroup

BuildCmds= \
	..\..\..\bin\tao_idl -Ge 1 -GC -I../.. -Wb,pre_include=ace/pre.h -Wb,post_include=ace/post.h -Wb,export_macro=TAO_PortableGroup_Export -Wb,export_include=PortableGroup/portablegroup_export.h $(InputName).idl

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

!ELSEIF  "$(CFG)" == "PortableGroup - Win32 MFC Release"

# PROP Ignore_Default_Tool 1
# Begin Custom Build - Invoking TAO_IDL Compiler on $(InputName)
InputPath=.\PortableGroup.idl
InputName=PortableGroup

BuildCmds= \
	..\..\..\bin\Release\tao_idl -Ge 1 -GC -I../.. -Wb,pre_include=ace/pre.h -Wb,post_include=ace/post.h -Wb,export_macro=TAO_PortableGroup_Export -Wb,export_include=PortableGroup/portablegroup_export.h $(InputName).idl

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
