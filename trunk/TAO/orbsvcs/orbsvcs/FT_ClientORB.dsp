# Microsoft Developer Studio Project File - Name="FT_ClientORB" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Dynamic-Link Library" 0x0102

CFG=FT_ClientORB - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE run the tool that generated this project file and specify the
!MESSAGE nmake output type.  You can then use the following command:
!MESSAGE
!MESSAGE NMAKE /f "FT_ClientORB.mak".
!MESSAGE
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE
!MESSAGE NMAKE /f "FT_ClientORB.mak" CFG="FT_ClientORB - Win32 Debug"
!MESSAGE
!MESSAGE Possible choices for configuration are:
!MESSAGE
!MESSAGE "FT_ClientORB - Win32 Release" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "FT_ClientORB - Win32 Debug" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "FT_ClientORB - Win32 Release"

# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "..\..\..\lib"
# PROP Intermediate_Dir "Release\FT_ClientORB"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD CPP /nologo /O2 /W3 /GX /MD /Zi /GR /I "..\..\orbsvcs\orbsvcs" /I "..\..\orbsvcs" /I "..\.." /I "..\..\tao" /I "..\..\.." /D NDEBUG /D WIN32 /D _WINDOWS /D TAO_FT_CLIENTORB_BUILD_DLL /FD /c
# SUBTRACT CPP /YX
# ADD RSC /l 0x409 /d NDEBUG /i "..\..\orbsvcs\orbsvcs" /i "..\..\orbsvcs" /i "..\.." /i "..\..\tao" /i "..\..\.."
BSC32=bscmake.exe
# ADD BSC32 /nologo /o"..\..\..\lib\TAO_FT_ClientORB.bsc"
LINK32=link.exe
# ADD LINK32 advapi32.lib user32.lib /INCREMENTAL:NO TAO_FTORB_Utils.lib TAO_PortableServer.lib TAO_IORInterceptor.lib TAO_ObjRefTemplate.lib TAO_Valuetype.lib TAO_IORManip.lib TAO.lib ACE.lib /libpath:"..\..\..\lib" /nologo /version:1.3.5 /subsystem:windows /pdb:"..\..\..\lib\TAO_FT_ClientORB.pdb" /dll  /machine:I386 /out:"..\..\..\lib\TAO_FT_ClientORB.dll"

!ELSEIF  "$(CFG)" == "FT_ClientORB - Win32 Debug"

# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "..\..\..\lib"
# PROP Intermediate_Dir "Debug\FT_ClientORB"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD CPP /nologo /Ob0 /W3 /Gm /GX /Zi /MDd /GR /Gy /I "..\..\orbsvcs\orbsvcs" /I "..\..\orbsvcs" /I "..\.." /I "..\..\tao" /I "..\..\.." /D _DEBUG /D WIN32 /D _WINDOWS /D TAO_FT_CLIENTORB_BUILD_DLL /FD /c
# SUBTRACT CPP /Fr /YX
# ADD RSC /l 0x409 /d _DEBUG /i "..\..\orbsvcs\orbsvcs" /i "..\..\orbsvcs" /i "..\.." /i "..\..\tao" /i "..\..\.."
BSC32=bscmake.exe
# ADD BSC32 /nologo /o"..\..\..\lib\TAO_FT_ClientORB.bsc"
LINK32=link.exe
# ADD LINK32 advapi32.lib user32.lib /INCREMENTAL:NO TAO_FTORB_Utilsd.lib TAO_PortableServerd.lib TAO_IORInterceptord.lib TAO_ObjRefTemplated.lib TAO_Valuetyped.lib TAO_IORManipd.lib TAOd.lib ACEd.lib /libpath:"..\..\..\lib" /nologo /version:1.3.5 /subsystem:windows /pdb:"..\..\..\lib\TAO_FT_ClientORBd.pdb" /dll /debug /machine:I386 /out:"..\..\..\lib\TAO_FT_ClientORBd.dll"

!ENDIF

# Begin Target

# Name "FT_ClientORB - Win32 Release"
# Name "FT_ClientORB - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;cxx;c"
# Begin Source File

SOURCE=".\FaultTolerance\FT_ClientORBInitializer.cpp"
# End Source File
# Begin Source File

SOURCE=".\FaultTolerance\FT_ClientPolicy_i.cpp"
# End Source File
# Begin Source File

SOURCE=".\FaultTolerance\FT_ClientPolicyFactory.cpp"
# End Source File
# Begin Source File

SOURCE=".\FaultTolerance\FT_ClientRequest_Interceptor.cpp"
# End Source File
# Begin Source File

SOURCE=".\FaultTolerance\FT_ClientService_Activate.cpp"
# End Source File
# Begin Source File

SOURCE=".\FaultTolerance\FT_Endpoint_Selector_Factory.cpp"
# End Source File
# Begin Source File

SOURCE=".\FaultTolerance\FT_Invocation_Endpoint_Selectors.cpp"
# End Source File
# Begin Source File

SOURCE=".\FaultTolerance\FT_Service_Callbacks.cpp"
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hh"
# Begin Source File

SOURCE=".\Channel_Clients_T.h"
# End Source File
# Begin Source File

SOURCE=".\FaultTolerance\FT_ClientORBInitializer.h"
# End Source File
# Begin Source File

SOURCE=".\FaultTolerance\FT_ClientPolicy_i.h"
# End Source File
# Begin Source File

SOURCE=".\FaultTolerance\FT_ClientPolicyFactory.h"
# End Source File
# Begin Source File

SOURCE=".\FaultTolerance\FT_ClientRequest_Interceptor.h"
# End Source File
# Begin Source File

SOURCE=".\FaultTolerance\FT_ClientService_Activate.h"
# End Source File
# Begin Source File

SOURCE=".\FaultTolerance\FT_Endpoint_Selector_Factory.h"
# End Source File
# Begin Source File

SOURCE=".\FaultTolerance\FT_Invocation_Endpoint_Selectors.h"
# End Source File
# Begin Source File

SOURCE=".\FaultTolerance\FT_Service_Callbacks.h"
# End Source File
# End Group
# Begin Group "Inline Files"

# PROP Default_Filter "i;inl"
# Begin Source File

SOURCE=".\Channel_Clients_T.i"
# End Source File
# Begin Source File

SOURCE=".\FaultTolerance\FT_ClientPolicy_i.inl"
# End Source File
# Begin Source File

SOURCE=".\FaultTolerance\FT_Invocation_Endpoint_Selectors.inl"
# End Source File
# Begin Source File

SOURCE=".\FaultTolerance\FT_Service_Callbacks.inl"
# End Source File
# End Group
# Begin Group "Template Files"

# PROP Default_Filter ""
# Begin Source File

SOURCE=".\Channel_Clients_T.cpp"
# PROP Exclude_From_Build 1
# End Source File
# End Group
# End Target
# End Project
