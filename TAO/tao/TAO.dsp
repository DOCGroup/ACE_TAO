# Microsoft Developer Studio Project File - Name="TAO DLL" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Dynamic-Link Library" 0x0102
# TARGTYPE "Win32 (ALPHA) Dynamic-Link Library" 0x0602

CFG=TAO DLL - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "TAO.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "TAO.mak" CFG="TAO DLL - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "TAO DLL - Win32 Alpha Release" (based on "Win32 (ALPHA) Dynamic-Link Library")
!MESSAGE "TAO DLL - Win32 Alpha Debug" (based on "Win32 (ALPHA) Dynamic-Link Library")
!MESSAGE "TAO DLL - Win32 MFC Release" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "TAO DLL - Win32 MFC Debug" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "TAO DLL - Win32 Release" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "TAO DLL - Win32 Debug" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath "Desktop"

!IF  "$(CFG)" == "TAO DLL - Win32 Alpha Release"

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
# ADD BASE LINK32 ace.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib /nologo /subsystem:windows /dll /machine:ALPHA /out:"..\..\bin\TAO.dll" /libpath:"..\..\ace"
# ADD LINK32 ace.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib /nologo /subsystem:windows /dll /machine:ALPHA /out:"..\..\bin\TAO.dll" /libpath:"..\..\ace"

!ELSEIF  "$(CFG)" == "TAO DLL - Win32 Alpha Debug"

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
# ADD BASE LINK32 aced.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib /nologo /subsystem:windows /dll /debug /machine:ALPHA /out:"..\..\bin\TAOd.dll" /libpath:"..\..\ace"
# ADD LINK32 aced.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib /nologo /subsystem:windows /dll /debug /machine:ALPHA /out:"..\..\bin\TAOd.dll" /libpath:"..\..\ace"

!ELSEIF  "$(CFG)" == "TAO DLL - Win32 MFC Release"

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
# ADD BASE LINK32 ace.lib /nologo /subsystem:windows /dll /machine:I386 /out:"..\..\bin\TAO.dll" /libpath:"..\..\ace"
# SUBTRACT BASE LINK32 /incremental:yes
# ADD LINK32 acemfc.lib /nologo /subsystem:windows /dll /machine:I386 /out:"..\..\bin\TAOmfc.dll" /libpath:"..\..\ace"
# SUBTRACT LINK32 /incremental:yes

!ELSEIF  "$(CFG)" == "TAO DLL - Win32 MFC Debug"

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
# ADD BASE LINK32 aced.lib /nologo /subsystem:windows /dll /debug /machine:I386 /out:"..\..\bin\TAOd.dll" /libpath:"..\..\ace"
# ADD LINK32 acemfcd.lib /nologo /subsystem:windows /dll /debug /machine:I386 /out:"..\..\bin\TAOmfcd.dll" /libpath:"..\..\ace"

!ELSEIF  "$(CFG)" == "TAO DLL - Win32 Release"

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
# ADD CPP /nologo /MD /W3 /GX /O2 /I "../../" /I "../" /D "NDEBUG" /D "WIN32" /D "_WINDOWS" /D "TAO_BUILD_DLL" /FD /c
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
# ADD LINK32 ace.lib /nologo /subsystem:windows /dll /machine:I386 /out:"..\..\bin\TAO.dll" /libpath:"..\..\ace"
# SUBTRACT LINK32 /incremental:yes

!ELSEIF  "$(CFG)" == "TAO DLL - Win32 Debug"

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
# ADD CPP /nologo /MDd /W3 /Gm /GX /Zi /Od /Gy /I "../../" /I "../" /D "_DEBUG" /D "WIN32" /D "_WINDOWS" /D "TAO_BUILD_DLL" /FD /c
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
# ADD LINK32 aced.lib /nologo /subsystem:windows /dll /debug /machine:I386 /out:"..\..\bin\TAOd.dll" /libpath:"..\..\ace"

!ENDIF 

# Begin Target

# Name "TAO DLL - Win32 Alpha Release"
# Name "TAO DLL - Win32 Alpha Debug"
# Name "TAO DLL - Win32 MFC Release"
# Name "TAO DLL - Win32 MFC Debug"
# Name "TAO DLL - Win32 Release"
# Name "TAO DLL - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;hpj;bat;for;f90"
# Begin Source File

SOURCE=.\Abstract_Servant_Base.cpp

!IF  "$(CFG)" == "TAO DLL - Win32 Alpha Release"

!ELSEIF  "$(CFG)" == "TAO DLL - Win32 Alpha Debug"

!ELSEIF  "$(CFG)" == "TAO DLL - Win32 MFC Release"

!ELSEIF  "$(CFG)" == "TAO DLL - Win32 MFC Debug"

!ELSEIF  "$(CFG)" == "TAO DLL - Win32 Release"

!ELSEIF  "$(CFG)" == "TAO DLL - Win32 Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Acceptor_Filter.cpp

!IF  "$(CFG)" == "TAO DLL - Win32 Alpha Release"

!ELSEIF  "$(CFG)" == "TAO DLL - Win32 Alpha Debug"

!ELSEIF  "$(CFG)" == "TAO DLL - Win32 MFC Release"

!ELSEIF  "$(CFG)" == "TAO DLL - Win32 MFC Debug"

!ELSEIF  "$(CFG)" == "TAO DLL - Win32 Release"

!ELSEIF  "$(CFG)" == "TAO DLL - Win32 Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Acceptor_Registry.cpp

!IF  "$(CFG)" == "TAO DLL - Win32 Alpha Release"

!ELSEIF  "$(CFG)" == "TAO DLL - Win32 Alpha Debug"

!ELSEIF  "$(CFG)" == "TAO DLL - Win32 MFC Release"

!ELSEIF  "$(CFG)" == "TAO DLL - Win32 MFC Debug"

!ELSEIF  "$(CFG)" == "TAO DLL - Win32 Release"

!ELSEIF  "$(CFG)" == "TAO DLL - Win32 Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Adapter.cpp

!IF  "$(CFG)" == "TAO DLL - Win32 Alpha Release"

!ELSEIF  "$(CFG)" == "TAO DLL - Win32 Alpha Debug"

!ELSEIF  "$(CFG)" == "TAO DLL - Win32 MFC Release"

!ELSEIF  "$(CFG)" == "TAO DLL - Win32 MFC Debug"

!ELSEIF  "$(CFG)" == "TAO DLL - Win32 Release"

!ELSEIF  "$(CFG)" == "TAO DLL - Win32 Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Any.cpp

!IF  "$(CFG)" == "TAO DLL - Win32 Alpha Release"

!ELSEIF  "$(CFG)" == "TAO DLL - Win32 Alpha Debug"

!ELSEIF  "$(CFG)" == "TAO DLL - Win32 MFC Release"

!ELSEIF  "$(CFG)" == "TAO DLL - Win32 MFC Debug"

!ELSEIF  "$(CFG)" == "TAO DLL - Win32 Release"

!ELSEIF  "$(CFG)" == "TAO DLL - Win32 Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\append.cpp

!IF  "$(CFG)" == "TAO DLL - Win32 Alpha Release"

!ELSEIF  "$(CFG)" == "TAO DLL - Win32 Alpha Debug"

!ELSEIF  "$(CFG)" == "TAO DLL - Win32 MFC Release"

!ELSEIF  "$(CFG)" == "TAO DLL - Win32 MFC Debug"

!ELSEIF  "$(CFG)" == "TAO DLL - Win32 Release"

!ELSEIF  "$(CFG)" == "TAO DLL - Win32 Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Asynch_Invocation.cpp

!IF  "$(CFG)" == "TAO DLL - Win32 Alpha Release"

!ELSEIF  "$(CFG)" == "TAO DLL - Win32 Alpha Debug"

!ELSEIF  "$(CFG)" == "TAO DLL - Win32 MFC Release"

!ELSEIF  "$(CFG)" == "TAO DLL - Win32 MFC Debug"

!ELSEIF  "$(CFG)" == "TAO DLL - Win32 Release"

!ELSEIF  "$(CFG)" == "TAO DLL - Win32 Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Asynch_Reply_Dispatcher.cpp

!IF  "$(CFG)" == "TAO DLL - Win32 Alpha Release"

!ELSEIF  "$(CFG)" == "TAO DLL - Win32 Alpha Debug"

!ELSEIF  "$(CFG)" == "TAO DLL - Win32 MFC Release"

!ELSEIF  "$(CFG)" == "TAO DLL - Win32 MFC Debug"

!ELSEIF  "$(CFG)" == "TAO DLL - Win32 Release"

!ELSEIF  "$(CFG)" == "TAO DLL - Win32 Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Base_Connection_Property.cpp

!IF  "$(CFG)" == "TAO DLL - Win32 Alpha Release"

!ELSEIF  "$(CFG)" == "TAO DLL - Win32 Alpha Debug"

!ELSEIF  "$(CFG)" == "TAO DLL - Win32 MFC Release"

!ELSEIF  "$(CFG)" == "TAO DLL - Win32 MFC Debug"

!ELSEIF  "$(CFG)" == "TAO DLL - Win32 Release"

!ELSEIF  "$(CFG)" == "TAO DLL - Win32 Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\BiDir_ORBInitializer.cpp

!IF  "$(CFG)" == "TAO DLL - Win32 Alpha Release"

!ELSEIF  "$(CFG)" == "TAO DLL - Win32 Alpha Debug"

!ELSEIF  "$(CFG)" == "TAO DLL - Win32 MFC Release"

!ELSEIF  "$(CFG)" == "TAO DLL - Win32 MFC Debug"

!ELSEIF  "$(CFG)" == "TAO DLL - Win32 Release"

!ELSEIF  "$(CFG)" == "TAO DLL - Win32 Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\BiDir_Policy_i.cpp

!IF  "$(CFG)" == "TAO DLL - Win32 Alpha Release"

!ELSEIF  "$(CFG)" == "TAO DLL - Win32 Alpha Debug"

!ELSEIF  "$(CFG)" == "TAO DLL - Win32 MFC Release"

!ELSEIF  "$(CFG)" == "TAO DLL - Win32 MFC Debug"

!ELSEIF  "$(CFG)" == "TAO DLL - Win32 Release"

!ELSEIF  "$(CFG)" == "TAO DLL - Win32 Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\BiDir_PolicyFactory.cpp

!IF  "$(CFG)" == "TAO DLL - Win32 Alpha Release"

!ELSEIF  "$(CFG)" == "TAO DLL - Win32 Alpha Debug"

!ELSEIF  "$(CFG)" == "TAO DLL - Win32 MFC Release"

!ELSEIF  "$(CFG)" == "TAO DLL - Win32 MFC Debug"

!ELSEIF  "$(CFG)" == "TAO DLL - Win32 Release"

!ELSEIF  "$(CFG)" == "TAO DLL - Win32 Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\BiDirPolicyC.cpp

!IF  "$(CFG)" == "TAO DLL - Win32 Alpha Release"

!ELSEIF  "$(CFG)" == "TAO DLL - Win32 Alpha Debug"

!ELSEIF  "$(CFG)" == "TAO DLL - Win32 MFC Release"

!ELSEIF  "$(CFG)" == "TAO DLL - Win32 MFC Debug"

!ELSEIF  "$(CFG)" == "TAO DLL - Win32 Release"

!ELSEIF  "$(CFG)" == "TAO DLL - Win32 Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Bind_Dispatcher_Guard.cpp

!IF  "$(CFG)" == "TAO DLL - Win32 Alpha Release"

!ELSEIF  "$(CFG)" == "TAO DLL - Win32 Alpha Debug"

!ELSEIF  "$(CFG)" == "TAO DLL - Win32 MFC Release"

!ELSEIF  "$(CFG)" == "TAO DLL - Win32 MFC Debug"

!ELSEIF  "$(CFG)" == "TAO DLL - Win32 Release"

!ELSEIF  "$(CFG)" == "TAO DLL - Win32 Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\BoundsC.cpp

!IF  "$(CFG)" == "TAO DLL - Win32 Alpha Release"

!ELSEIF  "$(CFG)" == "TAO DLL - Win32 Alpha Debug"

!ELSEIF  "$(CFG)" == "TAO DLL - Win32 MFC Release"

!ELSEIF  "$(CFG)" == "TAO DLL - Win32 MFC Debug"

!ELSEIF  "$(CFG)" == "TAO DLL - Win32 Release"

!ELSEIF  "$(CFG)" == "TAO DLL - Win32 Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Buffering_Constraint_Policy.cpp

!IF  "$(CFG)" == "TAO DLL - Win32 Alpha Release"

!ELSEIF  "$(CFG)" == "TAO DLL - Win32 Alpha Debug"

!ELSEIF  "$(CFG)" == "TAO DLL - Win32 MFC Release"

!ELSEIF  "$(CFG)" == "TAO DLL - Win32 MFC Debug"

!ELSEIF  "$(CFG)" == "TAO DLL - Win32 Release"

!ELSEIF  "$(CFG)" == "TAO DLL - Win32 Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Cache_Entries.cpp

!IF  "$(CFG)" == "TAO DLL - Win32 Alpha Release"

!ELSEIF  "$(CFG)" == "TAO DLL - Win32 Alpha Debug"

!ELSEIF  "$(CFG)" == "TAO DLL - Win32 MFC Release"

!ELSEIF  "$(CFG)" == "TAO DLL - Win32 MFC Debug"

!ELSEIF  "$(CFG)" == "TAO DLL - Win32 Release"

!ELSEIF  "$(CFG)" == "TAO DLL - Win32 Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\CDR.cpp

!IF  "$(CFG)" == "TAO DLL - Win32 Alpha Release"

!ELSEIF  "$(CFG)" == "TAO DLL - Win32 Alpha Debug"

!ELSEIF  "$(CFG)" == "TAO DLL - Win32 MFC Release"

!ELSEIF  "$(CFG)" == "TAO DLL - Win32 MFC Debug"

!ELSEIF  "$(CFG)" == "TAO DLL - Win32 Release"

!ELSEIF  "$(CFG)" == "TAO DLL - Win32 Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Cleanup_Func_Registry.cpp

!IF  "$(CFG)" == "TAO DLL - Win32 Alpha Release"

!ELSEIF  "$(CFG)" == "TAO DLL - Win32 Alpha Debug"

!ELSEIF  "$(CFG)" == "TAO DLL - Win32 MFC Release"

!ELSEIF  "$(CFG)" == "TAO DLL - Win32 MFC Debug"

!ELSEIF  "$(CFG)" == "TAO DLL - Win32 Release"

!ELSEIF  "$(CFG)" == "TAO DLL - Win32 Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Client_Priority_Policy.cpp

!IF  "$(CFG)" == "TAO DLL - Win32 Alpha Release"

!ELSEIF  "$(CFG)" == "TAO DLL - Win32 Alpha Debug"

!ELSEIF  "$(CFG)" == "TAO DLL - Win32 MFC Release"

!ELSEIF  "$(CFG)" == "TAO DLL - Win32 MFC Debug"

!ELSEIF  "$(CFG)" == "TAO DLL - Win32 Release"

!ELSEIF  "$(CFG)" == "TAO DLL - Win32 Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Client_Strategy_Factory.cpp

!IF  "$(CFG)" == "TAO DLL - Win32 Alpha Release"

!ELSEIF  "$(CFG)" == "TAO DLL - Win32 Alpha Debug"

!ELSEIF  "$(CFG)" == "TAO DLL - Win32 MFC Release"

!ELSEIF  "$(CFG)" == "TAO DLL - Win32 MFC Debug"

