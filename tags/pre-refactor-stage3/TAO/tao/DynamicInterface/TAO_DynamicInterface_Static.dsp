# Microsoft Developer Studio Project File - Name="DynamicInterface_Static" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Static Library" 0x0104

CFG=DynamicInterface_Static - Win32 Static Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "TAO_DynamicInterface_Static.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "TAO_DynamicInterface_Static.mak" CFG="DynamicInterface_Static - Win32 Static Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "DynamicInterface_Static - Win32 Static Release" (based on "Win32 (x86) Static Library")
!MESSAGE "DynamicInterface_Static - Win32 Static Debug" (based on "Win32 (x86) Static Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "DynamicInterface_Static - Win32 Static Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir ""
# PROP BASE Intermediate_Dir "LIB/Release/DynamicInterface"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir ""
# PROP Intermediate_Dir "LIB\Release\DynamicInterface"
# PROP Target_Dir ""
LINK32=link.exe -lib
MTL=midl.exe
# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_MBCS" /D "_LIB" /YX /FD /c
# ADD CPP /nologo /MD /W3 /GX /O2 /I "../" /I "../../" /I "../../../" /D "NDEBUG" /D "_WINDOWS" /D "_CONSOLE" /D "WIN32" /D "ACE_AS_STATIC_LIBS" /D "TAO_AS_STATIC_LIBS" /FD /c
# SUBTRACT CPP /YX
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo /out:"TAO_DynamicInterfaces.lib"

!ELSEIF  "$(CFG)" == "DynamicInterface_Static - Win32 Static Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir ""
# PROP BASE Intermediate_Dir "LIB/Debug/DynamicInterface"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir ""
# PROP Intermediate_Dir "LIB\Debug\DynamicInterface"
# PROP Target_Dir ""
LINK32=link.exe -lib
MTL=midl.exe
# ADD BASE CPP /nologo /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_MBCS" /D "_LIB" /YX /FD /c
# ADD CPP /nologo /MDd /W3 /Gm /GX /Zi /Od /I "../" /I "../../" /I "../../../" /D "_DEBUG" /D "_WINDOWS" /D "_CONSOLE" /D "WIN32" /D "ACE_AS_STATIC_LIBS" /D "TAO_AS_STATIC_LIBS" /FD /c
# SUBTRACT CPP /YX
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo /out:"TAO_DynamicInterfacesd.lib"

!ENDIF 

# Begin Target

# Name "DynamicInterface_Static - Win32 Static Release"
# Name "DynamicInterface_Static - Win32 Static Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\Context.cpp
# End Source File
# Begin Source File

SOURCE=.\DII_ClientRequestInfo.cpp
# End Source File
# Begin Source File

SOURCE=.\DII_Invocation.cpp
# End Source File
# Begin Source File

SOURCE=.\DII_Reply_Dispatcher.cpp
# End Source File
# Begin Source File

SOURCE=.\Dynamic_Adapter_Impl.cpp
# End Source File
# Begin Source File

SOURCE=.\Dynamic_Implementation.cpp
# End Source File
# Begin Source File

SOURCE=.\ExceptionList.cpp
# End Source File
# Begin Source File

SOURCE=.\Request.cpp
# End Source File
# Begin Source File

SOURCE=.\Server_Request.cpp
# End Source File
# Begin Source File

SOURCE=.\Unknown_User_Exception.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\Context.h
# End Source File
# Begin Source File

SOURCE=.\DII_ClientRequestInfo.h
# End Source File
# Begin Source File

SOURCE=.\DII_Invocation.h
# End Source File
# Begin Source File

SOURCE=.\DII_Reply_Dispatcher.h
# End Source File
# Begin Source File

SOURCE=.\Dynamic_Adapter_Impl.h
# End Source File
# Begin Source File

SOURCE=.\Dynamic_Implementation.h
# End Source File
# Begin Source File

SOURCE=.\dynamicinterface_export.h
# End Source File
# Begin Source File

SOURCE=.\ExceptionList.h
# End Source File
# Begin Source File

SOURCE=.\Request.h
# End Source File
# Begin Source File

SOURCE=.\Server_Request.h
# End Source File
# Begin Source File

SOURCE=.\Unknown_User_Exception.h
# End Source File
# End Group
# Begin Group "Inline Files"

# PROP Default_Filter "inl"
# Begin Source File

SOURCE=.\Context.inl
# End Source File
# Begin Source File

SOURCE=.\DII_Invocation.inl
# End Source File
# Begin Source File

SOURCE=.\ExceptionList.inl
# End Source File
# Begin Source File

SOURCE=.\Request.inl
# End Source File
# Begin Source File

SOURCE=.\Server_Request.inl
# End Source File
# End Group
# End Target
# End Project
