# Microsoft Developer Studio Project File - Name="TAO_NotifyTests DLL" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Dynamic-Link Library" 0x0102

CFG=TAO_NotifyTests DLL - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "TAO_NotifyTests.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "TAO_NotifyTests.mak" CFG="TAO_NotifyTests DLL - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "TAO_NotifyTests DLL - Win32 Release" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "TAO_NotifyTests DLL - Win32 Debug" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "TAO_NotifyTests DLL - Win32 Release"

# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "."
# PROP Intermediate_Dir "Release"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD CPP /nologo /MD /W3 /GR /GX /Zi /O2 /Ob2 /I "..\..\..\..\orbsvcs" /I "..\..\..\.." /I "..\..\..\..\tao" /I "..\..\..\..\.." /D NDEBUG=1 /D WIN32=1 /D _WINDOWS=1 /D TAO_NOTIFY_TEST_BUILD_DLL=1 /FD /c
# SUBTRACT CPP /YX
# ADD BASE RSC /l 0x409
# ADD RSC /l 0x409 /i "..\..\..\..\orbsvcs" /i "..\..\..\.." /i "..\..\..\..\tao" /i "..\..\..\..\.." /d NDEBUG=1
BSC32=bscmake.exe
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /machine:IX86
# ADD LINK32 advapi32.lib user32.lib TAO_CosEvent.lib TAO_CosNotification.lib TAO_ETCL.lib TAO_CosNaming.lib TAO_Svc_Utils.lib TAO_IORTable.lib TAO_DynamicAny.lib TAO_PortableServer.lib TAO.lib ACE.lib /nologo /version:1.3 /subsystem:windows /dll /machine:I386 /out:"..\..\..\..\..\bin\TAO_NotifyTests.dll" /libpath:"..\..\..\..\tao\PortableServer" /libpath:"..\..\..\..\tao\DynamicAny" /libpath:"..\..\..\..\tao\IORTable" /libpath:"..\..\..\..\orbsvcs\orbsvcs\ETCL" /libpath:"..\..\..\..\orbsvcs\orbsvcs" /libpath:"..\..\..\..\tao" /libpath:"..\..\..\..\..\ace"

!ELSEIF  "$(CFG)" == "TAO_NotifyTests DLL - Win32 Debug"

# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "."
# PROP Intermediate_Dir "Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD CPP /nologo /MDd /W3 /Gm /GR /GX /Zi /Od /Gy /I "..\..\..\..\orbsvcs" /I "..\..\..\.." /I "..\..\..\..\tao" /I "..\..\..\..\.." /D _DEBUG=1 /D WIN32=1 /D _WINDOWS=1 /D TAO_NOTIFY_TEST_BUILD_DLL=1 /FD /c
# SUBTRACT CPP /Fr /YX
# ADD BASE RSC /l 0x409
# ADD RSC /l 0x409 /i "..\..\..\..\orbsvcs" /i "..\..\..\.." /i "..\..\..\..\tao" /i "..\..\..\..\.." /d _DEBUG=1
BSC32=bscmake.exe
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /machine:IX86
# ADD LINK32 advapi32.lib user32.lib TAO_CosEventd.lib TAO_CosNotificationd.lib TAO_ETCLd.lib TAO_CosNamingd.lib TAO_Svc_Utilsd.lib TAO_IORTabled.lib TAO_DynamicAnyd.lib TAO_PortableServerd.lib TAOd.lib ACEd.lib /nologo /version:1.3 /subsystem:windows /dll /incremental:no /pdb:"TAO_NotifyTestsd.pdb" /debug /machine:I386 /out:"..\..\..\..\..\bin\TAO_NotifyTestsd.dll" /libpath:"..\..\..\..\tao\PortableServer" /libpath:"..\..\..\..\tao\DynamicAny" /libpath:"..\..\..\..\tao\IORTable" /libpath:"..\..\..\..\orbsvcs\orbsvcs\ETCL" /libpath:"..\..\..\..\orbsvcs\orbsvcs" /libpath:"..\..\..\..\tao" /libpath:"..\..\..\..\..\ace"
# SUBTRACT LINK32 /pdb:none

