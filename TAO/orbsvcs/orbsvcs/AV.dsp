# Microsoft Developer Studio Project File - Name="AV" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Dynamic-Link Library" 0x0102

CFG=AV - Win32 MFC Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "AV.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "AV.mak" CFG="AV - Win32 MFC Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "AV - Win32 Release" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "AV - Win32 Debug" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "AV - Win32 MFC Debug" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "AV - Win32 MFC Release" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "AV - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir ""
# PROP Intermediate_Dir "DLL\Release\AV"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "AV_EXPORTS" /YX /FD /c
# ADD CPP /nologo /MD /W3 /GX /O2 /I "../" /I "../../" /I "../../../" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "TAO_AV_BUILD_DLL" /D "TAO_ORBSVCS_HAS_Trader" /FD /c
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
# ADD LINK32 TAO_CosNaming.lib TAO_CosTrading.lib TAO_CosProperty.lib ace.lib ACE_QoS.lib TAO.lib TAO_PortableServer.lib /nologo /dll /machine:I386 /out:"..\..\..\bin\TAO_AV.dll" /libpath:"..\..\tao\PortableServer" /libpath:"..\..\tao" /libpath:"..\..\..\ace"

!ELSEIF  "$(CFG)" == "AV - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir ""
# PROP Intermediate_Dir "DLL\Debug\AV"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MTd /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "AV_EXPORTS" /YX /FD /c
# ADD CPP /nologo /MDd /W3 /Gm /GX /Zi /Od /I "../" /I "../../" /I "../../../" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "TAO_AV_BUILD_DLL" /D "TAO_ORBSVCS_HAS_Trader" /FD /c
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
# ADD LINK32 TAO_CosNamingd.lib TAO_CosTradingd.lib TAO_CosPropertyd.lib TAOd.lib aced.lib TAO_PortableServerd.lib ACE_QoSd.lib /nologo /dll /debug /machine:I386 /out:"..\..\..\bin\TAO_AVd.dll" /pdbtype:sept /libpath:"..\..\tao\PortableServer" /libpath:"..\..\tao" /libpath:"..\..\..\ace" /libpath:"..\..\..\ace\QoS"

!ELSEIF  "$(CFG)" == "AV - Win32 MFC Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "AV___Win32_MFC_Debug"
# PROP BASE Intermediate_Dir "AV___Win32_MFC_Debug"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir ""
# PROP Intermediate_Dir "DLL\Debug\AVMFC"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MDd /W3 /Gm /GX /Zi /Od /I "../" /I "../../" /I "../../../" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "TAO_AV_BUILD_DLL" /D "TAO_ORBSVCS_HAS_Trader" /FD /c
# SUBTRACT BASE CPP /YX
# ADD CPP /nologo /MDd /W3 /Gm /GX /Zi /Od /I "../" /I "../../" /I "../../../" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "TAO_AV_BUILD_DLL" /D "TAO_ORBSVCS_HAS_Trader" /D ACE_HAS_MFC=1 /FD /c
# SUBTRACT CPP /YX
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 TAOd.lib aced.lib TAO_CosNamingd.lib TAO_CosTradingd.lib TAO_CosPropertyd.lib /nologo /dll /debug /machine:I386 /out:"..\..\..\bin\TAO_AVd.dll" /pdbtype:sept /libpath:"..\..\tao" /libpath:"..\..\..\ace"
# ADD LINK32 TAOmfcd.lib acemfcd.lib ACE_QoSmfcd.lib TAO_CosNamingmfcd.lib TAO_CosTradingmfcd.lib TAO_CosPropertymfcd.lib TAO_PortableServermfcd.lib /nologo /dll /debug /machine:I386 /out:"..\..\..\bin\TAO_AVmfcd.dll" /pdbtype:sept /libpath:"..\..\tao\PortableServer" /libpath:"..\..\tao" /libpath:"..\..\..\ace"

