# Microsoft Developer Studio Project File - Name="RTEvent" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Dynamic-Link Library" 0x0102

CFG=RTEvent - Win32 MFC Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "RTEvent.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "RTEvent.mak" CFG="RTEvent - Win32 MFC Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "RTEvent - Win32 Release" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "RTEvent - Win32 Debug" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "RTEvent - Win32 MFC Debug" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "RTEvent - Win32 MFC Release" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "RTEvent - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir ""
# PROP Intermediate_Dir "DLL\Release\RTEvent"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "RTEvent_EXPORTS" /YX /FD /c
# ADD CPP /nologo /MD /W3 /GX /O2 /I "../" /I "../../" /I "../../../" /D "NDEBUG" /D "WIN32" /D "_WINDOWS" /D "TAO_RTEVENT_BUILD_DLL" /FD /c
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
# ADD LINK32 TAO_Svc_Utils.lib ace.lib TAO.lib TAO_PortableServer.lib TAO_Messaging.lib /nologo /dll /machine:I386 /out:"..\..\..\bin\TAO_RTEvent.dll" /libpath:"..\..\tao\Messaging" /libpath:"..\..\tao\PortableServer" /libpath:"..\..\tao" /libpath:"..\..\..\ace"

!ELSEIF  "$(CFG)" == "RTEvent - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir ""
# PROP Intermediate_Dir "DLL\Debug\RTEvent"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MTd /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "RTEvent_EXPORTS" /YX /FD /c
# ADD CPP /nologo /MDd /W3 /Gm /GX /Zi /Od /I "../" /I "../../" /I "../../../" /D "_DEBUG" /D "WIN32" /D "_WINDOWS" /D "TAO_RTEVENT_BUILD_DLL" /FD /c
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
# ADD LINK32 TAO_Svc_Utilsd.lib TAOd.lib aced.lib TAO_PortableServerd.lib TAO_Messagingd.lib /nologo /dll /debug /machine:I386 /out:"..\..\..\bin\TAO_RTEventd.dll" /pdbtype:sept /libpath:"..\..\tao\Messaging" /libpath:"..\..\tao\PortableServer" /libpath:"..\..\tao" /libpath:"..\..\..\ace"

!ELSEIF  "$(CFG)" == "RTEvent - Win32 MFC Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "RTEvent___Win32_MFC_Debug"
# PROP BASE Intermediate_Dir "RTEvent___Win32_MFC_Debug"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir ""
# PROP Intermediate_Dir "DLL\Debug\RTEventMFC"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MDd /W3 /Gm /GX /Zi /Od /I "../" /I "../../" /I "../../../" /D "_DEBUG" /D "WIN32" /D "_WINDOWS" /D "TAO_RTEVENT_BUILD_DLL" /FD /c
# SUBTRACT BASE CPP /YX
# ADD CPP /nologo /MDd /W3 /Gm /GX /Zi /Od /I "../" /I "../../" /I "../../../" /D "_DEBUG" /D "WIN32" /D "_WINDOWS" /D "TAO_RTEVENT_BUILD_DLL" /D ACE_HAS_MFC=1 /FD /c
# SUBTRACT CPP /YX
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 TAOd.lib aced.lib TAO_Svc_Utilsd.lib /nologo /dll /debug /machine:I386 /out:"..\..\..\bin\TAO_RTEventd.dll" /pdbtype:sept /libpath:"..\..\tao" /libpath:"..\..\..\ace"
# ADD LINK32 TAOmfcd.lib acemfcd.lib TAO_Svc_Utilsmfcd.lib TAO_PortableServermfcd.lib TAO_Messagingmfcd.lib /nologo /dll /debug /machine:I386 /out:"..\..\..\bin\TAO_RTEventmfcd.dll" /pdbtype:sept /libpath:"..\..\tao\Messaging" /libpath:"..\..\tao\PortableServer" /libpath:"..\..\tao" /libpath:"..\..\..\ace"

!ELSEIF  "$(CFG)" == "RTEvent - Win32 MFC Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "RTEvent___Win32_MFC_Release"
# PROP BASE Intermediate_Dir "RTEvent___Win32_MFC_Release"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir ""
# PROP Intermediate_Dir "DLL\Release\RTEventMFC"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MD /W3 /GX /O2 /I "../" /I "../../" /I "../../../" /D "NDEBUG" /D "WIN32" /D "_WINDOWS" /D "TAO_RTEVENT_BUILD_DLL" /FD /c
# SUBTRACT BASE CPP /YX
# ADD CPP /nologo /MD /W3 /GX /O2 /I "../" /I "../../" /I "../../../" /D "NDEBUG" /D "WIN32" /D "_WINDOWS" /D "TAO_RTEVENT_BUILD_DLL" /D ACE_HAS_MFC=1 /FD /c
# SUBTRACT CPP /YX
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 ace.lib TAO.lib TAO_Svc_Utils.lib /nologo /dll /machine:I386 /out:"..\..\..\bin\TAO_RTEvent.dll" /libpath:"..\..\tao" /libpath:"..\..\..\ace"
# ADD LINK32 acemfc.lib TAOmfc.lib TAO_Svc_Utilsmfc.lib TAO_PortableServermfc.lib TAO_Messagingmfc.lib /nologo /dll /machine:I386 /out:"..\..\..\bin\TAO_RTEventmfc.dll" /libpath:"..\..\tao\Messaging" /libpath:"..\..\tao\PortableServer" /libpath:"..\..\tao" /libpath:"..\..\..\ace"