!ELSEIF  "$(CFG)" == "TAO DLL - Win32 Release"

!ELSEIF  "$(CFG)" == "TAO DLL - Win32 Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\ClientRequestInfo.cpp

!IF  "$(CFG)" == "TAO DLL - Win32 Alpha Release"

!ELSEIF  "$(CFG)" == "TAO DLL - Win32 Alpha Debug"

!ELSEIF  "$(CFG)" == "TAO DLL - Win32 MFC Release"

!ELSEIF  "$(CFG)" == "TAO DLL - Win32 MFC Debug"

!ELSEIF  "$(CFG)" == "TAO DLL - Win32 Release"

!ELSEIF  "$(CFG)" == "TAO DLL - Win32 Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Connection_Cache_Manager.cpp

!IF  "$(CFG)" == "TAO DLL - Win32 Alpha Release"

!ELSEIF  "$(CFG)" == "TAO DLL - Win32 Alpha Debug"

!ELSEIF  "$(CFG)" == "TAO DLL - Win32 MFC Release"

!ELSEIF  "$(CFG)" == "TAO DLL - Win32 MFC Debug"

!ELSEIF  "$(CFG)" == "TAO DLL - Win32 Release"

!ELSEIF  "$(CFG)" == "TAO DLL - Win32 Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Connection_Descriptor_Interface.cpp

!IF  "$(CFG)" == "TAO DLL - Win32 Alpha Release"

!ELSEIF  "$(CFG)" == "TAO DLL - Win32 Alpha Debug"

!ELSEIF  "$(CFG)" == "TAO DLL - Win32 MFC Release"

!ELSEIF  "$(CFG)" == "TAO DLL - Win32 MFC Debug"

!ELSEIF  "$(CFG)" == "TAO DLL - Win32 Release"

!ELSEIF  "$(CFG)" == "TAO DLL - Win32 Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Connection_Handler.cpp

!IF  "$(CFG)" == "TAO DLL - Win32 Alpha Release"

!ELSEIF  "$(CFG)" == "TAO DLL - Win32 Alpha Debug"

!ELSEIF  "$(CFG)" == "TAO DLL - Win32 MFC Release"

!ELSEIF  "$(CFG)" == "TAO DLL - Win32 MFC Debug"

!ELSEIF  "$(CFG)" == "TAO DLL - Win32 Release"

!ELSEIF  "$(CFG)" == "TAO DLL - Win32 Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Connector_Impl.cpp

!IF  "$(CFG)" == "TAO DLL - Win32 Alpha Release"

!ELSEIF  "$(CFG)" == "TAO DLL - Win32 Alpha Debug"

!ELSEIF  "$(CFG)" == "TAO DLL - Win32 MFC Release"

!ELSEIF  "$(CFG)" == "TAO DLL - Win32 MFC Debug"

!ELSEIF  "$(CFG)" == "TAO DLL - Win32 Release"

!ELSEIF  "$(CFG)" == "TAO DLL - Win32 Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Connector_Registry.cpp

!IF  "$(CFG)" == "TAO DLL - Win32 Alpha Release"

!ELSEIF  "$(CFG)" == "TAO DLL - Win32 Alpha Debug"

!ELSEIF  "$(CFG)" == "TAO DLL - Win32 MFC Release"

!ELSEIF  "$(CFG)" == "TAO DLL - Win32 MFC Debug"

!ELSEIF  "$(CFG)" == "TAO DLL - Win32 Release"

!ELSEIF  "$(CFG)" == "TAO DLL - Win32 Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\CONV_FRAMEC.cpp

!IF  "$(CFG)" == "TAO DLL - Win32 Alpha Release"

!ELSEIF  "$(CFG)" == "TAO DLL - Win32 Alpha Debug"

!ELSEIF  "$(CFG)" == "TAO DLL - Win32 MFC Release"

!ELSEIF  "$(CFG)" == "TAO DLL - Win32 MFC Debug"

!ELSEIF  "$(CFG)" == "TAO DLL - Win32 Release"

!ELSEIF  "$(CFG)" == "TAO DLL - Win32 Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\CONV_FRAMES.cpp

!IF  "$(CFG)" == "TAO DLL - Win32 Alpha Release"

!ELSEIF  "$(CFG)" == "TAO DLL - Win32 Alpha Debug"

!ELSEIF  "$(CFG)" == "TAO DLL - Win32 MFC Release"

!ELSEIF  "$(CFG)" == "TAO DLL - Win32 MFC Debug"

!ELSEIF  "$(CFG)" == "TAO DLL - Win32 Release"

!ELSEIF  "$(CFG)" == "TAO DLL - Win32 Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\CORBA_String.cpp

!IF  "$(CFG)" == "TAO DLL - Win32 Alpha Release"

!ELSEIF  "$(CFG)" == "TAO DLL - Win32 Alpha Debug"

!ELSEIF  "$(CFG)" == "TAO DLL - Win32 MFC Release"

!ELSEIF  "$(CFG)" == "TAO DLL - Win32 MFC Debug"

!ELSEIF  "$(CFG)" == "TAO DLL - Win32 Release"

!ELSEIF  "$(CFG)" == "TAO DLL - Win32 Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\corbafwd.cpp

!IF  "$(CFG)" == "TAO DLL - Win32 Alpha Release"

!ELSEIF  "$(CFG)" == "TAO DLL - Win32 Alpha Debug"

!ELSEIF  "$(CFG)" == "TAO DLL - Win32 MFC Release"

!ELSEIF  "$(CFG)" == "TAO DLL - Win32 MFC Debug"

!ELSEIF  "$(CFG)" == "TAO DLL - Win32 Release"

!ELSEIF  "$(CFG)" == "TAO DLL - Win32 Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\CORBALOC_Parser.cpp

!IF  "$(CFG)" == "TAO DLL - Win32 Alpha Release"

!ELSEIF  "$(CFG)" == "TAO DLL - Win32 Alpha Debug"

!ELSEIF  "$(CFG)" == "TAO DLL - Win32 MFC Release"

!ELSEIF  "$(CFG)" == "TAO DLL - Win32 MFC Debug"

!ELSEIF  "$(CFG)" == "TAO DLL - Win32 Release"

!ELSEIF  "$(CFG)" == "TAO DLL - Win32 Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\CORBANAME_Parser.cpp

!IF  "$(CFG)" == "TAO DLL - Win32 Alpha Release"

!ELSEIF  "$(CFG)" == "TAO DLL - Win32 Alpha Debug"

!ELSEIF  "$(CFG)" == "TAO DLL - Win32 MFC Release"

!ELSEIF  "$(CFG)" == "TAO DLL - Win32 MFC Debug"

!ELSEIF  "$(CFG)" == "TAO DLL - Win32 Release"

!ELSEIF  "$(CFG)" == "TAO DLL - Win32 Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\CurrentC.cpp

!IF  "$(CFG)" == "TAO DLL - Win32 Alpha Release"

!ELSEIF  "$(CFG)" == "TAO DLL - Win32 Alpha Debug"

!ELSEIF  "$(CFG)" == "TAO DLL - Win32 MFC Release"

!ELSEIF  "$(CFG)" == "TAO DLL - Win32 MFC Debug"

!ELSEIF  "$(CFG)" == "TAO DLL - Win32 Release"

!ELSEIF  "$(CFG)" == "TAO DLL - Win32 Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\debug.cpp

!IF  "$(CFG)" == "TAO DLL - Win32 Alpha Release"

!ELSEIF  "$(CFG)" == "TAO DLL - Win32 Alpha Debug"

!ELSEIF  "$(CFG)" == "TAO DLL - Win32 MFC Release"

!ELSEIF  "$(CFG)" == "TAO DLL - Win32 MFC Debug"

!ELSEIF  "$(CFG)" == "TAO DLL - Win32 Release"

!ELSEIF  "$(CFG)" == "TAO DLL - Win32 Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\default_client.cpp

!IF  "$(CFG)" == "TAO DLL - Win32 Alpha Release"

!ELSEIF  "$(CFG)" == "TAO DLL - Win32 Alpha Debug"

!ELSEIF  "$(CFG)" == "TAO DLL - Win32 MFC Release"

!ELSEIF  "$(CFG)" == "TAO DLL - Win32 MFC Debug"

!ELSEIF  "$(CFG)" == "TAO DLL - Win32 Release"

!ELSEIF  "$(CFG)" == "TAO DLL - Win32 Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\default_resource.cpp

!IF  "$(CFG)" == "TAO DLL - Win32 Alpha Release"

!ELSEIF  "$(CFG)" == "TAO DLL - Win32 Alpha Debug"

!ELSEIF  "$(CFG)" == "TAO DLL - Win32 MFC Release"

!ELSEIF  "$(CFG)" == "TAO DLL - Win32 MFC Debug"

!ELSEIF  "$(CFG)" == "TAO DLL - Win32 Release"

!ELSEIF  "$(CFG)" == "TAO DLL - Win32 Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\default_server.cpp

!IF  "$(CFG)" == "TAO DLL - Win32 Alpha Release"

!ELSEIF  "$(CFG)" == "TAO DLL - Win32 Alpha Debug"

!ELSEIF  "$(CFG)" == "TAO DLL - Win32 MFC Release"

!ELSEIF  "$(CFG)" == "TAO DLL - Win32 MFC Debug"

!ELSEIF  "$(CFG)" == "TAO DLL - Win32 Release"

!ELSEIF  "$(CFG)" == "TAO DLL - Win32 Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\DLL_ORB.cpp

!IF  "$(CFG)" == "TAO DLL - Win32 Alpha Release"

!ELSEIF  "$(CFG)" == "TAO DLL - Win32 Alpha Debug"

!ELSEIF  "$(CFG)" == "TAO DLL - Win32 MFC Release"

!ELSEIF  "$(CFG)" == "TAO DLL - Win32 MFC Debug"

!ELSEIF  "$(CFG)" == "TAO DLL - Win32 Release"

!ELSEIF  "$(CFG)" == "TAO DLL - Win32 Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\DLL_Parser.cpp

!IF  "$(CFG)" == "TAO DLL - Win32 Alpha Release"

!ELSEIF  "$(CFG)" == "TAO DLL - Win32 Alpha Debug"

!ELSEIF  "$(CFG)" == "TAO DLL - Win32 MFC Release"

!ELSEIF  "$(CFG)" == "TAO DLL - Win32 MFC Debug"

!ELSEIF  "$(CFG)" == "TAO DLL - Win32 Release"

!ELSEIF  "$(CFG)" == "TAO DLL - Win32 Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\DomainC.cpp

!IF  "$(CFG)" == "TAO DLL - Win32 Alpha Release"

!ELSEIF  "$(CFG)" == "TAO DLL - Win32 Alpha Debug"

!ELSEIF  "$(CFG)" == "TAO DLL - Win32 MFC Release"

!ELSEIF  "$(CFG)" == "TAO DLL - Win32 MFC Debug"

!ELSEIF  "$(CFG)" == "TAO DLL - Win32 Release"

!ELSEIF  "$(CFG)" == "TAO DLL - Win32 Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Dynamic_Adapter.cpp

!IF  "$(CFG)" == "TAO DLL - Win32 Alpha Release"

!ELSEIF  "$(CFG)" == "TAO DLL - Win32 Alpha Debug"

!ELSEIF  "$(CFG)" == "TAO DLL - Win32 MFC Release"

!ELSEIF  "$(CFG)" == "TAO DLL - Win32 MFC Debug"

!ELSEIF  "$(CFG)" == "TAO DLL - Win32 Release"

!ELSEIF  "$(CFG)" == "TAO DLL - Win32 Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\DynamicC.cpp

!IF  "$(CFG)" == "TAO DLL - Win32 Alpha Release"

!ELSEIF  "$(CFG)" == "TAO DLL - Win32 Alpha Debug"

!ELSEIF  "$(CFG)" == "TAO DLL - Win32 MFC Release"

!ELSEIF  "$(CFG)" == "TAO DLL - Win32 MFC Debug"

!ELSEIF  "$(CFG)" == "TAO DLL - Win32 Release"

!ELSEIF  "$(CFG)" == "TAO DLL - Win32 Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Encodable.cpp

!IF  "$(CFG)" == "TAO DLL - Win32 Alpha Release"

!ELSEIF  "$(CFG)" == "TAO DLL - Win32 Alpha Debug"

!ELSEIF  "$(CFG)" == "TAO DLL - Win32 MFC Release"

!ELSEIF  "$(CFG)" == "TAO DLL - Win32 MFC Debug"

!ELSEIF  "$(CFG)" == "TAO DLL - Win32 Release"

!ELSEIF  "$(CFG)" == "TAO DLL - Win32 Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Endpoint.cpp

!IF  "$(CFG)" == "TAO DLL - Win32 Alpha Release"

!ELSEIF  "$(CFG)" == "TAO DLL - Win32 Alpha Debug"

!ELSEIF  "$(CFG)" == "TAO DLL - Win32 MFC Release"

!ELSEIF  "$(CFG)" == "TAO DLL - Win32 MFC Debug"

!ELSEIF  "$(CFG)" == "TAO DLL - Win32 Release"

!ELSEIF  "$(CFG)" == "TAO DLL - Win32 Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Environment.cpp

!IF  "$(CFG)" == "TAO DLL - Win32 Alpha Release"

!ELSEIF  "$(CFG)" == "TAO DLL - Win32 Alpha Debug"

!ELSEIF  "$(CFG)" == "TAO DLL - Win32 MFC Release"

!ELSEIF  "$(CFG)" == "TAO DLL - Win32 MFC Debug"

!ELSEIF  "$(CFG)" == "TAO DLL - Win32 Release"

!ELSEIF  "$(CFG)" == "TAO DLL - Win32 Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Exception.cpp

!IF  "$(CFG)" == "TAO DLL - Win32 Alpha Release"

!ELSEIF  "$(CFG)" == "TAO DLL - Win32 Alpha Debug"

!ELSEIF  "$(CFG)" == "TAO DLL - Win32 MFC Release"

!ELSEIF  "$(CFG)" == "TAO DLL - Win32 MFC Debug"

!ELSEIF  "$(CFG)" == "TAO DLL - Win32 Release"

!ELSEIF  "$(CFG)" == "TAO DLL - Win32 Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Exclusive_TMS.cpp

!IF  "$(CFG)" == "TAO DLL - Win32 Alpha Release"

!ELSEIF  "$(CFG)" == "TAO DLL - Win32 Alpha Debug"

!ELSEIF  "$(CFG)" == "TAO DLL - Win32 MFC Release"

!ELSEIF  "$(CFG)" == "TAO DLL - Win32 MFC Debug"

!ELSEIF  "$(CFG)" == "TAO DLL - Win32 Release"

!ELSEIF  "$(CFG)" == "TAO DLL - Win32 Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Fault_Tolerance_Service.cpp

!IF  "$(CFG)" == "TAO DLL - Win32 Alpha Release"

!ELSEIF  "$(CFG)" == "TAO DLL - Win32 Alpha Debug"

!ELSEIF  "$(CFG)" == "TAO DLL - Win32 MFC Release"

!ELSEIF  "$(CFG)" == "TAO DLL - Win32 MFC Debug"

!ELSEIF  "$(CFG)" == "TAO DLL - Win32 Release"

!ELSEIF  "$(CFG)" == "TAO DLL - Win32 Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\FILE_Parser.cpp

!IF  "$(CFG)" == "TAO DLL - Win32 Alpha Release"

!ELSEIF  "$(CFG)" == "TAO DLL - Win32 Alpha Debug"

!ELSEIF  "$(CFG)" == "TAO DLL - Win32 MFC Release"

