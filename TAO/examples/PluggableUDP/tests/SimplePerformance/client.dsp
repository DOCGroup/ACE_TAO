# Microsoft Developer Studio Project File - Name="Simple Performance Client" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Console Application" 0x0103

CFG=Simple Performance Client - Win32 Static Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "client.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "client.mak" CFG="Simple Performance Client - Win32 Static Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "Simple Performance Client - Win32 Release" (based on "Win32 (x86) Console Application")
!MESSAGE "Simple Performance Client - Win32 Debug" (based on "Win32 (x86) Console Application")
!MESSAGE "Simple Performance Client - Win32 Static Debug" (based on "Win32 (x86) Console Application")
!MESSAGE "Simple Performance Client - Win32 Static Release" (based on "Win32 (x86) Console Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "Simple Performance Client - Win32 Release"

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
# ADD CPP /nologo /MD /W3 /GX /O2 /I "..\..\..\..\\" /I "..\..\..\..\..\\" /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /D "_MBCS" /YX /FD /c
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /machine:I386
# ADD LINK32 ace.lib TAO.lib TAO_Strategies.lib /nologo /subsystem:console /machine:I386 /libpath:"..\..\..\..\..\ace" /libpath:"..\..\..\..\tao" /libpath:"..\..\..\..\tao\Strategies"

!ELSEIF  "$(CFG)" == "Simple Performance Client - Win32 Debug"

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
# ADD CPP /nologo /MDd /W3 /Gm /GX /Zi /Od /I "..\..\..\..\\" /I "..\..\..\..\..\\" /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /D "_MBCS" /FD /c
# SUBTRACT CPP /YX
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /debug /machine:I386 /pdbtype:sept
# ADD LINK32 aced.lib TAOd.lib TAO_Strategiesd.lib /nologo /subsystem:console /debug /machine:I386 /pdbtype:sept /libpath:"..\..\..\..\..\ace" /libpath:"..\..\..\..\tao" /libpath:"..\..\..\..\tao\Strategies"

!ELSEIF  "$(CFG)" == "Simple Performance Client - Win32 Static Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Simple_Performance_Client___Win32_Static_Debug"
# PROP BASE Intermediate_Dir "Simple_Performance_Client___Win32_Static_Debug"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Static_Debug"
# PROP Intermediate_Dir "Static_Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MDd /W3 /Gm /GX /Zi /Od /I "..\..\..\..\\" /I "..\..\..\..\..\\" /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /D "_MBCS" /FD /c
# SUBTRACT BASE CPP /YX
# ADD CPP /nologo /MDd /W3 /Gm /GX /Zi /Od /I "..\..\..\..\\" /I "..\..\..\..\..\\" /D "_DEBUG" /D "WIN32" /D "_CONSOLE" /D "ACE_AS_STATIC_LIBS" /D "TAO_AS_STATIC_LIBS" /FD /c
# SUBTRACT CPP /YX
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 aced.lib TAOd.lib /nologo /subsystem:console /debug /machine:I386 /pdbtype:sept /libpath:"..\..\..\..\..\ace" /libpath:"..\..\..\..\tao"
# ADD LINK32 TAOsd.lib acesd.lib TAO_Strategiessd.lib advapi32.lib user32.lib /nologo /subsystem:console /debug /machine:I386 /pdbtype:sept /libpath:"..\..\..\..\..\ace" /libpath:"..\..\..\..\tao" /libpath:"..\..\..\..\tao\Strategies"

!ELSEIF  "$(CFG)" == "Simple Performance Client - Win32 Static Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Simple_Performance_Client___Win32_Static_Release"
# PROP BASE Intermediate_Dir "Simple_Performance_Client___Win32_Static_Release"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "client\Static Release"
# PROP Intermediate_Dir "client\Static Release"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MD /W3 /GX /O2 /I "..\..\..\..\\" /I "..\..\..\..\..\\" /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /D "_MBCS" /YX /FD /c
# ADD CPP /nologo /MD /W3 /GX /O2 /I "..\..\..\..\\" /I "..\..\..\..\..\\" /D "NDEBUG" /D "WIN32" /D "_CONSOLE" /D "ACE_AS_STATIC_LIBS" /D "TAO_AS_STATIC_LIBS" /YX /FD /c
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 ace.lib TAO.lib /nologo /subsystem:console /machine:I386 /libpath:"..\..\..\..\..\ace" /libpath:"..\..\..\..\tao"
# ADD LINK32 TAOs.lib aces.lib TAO_Strategiess.lib advapi32.lib user32.lib /nologo /subsystem:console /machine:I386 /libpath:"..\..\..\..\..\ace" /libpath:"..\..\..\..\tao" /libpath:"..\..\..\..\tao\Strategies"

!ENDIF 

# Begin Target

# Name "Simple Performance Client - Win32 Release"
# Name "Simple Performance Client - Win32 Debug"
# Name "Simple Performance Client - Win32 Static Debug"
# Name "Simple Performance Client - Win32 Static Release"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\client.cpp
# End Source File
# Begin Source File

SOURCE=.\testC.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\testC.h
# End Source File
# End Group
# Begin Group "IDL Files"

# PROP Default_Filter "*.idl"
# Begin Source File

SOURCE=.\test.idl

!IF  "$(CFG)" == "Simple Performance Client - Win32 Release"

# PROP Ignore_Default_Tool 1
# Begin Custom Build - Invoking TAO_IDL Compiler on $(InputPath)
InputPath=.\test.idl
InputName=test

BuildCmds= \
	..\..\..\..\..\bin\Release\tao_idl -Ge 1 -Sp -Sd $(InputName).idl

"$(InputName)C.h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)C.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)C.i" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)S.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)S.i" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)S.h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)S_T.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)S_T.i" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)S_T.h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)
# End Custom Build

