# Microsoft Developer Studio Project File - Name="Gateway" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 5.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Dynamic-Link Library" 0x0102

CFG=Gateway - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "Gateway.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "Gateway.mak" CFG="Gateway - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "Gateway - Win32 Debug" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE 

# Begin Project
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe
# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir ".\Gateway\Debug"
# PROP BASE Intermediate_Dir ".\Gateway\Debug"
# PROP BASE Target_Dir ".\Gateway"
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "."
# PROP Intermediate_Dir ".\Gateway_Debug"
# PROP Target_Dir ".\Gateway"
# ADD BASE CPP /nologo /MTd /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /YX /c
# ADD CPP /nologo /MDd /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /YX /FD /c
# ADD BASE MTL /nologo /D "_DEBUG" /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /dll /debug /machine:I386
# ADD LINK32 aced.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /dll /debug /machine:I386
# Begin Target

# Name "Gateway - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;hpj;bat;for;f90"
# Begin Source File

SOURCE=.\Concrete_Proxy_Handlers.cpp
# End Source File
# Begin Source File

SOURCE=.\Config_Files.cpp
# End Source File
# Begin Source File

SOURCE=.\Event_Channel.cpp
# End Source File
# Begin Source File

SOURCE=.\Event_Forwarding_Discriminator.cpp
# End Source File
# Begin Source File

SOURCE=.\Gateway.cpp
# End Source File
# Begin Source File

SOURCE=.\Proxy_Handler.cpp
# End Source File
# Begin Source File

SOURCE=.\Proxy_Handler_Acceptor.cpp
# End Source File
# Begin Source File

SOURCE=.\Proxy_Handler_Connector.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl;fi;fd"
# Begin Source File

SOURCE=.\Concrete_Proxy_Handlers.h
# End Source File
# Begin Source File

SOURCE=.\Config_Files.h
# End Source File
# Begin Source File

SOURCE=.\Consumer_Dispatch_Set.h
# End Source File
# Begin Source File

SOURCE=.\Event.h
# End Source File
# Begin Source File

SOURCE=.\Event_Channel.h
# End Source File
# Begin Source File

SOURCE=.\Event_Forwarding_Discriminator.h
# End Source File
# Begin Source File

SOURCE=.\File_Parser.h
# End Source File
# Begin Source File

SOURCE=.\Gateway.h
# End Source File
# Begin Source File

SOURCE=.\Proxy_Handler.h
# End Source File
# Begin Source File

SOURCE=.\Proxy_Handler_Acceptor.h
# End Source File
# Begin Source File

SOURCE=.\Proxy_Handler_Connector.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;cnt;rtf;gif;jpg;jpeg;jpe"
# End Group
# Begin Source File

SOURCE=.\File_Parser.cpp
# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1
# End Source File
# End Target
# End Project