!ENDIF 

# Begin Target

# Name "RTEvent - Win32 Release"
# Name "RTEvent - Win32 Debug"
# Name "RTEvent - Win32 MFC Debug"
# Name "RTEvent - Win32 MFC Release"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\Event\CRC.cpp
# End Source File
# Begin Source File

SOURCE=.\Event\EC_And_Filter.cpp
# End Source File
# Begin Source File

SOURCE=.\Event\EC_Basic_Factory.cpp
# End Source File
# Begin Source File

SOURCE=.\Event\EC_Basic_Filter_Builder.cpp
# End Source File
# Begin Source File

SOURCE=.\Event\EC_Bitmask_Filter.cpp
# End Source File
# Begin Source File

SOURCE=.\Event\EC_Busy_Lock.cpp
# End Source File
# Begin Source File

SOURCE=.\Event\EC_Channel_Destroyer.cpp
# End Source File
# Begin Source File

SOURCE=.\Event\EC_Conjunction_Filter.cpp
# End Source File
# Begin Source File

SOURCE=.\Event\EC_ConsumerAdmin.cpp
# End Source File
# Begin Source File

SOURCE=.\Event\EC_ConsumerControl.cpp
# End Source File
# Begin Source File

SOURCE=.\Event\EC_Default_Factory.cpp
# End Source File
# Begin Source File

SOURCE=.\Event\EC_Default_ProxyConsumer.cpp
# End Source File
# Begin Source File

SOURCE=.\Event\EC_Default_ProxySupplier.cpp
# End Source File
# Begin Source File

SOURCE=.\Event\EC_Disjunction_Filter.cpp
# End Source File
# Begin Source File

SOURCE=.\Event\EC_Dispatching.cpp
# End Source File
# Begin Source File

SOURCE=.\Event\EC_Dispatching_Task.cpp
# End Source File
# Begin Source File

SOURCE=.\Event\EC_Event_Channel.cpp
# End Source File
# Begin Source File

SOURCE=.\Event\EC_Event_Channel_Base.cpp
# End Source File
# Begin Source File

SOURCE=.\Event\EC_Factory.cpp
# End Source File
# Begin Source File

SOURCE=.\Event\EC_Filter.cpp
# End Source File
# Begin Source File

SOURCE=.\Event\EC_Filter_Builder.cpp
# End Source File
# Begin Source File

SOURCE=.\Event\EC_Gateway.cpp
# End Source File
# Begin Source File

SOURCE=.\Event\EC_Gateway_IIOP.cpp
# End Source File
# Begin Source File

SOURCE=.\Event\EC_Group_Scheduling.cpp
# End Source File
# Begin Source File

SOURCE=.\Event\EC_Lifetime_Utils.cpp
# End Source File
# Begin Source File

SOURCE=.\Event\EC_Masked_Type_Filter.cpp
# End Source File
# Begin Source File

SOURCE=.\Event\EC_MT_Dispatching.cpp
# End Source File
# Begin Source File

SOURCE=.\Event\EC_Negation_Filter.cpp
# End Source File
# Begin Source File

SOURCE=.\Event\EC_Null_Factory.cpp
# End Source File
# Begin Source File

SOURCE=.\Event\EC_Null_Scheduling.cpp
# End Source File
# Begin Source File

SOURCE=.\Event\EC_ObserverStrategy.cpp
# End Source File
# Begin Source File

SOURCE=.\Event\EC_Per_Supplier_Filter.cpp
# End Source File
# Begin Source File

SOURCE=.\Event\EC_Prefix_Filter_Builder.cpp
# End Source File
# Begin Source File

SOURCE=.\Event\EC_Proxy_Disconnector.cpp
# End Source File
# Begin Source File

SOURCE=.\Event\EC_ProxyConsumer.cpp
# End Source File
# Begin Source File

SOURCE=.\Event\EC_ProxySupplier.cpp
# End Source File
# Begin Source File

SOURCE=.\Event\EC_QOS_Info.cpp
# End Source File
# Begin Source File

SOURCE=.\Event\EC_Reactive_ConsumerControl.cpp
# End Source File
# Begin Source File

SOURCE=.\Event\EC_Reactive_Dispatching.cpp
# End Source File
# Begin Source File

SOURCE=.\Event\EC_Reactive_SupplierControl.cpp
# End Source File
# Begin Source File

SOURCE=.\Event\EC_Reactive_Timeout_Generator.cpp
# End Source File
# Begin Source File

SOURCE=.\Event\EC_Scheduling_Strategy.cpp
# End Source File
# Begin Source File