!ELSEIF  "$(CFG)" == "TAO DLL - Win32 MFC Debug"

!ELSEIF  "$(CFG)" == "TAO DLL - Win32 Release"

!ELSEIF  "$(CFG)" == "TAO DLL - Win32 Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\GIOP_Message_Base.cpp

!IF  "$(CFG)" == "TAO DLL - Win32 Alpha Release"

!ELSEIF  "$(CFG)" == "TAO DLL - Win32 Alpha Debug"

!ELSEIF  "$(CFG)" == "TAO DLL - Win32 MFC Release"

!ELSEIF  "$(CFG)" == "TAO DLL - Win32 MFC Debug"

!ELSEIF  "$(CFG)" == "TAO DLL - Win32 Release"

!ELSEIF  "$(CFG)" == "TAO DLL - Win32 Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\GIOP_Message_Generator_Parser.cpp

!IF  "$(CFG)" == "TAO DLL - Win32 Alpha Release"

!ELSEIF  "$(CFG)" == "TAO DLL - Win32 Alpha Debug"

!ELSEIF  "$(CFG)" == "TAO DLL - Win32 MFC Release"

!ELSEIF  "$(CFG)" == "TAO DLL - Win32 MFC Debug"

!ELSEIF  "$(CFG)" == "TAO DLL - Win32 Release"

!ELSEIF  "$(CFG)" == "TAO DLL - Win32 Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\GIOP_Message_Generator_Parser_10.cpp

!IF  "$(CFG)" == "TAO DLL - Win32 Alpha Release"

!ELSEIF  "$(CFG)" == "TAO DLL - Win32 Alpha Debug"

!ELSEIF  "$(CFG)" == "TAO DLL - Win32 MFC Release"

!ELSEIF  "$(CFG)" == "TAO DLL - Win32 MFC Debug"

!ELSEIF  "$(CFG)" == "TAO DLL - Win32 Release"

!ELSEIF  "$(CFG)" == "TAO DLL - Win32 Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\GIOP_Message_Generator_Parser_11.cpp

!IF  "$(CFG)" == "TAO DLL - Win32 Alpha Release"

!ELSEIF  "$(CFG)" == "TAO DLL - Win32 Alpha Debug"

!ELSEIF  "$(CFG)" == "TAO DLL - Win32 MFC Release"

!ELSEIF  "$(CFG)" == "TAO DLL - Win32 MFC Debug"

!ELSEIF  "$(CFG)" == "TAO DLL - Win32 Release"

!ELSEIF  "$(CFG)" == "TAO DLL - Win32 Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\GIOP_Message_Generator_Parser_12.cpp

!IF  "$(CFG)" == "TAO DLL - Win32 Alpha Release"

!ELSEIF  "$(CFG)" == "TAO DLL - Win32 Alpha Debug"

!ELSEIF  "$(CFG)" == "TAO DLL - Win32 MFC Release"

!ELSEIF  "$(CFG)" == "TAO DLL - Win32 MFC Debug"

!ELSEIF  "$(CFG)" == "TAO DLL - Win32 Release"

!ELSEIF  "$(CFG)" == "TAO DLL - Win32 Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\GIOP_Message_Generator_Parser_Impl.cpp

!IF  "$(CFG)" == "TAO DLL - Win32 Alpha Release"

!ELSEIF  "$(CFG)" == "TAO DLL - Win32 Alpha Debug"

!ELSEIF  "$(CFG)" == "TAO DLL - Win32 MFC Release"

!ELSEIF  "$(CFG)" == "TAO DLL - Win32 MFC Debug"

!ELSEIF  "$(CFG)" == "TAO DLL - Win32 Release"

!ELSEIF  "$(CFG)" == "TAO DLL - Win32 Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\GIOP_Message_Lite.cpp

!IF  "$(CFG)" == "TAO DLL - Win32 Alpha Release"

!ELSEIF  "$(CFG)" == "TAO DLL - Win32 Alpha Debug"

!ELSEIF  "$(CFG)" == "TAO DLL - Win32 MFC Release"

!ELSEIF  "$(CFG)" == "TAO DLL - Win32 MFC Debug"

!ELSEIF  "$(CFG)" == "TAO DLL - Win32 Release"

!ELSEIF  "$(CFG)" == "TAO DLL - Win32 Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\GIOP_Message_Locate_Header.cpp

!IF  "$(CFG)" == "TAO DLL - Win32 Alpha Release"

!ELSEIF  "$(CFG)" == "TAO DLL - Win32 Alpha Debug"

!ELSEIF  "$(CFG)" == "TAO DLL - Win32 MFC Release"

!ELSEIF  "$(CFG)" == "TAO DLL - Win32 MFC Debug"

!ELSEIF  "$(CFG)" == "TAO DLL - Win32 Release"

!ELSEIF  "$(CFG)" == "TAO DLL - Win32 Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\GIOP_Message_State.cpp

!IF  "$(CFG)" == "TAO DLL - Win32 Alpha Release"

!ELSEIF  "$(CFG)" == "TAO DLL - Win32 Alpha Debug"

!ELSEIF  "$(CFG)" == "TAO DLL - Win32 MFC Release"

!ELSEIF  "$(CFG)" == "TAO DLL - Win32 MFC Debug"

!ELSEIF  "$(CFG)" == "TAO DLL - Win32 Release"

!ELSEIF  "$(CFG)" == "TAO DLL - Win32 Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\GIOP_Utils.cpp

!IF  "$(CFG)" == "TAO DLL - Win32 Alpha Release"

!ELSEIF  "$(CFG)" == "TAO DLL - Win32 Alpha Debug"

!ELSEIF  "$(CFG)" == "TAO DLL - Win32 MFC Release"

!ELSEIF  "$(CFG)" == "TAO DLL - Win32 MFC Debug"

!ELSEIF  "$(CFG)" == "TAO DLL - Win32 Release"

!ELSEIF  "$(CFG)" == "TAO DLL - Win32 Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\GIOPC.cpp

!IF  "$(CFG)" == "TAO DLL - Win32 Alpha Release"

!ELSEIF  "$(CFG)" == "TAO DLL - Win32 Alpha Debug"

!ELSEIF  "$(CFG)" == "TAO DLL - Win32 MFC Release"

!ELSEIF  "$(CFG)" == "TAO DLL - Win32 MFC Debug"

!ELSEIF  "$(CFG)" == "TAO DLL - Win32 Release"

!ELSEIF  "$(CFG)" == "TAO DLL - Win32 Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\IIOP_Acceptor.cpp

!IF  "$(CFG)" == "TAO DLL - Win32 Alpha Release"

!ELSEIF  "$(CFG)" == "TAO DLL - Win32 Alpha Debug"

!ELSEIF  "$(CFG)" == "TAO DLL - Win32 MFC Release"

!ELSEIF  "$(CFG)" == "TAO DLL - Win32 MFC Debug"

!ELSEIF  "$(CFG)" == "TAO DLL - Win32 Release"

!ELSEIF  "$(CFG)" == "TAO DLL - Win32 Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\IIOP_Connection_Handler.cpp

!IF  "$(CFG)" == "TAO DLL - Win32 Alpha Release"

!ELSEIF  "$(CFG)" == "TAO DLL - Win32 Alpha Debug"

!ELSEIF  "$(CFG)" == "TAO DLL - Win32 MFC Release"

!ELSEIF  "$(CFG)" == "TAO DLL - Win32 MFC Debug"

!ELSEIF  "$(CFG)" == "TAO DLL - Win32 Release"

!ELSEIF  "$(CFG)" == "TAO DLL - Win32 Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\IIOP_Connector.cpp

!IF  "$(CFG)" == "TAO DLL - Win32 Alpha Release"

!ELSEIF  "$(CFG)" == "TAO DLL - Win32 Alpha Debug"

!ELSEIF  "$(CFG)" == "TAO DLL - Win32 MFC Release"

!ELSEIF  "$(CFG)" == "TAO DLL - Win32 MFC Debug"

!ELSEIF  "$(CFG)" == "TAO DLL - Win32 Release"

!ELSEIF  "$(CFG)" == "TAO DLL - Win32 Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\IIOP_Endpoint.cpp

!IF  "$(CFG)" == "TAO DLL - Win32 Alpha Release"

!ELSEIF  "$(CFG)" == "TAO DLL - Win32 Alpha Debug"

!ELSEIF  "$(CFG)" == "TAO DLL - Win32 MFC Release"

!ELSEIF  "$(CFG)" == "TAO DLL - Win32 MFC Debug"

!ELSEIF  "$(CFG)" == "TAO DLL - Win32 Release"

!ELSEIF  "$(CFG)" == "TAO DLL - Win32 Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\iiop_endpoints.cpp

!IF  "$(CFG)" == "TAO DLL - Win32 Alpha Release"

!ELSEIF  "$(CFG)" == "TAO DLL - Win32 Alpha Debug"

!ELSEIF  "$(CFG)" == "TAO DLL - Win32 MFC Release"

!ELSEIF  "$(CFG)" == "TAO DLL - Win32 MFC Debug"

!ELSEIF  "$(CFG)" == "TAO DLL - Win32 Release"

!ELSEIF  "$(CFG)" == "TAO DLL - Win32 Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\IIOP_Factory.cpp

!IF  "$(CFG)" == "TAO DLL - Win32 Alpha Release"

!ELSEIF  "$(CFG)" == "TAO DLL - Win32 Alpha Debug"

!ELSEIF  "$(CFG)" == "TAO DLL - Win32 MFC Release"

!ELSEIF  "$(CFG)" == "TAO DLL - Win32 MFC Debug"

!ELSEIF  "$(CFG)" == "TAO DLL - Win32 Release"

!ELSEIF  "$(CFG)" == "TAO DLL - Win32 Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\IIOP_Lite_Factory.cpp

!IF  "$(CFG)" == "TAO DLL - Win32 Alpha Release"

!ELSEIF  "$(CFG)" == "TAO DLL - Win32 Alpha Debug"

!ELSEIF  "$(CFG)" == "TAO DLL - Win32 MFC Release"

!ELSEIF  "$(CFG)" == "TAO DLL - Win32 MFC Debug"

!ELSEIF  "$(CFG)" == "TAO DLL - Win32 Release"

!ELSEIF  "$(CFG)" == "TAO DLL - Win32 Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\IIOP_Profile.cpp

!IF  "$(CFG)" == "TAO DLL - Win32 Alpha Release"

!ELSEIF  "$(CFG)" == "TAO DLL - Win32 Alpha Debug"

!ELSEIF  "$(CFG)" == "TAO DLL - Win32 MFC Release"

!ELSEIF  "$(CFG)" == "TAO DLL - Win32 MFC Debug"

!ELSEIF  "$(CFG)" == "TAO DLL - Win32 Release"

!ELSEIF  "$(CFG)" == "TAO DLL - Win32 Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\IIOP_Transport.cpp

!IF  "$(CFG)" == "TAO DLL - Win32 Alpha Release"

!ELSEIF  "$(CFG)" == "TAO DLL - Win32 Alpha Debug"

!ELSEIF  "$(CFG)" == "TAO DLL - Win32 MFC Release"

!ELSEIF  "$(CFG)" == "TAO DLL - Win32 MFC Debug"

!ELSEIF  "$(CFG)" == "TAO DLL - Win32 Release"

!ELSEIF  "$(CFG)" == "TAO DLL - Win32 Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\IIOPC.cpp

!IF  "$(CFG)" == "TAO DLL - Win32 Alpha Release"

!ELSEIF  "$(CFG)" == "TAO DLL - Win32 Alpha Debug"

!ELSEIF  "$(CFG)" == "TAO DLL - Win32 MFC Release"

!ELSEIF  "$(CFG)" == "TAO DLL - Win32 MFC Debug"

!ELSEIF  "$(CFG)" == "TAO DLL - Win32 Release"

!ELSEIF  "$(CFG)" == "TAO DLL - Win32 Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Interceptor_List.cpp

!IF  "$(CFG)" == "TAO DLL - Win32 Alpha Release"

!ELSEIF  "$(CFG)" == "TAO DLL - Win32 Alpha Debug"

!ELSEIF  "$(CFG)" == "TAO DLL - Win32 MFC Release"

!ELSEIF  "$(CFG)" == "TAO DLL - Win32 MFC Debug"

!ELSEIF  "$(CFG)" == "TAO DLL - Win32 Release"

!ELSEIF  "$(CFG)" == "TAO DLL - Win32 Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\InterfaceC.cpp

!IF  "$(CFG)" == "TAO DLL - Win32 Alpha Release"

!ELSEIF  "$(CFG)" == "TAO DLL - Win32 Alpha Debug"

!ELSEIF  "$(CFG)" == "TAO DLL - Win32 MFC Release"

!ELSEIF  "$(CFG)" == "TAO DLL - Win32 MFC Debug"

!ELSEIF  "$(CFG)" == "TAO DLL - Win32 Release"

!ELSEIF  "$(CFG)" == "TAO DLL - Win32 Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Invocation.cpp

!IF  "$(CFG)" == "TAO DLL - Win32 Alpha Release"

!ELSEIF  "$(CFG)" == "TAO DLL - Win32 Alpha Debug"

!ELSEIF  "$(CFG)" == "TAO DLL - Win32 MFC Release"

!ELSEIF  "$(CFG)" == "TAO DLL - Win32 MFC Debug"

!ELSEIF  "$(CFG)" == "TAO DLL - Win32 Release"

!ELSEIF  "$(CFG)" == "TAO DLL - Win32 Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Invocation_Endpoint_Selectors.cpp

!IF  "$(CFG)" == "TAO DLL - Win32 Alpha Release"

!ELSEIF  "$(CFG)" == "TAO DLL - Win32 Alpha Debug"

!ELSEIF  "$(CFG)" == "TAO DLL - Win32 MFC Release"

!ELSEIF  "$(CFG)" == "TAO DLL - Win32 MFC Debug"

!ELSEIF  "$(CFG)" == "TAO DLL - Win32 Release"

!ELSEIF  "$(CFG)" == "TAO DLL - Win32 Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\IOPC.cpp

!IF  "$(CFG)" == "TAO DLL - Win32 Alpha Release"

!ELSEIF  "$(CFG)" == "TAO DLL - Win32 Alpha Debug"

!ELSEIF  "$(CFG)" == "TAO DLL - Win32 MFC Release"

!ELSEIF  "$(CFG)" == "TAO DLL - Win32 MFC Debug"

!ELSEIF  "$(CFG)" == "TAO DLL - Win32 Release"

!ELSEIF  "$(CFG)" == "TAO DLL - Win32 Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\IOPS.cpp

!IF  "$(CFG)" == "TAO DLL - Win32 Alpha Release"

!ELSEIF  "$(CFG)" == "TAO DLL - Win32 Alpha Debug"

!ELSEIF  "$(CFG)" == "TAO DLL - Win32 MFC Release"

!ELSEIF  "$(CFG)" == "TAO DLL - Win32 MFC Debug"

!ELSEIF  "$(CFG)" == "TAO DLL - Win32 Release"

!ELSEIF  "$(CFG)" == "TAO DLL - Win32 Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\IOR_Parser.cpp

!IF  "$(CFG)" == "TAO DLL - Win32 Alpha Release"

!ELSEIF  "$(CFG)" == "TAO DLL - Win32 Alpha Debug"

!ELSEIF  "$(CFG)" == "TAO DLL - Win32 MFC Release"

!ELSEIF  "$(CFG)" == "TAO DLL - Win32 MFC Debug"

!ELSEIF  "$(CFG)" == "TAO DLL - Win32 Release"

