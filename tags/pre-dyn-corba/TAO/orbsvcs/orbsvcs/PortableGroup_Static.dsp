# Microsoft Developer Studio Project File - Name="PortableGroup_Static" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Static Library" 0x0104

CFG=PortableGroup_Static - Win32 Static Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "PortableGroup_Static.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "PortableGroup_Static.mak" CFG="PortableGroup_Static - Win32 Static Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "PortableGroup_Static - Win32 Static Release" (based on "Win32 (x86) Static Library")
!MESSAGE "PortableGroup_Static - Win32 Static Debug" (based on "Win32 (x86) Static Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "PortableGroup_Static - Win32 Static Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir ""
# PROP Intermediate_Dir "LIB\Release\PortableGroup"
# PROP Target_Dir ""
LINK32=link.exe -lib
MTL=midl.exe
# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_MBCS" /D "_LIB" /YX /FD /c
# ADD CPP /nologo /MD /W3 /GX /O2 /I "../" /I "../../" /I "../../../" /D "_MBCS" /D "_LIB" /D "TAO_AS_STATIC_LIBS" /D "NDEBUG" /D "ACE_AS_STATIC_LIBS" /D "WIN32" /FD /c
# SUBTRACT CPP /YX
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo /out:"TAO_PortableGroups.lib"

!ELSEIF  "$(CFG)" == "PortableGroup_Static - Win32 Static Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir ""
# PROP Intermediate_Dir "LIB\Debug\PortableGroup"
# PROP Target_Dir ""
LINK32=link.exe -lib
MTL=midl.exe
# ADD BASE CPP /nologo /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_MBCS" /D "_LIB" /YX /FD /c
# ADD CPP /nologo /MDd /W3 /Gm /GX /Zi /Od /I "../" /I "../../" /I "../../../" /D "_MBCS" /D "_LIB" /D "TAO_AS_STATIC_LIBS" /D "_DEBUG" /D "ACE_AS_STATIC_LIBS" /D "WIN32" /FD /c
# SUBTRACT CPP /YX
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo /out:"TAO_PortableGroupsd.lib"

!ENDIF 

# Begin Target

# Name "PortableGroup_Static - Win32 Static Release"
# Name "PortableGroup_Static - Win32 Static Debug"
# Begin Group "IDL Files"

# PROP Default_Filter "idl"
# Begin Source File

SOURCE=.\miop.idl

!IF  "$(CFG)" == "PortableGroup_Static - Win32 Static Release"

# PROP Ignore_Default_Tool 1
# Begin Custom Build
InputPath=.\miop.idl
InputName=miop

BuildCmds= \
	..\..\..\bin\Release\tao_idl_static -Ge 1 -I../..  -Wb,pre_include=ace/pre.h -Wb,post_include=ace/post.h -Wb,export_macro=TAO_PortableGroup_Export -Wb,export_include=PortableGroup\portablegroup_export.h $(InputName).idl

"$(InputName)C.h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)C.i" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)C.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)S.h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)S.i" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)S.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)S_T.h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)S_T.i" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)S_T.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)
# End Custom Build

!ELSEIF  "$(CFG)" == "PortableGroup_Static - Win32 Static Debug"

# Begin Custom Build
InputPath=.\miop.idl
InputName=miop

BuildCmds= \
	..\..\..\bin\tao_idl_static -Ge 1 -I../..  -Wb,pre_include=ace/pre.h -Wb,post_include=ace/post.h -Wb,export_macro=TAO_PortableGroup_Export -Wb,export_include=PortableGroup\portablegroup_export.h $(InputName).idl

"$(InputName)C.h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)C.i" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)C.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)S.h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)S.i" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)S.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)S_T.h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)S_T.i" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)S_T.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)
# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\PortableGroup.idl

!IF  "$(CFG)" == "PortableGroup_Static - Win32 Static Release"

# PROP Ignore_Default_Tool 1
# Begin Custom Build - Invoking TAO_IDL on $(InputPath)
InputPath=.\PortableGroup.idl
InputName=PortableGroup

BuildCmds= \
	..\..\..\bin\Release\tao_idl_static -Ge 1 -GC -I../.. -Wb,pre_include=ace/pre.h -Wb,post_include=ace/post.h -Wb,export_macro=TAO_PortableGroup_Export -Wb,export_include=PortableGroup\portablegroup_export.h $(InputName).idl

"$(InputName)C.h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)C.i" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)C.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)S.h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)S.i" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)S.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)S_T.h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)S_T.i" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)S_T.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)
# End Custom Build