!ELSEIF  "$(CFG)" == "AV - Win32 MFC Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "AV___Win32_MFC_Release"
# PROP BASE Intermediate_Dir "AV___Win32_MFC_Release"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir ""
# PROP Intermediate_Dir "DLL\Release\AVMFC"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MD /W3 /GX /O2 /I "../" /I "../../" /I "../../../" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "TAO_AV_BUILD_DLL" /D "TAO_ORBSVCS_HAS_Trader" /FD /c
# SUBTRACT BASE CPP /YX
# ADD CPP /nologo /MD /W3 /GX /O2 /I "../" /I "../../" /I "../../../" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "TAO_AV_BUILD_DLL" /D "TAO_ORBSVCS_HAS_Trader" /D ACE_HAS_MFC=1 /FD /c
# SUBTRACT CPP /YX
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 ace.lib TAO.lib TAO_CosNaming.lib TAO_CosTrading.lib TAO_CosProperty.lib /nologo /dll /machine:I386 /out:"..\..\..\bin\TAO_AV.dll" /libpath:"..\..\tao" /libpath:"..\..\..\ace"
# ADD LINK32 acemfc.lib ACE_QoSmfc.lib TAOmfc.lib TAO_CosNamingmfc.lib TAO_CosTradingmfc.lib TAO_CosPropertymfc.lib TAO_PortableServermfc.lib /nologo /dll /machine:I386 /out:"..\..\..\bin\TAO_AVmfc.dll" /libpath:"..\..\tao\PortableServer" /libpath:"..\..\tao" /libpath:"..\..\..\ace"

!ENDIF 

# Begin Target

# Name "AV - Win32 Release"
# Name "AV - Win32 Debug"
# Name "AV - Win32 MFC Debug"
# Name "AV - Win32 MFC Release"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\AV\AV_Core.cpp
# End Source File
# Begin Source File

SOURCE=.\AV\AVStreams_i.cpp
# End Source File
# Begin Source File

SOURCE=.\AVStreamsC.cpp
# End Source File
# Begin Source File

SOURCE=.\AVStreamsS.cpp
# End Source File
# Begin Source File

SOURCE=.\AV\default_resource.cpp
# End Source File
# Begin Source File

SOURCE=.\AV\Endpoint_Strategy.cpp
# End Source File
# Begin Source File

SOURCE=.\AV\Fill_ACE_QoS.cpp
# End Source File
# Begin Source File

SOURCE=.\AV\FlowSpec_Entry.cpp
# End Source File
# Begin Source File

SOURCE=.\AV\MCast.cpp
# End Source File
# Begin Source File

SOURCE=.\AV\md5c.c
# End Source File
# Begin Source File

SOURCE=".\AV\media-timer.cpp"
# End Source File
# Begin Source File

SOURCE=.\AV\Nil.cpp
# End Source File
# Begin Source File

SOURCE=.\Null_MediaCtrlC.cpp
# End Source File
# Begin Source File

SOURCE=.\Null_MediaCtrlS.cpp
# End Source File
# Begin Source File

SOURCE=.\AV\Policy.cpp
# End Source File
# Begin Source File

SOURCE=.\AV\Protocol_Factory.cpp
# End Source File
# Begin Source File

SOURCE=.\AV\QoS_UDP.cpp
# End Source File
# Begin Source File

SOURCE=.\AV\RTCP.cpp
# End Source File
# Begin Source File

SOURCE=.\AV\RTCP_Channel.cpp
# End Source File
# Begin Source File

SOURCE=.\AV\RTCP_Packet.cpp
# End Source File
# Begin Source File

SOURCE=.\AV\RTP.cpp
# End Source File
# Begin Source File

SOURCE=.\AV\sfp.cpp
# End Source File
# Begin Source File

SOURCE=.\sfpC.cpp
# End Source File
# Begin Source File

SOURCE=.\sfpS.cpp
# End Source File
# Begin Source File

SOURCE=.\AV\TCP.cpp
# End Source File
# Begin Source File

SOURCE=.\AV\Transport.cpp
# End Source File
# Begin Source File

SOURCE=.\AV\UDP.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\AV\AV_Core.h
# End Source File
# Begin Source File

SOURCE=.\AV\AV_export.h
# End Source File
# Begin Source File

SOURCE=.\AV\AVStreams_i.h
# End Source File
# Begin Source File

SOURCE=.\AVStreamsC.h
# End Source File
# Begin Source File

SOURCE=.\AVStreamsS.h
# End Source File
# Begin Source File

SOURCE=.\AVStreamsS_T.h
# End Source File
# Begin Source File

SOURCE=.\AV\default_resource.h
# End Source File
# Begin Source File

SOURCE=.\AV\Endpoint_Strategy.h
# End Source File
# Begin Source File

SOURCE=.\AV\Endpoint_Strategy_T.h
# End Source File
# Begin Source File

SOURCE=.\AV\Fill_ACE_QoS.h
# End Source File
# Begin Source File

SOURCE=.\AV\Flows_T.h
# End Source File
# Begin Source File

SOURCE=.\AV\FlowSpec_Entry.h
# End Source File
# Begin Source File

SOURCE=.\AV\global.h
# End Source File
# Begin Source File

SOURCE=.\AV\MCast.h
# End Source File
# Begin Source File

SOURCE=.\AV\md5.h
# End Source File
# Begin Source File