!ELSEIF  "$(CFG)" == "TAO DLL - Win32 Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\IORInfo.cpp

!IF  "$(CFG)" == "TAO DLL - Win32 Alpha Release"

!ELSEIF  "$(CFG)" == "TAO DLL - Win32 Alpha Debug"

!ELSEIF  "$(CFG)" == "TAO DLL - Win32 MFC Release"

!ELSEIF  "$(CFG)" == "TAO DLL - Win32 MFC Debug"

!ELSEIF  "$(CFG)" == "TAO DLL - Win32 Release"

!ELSEIF  "$(CFG)" == "TAO DLL - Win32 Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Leader_Follower.cpp

!IF  "$(CFG)" == "TAO DLL - Win32 Alpha Release"

!ELSEIF  "$(CFG)" == "TAO DLL - Win32 Alpha Debug"

!ELSEIF  "$(CFG)" == "TAO DLL - Win32 MFC Release"

!ELSEIF  "$(CFG)" == "TAO DLL - Win32 MFC Debug"

!ELSEIF  "$(CFG)" == "TAO DLL - Win32 Release"

!ELSEIF  "$(CFG)" == "TAO DLL - Win32 Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\LocalObject.cpp

!IF  "$(CFG)" == "TAO DLL - Win32 Alpha Release"

!ELSEIF  "$(CFG)" == "TAO DLL - Win32 Alpha Debug"

!ELSEIF  "$(CFG)" == "TAO DLL - Win32 MFC Release"

!ELSEIF  "$(CFG)" == "TAO DLL - Win32 MFC Debug"

!ELSEIF  "$(CFG)" == "TAO DLL - Win32 Release"

!ELSEIF  "$(CFG)" == "TAO DLL - Win32 Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Managed_Types.cpp

!IF  "$(CFG)" == "TAO DLL - Win32 Alpha Release"

!ELSEIF  "$(CFG)" == "TAO DLL - Win32 Alpha Debug"

!ELSEIF  "$(CFG)" == "TAO DLL - Win32 MFC Release"

!ELSEIF  "$(CFG)" == "TAO DLL - Win32 MFC Debug"

!ELSEIF  "$(CFG)" == "TAO DLL - Win32 Release"

!ELSEIF  "$(CFG)" == "TAO DLL - Win32 Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Marshal.cpp

!IF  "$(CFG)" == "TAO DLL - Win32 Alpha Release"

!ELSEIF  "$(CFG)" == "TAO DLL - Win32 Alpha Debug"

!ELSEIF  "$(CFG)" == "TAO DLL - Win32 MFC Release"

!ELSEIF  "$(CFG)" == "TAO DLL - Win32 MFC Debug"

!ELSEIF  "$(CFG)" == "TAO DLL - Win32 Release"

!ELSEIF  "$(CFG)" == "TAO DLL - Win32 Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Messaging_ORBInitializer.cpp

!IF  "$(CFG)" == "TAO DLL - Win32 Alpha Release"

!ELSEIF  "$(CFG)" == "TAO DLL - Win32 Alpha Debug"

!ELSEIF  "$(CFG)" == "TAO DLL - Win32 MFC Release"

!ELSEIF  "$(CFG)" == "TAO DLL - Win32 MFC Debug"

!ELSEIF  "$(CFG)" == "TAO DLL - Win32 Release"

!ELSEIF  "$(CFG)" == "TAO DLL - Win32 Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Messaging_Policy_i.cpp

!IF  "$(CFG)" == "TAO DLL - Win32 Alpha Release"

!ELSEIF  "$(CFG)" == "TAO DLL - Win32 Alpha Debug"

!ELSEIF  "$(CFG)" == "TAO DLL - Win32 MFC Release"

!ELSEIF  "$(CFG)" == "TAO DLL - Win32 MFC Debug"

!ELSEIF  "$(CFG)" == "TAO DLL - Win32 Release"

!ELSEIF  "$(CFG)" == "TAO DLL - Win32 Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Messaging_PolicyFactory.cpp

!IF  "$(CFG)" == "TAO DLL - Win32 Alpha Release"

!ELSEIF  "$(CFG)" == "TAO DLL - Win32 Alpha Debug"

!ELSEIF  "$(CFG)" == "TAO DLL - Win32 MFC Release"

!ELSEIF  "$(CFG)" == "TAO DLL - Win32 MFC Debug"

!ELSEIF  "$(CFG)" == "TAO DLL - Win32 Release"

!ELSEIF  "$(CFG)" == "TAO DLL - Win32 Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\MessagingC.cpp

!IF  "$(CFG)" == "TAO DLL - Win32 Alpha Release"

!ELSEIF  "$(CFG)" == "TAO DLL - Win32 Alpha Debug"

!ELSEIF  "$(CFG)" == "TAO DLL - Win32 MFC Release"

!ELSEIF  "$(CFG)" == "TAO DLL - Win32 MFC Debug"

!ELSEIF  "$(CFG)" == "TAO DLL - Win32 Release"

!ELSEIF  "$(CFG)" == "TAO DLL - Win32 Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\MProfile.cpp

!IF  "$(CFG)" == "TAO DLL - Win32 Alpha Release"

!ELSEIF  "$(CFG)" == "TAO DLL - Win32 Alpha Debug"

!ELSEIF  "$(CFG)" == "TAO DLL - Win32 MFC Release"

!ELSEIF  "$(CFG)" == "TAO DLL - Win32 MFC Debug"

!ELSEIF  "$(CFG)" == "TAO DLL - Win32 Release"

!ELSEIF  "$(CFG)" == "TAO DLL - Win32 Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Muxed_TMS.cpp

!IF  "$(CFG)" == "TAO DLL - Win32 Alpha Release"

!ELSEIF  "$(CFG)" == "TAO DLL - Win32 Alpha Debug"

!ELSEIF  "$(CFG)" == "TAO DLL - Win32 MFC Release"

!ELSEIF  "$(CFG)" == "TAO DLL - Win32 MFC Debug"

!ELSEIF  "$(CFG)" == "TAO DLL - Win32 Release"

!ELSEIF  "$(CFG)" == "TAO DLL - Win32 Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\NVList.cpp

!IF  "$(CFG)" == "TAO DLL - Win32 Alpha Release"

!ELSEIF  "$(CFG)" == "TAO DLL - Win32 Alpha Debug"

!ELSEIF  "$(CFG)" == "TAO DLL - Win32 MFC Release"

!ELSEIF  "$(CFG)" == "TAO DLL - Win32 MFC Debug"

!ELSEIF  "$(CFG)" == "TAO DLL - Win32 Release"

!ELSEIF  "$(CFG)" == "TAO DLL - Win32 Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Object.cpp

!IF  "$(CFG)" == "TAO DLL - Win32 Alpha Release"

!ELSEIF  "$(CFG)" == "TAO DLL - Win32 Alpha Debug"

!ELSEIF  "$(CFG)" == "TAO DLL - Win32 MFC Release"

!ELSEIF  "$(CFG)" == "TAO DLL - Win32 MFC Debug"

!ELSEIF  "$(CFG)" == "TAO DLL - Win32 Release"

!ELSEIF  "$(CFG)" == "TAO DLL - Win32 Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Object_KeyC.cpp

!IF  "$(CFG)" == "TAO DLL - Win32 Alpha Release"

!ELSEIF  "$(CFG)" == "TAO DLL - Win32 Alpha Debug"

!ELSEIF  "$(CFG)" == "TAO DLL - Win32 MFC Release"

!ELSEIF  "$(CFG)" == "TAO DLL - Win32 MFC Debug"

!ELSEIF  "$(CFG)" == "TAO DLL - Win32 Release"

!ELSEIF  "$(CFG)" == "TAO DLL - Win32 Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Object_Loader.cpp

!IF  "$(CFG)" == "TAO DLL - Win32 Alpha Release"

!ELSEIF  "$(CFG)" == "TAO DLL - Win32 Alpha Debug"

!ELSEIF  "$(CFG)" == "TAO DLL - Win32 MFC Release"

!ELSEIF  "$(CFG)" == "TAO DLL - Win32 MFC Debug"

!ELSEIF  "$(CFG)" == "TAO DLL - Win32 Release"

!ELSEIF  "$(CFG)" == "TAO DLL - Win32 Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Object_Proxy_Broker.cpp

!IF  "$(CFG)" == "TAO DLL - Win32 Alpha Release"

!ELSEIF  "$(CFG)" == "TAO DLL - Win32 Alpha Debug"

!ELSEIF  "$(CFG)" == "TAO DLL - Win32 MFC Release"

!ELSEIF  "$(CFG)" == "TAO DLL - Win32 MFC Debug"

!ELSEIF  "$(CFG)" == "TAO DLL - Win32 Release"

!ELSEIF  "$(CFG)" == "TAO DLL - Win32 Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Object_Proxy_Impl.cpp

!IF  "$(CFG)" == "TAO DLL - Win32 Alpha Release"

!ELSEIF  "$(CFG)" == "TAO DLL - Win32 Alpha Debug"

!ELSEIF  "$(CFG)" == "TAO DLL - Win32 MFC Release"

!ELSEIF  "$(CFG)" == "TAO DLL - Win32 MFC Debug"

!ELSEIF  "$(CFG)" == "TAO DLL - Win32 Release"

!ELSEIF  "$(CFG)" == "TAO DLL - Win32 Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Object_Ref_Table.cpp

!IF  "$(CFG)" == "TAO DLL - Win32 Alpha Release"

!ELSEIF  "$(CFG)" == "TAO DLL - Win32 Alpha Debug"

!ELSEIF  "$(CFG)" == "TAO DLL - Win32 MFC Release"

!ELSEIF  "$(CFG)" == "TAO DLL - Win32 MFC Debug"

!ELSEIF  "$(CFG)" == "TAO DLL - Win32 Release"

!ELSEIF  "$(CFG)" == "TAO DLL - Win32 Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\ObjectIDList.cpp

!IF  "$(CFG)" == "TAO DLL - Win32 Alpha Release"

!ELSEIF  "$(CFG)" == "TAO DLL - Win32 Alpha Debug"

!ELSEIF  "$(CFG)" == "TAO DLL - Win32 MFC Release"

!ELSEIF  "$(CFG)" == "TAO DLL - Win32 MFC Debug"

!ELSEIF  "$(CFG)" == "TAO DLL - Win32 Release"

!ELSEIF  "$(CFG)" == "TAO DLL - Win32 Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\OctetSeqC.cpp

!IF  "$(CFG)" == "TAO DLL - Win32 Alpha Release"

!ELSEIF  "$(CFG)" == "TAO DLL - Win32 Alpha Debug"

!ELSEIF  "$(CFG)" == "TAO DLL - Win32 MFC Release"

!ELSEIF  "$(CFG)" == "TAO DLL - Win32 MFC Debug"

!ELSEIF  "$(CFG)" == "TAO DLL - Win32 Release"

!ELSEIF  "$(CFG)" == "TAO DLL - Win32 Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\operation_details.cpp

!IF  "$(CFG)" == "TAO DLL - Win32 Alpha Release"

!ELSEIF  "$(CFG)" == "TAO DLL - Win32 Alpha Debug"

!ELSEIF  "$(CFG)" == "TAO DLL - Win32 MFC Release"

!ELSEIF  "$(CFG)" == "TAO DLL - Win32 MFC Debug"

!ELSEIF  "$(CFG)" == "TAO DLL - Win32 Release"

!ELSEIF  "$(CFG)" == "TAO DLL - Win32 Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\ORB.cpp

!IF  "$(CFG)" == "TAO DLL - Win32 Alpha Release"

!ELSEIF  "$(CFG)" == "TAO DLL - Win32 Alpha Debug"

!ELSEIF  "$(CFG)" == "TAO DLL - Win32 MFC Release"

!ELSEIF  "$(CFG)" == "TAO DLL - Win32 MFC Debug"

!ELSEIF  "$(CFG)" == "TAO DLL - Win32 Release"

!ELSEIF  "$(CFG)" == "TAO DLL - Win32 Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\ORB_Core.cpp

!IF  "$(CFG)" == "TAO DLL - Win32 Alpha Release"

!ELSEIF  "$(CFG)" == "TAO DLL - Win32 Alpha Debug"

!ELSEIF  "$(CFG)" == "TAO DLL - Win32 MFC Release"

!ELSEIF  "$(CFG)" == "TAO DLL - Win32 MFC Debug"

!ELSEIF  "$(CFG)" == "TAO DLL - Win32 Release"

!ELSEIF  "$(CFG)" == "TAO DLL - Win32 Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\ORB_Table.cpp

!IF  "$(CFG)" == "TAO DLL - Win32 Alpha Release"

!ELSEIF  "$(CFG)" == "TAO DLL - Win32 Alpha Debug"

!ELSEIF  "$(CFG)" == "TAO DLL - Win32 MFC Release"

!ELSEIF  "$(CFG)" == "TAO DLL - Win32 MFC Debug"

!ELSEIF  "$(CFG)" == "TAO DLL - Win32 Release"

!ELSEIF  "$(CFG)" == "TAO DLL - Win32 Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\ORBInitializer_Registry.cpp

!IF  "$(CFG)" == "TAO DLL - Win32 Alpha Release"

!ELSEIF  "$(CFG)" == "TAO DLL - Win32 Alpha Debug"

!ELSEIF  "$(CFG)" == "TAO DLL - Win32 MFC Release"

!ELSEIF  "$(CFG)" == "TAO DLL - Win32 MFC Debug"

!ELSEIF  "$(CFG)" == "TAO DLL - Win32 Release"

!ELSEIF  "$(CFG)" == "TAO DLL - Win32 Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\ORBInitInfo.cpp

!IF  "$(CFG)" == "TAO DLL - Win32 Alpha Release"

!ELSEIF  "$(CFG)" == "TAO DLL - Win32 Alpha Debug"

!ELSEIF  "$(CFG)" == "TAO DLL - Win32 MFC Release"

!ELSEIF  "$(CFG)" == "TAO DLL - Win32 MFC Debug"

!ELSEIF  "$(CFG)" == "TAO DLL - Win32 Release"

!ELSEIF  "$(CFG)" == "TAO DLL - Win32 Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\params.cpp

!IF  "$(CFG)" == "TAO DLL - Win32 Alpha Release"

!ELSEIF  "$(CFG)" == "TAO DLL - Win32 Alpha Debug"

!ELSEIF  "$(CFG)" == "TAO DLL - Win32 MFC Release"

!ELSEIF  "$(CFG)" == "TAO DLL - Win32 MFC Debug"

!ELSEIF  "$(CFG)" == "TAO DLL - Win32 Release"

!ELSEIF  "$(CFG)" == "TAO DLL - Win32 Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Parser_Registry.cpp

!IF  "$(CFG)" == "TAO DLL - Win32 Alpha Release"

!ELSEIF  "$(CFG)" == "TAO DLL - Win32 Alpha Debug"

!ELSEIF  "$(CFG)" == "TAO DLL - Win32 MFC Release"

!ELSEIF  "$(CFG)" == "TAO DLL - Win32 MFC Debug"

!ELSEIF  "$(CFG)" == "TAO DLL - Win32 Release"

!ELSEIF  "$(CFG)" == "TAO DLL - Win32 Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Pluggable.cpp

!IF  "$(CFG)" == "TAO DLL - Win32 Alpha Release"

!ELSEIF  "$(CFG)" == "TAO DLL - Win32 Alpha Debug"

!ELSEIF  "$(CFG)" == "TAO DLL - Win32 MFC Release"

!ELSEIF  "$(CFG)" == "TAO DLL - Win32 MFC Debug"

