# Microsoft Developer Studio Project File - Name="TAO" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Dynamic-Link Library" 0x0102
# TARGTYPE "Win32 (ALPHA) Dynamic-Link Library" 0x0602

CFG=TAO - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "TAO.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "TAO.mak" CFG="TAO - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "TAO - Win32 Release" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "TAO - Win32 Debug" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "TAO - Win32 Alpha Release" (based on\
 "Win32 (ALPHA) Dynamic-Link Library")
!MESSAGE "TAO - Win32 Alpha Debug" (based on\
 "Win32 (ALPHA) Dynamic-Link Library")
!MESSAGE 

# Begin Project
# PROP Scc_ProjName ""
# PROP Scc_LocalPath "Desktop"

!IF  "$(CFG)" == "TAO - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir ".\Release"
# PROP BASE Intermediate_Dir ".\Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir ""
# PROP Intermediate_Dir "DLL\Release"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
CPP=cl.exe
# ADD BASE CPP /nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /YX /c
# ADD CPP /nologo /MD /W3 /GX /O2 /I "..\..\\" /I "..\\" /D "NDEBUG" /D "WIN32" /D "_WINDOWS" /D "TAO_BUILD_DLL" /FD /c
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

!ELSEIF  "$(CFG)" == "TAO - Win32 Debug"

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
# ADD CPP /nologo /MDd /W3 /Gm /GX /Zi /Od /Gy /I "..\..\\" /I "..\\" /D "_DEBUG" /D "WIN32" /D "_WINDOWS" /D "TAO_BUILD_DLL" /FD /c
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

!ELSEIF  "$(CFG)" == "TAO - Win32 Alpha Release"

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
MTL=midl.exe
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /o /win32 "NUL"
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /o /win32 "NUL"
CPP=cl.exe
# ADD BASE CPP /Gt0 nologo MT Gt0 W3 GX O2 I "..\..\\ /I" D "WIN32" D "NDEBUG" D "_WINDOWS" D "TAO_BUILD_DLL" FD ..\\ c
# ADD CPP /nologo /MD /Gt0 /W3 /GX /O2 /I "..\..\\" /I "..\\" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "TAO_BUILD_DLL" /FD /c
RSC=rc.exe
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 ace.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib /nologo /subsystem:windows /dll /machine:ALPHA /out:"..\..\bin\TAO.dll" /libpath:"..\..\ace"
# ADD LINK32 ace.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib /nologo /subsystem:windows /dll /machine:ALPHA /out:"..\..\bin\TAO.dll" /libpath:"..\..\ace"

!ELSEIF  "$(CFG)" == "TAO - Win32 Alpha Debug"

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
MTL=midl.exe
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /o /win32 "NUL"
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /o /win32 "NUL"
CPP=cl.exe
# ADD BASE CPP /Gt0 nologo Gt0 W3 GX Zi Od Gy I "..\..\\ /I" D "WIN32" D "_DEBUG" D "_WINDOWS" D "TAO_BUILD_DLL" FD MTd ..\\ c
# ADD CPP /nologo /Gt0 /W3 /GX /Zi /Od /Gy /I "..\..\\" /I "..\\" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "TAO_BUILD_DLL" /FD /MDd /c
RSC=rc.exe
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 aced.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib /nologo /subsystem:windows /dll /debug /machine:ALPHA /out:"..\..\bin\TAOd.dll" /libpath:"..\..\ace"
# ADD LINK32 aced.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib /nologo /subsystem:windows /dll /debug /machine:ALPHA /out:"..\..\bin\TAOd.dll" /libpath:"..\..\ace"

!ENDIF 

# Begin Target

# Name "TAO - Win32 Release"
# Name "TAO - Win32 Debug"
# Name "TAO - Win32 Alpha Release"
# Name "TAO - Win32 Alpha Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;hpj;bat;for;f90"
# Begin Source File

SOURCE=.\Active_Object_Map.cpp

!IF  "$(CFG)" == "TAO - Win32 Release"

!ELSEIF  "$(CFG)" == "TAO - Win32 Debug"

!ELSEIF  "$(CFG)" == "TAO - Win32 Alpha Release"

!ELSEIF  "$(CFG)" == "TAO - Win32 Alpha Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Any.cpp

!IF  "$(CFG)" == "TAO - Win32 Release"

!ELSEIF  "$(CFG)" == "TAO - Win32 Debug"

!ELSEIF  "$(CFG)" == "TAO - Win32 Alpha Release"

!ELSEIF  "$(CFG)" == "TAO - Win32 Alpha Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\append.cpp

!IF  "$(CFG)" == "TAO - Win32 Release"

!ELSEIF  "$(CFG)" == "TAO - Win32 Debug"

!ELSEIF  "$(CFG)" == "TAO - Win32 Alpha Release"

!ELSEIF  "$(CFG)" == "TAO - Win32 Alpha Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\CDR.cpp

!IF  "$(CFG)" == "TAO - Win32 Release"

