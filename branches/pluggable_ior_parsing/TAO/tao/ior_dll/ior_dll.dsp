# Microsoft Developer Studio Project File - Name="TAO IOR DLL DLL" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Dynamic-Link Library" 0x0102
# TARGTYPE "Win32 (ALPHA) Dynamic-Link Library" 0x0602

CFG=TAO IOR DLL DLL - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "ior_dll.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "ior_dll.mak" CFG="TAO IOR DLL DLL - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "TAO IOR DLL DLL - Win32 Alpha Release" (based on "Win32 (ALPHA) Dynamic-Link Library")
!MESSAGE "TAO IOR DLL DLL - Win32 Alpha Debug" (based on "Win32 (ALPHA) Dynamic-Link Library")
!MESSAGE "TAO IOR DLL DLL - Win32 MFC Release" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "TAO IOR DLL DLL - Win32 MFC Debug" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "TAO IOR DLL DLL - Win32 Release" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "TAO IOR DLL DLL - Win32 Debug" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath "Desktop"

!IF  "$(CFG)" == "TAO IOR DLL DLL - Win32 Alpha Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir ""
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir ""
# PROP Intermediate_Dir "Release"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
CPP=cl.exe
# ADD BASE CPP /Gt0 /I "../" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "TAO_BUILD_DLL" /FD nologo MT Gt0 W3 GX O2 I "../../" /c
# ADD CPP /nologo /MD /Gt0 /W3 /GX /O2 /I "../../" /I "../" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "TAO_BUILD_DLL" /FD /c
MTL=midl.exe
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /o /win32 "NUL"
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /o /win32 "NUL"
RSC=rc.exe
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 ace.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib /nologo /subsystem:windows /dll /machine:ALPHA /out:"..\..\bin\TAO_IOR_DLL.dll" /libpath:"..\..\ace"
# ADD LINK32 ace.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib /nologo /subsystem:windows /dll /machine:ALPHA /out:"..\..\bin\TAO_IOR_DLL.dll" /libpath:"..\..\ace"

!ELSEIF  "$(CFG)" == "TAO IOR DLL DLL - Win32 Alpha Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir ""
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir ""
# PROP Intermediate_Dir "Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
CPP=cl.exe
# ADD BASE CPP /Gt0 /I "../" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "TAO_BUILD_DLL" /FD /MTd nologo Gt0 W3 GX Zi Od Gy I "../../" /c
# ADD CPP /nologo /Gt0 /W3 /GX /Zi /Od /Gy /I "../../" /I "../" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "TAO_BUILD_DLL" /FD /MDd /c
MTL=midl.exe
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /o /win32 "NUL"
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /o /win32 "NUL"
RSC=rc.exe
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 aced.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib /nologo /subsystem:windows /dll /debug /machine:ALPHA /out:"..\..\bin\TAO_IOR_DLLd.dll" /libpath:"..\..\ace"
# ADD LINK32 aced.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib /nologo /subsystem:windows /dll /debug /machine:ALPHA /out:"..\..\bin\TAO_IOR_DLLd.dll" /libpath:"..\..\ace"

!ELSEIF  "$(CFG)" == "TAO IOR DLL DLL - Win32 MFC Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "TAO_DLL___Win32_MFC_Release"
# PROP BASE Intermediate_Dir "TAO_DLL___Win32_MFC_Release"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir ""
# PROP Intermediate_Dir "DLL\MFC_Release"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
CPP=cl.exe
# ADD BASE CPP /nologo /MD /W3 /GX /O2 /I "../../" /I "../" /D "NDEBUG" /D "WIN32" /D "_WINDOWS" /D "TAO_BUILD_DLL" /FD /c
# SUBTRACT BASE CPP /YX
# ADD CPP /nologo /MD /W3 /GX /O2 /I "../../" /I "../" /D "NDEBUG" /D "WIN32" /D "_WINDOWS" /D "TAO_BUILD_DLL" /D ACE_HAS_MFC=1 /D ACE_DOESNT_INSTANTIATE_NONSTATIC_OBJECT_MANAGER=1 /FD /c
# SUBTRACT CPP /YX
MTL=midl.exe
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
RSC=rc.exe
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 ace.lib /nologo /subsystem:windows /dll /machine:I386 /out:"..\..\bin\TAO_IOR_DLL.dll" /libpath:"..\..\ace"
# SUBTRACT BASE LINK32 /incremental:yes
# ADD LINK32 acemfc.lib /nologo /subsystem:windows /dll /machine:I386 /out:"..\..\bin\TAO_IOR_DLLmfc.dll" /libpath:"..\..\ace"
# SUBTRACT LINK32 /incremental:yes

