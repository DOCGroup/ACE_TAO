# Microsoft Developer Studio Project File - Name="Simple Time Client Static" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Console Application" 0x0103

CFG=Simple Time Client Static - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "client_static.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "client_static.mak"\
 CFG="Simple Time Client Static - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "Simple Time Client Static - Win32 Debug" (based on\
 "Win32 (x86) Console Application")
!MESSAGE "Simple Time Client Static - Win32 Release" (based on\
 "Win32 (x86) Console Application")
!MESSAGE 

# Begin Project
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "Simple Time Client Static - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "client\Static Debug"
# PROP BASE Intermediate_Dir "client\Static Debug"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir "client"
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug\Static"
# PROP Intermediate_Dir "Debug\Static"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir "client"
# ADD BASE CPP /nologo /MDd /W3 /Gm /GX /Zi /Od /I "..\..\\" /I "..\..\..\\" /D "_DEBUG" /D "WIN32" /D "_CONSOLE" /FD /c
# SUBTRACT BASE CPP /YX
# ADD CPP /nologo /MTd /W3 /Gm /GX /Zi /Od /I "..\..\..\\" /I "..\..\..\..\\" /I "..\..\..\orbsvcs" /D "_DEBUG" /D "WIN32" /D "_CONSOLE" /D TAO_HAS_DLL=0 /D ACE_NO_INLINE /D ACE_HAS_DLL=0 /FD /c
# SUBTRACT CPP /YX
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 aced.lib TAOd.lib /nologo /subsystem:console /debug /machine:I386 /libpath:"..\..\tao" /libpath:"..\..\..\ace"
# ADD LINK32 acesd.lib TAOsd.lib orbsvcssd.lib advapi32.lib user32.lib /nologo /subsystem:console /debug /machine:I386 /libpath:"..\..\..\tao" /libpath:"..\..\..\..\ace" /libpath:"..\..\..\orbsvcs\orbsvcs"

!ELSEIF  "$(CFG)" == "Simple Time Client Static - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "client\Static Release"
# PROP BASE Intermediate_Dir "client\Static Release"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir "client"
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release\Static"
# PROP Intermediate_Dir "Release\Static"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir "client"
# ADD BASE CPP /nologo /MD /W3 /GX /O2 /I "..\..\\" /I "..\..\..\\" /D "NDEBUG" /D "WIN32" /D "_CONSOLE" /FD /c
# SUBTRACT BASE CPP /YX
# ADD CPP /nologo /MT /W3 /GX /O2 /I "..\..\..\\" /I "..\..\..\..\\" /I "..\..\..\orbsvcs" /D "NDEBUG" /D "WIN32" /D "_CONSOLE" /D ACE_HAS_DLL=0 /D TAO_HAS_DLL=0 /D ACE_NO_INLINE /FD /c
# SUBTRACT CPP /YX
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 ace.lib TAO.lib /nologo /subsystem:console /machine:I386 /libpath:"..\..\tao" /libpath:"..\..\..\ace"
# ADD LINK32 aces.lib TAOs.lib orbsvcss.lib advapi32.lib user32.lib /nologo /subsystem:console /machine:I386 /libpath:"..\..\..\tao" /libpath:"..\..\..\..\ace" /libpath:"..\..\..\orbsvcs\orbsvcs"

!ENDIF 

# Begin Target

# Name "Simple Time Client Static - Win32 Debug"
# Name "Simple Time Client Static - Win32 Release"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;hpj;bat;for;f90"
# Begin Source File

SOURCE=.\client.cpp
# End Source File
# Begin Source File

SOURCE=.\TimeC.cpp
# End Source File
# Begin Source File

SOURCE=.\TimeS.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl;fi;fd"
# Begin Source File

SOURCE=.\Client_i.h
# End Source File
# End Group
# Begin Group "IDL Files"

# PROP Default_Filter ".idl"
# Begin Source File

SOURCE=.\Time.idl

!IF  "$(CFG)" == "Simple Time Client Static - Win32 Debug"

# PROP Ignore_Default_Tool 1
USERDEP__TIME_="..\..\..\tao_idl\tao_idl.exe"	
# Begin Custom Build - Invoking TAO_IDL Compiler
InputPath=.\Time.idl
InputName=Time

BuildCmds= \
	..\..\..\..\bin\tao_idl $(InputName).idl

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

!ELSEIF  "$(CFG)" == "Simple Time Client Static - Win32 Release"

# PROP Ignore_Default_Tool 1
USERDEP__TIME_="..\..\..\tao_idl\Release\tao_idl.exe"	
# Begin Custom Build - Invoking TAO_IDL Compiler
InputPath=.\Time.idl
InputName=Time

BuildCmds= \
	..\..\..\tao_idl\Release\tao_idl $(InputName).idl

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
# End Target
# End Project
