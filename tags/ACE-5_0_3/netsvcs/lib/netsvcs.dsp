# Microsoft Developer Studio Project File - Name="NETSVCS Library" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Dynamic-Link Library" 0x0102
# TARGTYPE "Win32 (ALPHA) Dynamic-Link Library" 0x0602

CFG=NETSVCS Library - Win32 Alpha Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "netsvcs.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "netsvcs.mak" CFG="NETSVCS Library - Win32 Alpha Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "NETSVCS Library - Win32 Release" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "NETSVCS Library - Win32 Debug" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "NETSVCS Library - Win32 Alpha Debug" (based on "Win32 (ALPHA) Dynamic-Link Library")
!MESSAGE "NETSVCS Library - Win32 Alpha Release" (based on "Win32 (ALPHA) Dynamic-Link Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""

!IF  "$(CFG)" == "NETSVCS Library - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir ".\netsvcs\Release"
# PROP BASE Intermediate_Dir ".\netsvcs\Release"
# PROP BASE Target_Dir ".\netsvcs"
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir ""
# PROP Intermediate_Dir "Release"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ".\netsvcs"
CPP=cl.exe
# ADD BASE CPP /nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /YX /c
# ADD CPP /nologo /MD /W3 /GX /O2 /I "..\..\\" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /FD /c
# SUBTRACT CPP /YX
MTL=midl.exe
# ADD BASE MTL /nologo /D "NDEBUG" /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
RSC=rc.exe
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /dll /machine:I386
# ADD LINK32 ace.lib /nologo /subsystem:windows /dll /machine:I386 /out:"..\..\bin\netsvcs.dll" /libpath:"..\..\ace"

!ELSEIF  "$(CFG)" == "NETSVCS Library - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir ".\netsvcs\Debug"
# PROP BASE Intermediate_Dir ".\netsvcs\Debug"
# PROP BASE Target_Dir ".\netsvcs"
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir ""
# PROP Intermediate_Dir "Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ".\netsvcs"
CPP=cl.exe
# ADD BASE CPP /nologo /MTd /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /YX /c
# ADD CPP /nologo /MDd /W3 /Gm /GX /Zi /Od /I "..\..\\" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /FD /c
# SUBTRACT CPP /YX
MTL=midl.exe
# ADD BASE MTL /nologo /D "_DEBUG" /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
RSC=rc.exe
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /dll /debug /machine:I386
# ADD LINK32 aced.lib /nologo /subsystem:windows /dll /debug /machine:I386 /out:"..\..\bin\netsvcsd.dll" /libpath:"..\..\ace"

!ELSEIF  "$(CFG)" == "NETSVCS Library - Win32 Alpha Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "netsvcs\Alpha Debug"
# PROP BASE Intermediate_Dir "netsvcs\Alpha Debug"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir "netsvcs"
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir ""
# PROP Intermediate_Dir "Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir "netsvcs"
CPP=cl.exe
# ADD BASE CPP /nologo /Gt0 /W3 /GX /Zi /Od /I "..\..\\ /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /YX /FD /MTd /c
# ADD CPP /nologo /Gt0 /W3 /GX /Zi /Od /I "..\..\\ /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /YX /FD /MDd /c
MTL=midl.exe
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /o "NUL" /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /o "NUL" /win32
RSC=rc.exe
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 aced.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib /nologo /subsystem:windows /dll /debug /machine:ALPHA /libpath:"..\..\ace"
# ADD LINK32 aced.lib /nologo /subsystem:windows /dll /debug /machine:ALPHA /libpath:"..\..\ace"

!ELSEIF  "$(CFG)" == "NETSVCS Library - Win32 Alpha Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "netsvcs\Alpha Release"
# PROP BASE Intermediate_Dir "netsvcs\Alpha Release"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir "netsvcs"
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir ""
# PROP Intermediate_Dir "Release"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir "netsvcs"
CPP=cl.exe
# ADD BASE CPP /nologo /MT /Gt0 /W3 /GX /O2 /I "..\..\\ /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /YX /FD /c
# ADD CPP /nologo /MD /Gt0 /W3 /GX /O2 /I "..\..\\ /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /YX /FD /c
MTL=midl.exe
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /o "NUL" /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /o "NUL" /win32
RSC=rc.exe
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 ace.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib /nologo /subsystem:windows /dll /machine:ALPHA /libpath:"..\..\ace"
# ADD LINK32 ace.lib /nologo /subsystem:windows /dll /machine:ALPHA /libpath:"..\..\ace"

!ENDIF 

# Begin Target

# Name "NETSVCS Library - Win32 Release"
# Name "NETSVCS Library - Win32 Debug"
# Name "NETSVCS Library - Win32 Alpha Debug"
# Name "NETSVCS Library - Win32 Alpha Release"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;hpj;bat;for;f90"
# Begin Source File

SOURCE=.\Client_Logging_Handler.cpp

!IF  "$(CFG)" == "NETSVCS Library - Win32 Release"

!ELSEIF  "$(CFG)" == "NETSVCS Library - Win32 Debug"

!ELSEIF  "$(CFG)" == "NETSVCS Library - Win32 Alpha Debug"

!ELSEIF  "$(CFG)" == "NETSVCS Library - Win32 Alpha Release"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Logging_Strategy.cpp

!IF  "$(CFG)" == "NETSVCS Library - Win32 Release"

!ELSEIF  "$(CFG)" == "NETSVCS Library - Win32 Debug"

!ELSEIF  "$(CFG)" == "NETSVCS Library - Win32 Alpha Debug"

!ELSEIF  "$(CFG)" == "NETSVCS Library - Win32 Alpha Release"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Name_Handler.cpp

!IF  "$(CFG)" == "NETSVCS Library - Win32 Release"

!ELSEIF  "$(CFG)" == "NETSVCS Library - Win32 Debug"

!ELSEIF  "$(CFG)" == "NETSVCS Library - Win32 Alpha Debug"

!ELSEIF  "$(CFG)" == "NETSVCS Library - Win32 Alpha Release"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Server_Logging_Handler.cpp

!IF  "$(CFG)" == "NETSVCS Library - Win32 Release"

!ELSEIF  "$(CFG)" == "NETSVCS Library - Win32 Debug"

!ELSEIF  "$(CFG)" == "NETSVCS Library - Win32 Alpha Debug"

!ELSEIF  "$(CFG)" == "NETSVCS Library - Win32 Alpha Release"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Token_Handler.cpp

!IF  "$(CFG)" == "NETSVCS Library - Win32 Release"

!ELSEIF  "$(CFG)" == "NETSVCS Library - Win32 Debug"

!ELSEIF  "$(CFG)" == "NETSVCS Library - Win32 Alpha Debug"

!ELSEIF  "$(CFG)" == "NETSVCS Library - Win32 Alpha Release"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\TS_Clerk_Handler.cpp

!IF  "$(CFG)" == "NETSVCS Library - Win32 Release"

!ELSEIF  "$(CFG)" == "NETSVCS Library - Win32 Debug"

!ELSEIF  "$(CFG)" == "NETSVCS Library - Win32 Alpha Debug"

!ELSEIF  "$(CFG)" == "NETSVCS Library - Win32 Alpha Release"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\TS_Server_Handler.cpp

!IF  "$(CFG)" == "NETSVCS Library - Win32 Release"

!ELSEIF  "$(CFG)" == "NETSVCS Library - Win32 Debug"

!ELSEIF  "$(CFG)" == "NETSVCS Library - Win32 Alpha Debug"

!ELSEIF  "$(CFG)" == "NETSVCS Library - Win32 Alpha Release"

!ENDIF 

# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl;fi;fd"
# Begin Source File

SOURCE=.\Client_Logging_Handler.h
# End Source File
# Begin Source File

SOURCE=.\Logging_Strategy.h
# End Source File
# Begin Source File

SOURCE=.\Name_Handler.h
# End Source File
# Begin Source File

SOURCE=.\Server_Logging_Handler.h
# End Source File
# Begin Source File

SOURCE=.\Token_Handler.h
# End Source File
# Begin Source File

SOURCE=.\TS_Clerk_Handler.h
# End Source File
# Begin Source File

SOURCE=.\TS_Server_Handler.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;cnt;rtf;gif;jpg;jpeg;jpe"
# End Group
# End Target
# End Project