SOURCE=.\Event\EC_Supplier_Filter.cpp
# End Source File
# Begin Source File

SOURCE=.\Event\EC_Supplier_Filter_Builder.cpp
# End Source File
# Begin Source File

SOURCE=.\Event\EC_SupplierAdmin.cpp
# End Source File
# Begin Source File

SOURCE=.\Event\EC_SupplierControl.cpp
# End Source File
# Begin Source File

SOURCE=.\Event\EC_Timeout_Filter.cpp
# End Source File
# Begin Source File

SOURCE=.\Event\EC_Timeout_Generator.cpp
# End Source File
# Begin Source File

SOURCE=.\Event\EC_Trivial_Supplier_Filter.cpp
# End Source File
# Begin Source File

SOURCE=.\Event\EC_Type_Filter.cpp
# End Source File
# Begin Source File

SOURCE=.\Event\EC_UDP_Admin.cpp
# End Source File
# Begin Source File

SOURCE=.\Event\ECG_Adapters.cpp
# End Source File
# Begin Source File

SOURCE=.\Event\ECG_Address_Server_Base.cpp
# End Source File
# Begin Source File

SOURCE=.\Event\ECG_CDR_Message_Receiver.cpp
# End Source File
# Begin Source File

SOURCE=.\Event\ECG_CDR_Message_Sender.cpp
# End Source File
# Begin Source File

SOURCE=.\Event\ECG_Complex_Address_Server.cpp
# End Source File
# Begin Source File

SOURCE=.\Event\ECG_Mcast_EH.cpp
# End Source File
# Begin Source File

SOURCE=.\Event\ECG_Mcast_Gateway.cpp
# End Source File
# Begin Source File

SOURCE=.\Event\ECG_Simple_Address_Server.cpp
# End Source File
# Begin Source File

SOURCE=.\Event\ECG_Simple_Mcast_EH.cpp
# End Source File
# Begin Source File

SOURCE=.\Event\ECG_UDP_EH.cpp
# End Source File
# Begin Source File

SOURCE=.\Event\ECG_UDP_Out_Endpoint.cpp
# End Source File
# Begin Source File

SOURCE=.\Event\ECG_UDP_Receiver.cpp
# End Source File
# Begin Source File

SOURCE=.\Event\ECG_UDP_Sender.cpp
# End Source File
# Begin Source File

SOURCE=.\Event_Utilities.cpp
# End Source File
# Begin Source File

SOURCE=.\RtecDefaultEventDataC.cpp
# ADD CPP /GR
# End Source File
# Begin Source File

SOURCE=.\RtecDefaultEventDataS.cpp
# End Source File
# Begin Source File

SOURCE=.\RtecEventChannelAdminC.cpp
# ADD CPP /GR
# End Source File
# Begin Source File

SOURCE=.\RtecEventChannelAdminS.cpp
# End Source File
# Begin Source File

SOURCE=.\RtecEventCommC.cpp
# ADD CPP /GR
# End Source File
# Begin Source File

SOURCE=.\RtecEventCommS.cpp
# End Source File
# Begin Source File

SOURCE=.\RtecUDPAdminC.cpp
# ADD CPP /GR
# End Source File
# Begin Source File

SOURCE=.\RtecUDPAdminS.cpp
# ADD CPP /GR-
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\Event\CRC.h
# End Source File
# Begin Source File

SOURCE=.\Event\EC_And_Filter.h
# End Source File
# Begin Source File

SOURCE=.\Event\EC_Basic_Factory.h
# End Source File
# Begin Source File

SOURCE=.\Event\EC_Basic_Filter_Builder.h
# End Source File
# Begin Source File

SOURCE=.\Event\EC_Bitmask_Filter.h
# End Source File
# Begin Source File

SOURCE=.\Event\EC_Busy_Lock.h
# End Source File
# Begin Source File

SOURCE=.\Event\EC_Conjunction_Filter.h
# End Source File
# Begin Source File

SOURCE=.\Event\EC_ConsumerAdmin.h
# End Source File
# Begin Source File

SOURCE=.\Event\EC_ConsumerControl.h
# End Source File
# Begin Source File

SOURCE=.\Event\EC_Default_Factory.h
# End Source File
# Begin Source File

SOURCE=.\Event\EC_Default_ProxyConsumer.h
# End Source File
# Begin Source File

SOURCE=.\Event\EC_Default_ProxySupplier.h
# End Source File
# Begin Source File

SOURCE=.\Event\EC_Defaults.h
# End Source File
# Begin Source File

SOURCE=.\Event\EC_Disjunction_Filter.h
# End Source File
# Begin Source File

SOURCE=.\Event\EC_Dispatching.h
# End Source File
# Begin Source File

SOURCE=.\Event\EC_Dispatching_Task.h
# End Source File
# Begin Source File

SOURCE=.\Event\EC_Event_Channel.h
# End Source File
# Begin Source File

SOURCE=.\Event\EC_Event_Channel_Base.h
# End Source File
# Begin Source File