!ELSEIF  "$(CFG)" == "Simple Performance Client - Win32 Debug"

# Begin Custom Build - Invoking TAO_IDL Compiler on $(InputPath)
InputPath=.\test.idl
InputName=test

BuildCmds= \
	..\..\..\..\..\bin\tao_idl -Ge 1 -Sp -Sd $(InputName).idl

"$(InputName)C.h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)C.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)C.i" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)S.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)S.i" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)S.h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)S_T.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)S_T.i" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)S_T.h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)
# End Custom Build

!ELSEIF  "$(CFG)" == "Simple Performance Client - Win32 Static Debug"

USERDEP__TEST_="..\..\..\..\..\bin\tao_idl_static.exe"	
# Begin Custom Build - Invoking TAO_IDL Compiler on $(InputPath)
InputPath=.\test.idl
InputName=test

BuildCmds= \
	..\..\..\..\..\bin\tao_idl_static -Ge 1 -Sp -Sd $(InputName).idl

"$(InputName)C.h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)C.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)C.i" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)S.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)S.i" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)S.h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)S_T.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)S_T.i" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)S_T.h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)
# End Custom Build

!ELSEIF  "$(CFG)" == "Simple Performance Client - Win32 Static Release"

# PROP BASE Ignore_Default_Tool 1
# PROP Ignore_Default_Tool 1
USERDEP__TEST_="..\..\..\..\..\bin\Release\tao_idl_static.exe"	
# Begin Custom Build - Invoking TAO_IDL Compiler on $(InputPath)
InputPath=.\test.idl
InputName=test

BuildCmds= \
	..\..\..\..\..\bin\Release\tao_idl_static -Ge 1 -Sp -Sd $(InputName).idl

"$(InputName)C.h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)C.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)C.i" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)S.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)S.i" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)S.h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)S_T.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)S_T.i" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName)S_T.h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)
# End Custom Build

!ENDIF 

# End Source File
# End Group
# End Target
# End Project