!ELSEIF  "$(CFG)" == "TAO - Win32 Debug"

!ELSEIF  "$(CFG)" == "TAO - Win32 Alpha Release"

!ELSEIF  "$(CFG)" == "TAO - Win32 Alpha Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\CDR_Interpreter.cpp

!IF  "$(CFG)" == "TAO - Win32 Release"

!ELSEIF  "$(CFG)" == "TAO - Win32 Debug"

!ELSEIF  "$(CFG)" == "TAO - Win32 Alpha Release"

!ELSEIF  "$(CFG)" == "TAO - Win32 Alpha Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Client_Strategy_Factory.cpp

!IF  "$(CFG)" == "TAO - Win32 Release"

!ELSEIF  "$(CFG)" == "TAO - Win32 Debug"

!ELSEIF  "$(CFG)" == "TAO - Win32 Alpha Release"

!ELSEIF  "$(CFG)" == "TAO - Win32 Alpha Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Connect.cpp

!IF  "$(CFG)" == "TAO - Win32 Release"

!ELSEIF  "$(CFG)" == "TAO - Win32 Debug"

!ELSEIF  "$(CFG)" == "TAO - Win32 Alpha Release"

!ELSEIF  "$(CFG)" == "TAO - Win32 Alpha Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Context.cpp

!IF  "$(CFG)" == "TAO - Win32 Release"

!ELSEIF  "$(CFG)" == "TAO - Win32 Debug"

!ELSEIF  "$(CFG)" == "TAO - Win32 Alpha Release"

!ELSEIF  "$(CFG)" == "TAO - Win32 Alpha Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\corbafwd.cpp

!IF  "$(CFG)" == "TAO - Win32 Release"

!ELSEIF  "$(CFG)" == "TAO - Win32 Debug"

!ELSEIF  "$(CFG)" == "TAO - Win32 Alpha Release"

!ELSEIF  "$(CFG)" == "TAO - Win32 Alpha Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\CurrentC.cpp

!IF  "$(CFG)" == "TAO - Win32 Release"

!ELSEIF  "$(CFG)" == "TAO - Win32 Debug"

!ELSEIF  "$(CFG)" == "TAO - Win32 Alpha Release"

!ELSEIF  "$(CFG)" == "TAO - Win32 Alpha Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\CurrentS.cpp

!IF  "$(CFG)" == "TAO - Win32 Release"

!ELSEIF  "$(CFG)" == "TAO - Win32 Debug"

!ELSEIF  "$(CFG)" == "TAO - Win32 Alpha Release"

!ELSEIF  "$(CFG)" == "TAO - Win32 Alpha Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\debug.cpp

!IF  "$(CFG)" == "TAO - Win32 Release"

!ELSEIF  "$(CFG)" == "TAO - Win32 Debug"

!ELSEIF  "$(CFG)" == "TAO - Win32 Alpha Release"

!ELSEIF  "$(CFG)" == "TAO - Win32 Alpha Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\decode.cpp

!IF  "$(CFG)" == "TAO - Win32 Release"

!ELSEIF  "$(CFG)" == "TAO - Win32 Debug"

!ELSEIF  "$(CFG)" == "TAO - Win32 Alpha Release"

!ELSEIF  "$(CFG)" == "TAO - Win32 Alpha Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\deep_free.cpp

!IF  "$(CFG)" == "TAO - Win32 Release"

!ELSEIF  "$(CFG)" == "TAO - Win32 Debug"

!ELSEIF  "$(CFG)" == "TAO - Win32 Alpha Release"

!ELSEIF  "$(CFG)" == "TAO - Win32 Alpha Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\default_client.cpp

!IF  "$(CFG)" == "TAO - Win32 Release"

!ELSEIF  "$(CFG)" == "TAO - Win32 Debug"

!ELSEIF  "$(CFG)" == "TAO - Win32 Alpha Release"

!ELSEIF  "$(CFG)" == "TAO - Win32 Alpha Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\default_resource.cpp

!IF  "$(CFG)" == "TAO - Win32 Release"

!ELSEIF  "$(CFG)" == "TAO - Win32 Debug"

!ELSEIF  "$(CFG)" == "TAO - Win32 Alpha Release"

!ELSEIF  "$(CFG)" == "TAO - Win32 Alpha Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\default_server.cpp

!IF  "$(CFG)" == "TAO - Win32 Release"

!ELSEIF  "$(CFG)" == "TAO - Win32 Debug"

!ELSEIF  "$(CFG)" == "TAO - Win32 Alpha Release"

!ELSEIF  "$(CFG)" == "TAO - Win32 Alpha Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\DomainC.cpp

!IF  "$(CFG)" == "TAO - Win32 Release"

!ELSEIF  "$(CFG)" == "TAO - Win32 Debug"

!ELSEIF  "$(CFG)" == "TAO - Win32 Alpha Release"

!ELSEIF  "$(CFG)" == "TAO - Win32 Alpha Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\DomainS.cpp

!IF  "$(CFG)" == "TAO - Win32 Release"