!ELSEIF  "$(CFG)" == "PortableGroup_Static - Win32 Static Debug"

# PROP Ignore_Default_Tool 1
USERDEP__PORTA="..\..\..\bin\tao_idl_static.exe"	
# Begin Custom Build - Invoking TAO_IDL on $(InputPath)
InputPath=.\PortableGroup.idl
InputName=PortableGroup

BuildCmds= \
	..\..\..\bin\tao_idl_static -Ge 1 -GC -I../.. -Wb,pre_include=ace/pre.h -Wb,post_include=ace/post.h -Wb,export_macro=TAO_PortableGroup_Export -Wb,export_include=PortableGroup\portablegroup_export.h $(InputName).idl

"$(InputName)C.h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)C.i" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)C.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)S.h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)S.i" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)S.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)S_T.h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)S_T.i" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)S_T.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)
# End Custom Build

!ENDIF 

# End Source File
# End Group
# Begin Group "Inline Files"

# PROP Default_Filter "i;inl"
# Begin Source File

SOURCE=.\miopC.i
# End Source File
# Begin Source File

SOURCE=.\PortableGroup\PG_Location_Equal_To.inl
# End Source File
# Begin Source File

SOURCE=.\PortableGroup\PG_Location_Hash.inl
# End Source File
# Begin Source File

SOURCE=.\PortableGroupC.i
# End Source File
# Begin Source File

SOURCE=.\PortableGroupS.i
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h"
# Begin Source File

SOURCE=.\miopC.h
# End Source File
# Begin Source File

SOURCE=.\PortableGroup\miopconf.h
# End Source File
# Begin Source File

SOURCE=.\PortableGroup\PG_conf.h
# End Source File
# Begin Source File

SOURCE=.\PortableGroup\PG_Default_Property_Validator.h
# End Source File
# Begin Source File

SOURCE=.\PortableGroup\PG_Factory_Map.h
# End Source File
# Begin Source File

SOURCE=.\PortableGroup\PG_Factory_Set.h
# End Source File
# Begin Source File

SOURCE=.\PortableGroup\PG_GenericFactory.h
# End Source File
# Begin Source File

SOURCE=.\PortableGroup\PG_Group_Guard.h
# End Source File
# Begin Source File

SOURCE=.\PortableGroup\PG_Location_Equal_To.h
# End Source File
# Begin Source File

SOURCE=.\PortableGroup\PG_Location_Hash.h
# End Source File
# Begin Source File

SOURCE=.\PortableGroup\PG_Location_Map.h
# End Source File
# Begin Source File

SOURCE=.\PortableGroup\PG_MemberInfo.h
# End Source File
# Begin Source File

SOURCE=.\PortableGroup\PG_Null_Property_Validator.h
# End Source File
# Begin Source File

SOURCE=.\PortableGroup\PG_ObjectGroup_Map.h
# End Source File
# Begin Source File

SOURCE=.\PortableGroup\PG_ObjectGroupManager.h
# End Source File
# Begin Source File

SOURCE=.\PortableGroup\PG_Operators.h
# End Source File
# Begin Source File

SOURCE=.\PortableGroup\PG_Properties.h
# End Source File
# Begin Source File

SOURCE=.\PortableGroup\PG_Property_Utils.h
# End Source File
# Begin Source File

SOURCE=.\PortableGroup\PG_Property_Validator.h
# End Source File
# Begin Source File

SOURCE=.\PortableGroup\PG_PropertyManager.h
# End Source File
# Begin Source File

SOURCE=.\PortableGroup\POA_Hooks.h
# End Source File
# Begin Source File

SOURCE=.\PortableGroup\Portable_Group_Map.h
# End Source File
# Begin Source File

SOURCE=.\PortableGroup\PortableGroup_Acceptor_Registry.h
# End Source File
# Begin Source File

SOURCE=.\PortableGroup\portablegroup_export.h
# End Source File
# Begin Source File

SOURCE=.\PortableGroup\PortableGroup_Loader.h
# End Source File
# Begin Source File

SOURCE=.\PortableGroup\PortableGroup_ORBInitializer.h
# End Source File
# Begin Source File

SOURCE=.\PortableGroup\PortableGroup_Request_Dispatcher.h
# End Source File
# Begin Source File

SOURCE=.\PortableGroupC.h
# End Source File
# Begin Source File

SOURCE=.\PortableGroupS.h
# End Source File
# Begin Source File

