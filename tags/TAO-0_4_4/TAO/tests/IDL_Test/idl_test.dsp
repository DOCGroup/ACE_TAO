# Microsoft Developer Studio Project File - Name="idl_test" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Console Application" 0x0103

CFG=idl_test - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "idl_test.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "idl_test.mak" CFG="idl_test - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "idl_test - Win32 Release" (based on "Win32 (x86) Console Application")
!MESSAGE "idl_test - Win32 Debug" (based on "Win32 (x86) Console Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "idl_test - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /D "_MBCS" /YX /FD /c
# ADD CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /D "_MBCS" /YX /FD /c
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /machine:I386
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /machine:I386

!ELSEIF  "$(CFG)" == "idl_test - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir ""
# PROP Intermediate_Dir ".\Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /D "_MBCS" /YX /FD  /c
# ADD CPP /nologo /MDd /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /D "_MBCS" /YX /FD  /c
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /debug /machine:I386 /pdbtype:sept
# ADD LINK32 aced.lib TAOd.lib /nologo /subsystem:console /debug /machine:I386 /out:"main.exe" /pdbtype:sept

!ENDIF 

# Begin Target

# Name "idl_test - Win32 Release"
# Name "idl_test - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\anonymousC.cpp
# End Source File
# Begin Source File

SOURCE=.\anonymousS.cpp
# End Source File
# Begin Source File

SOURCE=.\arrayC.cpp
# End Source File
# Begin Source File

SOURCE=.\arrayS.cpp
# End Source File
# Begin Source File

SOURCE=.\constantsC.cpp
# End Source File
# Begin Source File

SOURCE=.\constantsS.cpp
# End Source File
# Begin Source File

SOURCE=.\enum_in_structC.cpp
# End Source File
# Begin Source File

SOURCE=.\enum_in_structS.cpp
# End Source File
# Begin Source File

SOURCE=.\generic_objectC.cpp
# End Source File
# Begin Source File

SOURCE=.\generic_objectS.cpp
# End Source File
# Begin Source File

SOURCE=.\includedC.cpp
# End Source File
# Begin Source File

SOURCE=.\includedS.cpp
# End Source File
# Begin Source File

SOURCE=.\includingC.cpp
# End Source File
# Begin Source File

SOURCE=.\includingS.cpp
# End Source File
# Begin Source File

SOURCE=.\interfaceC.cpp
# End Source File
# Begin Source File

SOURCE=.\interfaceS.cpp
# End Source File
# Begin Source File

SOURCE=.\keywordsC.cpp
# End Source File
# Begin Source File

SOURCE=.\keywordsS.cpp
# End Source File
# Begin Source File

SOURCE=.\main.cpp
# End Source File
# Begin Source File

SOURCE=.\reopened_modulesC.cpp
# End Source File
# Begin Source File

SOURCE=.\reopened_modulesS.cpp
# End Source File
# Begin Source File

SOURCE=.\sequenceC.cpp
# End Source File
# Begin Source File

SOURCE=.\sequenceS.cpp
# End Source File
# Begin Source File

SOURCE=.\typedefC.cpp
# End Source File
# Begin Source File

SOURCE=.\typedefS.cpp
# End Source File
# Begin Source File

SOURCE=.\unionC.cpp
# End Source File
# Begin Source File

SOURCE=.\unionS.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\anonymousC.h
# End Source File
# Begin Source File

SOURCE=.\anonymousS.h
# End Source File
# Begin Source File

SOURCE=.\arrayC.h
# End Source File
# Begin Source File

SOURCE=.\arrayS.h
# End Source File
# Begin Source File

SOURCE=.\constantsC.h
# End Source File
# Begin Source File

SOURCE=.\constantsS.h
# End Source File
# Begin Source File

SOURCE=.\enum_in_structC.h
# End Source File
# Begin Source File

SOURCE=.\enum_in_structS.h
# End Source File
# Begin Source File

SOURCE=.\generic_objectC.h
# End Source File
# Begin Source File

SOURCE=.\generic_objectS.h
# End Source File
# Begin Source File

SOURCE=.\includedC.h
# End Source File
# Begin Source File

SOURCE=.\includedS.h
# End Source File
# Begin Source File

SOURCE=.\includingC.h
# End Source File
# Begin Source File

SOURCE=.\includingS.h
# End Source File
# Begin Source File

SOURCE=.\interfaceC.h
# End Source File
# Begin Source File

SOURCE=.\interfaceS.h
# End Source File
# Begin Source File

SOURCE=.\keywordsC.h
# End Source File
# Begin Source File

SOURCE=.\keywordsS.h
# End Source File
# Begin Source File

SOURCE=.\reopened_modulesC.h
# End Source File
# Begin Source File

SOURCE=.\reopened_modulesS.h
# End Source File
# Begin Source File

SOURCE=.\sequenceC.h
# End Source File
# Begin Source File

SOURCE=.\sequenceS.h
# End Source File
# Begin Source File

SOURCE=.\typedefC.h
# End Source File
# Begin Source File

SOURCE=.\typedefS.h
# End Source File
# Begin Source File

SOURCE=.\unionC.h
# End Source File
# Begin Source File

SOURCE=.\unionS.h
# End Source File
# End Group
# Begin Group "Inline Files"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\anonymousC.i
# End Source File
# Begin Source File

SOURCE=.\arrayC.i
# End Source File
# Begin Source File

SOURCE=.\constantsC.i
# End Source File
# Begin Source File

SOURCE=.\enum_in_structC.i
# End Source File
# Begin Source File

SOURCE=.\generic_objectC.i
# End Source File
# Begin Source File

SOURCE=.\includedC.i
# End Source File
# Begin Source File

SOURCE=.\includingC.i
# End Source File
# Begin Source File

SOURCE=.\interfaceC.i
# End Source File
# Begin Source File

SOURCE=.\keywordsC.i
# End Source File
# Begin Source File

SOURCE=.\reopened_modulesC.i
# End Source File
# Begin Source File

SOURCE=.\sequenceC.i
# End Source File
# Begin Source File

SOURCE=.\typedefC.i
# End Source File
# Begin Source File

SOURCE=.\unionC.i
# End Source File
# End Group
# Begin Group "IDL Files"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\anonymous.idl

!IF  "$(CFG)" == "idl_test - Win32 Release"

!ELSEIF  "$(CFG)" == "idl_test - Win32 Debug"

USERDEP__ANONY="..\..\..\bin\tao_idl.exe"	
# Begin Custom Build - Invoking TAO IDL Compiler
InputPath=.\anonymous.idl
InputName=anonymous

"$(InputName)C.h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	tao_idl $(InputName).idl

# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\array.idl

!IF  "$(CFG)" == "idl_test - Win32 Release"

!ELSEIF  "$(CFG)" == "idl_test - Win32 Debug"

USERDEP__ARRAY="..\..\..\bin\tao_idl.exe"	
# Begin Custom Build - Invoking TAO IDL Compiler
InputPath=.\array.idl
InputName=array

"$(InputName)C.h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	tao_idl $(InputName).idl

# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\constants.idl

!IF  "$(CFG)" == "idl_test - Win32 Release"

!ELSEIF  "$(CFG)" == "idl_test - Win32 Debug"

USERDEP__CONST="..\..\..\bin\tao_idl.exe"	
# Begin Custom Build - Invoking TAO IDL Compiler
InputPath=.\constants.idl
InputName=constants

"$(InputName)C.h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	tao_idl $(InputName).idl

# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\enum_in_struct.idl

!IF  "$(CFG)" == "idl_test - Win32 Release"

!ELSEIF  "$(CFG)" == "idl_test - Win32 Debug"

USERDEP__ENUM_="..\..\..\bin\tao_idl.exe"	
# Begin Custom Build - Invoking TAO IDL Compiler
InputPath=.\enum_in_struct.idl
InputName=enum_in_struct

"$(InputName)C.h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	tao_idl $(InputName).idl

# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\generic_object.idl

!IF  "$(CFG)" == "idl_test - Win32 Release"

!ELSEIF  "$(CFG)" == "idl_test - Win32 Debug"

USERDEP__GENER="..\..\..\bin\tao_idl.exe"	
# Begin Custom Build - Invoking TAO IDL Compiler
InputPath=.\generic_object.idl
InputName=generic_object

"$(InputName)C.h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	tao_idl $(InputName).idl

# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\included.idl

!IF  "$(CFG)" == "idl_test - Win32 Release"

!ELSEIF  "$(CFG)" == "idl_test - Win32 Debug"

USERDEP__INCLU="..\..\..\bin\tao_idl.exe"	
# Begin Custom Build - Invoking TAO IDL Compiler
InputPath=.\included.idl
InputName=included

"$(InputName)C.h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	tao_idl $(InputName).idl

# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\including.idl

!IF  "$(CFG)" == "idl_test - Win32 Release"

!ELSEIF  "$(CFG)" == "idl_test - Win32 Debug"

USERDEP__INCLUD="..\..\..\bin\tao_idl.exe"	
# Begin Custom Build - Invoking TAO IDL Compiler
InputPath=.\including.idl
InputName=including

"$(InputName)C.h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	tao_idl $(InputName).idl

# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\interface.idl

!IF  "$(CFG)" == "idl_test - Win32 Release"

!ELSEIF  "$(CFG)" == "idl_test - Win32 Debug"

USERDEP__INTER="..\..\..\bin\tao_idl.exe"	
# Begin Custom Build - Invoking TAO IDL Compiler
InputPath=.\interface.idl
InputName=interface

"$(InputName)C.h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	tao_idl $(InputName).idl

# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\keywords.idl

!IF  "$(CFG)" == "idl_test - Win32 Release"

!ELSEIF  "$(CFG)" == "idl_test - Win32 Debug"

USERDEP__KEYWO="..\..\..\bin\tao_idl.exe"	
# Begin Custom Build - Invoking TAO IDL Compiler
InputPath=.\keywords.idl
InputName=keywords

"$(InputName)C.h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	tao_idl $(InputName).idl

# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\reopened_modules.idl

!IF  "$(CFG)" == "idl_test - Win32 Release"

!ELSEIF  "$(CFG)" == "idl_test - Win32 Debug"

USERDEP__REOPE="..\..\..\bin\tao_idl.exe"	
# Begin Custom Build - Invoking TAO IDL Compiler
InputPath=.\reopened_modules.idl
InputName=reopened_modules

"$(InputName)C.h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	tao_idl $(InputName).idl

# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\sequence.idl

!IF  "$(CFG)" == "idl_test - Win32 Release"

!ELSEIF  "$(CFG)" == "idl_test - Win32 Debug"

USERDEP__SEQUE="..\..\..\bin\tao_idl.exe"	
# Begin Custom Build - Invoking TAO IDL Compiler
InputPath=.\sequence.idl
InputName=sequence

"$(InputName)C.h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	tao_idl $(InputName).idl

# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\typedef.idl

!IF  "$(CFG)" == "idl_test - Win32 Release"

!ELSEIF  "$(CFG)" == "idl_test - Win32 Debug"

USERDEP__TYPED="..\..\..\bin\tao_idl.exe"	
# Begin Custom Build - Invoking TAO IDL Compiler
InputPath=.\typedef.idl
InputName=typedef

"$(InputName)C.h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	tao_idl $(InputName).idl

# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\union.idl

!IF  "$(CFG)" == "idl_test - Win32 Release"

!ELSEIF  "$(CFG)" == "idl_test - Win32 Debug"

USERDEP__UNION="..\..\..\bin\tao_idl.exe"	
# Begin Custom Build - Invoking TAO IDL Compiler
InputPath=.\union.idl
InputName=union

"$(InputName)C.h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	tao_idl $(InputName).idl

# End Custom Build

!ENDIF 

# End Source File
# End Group
# End Target
# End Project
