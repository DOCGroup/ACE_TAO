# Microsoft Developer Studio Project File - Name="RTECPerf DLL" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Dynamic-Link Library" 0x0102

CFG=RTECPerf DLL - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "TAO_RTEC_Perf.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "TAO_RTEC_Perf.mak" CFG="RTECPerf DLL - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "RTECPerf DLL - Win32 Release" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "RTECPerf DLL - Win32 Debug" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "RTECPerf DLL - Win32 Release"

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
# ADD BASE CPP /nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /YX /FD /c
# ADD CPP /nologo /MD /W3 /GX /O2 /I "..\..\.." /I "..\..\..\.." /I "..\..\..\..\.." /D EC_Test_HAS_DLL=1 /D "TAO_RTEC_PERF_BUILD_DLL" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /FD /c
# SUBTRACT CPP /YX
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /o "NUL" /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /o "NUL" /win32
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /dll /machine:I386
# ADD LINK32 ace.lib TAO.lib TAO_PortableServer.lib TAO_Svc_Utils.lib TAO_RTEvent.lib TAO_RTCORBAEvent.lib TAO_Messaging.lib TAO_RTCORBA.lib TAO_RTPortableServer.lib /nologo /subsystem:windows /dll /machine:I386 /out:"..\..\..\..\..\bin\RTECPerf.dll" /libpath:"..\..\..\..\tao" /libpath:"..\..\..\orbsvcs" /libpath:"..\..\..\..\tao\PortableServer" /libpath:"..\..\..\..\tao\RTCORBA" /libpath:"..\..\..\..\tao\RTPortableServer" /libpath:"..\..\..\..\tao\Messaging" /libpath:"..\..\..\..\..\ace"

!ELSEIF  "$(CFG)" == "RTECPerf DLL - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir ""
# PROP Intermediate_Dir "Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MTd /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /YX /FD /c
# ADD CPP /nologo /MDd /W3 /Gm /GX /Zi /Od /I "..\..\.." /I "..\..\..\.." /I "..\..\..\..\.." /D EC_Test_HAS_DLL=1 /D "TAO_RTEC_PERF_BUILD_DLL" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /FD /c
# SUBTRACT CPP /YX
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /o "NUL" /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /o "NUL" /win32
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /dll /debug /machine:I386 /pdbtype:sept
# ADD LINK32 aced.lib TAOd.lib TAO_PortableServerd.lib TAO_Svc_Utilsd.lib TAO_RTEventd.lib TAO_RTCORBAEventd.lib TAO_Messagingd.lib TAO_RTCORBAd.lib TAO_RTPortableServerd.lib /nologo /subsystem:windows /dll /debug /machine:I386 /out:"..\..\..\..\..\bin\RTECPerfd.dll" /pdbtype:sept /libpath:"..\..\..\orbsvcs" /libpath:"..\..\..\..\tao" /libpath:"..\..\..\..\tao\PortableServer" /libpath:"..\..\..\..\tao\RTCORBA" /libpath:"..\..\..\..\tao\RTPortableServer" /libpath:"..\..\..\..\tao\Messaging" /libpath:"..\..\..\..\..\ace"

!ENDIF 

# Begin Target

# Name "RTECPerf DLL - Win32 Release"
# Name "RTECPerf DLL - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter ".cpp"
# Begin Source File

SOURCE=.\Client_Group.cpp
# End Source File
# Begin Source File

SOURCE=.\Client_Options.cpp
# End Source File
# Begin Source File

SOURCE=.\Client_Pair.cpp
# End Source File
# Begin Source File

SOURCE=.\Consumer.cpp
# End Source File
# Begin Source File

SOURCE=.\Control.cpp
# End Source File
# Begin Source File

SOURCE=.\EC_Destroyer.cpp
# End Source File
# Begin Source File

SOURCE=.\Federated_TestC.cpp
# ADD CPP /GR
# End Source File
# Begin Source File

SOURCE=.\Federated_TestS.cpp
# End Source File
# Begin Source File

