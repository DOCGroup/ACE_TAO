# Microsoft Developer Studio Project File - Name="RT_Notification LIB" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Static Library" 0x0104

CFG=RT_Notification LIB - Win32 Static Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE run the tool that generated this project file and specify the
!MESSAGE nmake output type.  You can then use the following command:
!MESSAGE
!MESSAGE NMAKE /f "RT_Notification_Static.mak".
!MESSAGE
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE
!MESSAGE NMAKE /f "RT_Notification_Static.mak" CFG="RT_Notification LIB - Win32 Static Debug"
!MESSAGE
!MESSAGE Possible choices for configuration are:
!MESSAGE
!MESSAGE "RT_Notification LIB - Win32 Static Release" (based on "Win32 (x86) Static Library")
!MESSAGE "RT_Notification LIB - Win32 Static Debug" (based on "Win32 (x86) Static Library")
!MESSAGE

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "RT_Notification LIB - Win32 Static Release"

# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Static_Release"
# PROP Intermediate_Dir "Static_Release"
# PROP Target_Dir ""
LINK32=link.exe -lib
# ADD CPP /nologo /G5 /W3 /GX /O2 /MD /Zi /GR /I "..\..\orbsvcs" /I "..\.." /I "..\..\tao" /I "..\..\.." /D NDEBUG=1 /D WIN32=1 /D _WINDOWS=1 /D TAO_AS_STATIC_LIBS=1 /D ACE_AS_STATIC_LIBS=1 /FD /c
# SUBTRACT CPP /YX
# ADD RSC /l 0x409 /d NDEBUG=1 /i "..\..\orbsvcs" /i "..\.." /i "..\..\tao" /i "..\..\.."
BSC32=bscmake.exe
# ADD BSC32 /nologo /o".\TAO_RT_Notification.bsc"
LIB32=link.exe -lib
# ADD LIB32 /nologo /out:".\TAO_RT_Notifications.lib"

!ELSEIF  "$(CFG)" == "RT_Notification LIB - Win32 Static Debug"

# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Static_Debug"
# PROP Intermediate_Dir "Static_Debug"
# PROP Target_Dir ""
LINK32=link.exe -lib
# ADD CPP /nologo /G5 /W3 /Gm /GX /Zi /Od /GR /Gy /MDd /I "..\..\orbsvcs" /I "..\.." /I "..\..\tao" /I "..\..\.." /D _DEBUG=1 /D WIN32=1 /D _WINDOWS=1 /D TAO_AS_STATIC_LIBS=1 /D ACE_AS_STATIC_LIBS=1 /FD /c
# SUBTRACT CPP /Fr /YX
# ADD RSC /l 0x409 /d _DEBUG=1 /i "..\..\orbsvcs" /i "..\.." /i "..\..\tao" /i "..\..\.."
BSC32=bscmake.exe
# ADD BSC32 /nologo /o".\TAO_RT_Notification.bsc"
LIB32=link.exe -lib
# ADD LIB32 /nologo /out:".\TAO_RT_Notificationsd.lib"

!ENDIF

# Begin Target

# Name "RT_Notification LIB - Win32 Static Release"
# Name "RT_Notification LIB - Win32 Static Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;cxx;c"
# Begin Source File

SOURCE=.\Notify\RT_Builder.cpp
# End Source File
# Begin Source File

SOURCE=.\Notify\RT_Factory.cpp
# End Source File
# Begin Source File

SOURCE=.\Notify\RT_Notify_Service.cpp
# End Source File
# Begin Source File

SOURCE=.\Notify\RT_POA_Helper.cpp
# End Source File
# Begin Source File

SOURCE=.\Notify\RT_Properties.cpp
# End Source File
# Begin Source File

SOURCE=.\Notify\Structured\RT_StructuredProxyPushSupplier.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hh"
# Begin Source File

SOURCE=.\Channel_Clients_T.h
# End Source File
# Begin Source File

SOURCE=.\CosEventChannelAdminS_T.h
# End Source File
# Begin Source File