!ELSEIF  "$(CFG)" == "TAO - Win32 Debug"

!ELSEIF  "$(CFG)" == "TAO - Win32 Alpha Release"

!ELSEIF  "$(CFG)" == "TAO - Win32 Alpha Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\DynAny_i.cpp

!IF  "$(CFG)" == "TAO - Win32 Release"

!ELSEIF  "$(CFG)" == "TAO - Win32 Debug"

!ELSEIF  "$(CFG)" == "TAO - Win32 Alpha Release"

!ELSEIF  "$(CFG)" == "TAO - Win32 Alpha Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\DynAnyC.cpp

!IF  "$(CFG)" == "TAO - Win32 Release"

!ELSEIF  "$(CFG)" == "TAO - Win32 Debug"

!ELSEIF  "$(CFG)" == "TAO - Win32 Alpha Release"

!ELSEIF  "$(CFG)" == "TAO - Win32 Alpha Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\DynAnyS.cpp

!IF  "$(CFG)" == "TAO - Win32 Release"

!ELSEIF  "$(CFG)" == "TAO - Win32 Debug"

!ELSEIF  "$(CFG)" == "TAO - Win32 Alpha Release"

!ELSEIF  "$(CFG)" == "TAO - Win32 Alpha Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\DynArray_i.cpp

!IF  "$(CFG)" == "TAO - Win32 Release"

!ELSEIF  "$(CFG)" == "TAO - Win32 Debug"

!ELSEIF  "$(CFG)" == "TAO - Win32 Alpha Release"

!ELSEIF  "$(CFG)" == "TAO - Win32 Alpha Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\DynEnum_i.cpp

!IF  "$(CFG)" == "TAO - Win32 Release"

!ELSEIF  "$(CFG)" == "TAO - Win32 Debug"

!ELSEIF  "$(CFG)" == "TAO - Win32 Alpha Release"

!ELSEIF  "$(CFG)" == "TAO - Win32 Alpha Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\DynSequence_i.cpp

!IF  "$(CFG)" == "TAO - Win32 Release"

!ELSEIF  "$(CFG)" == "TAO - Win32 Debug"

!ELSEIF  "$(CFG)" == "TAO - Win32 Alpha Release"

!ELSEIF  "$(CFG)" == "TAO - Win32 Alpha Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\DynStruct_i.cpp

!IF  "$(CFG)" == "TAO - Win32 Release"

!ELSEIF  "$(CFG)" == "TAO - Win32 Debug"

!ELSEIF  "$(CFG)" == "TAO - Win32 Alpha Release"

!ELSEIF  "$(CFG)" == "TAO - Win32 Alpha Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\DynUnion_i.cpp

!IF  "$(CFG)" == "TAO - Win32 Release"

!ELSEIF  "$(CFG)" == "TAO - Win32 Debug"

!ELSEIF  "$(CFG)" == "TAO - Win32 Alpha Release"

!ELSEIF  "$(CFG)" == "TAO - Win32 Alpha Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\encode.cpp

!IF  "$(CFG)" == "TAO - Win32 Release"

!ELSEIF  "$(CFG)" == "TAO - Win32 Debug"

!ELSEIF  "$(CFG)" == "TAO - Win32 Alpha Release"

!ELSEIF  "$(CFG)" == "TAO - Win32 Alpha Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Environment.cpp

!IF  "$(CFG)" == "TAO - Win32 Release"

!ELSEIF  "$(CFG)" == "TAO - Win32 Debug"

!ELSEIF  "$(CFG)" == "TAO - Win32 Alpha Release"

!ELSEIF  "$(CFG)" == "TAO - Win32 Alpha Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Exception.cpp

!IF  "$(CFG)" == "TAO - Win32 Release"

!ELSEIF  "$(CFG)" == "TAO - Win32 Debug"

!ELSEIF  "$(CFG)" == "TAO - Win32 Alpha Release"

!ELSEIF  "$(CFG)" == "TAO - Win32 Alpha Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Forwarding_Servant.cpp

!IF  "$(CFG)" == "TAO - Win32 Release"

!ELSEIF  "$(CFG)" == "TAO - Win32 Debug"

!ELSEIF  "$(CFG)" == "TAO - Win32 Alpha Release"

!ELSEIF  "$(CFG)" == "TAO - Win32 Alpha Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\GIOP.cpp

!IF  "$(CFG)" == "TAO - Win32 Release"

!ELSEIF  "$(CFG)" == "TAO - Win32 Debug"

!ELSEIF  "$(CFG)" == "TAO - Win32 Alpha Release"

!ELSEIF  "$(CFG)" == "TAO - Win32 Alpha Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\GIOP_Server_Request.cpp

!IF  "$(CFG)" == "TAO - Win32 Release"

!ELSEIF  "$(CFG)" == "TAO - Win32 Debug"

!ELSEIF  "$(CFG)" == "TAO - Win32 Alpha Release"

!ELSEIF  "$(CFG)" == "TAO - Win32 Alpha Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\IIOP_Acceptor.cpp

