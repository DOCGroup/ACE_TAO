# Microsoft Developer Studio Project File - Name="IFR_Service" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Console Application" 0x0103

CFG=IFR_Service - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "IFR_Service.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "IFR_Service.mak" CFG="IFR_Service - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "IFR_Service - Win32 Release" (based on "Win32 (x86) Console Application")
!MESSAGE "IFR_Service - Win32 Debug" (based on "Win32 (x86) Console Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "IFR_Service - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /D "_MBCS" /YX /FD /c
# ADD CPP /nologo /MD /W3 /GX /O2 /I "..\..\..\\" /I "..\..\\" /I "..\\" /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /D "_MBCS" /FD /c
# SUBTRACT CPP /YX
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /machine:I386
# ADD LINK32 TAO_TypeCodeFactory.lib TAO_Svc_Utils.lib TAO_IFR_Client.lib TAO_IORTable.lib TAO_PortableServer.lib TAO.lib ace.lib /nologo /subsystem:console /machine:I386 /libpath:"..\orbsvcs" /libpath:"..\..\tao\TypeCodeFactory" /libpath:"..\..\tao\IFR_Client" /libpath:"..\..\tao\IORTable" /libpath:"..\..\tao\PortableServer" /libpath:"..\..\tao" /libpath:"..\..\..\ace"

!ELSEIF  "$(CFG)" == "IFR_Service - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir ""
# PROP Intermediate_Dir "Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /D "_MBCS" /YX /FD /c
# ADD CPP /nologo /MDd /W3 /Gm /GX /Zi /Od /I "..\..\..\\" /I "..\..\\" /I "..\\" /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /D "_MBCS" /FD /c
# SUBTRACT CPP /YX
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /debug /machine:I386 /pdbtype:sept
# ADD LINK32 TAO_TypeCodeFactoryd.lib TAO_Svc_Utilsd.lib TAO_IFR_Clientd.lib TAO_IORTabled.lib TAO_PortableServerd.lib TAOd.lib aced.lib /nologo /subsystem:console /debug /machine:I386 /pdbtype:sept /libpath:"..\orbsvcs" /libpath:"..\..\tao\TypeCodeFactory" /libpath:"..\..\tao\IFR_Client" /libpath:"..\..\tao\IORTable" /libpath:"..\..\tao\PortableServer" /libpath:"..\..\tao" /libpath:"..\..\..\ace"

!ENDIF 

# Begin Target

# Name "IFR_Service - Win32 Release"
# Name "IFR_Service - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\AbstractInterfaceDef_i.cpp
# End Source File
# Begin Source File

SOURCE=.\AliasDef_i.cpp
# End Source File
# Begin Source File

SOURCE=.\ArrayDef_i.cpp
# End Source File
# Begin Source File

SOURCE=.\AttributeDef_i.cpp
# End Source File
# Begin Source File

SOURCE=.\ComponentDef_i.cpp
# End Source File
# Begin Source File

SOURCE=.\ComponentRepository_i.cpp
# End Source File
# Begin Source File

SOURCE=.\ConstantDef_i.cpp
# End Source File
# Begin Source File

SOURCE=.\ConsumesDef_i.cpp
# End Source File
# Begin Source File

SOURCE=.\Contained_i.cpp
# End Source File
# Begin Source File

SOURCE=.\Container_i.cpp
# End Source File
# Begin Source File

SOURCE=.\EmitsDef_i.cpp
# End Source File
# Begin Source File

SOURCE=.\EnumDef_i.cpp
# End Source File
# Begin Source File

SOURCE=.\EventDef_i.cpp
# End Source File
# Begin Source File

SOURCE=.\ExceptionDef_i.cpp
# End Source File
# Begin Source File

SOURCE=.\FactoryDef_i.cpp
# End Source File
# Begin Source File

SOURCE=.\FinderDef_i.cpp
# End Source File
# Begin Source File

SOURCE=.\FixedDef_i.cpp
# End Source File
# Begin Source File

SOURCE=.\HomeDef_i.cpp
# End Source File
# Begin Source File

SOURCE=.\IDLType_i.cpp
# End Source File
# Begin Source File

SOURCE=.\IFR_BaseS.cpp
# End Source File
# Begin Source File

SOURCE=.\IFR_BasicS.cpp
# End Source File
# Begin Source File

SOURCE=.\IFR_ComponentsS.cpp
# End Source File
# Begin Source File

SOURCE=.\IFR_ExtendedS.cpp
# End Source File
# Begin Source File

SOURCE=.\IFR_Server.cpp
# End Source File
# Begin Source File

SOURCE=.\IFR_Service.cpp
# End Source File
# Begin Source File

SOURCE=.\InterfaceDef_i.cpp
# End Source File
# Begin Source File

SOURCE=.\IRObject_i.cpp
# End Source File
# Begin Source File

SOURCE=.\LocalInterfaceDef_i.cpp
# End Source File
# Begin Source File

SOURCE=.\ModuleDef_i.cpp
# End Source File
# Begin Source File

SOURCE=.\NativeDef_i.cpp
# End Source File
# Begin Source File

SOURCE=.\OperationDef_i.cpp
# End Source File
# Begin Source File

SOURCE=.\Options.cpp
# End Source File
# Begin Source File

SOURCE=.\PrimaryKeyDef_i.cpp
# End Source File
# Begin Source File

SOURCE=.\PrimitiveDef_i.cpp
# End Source File
# Begin Source File

SOURCE=.\ProvidesDef_i.cpp
# End Source File
# Begin Source File

SOURCE=.\PublishesDef_i.cpp
# End Source File
# Begin Source File

SOURCE=.\Repository_i.cpp
# End Source File
# Begin Source File

SOURCE=.\SequenceDef_i.cpp
# End Source File
# Begin Source File

SOURCE=.\Servant_Factory.cpp
# End Source File
# Begin Source File

SOURCE=.\Servant_Locator.cpp
# End Source File
# Begin Source File

SOURCE=.\StringDef_i.cpp
# End Source File
# Begin Source File

SOURCE=.\StructDef_i.cpp
# End Source File
# Begin Source File

SOURCE=.\tmplinst.cpp
# End Source File
# Begin Source File

SOURCE=.\TypedefDef_i.cpp
# End Source File
# Begin Source File

SOURCE=.\UnionDef_i.cpp
# End Source File
# Begin Source File

SOURCE=.\UsesDef_i.cpp
# End Source File
# Begin Source File

SOURCE=.\ValueBoxDef_i.cpp
# End Source File
# Begin Source File

SOURCE=.\ValueDef_i.cpp
# End Source File
# Begin Source File

SOURCE=.\ValueMemberDef_i.cpp
# End Source File
# Begin Source File

SOURCE=.\WstringDef_i.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\AbstractInterfaceDef_i.h
# End Source File
# Begin Source File

SOURCE=.\AliasDef_i.h
# End Source File
# Begin Source File

SOURCE=.\ArrayDef_i.h
# End Source File
# Begin Source File

SOURCE=.\AttributeDef_i.h
# End Source File
# Begin Source File

SOURCE=.\ComponentDef_i.h
# End Source File
# Begin Source File

SOURCE=.\ComponentRepository_i.h
# End Source File
# Begin Source File

SOURCE=.\concrete_classes.h
# End Source File
# Begin Source File

SOURCE=.\ConstantDef_i.h
# End Source File
# Begin Source File

SOURCE=.\ConsumesDef_i.h
# End Source File
# Begin Source File

SOURCE=.\Contained_i.h
# End Source File
# Begin Source File

SOURCE=.\Container_i.h
# End Source File
# Begin Source File

SOURCE=.\EmitsDef_i.h
# End Source File
# Begin Source File

SOURCE=.\EnumDef_i.h
# End Source File
# Begin Source File

SOURCE=.\EventDef_i.h
# End Source File
# Begin Source File

SOURCE=.\ExceptionDef_i.h
# End Source File
# Begin Source File

SOURCE=.\FactoryDef_i.h
# End Source File
# Begin Source File

SOURCE=.\FinderDef_i.h
# End Source File
# Begin Source File

SOURCE=.\FixedDef_i.h
# End Source File
# Begin Source File

SOURCE=.\HomeDef_i.h
# End Source File
# Begin Source File

SOURCE=.\IDLType_i.h
# End Source File
# Begin Source File

SOURCE=.\IFR_BaseS.h
# End Source File
# Begin Source File

SOURCE=.\IFR_BaseS_T.h
# End Source File
# Begin Source File

SOURCE=.\IFR_BasicS.h
# End Source File
# Begin Source File

SOURCE=.\IFR_BasicS_T.h
# End Source File
# Begin Source File

SOURCE=.\IFR_ComponentsS.h
# End Source File
# Begin Source File

SOURCE=.\IFR_ComponentsS_T.h
# End Source File
# Begin Source File

SOURCE=.\IFR_ExtendedS.h
# End Source File
# Begin Source File

SOURCE=.\IFR_ExtendedS_T.h
# End Source File
# Begin Source File

SOURCE=.\IFR_macro.h
# End Source File
# Begin Source File

SOURCE=.\IFR_Service.h
# End Source File
# Begin Source File

SOURCE=.\InterfaceDef_i.h
# End Source File
# Begin Source File

SOURCE=.\IRObject_i.h
# End Source File
# Begin Source File

SOURCE=.\LocalInterfaceDef_i.h
# End Source File
# Begin Source File

SOURCE=.\ModuleDef_i.h
# End Source File
# Begin Source File

SOURCE=.\NativeDef_i.h
# End Source File
# Begin Source File

SOURCE=.\OperationDef_i.h
# End Source File
# Begin Source File

SOURCE=.\Options.h
# End Source File
# Begin Source File

SOURCE=.\PrimaryKeyDef_i.h
# End Source File
# Begin Source File

SOURCE=.\PrimitiveDef_i.h
# End Source File
# Begin Source File

SOURCE=.\ProvidesDef_i.h
# End Source File
# Begin Source File

SOURCE=.\PublishesDef_i.h
# End Source File
# Begin Source File

SOURCE=.\Repository_i.h
# End Source File
# Begin Source File

SOURCE=.\SequenceDef_i.h
# End Source File
# Begin Source File

SOURCE=.\Servant_Factory.h
# End Source File
# Begin Source File

SOURCE=.\Servant_Locator.h
# End Source File
# Begin Source File

SOURCE=.\StringDef_i.h
# End Source File
# Begin Source File

SOURCE=.\StructDef_i.h
# End Source File
# Begin Source File

SOURCE=.\TypedefDef_i.h
# End Source File
# Begin Source File

SOURCE=.\UnionDef_i.h
# End Source File
# Begin Source File

SOURCE=.\UsesDef_i.h
# End Source File
# Begin Source File

SOURCE=.\ValueBoxDef_i.h
# End Source File
# Begin Source File

SOURCE=.\ValueDef_i.h
# End Source File
# Begin Source File

SOURCE=.\ValueMemberDef_i.h
# End Source File
# Begin Source File

SOURCE=.\WstringDef_i.h
# End Source File
# End Group
# Begin Group "Inline Files"

# PROP Default_Filter "i"
# Begin Source File

SOURCE=.\IFR_BaseS.i
# End Source File
# Begin Source File

SOURCE=.\IFR_BaseS_T.i
# End Source File
# Begin Source File

SOURCE=.\IFR_BasicS.i
# End Source File
# Begin Source File

SOURCE=.\IFR_BasicS_T.i
# End Source File
# Begin Source File

SOURCE=.\IFR_ComponentsS.i
# End Source File
# Begin Source File

SOURCE=.\IFR_ComponentsS_T.i
# End Source File
# Begin Source File

SOURCE=.\IFR_ExtendedS.i
# End Source File
# Begin Source File

SOURCE=.\IFR_ExtendedS_T.i
# End Source File
# End Group
# End Target
# End Project