!ENDIF 

# Begin Target

# Name "TAO_NotifyTests DLL - Win32 Release"
# Name "TAO_NotifyTests DLL - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;cxx;c"
# Begin Source File

SOURCE=.\Activation_Manager.cpp
# End Source File
# Begin Source File

SOURCE=.\Activation_ManagerC.cpp
# End Source File
# Begin Source File

SOURCE=.\Activation_ManagerS.cpp
# End Source File
# Begin Source File

SOURCE=.\Application_Command.cpp
# End Source File
# Begin Source File

SOURCE=.\Command.cpp
# End Source File
# Begin Source File

SOURCE=.\Command_Builder.cpp
# End Source File
# Begin Source File

SOURCE=.\Command_Factory.cpp
# End Source File
# Begin Source File

SOURCE=.\Command_Factory_T.cpp
# End Source File
# Begin Source File

SOURCE=.\ConsumerAdmin_Command.cpp
# End Source File
# Begin Source File

SOURCE=.\Driver.cpp
# End Source File
# Begin Source File

SOURCE=.\EventChannel_Command.cpp
# End Source File
# Begin Source File

SOURCE=.\Factories_Define.cpp
# End Source File
# Begin Source File

SOURCE=.\Filter_Command.cpp
# End Source File
# Begin Source File

SOURCE=.\LookupManager.cpp
# End Source File
# Begin Source File

SOURCE=.\Name.cpp
# End Source File
# Begin Source File

SOURCE=.\Notify_PushConsumer.cpp
# End Source File
# Begin Source File

SOURCE=.\Notify_PushSupplier.cpp
# End Source File
# Begin Source File

SOURCE=.\Notify_SequencePushConsumer.cpp
# End Source File
# Begin Source File

SOURCE=.\Notify_SequencePushSupplier.cpp
# End Source File
# Begin Source File

SOURCE=.\Notify_StructuredPushConsumer.cpp
# End Source File
# Begin Source File

SOURCE=.\Notify_StructuredPushSupplier.cpp
# End Source File
# Begin Source File

SOURCE=.\Notify_Test_Client.cpp
# End Source File
# Begin Source File

SOURCE=.\Options_Parser.cpp
# End Source File
# Begin Source File

SOURCE=.\Periodic_Consumer.cpp
# End Source File
# Begin Source File

SOURCE=.\Periodic_Consumer_Command.cpp
# End Source File
# Begin Source File

SOURCE=.\Periodic_Supplier.cpp
# End Source File
# Begin Source File

SOURCE=.\Periodic_Supplier_Command.cpp
# End Source File
# Begin Source File

SOURCE=.\Priority_Mapping.cpp
# End Source File
# Begin Source File

SOURCE=.\StructuredEvent.cpp
# End Source File
# Begin Source File

SOURCE=.\SupplierAdmin_Command.cpp
# End Source File
# Begin Source File

SOURCE=.\Task_Stats.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hh"
# Begin Source File

SOURCE=.\Activation_Manager.h
# End Source File
# Begin Source File

SOURCE=.\Activation_ManagerC.h
# End Source File
# Begin Source File

SOURCE=.\Activation_ManagerS.h
# End Source File
# Begin Source File

SOURCE=.\Application_Command.h
# End Source File
# Begin Source File

SOURCE=.\Command.h
# End Source File
# Begin Source File

SOURCE=.\Command_Builder.h
# End Source File
# Begin Source File

SOURCE=.\Command_Factory.h
# End Source File
# Begin Source File

SOURCE=.\Command_Factory_T.h
# End Source File
# Begin Source File

SOURCE=.\ConsumerAdmin_Command.h
# End Source File
# Begin Source File

SOURCE=.\Driver.h
# End Source File
# Begin Source File

SOURCE=.\EventChannel_Command.h
# End Source File
# Begin Source File

SOURCE=.\Factories_Define.h
# End Source File
# Begin Source File