!IF  "$(CFG)" == "TAO - Win32 Release"

!ELSEIF  "$(CFG)" == "TAO - Win32 Debug"

!ELSEIF  "$(CFG)" == "TAO - Win32 Alpha Release"

!ELSEIF  "$(CFG)" == "TAO - Win32 Alpha Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\IIOP_Connector.cpp

!IF  "$(CFG)" == "TAO - Win32 Release"

!ELSEIF  "$(CFG)" == "TAO - Win32 Debug"

!ELSEIF  "$(CFG)" == "TAO - Win32 Alpha Release"

!ELSEIF  "$(CFG)" == "TAO - Win32 Alpha Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\IIOP_Profile.cpp

!IF  "$(CFG)" == "TAO - Win32 Release"

!ELSEIF  "$(CFG)" == "TAO - Win32 Debug"

!ELSEIF  "$(CFG)" == "TAO - Win32 Alpha Release"

!ELSEIF  "$(CFG)" == "TAO - Win32 Alpha Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\IIOP_Transport.cpp

!IF  "$(CFG)" == "TAO - Win32 Release"

!ELSEIF  "$(CFG)" == "TAO - Win32 Debug"

!ELSEIF  "$(CFG)" == "TAO - Win32 Alpha Release"

!ELSEIF  "$(CFG)" == "TAO - Win32 Alpha Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\InconsistentTypeCodeC.cpp

!IF  "$(CFG)" == "TAO - Win32 Release"

!ELSEIF  "$(CFG)" == "TAO - Win32 Debug"

!ELSEIF  "$(CFG)" == "TAO - Win32 Alpha Release"

!ELSEIF  "$(CFG)" == "TAO - Win32 Alpha Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Invocation.cpp

!IF  "$(CFG)" == "TAO - Win32 Release"

!ELSEIF  "$(CFG)" == "TAO - Win32 Debug"

!ELSEIF  "$(CFG)" == "TAO - Win32 Alpha Release"

!ELSEIF  "$(CFG)" == "TAO - Win32 Alpha Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\IOR_LookupTable.cpp

!IF  "$(CFG)" == "TAO - Win32 Release"

!ELSEIF  "$(CFG)" == "TAO - Win32 Debug"

!ELSEIF  "$(CFG)" == "TAO - Win32 Alpha Release"

!ELSEIF  "$(CFG)" == "TAO - Win32 Alpha Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Key_Adapters.cpp

!IF  "$(CFG)" == "TAO - Win32 Release"

!ELSEIF  "$(CFG)" == "TAO - Win32 Debug"

!ELSEIF  "$(CFG)" == "TAO - Win32 Alpha Release"

!ELSEIF  "$(CFG)" == "TAO - Win32 Alpha Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Managed_Types.cpp

!IF  "$(CFG)" == "TAO - Win32 Release"

!ELSEIF  "$(CFG)" == "TAO - Win32 Debug"

!ELSEIF  "$(CFG)" == "TAO - Win32 Alpha Release"

!ELSEIF  "$(CFG)" == "TAO - Win32 Alpha Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Marshal.cpp

!IF  "$(CFG)" == "TAO - Win32 Release"

!ELSEIF  "$(CFG)" == "TAO - Win32 Debug"

!ELSEIF  "$(CFG)" == "TAO - Win32 Alpha Release"

!ELSEIF  "$(CFG)" == "TAO - Win32 Alpha Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Messaging_Policy_i.cpp

!IF  "$(CFG)" == "TAO - Win32 Release"

!ELSEIF  "$(CFG)" == "TAO - Win32 Debug"

!ELSEIF  "$(CFG)" == "TAO - Win32 Alpha Release"

!ELSEIF  "$(CFG)" == "TAO - Win32 Alpha Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\MessagingC.cpp

!IF  "$(CFG)" == "TAO - Win32 Release"

!ELSEIF  "$(CFG)" == "TAO - Win32 Debug"

!ELSEIF  "$(CFG)" == "TAO - Win32 Alpha Release"

!ELSEIF  "$(CFG)" == "TAO - Win32 Alpha Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\MessagingS.cpp

!IF  "$(CFG)" == "TAO - Win32 Release"

!ELSEIF  "$(CFG)" == "TAO - Win32 Debug"

!ELSEIF  "$(CFG)" == "TAO - Win32 Alpha Release"

!ELSEIF  "$(CFG)" == "TAO - Win32 Alpha Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\MProfile.cpp

!IF  "$(CFG)" == "TAO - Win32 Release"

!ELSEIF  "$(CFG)" == "TAO - Win32 Debug"

!ELSEIF  "$(CFG)" == "TAO - Win32 Alpha Release"

!ELSEIF  "$(CFG)" == "TAO - Win32 Alpha Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\NVList.cpp

!IF  "$(CFG)" == "TAO - Win32 Release"

!ELSEIF  "$(CFG)" == "TAO - Win32 Debug"

!ELSEIF  "$(CFG)" == "TAO - Win32 Alpha Release"

