# Microsoft Developer Studio Project File - Name="Valuetype_Static" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Static Library" 0x0104

CFG=Valuetype_Static - Win32 Static Release
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "TAO_Valuetype_Static.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "TAO_Valuetype_Static.mak" CFG="Valuetype_Static - Win32 Static Release"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "Valuetype_Static - Win32 Static Release" (based on "Win32 (x86) Static Library")
!MESSAGE "Valuetype_Static - Win32 Static Debug" (based on "Win32 (x86) Static Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "Valuetype_Static - Win32 Static Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir ""
# PROP Intermediate_Dir "Release"
# PROP Target_Dir ""
MTL=midl.exe
LINK32=link.exe -lib
# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_MBCS" /D "_LIB" /YX /FD /c
# ADD CPP /nologo /MD /W3 /GX /O2 /I "..\.." /I "..\..\.." /D "NDEBUG" /D "WIN32" /D "_MBCS" /D "_LIB" /D "ACE_AS_STATIC_LIBS" /D "TAO_AS_STATIC_LIBS" /YX /FD /c
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo /out:"TAO_Valuetypes.lib"

!ELSEIF  "$(CFG)" == "Valuetype_Static - Win32 Static Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir ""
# PROP Intermediate_Dir "Debug"
# PROP Target_Dir ""
MTL=midl.exe
LINK32=link.exe -lib
# ADD BASE CPP /nologo /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_MBCS" /D "_LIB" /YX /FD  /c
# ADD CPP /nologo /MDd /W3 /Gm /GX /Zi /Od /I "..\.." /I "..\..\.." /D "_DEBUG" /D "WIN32" /D "_MBCS" /D "_LIB" /D "ACE_AS_STATIC_LIBS" /D "TAO_AS_STATIC_LIBS" /YX /FD  /c
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo /out:"TAO_Valuetypesd.lib"

!ENDIF 

# Begin Target

# Name "Valuetype_Static - Win32 Static Release"
# Name "Valuetype_Static - Win32 Static Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\AbstractBase.cpp
# End Source File
# Begin Source File

SOURCE=.\Seq_Tmplinst.cpp
# End Source File
# Begin Source File

SOURCE=.\ValueBase.cpp
# End Source File
# Begin Source File

SOURCE=.\ValueFactory.cpp
# End Source File
# Begin Source File

SOURCE=.\ValueFactory_Map.cpp
# End Source File
# Begin Source File

SOURCE=.\Valuetype_Adapter_Impl.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\AbstractBase.h
# End Source File
# Begin Source File

SOURCE=.\Sequence_T.h
# End Source File
# Begin Source File

SOURCE=.\Value_VarOut_T.h
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

SOURCE=.\Valuetype_Adapter_Impl.h
# End Source File
# Begin Source File

SOURCE=.\valuetype_export.h
# End Source File
# End Group
# Begin Group "Inline Files"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\AbstractBase.inl
# End Source File
# Begin Source File

SOURCE=.\Sequence_T.inl
# End Source File
# Begin Source File

SOURCE=.\Value_VarOut_T.inl
# End Source File
# Begin Source File

SOURCE=.\ValueFactory.inl
# End Source File
# Begin Source File

SOURCE=.\ValueFactory_Map.inl
# End Source File
# End Group
# Begin Group "Template Files"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\Sequence_T.cpp
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\Value_VarOut_T.cpp
# PROP Exclude_From_Build 1
# End Source File
# End Group
# End Target
# End Project