SOURCE=.\CosEventCommS_T.h
# End Source File
# Begin Source File

SOURCE=.\CosNamingS_T.h
# End Source File
# Begin Source File

SOURCE=.\CosNotificationS_T.h
# End Source File
# Begin Source File

SOURCE=.\CosNotifyChannelAdminS_T.h
# End Source File
# Begin Source File

SOURCE=.\CosNotifyCommS_T.h
# End Source File
# Begin Source File

SOURCE=.\CosNotifyFilterS_T.h
# End Source File
# Begin Source File

SOURCE=.\Event_ForwarderS_T.h
# End Source File
# Begin Source File

SOURCE=.\Notify\RT_Builder.h
# End Source File
# Begin Source File

SOURCE=.\Notify\RT_Factory.h
# End Source File
# Begin Source File

SOURCE=.\Notify\RT_Notify_Service.h
# End Source File
# Begin Source File

SOURCE=.\Notify\RT_POA_Helper.h
# End Source File
# Begin Source File

SOURCE=.\Notify\RT_Properties.h
# End Source File
# Begin Source File

SOURCE=.\Notify\Structured\RT_StructuredProxyPushSupplier.h
# End Source File
# Begin Source File

SOURCE=.\NotifyExtS_T.h
# End Source File
# Begin Source File

SOURCE=.\RtecBaseS_T.h
# End Source File
# Begin Source File

SOURCE=.\TimeBaseS_T.h
# End Source File
# End Group
# Begin Group "Inline Files"

# PROP Default_Filter "i;inl"
# Begin Source File

SOURCE=.\Channel_Clients_T.i
# End Source File
# Begin Source File

SOURCE=.\CosEventChannelAdminS_T.i
# End Source File
# Begin Source File

SOURCE=.\CosEventCommS_T.i
# End Source File
# Begin Source File

SOURCE=.\CosNamingS_T.i
# End Source File
# Begin Source File

SOURCE=.\CosNotificationS_T.i
# End Source File
# Begin Source File

SOURCE=.\CosNotifyChannelAdminS_T.i
# End Source File
# Begin Source File

SOURCE=.\CosNotifyCommS_T.i
# End Source File
# Begin Source File

SOURCE=.\CosNotifyFilterS_T.i
# End Source File
# Begin Source File

SOURCE=.\Event_ForwarderS_T.i
# End Source File
# Begin Source File

SOURCE=.\Notify\RT_Builder.inl
# End Source File
# Begin Source File

SOURCE=.\Notify\RT_Factory.inl
# End Source File
# Begin Source File

SOURCE=.\Notify\RT_Notify_Service.inl
# End Source File
# Begin Source File

SOURCE=.\Notify\RT_POA_Helper.inl
# End Source File
# Begin Source File

SOURCE=.\Notify\RT_Properties.inl
# End Source File
# Begin Source File

SOURCE=.\Notify\Structured\RT_StructuredProxyPushSupplier.inl
# End Source File
# Begin Source File

SOURCE=.\NotifyExtS_T.i
# End Source File
# Begin Source File

SOURCE=.\RtecBaseS_T.i
# End Source File
# Begin Source File

SOURCE=.\TimeBaseS_T.i
# End Source File
# End Group
# Begin Group "Template Files"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\Channel_Clients_T.cpp
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\CosEventChannelAdminS_T.cpp
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\CosEventCommS_T.cpp
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\CosNamingS_T.cpp
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\CosNotificationS_T.cpp
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\CosNotifyChannelAdminS_T.cpp
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\CosNotifyCommS_T.cpp
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\CosNotifyFilterS_T.cpp
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\Event_ForwarderS_T.cpp
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\NotifyExtS_T.cpp
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\RtecBaseS_T.cpp
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\TimeBaseS_T.cpp
# PROP Exclude_From_Build 1
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "rc;ico;cur;bmp;dlg;rc2;rct;bin;cnt;rtf;gif;jpg;jpeg;jpe"
# Begin Source File

SOURCE=.\RT_Notification.rc
# End Source File
# End Group
# End Target
# End Project