!ELSEIF  "$(CFG)" == "TAO - Win32 Alpha Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Object.cpp

!IF  "$(CFG)" == "TAO - Win32 Release"

!ELSEIF  "$(CFG)" == "TAO - Win32 Debug"

!ELSEIF  "$(CFG)" == "TAO - Win32 Alpha Release"

!ELSEIF  "$(CFG)" == "TAO - Win32 Alpha Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Object_Adapter.cpp

!IF  "$(CFG)" == "TAO - Win32 Release"

!ELSEIF  "$(CFG)" == "TAO - Win32 Debug"

!ELSEIF  "$(CFG)" == "TAO - Win32 Alpha Release"

!ELSEIF  "$(CFG)" == "TAO - Win32 Alpha Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Object_KeyC.cpp

!IF  "$(CFG)" == "TAO - Win32 Release"

!ELSEIF  "$(CFG)" == "TAO - Win32 Debug"

!ELSEIF  "$(CFG)" == "TAO - Win32 Alpha Release"

!ELSEIF  "$(CFG)" == "TAO - Win32 Alpha Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\ObjectIDList.cpp

!IF  "$(CFG)" == "TAO - Win32 Release"

!ELSEIF  "$(CFG)" == "TAO - Win32 Debug"

!ELSEIF  "$(CFG)" == "TAO - Win32 Alpha Release"

!ELSEIF  "$(CFG)" == "TAO - Win32 Alpha Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Operation_Table.cpp

!IF  "$(CFG)" == "TAO - Win32 Release"

!ELSEIF  "$(CFG)" == "TAO - Win32 Debug"

!ELSEIF  "$(CFG)" == "TAO - Win32 Alpha Release"

!ELSEIF  "$(CFG)" == "TAO - Win32 Alpha Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\ORB.cpp

!IF  "$(CFG)" == "TAO - Win32 Release"

!ELSEIF  "$(CFG)" == "TAO - Win32 Debug"

!ELSEIF  "$(CFG)" == "TAO - Win32 Alpha Release"

!ELSEIF  "$(CFG)" == "TAO - Win32 Alpha Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\ORB_Core.cpp

!IF  "$(CFG)" == "TAO - Win32 Release"

!ELSEIF  "$(CFG)" == "TAO - Win32 Debug"

!ELSEIF  "$(CFG)" == "TAO - Win32 Alpha Release"

!ELSEIF  "$(CFG)" == "TAO - Win32 Alpha Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\params.cpp

!IF  "$(CFG)" == "TAO - Win32 Release"

!ELSEIF  "$(CFG)" == "TAO - Win32 Debug"

!ELSEIF  "$(CFG)" == "TAO - Win32 Alpha Release"

!ELSEIF  "$(CFG)" == "TAO - Win32 Alpha Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Pluggable.cpp

!IF  "$(CFG)" == "TAO - Win32 Release"

!ELSEIF  "$(CFG)" == "TAO - Win32 Debug"

!ELSEIF  "$(CFG)" == "TAO - Win32 Alpha Release"

!ELSEIF  "$(CFG)" == "TAO - Win32 Alpha Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\POA.cpp

!IF  "$(CFG)" == "TAO - Win32 Release"

!ELSEIF  "$(CFG)" == "TAO - Win32 Debug"

!ELSEIF  "$(CFG)" == "TAO - Win32 Alpha Release"

!ELSEIF  "$(CFG)" == "TAO - Win32 Alpha Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\POAC.cpp

!IF  "$(CFG)" == "TAO - Win32 Release"

!ELSEIF  "$(CFG)" == "TAO - Win32 Debug"

!ELSEIF  "$(CFG)" == "TAO - Win32 Alpha Release"

!ELSEIF  "$(CFG)" == "TAO - Win32 Alpha Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\POAManager.cpp

!IF  "$(CFG)" == "TAO - Win32 Release"

!ELSEIF  "$(CFG)" == "TAO - Win32 Debug"

!ELSEIF  "$(CFG)" == "TAO - Win32 Alpha Release"

!ELSEIF  "$(CFG)" == "TAO - Win32 Alpha Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\POAS.cpp

!IF  "$(CFG)" == "TAO - Win32 Release"

!ELSEIF  "$(CFG)" == "TAO - Win32 Debug"

!ELSEIF  "$(CFG)" == "TAO - Win32 Alpha Release"

!ELSEIF  "$(CFG)" == "TAO - Win32 Alpha Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Policy_Manager.cpp

!IF  "$(CFG)" == "TAO - Win32 Release"

!ELSEIF  "$(CFG)" == "TAO - Win32 Debug"

!ELSEIF  "$(CFG)" == "TAO - Win32 Alpha Release"

!ELSEIF  "$(CFG)" == "TAO - Win32 Alpha Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\PolicyC.cpp

!IF  "$(CFG)" == "TAO - Win32 Release"

!ELSEIF  "$(CFG)" == "TAO - Win32 Debug"

!ELSEIF  "$(CFG)" == "TAO - Win32 Alpha Release"