SOURCE=.\Event\EC_Factory.h
# End Source File
# Begin Source File

SOURCE=.\Event\EC_Filter.h
# End Source File
# Begin Source File

SOURCE=.\Event\EC_Filter_Builder.h
# End Source File
# Begin Source File

SOURCE=.\Event\EC_Gateway.h
# End Source File
# Begin Source File

SOURCE=.\Event\EC_Gateway_IIOP.h
# End Source File
# Begin Source File

SOURCE=.\Event\EC_Group_Scheduling.h
# End Source File
# Begin Source File

SOURCE=.\Event\EC_Group_Scheduling.inl
# End Source File
# Begin Source File

SOURCE=.\Event\EC_Masked_Type_Filter.h
# End Source File
# Begin Source File

SOURCE=.\Event\EC_MT_Dispatching.h
# End Source File
# Begin Source File

SOURCE=.\Event\EC_Negation_Filter.h
# End Source File
# Begin Source File

SOURCE=.\Event\EC_Null_Factory.h
# End Source File
# Begin Source File

SOURCE=.\Event\EC_Null_Scheduling.h
# End Source File
# Begin Source File

SOURCE=.\Event\EC_ObserverStrategy.h
# End Source File
# Begin Source File

SOURCE=.\Event\EC_Per_Supplier_Filter.h
# End Source File
# Begin Source File

SOURCE=.\Event\EC_Prefix_Filter_Builder.h
# End Source File
# Begin Source File

SOURCE=.\Event\EC_ProxyConsumer.h
# End Source File
# Begin Source File

SOURCE=.\Event\EC_ProxySupplier.h
# End Source File
# Begin Source File

SOURCE=.\Event\EC_QOS_Info.h
# End Source File
# Begin Source File

SOURCE=.\Event\EC_Reactive_ConsumerControl.h
# End Source File
# Begin Source File

SOURCE=.\Event\EC_Reactive_Dispatching.h
# End Source File
# Begin Source File

SOURCE=.\Event\EC_Reactive_SupplierControl.h
# End Source File
# Begin Source File

SOURCE=.\Event\EC_Reactive_Timeout_Generator.h
# End Source File
# Begin Source File

SOURCE=.\Event\EC_Scheduling_Strategy.h
# End Source File
# Begin Source File

SOURCE=.\Event\EC_Supplier_Filter.h
# End Source File
# Begin Source File

SOURCE=.\Event\EC_Supplier_Filter_Builder.h
# End Source File
# Begin Source File

SOURCE=.\Event\EC_SupplierAdmin.h
# End Source File
# Begin Source File

SOURCE=.\Event\EC_SupplierControl.h
# End Source File
# Begin Source File

SOURCE=.\Event\EC_Timeout_Filter.h
# End Source File
# Begin Source File

SOURCE=.\Event\EC_Timeout_Generator.h
# End Source File
# Begin Source File

SOURCE=.\Event\EC_Trivial_Supplier_Filter.h
# End Source File
# Begin Source File

SOURCE=.\Event\EC_Type_Filter.h
# End Source File
# Begin Source File

SOURCE=.\Event\EC_UDP_Admin.h
# End Source File
# Begin Source File

SOURCE=.\Event\ECG_Adapters.h
# End Source File
# Begin Source File

SOURCE=.\Event\ECG_Address_Server_Base.h
# End Source File
# Begin Source File

SOURCE=.\Event\ECG_CDR_Message_Receiver.h
# End Source File
# Begin Source File

SOURCE=.\Event\ECG_CDR_Message_Sender.h
# End Source File
# Begin Source File

SOURCE=.\Event\ECG_Complex_Address_Server.h
# End Source File
# Begin Source File

SOURCE=.\Event\ECG_Defaults.h
# End Source File
# Begin Source File

SOURCE=.\Event\ECG_Mcast_EH.h
# End Source File
# Begin Source File

SOURCE=.\Event\ECG_Mcast_Gateway.h
# End Source File
# Begin Source File

SOURCE=.\Event\ECG_Simple_Address_Server.h
# End Source File
# Begin Source File

SOURCE=.\Event\ECG_Simple_Mcast_EH.h
# End Source File
# Begin Source File

SOURCE=.\Event\ECG_UDP_EH.h
# End Source File
# Begin Source File

SOURCE=.\Event\ECG_UDP_Out_Endpoint.h
# End Source File
# Begin Source File

SOURCE=.\Event\ECG_UDP_Protocol.h
# End Source File
# Begin Source File

SOURCE=.\Event\ECG_UDP_Receiver.h
# End Source File
# Begin Source File

SOURCE=.\Event\ECG_UDP_Sender.h
# End Source File
# Begin Source File

SOURCE=.\Event\ECG_UDP_Sender.inl
# End Source File
# Begin Source File

SOURCE=.\Event\Event_Channel.h
# End Source File
# Begin Source File

SOURCE=.\Event\event_export.h
# End Source File
# Begin Source File

SOURCE=.\Event_Utilities.h
# End Source File
# Begin Source File