!ELSEIF  "$(CFG)" == "TAO DLL - Win32 Release"

!ELSEIF  "$(CFG)" == "TAO DLL - Win32 Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Pluggable_Messaging.cpp

!IF  "$(CFG)" == "TAO DLL - Win32 Alpha Release"

!ELSEIF  "$(CFG)" == "TAO DLL - Win32 Alpha Debug"

!ELSEIF  "$(CFG)" == "TAO DLL - Win32 MFC Release"

!ELSEIF  "$(CFG)" == "TAO DLL - Win32 MFC Debug"

!ELSEIF  "$(CFG)" == "TAO DLL - Win32 Release"

!ELSEIF  "$(CFG)" == "TAO DLL - Win32 Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Pluggable_Messaging_Utils.cpp

!IF  "$(CFG)" == "TAO DLL - Win32 Alpha Release"

!ELSEIF  "$(CFG)" == "TAO DLL - Win32 Alpha Debug"

!ELSEIF  "$(CFG)" == "TAO DLL - Win32 MFC Release"

!ELSEIF  "$(CFG)" == "TAO DLL - Win32 MFC Debug"

!ELSEIF  "$(CFG)" == "TAO DLL - Win32 Release"

!ELSEIF  "$(CFG)" == "TAO DLL - Win32 Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Policy_Manager.cpp

!IF  "$(CFG)" == "TAO DLL - Win32 Alpha Release"

!ELSEIF  "$(CFG)" == "TAO DLL - Win32 Alpha Debug"

!ELSEIF  "$(CFG)" == "TAO DLL - Win32 MFC Release"

!ELSEIF  "$(CFG)" == "TAO DLL - Win32 MFC Debug"

!ELSEIF  "$(CFG)" == "TAO DLL - Win32 Release"

!ELSEIF  "$(CFG)" == "TAO DLL - Win32 Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\PolicyC.cpp

!IF  "$(CFG)" == "TAO DLL - Win32 Alpha Release"

!ELSEIF  "$(CFG)" == "TAO DLL - Win32 Alpha Debug"

!ELSEIF  "$(CFG)" == "TAO DLL - Win32 MFC Release"

!ELSEIF  "$(CFG)" == "TAO DLL - Win32 MFC Debug"

!ELSEIF  "$(CFG)" == "TAO DLL - Win32 Release"

!ELSEIF  "$(CFG)" == "TAO DLL - Win32 Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\PolicyFactory_Registry.cpp

!IF  "$(CFG)" == "TAO DLL - Win32 Alpha Release"

!ELSEIF  "$(CFG)" == "TAO DLL - Win32 Alpha Debug"

!ELSEIF  "$(CFG)" == "TAO DLL - Win32 MFC Release"

!ELSEIF  "$(CFG)" == "TAO DLL - Win32 MFC Debug"

!ELSEIF  "$(CFG)" == "TAO DLL - Win32 Release"

!ELSEIF  "$(CFG)" == "TAO DLL - Win32 Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\PollableC.cpp

!IF  "$(CFG)" == "TAO DLL - Win32 Alpha Release"

!ELSEIF  "$(CFG)" == "TAO DLL - Win32 Alpha Debug"

!ELSEIF  "$(CFG)" == "TAO DLL - Win32 MFC Release"

!ELSEIF  "$(CFG)" == "TAO DLL - Win32 MFC Debug"

!ELSEIF  "$(CFG)" == "TAO DLL - Win32 Release"

!ELSEIF  "$(CFG)" == "TAO DLL - Win32 Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\PollableS.cpp

!IF  "$(CFG)" == "TAO DLL - Win32 Alpha Release"

!ELSEIF  "$(CFG)" == "TAO DLL - Win32 Alpha Debug"

!ELSEIF  "$(CFG)" == "TAO DLL - Win32 MFC Release"

!ELSEIF  "$(CFG)" == "TAO DLL - Win32 MFC Debug"

!ELSEIF  "$(CFG)" == "TAO DLL - Win32 Release"

!ELSEIF  "$(CFG)" == "TAO DLL - Win32 Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Pool_Per_Endpoint.cpp

!IF  "$(CFG)" == "TAO DLL - Win32 Alpha Release"

!ELSEIF  "$(CFG)" == "TAO DLL - Win32 Alpha Debug"

!ELSEIF  "$(CFG)" == "TAO DLL - Win32 MFC Release"

!ELSEIF  "$(CFG)" == "TAO DLL - Win32 MFC Debug"

!ELSEIF  "$(CFG)" == "TAO DLL - Win32 Release"

!ELSEIF  "$(CFG)" == "TAO DLL - Win32 Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\PortableInterceptor.cpp

!IF  "$(CFG)" == "TAO DLL - Win32 Alpha Release"

!ELSEIF  "$(CFG)" == "TAO DLL - Win32 Alpha Debug"

!ELSEIF  "$(CFG)" == "TAO DLL - Win32 MFC Release"

!ELSEIF  "$(CFG)" == "TAO DLL - Win32 MFC Debug"

!ELSEIF  "$(CFG)" == "TAO DLL - Win32 Release"

!ELSEIF  "$(CFG)" == "TAO DLL - Win32 Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\PortableInterceptorC.cpp

!IF  "$(CFG)" == "TAO DLL - Win32 Alpha Release"

!ELSEIF  "$(CFG)" == "TAO DLL - Win32 Alpha Debug"

!ELSEIF  "$(CFG)" == "TAO DLL - Win32 MFC Release"

!ELSEIF  "$(CFG)" == "TAO DLL - Win32 MFC Debug"

!ELSEIF  "$(CFG)" == "TAO DLL - Win32 Release"

!ELSEIF  "$(CFG)" == "TAO DLL - Win32 Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Principal.cpp

!IF  "$(CFG)" == "TAO DLL - Win32 Alpha Release"

!ELSEIF  "$(CFG)" == "TAO DLL - Win32 Alpha Debug"

!ELSEIF  "$(CFG)" == "TAO DLL - Win32 MFC Release"

!ELSEIF  "$(CFG)" == "TAO DLL - Win32 MFC Debug"

!ELSEIF  "$(CFG)" == "TAO DLL - Win32 Release"

!ELSEIF  "$(CFG)" == "TAO DLL - Win32 Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Priority_Mapping.cpp

!IF  "$(CFG)" == "TAO DLL - Win32 Alpha Release"

!ELSEIF  "$(CFG)" == "TAO DLL - Win32 Alpha Debug"

!ELSEIF  "$(CFG)" == "TAO DLL - Win32 MFC Release"

!ELSEIF  "$(CFG)" == "TAO DLL - Win32 MFC Debug"

!ELSEIF  "$(CFG)" == "TAO DLL - Win32 Release"

!ELSEIF  "$(CFG)" == "TAO DLL - Win32 Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Priority_Mapping_Manager.cpp

!IF  "$(CFG)" == "TAO DLL - Win32 Alpha Release"

!ELSEIF  "$(CFG)" == "TAO DLL - Win32 Alpha Debug"

!ELSEIF  "$(CFG)" == "TAO DLL - Win32 MFC Release"

!ELSEIF  "$(CFG)" == "TAO DLL - Win32 MFC Debug"

!ELSEIF  "$(CFG)" == "TAO DLL - Win32 Release"

!ELSEIF  "$(CFG)" == "TAO DLL - Win32 Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Private_Connection_Descriptor.cpp

!IF  "$(CFG)" == "TAO DLL - Win32 Alpha Release"

!ELSEIF  "$(CFG)" == "TAO DLL - Win32 Alpha Debug"

!ELSEIF  "$(CFG)" == "TAO DLL - Win32 MFC Release"

!ELSEIF  "$(CFG)" == "TAO DLL - Win32 MFC Debug"

!ELSEIF  "$(CFG)" == "TAO DLL - Win32 Release"

!ELSEIF  "$(CFG)" == "TAO DLL - Win32 Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Profile.cpp

!IF  "$(CFG)" == "TAO DLL - Win32 Alpha Release"

!ELSEIF  "$(CFG)" == "TAO DLL - Win32 Alpha Debug"

!ELSEIF  "$(CFG)" == "TAO DLL - Win32 MFC Release"

!ELSEIF  "$(CFG)" == "TAO DLL - Win32 MFC Debug"

!ELSEIF  "$(CFG)" == "TAO DLL - Win32 Release"

!ELSEIF  "$(CFG)" == "TAO DLL - Win32 Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Protocol_Factory.cpp

!IF  "$(CFG)" == "TAO DLL - Win32 Alpha Release"

!ELSEIF  "$(CFG)" == "TAO DLL - Win32 Alpha Debug"

!ELSEIF  "$(CFG)" == "TAO DLL - Win32 MFC Release"

!ELSEIF  "$(CFG)" == "TAO DLL - Win32 MFC Debug"

!ELSEIF  "$(CFG)" == "TAO DLL - Win32 Release"

!ELSEIF  "$(CFG)" == "TAO DLL - Win32 Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Reactor_Registry.cpp

!IF  "$(CFG)" == "TAO DLL - Win32 Alpha Release"

!ELSEIF  "$(CFG)" == "TAO DLL - Win32 Alpha Debug"

!ELSEIF  "$(CFG)" == "TAO DLL - Win32 MFC Release"

!ELSEIF  "$(CFG)" == "TAO DLL - Win32 MFC Debug"

!ELSEIF  "$(CFG)" == "TAO DLL - Win32 Release"

!ELSEIF  "$(CFG)" == "TAO DLL - Win32 Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Remote_Object_Proxy_Broker.cpp

!IF  "$(CFG)" == "TAO DLL - Win32 Alpha Release"

!ELSEIF  "$(CFG)" == "TAO DLL - Win32 Alpha Debug"

!ELSEIF  "$(CFG)" == "TAO DLL - Win32 MFC Release"

!ELSEIF  "$(CFG)" == "TAO DLL - Win32 MFC Debug"

!ELSEIF  "$(CFG)" == "TAO DLL - Win32 Release"

!ELSEIF  "$(CFG)" == "TAO DLL - Win32 Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Remote_Object_Proxy_Impl.cpp

!IF  "$(CFG)" == "TAO DLL - Win32 Alpha Release"

!ELSEIF  "$(CFG)" == "TAO DLL - Win32 Alpha Debug"

!ELSEIF  "$(CFG)" == "TAO DLL - Win32 MFC Release"

!ELSEIF  "$(CFG)" == "TAO DLL - Win32 MFC Debug"

!ELSEIF  "$(CFG)" == "TAO DLL - Win32 Release"

!ELSEIF  "$(CFG)" == "TAO DLL - Win32 Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Reply_Dispatcher.cpp

!IF  "$(CFG)" == "TAO DLL - Win32 Alpha Release"

!ELSEIF  "$(CFG)" == "TAO DLL - Win32 Alpha Debug"

!ELSEIF  "$(CFG)" == "TAO DLL - Win32 MFC Release"

!ELSEIF  "$(CFG)" == "TAO DLL - Win32 MFC Debug"

!ELSEIF  "$(CFG)" == "TAO DLL - Win32 Release"

!ELSEIF  "$(CFG)" == "TAO DLL - Win32 Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\RequestInfo_Util.cpp

!IF  "$(CFG)" == "TAO DLL - Win32 Alpha Release"

!ELSEIF  "$(CFG)" == "TAO DLL - Win32 Alpha Debug"

!ELSEIF  "$(CFG)" == "TAO DLL - Win32 MFC Release"

!ELSEIF  "$(CFG)" == "TAO DLL - Win32 MFC Debug"

!ELSEIF  "$(CFG)" == "TAO DLL - Win32 Release"

!ELSEIF  "$(CFG)" == "TAO DLL - Win32 Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Resource_Factory.cpp

!IF  "$(CFG)" == "TAO DLL - Win32 Alpha Release"

!ELSEIF  "$(CFG)" == "TAO DLL - Win32 Alpha Debug"

!ELSEIF  "$(CFG)" == "TAO DLL - Win32 MFC Release"

!ELSEIF  "$(CFG)" == "TAO DLL - Win32 MFC Debug"

!ELSEIF  "$(CFG)" == "TAO DLL - Win32 Release"

!ELSEIF  "$(CFG)" == "TAO DLL - Win32 Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\RT_Current.cpp

!IF  "$(CFG)" == "TAO DLL - Win32 Alpha Release"

!ELSEIF  "$(CFG)" == "TAO DLL - Win32 Alpha Debug"

!ELSEIF  "$(CFG)" == "TAO DLL - Win32 MFC Release"

!ELSEIF  "$(CFG)" == "TAO DLL - Win32 MFC Debug"

!ELSEIF  "$(CFG)" == "TAO DLL - Win32 Release"

!ELSEIF  "$(CFG)" == "TAO DLL - Win32 Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\RT_Mutex.cpp

!IF  "$(CFG)" == "TAO DLL - Win32 Alpha Release"

!ELSEIF  "$(CFG)" == "TAO DLL - Win32 Alpha Debug"

!ELSEIF  "$(CFG)" == "TAO DLL - Win32 MFC Release"

!ELSEIF  "$(CFG)" == "TAO DLL - Win32 MFC Debug"

!ELSEIF  "$(CFG)" == "TAO DLL - Win32 Release"

!ELSEIF  "$(CFG)" == "TAO DLL - Win32 Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\RT_ORB.cpp

!IF  "$(CFG)" == "TAO DLL - Win32 Alpha Release"

!ELSEIF  "$(CFG)" == "TAO DLL - Win32 Alpha Debug"

!ELSEIF  "$(CFG)" == "TAO DLL - Win32 MFC Release"

!ELSEIF  "$(CFG)" == "TAO DLL - Win32 MFC Debug"

!ELSEIF  "$(CFG)" == "TAO DLL - Win32 Release"

!ELSEIF  "$(CFG)" == "TAO DLL - Win32 Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\RT_ORBInitializer.cpp

!IF  "$(CFG)" == "TAO DLL - Win32 Alpha Release"

!ELSEIF  "$(CFG)" == "TAO DLL - Win32 Alpha Debug"

!ELSEIF  "$(CFG)" == "TAO DLL - Win32 MFC Release"

!ELSEIF  "$(CFG)" == "TAO DLL - Win32 MFC Debug"

!ELSEIF  "$(CFG)" == "TAO DLL - Win32 Release"

!ELSEIF  "$(CFG)" == "TAO DLL - Win32 Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\RT_Policy_i.cpp

!IF  "$(CFG)" == "TAO DLL - Win32 Alpha Release"

!ELSEIF  "$(CFG)" == "TAO DLL - Win32 Alpha Debug"

!ELSEIF  "$(CFG)" == "TAO DLL - Win32 MFC Release"

!ELSEIF  "$(CFG)" == "TAO DLL - Win32 MFC Debug"

!ELSEIF  "$(CFG)" == "TAO DLL - Win32 Release"

!ELSEIF  "$(CFG)" == "TAO DLL - Win32 Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\RT_PolicyFactory.cpp

!IF  "$(CFG)" == "TAO DLL - Win32 Alpha Release"

!ELSEIF  "$(CFG)" == "TAO DLL - Win32 Alpha Debug"

!ELSEIF  "$(CFG)" == "TAO DLL - Win32 MFC Release"

!ELSEIF  "$(CFG)" == "TAO DLL - Win32 MFC Debug"

!ELSEIF  "$(CFG)" == "TAO DLL - Win32 Release"

!ELSEIF  "$(CFG)" == "TAO DLL - Win32 Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\RTCORBAC.cpp

!IF  "$(CFG)" == "TAO DLL - Win32 Alpha Release"