!ELSEIF  "$(CFG)" == "TAO - Win32 Alpha Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\PolicyS.cpp

!IF  "$(CFG)" == "TAO - Win32 Release"

!ELSEIF  "$(CFG)" == "TAO - Win32 Debug"

!ELSEIF  "$(CFG)" == "TAO - Win32 Alpha Release"

!ELSEIF  "$(CFG)" == "TAO - Win32 Alpha Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Principal.cpp

!IF  "$(CFG)" == "TAO - Win32 Release"

!ELSEIF  "$(CFG)" == "TAO - Win32 Debug"

!ELSEIF  "$(CFG)" == "TAO - Win32 Alpha Release"

!ELSEIF  "$(CFG)" == "TAO - Win32 Alpha Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Request.cpp

!IF  "$(CFG)" == "TAO - Win32 Release"

!ELSEIF  "$(CFG)" == "TAO - Win32 Debug"

!ELSEIF  "$(CFG)" == "TAO - Win32 Alpha Release"

!ELSEIF  "$(CFG)" == "TAO - Win32 Alpha Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Resource_Factory.cpp

!IF  "$(CFG)" == "TAO - Win32 Release"

!ELSEIF  "$(CFG)" == "TAO - Win32 Debug"

!ELSEIF  "$(CFG)" == "TAO - Win32 Alpha Release"

!ELSEIF  "$(CFG)" == "TAO - Win32 Alpha Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Sequence.cpp

!IF  "$(CFG)" == "TAO - Win32 Release"

!ELSEIF  "$(CFG)" == "TAO - Win32 Debug"

!ELSEIF  "$(CFG)" == "TAO - Win32 Alpha Release"

!ELSEIF  "$(CFG)" == "TAO - Win32 Alpha Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Servant_Base.cpp

!IF  "$(CFG)" == "TAO - Win32 Release"

!ELSEIF  "$(CFG)" == "TAO - Win32 Debug"

!ELSEIF  "$(CFG)" == "TAO - Win32 Alpha Release"

!ELSEIF  "$(CFG)" == "TAO - Win32 Alpha Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Server_Request.cpp

!IF  "$(CFG)" == "TAO - Win32 Release"

!ELSEIF  "$(CFG)" == "TAO - Win32 Debug"

!ELSEIF  "$(CFG)" == "TAO - Win32 Alpha Release"

!ELSEIF  "$(CFG)" == "TAO - Win32 Alpha Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Server_Strategy_Factory.cpp

!IF  "$(CFG)" == "TAO - Win32 Release"

!ELSEIF  "$(CFG)" == "TAO - Win32 Debug"

!ELSEIF  "$(CFG)" == "TAO - Win32 Alpha Release"

!ELSEIF  "$(CFG)" == "TAO - Win32 Alpha Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Services.cpp

!IF  "$(CFG)" == "TAO - Win32 Release"

!ELSEIF  "$(CFG)" == "TAO - Win32 Debug"

!ELSEIF  "$(CFG)" == "TAO - Win32 Alpha Release"

!ELSEIF  "$(CFG)" == "TAO - Win32 Alpha Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\skip.cpp

!IF  "$(CFG)" == "TAO - Win32 Release"

!ELSEIF  "$(CFG)" == "TAO - Win32 Debug"

!ELSEIF  "$(CFG)" == "TAO - Win32 Alpha Release"

!ELSEIF  "$(CFG)" == "TAO - Win32 Alpha Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Stub.cpp

!IF  "$(CFG)" == "TAO - Win32 Release"

!ELSEIF  "$(CFG)" == "TAO - Win32 Debug"

!ELSEIF  "$(CFG)" == "TAO - Win32 Alpha Release"

!ELSEIF  "$(CFG)" == "TAO - Win32 Alpha Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\TAO.cpp

!IF  "$(CFG)" == "TAO - Win32 Release"

!ELSEIF  "$(CFG)" == "TAO - Win32 Debug"

!ELSEIF  "$(CFG)" == "TAO - Win32 Alpha Release"

!ELSEIF  "$(CFG)" == "TAO - Win32 Alpha Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\TAO_Internal.cpp

!IF  "$(CFG)" == "TAO - Win32 Release"

!ELSEIF  "$(CFG)" == "TAO - Win32 Debug"

!ELSEIF  "$(CFG)" == "TAO - Win32 Alpha Release"

!ELSEIF  "$(CFG)" == "TAO - Win32 Alpha Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\TimeBaseC.cpp

!IF  "$(CFG)" == "TAO - Win32 Release"

!ELSEIF  "$(CFG)" == "TAO - Win32 Debug"

!ELSEIF  "$(CFG)" == "TAO - Win32 Alpha Release"

!ELSEIF  "$(CFG)" == "TAO - Win32 Alpha Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\TimeBaseS.cpp

!IF  "$(CFG)" == "TAO - Win32 Release"

!ELSEIF  "$(CFG)" == "TAO - Win32 Debug"

!ELSEIF  "$(CFG)" == "TAO - Win32 Alpha Release"

