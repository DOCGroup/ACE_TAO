# Microsoft Developer Studio Project File - Name="TAO_Strategies" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Dynamic-Link Library" 0x0102

CFG=TAO_Strategies - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "TAO_Strategies.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "TAO_Strategies.mak" CFG="TAO_Strategies - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "TAO_Strategies - Win32 Release" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "TAO_Strategies - Win32 Debug" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "TAO_Strategies - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir ""
# PROP Intermediate_Dir "DLL\Release\Strategies"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "TAO_STRATEGIES_EXPORTS" /YX /FD /c
# ADD CPP /nologo /MD /W3 /GX /Zi /O2 /I "../" /I "../../" /I "../../../" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "TAO_STRATEGIES_BUILD_DLL" /FD /c
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
# ADD LINK32 TAO.lib ace.lib /nologo /dll /map /debug /machine:I386 /out:"..\..\..\bin\TAO_Strategies.dll" /libpath:"..\..\tao" /libpath:"..\..\..\ace"

!ELSEIF  "$(CFG)" == "TAO_Strategies - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir ""
# PROP Intermediate_Dir "DLL\Debug\Strategies"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MTd /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "TAO_STRATEGIES_EXPORTS" /YX /FD /c
# ADD CPP /nologo /MDd /W3 /Gm /GX /Zi /Od /I "../" /I "../../" /I "../../../" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "TAO_STRATEGIES_BUILD_DLL" /FD /c
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
# ADD LINK32 TAOd.lib aced.lib /nologo /dll /debug /machine:I386 /out:"..\..\..\bin\TAO_Strategiesd.dll" /pdbtype:sept /libpath:"..\..\tao" /libpath:"..\..\..\ace"

!ENDIF 

# Begin Target

# Name "TAO_Strategies - Win32 Release"
# Name "TAO_Strategies - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\advanced_resource.cpp
# End Source File
# Begin Source File

SOURCE=.\DIOP_Acceptor.cpp
# End Source File
# Begin Source File

SOURCE=.\DIOP_Connection_Handler.cpp
# End Source File
# Begin Source File

SOURCE=.\DIOP_Connector.cpp
# End Source File
# Begin Source File

SOURCE=.\DIOP_Endpoint.cpp
# End Source File
# Begin Source File

SOURCE=.\DIOP_Factory.cpp
# End Source File
# Begin Source File

SOURCE=.\DIOP_Profile.cpp
# End Source File
# Begin Source File

SOURCE=.\DIOP_Transport.cpp
# End Source File
# Begin Source File

SOURCE=.\FIFO_Connection_Purging_Strategy.cpp
# End Source File
# Begin Source File

SOURCE=.\LF_Strategy_Null.cpp
# End Source File
# Begin Source File

SOURCE=.\LFU_Connection_Purging_Strategy.cpp
# End Source File
# Begin Source File

SOURCE=.\NULL_Connection_Purging_Strategy.cpp
# End Source File
# Begin Source File

SOURCE=.\SHMIOP_Acceptor.cpp
# End Source File
# Begin Source File

SOURCE=.\SHMIOP_Connection_Handler.cpp
# End Source File
# Begin Source File

SOURCE=.\SHMIOP_Connector.cpp
# End Source File
# Begin Source File

SOURCE=.\SHMIOP_Endpoint.cpp
# End Source File
# Begin Source File

SOURCE=.\SHMIOP_Factory.cpp
# End Source File
# Begin Source File

SOURCE=.\SHMIOP_Profile.cpp
# End Source File
# Begin Source File

SOURCE=.\SHMIOP_Transport.cpp
# End Source File
# Begin Source File

SOURCE=.\UIOP_Acceptor.cpp
# End Source File
# Begin Source File

SOURCE=.\UIOP_Connection_Handler.cpp
# End Source File
# Begin Source File

SOURCE=.\UIOP_Connector.cpp
# End Source File
# Begin Source File

SOURCE=.\UIOP_Endpoint.cpp
# End Source File
# Begin Source File

SOURCE=.\uiop_endpoints.cpp
# End Source File
# Begin Source File

SOURCE=.\UIOP_Factory.cpp
# End Source File
# Begin Source File

SOURCE=.\UIOP_Lite_Factory.cpp
# End Source File
# Begin Source File