!ELSEIF  "$(CFG)" == "TAO DLL - Win32 Alpha Debug"

!ELSEIF  "$(CFG)" == "TAO DLL - Win32 MFC Release"

!ELSEIF  "$(CFG)" == "TAO DLL - Win32 MFC Debug"

!ELSEIF  "$(CFG)" == "TAO DLL - Win32 Release"

!ELSEIF  "$(CFG)" == "TAO DLL - Win32 Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\RTCORBAS.cpp

!IF  "$(CFG)" == "TAO DLL - Win32 Alpha Release"

!ELSEIF  "$(CFG)" == "TAO DLL - Win32 Alpha Debug"

!ELSEIF  "$(CFG)" == "TAO DLL - Win32 MFC Release"

!ELSEIF  "$(CFG)" == "TAO DLL - Win32 MFC Debug"

!ELSEIF  "$(CFG)" == "TAO DLL - Win32 Release"

!ELSEIF  "$(CFG)" == "TAO DLL - Win32 Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\RTCORBAS_T.cpp

!IF  "$(CFG)" == "TAO DLL - Win32 Alpha Release"

!ELSEIF  "$(CFG)" == "TAO DLL - Win32 Alpha Debug"

!ELSEIF  "$(CFG)" == "TAO DLL - Win32 MFC Release"

!ELSEIF  "$(CFG)" == "TAO DLL - Win32 MFC Debug"

!ELSEIF  "$(CFG)" == "TAO DLL - Win32 Release"

!ELSEIF  "$(CFG)" == "TAO DLL - Win32 Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Sequence.cpp

!IF  "$(CFG)" == "TAO DLL - Win32 Alpha Release"

!ELSEIF  "$(CFG)" == "TAO DLL - Win32 Alpha Debug"

!ELSEIF  "$(CFG)" == "TAO DLL - Win32 MFC Release"

!ELSEIF  "$(CFG)" == "TAO DLL - Win32 MFC Debug"

!ELSEIF  "$(CFG)" == "TAO DLL - Win32 Release"

!ELSEIF  "$(CFG)" == "TAO DLL - Win32 Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Server_Strategy_Factory.cpp

!IF  "$(CFG)" == "TAO DLL - Win32 Alpha Release"

!ELSEIF  "$(CFG)" == "TAO DLL - Win32 Alpha Debug"

!ELSEIF  "$(CFG)" == "TAO DLL - Win32 MFC Release"

!ELSEIF  "$(CFG)" == "TAO DLL - Win32 MFC Debug"

!ELSEIF  "$(CFG)" == "TAO DLL - Win32 Release"

!ELSEIF  "$(CFG)" == "TAO DLL - Win32 Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Service_Callbacks.cpp

!IF  "$(CFG)" == "TAO DLL - Win32 Alpha Release"

!ELSEIF  "$(CFG)" == "TAO DLL - Win32 Alpha Debug"

!ELSEIF  "$(CFG)" == "TAO DLL - Win32 MFC Release"

!ELSEIF  "$(CFG)" == "TAO DLL - Win32 MFC Debug"

!ELSEIF  "$(CFG)" == "TAO DLL - Win32 Release"

!ELSEIF  "$(CFG)" == "TAO DLL - Win32 Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Service_Context.cpp

!IF  "$(CFG)" == "TAO DLL - Win32 Alpha Release"

!ELSEIF  "$(CFG)" == "TAO DLL - Win32 Alpha Debug"

!ELSEIF  "$(CFG)" == "TAO DLL - Win32 MFC Release"

!ELSEIF  "$(CFG)" == "TAO DLL - Win32 MFC Debug"

!ELSEIF  "$(CFG)" == "TAO DLL - Win32 Release"

!ELSEIF  "$(CFG)" == "TAO DLL - Win32 Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Services.cpp

!IF  "$(CFG)" == "TAO DLL - Win32 Alpha Release"

!ELSEIF  "$(CFG)" == "TAO DLL - Win32 Alpha Debug"

!ELSEIF  "$(CFG)" == "TAO DLL - Win32 MFC Release"

!ELSEIF  "$(CFG)" == "TAO DLL - Win32 MFC Debug"

!ELSEIF  "$(CFG)" == "TAO DLL - Win32 Release"

!ELSEIF  "$(CFG)" == "TAO DLL - Win32 Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Services_Activate.cpp

!IF  "$(CFG)" == "TAO DLL - Win32 Alpha Release"

!ELSEIF  "$(CFG)" == "TAO DLL - Win32 Alpha Debug"

!ELSEIF  "$(CFG)" == "TAO DLL - Win32 MFC Release"

!ELSEIF  "$(CFG)" == "TAO DLL - Win32 MFC Debug"

!ELSEIF  "$(CFG)" == "TAO DLL - Win32 Release"

!ELSEIF  "$(CFG)" == "TAO DLL - Win32 Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Single_Reactor.cpp

!IF  "$(CFG)" == "TAO DLL - Win32 Alpha Release"

!ELSEIF  "$(CFG)" == "TAO DLL - Win32 Alpha Debug"

!ELSEIF  "$(CFG)" == "TAO DLL - Win32 MFC Release"

!ELSEIF  "$(CFG)" == "TAO DLL - Win32 MFC Debug"

!ELSEIF  "$(CFG)" == "TAO DLL - Win32 Release"

!ELSEIF  "$(CFG)" == "TAO DLL - Win32 Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\skip.cpp

!IF  "$(CFG)" == "TAO DLL - Win32 Alpha Release"

!ELSEIF  "$(CFG)" == "TAO DLL - Win32 Alpha Debug"

!ELSEIF  "$(CFG)" == "TAO DLL - Win32 MFC Release"

!ELSEIF  "$(CFG)" == "TAO DLL - Win32 MFC Debug"

!ELSEIF  "$(CFG)" == "TAO DLL - Win32 Release"

!ELSEIF  "$(CFG)" == "TAO DLL - Win32 Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\StringSeqC.cpp

!IF  "$(CFG)" == "TAO DLL - Win32 Alpha Release"

!ELSEIF  "$(CFG)" == "TAO DLL - Win32 Alpha Debug"

!ELSEIF  "$(CFG)" == "TAO DLL - Win32 MFC Release"

!ELSEIF  "$(CFG)" == "TAO DLL - Win32 MFC Debug"

!ELSEIF  "$(CFG)" == "TAO DLL - Win32 Release"

!ELSEIF  "$(CFG)" == "TAO DLL - Win32 Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Stub.cpp

!IF  "$(CFG)" == "TAO DLL - Win32 Alpha Release"

!ELSEIF  "$(CFG)" == "TAO DLL - Win32 Alpha Debug"

!ELSEIF  "$(CFG)" == "TAO DLL - Win32 MFC Release"

!ELSEIF  "$(CFG)" == "TAO DLL - Win32 MFC Debug"

!ELSEIF  "$(CFG)" == "TAO DLL - Win32 Release"

!ELSEIF  "$(CFG)" == "TAO DLL - Win32 Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Sync_Strategies.cpp

!IF  "$(CFG)" == "TAO DLL - Win32 Alpha Release"

!ELSEIF  "$(CFG)" == "TAO DLL - Win32 Alpha Debug"

!ELSEIF  "$(CFG)" == "TAO DLL - Win32 MFC Release"

!ELSEIF  "$(CFG)" == "TAO DLL - Win32 MFC Debug"

!ELSEIF  "$(CFG)" == "TAO DLL - Win32 Release"

!ELSEIF  "$(CFG)" == "TAO DLL - Win32 Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Synch_Reply_Dispatcher.cpp

!IF  "$(CFG)" == "TAO DLL - Win32 Alpha Release"

!ELSEIF  "$(CFG)" == "TAO DLL - Win32 Alpha Debug"

!ELSEIF  "$(CFG)" == "TAO DLL - Win32 MFC Release"

!ELSEIF  "$(CFG)" == "TAO DLL - Win32 MFC Debug"

!ELSEIF  "$(CFG)" == "TAO DLL - Win32 Release"

!ELSEIF  "$(CFG)" == "TAO DLL - Win32 Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Tagged_Components.cpp

!IF  "$(CFG)" == "TAO DLL - Win32 Alpha Release"

!ELSEIF  "$(CFG)" == "TAO DLL - Win32 Alpha Debug"

!ELSEIF  "$(CFG)" == "TAO DLL - Win32 MFC Release"

!ELSEIF  "$(CFG)" == "TAO DLL - Win32 MFC Debug"

!ELSEIF  "$(CFG)" == "TAO DLL - Win32 Release"

!ELSEIF  "$(CFG)" == "TAO DLL - Win32 Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Tagged_Profile.cpp

!IF  "$(CFG)" == "TAO DLL - Win32 Alpha Release"

!ELSEIF  "$(CFG)" == "TAO DLL - Win32 Alpha Debug"

!ELSEIF  "$(CFG)" == "TAO DLL - Win32 MFC Release"

!ELSEIF  "$(CFG)" == "TAO DLL - Win32 MFC Debug"

!ELSEIF  "$(CFG)" == "TAO DLL - Win32 Release"

!ELSEIF  "$(CFG)" == "TAO DLL - Win32 Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\TAO_Internal.cpp

!IF  "$(CFG)" == "TAO DLL - Win32 Alpha Release"

!ELSEIF  "$(CFG)" == "TAO DLL - Win32 Alpha Debug"

!ELSEIF  "$(CFG)" == "TAO DLL - Win32 MFC Release"

!ELSEIF  "$(CFG)" == "TAO DLL - Win32 MFC Debug"

!ELSEIF  "$(CFG)" == "TAO DLL - Win32 Release"

!ELSEIF  "$(CFG)" == "TAO DLL - Win32 Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\TAO_Server_Request.cpp

!IF  "$(CFG)" == "TAO DLL - Win32 Alpha Release"

!ELSEIF  "$(CFG)" == "TAO DLL - Win32 Alpha Debug"

!ELSEIF  "$(CFG)" == "TAO DLL - Win32 MFC Release"

!ELSEIF  "$(CFG)" == "TAO DLL - Win32 MFC Debug"

!ELSEIF  "$(CFG)" == "TAO DLL - Win32 Release"

!ELSEIF  "$(CFG)" == "TAO DLL - Win32 Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\TAO_Singleton_Manager.cpp

!IF  "$(CFG)" == "TAO DLL - Win32 Alpha Release"

!ELSEIF  "$(CFG)" == "TAO DLL - Win32 Alpha Debug"

!ELSEIF  "$(CFG)" == "TAO DLL - Win32 MFC Release"

!ELSEIF  "$(CFG)" == "TAO DLL - Win32 MFC Debug"

!ELSEIF  "$(CFG)" == "TAO DLL - Win32 Release"

!ELSEIF  "$(CFG)" == "TAO DLL - Win32 Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\TAOC.cpp

!IF  "$(CFG)" == "TAO DLL - Win32 Alpha Release"

!ELSEIF  "$(CFG)" == "TAO DLL - Win32 Alpha Debug"

!ELSEIF  "$(CFG)" == "TAO DLL - Win32 MFC Release"

!ELSEIF  "$(CFG)" == "TAO DLL - Win32 MFC Debug"

!ELSEIF  "$(CFG)" == "TAO DLL - Win32 Release"

!ELSEIF  "$(CFG)" == "TAO DLL - Win32 Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\TAOS.cpp

!IF  "$(CFG)" == "TAO DLL - Win32 Alpha Release"

!ELSEIF  "$(CFG)" == "TAO DLL - Win32 Alpha Debug"

!ELSEIF  "$(CFG)" == "TAO DLL - Win32 MFC Release"

!ELSEIF  "$(CFG)" == "TAO DLL - Win32 MFC Debug"

!ELSEIF  "$(CFG)" == "TAO DLL - Win32 Release"

!ELSEIF  "$(CFG)" == "TAO DLL - Win32 Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\target_specification.cpp

!IF  "$(CFG)" == "TAO DLL - Win32 Alpha Release"

!ELSEIF  "$(CFG)" == "TAO DLL - Win32 Alpha Debug"

!ELSEIF  "$(CFG)" == "TAO DLL - Win32 MFC Release"

!ELSEIF  "$(CFG)" == "TAO DLL - Win32 MFC Debug"

!ELSEIF  "$(CFG)" == "TAO DLL - Win32 Release"

!ELSEIF  "$(CFG)" == "TAO DLL - Win32 Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\TimeBaseC.cpp

!IF  "$(CFG)" == "TAO DLL - Win32 Alpha Release"

!ELSEIF  "$(CFG)" == "TAO DLL - Win32 Alpha Debug"

!ELSEIF  "$(CFG)" == "TAO DLL - Win32 MFC Release"

!ELSEIF  "$(CFG)" == "TAO DLL - Win32 MFC Debug"

!ELSEIF  "$(CFG)" == "TAO DLL - Win32 Release"

!ELSEIF  "$(CFG)" == "TAO DLL - Win32 Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\TimeBaseS.cpp

!IF  "$(CFG)" == "TAO DLL - Win32 Alpha Release"

!ELSEIF  "$(CFG)" == "TAO DLL - Win32 Alpha Debug"

!ELSEIF  "$(CFG)" == "TAO DLL - Win32 MFC Release"

!ELSEIF  "$(CFG)" == "TAO DLL - Win32 MFC Debug"

!ELSEIF  "$(CFG)" == "TAO DLL - Win32 Release"

!ELSEIF  "$(CFG)" == "TAO DLL - Win32 Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Transport_Mux_Strategy.cpp

!IF  "$(CFG)" == "TAO DLL - Win32 Alpha Release"

!ELSEIF  "$(CFG)" == "TAO DLL - Win32 Alpha Debug"

!ELSEIF  "$(CFG)" == "TAO DLL - Win32 MFC Release"

!ELSEIF  "$(CFG)" == "TAO DLL - Win32 MFC Debug"

!ELSEIF  "$(CFG)" == "TAO DLL - Win32 Release"

!ELSEIF  "$(CFG)" == "TAO DLL - Win32 Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Typecode.cpp

!IF  "$(CFG)" == "TAO DLL - Win32 Alpha Release"

!ELSEIF  "$(CFG)" == "TAO DLL - Win32 Alpha Debug"

!ELSEIF  "$(CFG)" == "TAO DLL - Win32 MFC Release"

!ELSEIF  "$(CFG)" == "TAO DLL - Win32 MFC Debug"

!ELSEIF  "$(CFG)" == "TAO DLL - Win32 Release"

!ELSEIF  "$(CFG)" == "TAO DLL - Win32 Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Typecode_Constants.cpp

!IF  "$(CFG)" == "TAO DLL - Win32 Alpha Release"

!ELSEIF  "$(CFG)" == "TAO DLL - Win32 Alpha Debug"

!ELSEIF  "$(CFG)" == "TAO DLL - Win32 MFC Release"

!ELSEIF  "$(CFG)" == "TAO DLL - Win32 MFC Debug"

!ELSEIF  "$(CFG)" == "TAO DLL - Win32 Release"

!ELSEIF  "$(CFG)" == "TAO DLL - Win32 Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\ValueBase.cpp

!IF  "$(CFG)" == "TAO DLL - Win32 Alpha Release"

!ELSEIF  "$(CFG)" == "TAO DLL - Win32 Alpha Debug"

!ELSEIF  "$(CFG)" == "TAO DLL - Win32 MFC Release"

!ELSEIF  "$(CFG)" == "TAO DLL - Win32 MFC Debug"

!ELSEIF  "$(CFG)" == "TAO DLL - Win32 Release"

!ELSEIF  "$(CFG)" == "TAO DLL - Win32 Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\ValueFactory.cpp

