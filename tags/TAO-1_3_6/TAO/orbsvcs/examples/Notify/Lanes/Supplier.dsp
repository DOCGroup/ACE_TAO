# Microsoft Developer Studio Project File - Name="Lanes_Supplier" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Console Application" 0x0103

CFG=Lanes_Supplier - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "Supplier.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "Supplier.mak" CFG="Lanes_Supplier - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "Lanes_Supplier - Win32 Release" (based on "Win32 (x86) Console Application")
!MESSAGE "Lanes_Supplier - Win32 Debug" (based on "Win32 (x86) Console Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "Lanes_Supplier - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /D "_MBCS" /YX /FD /c
# ADD CPP /nologo /MD /W3 /Gm /GR /GX /Zi /Od /I "..\..\..\..\\" /I "..\..\..\..\..\\" /I "..\..\..\\" /D "WIN32" /D "_CONSOLE" /Fo"" /Fd"" /FD /c
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /machine:I386
# ADD LINK32 TAO_RTPortableServer.lib TAO_RTCORBA.lib ace.lib TAO.lib TAO_PortableServer.lib TAO_Svc_Utils.lib TAO_ETCL.lib TAO_RT_Notification.lib TAO_CosNotification.lib TAO_CosNaming.lib /nologo /subsystem:console /incremental:yes /pdb:"Lanes_Supplier.pdb" /map /debug /machine:I386 /out:"Lanes_Supplier.exe" /pdbtype:sept /libpath:"..\..\..\orbsvcs" /libpath:"..\..\..\orbsvcs\ETCL" /libpath:"..\..\..\..\tao" /libpath:"..\..\..\..\tao\PortableServer" /libpath:"..\..\..\..\..\ace" /libpath:"..\..\..\..\tao\RTCORBA" /libpath:"..\..\..\..\tao\RTPortableServer"
# SUBTRACT LINK32 /pdb:none

!ELSEIF  "$(CFG)" == "Lanes_Supplier - Win32 Debug"

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
# ADD BASE CPP /nologo /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /D "_MBCS" /YX /FD /c
# ADD CPP /nologo /MDd /W3 /Gm /GR /GX /Zi /Od /I "\\" /I "..\..\..\..\\" /I "..\..\..\..\..\\" /I "..\..\..\\" /D "_DEBUG" /D "WIN32" /D "_CONSOLE" /FD /c
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /debug /machine:I386 /pdbtype:sept
# ADD LINK32 TAO_RTPortableServerd.lib TAO_RTCORBAd.lib aced.lib TAOd.lib TAO_PortableServerd.lib TAO_Svc_Utilsd.lib TAO_ETCLd.lib TAO_RT_Notificationd.lib TAO_CosNotificationd.lib TAO_CosNamingd.lib /nologo /subsystem:console /map /debug /machine:I386 /pdbtype:sept /libpath:"..\..\..\orbsvcs" /libpath:"..\..\..\orbsvcs\ETCL" /libpath:"..\..\..\..\tao" /libpath:"..\..\..\..\tao\PortableServer" /libpath:"..\..\..\..\..\ace" /libpath:"..\..\..\..\tao\RTCORBA" /libpath:"..\..\..\..\tao\RTPortableServer"
# SUBTRACT LINK32 /pdb:none /nodefaultlib

!ENDIF 

# Begin Target

# Name "Lanes_Supplier - Win32 Release"
# Name "Lanes_Supplier - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\ORB_Objects.cpp
# End Source File
# Begin Source File

SOURCE=.\ORB_Run_Task.cpp
# End Source File
# Begin Source File

SOURCE=.\Supplier.cpp
# End Source File
# Begin Source File

SOURCE=.\Supplier_Client.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\Lanes_Supplier.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter ""
# End Group
# End Target
# End Project
