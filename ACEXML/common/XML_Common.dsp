# Microsoft Developer Studio Project File - Name="XML_Common" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Dynamic-Link Library" 0x0102

CFG=XML_Common - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "XML_Common.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "XML_Common.mak" CFG="XML_Common - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "XML_Common - Win32 Release" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "XML_Common - Win32 Debug" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "XML_Common - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir ""
# PROP Intermediate_Dir "Release"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "XML_COMMON_EXPORTS" /YX /FD /c
# ADD CPP /nologo /MD /W3 /GX /O2 /I "../.." /I ".." /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "ACEXML_BUILD_DLL" /YX /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /machine:I386
# ADD LINK32 ace.lib /nologo /dll /machine:I386 /out:"../../bin/ACEXML.dll" /libpath:"w:\ace_wrappers\ace"

!ELSEIF  "$(CFG)" == "XML_Common - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "XML_Common___Win32_Debug"
# PROP BASE Intermediate_Dir "XML_Common___Win32_Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir ""
# PROP Intermediate_Dir "Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MTd /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "XML_COMMON_EXPORTS" /YX /FD /c
# ADD CPP /nologo /MDd /W3 /Gm /GX /Zi /Od /I "../.." /I ".." /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D ACEXML_HAS_DLL=1 /D "ACEXML_BUILD_DLL" /YX /FD /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /debug /machine:I386 /pdbtype:sept
# ADD LINK32 aced.lib /nologo /dll /debug /machine:I386 /out:"../../bin/ACEXMLD.dll" /pdbtype:sept /libpath:"w:\ace_wrappers\ace"

!ENDIF 

# Begin Target

# Name "XML_Common - Win32 Release"
# Name "XML_Common - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\Attributes_Def_Builder.cpp
# End Source File
# Begin Source File

SOURCE=.\AttributesImpl.cpp
# End Source File
# Begin Source File

SOURCE=.\CharStream.cpp
# End Source File
# Begin Source File

SOURCE=.\DefaultHandler.cpp
# End Source File
# Begin Source File

SOURCE=.\DTD_Manager.cpp
# End Source File
# Begin Source File

SOURCE=.\Element_Def_Builder.cpp
# End Source File
# Begin Source File

SOURCE=.\Env.cpp
# End Source File
# Begin Source File

SOURCE=.\Exception.cpp
# End Source File
# Begin Source File

SOURCE=.\FileCharStream.cpp
# End Source File
# Begin Source File

SOURCE=.\InputSource.cpp
# End Source File
# Begin Source File

SOURCE=.\LocatorImpl.cpp
# End Source File
# Begin Source File

SOURCE=.\NamespaceSupport.cpp
# End Source File
# Begin Source File

SOURCE=.\SAXExceptions.cpp
# End Source File
# Begin Source File

SOURCE=.\Transcode.cpp
# End Source File
# Begin Source File

SOURCE=.\Validator.cpp
# End Source File
# Begin Source File

SOURCE=.\XMLFilterImpl.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\ACEXML_Export.h
# End Source File
# Begin Source File

SOURCE=.\Attributes.h
# End Source File
# Begin Source File

SOURCE=.\Attributes_Def_Builder.h
# End Source File
# Begin Source File

SOURCE=.\AttributesImpl.h
# End Source File
# Begin Source File

SOURCE=.\CharStream.h
# End Source File
# Begin Source File

SOURCE=.\ContentHandler.h
# End Source File
# Begin Source File

SOURCE=.\DefaultHandler.h
# End Source File
# Begin Source File

SOURCE=.\DTD_Manager.h
# End Source File
# Begin Source File

SOURCE=.\DTDHandler.h
# End Source File
# Begin Source File

SOURCE=.\Element_Def_Builder.h
# End Source File
# Begin Source File

SOURCE=.\EntityResolver.h
# End Source File
# Begin Source File

SOURCE=.\Env.h
# End Source File
# Begin Source File

SOURCE=.\ErrorHandler.h
# End Source File
# Begin Source File

SOURCE=.\Exception.h
# End Source File
# Begin Source File

SOURCE=.\FileCharStream.h
# End Source File
# Begin Source File

SOURCE=.\InputSource.h
# End Source File
# Begin Source File

SOURCE=.\Locator.h
# End Source File
# Begin Source File

SOURCE=.\LocatorImpl.h
# End Source File
# Begin Source File

SOURCE=.\NamespaceSupport.h
# End Source File
# Begin Source File

SOURCE=.\SAXExceptions.h
# End Source File
# Begin Source File

SOURCE=.\Transcode.h
# End Source File
# Begin Source File

SOURCE=.\Validator.h
# End Source File
# Begin Source File

SOURCE=.\XML_Types.h
# End Source File
# Begin Source File

SOURCE=.\XMLFilter.h
# End Source File
# Begin Source File

SOURCE=.\XMLFilterImpl.h
# End Source File
# Begin Source File

SOURCE=.\XMLReader.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# End Group
# Begin Group "Inline Files"

# PROP Default_Filter "i"
# Begin Source File

SOURCE=.\Env.i
# End Source File
# Begin Source File

SOURCE=.\Exception.i
# End Source File
# End Group
# End Target
# End Project
