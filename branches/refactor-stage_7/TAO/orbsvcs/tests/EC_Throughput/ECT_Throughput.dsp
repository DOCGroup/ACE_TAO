# Microsoft Developer Studio Project File - Name="ECT_Throughput" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Console Application" 0x0103

CFG=ECT_Throughput - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE run the tool that generated this project file and specify the
!MESSAGE nmake output type.  You can then use the following command:
!MESSAGE
!MESSAGE NMAKE /f "ECT_Throughput.mak".
!MESSAGE
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE
!MESSAGE NMAKE /f "ECT_Throughput.mak" CFG="ECT_Throughput - Win32 Debug"
!MESSAGE
!MESSAGE Possible choices for configuration are:
!MESSAGE
!MESSAGE "ECT_Throughput - Win32 Release" (based on "Win32 (x86) Console Application")
!MESSAGE "ECT_Throughput - Win32 Debug" (based on "Win32 (x86) Console Application")
!MESSAGE

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "ECT_Throughput - Win32 Release"

# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release\ECT_Throughput"
# PROP Target_Dir ""
# ADD CPP /nologo /O2 /W3 /GX /MD /GR /I "..\..\..\orbsvcs" /I "..\..\.." /I "..\..\..\tao" /I "..\..\..\.." /D NDEBUG /D WIN32 /D _CONSOLE /FD /c
# SUBTRACT CPP /YX
# ADD RSC /l 0x409 /d NDEBUG /i "..\..\..\orbsvcs" /i "..\..\.." /i "..\..\..\tao" /i "..\..\..\.."
BSC32=bscmake.exe
# ADD BSC32 /nologo 
LINK32=link.exe
# ADD LINK32 advapi32.lib user32.lib /INCREMENTAL:NO TAO_RTOLDEvent.lib TAO_RTSched.lib TAO_CosNaming.lib TAO_IORTable.lib TAO_RTEvent.lib TAO_Svc_Utils.lib TAO_Messaging.lib TAO_PortableServer.lib TAO_IORInterceptor.lib TAO_Valuetype.lib TAO_ObjRefTemplate.lib TAO.lib ACE.lib /libpath:"..\..\..\..\lib" /nologo /version:1.3.4 /subsystem:console /pdb:"Release\ECT_Throughput.pdb"  /machine:I386 /out:"Release\ECT_Throughput.exe"

!ELSEIF  "$(CFG)" == "ECT_Throughput - Win32 Debug"

# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "."
# PROP Intermediate_Dir "Debug\ECT_Throughput"
# PROP Target_Dir ""
# ADD CPP /nologo /Ob0 /W3 /Gm /GX /Zi /MDd /GR /Gy /I "..\..\..\orbsvcs" /I "..\..\.." /I "..\..\..\tao" /I "..\..\..\.." /D _DEBUG /D WIN32 /D _CONSOLE /FD /c
# SUBTRACT CPP /YX
# ADD RSC /l 0x409 /d _DEBUG /i "..\..\..\orbsvcs" /i "..\..\.." /i "..\..\..\tao" /i "..\..\..\.."
BSC32=bscmake.exe
# ADD BSC32 /nologo 
LINK32=link.exe
# ADD LINK32 advapi32.lib user32.lib /INCREMENTAL:NO TAO_RTOLDEventd.lib TAO_RTSchedd.lib TAO_CosNamingd.lib TAO_IORTabled.lib TAO_RTEventd.lib TAO_Svc_Utilsd.lib TAO_Messagingd.lib TAO_PortableServerd.lib TAO_IORInterceptord.lib TAO_Valuetyped.lib TAO_ObjRefTemplated.lib TAOd.lib ACEd.lib /libpath:"..\..\..\..\lib" /nologo /version:1.3.4 /subsystem:console /pdb:".\ECT_Throughput.pdb" /debug /machine:I386 /out:".\ECT_Throughput.exe"

!ENDIF

# Begin Target

# Name "ECT_Throughput - Win32 Release"
# Name "ECT_Throughput - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;cxx;c"
# Begin Source File

SOURCE=".\ECT_Consumer.cpp"
# End Source File
# Begin Source File

SOURCE=".\ECT_Driver.cpp"
# End Source File
# Begin Source File

SOURCE=".\ECT_Supplier.cpp"
# End Source File
# Begin Source File

SOURCE=".\ECT_Throughput.cpp"
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hh"
# Begin Source File

SOURCE=".\ECT_Consumer.h"
# End Source File
# Begin Source File

SOURCE=".\ECT_Driver.h"
# End Source File
# Begin Source File

SOURCE=".\ECT_Supplier.h"
# End Source File
# Begin Source File

SOURCE=".\ECT_Throughput.h"
# End Source File
# End Group
# Begin Group "Inline Files"

# PROP Default_Filter "i;inl"
# Begin Source File

SOURCE=".\ECT_Driver.i"
# End Source File
# End Group
# Begin Group "Documentation"

# PROP Default_Filter ""
# Begin Source File

SOURCE=".\README"
# End Source File
# End Group
# End Target
# End Project
