# Microsoft Developer Studio Project File - Name="UDP Basic Client" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Console Application" 0x0103
# TARGTYPE "Win32 (ALPHA) Console Application" 0x0603

CFG=UDP Basic Client - Win32 Static Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "client.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "client.mak" CFG="UDP Basic Client - Win32 Static Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "UDP Basic Client - Win32 Release" (based on "Win32 (x86) Console Application")
!MESSAGE "UDP Basic Client - Win32 Debug" (based on "Win32 (x86) Console Application")
!MESSAGE "UDP Basic Client - Win32 Alpha Debug" (based on "Win32 (ALPHA) Console Application")
!MESSAGE "UDP Basic Client - Win32 Alpha Release" (based on "Win32 (ALPHA) Console Application")
!MESSAGE "UDP Basic Client - Win32 Static Release" (based on "Win32 (x86) Console Application")
!MESSAGE "UDP Basic Client - Win32 Static Debug" (based on "Win32 (x86) Console Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""

!IF  "$(CFG)" == "UDP Basic Client - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir ".\client\Release"
# PROP BASE Intermediate_Dir ".\client\Release"
# PROP BASE Target_Dir ".\client"
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ".\client"
CPP=cl.exe
# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /YX /c
# ADD CPP /nologo /MD /W3 /GX /O2 /I "..\..\..\..\\" /I "..\..\..\..\..\\" /D "NDEBUG" /D "WIN32" /D "_CONSOLE" /FD /c
# SUBTRACT CPP /YX
RSC=rc.exe
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /machine:I386
# ADD LINK32 ace.lib TAO.lib TAO_PortableServer.lib TAO_Strategies.lib /nologo /subsystem:console /machine:I386 /libpath:"..\..\..\..\..\ace" /libpath:"..\..\..\..\tao" /libpath:"..\..\..\..\tao\PortableServer" /libpath:"..\..\..\..\tao\Strategies"

!ELSEIF  "$(CFG)" == "UDP Basic Client - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir ".\client\Debug"
# PROP BASE Intermediate_Dir ".\client\Debug"
# PROP BASE Target_Dir ".\client"
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir ""
# PROP Intermediate_Dir ".\Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ".\client"
CPP=cl.exe
# ADD BASE CPP /nologo /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /YX /c
# ADD CPP /nologo /MDd /W3 /Gm /GX /Zi /Od /I "..\..\..\..\\" /I "..\..\..\..\..\\" /D "_DEBUG" /D "WIN32" /D "_CONSOLE" /FD /c
# SUBTRACT CPP /YX
RSC=rc.exe
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /debug /machine:I386
# ADD LINK32 aced.lib TAOd.lib TAO_PortableServerd.lib TAO_Strategiesd.lib /nologo /subsystem:console /debug /machine:I386 /libpath:"..\..\..\..\..\ace" /libpath:"..\..\..\..\tao" /libpath:"..\..\..\..\tao\PortableServer" /libpath:"..\..\..\..\tao\Strategies"

!ELSEIF  "$(CFG)" == "UDP Basic Client - Win32 Alpha Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "client\Alpha Debug"
# PROP BASE Intermediate_Dir "client\Alpha Debug"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir "client"
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir ""
# PROP Intermediate_Dir "Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir "client"
CPP=cl.exe
# ADD BASE CPP /nologo /Gt0 /W3 /GX /Zi /Od /I "..\..\..\" /I "..\..\..\..\" /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /D "_MBCS" /FD /c
# SUBTRACT BASE CPP /YX
# ADD CPP /nologo /Gt0 /W3 /GX /Zi /Od /I "..\..\..\" /I "..\..\..\..\" /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /D "_MBCS" /FD /MDd /c
# SUBTRACT CPP /YX
RSC=rc.exe
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 aced.lib TAOd.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib /nologo /subsystem:console /debug /machine:ALPHA /libpath:"..\..\..\tao" /libpath:"..\..\..\..\ace"
# ADD LINK32 aced.lib TAOd.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib /nologo /subsystem:console /debug /machine:ALPHA /libpath:"..\..\..\..\tao" /libpath:"..\..\..\..\..\ace" /libpath:"..\..\..\..\tao\PortableServer"

!ELSEIF  "$(CFG)" == "UDP Basic Client - Win32 Alpha Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "client\Alpha Release"
# PROP BASE Intermediate_Dir "client\Alpha Release"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir "client"
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir "client"
CPP=cl.exe
# ADD BASE CPP /nologo /Gt0 /W3 /GX /O2 /I "..\..\..\" /I "..\..\..\..\" /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /D "_MBCS" /FD /c
# SUBTRACT BASE CPP /YX
# ADD CPP /nologo /MD /Gt0 /W3 /GX /O2 /I "..\..\..\" /I "..\..\..\..\" /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /D "_MBCS" /FD /c
# SUBTRACT CPP /YX
RSC=rc.exe
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 ace.lib TAO.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib /nologo /subsystem:console /machine:ALPHA /libpath:"..\..\..\tao" /libpath:"..\..\..\..\ace"
# ADD LINK32 ace.lib TAO.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib /nologo /subsystem:console /machine:ALPHA /libpath:"..\..\..\..\tao" /libpath:"..\..\..\..\..\ace" /libpath:"..\..\..\..\tao\PortableServer"
# Begin Custom Build
InputPath=.\Release\client.exe
InputName=client
SOURCE="$(InputPath)"

"d" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	..\..\..\..\..\..\bin\Release\tao_idl -Ge 1 $(InputName).idl

# End Custom Build

!ELSEIF  "$(CFG)" == "UDP Basic Client - Win32 Static Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "client\Static Release"
# PROP BASE Intermediate_Dir "client\Static Release"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir "client"
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "client\Static Release"
# PROP Intermediate_Dir "client\Static Release"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir "client"
CPP=cl.exe
# ADD BASE CPP /nologo /MD /W3 /GX /O2 /I "..\..\..\..\\" /I "..\..\..\..\..\\" /D "NDEBUG" /D "WIN32" /D "_CONSOLE" /FD /c
# SUBTRACT BASE CPP /YX
# ADD CPP /nologo /MD /W3 /GX /O2 /I "..\..\..\..\\" /I "..\..\..\..\..\\" /D "NDEBUG" /D "WIN32" /D "_CONSOLE" /D "ACE_AS_STATIC_LIBS" /D "TAO_AS_STATIC_LIBS" /FD /c
# SUBTRACT CPP /YX
RSC=rc.exe
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 ace.lib TAO.lib TAO_PortableServer.lib /nologo /subsystem:console /machine:I386 /libpath:"..\..\..\..\..\ace" /libpath:"..\..\..\..\tao" /libpath:"..\..\..\..\tao\PortableServer"
# ADD LINK32 TAOs.lib aces.lib TAO_PortableServers.lib TAO_Strategiess.lib advapi32.lib user32.lib /nologo /subsystem:console /machine:I386 /libpath:"..\..\..\..\..\ace" /libpath:"..\..\..\..\tao" /libpath:"..\..\..\..\tao\PortableServer" /libpath:"..\..\..\..\tao\Strategies"

!ELSEIF  "$(CFG)" == "UDP Basic Client - Win32 Static Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "client\Static Debug"
# PROP BASE Intermediate_Dir "client\Static Debug"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir "client"
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Static_Debug"
# PROP Intermediate_Dir "Static_Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir "client"
CPP=cl.exe
# ADD BASE CPP /nologo /MDd /W3 /Gm /GX /Zi /Od /I "..\..\..\..\\" /I "..\..\..\..\..\\" /D "_DEBUG" /D "WIN32" /D "_CONSOLE" /FD /c
# SUBTRACT BASE CPP /YX
# ADD CPP /nologo /MDd /W3 /Gm /GX /Zi /Od /I "..\..\..\..\\" /I "..\..\..\..\..\\" /D "_DEBUG" /D "WIN32" /D "_CONSOLE" /D "ACE_AS_STATIC_LIBS" /D "TAO_AS_STATIC_LIBS" /FD /c
# SUBTRACT CPP /YX
RSC=rc.exe
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 aced.lib TAOd.lib TAO_PortableServerd.lib /nologo /subsystem:console /debug /machine:I386 /libpath:"..\..\..\..\..\ace" /libpath:"..\..\..\..\tao" /libpath:"..\..\..\..\tao\PortableServer"
# ADD LINK32 TAOsd.lib acesd.lib TAO_PortableServersd.lib TAO_Strategiessd.lib advapi32.lib user32.lib /nologo /subsystem:console /debug /machine:I386 /libpath:"..\..\..\..\..\ace" /libpath:"..\..\..\..\tao" /libpath:"..\..\..\..\tao\PortableServer" /libpath:"..\..\..\..\tao\Strategies"

!ENDIF 

# Begin Target

# Name "UDP Basic Client - Win32 Release"
# Name "UDP Basic Client - Win32 Debug"
# Name "UDP Basic Client - Win32 Alpha Debug"
# Name "UDP Basic Client - Win32 Alpha Release"
# Name "UDP Basic Client - Win32 Static Release"
# Name "UDP Basic Client - Win32 Static Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;hpj;bat;for;f90"
# Begin Source File

SOURCE=.\client.cpp

!IF  "$(CFG)" == "UDP Basic Client - Win32 Release"

!ELSEIF  "$(CFG)" == "UDP Basic Client - Win32 Debug"

!ELSEIF  "$(CFG)" == "UDP Basic Client - Win32 Alpha Debug"

!ELSEIF  "$(CFG)" == "UDP Basic Client - Win32 Alpha Release"

!ELSEIF  "$(CFG)" == "UDP Basic Client - Win32 Static Release"

!ELSEIF  "$(CFG)" == "UDP Basic Client - Win32 Static Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\UDP_Client_i.cpp

!IF  "$(CFG)" == "UDP Basic Client - Win32 Release"

!ELSEIF  "$(CFG)" == "UDP Basic Client - Win32 Debug"

!ELSEIF  "$(CFG)" == "UDP Basic Client - Win32 Alpha Debug"

!ELSEIF  "$(CFG)" == "UDP Basic Client - Win32 Alpha Release"

!ELSEIF  "$(CFG)" == "UDP Basic Client - Win32 Static Release"

!ELSEIF  "$(CFG)" == "UDP Basic Client - Win32 Static Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\UDP_i.cpp

!IF  "$(CFG)" == "UDP Basic Client - Win32 Release"

!ELSEIF  "$(CFG)" == "UDP Basic Client - Win32 Debug"

!ELSEIF  "$(CFG)" == "UDP Basic Client - Win32 Alpha Debug"

!ELSEIF  "$(CFG)" == "UDP Basic Client - Win32 Alpha Release"

!ELSEIF  "$(CFG)" == "UDP Basic Client - Win32 Static Release"

!ELSEIF  "$(CFG)" == "UDP Basic Client - Win32 Static Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\UDPC.cpp

!IF  "$(CFG)" == "UDP Basic Client - Win32 Release"

# ADD CPP /GR

!ELSEIF  "$(CFG)" == "UDP Basic Client - Win32 Debug"

# ADD CPP /GR

!ELSEIF  "$(CFG)" == "UDP Basic Client - Win32 Alpha Debug"

!ELSEIF  "$(CFG)" == "UDP Basic Client - Win32 Alpha Release"

!ELSEIF  "$(CFG)" == "UDP Basic Client - Win32 Static Release"

# ADD CPP /GR

!ELSEIF  "$(CFG)" == "UDP Basic Client - Win32 Static Debug"

# ADD CPP /GR

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\UDPS.cpp

!IF  "$(CFG)" == "UDP Basic Client - Win32 Release"

!ELSEIF  "$(CFG)" == "UDP Basic Client - Win32 Debug"

!ELSEIF  "$(CFG)" == "UDP Basic Client - Win32 Alpha Debug"

!ELSEIF  "$(CFG)" == "UDP Basic Client - Win32 Alpha Release"

!ELSEIF  "$(CFG)" == "UDP Basic Client - Win32 Static Release"

!ELSEIF  "$(CFG)" == "UDP Basic Client - Win32 Static Debug"

!ENDIF 

# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl;fi;fd"
# Begin Source File

SOURCE=.\UDP_Client_i.h
# End Source File
# Begin Source File

SOURCE=.\UDP_i.h
# End Source File
# Begin Source File

SOURCE=.\UDPC.h
# End Source File
# End Group
# Begin Group "IDL Files"

# PROP Default_Filter ".idl"
# Begin Source File

SOURCE=.\UDP.idl

!IF  "$(CFG)" == "UDP Basic Client - Win32 Release"

# PROP Ignore_Default_Tool 1
USERDEP__UDP_I="..\..\..\..\..\bin\Release\tao_idl.exe"	
# Begin Custom Build - Invoking TAO_IDL Compiler on $(InputPath)
InputPath=.\UDP.idl
InputName=UDP

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

!ELSEIF  "$(CFG)" == "UDP Basic Client - Win32 Debug"

# PROP Ignore_Default_Tool 1
USERDEP__UDP_I="..\..\..\..\..\bin\tao_idl.exe"	
# Begin Custom Build - Invoking TAO_IDL Compiler on $(InputPath)
InputPath=.\UDP.idl
InputName=UDP

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

!ELSEIF  "$(CFG)" == "UDP Basic Client - Win32 Alpha Debug"

# PROP BASE Ignore_Default_Tool 1
# PROP Ignore_Default_Tool 1
USERDEP__UDP_I="..\..\..\..\..\bin\tao_idl.exe"	
# Begin Custom Build - Invoking TAO_IDL Compiler
InputPath=.\UDP.idl
InputName=UDP

BuildCmds= \
	..\..\..\..\..\bin\tao_idl -Ge 1 $(InputName).idl

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

!ELSEIF  "$(CFG)" == "UDP Basic Client - Win32 Alpha Release"

# PROP BASE Ignore_Default_Tool 1
# PROP Ignore_Default_Tool 1
USERDEP__UDP_I="..\..\..\..\..\bin\Release\tao_idl.exe"	
# Begin Custom Build - Invoking TAO_IDL Compiler
InputPath=.\UDP.idl
InputName=UDP

BuildCmds= \
	..\..\..\..\..\bin\Release\tao_idl -Ge 1 $(InputName).idl

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

!ELSEIF  "$(CFG)" == "UDP Basic Client - Win32 Static Release"

# PROP BASE Ignore_Default_Tool 1
# PROP Ignore_Default_Tool 1
USERDEP__UDP_I="..\..\..\..\..\bin\Release\tao_idl_static.exe"	
# Begin Custom Build - Invoking TAO_IDL Compiler on $(InputPath)
InputPath=.\UDP.idl
InputName=UDP

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

!ELSEIF  "$(CFG)" == "UDP Basic Client - Win32 Static Debug"

# PROP BASE Ignore_Default_Tool 1
# PROP Ignore_Default_Tool 1
USERDEP__UDP_I="..\..\..\..\..\bin\tao_idl_static.exe"	
# Begin Custom Build - Invoking TAO_IDL Compiler on $(InputPath)
InputPath=.\UDP.idl
InputName=UDP

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

!ENDIF 

# End Source File
# End Group
# End Target
# End Project