SOURCE=.\RtecDefaultEventDataC.h
# End Source File
# Begin Source File

SOURCE=.\RtecDefaultEventDataS.h
# End Source File
# Begin Source File

SOURCE=.\RtecDefaultEventDataS_T.h
# End Source File
# Begin Source File

SOURCE=.\RtecEventChannelAdminC.h
# End Source File
# Begin Source File

SOURCE=.\RtecEventChannelAdminS.h
# End Source File
# Begin Source File

SOURCE=.\RtecEventChannelAdminS_T.h
# End Source File
# Begin Source File

SOURCE=.\RtecEventCommC.h
# End Source File
# Begin Source File

SOURCE=.\RtecEventCommS.h
# End Source File
# Begin Source File

SOURCE=.\RtecEventCommS_T.h
# End Source File
# Begin Source File

SOURCE=.\RtecUDPAdminC.h
# End Source File
# Begin Source File

SOURCE=.\RtecUDPAdminS.h
# End Source File
# Begin Source File

SOURCE=.\RtecUDPAdminS_T.h
# End Source File
# End Group
# Begin Group "IDL Files"

# PROP Default_Filter ".idl"
# Begin Source File

SOURCE=.\RtecDefaultEventData.idl

!IF  "$(CFG)" == "RTEvent - Win32 Release"

# PROP Ignore_Default_Tool 1
USERDEP__RTECD="..\..\..\bin\Release\tao_idl.exe"	
# Begin Custom Build - Invoking TAO_IDL Compiler on $(InputName)
InputPath=.\RtecDefaultEventData.idl
InputName=RtecDefaultEventData

BuildCmds= \
	..\..\..\bin\Release\tao_idl -Ge 1 -I../../ -I../../tao  -Wb,pre_include=ace/pre.h -Wb,post_include=ace/post.h -Wb,export_macro=TAO_RTEvent_Export -Wb,export_include=Event\event_export.h $(InputName).idl

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

!ELSEIF  "$(CFG)" == "RTEvent - Win32 Debug"

# PROP Ignore_Default_Tool 1
USERDEP__RTECD="..\..\..\bin\tao_idl.exe"	
# Begin Custom Build - Invoking TAO_IDL Compiler on $(InputName)
InputPath=.\RtecDefaultEventData.idl
InputName=RtecDefaultEventData

BuildCmds= \
	..\..\..\bin\tao_idl -Ge 1 -I../../ -I../../tao  -Wb,pre_include=ace/pre.h -Wb,post_include=ace/post.h -Wb,export_macro=TAO_RTEvent_Export -Wb,export_include=Event\event_export.h $(InputName).idl

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

!ELSEIF  "$(CFG)" == "RTEvent - Win32 MFC Debug"

# PROP BASE Ignore_Default_Tool 1
# PROP Ignore_Default_Tool 1
# Begin Custom Build - Invoking TAO_IDL Compiler on $(InputName)
InputPath=.\RtecDefaultEventData.idl
InputName=RtecDefaultEventData

BuildCmds= \
	..\..\..\bin\tao_idl -Ge 1 -I../../ -I../../tao  -Wb,pre_include=ace/pre.h -Wb,post_include=ace/post.h -Wb,export_macro=TAO_RTEvent_Export -Wb,export_include=Event\event_export.h $(InputName).idl

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

!ELSEIF  "$(CFG)" == "RTEvent - Win32 MFC Release"

# PROP BASE Ignore_Default_Tool 1
# PROP Ignore_Default_Tool 1
# Begin Custom Build - Invoking TAO_IDL Compiler on $(InputName)
InputPath=.\RtecDefaultEventData.idl
InputName=RtecDefaultEventData

BuildCmds= \
	..\..\..\bin\Release\tao_idl -Ge 1 -I../../ -I../../tao  -Wb,pre_include=ace/pre.h -Wb,post_include=ace/post.h -Wb,export_macro=TAO_RTEvent_Export -Wb,export_include=Event\event_export.h $(InputName).idl

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

SOURCE=.\RtecEventChannelAdmin.idl

!IF  "$(CFG)" == "RTEvent - Win32 Release"

# PROP Ignore_Default_Tool 1
USERDEP__RTECE="..\..\..\bin\Release\tao_idl.exe"	
# Begin Custom Build - Invoking TAO_IDL Compiler on $(InputName)
InputPath=.\RtecEventChannelAdmin.idl
InputName=RtecEventChannelAdmin

BuildCmds= \
	..\..\..\bin\Release\tao_idl -Ge 1 -I../../ -I../../tao  -Wb,pre_include=ace/pre.h -Wb,post_include=ace/post.h -Wb,export_macro=TAO_RTEvent_Export -Wb,export_include=Event\event_export.h $(InputName).idl

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

!ELSEIF  "$(CFG)" == "RTEvent - Win32 Debug"

# PROP Ignore_Default_Tool 1
USERDEP__RTECE="..\..\..\bin\tao_idl.exe"	
# Begin Custom Build - Invoking TAO_IDL Compiler on $(InputName)
InputPath=.\RtecEventChannelAdmin.idl
InputName=RtecEventChannelAdmin

