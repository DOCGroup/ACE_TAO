# Microsoft Developer Studio Project File - Name="Simple_Naming" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Console Application" 0x0103

CFG=Simple_Naming - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "Simple_Naming.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "Simple_Naming.mak" CFG="Simple_Naming - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "Simple_Naming - Win32 Release" (based on "Win32 (x86) Console Application")
!MESSAGE "Simple_Naming - Win32 Debug" (based on "Win32 (x86) Console Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "Simple_Naming - Win32 Release"

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
# ADD CPP /nologo /MD /W3 /GX /O2 /I "..\..\.." /I "..\..\..\.." /I "..\..\..\..\.." /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /D "_MBCS" /FD /c
# SUBTRACT CPP /YX
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /machine:I386
# ADD LINK32 TAO_PortableServer.lib TAO.lib ace.lib TAO_CosNaming.lib TAO_RTEvent.lib TAO_RTSched.lib TAO_Messaging.lib /nologo /subsystem:console /machine:I386 /libpath:"..\..\..\orbsvcs" /libpath:"..\..\..\..\tao\PortableServer" /libpath:"..\..\..\..\tao" /libpath:"..\..\..\..\..\ace" /libpath:"..\..\..\..\tao\Messaging"

!ELSEIF  "$(CFG)" == "Simple_Naming - Win32 Debug"

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
# ADD CPP /nologo /MDd /W3 /Gm /GX /Zi /Od /I "..\..\.." /I "..\..\..\.." /I "..\..\..\..\.." /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /D "_MBCS" /FD /c
# SUBTRACT CPP /YX
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /debug /machine:I386 /pdbtype:sept
# ADD LINK32 TAO_PSDL_Datastored.lib TAO_PSDLd.lib TAO_PortableServerd.lib TAOd.lib aced.lib /nologo /subsystem:console /debug /machine:I386 /pdbtype:sept /libpath:"..\..\PSS" /libpath:"..\..\..\tao\PortableServer" /libpath:"..\..\..\tao" /libpath:"..\..\..\..\ace"

!ENDIF 

# Begin Target

# Name "Simple_Naming - Win32 Release"
# Name "Simple_Naming - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter ".cpp"
# Begin Source File

SOURCE=.\naming_dataC.cpp
# End Source File
# Begin Source File

SOURCE=.\Simple_Naming.cpp
# End Source File
# Begin Source File

SOURCE=.\Simple_Naming_i.cpp
# End Source File
# Begin Source File

SOURCE=.\Simple_NamingC.cpp
# End Source File
# Begin Source File

SOURCE=.\Simple_NamingS.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter ".h"
# Begin Source File

SOURCE=.\naming_dataC.h
# End Source File
# Begin Source File

SOURCE=.\Simple_Naming_i.h
# End Source File
# Begin Source File

SOURCE=.\Simple_NamingC.h
# End Source File
# Begin Source File

SOURCE=.\Simple_NamingS.h
# End Source File
# End Group
# Begin Group "IDL Files"

# PROP Default_Filter "idl, psdl"
# Begin Source File

SOURCE=.\Simple_Naming.idl

!IF  "$(CFG)" == "Simple_Naming - Win32 Release"

!ELSEIF  "$(CFG)" == "Simple_Naming - Win32 Debug"

# Begin Custom Build - Invoking TAO_IDL Compiler
InputPath=.\Simple_Naming.idl
InputName=Simple_Naming

BuildCmds= \
	..\..\..\..\bin\tao_idl -Ge 1 $(InputName).idl

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
# End Custom Build

!ENDIF 

# End Source File
# End Group
# Begin Group "Inline Files"

# PROP Default_Filter "i, inl"
# Begin Source File

SOURCE=.\naming_dataC.i
# End Source File
# Begin Source File

SOURCE=.\Simple_NamingC.i
# End Source File
# Begin Source File

SOURCE=.\Simple_NamingS.i
# End Source File
# End Group
# Begin Group "PSDL Files"

# PROP Default_Filter ".psdl"
# Begin Source File

SOURCE=.\naming_data.psdl

!IF  "$(CFG)" == "Simple_Naming - Win32 Release"

!ELSEIF  "$(CFG)" == "Simple_Naming - Win32 Debug"

# Begin Custom Build - Invoking TAO_PSDL Compiler
InputPath=.\naming_data.psdl
InputName=naming_data

BuildCmds= \
	..\..\..\..\bin\psdl_tao $(InputName).psdl

"$(InputName)C.h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)C.i" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)C.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)
# End Custom Build

!ENDIF 

# End Source File
# End Group
# End Target
# End Project