SOURCE=.\PortableGroup\UIPMC_Acceptor.h
# End Source File
# Begin Source File

SOURCE=.\PortableGroup\UIPMC_Connection_Handler.h
# End Source File
# Begin Source File

SOURCE=.\PortableGroup\UIPMC_Connector.h
# End Source File
# Begin Source File

SOURCE=.\PortableGroup\UIPMC_Endpoint.h
# End Source File
# Begin Source File

SOURCE=.\PortableGroup\UIPMC_Factory.h
# End Source File
# Begin Source File

SOURCE=.\PortableGroup\UIPMC_Profile.h
# End Source File
# Begin Source File

SOURCE=.\PortableGroup\UIPMC_Transport.h
# End Source File
# Begin Source File

SOURCE=.\PortableGroup\UIPMC_Wait_Never.h
# End Source File
# End Group
# Begin Group "Source Files"

# PROP Default_Filter "cpp"
# Begin Source File

SOURCE=.\miopC.cpp
# End Source File
# Begin Source File

SOURCE=.\PortableGroup\PG_Default_Property_Validator.cpp
# End Source File
# Begin Source File

SOURCE=.\PortableGroup\PG_Factory_Map.cpp
# End Source File
# Begin Source File

SOURCE=.\PortableGroup\PG_Factory_Set.cpp
# End Source File
# Begin Source File

SOURCE=.\PortableGroup\PG_GenericFactory.cpp
# End Source File
# Begin Source File

SOURCE=.\PortableGroup\PG_Group_Guard.cpp
# End Source File
# Begin Source File

SOURCE=.\PortableGroup\PG_Location_Equal_To.cpp
# End Source File
# Begin Source File

SOURCE=.\PortableGroup\PG_Location_Hash.cpp
# End Source File
# Begin Source File

SOURCE=.\PortableGroup\PG_Location_Map.cpp
# End Source File
# Begin Source File

SOURCE=.\PortableGroup\PG_MemberInfo.cpp
# End Source File
# Begin Source File

SOURCE=.\PortableGroup\PG_Null_Property_Validator.cpp
# End Source File
# Begin Source File

SOURCE=.\PortableGroup\PG_ObjectGroup_Map.cpp
# End Source File
# Begin Source File

SOURCE=.\PortableGroup\PG_ObjectGroupManager.cpp
# End Source File
# Begin Source File

SOURCE=.\PortableGroup\PG_Operators.cpp
# End Source File
# Begin Source File

SOURCE=.\PortableGroup\PG_Property_Utils.cpp
# End Source File
# Begin Source File

SOURCE=.\PortableGroup\PG_PropertyManager.cpp
# End Source File
# Begin Source File

SOURCE=.\PortableGroup\POA_Hooks.cpp
# End Source File
# Begin Source File

SOURCE=.\PortableGroup\Portable_Group_Map.cpp
# End Source File
# Begin Source File

SOURCE=.\PortableGroup\PortableGroup_Acceptor_Registry.cpp
# End Source File
# Begin Source File

SOURCE=.\PortableGroup\PortableGroup_Loader.cpp
# End Source File
# Begin Source File

SOURCE=.\PortableGroup\PortableGroup_ORBInitializer.cpp
# End Source File
# Begin Source File

SOURCE=.\PortableGroup\PortableGroup_Request_Dispatcher.cpp
# End Source File
# Begin Source File

SOURCE=.\PortableGroupC.cpp
# End Source File
# Begin Source File

SOURCE=.\PortableGroupS.cpp
# End Source File
# Begin Source File

SOURCE=.\PortableGroup\UIPMC_Acceptor.cpp
# End Source File
# Begin Source File

SOURCE=.\PortableGroup\UIPMC_Connection_Handler.cpp
# End Source File
# Begin Source File

SOURCE=.\PortableGroup\UIPMC_Connector.cpp
# End Source File
# Begin Source File

SOURCE=.\PortableGroup\UIPMC_Endpoint.cpp
# End Source File
# Begin Source File

SOURCE=.\PortableGroup\UIPMC_Factory.cpp
# End Source File
# Begin Source File

SOURCE=.\PortableGroup\UIPMC_Profile.cpp
# End Source File
# Begin Source File

SOURCE=.\PortableGroup\UIPMC_Transport.cpp
# End Source File
# Begin Source File

SOURCE=.\PortableGroup\UIPMC_Wait_Never.cpp
# End Source File
# End Group
# End Target
# End Project