!IF  "$(CFG)" == "TAO DLL - Win32 Alpha Release"

!ELSEIF  "$(CFG)" == "TAO DLL - Win32 Alpha Debug"

!ELSEIF  "$(CFG)" == "TAO DLL - Win32 MFC Release"

!ELSEIF  "$(CFG)" == "TAO DLL - Win32 MFC Debug"

!ELSEIF  "$(CFG)" == "TAO DLL - Win32 Release"

!ELSEIF  "$(CFG)" == "TAO DLL - Win32 Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\ValueFactory_Map.cpp

!IF  "$(CFG)" == "TAO DLL - Win32 Alpha Release"

!ELSEIF  "$(CFG)" == "TAO DLL - Win32 Alpha Debug"

!ELSEIF  "$(CFG)" == "TAO DLL - Win32 MFC Release"

!ELSEIF  "$(CFG)" == "TAO DLL - Win32 MFC Debug"

!ELSEIF  "$(CFG)" == "TAO DLL - Win32 Release"

!ELSEIF  "$(CFG)" == "TAO DLL - Win32 Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Wait_On_Leader_Follower.cpp

!IF  "$(CFG)" == "TAO DLL - Win32 Alpha Release"

!ELSEIF  "$(CFG)" == "TAO DLL - Win32 Alpha Debug"

!ELSEIF  "$(CFG)" == "TAO DLL - Win32 MFC Release"

!ELSEIF  "$(CFG)" == "TAO DLL - Win32 MFC Debug"

!ELSEIF  "$(CFG)" == "TAO DLL - Win32 Release"

!ELSEIF  "$(CFG)" == "TAO DLL - Win32 Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Wait_On_Reactor.cpp

!IF  "$(CFG)" == "TAO DLL - Win32 Alpha Release"

!ELSEIF  "$(CFG)" == "TAO DLL - Win32 Alpha Debug"

!ELSEIF  "$(CFG)" == "TAO DLL - Win32 MFC Release"

!ELSEIF  "$(CFG)" == "TAO DLL - Win32 MFC Debug"

!ELSEIF  "$(CFG)" == "TAO DLL - Win32 Release"

!ELSEIF  "$(CFG)" == "TAO DLL - Win32 Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Wait_On_Read.cpp

!IF  "$(CFG)" == "TAO DLL - Win32 Alpha Release"

!ELSEIF  "$(CFG)" == "TAO DLL - Win32 Alpha Debug"

!ELSEIF  "$(CFG)" == "TAO DLL - Win32 MFC Release"

!ELSEIF  "$(CFG)" == "TAO DLL - Win32 MFC Debug"

!ELSEIF  "$(CFG)" == "TAO DLL - Win32 Release"

!ELSEIF  "$(CFG)" == "TAO DLL - Win32 Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Wait_Strategy.cpp

!IF  "$(CFG)" == "TAO DLL - Win32 Alpha Release"

!ELSEIF  "$(CFG)" == "TAO DLL - Win32 Alpha Debug"

!ELSEIF  "$(CFG)" == "TAO DLL - Win32 MFC Release"

!ELSEIF  "$(CFG)" == "TAO DLL - Win32 MFC Debug"

!ELSEIF  "$(CFG)" == "TAO DLL - Win32 Release"

!ELSEIF  "$(CFG)" == "TAO DLL - Win32 Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\WrongTransactionC.cpp

!IF  "$(CFG)" == "TAO DLL - Win32 Alpha Release"

!ELSEIF  "$(CFG)" == "TAO DLL - Win32 Alpha Debug"

!ELSEIF  "$(CFG)" == "TAO DLL - Win32 MFC Release"

!ELSEIF  "$(CFG)" == "TAO DLL - Win32 MFC Debug"

!ELSEIF  "$(CFG)" == "TAO DLL - Win32 Release"

!ELSEIF  "$(CFG)" == "TAO DLL - Win32 Debug"

!ENDIF 

# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl;fi;fd"
# Begin Source File

SOURCE=.\Abstract_Servant_Base.h
# End Source File
# Begin Source File

SOURCE=.\Acceptor_Filter.h
# End Source File
# Begin Source File

SOURCE=.\Acceptor_Registry.h
# End Source File
# Begin Source File

SOURCE=.\Adapter.h
# End Source File
# Begin Source File

SOURCE=.\any.h
# End Source File
# Begin Source File

SOURCE=.\arg_shifter.h
# End Source File
# Begin Source File

SOURCE=.\Asynch_Invocation.h
# End Source File
# Begin Source File

SOURCE=.\Asynch_Reply_Dispatcher.h
# End Source File
# Begin Source File

SOURCE=.\Base_Connection_Property.h
# End Source File
# Begin Source File

SOURCE=.\BiDir_ORBInitializer.h
# End Source File
# Begin Source File

SOURCE=.\BiDir_Policy_i.h
# End Source File
# Begin Source File

SOURCE=.\BiDir_PolicyFactory.h
# End Source File
# Begin Source File

SOURCE=.\BiDirPolicyC.h
# End Source File
# Begin Source File

SOURCE=.\Bind_Dispatcher_Guard.h
# End Source File
# Begin Source File

SOURCE=.\BoundsC.h
# End Source File
# Begin Source File

SOURCE=.\Buffering_Constraint_Policy.h
# End Source File
# Begin Source File

SOURCE=.\Cache_Entries.h
# End Source File
# Begin Source File

SOURCE=.\cdr.h
# End Source File
# Begin Source File

SOURCE=.\Cleanup_Func_Registry.h
# End Source File
# Begin Source File

SOURCE=.\Cleanup_Func_Registry.inl
# End Source File
# Begin Source File

SOURCE=.\Client_Priority_Policy.h
# End Source File
# Begin Source File

SOURCE=.\Client_Strategy_Factory.h
# End Source File
# Begin Source File

SOURCE=.\ClientRequestInfo.h
# End Source File
# Begin Source File

SOURCE=.\Connection_Cache_Manager.h
# End Source File
# Begin Source File

SOURCE=.\Connection_Descriptor_Interface.h
# End Source File
# Begin Source File

SOURCE=.\Connection_Handler.h
# End Source File
# Begin Source File

SOURCE=.\Connector_Impl.h
# End Source File
# Begin Source File

SOURCE=.\Connector_Registry.h
# End Source File
# Begin Source File

SOURCE=.\CONV_FRAMEC.h
# End Source File
# Begin Source File

SOURCE=.\CONV_FRAMES.h
# End Source File
# Begin Source File

SOURCE=.\CONV_FRAMES_T.h
# End Source File
# Begin Source File

SOURCE=.\corba.h
# End Source File
# Begin Source File

SOURCE=.\CORBA_String.h
# End Source File
# Begin Source File

SOURCE=.\corbafwd.h
# End Source File
# Begin Source File

SOURCE=.\CORBALOC_Parser.h
# End Source File
# Begin Source File

SOURCE=.\CORBANAME_Parser.h
# End Source File
# Begin Source File

SOURCE=.\CurrentC.h
# End Source File
# Begin Source File

SOURCE=.\debug.h
# End Source File
# Begin Source File

SOURCE=.\default_client.h
# End Source File
# Begin Source File

SOURCE=.\default_resource.h
# End Source File
# Begin Source File

SOURCE=.\default_server.h
# End Source File
# Begin Source File

SOURCE=.\DLL_ORB.h
# End Source File
# Begin Source File

SOURCE=.\DLL_Parser.h
# End Source File
# Begin Source File

SOURCE=.\DomainC.h
# End Source File
# Begin Source File

SOURCE=.\Dynamic_Adapter.h
# End Source File
# Begin Source File

SOURCE=.\DynamicC.h
# End Source File
# Begin Source File

SOURCE=.\Encodable.h
# End Source File
# Begin Source File

SOURCE=.\Endpoint.h
# End Source File
# Begin Source File

SOURCE=.\Environment.h
# End Source File
# Begin Source File

SOURCE=.\Exception.h
# End Source File
# Begin Source File

SOURCE=.\Exclusive_TMS.h
# End Source File
# Begin Source File

SOURCE=.\Fault_Tolerance_Service.h
# End Source File
# Begin Source File

SOURCE=.\FILE_Parser.h
# End Source File
# Begin Source File

SOURCE=.\giop.h
# End Source File
# Begin Source File

SOURCE=.\GIOP_Message_Base.h
# End Source File
# Begin Source File

SOURCE=.\GIOP_Message_Generator_Parser.h
# End Source File
# Begin Source File

SOURCE=.\GIOP_Message_Generator_Parser_10.h
# End Source File
# Begin Source File

SOURCE=.\GIOP_Message_Generator_Parser_11.h
# End Source File
# Begin Source File

SOURCE=.\GIOP_Message_Generator_Parser_12.h
# End Source File
# Begin Source File

SOURCE=.\GIOP_Message_Generator_Parser_Impl.h
# End Source File
# Begin Source File

SOURCE=.\GIOP_Message_Lite.h
# End Source File
# Begin Source File

SOURCE=.\GIOP_Message_Locate_Header.h
# End Source File
# Begin Source File

SOURCE=.\GIOP_Message_State.h
# End Source File
# Begin Source File

SOURCE=.\GIOP_Utils.h
# End Source File
# Begin Source File

SOURCE=.\GIOPC.h
# End Source File
# Begin Source File

SOURCE=.\GIOPS_T.h
# End Source File
# Begin Source File

SOURCE=.\ifrfwd.h
# End Source File
# Begin Source File

SOURCE=.\IIOP_Acceptor.h
# End Source File
# Begin Source File

SOURCE=.\IIOP_Connection_Handler.h
# End Source File
# Begin Source File

SOURCE=.\IIOP_Connector.h
# End Source File
# Begin Source File

SOURCE=.\IIOP_Endpoint.h
# End Source File
# Begin Source File

SOURCE=.\iiop_endpoints.h
# End Source File
# Begin Source File

SOURCE=.\IIOP_Factory.h
# End Source File
# Begin Source File

SOURCE=.\IIOP_Lite_Factory.h
# End Source File
# Begin Source File

SOURCE=.\IIOP_Profile.h
# End Source File
# Begin Source File

SOURCE=.\IIOP_Transport.h
# End Source File
# Begin Source File

SOURCE=.\IIOPC.h
# End Source File
# Begin Source File

SOURCE=.\Interceptor_List.h
# End Source File
# Begin Source File

SOURCE=.\InterfaceC.h
# End Source File
# Begin Source File

SOURCE=.\Invocation.h
# End Source File
# Begin Source File

SOURCE=.\Invocation_Endpoint_Selectors.h
# End Source File
# Begin Source File

SOURCE=.\IOPC.h
# End Source File
# Begin Source File

SOURCE=.\IOPS.h
# End Source File
# Begin Source File

SOURCE=.\IOPS_T.h
# End Source File
# Begin Source File

SOURCE=.\ior_corbaloc_export.h
# End Source File
# Begin Source File

SOURCE=.\ior_corbaname_export.h
# End Source File
# Begin Source File

SOURCE=.\IOR_Parser.h
# End Source File
# Begin Source File

SOURCE=.\IORInfo.h
# End Source File
# Begin Source File

SOURCE=.\Leader_Follower.h
# End Source File
# Begin Source File

SOURCE=.\LocalObject.h
# End Source File
# Begin Source File

SOURCE=.\Managed_Types.h
# End Source File
# Begin Source File

SOURCE=.\marshal.h
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

SOURCE=.\MessagingC.h
# End Source File
# Begin Source File

SOURCE=.\MessagingS.h
# End Source File
# Begin Source File

SOURCE=.\MProfile.h
# End Source File
# Begin Source File

SOURCE=.\Muxed_TMS.h
# End Source File
# Begin Source File

SOURCE=.\nvlist.h
# End Source File
# Begin Source File

SOURCE=.\object.h
# End Source File
# Begin Source File

SOURCE=.\Object_KeyC.h
# End Source File
# Begin Source File

SOURCE=.\Object_Loader.h
# End Source File
# Begin Source File

SOURCE=.\Object_Proxy_Broker.h
# End Source File
# Begin Source File

SOURCE=.\Object_Proxy_Impl.h
# End Source File
# Begin Source File

SOURCE=.\Object_Ref_Table.h
# End Source File
# Begin Source File

SOURCE=.\ObjectIDList.h
# End Source File
# Begin Source File

SOURCE=.\OctetSeqC.h
# End Source File
# Begin Source File

SOURCE=.\operation_details.h
# End Source File
# Begin Source File

SOURCE=.\orb.h
# End Source File
# Begin Source File

SOURCE=.\orb_core.h
# End Source File
# Begin Source File

SOURCE=.\ORB_Strategies_T.h
# End Source File
# Begin Source File

SOURCE=.\ORB_Table.h
# End Source File
# Begin Source File

SOURCE=.\orbconf.h
# End Source File
# Begin Source File

SOURCE=.\ORBInitializer_Registry.h
# End Source File
# Begin Source File

SOURCE=.\ORBInitInfo.h
# End Source File
# Begin Source File

SOURCE=.\params.h
# End Source File
# Begin Source File

SOURCE=.\Parser_Registry.h
# End Source File
# Begin Source File

SOURCE=.\Pluggable.h
# End Source File
# Begin Source File

SOURCE=.\Pluggable_Messaging.h
# End Source File
# Begin Source File

SOURCE=.\Pluggable_Messaging_Utils.h
# End Source File
# Begin Source File

SOURCE=.\Policy_Manager.h
# End Source File
# Begin Source File

SOURCE=.\PolicyC.h
# End Source File
# Begin Source File

SOURCE=.\PolicyFactory_Registry.h
# End Source File
# Begin Source File

SOURCE=.\PollableC.h
# End Source File
# Begin Source File

SOURCE=.\PollableS.h
# End Source File
# Begin Source File

SOURCE=.\PollableS_T.h
# End Source File
# Begin Source File

SOURCE=.\Pool_Per_Endpoint.h
# End Source File
# Begin Source File

SOURCE=.\PortableInterceptor.h
# End Source File
# Begin Source File

SOURCE=.\PortableInterceptorC.h
# End Source File
# Begin Source File

SOURCE=.\Principal.h
# End Source File
# Begin Source File

SOURCE=.\Priority_Mapping.h
# End Source File
# Begin Source File

SOURCE=.\Priority_Mapping_Manager.h
# End Source File
# Begin Source File

SOURCE=.\Private_Connection_Descriptor.h
# End Source File
# Begin Source File

SOURCE=.\Profile.h
# End Source File
# Begin Source File

SOURCE=.\Protocol_Factory.h
# End Source File
# Begin Source File

SOURCE=.\Reactor_Registry.h
# End Source File
# Begin Source File

SOURCE=.\Remote_Object_Proxy_Broker.h
# End Source File
# Begin Source File

SOURCE=.\Remote_Object_Proxy_Impl.h
# End Source File
# Begin Source File

SOURCE=.\Reply_Dispatcher.h
# End Source File
# Begin Source File

SOURCE=.\RequestInfo_Util.h
# End Source File
# Begin Source File

SOURCE=.\Resource_Factory.h
# End Source File
# Begin Source File

SOURCE=.\RT_Current.h
# End Source File
# Begin Source File

SOURCE=.\RT_Mutex.h
# End Source File
# Begin Source File

SOURCE=.\RT_ORB.h
# End Source File
# Begin Source File

SOURCE=.\RT_ORBInitializer.h
# End Source File
# Begin Source File

SOURCE=.\RT_Policy_i.h
# End Source File
# Begin Source File