SOURCE=.\Filter_Command.h
# End Source File
# Begin Source File

SOURCE=.\LookupManager.h
# End Source File
# Begin Source File

SOURCE=.\Name.h
# End Source File
# Begin Source File

SOURCE=.\Notify_PushConsumer.h
# End Source File
# Begin Source File

SOURCE=.\Notify_PushSupplier.h
# End Source File
# Begin Source File

SOURCE=.\Notify_SequencePushConsumer.h
# End Source File
# Begin Source File

SOURCE=.\Notify_SequencePushSupplier.h
# End Source File
# Begin Source File

SOURCE=.\Notify_StructuredPushConsumer.h
# End Source File
# Begin Source File

SOURCE=.\Notify_StructuredPushSupplier.h
# End Source File
# Begin Source File

SOURCE=.\Notify_Test_Client.h
# End Source File
# Begin Source File

SOURCE=.\Options_Parser.h
# End Source File
# Begin Source File

SOURCE=.\Periodic_Consumer.h
# End Source File
# Begin Source File

SOURCE=.\Periodic_Consumer_Command.h
# End Source File
# Begin Source File

SOURCE=.\Periodic_Supplier.h
# End Source File
# Begin Source File

SOURCE=.\Periodic_Supplier_Command.h
# End Source File
# Begin Source File

SOURCE=.\Priority_Mapping.h
# End Source File
# Begin Source File

SOURCE=.\StructuredEvent.h
# End Source File
# Begin Source File

SOURCE=.\SupplierAdmin_Command.h
# End Source File
# Begin Source File

SOURCE=.\Task_Stats.h
# End Source File
# End Group
# Begin Group "Inline Files"

# PROP Default_Filter "i;inl"
# Begin Source File

SOURCE=.\Activation_ManagerC.i
# End Source File
# Begin Source File

SOURCE=.\Activation_ManagerS.i
# End Source File
# Begin Source File

SOURCE=.\Task_Stats.inl
# End Source File
# End Group
# Begin Group "Template Files"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\Command_Factory_T.cpp
# PROP Exclude_From_Build 1
# End Source File
# End Group
# Begin Group "Documentation"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\README
# End Source File
# End Group
# Begin Group "IDL Files"

# PROP Default_Filter "idl"
# Begin Source File

SOURCE=.\Activation_Manager.idl

!IF  "$(CFG)" == "TAO_NotifyTests DLL - Win32 Release"

# PROP Ignore_Default_Tool 1
# Begin Custom Build - Invoking TAO_IDL Compiler on $(InputPath)
OutDir=.\.
InputPath=.\Activation_Manager.idl
InputName=Activation_Manager

BuildCmds= \
	..\..\..\..\..\bin\Release\tao_idl -o $(OutDir) -I..\..\..\.. -I..\..\orbsvcs -Ge 1 -Sc $(InputPath)

"$(OutDir)\$(InputName)C.h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(OutDir)\$(InputName)C.i" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(OutDir)\$(InputName)C.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(OutDir)\$(InputName)S.h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(OutDir)\$(InputName)S.i" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(OutDir)\$(InputName)S.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)
# End Custom Build

!ELSEIF  "$(CFG)" == "TAO_NotifyTests DLL - Win32 Debug"

# PROP Ignore_Default_Tool 1
# Begin Custom Build - Invoking TAO_IDL Compiler on $(InputPath)
OutDir=.\.
InputPath=.\Activation_Manager.idl
InputName=Activation_Manager

BuildCmds= \
	..\..\..\..\..\bin\tao_idl -o $(OutDir) -I..\..\..\.. -I..\..\orbsvcs -Ge 1 -Sc $(InputPath)

"$(OutDir)\$(InputName)C.h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(OutDir)\$(InputName)C.i" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(OutDir)\$(InputName)C.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(OutDir)\$(InputName)S.h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(OutDir)\$(InputName)S.i" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(OutDir)\$(InputName)S.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)
# End Custom Build

!ENDIF 

# End Source File
# End Group
# End Target
# End Project