!ELSEIF  "$(CFG)" == "TAO - Win32 Alpha Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Typecode.cpp

!IF  "$(CFG)" == "TAO - Win32 Release"

!ELSEIF  "$(CFG)" == "TAO - Win32 Debug"

!ELSEIF  "$(CFG)" == "TAO - Win32 Alpha Release"

!ELSEIF  "$(CFG)" == "TAO - Win32 Alpha Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Typecode_Constants.cpp

!IF  "$(CFG)" == "TAO - Win32 Release"

!ELSEIF  "$(CFG)" == "TAO - Win32 Debug"

!ELSEIF  "$(CFG)" == "TAO - Win32 Alpha Release"

!ELSEIF  "$(CFG)" == "TAO - Win32 Alpha Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Union.cpp

!IF  "$(CFG)" == "TAO - Win32 Release"

!ELSEIF  "$(CFG)" == "TAO - Win32 Debug"

!ELSEIF  "$(CFG)" == "TAO - Win32 Alpha Release"

!ELSEIF  "$(CFG)" == "TAO - Win32 Alpha Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\ValueBase.cpp

!IF  "$(CFG)" == "TAO - Win32 Release"

!ELSEIF  "$(CFG)" == "TAO - Win32 Debug"

!ELSEIF  "$(CFG)" == "TAO - Win32 Alpha Release"

!ELSEIF  "$(CFG)" == "TAO - Win32 Alpha Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\ValueFactory.cpp

!IF  "$(CFG)" == "TAO - Win32 Release"

!ELSEIF  "$(CFG)" == "TAO - Win32 Debug"

!ELSEIF  "$(CFG)" == "TAO - Win32 Alpha Release"

!ELSEIF  "$(CFG)" == "TAO - Win32 Alpha Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\ValueFactory_Map.cpp

!IF  "$(CFG)" == "TAO - Win32 Release"

!ELSEIF  "$(CFG)" == "TAO - Win32 Debug"

!ELSEIF  "$(CFG)" == "TAO - Win32 Alpha Release"

!ELSEIF  "$(CFG)" == "TAO - Win32 Alpha Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\WrongTransactionC.cpp

!IF  "$(CFG)" == "TAO - Win32 Release"

!ELSEIF  "$(CFG)" == "TAO - Win32 Debug"

!ELSEIF  "$(CFG)" == "TAO - Win32 Alpha Release"

!ELSEIF  "$(CFG)" == "TAO - Win32 Alpha Debug"

!ENDIF 

# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl;fi;fd"
# Begin Source File

SOURCE=.\Active_Object_Map.h
# End Source File
# Begin Source File

SOURCE=.\any.h
# End Source File
# Begin Source File

SOURCE=.\arg_shifter.h
# End Source File
# Begin Source File

SOURCE=.\cdr.h
# End Source File
# Begin Source File

SOURCE=.\CDR_Interpreter.h
# End Source File
# Begin Source File

SOURCE=.\Client_Strategy_Factory.h
# End Source File
# Begin Source File

SOURCE=.\connect.h
# End Source File
# Begin Source File

SOURCE=.\Context.h
# End Source File
# Begin Source File

SOURCE=.\corba.h
# End Source File
# Begin Source File

SOURCE=.\corbafwd.h
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

SOURCE=.\DomainC.h
# End Source File
# Begin Source File

SOURCE=.\DomainS.h
# End Source File
# Begin Source File

SOURCE=.\DynAny_i.h
# End Source File
# Begin Source File

SOURCE=.\DynAnyC.h
# End Source File
# Begin Source File

SOURCE=.\DynArray_i.h
# End Source File
# Begin Source File

SOURCE=.\DynEnum_i.h
# End Source File
# Begin Source File

SOURCE=.\DynSequence_i.h
# End Source File
# Begin Source File

SOURCE=.\DynStruct_i.h
# End Source File
# Begin Source File

SOURCE=.\DynUnion_i.h
# End Source File
# Begin Source File

SOURCE=.\Environment.h
# End Source File
# Begin Source File

SOURCE=.\Exception.h
# End Source File
# Begin Source File

SOURCE=.\Forwarding_Servant.h
# End Source File
# Begin Source File

SOURCE=.\giop.h
# End Source File
# Begin Source File

SOURCE=.\GIOP_Server_Request.h
# End Source File
# Begin Source File

SOURCE=.\IIOP_Acceptor.h
# End Source File
# Begin Source File

SOURCE=.\IIOP_Connector.h
# End Source File
# Begin Source File

SOURCE=.\IIOP_Profile.h
# End Source File
# Begin Source File

SOURCE=.\IIOP_Transport.h
# End Source File
# Begin Source File

SOURCE=.\InconsistentTypeCodeC.h
# End Source File
# Begin Source File

SOURCE=.\Invocation.h
# End Source File
# Begin Source File

SOURCE=.\IOR_LookupTable.h
# End Source File
# Begin Source File

SOURCE=.\Managed_Types.h
# End Source File
# Begin Source File

SOURCE=.\marshal.h
# End Source File
# Begin Source File