SOURCE=.\UIOP_Profile.cpp
# End Source File
# Begin Source File

SOURCE=.\UIOP_Transport.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\advanced_resource.h
# End Source File
# Begin Source File

SOURCE=.\DIOP_Acceptor.h
# End Source File
# Begin Source File

SOURCE=.\DIOP_Connection_Handler.h
# End Source File
# Begin Source File

SOURCE=.\DIOP_Connector.h
# End Source File
# Begin Source File

SOURCE=.\DIOP_Endpoint.h
# End Source File
# Begin Source File

SOURCE=.\DIOP_Factory.h
# End Source File
# Begin Source File

SOURCE=.\DIOP_Profile.h
# End Source File
# Begin Source File

SOURCE=.\DIOP_Transport.h
# End Source File
# Begin Source File

SOURCE=.\LF_Strategy_Null.h
# End Source File
# Begin Source File

SOURCE=.\LF_Strategy_Null.inl
# End Source File
# Begin Source File

SOURCE=.\SHMIOP_Acceptor.h
# End Source File
# Begin Source File

SOURCE=.\SHMIOP_Connect.h
# End Source File
# Begin Source File

SOURCE=.\SHMIOP_Connection_Handler.h
# End Source File
# Begin Source File

SOURCE=.\SHMIOP_Connector.h
# End Source File
# Begin Source File

SOURCE=.\SHMIOP_Endpoint.h
# End Source File
# Begin Source File

SOURCE=.\SHMIOP_Factory.h
# End Source File
# Begin Source File

SOURCE=.\SHMIOP_Profile.h
# End Source File
# Begin Source File

SOURCE=.\SHMIOP_Transport.h
# End Source File
# Begin Source File

SOURCE=.\strategies_export.h
# End Source File
# Begin Source File

SOURCE=.\UIOP_Acceptor.h
# End Source File
# Begin Source File

SOURCE=.\UIOP_Connect.h
# End Source File
# Begin Source File

SOURCE=.\UIOP_Connection_Handler.h
# End Source File
# Begin Source File

SOURCE=.\UIOP_Connector.h
# End Source File
# Begin Source File

SOURCE=.\UIOP_Endpoint.h
# End Source File
# Begin Source File

SOURCE=.\uiop_endpoints.h
# End Source File
# Begin Source File

SOURCE=.\UIOP_Factory.h
# End Source File
# Begin Source File

SOURCE=.\UIOP_Lite_Factory.h
# End Source File
# Begin Source File

SOURCE=.\UIOP_Profile.h
# End Source File
# Begin Source File

SOURCE=.\UIOP_Transport.h
# End Source File
# End Group
# Begin Group "Inline Files"

# PROP Default_Filter ".i"
# Begin Source File

SOURCE=.\advanced_resource.i
# End Source File
# Begin Source File

SOURCE=.\DIOP_Acceptor.i
# End Source File
# Begin Source File

SOURCE=.\DIOP_Connection_Handler.i
# End Source File
# Begin Source File

SOURCE=.\DIOP_Endpoint.i
# End Source File
# Begin Source File

SOURCE=.\DIOP_Profile.i
# End Source File
# Begin Source File

SOURCE=.\DIOP_Transport.i
# End Source File
# Begin Source File

SOURCE=.\SHMIOP_Acceptor.i
# End Source File
# Begin Source File

SOURCE=.\SHMIOP_Connect.i
# End Source File
# Begin Source File

SOURCE=.\SHMIOP_Connection_Handler.inl
# End Source File
# Begin Source File

SOURCE=.\SHMIOP_Endpoint.i
# End Source File
# Begin Source File

SOURCE=.\SHMIOP_Profile.i
# End Source File
# Begin Source File

SOURCE=.\SHMIOP_Transport.i
# End Source File
# Begin Source File

SOURCE=.\UIOP_Connect.i
# End Source File
# Begin Source File

SOURCE=.\UIOP_Connection_Handler.inl
# End Source File
# Begin Source File

SOURCE=.\UIOP_Endpoint.i
# End Source File
# Begin Source File

SOURCE=.\uiop_endpoints.i
# End Source File
# Begin Source File

SOURCE=.\UIOP_Profile.i
# End Source File
# Begin Source File

SOURCE=.\UIOP_Transport.i
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# End Group
# End Target
# End Project
