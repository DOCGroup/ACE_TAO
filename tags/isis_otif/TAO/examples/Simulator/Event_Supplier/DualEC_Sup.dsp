# Microsoft Developer Studio Project File - Name="DualEC_Sup" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Console Application" 0x0103

CFG=DualEC_Sup - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "DualEC_Sup.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "DualEC_Sup.mak" CFG="DualEC_Sup - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "DualEC_Sup - Win32 Release" (based on "Win32 (x86) Console Application")
!MESSAGE "DualEC_Sup - Win32 Debug" (based on "Win32 (x86) Console Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "DualEC_Sup - Win32 Release"

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
# ADD CPP /nologo /MD /W3 /GX /O2 /I "..\..\..\..\\" /I "..\..\..\\" /I "..\..\..\orbsvcs\\" /I "..\..\..\orbsvcs\orbsvcs\\" /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /D "_MBCS" /FD /c
# SUBTRACT CPP /YX
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /machine:I386
# ADD LINK32 ace.lib TAO.lib TAO_PortableServer.lib TAO_CosNaming.lib TAO_RTOLDEvent.lib TAO_RTEvent.lib TAO_RTSched.lib TAO_Svc_Utils.lib TAO_Messaging.lib /nologo /subsystem:console /machine:I386 /libpath:"..\..\..\..\ace" /libpath:"..\..\..\tao" /libpath:"..\..\..\tao\PortableServer" /libpath:"..\..\..\orbsvcs\orbsvcs" /libpath:"..\..\..\tao\Messaging"

!ELSEIF  "$(CFG)" == "DualEC_Sup - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "DualEC_S"
# PROP BASE Intermediate_Dir "DualEC_S"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir ""
# PROP Intermediate_Dir "Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /D "_MBCS" /YX /FD /c
# ADD CPP /nologo /MDd /W3 /Gm /GX /Zi /Od /I "..\..\..\..\\" /I "..\..\..\\" /I "..\..\..\orbsvcs\\" /I "..\..\..\orbsvcs\orbsvcs\\" /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /D "_MBCS" /D "TAO_ORBSVCS_HAS_DLL" /FD /c
# SUBTRACT CPP /YX
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /debug /machine:I386 /pdbtype:sept
# ADD LINK32 aced.lib TAOd.lib TAO_PortableServerd.lib TAO_CosNamingd.lib TAO_RTOLDEventd.lib TAO_RTEventd.lib TAO_RTSchedd.lib TAO_Svc_Utilsd.lib TAO_Messagingd.lib /nologo /subsystem:console /debug /machine:I386 /pdbtype:sept /libpath:"..\..\..\..\ace" /libpath:"..\..\..\tao" /libpath:"..\..\..\tao\PortableServer" /libpath:"..\..\..\orbsvcs\orbsvcs" /libpath:"..\..\..\tao\Messaging"

!ENDIF 

# Begin Target

# Name "DualEC_Sup - Win32 Release"
# Name "DualEC_Sup - Win32 Debug"
# Begin Group "Source files"

# PROP Default_Filter "*.cpp"
# Begin Source File

SOURCE=.\DOVE_Supplier.cpp
# End Source File
# Begin Source File

SOURCE=.\DualEC_Sup.cpp
# End Source File
# Begin Source File

SOURCE=.\NavWeapC.cpp
# ADD CPP /GR
# End Source File
# Begin Source File

SOURCE=.\NavWeapS.cpp
# End Source File
# End Group
# Begin Group "Header files"

# PROP Default_Filter "*.h"
# Begin Source File

SOURCE=.\DOVE_Supplier.h
# End Source File
# Begin Source File

SOURCE=.\DualEC_Sup.h
# End Source File
# Begin Source File

SOURCE=.\NavWeapC.h
# End Source File
# Begin Source File

SOURCE=.\NavWeapS.h
# End Source File
# End Group
# Begin Group "IDL files"

# PROP Default_Filter "*.idl"
# Begin Source File

SOURCE="..\NavWeap.idl"

!IF  "$(CFG)" == "DualEC_Sup - Win32 Release"

# PROP Ignore_Default_Tool 1
USERDEP__NAVWE="..\..\..\..\bin\Release\tao_idl.exe"	
# Begin Custom Build - Invoking TAO_IDL on $(InputPath)
InputPath="..\NavWeap.idl"
InputName=NavWeap

BuildCmds= \
	..\..\..\..\bin\Release\tao_idl -Ge 1 -I ..\..\..\ -I ..\..\..\orbsvcs\orbsvcs $(InputPath)

"$(InputName)C.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)C.h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)C.i" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)S.h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)S.i" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)S.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)
# End Custom Build

!ELSEIF  "$(CFG)" == "DualEC_Sup - Win32 Debug"

# PROP Ignore_Default_Tool 1
USERDEP__NAVWE="..\..\..\..\bin\tao_idl.exe"	
# Begin Custom Build - Invoking TAO_IDL on $(InputPath)
InputPath="..\NavWeap.idl"
InputName=NavWeap

BuildCmds= \
	..\..\..\..\bin\tao_idl -Ge 1 -I ..\..\..\ -I ..\..\..\orbsvcs\orbsvcs $(InputPath)

"$(InputName)C.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)C.h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)C.i" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)S.h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)S.i" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)S.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)
# End Custom Build

!ENDIF 

# End Source File
# End Group
# End Target
# End Project
