# Microsoft Developer Studio Project File - Name="Messaging_Static" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Static Library" 0x0104

CFG=Messaging_Static - Win32 Static Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "TAO_Messaging_Static.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "TAO_Messaging_Static.mak" CFG="Messaging_Static - Win32 Static Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "Messaging_Static - Win32 Static Release" (based on "Win32 (x86) Static Library")
!MESSAGE "Messaging_Static - Win32 Static Debug" (based on "Win32 (x86) Static Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "Messaging_Static - Win32 Static Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir ""
# PROP Intermediate_Dir "LIB\Release\Messaging"
# PROP Target_Dir ""
MTL=midl.exe
LINK32=link.exe -lib
# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_MBCS" /D "_LIB" /YX /FD /c
# ADD CPP /nologo /MD /W3 /GX /Zi /O2 /I "../" /I "../../" /I "../../../" /D "_WINDOWS" /D "_CONSOLE" /D "NDEBUG" /D "WIN32" /D "TAO_AS_STATIC_LIBS" /D "ACE_AS_STATIC_LIBS" /FD /c
# SUBTRACT CPP /YX
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo /out:"TAO_Messagings.lib"

!ELSEIF  "$(CFG)" == "Messaging_Static - Win32 Static Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir ""
# PROP Intermediate_Dir "LIB\Debug\Messaging"
# PROP Target_Dir ""
MTL=midl.exe
LINK32=link.exe -lib
# ADD BASE CPP /nologo /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_MBCS" /D "_LIB" /YX /FD /c
# ADD CPP /nologo /MDd /W3 /Gm /GX /Zi /Od /I "../" /I "../../" /I "../../../" /D "_WINDOWS" /D "_CONSOLE" /D "_DEBUG" /D "WIN32" /D "ACE_AS_STATIC_LIBS" /D "TAO_AS_STATIC_LIBS" /FD /c
# SUBTRACT CPP /YX
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo /out:"TAO_Messagingsd.lib"

!ENDIF 

# Begin Target

# Name "Messaging_Static - Win32 Static Release"
# Name "Messaging_Static - Win32 Static Debug"
# Begin Group "Template Files"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\MessagingS_T.cpp
# PROP Exclude_From_Build 1
# End Source File
# End Group
# Begin Group "Inline Files"

# PROP Default_Filter "i,inl"
# Begin Source File

SOURCE=.\Asynch_Reply_Dispatcher.i
# End Source File
# Begin Source File

SOURCE=.\Messaging_No_ImplC.i
# End Source File
# Begin Source File

SOURCE=.\Messaging_Policy_i.i
# End Source File
# Begin Source File

SOURCE=.\Messaging_RT_PolicyC.i
# End Source File
# Begin Source File

SOURCE=.\Messaging_SyncScope_PolicyC.i
# End Source File
# Begin Source File

SOURCE=.\MessagingC.i
# End Source File
# Begin Source File

SOURCE=.\MessagingS.i
# End Source File
# Begin Source File

SOURCE=.\MessagingS_T.i
# End Source File
# Begin Source File

SOURCE=.\TAO_ExtC.i
# End Source File
# Begin Source File

SOURCE=.\Twoway_Asynch_Invocation.i
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h"
# Begin Source File

SOURCE=.\AMI_ClientRequestInfo_i.h
# End Source File
# Begin Source File

SOURCE=.\Asynch_Reply_Dispatcher.h
# End Source File
# Begin Source File

SOURCE=.\Asynch_Timeout_Handler.h
# End Source File
# Begin Source File

SOURCE=.\Connection_Timeout_Policy_i.h
# End Source File
# Begin Source File

SOURCE=.\Messaging.h
# End Source File
# Begin Source File

SOURCE=.\messaging_export.h
# End Source File
# Begin Source File

SOURCE=.\Messaging_No_ImplC.h
# End Source File
# Begin Source File

SOURCE=.\Messaging_ORBInitializer.h
# End Source File
# Begin Source File

SOURCE=.\Messaging_Policy_i.h
# End Source File
# Begin Source File

SOURCE=.\Messaging_PolicyFactory.h
# End Source File
# Begin Source File

SOURCE=.\Messaging_RT_PolicyC.h
# End Source File
# Begin Source File

SOURCE=.\Messaging_SyncScope_PolicyC.h
# End Source File
# Begin Source File

SOURCE=.\MessagingC.h
# End Source File
# Begin Source File

SOURCE=.\MessagingS.h
# End Source File
# Begin Source File

SOURCE=.\MessagingS_T.h
# End Source File
# Begin Source File

SOURCE=.\TAO_ExtC.h
# End Source File
# Begin Source File

SOURCE=.\Twoway_Asynch_Invocation.h
# End Source File
# End Group
# Begin Group "Source Files"

# PROP Default_Filter "cpp"
# Begin Source File

SOURCE=.\AMI_ClientRequestInfo_i.cpp
# End Source File
# Begin Source File

SOURCE=.\Asynch_Reply_Dispatcher.cpp
# End Source File
# Begin Source File

SOURCE=.\Asynch_Timeout_Handler.cpp
# End Source File
# Begin Source File

SOURCE=.\Connection_Timeout_Policy_i.cpp
# End Source File
# Begin Source File

SOURCE=.\Messaging.cpp
# End Source File
# Begin Source File

SOURCE=.\Messaging_No_ImplC.cpp
# End Source File
# Begin Source File

SOURCE=.\Messaging_ORBInitializer.cpp
# End Source File
# Begin Source File

SOURCE=.\Messaging_Policy_i.cpp
# End Source File
# Begin Source File

SOURCE=.\Messaging_PolicyFactory.cpp
# End Source File
# Begin Source File

SOURCE=.\Messaging_RT_PolicyC.cpp
# End Source File
# Begin Source File

SOURCE=.\Messaging_SyncScope_PolicyC.cpp
# End Source File
# Begin Source File

SOURCE=.\MessagingC.cpp
# End Source File
# Begin Source File

SOURCE=.\MessagingS.cpp
# End Source File
# Begin Source File

SOURCE=.\TAO_ExtC.cpp
# End Source File
# Begin Source File

SOURCE=.\Twoway_Asynch_Invocation.cpp
# End Source File
# End Group
# End Target
# End Project
