# Microsoft Developer Studio Project File - Name="airplane_client" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Console Application" 0x0103

CFG=airplane_client - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "airplane_client.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "airplane_client.mak" CFG="airplane_client - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "airplane_client - Win32 Release" (based on "Win32 (x86) Console Application")
!MESSAGE "airplane_client - Win32 Debug" (based on "Win32 (x86) Console Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "airplane_client - Win32 Release"

# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release\airplane_client"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD CPP /nologo /MD /W3 /GR /GX /O2 /I "..\..\..\orbsvcs" /I "..\..\.." /I "..\..\..\tao" /I "..\..\..\.." /D "NDEBUG" /D "WIN32" /D "_CONSOLE" /FD /c
# SUBTRACT CPP /YX
# ADD BASE RSC /l 0x409
# ADD RSC /l 0x409 /i "..\..\..\orbsvcs" /i "..\..\.." /i "..\..\..\tao" /i "..\..\..\.." /d "NDEBUG"
BSC32=bscmake.exe
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /machine:IX86
# ADD LINK32 advapi32.lib user32.lib TAO_IORTable.lib TAO_PortableServer.lib TAO_IORInterceptor.lib TAO_Valuetype.lib TAO_ObjRefTemplate.lib TAO.lib ACE.lib /nologo /subsystem:console /machine:I386 /libpath:"../../orbsvcs" /libpath:"../../../tao/IORTable" /libpath:"../../../tao/PortableServer" /libpath:"../../../tao/Valuetype" /libpath:"../../../tao/IORInterceptor" /libpath:"../../../tao/ObjRefTemplate" /libpath:"../../../tao" /libpath:"../../../../ace"
# SUBTRACT LINK32 /pdb:none

!ELSEIF  "$(CFG)" == "airplane_client - Win32 Debug"

# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "."
# PROP Intermediate_Dir "Debug\airplane_client"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD CPP /nologo /MDd /W3 /Gm /GR /GX /Zi /Gy /I "..\..\..\orbsvcs" /I "..\..\.." /I "..\..\..\tao" /I "..\..\..\.." /D "_DEBUG" /D "WIN32" /D "_CONSOLE" /FD /c
# SUBTRACT CPP /YX
# ADD BASE RSC /l 0x409
# ADD RSC /l 0x409 /i "..\..\..\orbsvcs" /i "..\..\.." /i "..\..\..\tao" /i "..\..\..\.." /d "_DEBUG"
BSC32=bscmake.exe
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /machine:IX86
# ADD LINK32 advapi32.lib user32.lib TAO_IORTabled.lib TAO_PortableServerd.lib TAO_IORInterceptord.lib TAO_Valuetyped.lib TAO_ObjRefTemplated.lib TAOd.lib ACEd.lib /nologo /subsystem:console /incremental:no /debug /machine:I386 /libpath:"../../orbsvcs" /libpath:"../../../tao/IORTable" /libpath:"../../../tao/PortableServer" /libpath:"../../../tao/Valuetype" /libpath:"../../../tao/IORInterceptor" /libpath:"../../../tao/ObjRefTemplate" /libpath:"../../../tao" /libpath:"../../../../ace"
# SUBTRACT LINK32 /pdb:none

!ENDIF 

# Begin Target

# Name "airplane_client - Win32 Release"
# Name "airplane_client - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;cxx;c"
# Begin Source File

SOURCE=".\airplane_client.cpp"
# End Source File
# Begin Source File

SOURCE=".\airplane_client_i.cpp"
# End Source File
# Begin Source File

SOURCE=".\AirplaneC.cpp"
# End Source File
# Begin Source File

SOURCE=".\AirplaneS.cpp"
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hh"
# Begin Source File

SOURCE=".\airplane_client_i.h"
# End Source File
# Begin Source File

SOURCE=".\AirplaneC.h"
# End Source File
# Begin Source File

SOURCE=".\AirplaneS.h"
# End Source File
# End Group
# Begin Group "Inline Files"

# PROP Default_Filter "i;inl"
# Begin Source File

SOURCE=".\AirplaneC.i"
# End Source File
# Begin Source File

SOURCE=".\AirplaneS.i"
# End Source File
# End Group
# Begin Group "Documentation"

# PROP Default_Filter ""
# Begin Source File

SOURCE=".\README"
# End Source File
# End Group
# Begin Group "IDL Files"

# PROP Default_Filter "idl"
# Begin Source File

SOURCE=".\Airplane.idl"

!IF  "$(CFG)" == "airplane_client - Win32 Release"

# PROP Ignore_Default_Tool 1
# Begin Custom Build - Invoking IDL Compiler on $(InputPath)
InputPath=".\Airplane.idl"
InputName=Airplane

BuildCmds= \
	PATH=%PATH%;..\..\..\..\lib \
	..\..\..\..\bin\tao_idl -o . -Gv -Ge 1 -Sc -Wb,pre_include=ace\pre.h -Wb,post_include=ace\post.h $(InputPath) \
	

".\$(InputName)C.h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

".\$(InputName)C.i" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

".\$(InputName)C.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

".\$(InputName)S.h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

".\$(InputName)S.i" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

".\$(InputName)S.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)
# End Custom Build

!ELSEIF  "$(CFG)" == "airplane_client - Win32 Debug"

# PROP Ignore_Default_Tool 1
# Begin Custom Build - Invoking IDL Compiler on $(InputPath)
InputPath=".\Airplane.idl"
InputName=Airplane

BuildCmds= \
	PATH=%PATH%;..\..\..\..\lib \
	..\..\..\..\bin\tao_idl -o . -Gv -Ge 1 -Sc -Wb,pre_include=ace\pre.h -Wb,post_include=ace\post.h $(InputPath) \
	

".\$(InputName)C.h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

".\$(InputName)C.i" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

".\$(InputName)C.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

".\$(InputName)S.h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

".\$(InputName)S.i" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

".\$(InputName)S.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)
# End Custom Build

!ENDIF 

# End Source File
# End Group
# End Target
# End Project