SOURCE=.\RT_PolicyFactory.h
# End Source File
# Begin Source File

SOURCE=.\RTCORBAC.h
# End Source File
# Begin Source File

SOURCE=.\RTCORBAS.h
# End Source File
# Begin Source File

SOURCE=.\RTCORBAS_T.h
# End Source File
# Begin Source File

SOURCE=.\sequence.h
# End Source File
# Begin Source File

SOURCE=.\Sequence_T.h
# End Source File
# Begin Source File

SOURCE=.\Server_Strategy_Factory.h
# End Source File
# Begin Source File

SOURCE=.\Service_Callbacks.h
# End Source File
# Begin Source File

SOURCE=.\Service_Context.h
# End Source File
# Begin Source File

SOURCE=.\Services.h
# End Source File
# Begin Source File

SOURCE=.\Services_Activate.h
# End Source File
# Begin Source File

SOURCE=.\Single_Reactor.h
# End Source File
# Begin Source File

SOURCE=.\singletons.h
# End Source File
# Begin Source File

SOURCE=.\StringSeqC.h
# End Source File
# Begin Source File

SOURCE=.\stub.h
# End Source File
# Begin Source File

SOURCE=.\Sync_Strategies.h
# End Source File
# Begin Source File

SOURCE=.\Synch_Reply_Dispatcher.h
# End Source File
# Begin Source File

SOURCE=.\Tagged_Components.h
# End Source File
# Begin Source File

SOURCE=.\Tagged_Profile.h
# End Source File
# Begin Source File

SOURCE=.\TAO_Export.h
# End Source File
# Begin Source File

SOURCE=.\TAO_Internal.h
# End Source File
# Begin Source File

SOURCE=.\TAO_Server_Request.h
# End Source File
# Begin Source File

SOURCE=.\TAO_Singleton.h
# End Source File
# Begin Source File

SOURCE=.\TAO_Singleton_Manager.h
# End Source File
# Begin Source File

SOURCE=.\TAOC.h
# End Source File
# Begin Source File

SOURCE=.\TAOS.h
# End Source File
# Begin Source File

SOURCE=.\TAOS_T.h
# End Source File
# Begin Source File

SOURCE=.\target_specification.h
# End Source File
# Begin Source File

SOURCE=.\TimeBaseC.h
# End Source File
# Begin Source File

SOURCE=.\TimeBaseS.h
# End Source File
# Begin Source File

SOURCE=.\TimeBaseS_T.h
# End Source File
# Begin Source File

SOURCE=.\Timeprobe.h
# End Source File
# Begin Source File

SOURCE=.\Transport_Mux_Strategy.h
# End Source File
# Begin Source File

SOURCE=.\try_macros.h
# End Source File
# Begin Source File

SOURCE=.\typecode.h
# End Source File
# Begin Source File

SOURCE=.\ValueBase.h
# End Source File
# Begin Source File

SOURCE=.\ValueFactory.h
# End Source File
# Begin Source File

SOURCE=.\ValueFactory_Map.h
# End Source File
# Begin Source File

SOURCE=.\varbase.h
# End Source File
# Begin Source File

SOURCE=.\Wait_On_Leader_Follower.h
# End Source File
# Begin Source File

SOURCE=.\Wait_On_Reactor.h
# End Source File
# Begin Source File

SOURCE=.\Wait_On_Read.h
# End Source File
# Begin Source File

SOURCE=.\Wait_Strategy.h
# End Source File
# Begin Source File

SOURCE=.\WrongTransactionC.h
# End Source File
# End Group
# Begin Group "Inline Files"

# PROP Default_Filter "i"
# Begin Source File

SOURCE=.\Acceptor_Filter.i
# End Source File
# Begin Source File

SOURCE=.\Acceptor_Registry.i
# End Source File
# Begin Source File

SOURCE=.\Adapter.i
# End Source File
# Begin Source File

SOURCE=.\any.i
# End Source File
# Begin Source File

SOURCE=.\Asynch_Invocation.i
# End Source File
# Begin Source File

SOURCE=.\Asynch_Reply_Dispatcher.i
# End Source File
# Begin Source File

SOURCE=.\Base_Connection_Property.i
# End Source File
# Begin Source File

SOURCE=.\BiDir_Policy_i.inl
# End Source File
# Begin Source File

SOURCE=.\BiDirPolicyC.i
# End Source File
# Begin Source File

SOURCE=.\Bind_Dispatcher_Guard.i
# End Source File
# Begin Source File

SOURCE=.\BoundsC.i
# End Source File
# Begin Source File

SOURCE=.\Buffering_Constraint_Policy.i
# End Source File
# Begin Source File

SOURCE=.\Cache_Entries.i
# End Source File
# Begin Source File

SOURCE=.\cdr.i
# End Source File
# Begin Source File

SOURCE=.\Client_Priority_Policy.i
# End Source File
# Begin Source File

SOURCE=.\Connection_Cache_Manager.i
# End Source File
# Begin Source File

SOURCE=.\Connection_Descriptor_Interface.inl
# End Source File
# Begin Source File

SOURCE=.\Connection_Handler.i
# End Source File
# Begin Source File

SOURCE=.\CONV_FRAMEC.i
# End Source File
# Begin Source File

SOURCE=.\CONV_FRAMES.i
# End Source File
# Begin Source File

SOURCE=.\CONV_FRAMES_T.i
# End Source File
# Begin Source File

SOURCE=.\CORBA_String.inl
# End Source File
# Begin Source File

SOURCE=.\corbafwd.i
# End Source File
# Begin Source File

SOURCE=.\CORBALOC_Parser.i
# End Source File
# Begin Source File

SOURCE=.\CORBANAME_Parser.i
# End Source File
# Begin Source File

SOURCE=.\CurrentC.i
# End Source File
# Begin Source File

SOURCE=.\default_client.i
# End Source File
# Begin Source File

SOURCE=.\default_resource.i
# End Source File
# Begin Source File

SOURCE=.\default_server.i
# End Source File
# Begin Source File

SOURCE=.\DLL_ORB.inl
# End Source File
# Begin Source File

SOURCE=.\DLL_Parser.i
# End Source File
# Begin Source File

SOURCE=.\DomainC.i
# End Source File
# Begin Source File

SOURCE=.\DynamicC.i
# End Source File
# Begin Source File

SOURCE=.\Endpoint.i
# End Source File
# Begin Source File

SOURCE=.\Environment.i
# End Source File
# Begin Source File

SOURCE=.\Exception.i
# End Source File
# Begin Source File

SOURCE=.\Fault_Tolerance_Service.i
# End Source File
# Begin Source File

SOURCE=.\FILE_Parser.i
# End Source File
# Begin Source File

SOURCE=.\giop.i
# End Source File
# Begin Source File

SOURCE=.\GIOP_Message_Base.i
# End Source File
# Begin Source File

SOURCE=.\GIOP_Message_Generator_Parser.inl
# End Source File
# Begin Source File

SOURCE=.\GIOP_Message_Generator_Parser_10.inl
# End Source File
# Begin Source File

SOURCE=.\GIOP_Message_Generator_Parser_11.inl
# End Source File
# Begin Source File

SOURCE=.\GIOP_Message_Generator_Parser_12.inl
# End Source File
# Begin Source File

SOURCE=.\GIOP_Message_Generator_Parser_Impl.inl
# End Source File
# Begin Source File

SOURCE=.\GIOP_Message_Lite.i
# End Source File
# Begin Source File

SOURCE=.\GIOP_Message_Locate_Header.i
# End Source File
# Begin Source File

SOURCE=.\GIOP_Message_State.i
# End Source File
# Begin Source File

SOURCE=.\GIOP_Utils.i
# End Source File
# Begin Source File

SOURCE=.\GIOPC.i
# End Source File
# Begin Source File

SOURCE=.\GIOPS.i
# End Source File
# Begin Source File

SOURCE=.\GIOPS_T.i
# End Source File
# Begin Source File

SOURCE=.\IIOP_Acceptor.i
# End Source File
# Begin Source File

SOURCE=.\IIOP_Connection_Handler.i
# End Source File
# Begin Source File

SOURCE=.\IIOP_Endpoint.i
# End Source File
# Begin Source File

SOURCE=.\iiop_endpoints.i
# End Source File
# Begin Source File

SOURCE=.\IIOP_Profile.i
# End Source File
# Begin Source File

SOURCE=.\IIOP_Transport.i
# End Source File
# Begin Source File

SOURCE=.\IIOPC.i
# End Source File
# Begin Source File

SOURCE=.\Interceptor_List.inl
# End Source File
# Begin Source File

SOURCE=.\InterfaceC.i
# End Source File
# Begin Source File

SOURCE=.\Invocation.i
# End Source File
# Begin Source File

SOURCE=.\Invocation_Endpoint_Selectors.i
# End Source File
# Begin Source File

SOURCE=.\IOPC.i
# End Source File
# Begin Source File

SOURCE=.\IOPS.i
# End Source File
# Begin Source File

SOURCE=.\IOPS_T.i
# End Source File
# Begin Source File

SOURCE=.\IOR_Parser.i
# End Source File
# Begin Source File

SOURCE=.\Leader_Follower.i
# End Source File
# Begin Source File

SOURCE=.\LocalObject.i
# End Source File
# Begin Source File

SOURCE=.\Managed_Types.i
# End Source File
# Begin Source File

SOURCE=.\marshal.i
# End Source File
# Begin Source File

SOURCE=.\Messaging_Policy_i.i
# End Source File
# Begin Source File

SOURCE=.\MessagingC.i
# End Source File
# Begin Source File

SOURCE=.\MessagingS.i
# End Source File
# Begin Source File

SOURCE=.\MProfile.i
# End Source File
# Begin Source File

SOURCE=.\NVList.i
# End Source File
# Begin Source File

SOURCE=.\object.i
# End Source File
# Begin Source File

SOURCE=.\Object_KeyC.i
# End Source File
# Begin Source File

SOURCE=.\Object_Loader.i
# End Source File
# Begin Source File

SOURCE=.\ObjectIDList.i
# End Source File
# Begin Source File

SOURCE=.\OctetSeqC.i
# End Source File
# Begin Source File

SOURCE=.\operation_details.i
# End Source File
# Begin Source File

SOURCE=.\ORB.i
# End Source File
# Begin Source File

SOURCE=.\orb_core.i
# End Source File
# Begin Source File

SOURCE=.\ORB_Strategies_T.i
# End Source File
# Begin Source File

SOURCE=.\ORB_Table.inl
# End Source File
# Begin Source File

SOURCE=.\ORBInitializer_Registry.inl
# End Source File
# Begin Source File

SOURCE=.\params.i
# End Source File
# Begin Source File

SOURCE=.\Parser_Registry.i
# End Source File
# Begin Source File

SOURCE=.\Pluggable.i
# End Source File
# Begin Source File

SOURCE=.\Pluggable_Messaging.i
# End Source File
# Begin Source File

SOURCE=.\Pluggable_Messaging_Utils.i
# End Source File
# Begin Source File

SOURCE=.\Policy_Manager.i
# End Source File
# Begin Source File

SOURCE=.\PolicyC.i
# End Source File
# Begin Source File

SOURCE=.\PollableC.i
# End Source File
# Begin Source File

SOURCE=.\PollableS.i
# End Source File
# Begin Source File

SOURCE=.\PollableS_T.i
# End Source File
# Begin Source File

SOURCE=.\Pool_Per_Endpoint.i
# End Source File
# Begin Source File

SOURCE=.\PortableInterceptor.i
# End Source File
# Begin Source File

SOURCE=.\PortableInterceptorC.i
# End Source File
# Begin Source File

SOURCE=.\Principal.i
# End Source File
# Begin Source File

SOURCE=.\Priority_Mapping.i
# End Source File
# Begin Source File

SOURCE=.\Priority_Mapping_Manager.i
# End Source File
# Begin Source File

SOURCE=.\Private_Connection_Descriptor.inl
# End Source File
# Begin Source File

SOURCE=.\Profile.i
# End Source File
# Begin Source File

SOURCE=.\Reactor_Registry.i
# End Source File
# Begin Source File

SOURCE=.\Reply_Dispatcher.i
# End Source File
# Begin Source File

SOURCE=.\RT_Current.i
# End Source File
# Begin Source File

SOURCE=.\RT_Mutex.i
# End Source File
# Begin Source File

SOURCE=.\RT_ORB.i
# End Source File
# Begin Source File

SOURCE=.\RT_Policy_i.i
# End Source File
# Begin Source File

SOURCE=.\RTCORBAC.i
# End Source File
# Begin Source File

SOURCE=.\RTCORBAS.i
# End Source File
# Begin Source File

SOURCE=.\RTCORBAS_T.i
# End Source File
# Begin Source File

SOURCE=.\sequence.i
# End Source File
# Begin Source File

SOURCE=.\Sequence_T.i
# End Source File
# Begin Source File

SOURCE=.\Service_Callbacks.i
# End Source File
# Begin Source File

SOURCE=.\Service_Context.inl
# End Source File
# Begin Source File

SOURCE=.\Services.i
# End Source File
# Begin Source File

SOURCE=.\Single_Reactor.i
# End Source File
# Begin Source File

SOURCE=.\StringSeqC.i
# End Source File
# Begin Source File

SOURCE=.\stub.i
# End Source File
# Begin Source File

SOURCE=.\Sync_Strategies.i
# End Source File
# Begin Source File

SOURCE=.\Tagged_Components.i
# End Source File
# Begin Source File

SOURCE=.\Tagged_Profile.i
# End Source File
# Begin Source File

SOURCE=.\TAO_Internal.i
# End Source File
# Begin Source File

SOURCE=.\TAO_Server_Request.i
# End Source File
# Begin Source File

SOURCE=.\TAO_Singleton.inl
# End Source File
# Begin Source File

SOURCE=.\TAO_Singleton_Manager.inl
# End Source File
# Begin Source File

SOURCE=.\TAOC.i
# End Source File
# Begin Source File

SOURCE=.\TAOS.i
# End Source File
# Begin Source File

SOURCE=.\TAOS_T.i
# End Source File
# Begin Source File

SOURCE=.\target_specification.i
# End Source File
# Begin Source File

SOURCE=.\TimeBaseC.i
# End Source File
# Begin Source File

SOURCE=.\TimeBaseS.i
# End Source File
# Begin Source File

SOURCE=.\TimeBaseS_T.i
# End Source File
# Begin Source File

SOURCE=.\typecode.i
# End Source File
# Begin Source File

SOURCE=.\ValueBase.i
# End Source File
# Begin Source File

SOURCE=.\ValueFactory.i
# End Source File
# Begin Source File

SOURCE=.\ValueFactory_Map.i
# End Source File
# End Group
# Begin Group "Resource files"

# PROP Default_Filter "rc"
# Begin Source File

SOURCE=.\tao.rc
# End Source File
# End Group
# Begin Group "Template Files"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\TAO_Singleton.cpp

!IF  "$(CFG)" == "TAO DLL - Win32 Alpha Release"

!ELSEIF  "$(CFG)" == "TAO DLL - Win32 Alpha Debug"

!ELSEIF  "$(CFG)" == "TAO DLL - Win32 MFC Release"

!ELSEIF  "$(CFG)" == "TAO DLL - Win32 MFC Debug"

!ELSEIF  "$(CFG)" == "TAO DLL - Win32 Release"

!ELSEIF  "$(CFG)" == "TAO DLL - Win32 Debug"

!ENDIF 

# End Source File
# End Group
# End Target
# End Project