BuildCmds= \
	..\..\..\bin\tao_idl -Ge 1 -I../../ -I../../tao  -Wb,pre_include=ace/pre.h -Wb,post_include=ace/post.h -Wb,export_macro=TAO_RTEvent_Export -Wb,export_include=Event\event_export.h $(InputName).idl

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

!ELSEIF  "$(CFG)" == "RTEvent - Win32 MFC Debug"

# PROP BASE Ignore_Default_Tool 1
# PROP Ignore_Default_Tool 1
# Begin Custom Build - Invoking TAO_IDL Compiler on $(InputName)
InputPath=.\RtecEventChannelAdmin.idl
InputName=RtecEventChannelAdmin

BuildCmds= \
	..\..\..\bin\tao_idl -Ge 1 -I../../ -I../../tao  -Wb,pre_include=ace/pre.h -Wb,post_include=ace/post.h -Wb,export_macro=TAO_RTEvent_Export -Wb,export_include=Event\event_export.h $(InputName).idl

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

!ELSEIF  "$(CFG)" == "RTEvent - Win32 MFC Release"

# PROP BASE Ignore_Default_Tool 1
# PROP Ignore_Default_Tool 1
# Begin Custom Build - Invoking TAO_IDL Compiler on $(InputName)
InputPath=.\RtecEventChannelAdmin.idl
InputName=RtecEventChannelAdmin

BuildCmds= \
	..\..\..\bin\Release\tao_idl -Ge 1 -I../../ -I../../tao  -Wb,pre_include=ace/pre.h -Wb,post_include=ace/post.h -Wb,export_macro=TAO_RTEvent_Export -Wb,export_include=Event\event_export.h $(InputName).idl

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

SOURCE=.\RtecEventComm.idl

!IF  "$(CFG)" == "RTEvent - Win32 Release"

# PROP Ignore_Default_Tool 1
USERDEP__RTECEV="..\..\..\bin\Release\tao_idl.exe"	
# Begin Custom Build - Invoking TAO_IDL Compiler on $(InputName)
InputPath=.\RtecEventComm.idl
InputName=RtecEventComm

BuildCmds= \
	..\..\..\bin\Release\tao_idl -Ge 1 -I../../ -I../../tao  -Wb,pre_include=ace/pre.h -Wb,post_include=ace/post.h -Wb,export_macro=TAO_RTEvent_Export -Wb,export_include=Event\event_export.h $(InputName).idl

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

!ELSEIF  "$(CFG)" == "RTEvent - Win32 Debug"

# PROP Ignore_Default_Tool 1
USERDEP__RTECEV="..\..\..\bin\tao_idl.exe"	
# Begin Custom Build - Invoking TAO_IDL Compiler on $(InputName)
InputPath=.\RtecEventComm.idl
InputName=RtecEventComm

BuildCmds= \
	..\..\..\bin\tao_idl -Ge 1 -I../../ -I../../tao  -Wb,pre_include=ace/pre.h -Wb,post_include=ace/post.h -Wb,export_macro=TAO_RTEvent_Export -Wb,export_include=Event\event_export.h $(InputName).idl

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

!ELSEIF  "$(CFG)" == "RTEvent - Win32 MFC Debug"

# PROP BASE Ignore_Default_Tool 1
# PROP Ignore_Default_Tool 1
# Begin Custom Build - Invoking TAO_IDL Compiler on $(InputName)
InputPath=.\RtecEventComm.idl
InputName=RtecEventComm

BuildCmds= \
	..\..\..\bin\tao_idl -Ge 1 -I../../ -I../../tao  -Wb,pre_include=ace/pre.h -Wb,post_include=ace/post.h -Wb,export_macro=TAO_RTEvent_Export -Wb,export_include=Event\event_export.h $(InputName).idl

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

!ELSEIF  "$(CFG)" == "RTEvent - Win32 MFC Release"

# PROP BASE Ignore_Default_Tool 1
# PROP Ignore_Default_Tool 1
# Begin Custom Build - Invoking TAO_IDL Compiler on $(InputName)
InputPath=.\RtecEventComm.idl
InputName=RtecEventComm

BuildCmds= \
	..\..\..\bin\Release\tao_idl -Ge 1 -I../../ -I../../tao  -Wb,pre_include=ace/pre.h -Wb,post_include=ace/post.h -Wb,export_macro=TAO_RTEvent_Export -Wb,export_include=Event\event_export.h $(InputName).idl

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

SOURCE=.\RtecUDPAdmin.idl

!IF  "$(CFG)" == "RTEvent - Win32 Release"

# PROP Ignore_Default_Tool 1
USERDEP__RTECU="..\..\..\bin\Release\tao_idl.exe"	
# Begin Custom Build - Invoking TAO_IDL Compiler on $(InputName)
InputPath=.\RtecUDPAdmin.idl
InputName=RtecUDPAdmin