SOURCE=.\Implicit_Deactivator.cpp
# End Source File
# Begin Source File

SOURCE=.\Loopback.cpp
# End Source File
# Begin Source File

SOURCE=.\Loopback_Consumer.cpp
# End Source File
# Begin Source File

SOURCE=.\Loopback_Pair.cpp
# End Source File
# Begin Source File

SOURCE=.\Loopback_Supplier.cpp
# End Source File
# Begin Source File

SOURCE=.\ORB_Holder.cpp
# End Source File
# Begin Source File

SOURCE=.\ORB_Shutdown.cpp
# End Source File
# Begin Source File

SOURCE=.\ORB_Task.cpp
# End Source File
# Begin Source File

SOURCE=.\ORB_Task_Activator.cpp
# End Source File
# Begin Source File

SOURCE=.\Peer_Base.cpp
# End Source File
# Begin Source File

SOURCE=.\PriorityBand_Setup.cpp
# End Source File
# Begin Source File

SOURCE=.\RT_Class.cpp
# End Source File
# Begin Source File

SOURCE=.\RTClient_Setup.cpp
# End Source File
# Begin Source File

SOURCE=.\RTCORBA_Setup.cpp
# End Source File
# Begin Source File

SOURCE=.\RTEC_Initializer.cpp
# End Source File
# Begin Source File

SOURCE=.\RTPOA_Setup.cpp
# End Source File
# Begin Source File

SOURCE=.\RTServer_Setup.cpp
# End Source File
# Begin Source File

SOURCE=.\Send_Task.cpp
# End Source File
# Begin Source File

SOURCE=.\Send_Task_Stopper.cpp
# End Source File
# Begin Source File

SOURCE=.\Supplier.cpp
# End Source File
# Begin Source File

SOURCE=.\SyncScope_Setup.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter ".h"
# Begin Source File

SOURCE=.\Auto_Disconnect.h
# End Source File
# Begin Source File

SOURCE=.\Client_Group.h
# End Source File
# Begin Source File

SOURCE=.\Client_Options.h
# End Source File
# Begin Source File

SOURCE=.\Client_Pair.h
# End Source File
# Begin Source File

SOURCE=.\Consumer.h
# End Source File
# Begin Source File

SOURCE=.\Control.h
# End Source File
# Begin Source File

SOURCE=.\EC_Destroyer.h
# End Source File
# Begin Source File

SOURCE=.\Federated_TestC.h
# End Source File
# Begin Source File

SOURCE=.\Federated_TestS.h
# End Source File
# Begin Source File

SOURCE=.\Federated_TestS_T.h
# End Source File
# Begin Source File

SOURCE=.\Implicit_Deactivator.h
# End Source File
# Begin Source File

SOURCE=.\Loopback.h
# End Source File
# Begin Source File

SOURCE=.\Loopback_Consumer.h
# End Source File
# Begin Source File

SOURCE=.\Loopback_Pair.h
# End Source File
# Begin Source File

SOURCE=.\Loopback_Supplier.h
# End Source File
# Begin Source File

SOURCE=.\ORB_Holder.h
# End Source File
# Begin Source File

SOURCE=.\ORB_Shutdown.h
# End Source File
# Begin Source File

SOURCE=.\ORB_Task.h
# End Source File
# Begin Source File

SOURCE=.\ORB_Task_Activator.h
# End Source File
# Begin Source File

SOURCE=.\Peer_Base.h
# End Source File
# Begin Source File

SOURCE=.\PriorityBand_Setup.h
# End Source File
# Begin Source File

SOURCE=.\RIR_Narrow.h
# End Source File
# Begin Source File

SOURCE=.\RT_Class.h
# End Source File
# Begin Source File

SOURCE=.\RTClient_Setup.h
# End Source File
# Begin Source File

SOURCE=.\RTCORBA_Setup.h
# End Source File
# Begin Source File

SOURCE=.\RTEC_Initializer.h
# End Source File
# Begin Source File

SOURCE=.\rtec_perf_export.h
# End Source File
# Begin Source File

SOURCE=.\RTPOA_Setup.h
# End Source File
# Begin Source File

