# Microsoft Developer Studio Project File - Name="CosEC RTEC_Based Library" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Dynamic-Link Library" 0x0102

CFG=CosEC RTEC_Based Library - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "TAO_RTEC_COSEC.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "TAO_RTEC_COSEC.mak" CFG="CosEC RTEC_Based Library - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "CosEC RTEC_Based Library - Win32 MFC Release" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "CosEC RTEC_Based Library - Win32 MFC Debug" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "CosEC RTEC_Based Library - Win32 Release" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "CosEC RTEC_Based Library - Win32 Debug" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "CosEC RTEC_Based Library - Win32 MFC Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir ""
# PROP BASE Intermediate_Dir "Release\MFC\TAO_RTEC_COSEC"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir ""
# PROP Intermediate_Dir "Release\MFC\TAO_RTEC_COSEC"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /GX /D "WIN32" /D "_WINDOWS" /D "_CONSOLE" /D "_MBCS" /FD /c
# ADD CPP /nologo /W3 /GX /D "WIN32" /D "_WINDOWS" /D "_CONSOLE" /D "_MBCS" /FD /c /D "NDEBUG" /O2 /MD /D "TAO_RTEC_COSEC_BUILD_DLL" /I "../../../../../../" /I "../../../../../" /I "../../../../" /I "" /D ACE_HAS_MFC=1
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /machine:I386
# ADD LINK32 /nologo /dll /machine:I386 ../../../../../../ace/ACEmfc.lib ../../../../../tao/TAOmfc.lib ../../../../../tao/PortableServer/TAO_PortableServermfc.lib ../../../../orbsvcs/TAO_CosNamingmfc.lib ../../../../orbsvcs/TAO_RTEventmfc.lib ../../../../orbsvcs/TAO_Svc_Utilsmfc.lib ../../../../orbsvcs/TAO_CosEventmfc.lib /out:"../../../../../../bin/TAO_RTEC_COSECmfc.dll" 

!ELSEIF  "$(CFG)" == "CosEC RTEC_Based Library - Win32 MFC Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir ""
# PROP BASE Intermediate_Dir "Debug\MFC\TAO_RTEC_COSEC"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir ""
# PROP Intermediate_Dir "Debug\MFC\TAO_RTEC_COSEC"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /GX /D "WIN32" /D "_WINDOWS" /D "_CONSOLE" /D "_MBCS" /FD /c /Gm /Zi
# ADD CPP /nologo /W3 /GX /D "WIN32" /D "_WINDOWS" /D "_CONSOLE" /D "_MBCS" /FD /c /Gm /Zi /D "_DEBUG" /Od /MDd /D "TAO_RTEC_COSEC_BUILD_DLL" /I "../../../../../../" /I "../../../../../" /I "../../../../" /I "" /D ACE_HAS_MFC=1
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /machine:I386 /debug /pdbtype:sept
# ADD LINK32 /nologo /dll /machine:I386 /debug /pdbtype:sept ../../../../../../ace/ACEmfcd.lib ../../../../../tao/TAOmfcd.lib ../../../../../tao/PortableServer/TAO_PortableServermfcd.lib ../../../../orbsvcs/TAO_CosNamingmfcd.lib ../../../../orbsvcs/TAO_RTEventmfcd.lib ../../../../orbsvcs/TAO_Svc_Utilsmfcd.lib ../../../../orbsvcs/TAO_CosEventmfcd.lib /out:"../../../../../../bin/TAO_RTEC_COSECmfcd.dll" 