BuildCmds= \
	..\..\..\bin\Release\tao_idl -Ge 1 -I../../ -I../../tao  -Wb,pre_include=ace/pre.h -Wb,post_include=ace/post.h -Wb,export_macro=TAO_RTEvent_Export -Wb,export_include=Event\event_export.h $(InputName).idl

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

!ELSEIF  "$(CFG)" == "RTEvent - Win32 Debug"

# PROP Ignore_Default_Tool 1
USERDEP__RTECU="..\..\..\bin\tao_idl.exe"	
# Begin Custom Build - Invoking TAO_IDL Compiler on $(InputName)
InputPath=.\RtecUDPAdmin.idl
InputName=RtecUDPAdmin

BuildCmds= \
	..\..\..\bin\tao_idl -Ge 1 -I../../ -I../../tao  -Wb,pre_include=ace/pre.h -Wb,post_include=ace/post.h -Wb,export_macro=TAO_RTEvent_Export -Wb,export_include=Event\event_export.h $(InputName).idl

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

!ELSEIF  "$(CFG)" == "RTEvent - Win32 MFC Debug"

# PROP BASE Ignore_Default_Tool 1
# PROP Ignore_Default_Tool 1
# Begin Custom Build - Invoking TAO_IDL Compiler on $(InputName)
InputPath=.\RtecUDPAdmin.idl
InputName=RtecUDPAdmin

BuildCmds= \
	..\..\..\bin\tao_idl -Ge 1 -I../../ -I../../tao  -Wb,pre_include=ace/pre.h -Wb,post_include=ace/post.h -Wb,export_macro=TAO_RTEvent_Export -Wb,export_include=Event\event_export.h $(InputName).idl

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

!ELSEIF  "$(CFG)" == "RTEvent - Win32 MFC Release"

# PROP BASE Ignore_Default_Tool 1
# PROP Ignore_Default_Tool 1
# Begin Custom Build - Invoking TAO_IDL Compiler on $(InputName)
InputPath=.\RtecUDPAdmin.idl
InputName=RtecUDPAdmin

BuildCmds= \
	..\..\..\bin\Release\tao_idl -Ge 1 -I../../ -I../../tao  -Wb,pre_include=ace/pre.h -Wb,post_include=ace/post.h -Wb,export_macro=TAO_RTEvent_Export -Wb,export_include=Event\event_export.h $(InputName).idl

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
# Begin Group "Inline Files"

# PROP Default_Filter ".i"
# Begin Source File

SOURCE=.\Event\EC_And_Filter.i
# End Source File
# Begin Source File

SOURCE=.\Event\EC_Basic_Factory.i
# End Source File
# Begin Source File

SOURCE=.\Event\EC_Basic_Filter_Builder.i
# End Source File
# Begin Source File

SOURCE=.\Event\EC_Bitmask_Filter.i
# End Source File
# Begin Source File

SOURCE=.\Event\EC_Busy_Lock.i
# End Source File
# Begin Source File

SOURCE=.\Event\EC_Conjunction_Filter.i
# End Source File
# Begin Source File

SOURCE=.\Event\EC_ConsumerAdmin.i
# End Source File
# Begin Source File

SOURCE=.\Event\EC_ConsumerControl.i
# End Source File
# Begin Source File

SOURCE=.\Event\EC_Default_Factory.i
# End Source File
# Begin Source File

SOURCE=.\Event\EC_Default_ProxyConsumer.i
# End Source File
# Begin Source File

SOURCE=.\Event\EC_Default_ProxySupplier.i
# End Source File
# Begin Source File

SOURCE=.\Event\EC_Disjunction_Filter.i
# End Source File
# Begin Source File

SOURCE=.\Event\EC_Dispatching.i
# End Source File
# Begin Source File

SOURCE=.\Event\EC_Dispatching_Task.i
# End Source File
# Begin Source File

SOURCE=.\Event\EC_Event_Channel.i
# End Source File
# Begin Source File

SOURCE=.\Event\EC_Event_Channel_Base.i
# End Source File
# Begin Source File

SOURCE=.\Event\EC_Factory.i
# End Source File
# Begin Source File

SOURCE=.\Event\EC_Filter.i
# End Source File
# Begin Source File

SOURCE=.\Event\EC_Filter_Builder.i
# End Source File
# Begin Source File

SOURCE=.\Event\EC_Masked_Type_Filter.i
# End Source File
# Begin Source File

SOURCE=.\Event\EC_MT_Dispatching.i
# End Source File
# Begin Source File

SOURCE=.\Event\EC_Negation_Filter.i
# End Source File
# Begin Source File

SOURCE=.\Event\EC_Null_Factory.i
# End Source File
# Begin Source File

SOURCE=.\Event\EC_Null_Scheduling.i
# End Source File
# Begin Source File

SOURCE=.\Event\EC_ObserverStrategy.i
# End Source File
# Begin Source File

SOURCE=.\Event\EC_Per_Supplier_Filter.i
# End Source File
# Begin Source File

SOURCE=.\Event\EC_Prefix_Filter_Builder.i
# End Source File
# Begin Source File