SOURCE=.\RTServer_Setup.h
# End Source File
# Begin Source File

SOURCE=.\Send_Task.h
# End Source File
# Begin Source File

SOURCE=.\Send_Task_Stopper.h
# End Source File
# Begin Source File

SOURCE=.\Servant_var.h
# End Source File
# Begin Source File

SOURCE=.\Supplier.h
# End Source File
# Begin Source File

SOURCE=.\SyncScope_Setup.h
# End Source File
# End Group
# Begin Group "Inline Files"

# PROP Default_Filter ".i;.inl"
# Begin Source File

SOURCE=.\Auto_Disconnect.inl
# End Source File
# Begin Source File

SOURCE=.\Client_Group.inl
# End Source File
# Begin Source File

SOURCE=.\Client_Pair.inl
# End Source File
# Begin Source File

SOURCE=.\EC_Destroyer.inl
# End Source File
# Begin Source File

SOURCE=.\Federated_TestC.i
# End Source File
# Begin Source File

SOURCE=.\Federated_TestS.i
# End Source File
# Begin Source File

SOURCE=.\Federated_TestS_T.i
# End Source File
# Begin Source File

SOURCE=.\Implicit_Deactivator.inl
# End Source File
# Begin Source File

SOURCE=.\Loopback_Pair.inl
# End Source File
# Begin Source File

SOURCE=.\ORB_Holder.inl
# End Source File
# Begin Source File

SOURCE=.\ORB_Shutdown.inl
# End Source File
# Begin Source File

SOURCE=.\ORB_Task_Activator.inl
# End Source File
# Begin Source File

SOURCE=.\PriorityBand_Setup.inl
# End Source File
# Begin Source File

SOURCE=.\RT_Class.inl
# End Source File
# Begin Source File

SOURCE=.\RTClient_Setup.inl
# End Source File
# Begin Source File

SOURCE=.\RTCORBA_Setup.inl
# End Source File
# Begin Source File

SOURCE=.\RTPOA_Setup.inl
# End Source File
# Begin Source File

SOURCE=.\RTServer_Setup.inl
# End Source File
# Begin Source File

SOURCE=.\Send_Task_Stopper.inl
# End Source File
# Begin Source File

SOURCE=.\Servant_var.inl
# End Source File
# Begin Source File

SOURCE=.\SyncScope_Setup.inl
# End Source File
# End Group
# Begin Group "Template Files"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\Auto_Disconnect.cpp
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\Federated_TestS_T.cpp
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\RIR_Narrow.cpp
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\Servant_var.cpp
# PROP Exclude_From_Build 1
# End Source File
# End Group
# Begin Group "IDL Files"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\Federated_Test.idl

!IF  "$(CFG)" == "RTECPerf DLL - Win32 Release"

# PROP Ignore_Default_Tool 1
# Begin Custom Build
InputPath=.\Federated_Test.idl
InputName=Federated_Test

BuildCmds= \
	..\..\..\..\..\bin\Release\tao_idl.exe -Ge 1 -Wb,export_macro=TAO_RTEC_Perf_Export -Wb,export_include=rtec_perf_export.h $(InputPath)

"$(InputName)C.h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)S.h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)S_T.h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)C.i" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)S.i" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)S_T.i" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)C.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)S.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)S_T.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)
# End Custom Build

!ELSEIF  "$(CFG)" == "RTECPerf DLL - Win32 Debug"

# PROP Ignore_Default_Tool 1
# Begin Custom Build
InputPath=.\Federated_Test.idl
InputName=Federated_Test

BuildCmds= \
	..\..\..\..\..\bin\tao_idl.exe -Ge 1 -Wb,export_macro=TAO_RTEC_Perf_Export -Wb,export_include=rtec_perf_export.h $(InputPath)

"$(InputName)C.h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)S.h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)S_T.h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)C.i" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)S.i" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)S_T.i" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)C.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)S.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)S_T.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)
# End Custom Build

!ENDIF 

# End Source File
# End Group
# End Target
# End Project