!ELSEIF  "$(CFG)" == "CosEC RTEC_Based Library - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir ""
# PROP BASE Intermediate_Dir "Release\TAO_RTEC_COSEC"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir ""
# PROP Intermediate_Dir "Release\TAO_RTEC_COSEC"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /GX /D "WIN32" /D "_WINDOWS" /D "_CONSOLE" /D "_MBCS" /FD /c
# ADD CPP /nologo /W3 /GX /D "WIN32" /D "_WINDOWS" /D "_CONSOLE" /D "_MBCS" /FD /c /D "NDEBUG" /O2 /MD /D "TAO_RTEC_COSEC_BUILD_DLL" /I "../../../../../../" /I "../../../../../" /I "../../../../" /I ""
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /machine:I386
# ADD LINK32 /nologo /dll /machine:I386 ../../../../../../ace/ACE.lib ../../../../../tao/TAO.lib ../../../../../tao/PortableServer/TAO_PortableServer.lib ../../../../orbsvcs/TAO_CosNaming.lib ../../../../orbsvcs/TAO_RTEvent.lib ../../../../orbsvcs/TAO_Svc_Utils.lib ../../../../orbsvcs/TAO_CosEvent.lib /out:"../../../../../../bin/TAO_RTEC_COSEC.dll" 

!ELSEIF  "$(CFG)" == "CosEC RTEC_Based Library - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir ""
# PROP BASE Intermediate_Dir "Debug\TAO_RTEC_COSEC"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir ""
# PROP Intermediate_Dir "Debug\TAO_RTEC_COSEC"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /GX /D "WIN32" /D "_WINDOWS" /D "_CONSOLE" /D "_MBCS" /FD /c /Gm /Zi
# ADD CPP /nologo /W3 /GX /D "WIN32" /D "_WINDOWS" /D "_CONSOLE" /D "_MBCS" /FD /c /Gm /Zi /D "_DEBUG" /Od /MDd /D "TAO_RTEC_COSEC_BUILD_DLL" /I "../../../../../../" /I "../../../../../" /I "../../../../" /I ""
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /machine:I386 /debug /pdbtype:sept
# ADD LINK32 /nologo /dll /machine:I386 /debug /pdbtype:sept ../../../../../../ace/ACEd.lib ../../../../../tao/TAOd.lib ../../../../../tao/PortableServer/TAO_PortableServerd.lib ../../../../orbsvcs/TAO_CosNamingd.lib ../../../../orbsvcs/TAO_RTEventd.lib ../../../../orbsvcs/TAO_Svc_Utilsd.lib ../../../../orbsvcs/TAO_CosEventd.lib /out:"../../../../../../bin/TAO_RTEC_COSECd.dll" 

!ENDIF

# Begin Target

# Name "CosEC RTEC_Based Library - Win32 MFC Release"
# Name "CosEC RTEC_Based Library - Win32 MFC Debug"
# Name "CosEC RTEC_Based Library - Win32 Release"
# Name "CosEC RTEC_Based Library - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;hpj;bat;for;f90"
# Begin Source File

SOURCE=.\ConsumerAdmin_i.cpp
# End Source File
# Begin Source File

SOURCE=.\CosEvent_Utilities.cpp
# End Source File
# Begin Source File

SOURCE=.\EventChannel_i.cpp
# End Source File
# Begin Source File

SOURCE=.\ProxyPushConsumer_i.cpp
# End Source File
# Begin Source File

SOURCE=.\ProxyPushSupplier_i.cpp
# End Source File
# Begin Source File

SOURCE=.\SupplierAdmin_i.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;fi;fd"
# Begin Source File

SOURCE=.\ConsumerAdmin_i.h
# End Source File
# Begin Source File

SOURCE=.\CosEvent_Utilities.h
# End Source File
# Begin Source File

SOURCE=.\EventChannel_i.h
# End Source File
# Begin Source File

SOURCE=.\ProxyPushConsumer_i.h
# End Source File
# Begin Source File

SOURCE=.\ProxyPushSupplier_i.h
# End Source File
# Begin Source File

SOURCE=.\SupplierAdmin_i.h
# End Source File
# Begin Source File

SOURCE=.\rtec_cosec_export.h
# End Source File
# End Group
# Begin Group "IDL Files"

# PROP Default_Filter "idl;pidl"
# End Group
# Begin Group "Inline Files"

# PROP Default_Filter "inl;i"
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;cnt;rtf;gif;jpg;jpeg;jpe"
# End Group
# End Target
# End Project
