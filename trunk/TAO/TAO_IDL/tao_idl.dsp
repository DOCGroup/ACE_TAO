# Microsoft Developer Studio Project File - Name="TAO_IDL Compiler" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Console Application" 0x0103

CFG=TAO_IDL Compiler - Win32 Release
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "tao_idl.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "tao_idl.mak" CFG="TAO_IDL Compiler - Win32 Release"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "TAO_IDL Compiler - Win32 Release" (based on "Win32 (x86) Console Application")
!MESSAGE "TAO_IDL Compiler - Win32 Debug" (based on "Win32 (x86) Console Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "TAO_IDL Compiler - Win32 Release"

# PROP Ignore_Export_Lib 0
# ADD CPP /MD /I "include" /I "be_include" /I "../../" /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /D "_MBCS" /D TAO_IDL_BE_HAS_DLL=1 /D TAO_IDL_FE_HAS_DLL=1
BSC32=bscmake.exe
LINK32=link.exe
# ADD BASE LINK32 /machine:IX86
# ADD LINK32 TAO_IDL_BE_DLL.lib TAO_IDL_FE_DLL.lib ace.lib /nologo /subsystem:console /debug /machine:IX86 /out:"..\..\bin\Release\tao_idl.exe" /libpath:"..\..\ace"
# SUBTRACT LINK32 /pdb:none

!ELSEIF  "$(CFG)" == "TAO_IDL Compiler - Win32 Debug"

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
# ADD BASE CPP /nologo /W3 /Gm /GX /Zi /Od /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /D "_MBCS" /D "_DEBUG" /YX /FD /c
# ADD CPP /nologo /MDd /W3 /Gm /GX /Zi /Od /I "include" /I "be_include" /I "../../" /D "WIN32" /D "_CONSOLE" /D "_MBCS" /D "_DEBUG" /YX /FD /c
# ADD BASE RSC /l 0x409 /d "NDEBUG" /d "_DEBUG"
# ADD RSC /l 0x409 /d "NDEBUG" /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /debug /machine:I386 /pdbtype:sept
# ADD LINK32 TAO_IDL_BE_DLLd.lib TAO_IDL_FE_DLLd.lib aced.lib /nologo /subsystem:console /debug /machine:I386 /out:"..\..\bin\tao_idl.exe" /pdbtype:sept /libpath:"..\..\ace"

!ENDIF 

# Begin Target

# Name "TAO_IDL Compiler - Win32 Release"
# Name "TAO_IDL Compiler - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\driver\drv_args.cpp
NODEP_CPP_DRV_A=\
	".\driver\ace\Process.h"\
	".\driver\be.h"\
	".\driver\drv_extern.h"\
	".\driver\drv_private.h"\
	".\driver\idl.h"\
	".\driver\idl_extern.h"\
	

!IF  "$(CFG)" == "TAO_IDL Compiler - Win32 Release"

!ELSEIF  "$(CFG)" == "TAO_IDL Compiler - Win32 Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\driver\drv_init.cpp
NODEP_CPP_DRV_I=\
	".\driver\be.h"\
	".\driver\drv_private.h"\
	".\driver\idl.h"\
	".\driver\idl_extern.h"\
	

!IF  "$(CFG)" == "TAO_IDL Compiler - Win32 Release"

!ELSEIF  "$(CFG)" == "TAO_IDL Compiler - Win32 Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\driver\drv_preproc.cpp
NODEP_CPP_DRV_P=\
	".\driver\ace\ARGV.h"\
	".\driver\ace\Env_Value_T.h"\
	".\driver\ace\Process_Manager.h"\
	".\driver\ace\SString.h"\
	".\driver\drv_extern.h"\
	".\driver\drv_private.h"\
	".\driver\idl.h"\
	".\driver\idl_extern.h"\
	

!IF  "$(CFG)" == "TAO_IDL Compiler - Win32 Release"

!ELSEIF  "$(CFG)" == "TAO_IDL Compiler - Win32 Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\driver\drv_private.cpp
NODEP_CPP_DRV_PR=\
	".\driver\drv_private.h"\
	".\driver\idl.h"\
	".\driver\idl_extern.h"\
	

!IF  "$(CFG)" == "TAO_IDL Compiler - Win32 Release"

!ELSEIF  "$(CFG)" == "TAO_IDL Compiler - Win32 Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\tao_idl.cpp
NODEP_CPP_TAO_I=\
	"..\Version.h"\
	".\ace\Process.h"\
	".\be.h"\
	".\drv_extern.h"\
	".\drv_private.h"\
	".\fe_extern.h"\
	".\global_extern.h"\
	".\idl.h"\
	

!IF  "$(CFG)" == "TAO_IDL Compiler - Win32 Release"

!ELSEIF  "$(CFG)" == "TAO_IDL Compiler - Win32 Debug"

!ENDIF 

# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\include\drv_extern.h
# End Source File
# Begin Source File

SOURCE=.\include\drv_private.h
# End Source File
# End Group
# End Target
# End Project