SOURCE=.\Event\EC_ProxyConsumer.i
# End Source File
# Begin Source File

SOURCE=.\Event\EC_ProxySupplier.i
# End Source File
# Begin Source File

SOURCE=.\Event\EC_QOS_Info.i
# End Source File
# Begin Source File

SOURCE=.\Event\EC_Reactive_ConsumerControl.i
# End Source File
# Begin Source File

SOURCE=.\Event\EC_Reactive_Dispatching.i
# End Source File
# Begin Source File

SOURCE=.\Event\EC_Reactive_SupplierControl.i
# End Source File
# Begin Source File

SOURCE=.\Event\EC_Reactive_Timeout_Generator.i
# End Source File
# Begin Source File

SOURCE=.\Event\EC_Scheduling_Strategy.i
# End Source File
# Begin Source File

SOURCE=.\Event\EC_Supplier_Filter.i
# End Source File
# Begin Source File

SOURCE=.\Event\EC_Supplier_Filter_Builder.i
# End Source File
# Begin Source File

SOURCE=.\Event\EC_SupplierAdmin.i
# End Source File
# Begin Source File

SOURCE=.\Event\EC_SupplierControl.i
# End Source File
# Begin Source File

SOURCE=.\Event\EC_Timeout_Filter.i
# End Source File
# Begin Source File

SOURCE=.\Event\EC_Timeout_Generator.i
# End Source File
# Begin Source File

SOURCE=.\Event\EC_Trivial_Supplier_Filter.i
# End Source File
# Begin Source File

SOURCE=.\Event\EC_Type_Filter.i
# End Source File
# Begin Source File

SOURCE=.\Event\ECG_Address_Server_Base.i
# End Source File
# Begin Source File

SOURCE=.\Event\ECG_CDR_Message_Receiver.i
# End Source File
# Begin Source File

SOURCE=.\Event\ECG_CDR_Message_Sender.i
# End Source File
# Begin Source File

SOURCE=.\Event\ECG_Complex_Address_Server.i
# End Source File
# Begin Source File

SOURCE=.\Event\ECG_Mcast_EH.i
# End Source File
# Begin Source File

SOURCE=.\Event\ECG_Mcast_Gateway.i
# End Source File
# Begin Source File

SOURCE=.\Event\ECG_Simple_Address_Server.i
# End Source File
# Begin Source File

SOURCE=.\Event\ECG_Simple_Mcast_EH.i
# End Source File
# Begin Source File

SOURCE=.\Event\ECG_UDP_EH.i
# End Source File
# Begin Source File

SOURCE=.\Event\ECG_UDP_Out_Endpoint.i
# End Source File
# Begin Source File

SOURCE=.\Event\ECG_UDP_Receiver.i
# End Source File
# Begin Source File

SOURCE=.\Event_Utilities.i
# End Source File
# Begin Source File

SOURCE=.\RtecDefaultEventDataC.i
# End Source File
# Begin Source File

SOURCE=.\RtecDefaultEventDataS.i
# End Source File
# Begin Source File

SOURCE=.\RtecDefaultEventDataS_T.i
# End Source File
# Begin Source File

SOURCE=.\RtecEventChannelAdminC.i
# End Source File
# Begin Source File

SOURCE=.\RtecEventChannelAdminS.i
# End Source File
# Begin Source File

SOURCE=.\RtecEventChannelAdminS_T.i
# End Source File
# Begin Source File

SOURCE=.\RtecEventCommC.i
# End Source File
# Begin Source File

SOURCE=.\RtecEventCommS.i
# End Source File
# Begin Source File

SOURCE=.\RtecEventCommS_T.i
# End Source File
# Begin Source File

SOURCE=.\RtecUDPAdminC.i
# End Source File
# Begin Source File

SOURCE=.\RtecUDPAdminS.i
# End Source File
# Begin Source File

SOURCE=.\RtecUDPAdminS_T.i
# End Source File
# End Group
# Begin Group "Template Files"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\RtecDefaultEventDataS_T.cpp

!IF  "$(CFG)" == "RTEvent - Win32 Release"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "RTEvent - Win32 Debug"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "RTEvent - Win32 MFC Debug"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "RTEvent - Win32 MFC Release"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\RtecEventChannelAdminS_T.cpp

!IF  "$(CFG)" == "RTEvent - Win32 Release"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "RTEvent - Win32 Debug"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "RTEvent - Win32 MFC Debug"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "RTEvent - Win32 MFC Release"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\RtecEventCommS_T.cpp

!IF  "$(CFG)" == "RTEvent - Win32 Release"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "RTEvent - Win32 Debug"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "RTEvent - Win32 MFC Debug"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "RTEvent - Win32 MFC Release"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\RtecUDPAdminS_T.cpp

!IF  "$(CFG)" == "RTEvent - Win32 Release"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "RTEvent - Win32 Debug"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "RTEvent - Win32 MFC Debug"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "RTEvent - Win32 MFC Release"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "rc"
# Begin Source File

SOURCE=.\RTEvent.rc
# End Source File
# End Group
# End Target
# End Project