SOURCE=.\Messaging_Policy_i.h
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

SOURCE=.\nvlist.h
# End Source File
# Begin Source File

SOURCE=.\object.h
# End Source File
# Begin Source File

SOURCE=.\Object_Adapter.h
# End Source File
# Begin Source File

SOURCE=.\Object_KeyC.h
# End Source File
# Begin Source File

SOURCE=.\ObjectIDList.h
# End Source File
# Begin Source File

SOURCE=.\Operation_Table.h
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

SOURCE=.\orbconf.h
# End Source File
# Begin Source File

SOURCE=.\params.h
# End Source File
# Begin Source File

SOURCE=.\Pluggable.h
# End Source File
# Begin Source File

SOURCE=.\poa.h
# End Source File
# Begin Source File

SOURCE=.\POA_CORBA.h
# End Source File
# Begin Source File

SOURCE=.\poa_macros.h
# End Source File
# Begin Source File

SOURCE=.\poa_T.h
# End Source File
# Begin Source File

SOURCE=.\poaC.h
# End Source File
# Begin Source File

SOURCE=.\POAManager.h
# End Source File
# Begin Source File

SOURCE=.\poaS.h
# End Source File
# Begin Source File

SOURCE=.\Policy_Manager.h
# End Source File
# Begin Source File

SOURCE=.\PolicyC.h
# End Source File
# Begin Source File

SOURCE=.\Principal.h
# End Source File
# Begin Source File

SOURCE=.\request.h
# End Source File
# Begin Source File

SOURCE=.\Resource_Factory.h
# End Source File
# Begin Source File

SOURCE=.\sequence.h
# End Source File
# Begin Source File

SOURCE=.\Sequence_T.h
# End Source File
# Begin Source File

SOURCE=.\servant_base.h
# End Source File
# Begin Source File

SOURCE=.\Server_Request.h
# End Source File
# Begin Source File

SOURCE=.\Server_Strategy_Factory.h
# End Source File
# Begin Source File

SOURCE=.\Services.h
# End Source File
# Begin Source File

SOURCE=.\singletons.h
# End Source File
# Begin Source File

SOURCE=.\stub.h
# End Source File
# Begin Source File

SOURCE=.\TAO.h
# End Source File
# Begin Source File

SOURCE=.\TAO_Internal.h
# End Source File
# Begin Source File

SOURCE=.\Timeprobe.h
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

SOURCE=.\varout.h
# End Source File
# Begin Source File

SOURCE=.\WrongTransactionC.h
# End Source File
# End Group
# Begin Group "Inline Files"

# PROP Default_Filter "*.i"
# Begin Source File

SOURCE=.\Active_Object_Map.i
# End Source File
# Begin Source File

SOURCE=.\any.i
# End Source File
# Begin Source File

SOURCE=.\cdr.i
# End Source File
# Begin Source File

SOURCE=.\connect.i
# End Source File
# Begin Source File

SOURCE=.\Context.i
# End Source File
# Begin Source File

SOURCE=.\corbafwd.i
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

SOURCE=.\DomainC.i
# End Source File
# Begin Source File

SOURCE=.\DomainS_T.i
# End Source File
# Begin Source File

SOURCE=.\DynAnyC.i
# End Source File
# Begin Source File

SOURCE=.\Environment.i
# End Source File
# Begin Source File

SOURCE=.\Exception.i
# End Source File
# Begin Source File

SOURCE=.\giop.i
# End Source File
# Begin Source File

SOURCE=.\GIOP_Server_Request.i
# End Source File
# Begin Source File

SOURCE=.\IIOP_ORB.i
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

SOURCE=.\NVList.i
# End Source File
# Begin Source File

SOURCE=.\object.i
# End Source File
# Begin Source File

SOURCE=.\Object_KeyC.i
# End Source File
# Begin Source File

SOURCE=.\ObjectIDList.i
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

SOURCE=.\params.i
# End Source File
# Begin Source File

SOURCE=.\poaC.i
# End Source File
# Begin Source File

SOURCE=.\POAManager.i
# End Source File
# Begin Source File

SOURCE=.\poaS.i
# End Source File
# Begin Source File

SOURCE=.\Policy_Manager.i
# End Source File
# Begin Source File

SOURCE=.\PolicyC.i
# End Source File
# Begin Source File

SOURCE=.\Request.i
# End Source File
# Begin Source File

SOURCE=.\sequence.i
# End Source File
# Begin Source File

SOURCE=.\Sequence_T.i
# End Source File
# Begin Source File

SOURCE=.\Server_Request.i
# End Source File
# Begin Source File

SOURCE=.\Services.i
# End Source File
# Begin Source File

SOURCE=.\stub.i
# End Source File
# Begin Source File

SOURCE=.\TAO_Internal.i
# End Source File
# Begin Source File

SOURCE=.\Timeprobe.i
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
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;cnt;rtf;gif;jpg;jpeg;jpe"
# End Group
# End Target
# End Project