!ELSEIF  "$(CFG)" == "TAO IOR DLL DLL - Win32 MFC Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "TAO_DLL___Win32_MFC_Debug"
# PROP BASE Intermediate_Dir "TAO_DLL___Win32_MFC_Debug"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir ""
# PROP Intermediate_Dir "DLL\MFC_Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
CPP=cl.exe
# ADD BASE CPP /nologo /MDd /W3 /Gm /GX /Zi /Od /Gy /I "../../" /I "../" /D "_DEBUG" /D "WIN32" /D "_WINDOWS" /D "TAO_BUILD_DLL" /FD /c
# SUBTRACT BASE CPP /YX
# ADD CPP /nologo /MDd /W3 /Gm /GX /Zi /Od /Gy /I "../../" /I "../" /D "_DEBUG" /D "WIN32" /D "_WINDOWS" /D "TAO_BUILD_DLL" /D ACE_HAS_MFC=1 /D ACE_DOESNT_INSTANTIATE_NONSTATIC_OBJECT_MANAGER=1 /FD /c
# SUBTRACT CPP /YX
MTL=midl.exe
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
RSC=rc.exe
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 aced.lib /nologo /subsystem:windows /dll /debug /machine:I386 /out:"..\..\bin\TAO_IOR_DLLd.dll" /libpath:"..\..\ace"
# ADD LINK32 acemfcd.lib /nologo /subsystem:windows /dll /debug /machine:I386 /out:"..\..\bin\TAO_IOR_DLLmfcd.dll" /libpath:"..\..\ace"

!ELSEIF  "$(CFG)" == "TAO IOR DLL DLL - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir ".\Release"
# PROP BASE Intermediate_Dir ".\Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir ""
# PROP Intermediate_Dir ".\DLL\Release"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
CPP=cl.exe
# ADD BASE CPP /nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /YX /c
# ADD CPP /nologo /MD /W3 /GX /O2 /I "../../" /I "../" /D "NDEBUG" /D "WIN32" /D "_WINDOWS" /D "TAO_IOR_DLL_BUILD_DLL" /FD /c
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
# ADD LINK32 TAO.lib ace.lib /nologo /subsystem:windows /dll /machine:I386 /out:"..\..\..\bin\TAO_IOR_DLL.dll" /libpath:"..\..\..\ace" /libpath:".."
# SUBTRACT LINK32 /incremental:yes

!ELSEIF  "$(CFG)" == "TAO IOR DLL DLL - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir ".\Debug"
# PROP BASE Intermediate_Dir ".\Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir ""
# PROP Intermediate_Dir "DLL\Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
CPP=cl.exe
# ADD BASE CPP /nologo /MTd /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /YX /c
# ADD CPP /nologo /MDd /W3 /Gm /GX /Zi /Od /Gy /I "../../../" /I "../.." /D "_DEBUG" /D "WIN32" /D "_WINDOWS" /D "TAO_IOR_DLL_BUILD_DLL" /FD /c
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
# ADD LINK32 TAOd.lib aced.lib /nologo /subsystem:windows /dll /debug /machine:I386 /out:"..\..\..\bin\TAO_IOR_DLLd.dll" /libpath:"..\..\..\ace" /libpath:".."

!ENDIF 

# Begin Target

# Name "TAO IOR DLL DLL - Win32 Alpha Release"
# Name "TAO IOR DLL DLL - Win32 Alpha Debug"
# Name "TAO IOR DLL DLL - Win32 MFC Release"
# Name "TAO IOR DLL DLL - Win32 MFC Debug"
# Name "TAO IOR DLL DLL - Win32 Release"
# Name "TAO IOR DLL DLL - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;hpj;bat;for;f90"
# Begin Source File

SOURCE=.\DLL_Parser.cpp

!IF  "$(CFG)" == "TAO IOR DLL DLL - Win32 Alpha Release"

!ELSEIF  "$(CFG)" == "TAO IOR DLL DLL - Win32 Alpha Debug"

!ELSEIF  "$(CFG)" == "TAO IOR DLL DLL - Win32 MFC Release"

!ELSEIF  "$(CFG)" == "TAO IOR DLL DLL - Win32 MFC Debug"

!ELSEIF  "$(CFG)" == "TAO IOR DLL DLL - Win32 Release"

!ELSEIF  "$(CFG)" == "TAO IOR DLL DLL - Win32 Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Object_Loader.cpp

!IF  "$(CFG)" == "TAO IOR DLL DLL - Win32 Alpha Release"

!ELSEIF  "$(CFG)" == "TAO IOR DLL DLL - Win32 Alpha Debug"

!ELSEIF  "$(CFG)" == "TAO IOR DLL DLL - Win32 MFC Release"

!ELSEIF  "$(CFG)" == "TAO IOR DLL DLL - Win32 MFC Debug"

!ELSEIF  "$(CFG)" == "TAO IOR DLL DLL - Win32 Release"

!ELSEIF  "$(CFG)" == "TAO IOR DLL DLL - Win32 Debug"

!ENDIF 

# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl;fi;fd"
# Begin Source File

SOURCE=.\DLL_Parser.h
# End Source File
# Begin Source File

SOURCE=.\ior_dll_export.h
# End Source File
# Begin Source File

SOURCE=.\Object_Loader.h
# End Source File
# End Group
# Begin Group "Inline Files"

# PROP Default_Filter "i"
# Begin Source File

SOURCE=.\DLL_Parser.i
# End Source File
# Begin Source File

SOURCE=.\Object_Loader.i
# End Source File
# End Group
# End Target
# End Project