SOURCE=".\AV\media-timer.h"
# End Source File
# Begin Source File

SOURCE=.\Null_MediaCtrlC.h
# End Source File
# Begin Source File

SOURCE=.\Null_MediaCtrlS.h
# End Source File
# Begin Source File

SOURCE=.\Null_MediaCtrlS_T.h
# End Source File
# Begin Source File

SOURCE=.\AV\Policy.h
# End Source File
# Begin Source File

SOURCE=.\AV\Protocol_Factory.h
# End Source File
# Begin Source File

SOURCE=.\AV\QoS_UDP.h
# End Source File
# Begin Source File

SOURCE=.\AV\RTCP.h
# End Source File
# Begin Source File

SOURCE=.\AV\RTCP_Channel.h
# End Source File
# Begin Source File

SOURCE=.\AV\RTCP_Packet.h
# End Source File
# Begin Source File

SOURCE=.\AV\RTP.h
# End Source File
# Begin Source File

SOURCE=.\AV\sfp.h
# End Source File
# Begin Source File

SOURCE=.\sfpC.h
# End Source File
# Begin Source File

SOURCE=.\sfpS.h
# End Source File
# Begin Source File

SOURCE=.\AV\TCP.h
# End Source File
# Begin Source File

SOURCE=.\AV\Transport.h
# End Source File
# Begin Source File

SOURCE=.\AV\UDP.h
# End Source File
# End Group
# Begin Group "IDL Files"

# PROP Default_Filter ".idl"
# Begin Source File

SOURCE=.\AVStreams.idl

!IF  "$(CFG)" == "AV - Win32 Release"

# PROP Ignore_Default_Tool 1
USERDEP__AVSTR="..\..\..\bin\Release\tao_idl.exe"	
# Begin Custom Build - Invoking TAO_IDL Compiler on $(InputPath)
InputPath=.\AVStreams.idl
InputName=AVStreams

BuildCmds= \
	..\..\..\bin\Release\tao_idl -Ge 1 -I../../ -I../../tao  -Wb,pre_include=ace/pre.h   -Wb,post_include=ace/post.h   -Wb,export_macro=TAO_AV_Export  -Wb,export_include=AV/AV_export.h $(InputName).idl

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

!ELSEIF  "$(CFG)" == "AV - Win32 Debug"

# PROP Ignore_Default_Tool 1
USERDEP__AVSTR="..\..\..\bin\tao_idl.exe"	
# Begin Custom Build - Invoking TAO_IDL Compiler on $(InputPath)
InputPath=.\AVStreams.idl
InputName=AVStreams

BuildCmds= \
	..\..\..\bin\tao_idl -Ge 1 -I../../ -I../../tao -Wb,pre_include=ace/pre.h   -Wb,post_include=ace/post.h  -Wb,export_macro=TAO_AV_Export                                                   -Wb,export_include=AV/AV_export.h $(InputName).idl

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

!ELSEIF  "$(CFG)" == "AV - Win32 MFC Debug"

# PROP BASE Ignore_Default_Tool 1
# PROP Ignore_Default_Tool 1

!ELSEIF  "$(CFG)" == "AV - Win32 MFC Release"

# PROP BASE Ignore_Default_Tool 1
# PROP Ignore_Default_Tool 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Null_MediaCtrl.idl

!IF  "$(CFG)" == "AV - Win32 Release"

# PROP Ignore_Default_Tool 1
USERDEP__NULL_="..\..\..\bin\Release\tao_idl.exe"	
# Begin Custom Build - Invoking TAO_IDL Compiler on $(InputPath)
InputPath=.\Null_MediaCtrl.idl
InputName=Null_MediaCtrl

BuildCmds= \
	..\..\..\bin\Release\tao_idl -Ge 1 -I../../ -I../../tao  -Wb,pre_include=ace/pre.h   -Wb,post_include=ace/post.h   -Wb,export_macro=TAO_AV_Export  -Wb,export_include=AV/AV_export.h $(InputName).idl

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

!ELSEIF  "$(CFG)" == "AV - Win32 Debug"

# PROP Ignore_Default_Tool 1
USERDEP__NULL_="..\..\..\bin\tao_idl.exe"	
# Begin Custom Build - Invoking TAO_IDL Compiler on $(InputPath)
InputPath=.\Null_MediaCtrl.idl
InputName=Null_MediaCtrl

BuildCmds= \
	..\..\..\bin\tao_idl -Ge 1 -I../../ -I../../tao -Wb,pre_include=ace/pre.h   -Wb,post_include=ace/post.h  -Wb,export_macro=TAO_AV_Export                                                   -Wb,export_include=AV/AV_export.h $(InputName).idl

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

!ELSEIF  "$(CFG)" == "AV - Win32 MFC Debug"

# PROP BASE Ignore_Default_Tool 1
# PROP Ignore_Default_Tool 1

!ELSEIF  "$(CFG)" == "AV - Win32 MFC Release"

# PROP BASE Ignore_Default_Tool 1
# PROP Ignore_Default_Tool 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\sfp.idl

!IF  "$(CFG)" == "AV - Win32 Release"

# PROP Ignore_Default_Tool 1
USERDEP__SFP_I="..\..\..\bin\Release\tao_idl.exe"	
# Begin Custom Build - Invoking TAO_IDL Compiler on $(InputPath)
InputPath=.\sfp.idl
InputName=sfp

BuildCmds= \
	..\..\..\bin\Release\tao_idl -Ge 1 -I../../ -I../../tao  -Wb,pre_include=ace/pre.h   -Wb,post_include=ace/post.h   -Wb,export_macro=TAO_AV_Export  -Wb,export_include=AV/AV_export.h $(InputName).idl

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

!ELSEIF  "$(CFG)" == "AV - Win32 Debug"

# PROP Ignore_Default_Tool 1
USERDEP__SFP_I="..\..\..\bin\tao_idl.exe"	
# Begin Custom Build - Invoking TAO_IDL Compiler on $(InputPath)
InputPath=.\sfp.idl
InputName=sfp

BuildCmds= \
	..\..\..\bin\tao_idl -Ge 1 -I../../ -I../../tao -Wb,pre_include=ace/pre.h   -Wb,post_include=ace/post.h  -Wb,export_macro=TAO_AV_Export                                                   -Wb,export_include=AV/AV_export.h $(InputName).idl

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

!ELSEIF  "$(CFG)" == "AV - Win32 MFC Debug"

# PROP BASE Ignore_Default_Tool 1
# PROP Ignore_Default_Tool 1

!ELSEIF  "$(CFG)" == "AV - Win32 MFC Release"

# PROP BASE Ignore_Default_Tool 1
# PROP Ignore_Default_Tool 1

!ENDIF 

# End Source File
# End Group
# Begin Group "Inline Files"

# PROP Default_Filter ".i"
# Begin Source File

SOURCE=.\AV\AVStreams_i.i
# End Source File
# Begin Source File

SOURCE=.\AVStreamsC.i
# End Source File
# Begin Source File

SOURCE=.\AVStreamsS.i
# End Source File
# Begin Source File

SOURCE=.\AVStreamsS_T.i
# End Source File
# Begin Source File

SOURCE=.\AV\FlowSpec_Entry.i
# End Source File
# Begin Source File

SOURCE=.\AV\MCast.i
# End Source File
# Begin Source File

SOURCE=.\Null_MediaCtrlC.i
# End Source File
# Begin Source File

SOURCE=.\Null_MediaCtrlS.i
# End Source File
# Begin Source File

SOURCE=.\Null_MediaCtrlS_T.i
# End Source File
# Begin Source File

SOURCE=.\AV\Policy.i
# End Source File
# Begin Source File

SOURCE=.\AV\QoS_UDP.i
# End Source File
# Begin Source File

SOURCE=.\sfpC.i
# End Source File
# Begin Source File

SOURCE=.\sfpS.i
# End Source File
# Begin Source File

SOURCE=.\sfpS_T.i
# End Source File
# Begin Source File

SOURCE=.\AV\source.i
# End Source File
# Begin Source File

SOURCE=.\AV\Transport.i
# End Source File
# Begin Source File

SOURCE=.\AV\UDP.i
# End Source File
# End Group
# Begin Group "Template Files"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\AVStreamsS_T.cpp

!IF  "$(CFG)" == "AV - Win32 Release"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "AV - Win32 Debug"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "AV - Win32 MFC Debug"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "AV - Win32 MFC Release"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\AV\Endpoint_Strategy_T.cpp

!IF  "$(CFG)" == "AV - Win32 Release"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "AV - Win32 Debug"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "AV - Win32 MFC Debug"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "AV - Win32 MFC Release"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\AV\Flows_T.cpp

!IF  "$(CFG)" == "AV - Win32 Release"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "AV - Win32 Debug"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "AV - Win32 MFC Debug"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "AV - Win32 MFC Release"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Null_MediaCtrlS_T.cpp

!IF  "$(CFG)" == "AV - Win32 Release"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "AV - Win32 Debug"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "AV - Win32 MFC Debug"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "AV - Win32 MFC Release"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "rc"
# Begin Source File

SOURCE=.\AV.rc
# End Source File
# End Group
# End Target
# End Project
