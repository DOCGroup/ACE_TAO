# Microsoft Developer Studio Project File - Name="ACE static library" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (ALPHA) Static Library" 0x0604
# TARGTYPE "Win32 (x86) Static Library" 0x0104

CFG=ACE static library - Win32 Alpha Unicode Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "ace_lib.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "ace_lib.mak" CFG="ACE static library - Win32 Alpha Unicode Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "ACE static library - Win32 Debug" (based on "Win32 (x86) Static Library")
!MESSAGE "ACE static library - Win32 Release" (based on "Win32 (x86) Static Library")
!MESSAGE "ACE static library - Win32 Unicode Debug" (based on "Win32 (x86) Static Library")
!MESSAGE "ACE static library - Win32 Unicode Release" (based on "Win32 (x86) Static Library")
!MESSAGE "ACE static library - Win32 Alpha Debug" (based on "Win32 (ALPHA) Static Library")
!MESSAGE "ACE static library - Win32 Alpha Release" (based on "Win32 (ALPHA) Static Library")
!MESSAGE "ACE static library - Win32 Alpha Unicode Debug" (based on "Win32 (ALPHA) Static Library")
!MESSAGE "ACE static library - Win32 Alpha Unicode Release" (based on "Win32 (ALPHA) Static Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath "Desktop"

!IF  "$(CFG)" == "ACE static library - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir ""
# PROP Intermediate_Dir ".\LIB\Debug"
# PROP Target_Dir ""
CPP=cl.exe
# ADD BASE CPP /nologo /G5 /MTd /W3 /Gm /GX /Zi /Od /Gy /I "..\STL" /I "..\\" /D "_DEBUG" /D "WIN32" /D "_WINDOWS" /D ACE_HAS_DLL=0 /D "ACE_NO_INLINE" /YX /FD /c
# ADD CPP /nologo /MDd /W3 /Gm /GX /Zi /Od /Gy /I "..\\" /D "_DEBUG" /D "WIN32" /D "_WINDOWS" /D ACE_HAS_DLL=0 /FD /c
# SUBTRACT CPP /YX
RSC=rc.exe
# ADD BASE RSC /l 0x409
# ADD RSC /l 0x409
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo /o"ace.bsc"
# ADD BSC32 /nologo /o".\ace.bsc"
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo /out:".\libace.lib"
# ADD LIB32 /nologo /out:".\acesd.lib"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir ""
# PROP Intermediate_Dir ".\LIB\Release"
# PROP Target_Dir ""
CPP=cl.exe
# ADD BASE CPP /nologo /G5 /MT /W3 /GX /O1 /I "..\STL" /I "..\\" /D "NDEBUG" /D "WIN32" /D "_WINDOWS" /D ACE_HAS_DLL=0 /D "ACE_NO_INLINE" /YX /FD /c
# ADD CPP /nologo /MD /W3 /GX /O1 /I "..\\" /D ACE_HAS_DLL=0 /D "ACE_NO_INLINE" /D "NDEBUG" /D "WIN32" /D "_WINDOWS" /FD /c
# SUBTRACT CPP /YX
RSC=rc.exe
# ADD BASE RSC /l 0x409
# ADD RSC /l 0x409
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo /o"ace.bsc"
# ADD BSC32 /nologo /o".\ace.bsc"
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo /out:".\libace-r.lib"
# ADD LIB32 /nologo /out:".\aces.lib"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Unicode Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Unicode Debug"
# PROP BASE Intermediate_Dir "Unicode Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir ""
# PROP Intermediate_Dir ".\LIB\Unicode_Debug"
# PROP Target_Dir ""
CPP=cl.exe
# ADD BASE CPP /nologo /G5 /MTd /W3 /Gm /GX /Zi /Od /Gy /I "..\STL" /I "..\\" /D "_DEBUG" /D "WIN32" /D "_WINDOWS" /D ACE_HAS_DLL=0 /D "ACE_NO_INLINE" /YX /FD /c
# ADD CPP /nologo /MDd /W3 /Gm /GX /Zi /Od /Gy /I "..\\" /D ACE_HAS_DLL=0 /D "ACE_NO_INLINE" /D "_DEBUG" /D "UNICODE" /D "WIN32" /D "_WINDOWS" /FD /c
# SUBTRACT CPP /YX
RSC=rc.exe
# ADD BASE RSC /l 0x409
# ADD RSC /l 0x409
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo /o"ace.bsc"
# ADD BSC32 /nologo /o".\ace.bsc"
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo /out:".\libace.lib"
# ADD LIB32 /nologo /out:".\acesud.lib"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Unicode Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Unicode Release"
# PROP BASE Intermediate_Dir "Unicode Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir ""
# PROP Intermediate_Dir ".\LIB\Unicode_Release"
# PROP Target_Dir ""
CPP=cl.exe
# ADD BASE CPP /nologo /G5 /MT /W3 /GX /O1 /I "..\STL" /I "..\\" /D "NDEBUG" /D "WIN32" /D "_WINDOWS" /D ACE_HAS_DLL=0 /D "ACE_NO_INLINE" /YX /FD /c
# ADD CPP /nologo /MD /W3 /GX /O1 /I "..\\" /D ACE_HAS_DLL=0 /D "ACE_NO_INLINE" /D "NDEBUG" /D "UNICODE" /D "WIN32" /D "_WINDOWS" /FD /c
# SUBTRACT CPP /YX
RSC=rc.exe
# ADD BASE RSC /l 0x409
# ADD RSC /l 0x409
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo /o"ace.bsc"
# ADD BSC32 /nologo /o".\ace.bsc"
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo /out:".\libace-r.lib"
# ADD LIB32 /nologo /out:".\acesu.lib"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Alpha Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "ACE_stat"
# PROP BASE Intermediate_Dir "ACE_stat"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir ""
# PROP Intermediate_Dir "Lib\Debug"
# PROP Target_Dir ""
CPP=cl.exe
# ADD BASE CPP /Gt0 nologo Gt0 W3 GX Zi Od Gy D "_DEBUG" D "_WINDOWS" FD I "..\\ /D " WIN32" c
# ADD CPP /nologo /MTd /Gt0 /W3 /GX /Zi /Od /Gy /I "..\\" /D "_DEBUG" /D "_WINDOWS" /D "WIN32" /D ACE_HAS_DLL=0 /FD /c
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo /o".\ace.bsc"
# ADD BSC32 /nologo /o".\ace.bsc"
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo /out:".\acesd.lib"
# ADD LIB32 /nologo /out:".\acesd.lib"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Alpha Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "ACE_stat"
# PROP BASE Intermediate_Dir "ACE_stat"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir ""
# PROP Intermediate_Dir "Lib\Release"
# PROP Target_Dir ""
CPP=cl.exe
# ADD BASE CPP /Gt0 nologo Gt0 W3 GX O1 D "NDEBUG" D "_WINDOWS" FD I "..\\ /D " WIN32" c
# ADD CPP /nologo /MT /Gt0 /W3 /GX /O1 /I "..\\" /D "NDEBUG" /D "_WINDOWS" /D "WIN32" /D ACE_HAS_DLL=0 /D "ACE_NO_INLINE" /FD /c
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo /o".\ace.bsc"
# ADD BSC32 /nologo /o".\ace.bsc"
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo /out:".\aces.lib"
# ADD LIB32 /nologo /out:".\aces.lib"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Alpha Unicode Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "ACE_sta0"
# PROP BASE Intermediate_Dir "ACE_sta0"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir ""
# PROP Intermediate_Dir "LIB\Unicode_Debug"
# PROP Target_Dir ""
CPP=cl.exe
# ADD BASE CPP /Gt0 nologo Gt0 W3 GX Zi Od Gy D "_DEBUG" D "_WINDOWS" D "UNICODE" FD I "..\\ /D " WIN32" c
# ADD CPP /nologo /MTd /Gt0 /W3 /GX /Zi /Od /Gy /I "..\\" /D "_DEBUG" /D "_WINDOWS" /D "UNICODE" /D "WIN32" /D ACE_HAS_DLL=0 /FD /c
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo /o".\ace.bsc"
# ADD BSC32 /nologo /o".\ace.bsc"
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo /out:".\acesud.lib"
# ADD LIB32 /nologo /out:".\acesud.lib"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Alpha Unicode Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "ACE_sta1"
# PROP BASE Intermediate_Dir "ACE_sta1"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir ""
# PROP Intermediate_Dir "LIB\Unicode_Release"
# PROP Target_Dir ""
CPP=cl.exe
# ADD BASE CPP /Gt0 nologo Gt0 W3 GX O1 D "NDEBUG" D "_WINDOWS" D "UNICODE" FD I "..\\ /D " WIN32" c
# ADD CPP /nologo /MT /Gt0 /W3 /GX /O1 /I "..\\" /D "NDEBUG" /D "_WINDOWS" /D "UNICODE" /D "WIN32" /D ACE_HAS_DLL=0 /D "ACE_NO_INLINE" /FD /c
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo /o".\ace.bsc"
# ADD BSC32 /nologo /o".\ace.bsc"
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo /out:".\acesu.lib"
# ADD LIB32 /nologo /out:".\acesu.lib"

!ENDIF 

# Begin Target

# Name "ACE static library - Win32 Debug"
# Name "ACE static library - Win32 Release"
# Name "ACE static library - Win32 Unicode Debug"
# Name "ACE static library - Win32 Unicode Release"
# Name "ACE static library - Win32 Alpha Debug"
# Name "ACE static library - Win32 Alpha Release"
# Name "ACE static library - Win32 Alpha Unicode Debug"
# Name "ACE static library - Win32 Alpha Unicode Release"
# Begin Group "Source Files"

# PROP Default_Filter "cpp"
# Begin Source File

SOURCE=.\ACE.cpp

!IF  "$(CFG)" == "ACE static library - Win32 Debug"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Release"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Unicode Debug"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Unicode Release"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Alpha Debug"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Alpha Release"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Alpha Unicode Debug"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Alpha Unicode Release"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Activation_Queue.cpp

!IF  "$(CFG)" == "ACE static library - Win32 Debug"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Release"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Unicode Debug"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Unicode Release"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Alpha Debug"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Alpha Release"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Alpha Unicode Debug"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Alpha Unicode Release"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Active_Map_Manager.cpp

!IF  "$(CFG)" == "ACE static library - Win32 Debug"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Release"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Unicode Debug"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Unicode Release"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Alpha Debug"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Alpha Release"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Alpha Unicode Debug"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Alpha Unicode Release"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Addr.cpp

!IF  "$(CFG)" == "ACE static library - Win32 Debug"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Release"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Unicode Debug"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Unicode Release"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Alpha Debug"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Alpha Release"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Alpha Unicode Debug"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Alpha Unicode Release"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Arg_Shifter.cpp

!IF  "$(CFG)" == "ACE static library - Win32 Debug"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Release"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Unicode Debug"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Unicode Release"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Alpha Debug"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Alpha Release"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Alpha Unicode Debug"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Alpha Unicode Release"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\ARGV.cpp

!IF  "$(CFG)" == "ACE static library - Win32 Debug"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Release"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Unicode Debug"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Unicode Release"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Alpha Debug"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Alpha Release"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Alpha Unicode Debug"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Alpha Unicode Release"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Asynch_IO.cpp

!IF  "$(CFG)" == "ACE static library - Win32 Debug"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Release"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Unicode Debug"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Unicode Release"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Alpha Debug"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Alpha Release"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Alpha Unicode Debug"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Alpha Unicode Release"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Asynch_IO_Impl.cpp

!IF  "$(CFG)" == "ACE static library - Win32 Debug"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Release"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Unicode Debug"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Unicode Release"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Alpha Debug"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Alpha Release"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Alpha Unicode Debug"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Alpha Unicode Release"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Basic_Types.cpp

!IF  "$(CFG)" == "ACE static library - Win32 Debug"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Release"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Unicode Debug"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Unicode Release"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Alpha Debug"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Alpha Release"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Alpha Unicode Debug"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Alpha Unicode Release"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\CDR_Stream.cpp

!IF  "$(CFG)" == "ACE static library - Win32 Debug"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Release"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Unicode Debug"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Unicode Release"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Alpha Debug"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Alpha Release"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Alpha Unicode Debug"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Alpha Unicode Release"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Containers.cpp

!IF  "$(CFG)" == "ACE static library - Win32 Debug"

# PROP BASE Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Release"

# PROP BASE Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Unicode Debug"

# PROP BASE Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Unicode Release"

# PROP BASE Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Alpha Debug"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Alpha Release"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Alpha Unicode Debug"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Alpha Unicode Release"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\CORBA_Handler.cpp

!IF  "$(CFG)" == "ACE static library - Win32 Debug"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Release"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Unicode Debug"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Unicode Release"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Alpha Debug"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Alpha Release"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Alpha Unicode Debug"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Alpha Unicode Release"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\CORBA_Ref.cpp

!IF  "$(CFG)" == "ACE static library - Win32 Debug"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Release"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Unicode Debug"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Unicode Release"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Alpha Debug"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Alpha Release"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Alpha Unicode Debug"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Alpha Unicode Release"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Date_Time.cpp

!IF  "$(CFG)" == "ACE static library - Win32 Debug"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Release"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Unicode Debug"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Unicode Release"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Alpha Debug"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Alpha Release"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Alpha Unicode Debug"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Alpha Unicode Release"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\DEV.cpp

!IF  "$(CFG)" == "ACE static library - Win32 Debug"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Release"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Unicode Debug"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Unicode Release"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Alpha Debug"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Alpha Release"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Alpha Unicode Debug"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Alpha Unicode Release"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\DEV_Addr.cpp

!IF  "$(CFG)" == "ACE static library - Win32 Debug"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Release"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Unicode Debug"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Unicode Release"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Alpha Debug"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Alpha Release"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Alpha Unicode Debug"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Alpha Unicode Release"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\DEV_Connector.cpp

!IF  "$(CFG)" == "ACE static library - Win32 Debug"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Release"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Unicode Debug"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Unicode Release"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Alpha Debug"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Alpha Release"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Alpha Unicode Debug"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Alpha Unicode Release"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\DEV_IO.cpp

!IF  "$(CFG)" == "ACE static library - Win32 Debug"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Release"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Unicode Debug"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Unicode Release"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Alpha Debug"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Alpha Release"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Alpha Unicode Debug"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Alpha Unicode Release"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Dirent.cpp

!IF  "$(CFG)" == "ACE static library - Win32 Debug"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Release"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Unicode Debug"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Unicode Release"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Alpha Debug"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Alpha Release"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Alpha Unicode Debug"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Alpha Unicode Release"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\DLL.cpp

!IF  "$(CFG)" == "ACE static library - Win32 Debug"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Release"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Unicode Debug"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Unicode Release"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Alpha Debug"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Alpha Release"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Alpha Unicode Debug"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Alpha Unicode Release"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Dump.cpp

!IF  "$(CFG)" == "ACE static library - Win32 Debug"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Release"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Unicode Debug"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Unicode Release"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Alpha Debug"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Alpha Release"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Alpha Unicode Debug"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Alpha Unicode Release"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Dynamic.cpp

!IF  "$(CFG)" == "ACE static library - Win32 Debug"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Release"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Unicode Debug"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Unicode Release"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Alpha Debug"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Alpha Release"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Alpha Unicode Debug"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Alpha Unicode Release"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Dynamic_Service.cpp

!IF  "$(CFG)" == "ACE static library - Win32 Debug"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Release"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Unicode Debug"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Unicode Release"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Alpha Debug"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Alpha Release"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Alpha Unicode Debug"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Alpha Unicode Release"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Event_Handler.cpp

!IF  "$(CFG)" == "ACE static library - Win32 Debug"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Release"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Unicode Debug"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Unicode Release"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Alpha Debug"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Alpha Release"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Alpha Unicode Debug"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Alpha Unicode Release"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Event_Handler_T.cpp

!IF  "$(CFG)" == "ACE static library - Win32 Debug"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Release"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Unicode Debug"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Unicode Release"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Alpha Debug"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Alpha Release"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Alpha Unicode Debug"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Alpha Unicode Release"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\FIFO.cpp

!IF  "$(CFG)" == "ACE static library - Win32 Debug"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Release"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Unicode Debug"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Unicode Release"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Alpha Debug"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Alpha Release"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Alpha Unicode Debug"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Alpha Unicode Release"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\FIFO_Recv.cpp

!IF  "$(CFG)" == "ACE static library - Win32 Debug"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Release"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Unicode Debug"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Unicode Release"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Alpha Debug"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Alpha Release"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Alpha Unicode Debug"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Alpha Unicode Release"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\FIFO_Recv_Msg.cpp

!IF  "$(CFG)" == "ACE static library - Win32 Debug"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Release"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Unicode Debug"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Unicode Release"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Alpha Debug"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Alpha Release"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Alpha Unicode Debug"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Alpha Unicode Release"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\FIFO_Send.cpp

!IF  "$(CFG)" == "ACE static library - Win32 Debug"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Release"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Unicode Debug"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Unicode Release"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Alpha Debug"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Alpha Release"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Alpha Unicode Debug"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Alpha Unicode Release"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\FIFO_Send_Msg.cpp

!IF  "$(CFG)" == "ACE static library - Win32 Debug"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Release"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Unicode Debug"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Unicode Release"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Alpha Debug"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Alpha Release"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Alpha Unicode Debug"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Alpha Unicode Release"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\FILE.cpp

!IF  "$(CFG)" == "ACE static library - Win32 Debug"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Release"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Unicode Debug"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Unicode Release"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Alpha Debug"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Alpha Release"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Alpha Unicode Debug"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Alpha Unicode Release"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\FILE_Addr.cpp

!IF  "$(CFG)" == "ACE static library - Win32 Debug"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Release"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Unicode Debug"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Unicode Release"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Alpha Debug"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Alpha Release"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Alpha Unicode Debug"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Alpha Unicode Release"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\FILE_Connector.cpp

!IF  "$(CFG)" == "ACE static library - Win32 Debug"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Release"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Unicode Debug"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Unicode Release"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Alpha Debug"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Alpha Release"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Alpha Unicode Debug"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Alpha Unicode Release"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\FILE_IO.cpp

!IF  "$(CFG)" == "ACE static library - Win32 Debug"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Release"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Unicode Debug"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Unicode Release"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Alpha Debug"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Alpha Release"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Alpha Unicode Debug"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Alpha Unicode Release"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Filecache.cpp

!IF  "$(CFG)" == "ACE static library - Win32 Debug"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Release"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Unicode Debug"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Unicode Release"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Alpha Debug"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Alpha Release"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Alpha Unicode Debug"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Alpha Unicode Release"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Functor.cpp

!IF  "$(CFG)" == "ACE static library - Win32 Debug"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Release"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Unicode Debug"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Unicode Release"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Alpha Debug"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Alpha Release"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Alpha Unicode Debug"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Alpha Unicode Release"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Future.cpp

!IF  "$(CFG)" == "ACE static library - Win32 Debug"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Release"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Unicode Debug"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Unicode Release"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Alpha Debug"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Alpha Release"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Alpha Unicode Debug"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Alpha Unicode Release"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Get_Opt.cpp

!IF  "$(CFG)" == "ACE static library - Win32 Debug"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Release"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Unicode Debug"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Unicode Release"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Alpha Debug"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Alpha Release"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Alpha Unicode Debug"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Alpha Unicode Release"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Handle_Set.cpp

!IF  "$(CFG)" == "ACE static library - Win32 Debug"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Release"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Unicode Debug"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Unicode Release"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Alpha Debug"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Alpha Release"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Alpha Unicode Debug"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Alpha Unicode Release"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Hash_Map_Manager.cpp

!IF  "$(CFG)" == "ACE static library - Win32 Debug"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Release"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Unicode Debug"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Unicode Release"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Alpha Debug"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Alpha Release"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Alpha Unicode Debug"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Alpha Unicode Release"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\High_Res_Timer.cpp

!IF  "$(CFG)" == "ACE static library - Win32 Debug"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Release"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Unicode Debug"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Unicode Release"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Alpha Debug"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Alpha Release"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Alpha Unicode Debug"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Alpha Unicode Release"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\INET_Addr.cpp

!IF  "$(CFG)" == "ACE static library - Win32 Debug"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Release"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Unicode Debug"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Unicode Release"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Alpha Debug"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Alpha Release"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Alpha Unicode Debug"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Alpha Unicode Release"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\IO_Cntl_Msg.cpp

!IF  "$(CFG)" == "ACE static library - Win32 Debug"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Release"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Unicode Debug"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Unicode Release"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Alpha Debug"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Alpha Release"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Alpha Unicode Debug"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Alpha Unicode Release"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\IO_SAP.cpp

!IF  "$(CFG)" == "ACE static library - Win32 Debug"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Release"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Unicode Debug"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Unicode Release"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Alpha Debug"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Alpha Release"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Alpha Unicode Debug"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Alpha Unicode Release"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\IOStream.cpp

!IF  "$(CFG)" == "ACE static library - Win32 Debug"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Release"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Unicode Debug"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Unicode Release"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Alpha Debug"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Alpha Release"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Alpha Unicode Debug"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Alpha Unicode Release"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\IPC_SAP.cpp

!IF  "$(CFG)" == "ACE static library - Win32 Debug"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Release"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Unicode Debug"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Unicode Release"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Alpha Debug"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Alpha Release"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Alpha Unicode Debug"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Alpha Unicode Release"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Local_Name_Space.cpp

!IF  "$(CFG)" == "ACE static library - Win32 Debug"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Release"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Unicode Debug"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Unicode Release"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Alpha Debug"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Alpha Release"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Alpha Unicode Debug"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Alpha Unicode Release"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Local_Tokens.cpp

!IF  "$(CFG)" == "ACE static library - Win32 Debug"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Release"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Unicode Debug"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Unicode Release"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Alpha Debug"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Alpha Release"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Alpha Unicode Debug"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Alpha Unicode Release"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Log_Msg.cpp

!IF  "$(CFG)" == "ACE static library - Win32 Debug"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Release"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Unicode Debug"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Unicode Release"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Alpha Debug"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Alpha Release"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Alpha Unicode Debug"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Alpha Unicode Release"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Log_Record.cpp

!IF  "$(CFG)" == "ACE static library - Win32 Debug"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Release"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Unicode Debug"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Unicode Release"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Alpha Debug"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Alpha Release"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Alpha Unicode Debug"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Alpha Unicode Release"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\LSOCK.cpp

!IF  "$(CFG)" == "ACE static library - Win32 Debug"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Release"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Unicode Debug"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Unicode Release"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Alpha Debug"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Alpha Release"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Alpha Unicode Debug"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Alpha Unicode Release"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\LSOCK_Acceptor.cpp

!IF  "$(CFG)" == "ACE static library - Win32 Debug"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Release"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Unicode Debug"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Unicode Release"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Alpha Debug"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Alpha Release"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Alpha Unicode Debug"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Alpha Unicode Release"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\LSOCK_CODgram.cpp

!IF  "$(CFG)" == "ACE static library - Win32 Debug"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Release"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Unicode Debug"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Unicode Release"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Alpha Debug"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Alpha Release"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Alpha Unicode Debug"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Alpha Unicode Release"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\LSOCK_Connector.cpp

!IF  "$(CFG)" == "ACE static library - Win32 Debug"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Release"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Unicode Debug"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Unicode Release"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Alpha Debug"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Alpha Release"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Alpha Unicode Debug"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Alpha Unicode Release"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\LSOCK_Dgram.cpp

!IF  "$(CFG)" == "ACE static library - Win32 Debug"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Release"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Unicode Debug"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Unicode Release"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Alpha Debug"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Alpha Release"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Alpha Unicode Debug"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Alpha Unicode Release"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\LSOCK_Stream.cpp

!IF  "$(CFG)" == "ACE static library - Win32 Debug"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Release"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Unicode Debug"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Unicode Release"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Alpha Debug"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Alpha Release"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Alpha Unicode Debug"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Alpha Unicode Release"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Malloc.cpp

!IF  "$(CFG)" == "ACE static library - Win32 Debug"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Release"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Unicode Debug"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Unicode Release"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Alpha Debug"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Alpha Release"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Alpha Unicode Debug"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Alpha Unicode Release"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Map.cpp

!IF  "$(CFG)" == "ACE static library - Win32 Debug"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Release"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Unicode Debug"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Unicode Release"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Alpha Debug"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Alpha Release"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Alpha Unicode Debug"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Alpha Unicode Release"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Mem_Map.cpp

!IF  "$(CFG)" == "ACE static library - Win32 Debug"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Release"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Unicode Debug"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Unicode Release"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Alpha Debug"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Alpha Release"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Alpha Unicode Debug"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Alpha Unicode Release"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Memory_Pool.cpp

!IF  "$(CFG)" == "ACE static library - Win32 Debug"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Release"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Unicode Debug"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Unicode Release"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Alpha Debug"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Alpha Release"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Alpha Unicode Debug"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Alpha Unicode Release"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Message_Block.cpp

!IF  "$(CFG)" == "ACE static library - Win32 Debug"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Release"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Unicode Debug"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Unicode Release"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Alpha Debug"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Alpha Release"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Alpha Unicode Debug"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Alpha Unicode Release"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Message_Queue.cpp

!IF  "$(CFG)" == "ACE static library - Win32 Debug"

# PROP BASE Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Release"

# PROP BASE Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Unicode Debug"

# PROP BASE Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Unicode Release"

# PROP BASE Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Alpha Debug"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Alpha Release"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Alpha Unicode Debug"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Alpha Unicode Release"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Method_Request.cpp

!IF  "$(CFG)" == "ACE static library - Win32 Debug"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Release"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Unicode Debug"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Unicode Release"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Alpha Debug"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Alpha Release"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Alpha Unicode Debug"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Alpha Unicode Release"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Msg_WFMO_Reactor.cpp

!IF  "$(CFG)" == "ACE static library - Win32 Debug"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Release"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Unicode Debug"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Unicode Release"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Alpha Debug"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Alpha Release"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Alpha Unicode Debug"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Alpha Unicode Release"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Multiplexor.cpp

!IF  "$(CFG)" == "ACE static library - Win32 Debug"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Release"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Unicode Debug"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Unicode Release"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Alpha Debug"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Alpha Release"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Alpha Unicode Debug"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Alpha Unicode Release"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Name_Proxy.cpp

!IF  "$(CFG)" == "ACE static library - Win32 Debug"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Release"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Unicode Debug"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Unicode Release"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Alpha Debug"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Alpha Release"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Alpha Unicode Debug"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Alpha Unicode Release"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Name_Request_Reply.cpp

!IF  "$(CFG)" == "ACE static library - Win32 Debug"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Release"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Unicode Debug"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Unicode Release"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Alpha Debug"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Alpha Release"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Alpha Unicode Debug"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Alpha Unicode Release"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Name_Space.cpp

!IF  "$(CFG)" == "ACE static library - Win32 Debug"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Release"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Unicode Debug"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Unicode Release"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Alpha Debug"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Alpha Release"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Alpha Unicode Debug"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Alpha Unicode Release"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Naming_Context.cpp

!IF  "$(CFG)" == "ACE static library - Win32 Debug"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Release"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Unicode Debug"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Unicode Release"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Alpha Debug"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Alpha Release"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Alpha Unicode Debug"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Alpha Unicode Release"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\NT_Service.cpp

!IF  "$(CFG)" == "ACE static library - Win32 Debug"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Release"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Unicode Debug"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Unicode Release"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Alpha Debug"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Alpha Release"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Alpha Unicode Debug"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Alpha Unicode Release"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Object_Manager.cpp

!IF  "$(CFG)" == "ACE static library - Win32 Debug"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Release"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Unicode Debug"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Unicode Release"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Alpha Debug"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Alpha Release"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Alpha Unicode Debug"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Alpha Unicode Release"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Obstack.cpp

!IF  "$(CFG)" == "ACE static library - Win32 Debug"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Release"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Unicode Debug"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Unicode Release"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Alpha Debug"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Alpha Release"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Alpha Unicode Debug"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Alpha Unicode Release"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\OS.cpp

!IF  "$(CFG)" == "ACE static library - Win32 Debug"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Release"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Unicode Debug"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Unicode Release"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Alpha Debug"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Alpha Release"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Alpha Unicode Debug"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Alpha Unicode Release"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Pair.cpp

!IF  "$(CFG)" == "ACE static library - Win32 Debug"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Release"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Unicode Debug"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Unicode Release"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Alpha Debug"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Alpha Release"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Alpha Unicode Debug"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Alpha Unicode Release"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Parse_Node.cpp

!IF  "$(CFG)" == "ACE static library - Win32 Debug"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Release"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Unicode Debug"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Unicode Release"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Alpha Debug"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Alpha Release"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Alpha Unicode Debug"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Alpha Unicode Release"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Pipe.cpp

!IF  "$(CFG)" == "ACE static library - Win32 Debug"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Release"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Unicode Debug"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Unicode Release"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Alpha Debug"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Alpha Release"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Alpha Unicode Debug"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Alpha Unicode Release"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Priority_Reactor.cpp

!IF  "$(CFG)" == "ACE static library - Win32 Debug"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Release"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Unicode Debug"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Unicode Release"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Alpha Debug"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Alpha Release"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Alpha Unicode Debug"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Alpha Unicode Release"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Proactor.cpp

!IF  "$(CFG)" == "ACE static library - Win32 Debug"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Release"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Unicode Debug"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Unicode Release"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Alpha Debug"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Alpha Release"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Alpha Unicode Debug"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Alpha Unicode Release"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Process.cpp

!IF  "$(CFG)" == "ACE static library - Win32 Debug"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Release"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Unicode Debug"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Unicode Release"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Alpha Debug"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Alpha Release"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Alpha Unicode Debug"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Alpha Unicode Release"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Process_Manager.cpp

!IF  "$(CFG)" == "ACE static library - Win32 Debug"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Release"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Unicode Debug"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Unicode Release"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Alpha Debug"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Alpha Release"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Alpha Unicode Debug"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Alpha Unicode Release"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Profile_Timer.cpp

!IF  "$(CFG)" == "ACE static library - Win32 Debug"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Release"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Unicode Debug"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Unicode Release"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Alpha Debug"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Alpha Release"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Alpha Unicode Debug"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Alpha Unicode Release"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Reactor.cpp

!IF  "$(CFG)" == "ACE static library - Win32 Debug"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Release"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Unicode Debug"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Unicode Release"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Alpha Debug"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Alpha Release"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Alpha Unicode Debug"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Alpha Unicode Release"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Read_Buffer.cpp

!IF  "$(CFG)" == "ACE static library - Win32 Debug"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Release"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Unicode Debug"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Unicode Release"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Alpha Debug"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Alpha Release"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Alpha Unicode Debug"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Alpha Unicode Release"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Registry.cpp

!IF  "$(CFG)" == "ACE static library - Win32 Debug"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Release"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Unicode Debug"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Unicode Release"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Alpha Debug"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Alpha Release"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Alpha Unicode Debug"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Alpha Unicode Release"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Registry_Name_Space.cpp

!IF  "$(CFG)" == "ACE static library - Win32 Debug"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Release"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Unicode Debug"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Unicode Release"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Alpha Debug"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Alpha Release"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Alpha Unicode Debug"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Alpha Unicode Release"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Remote_Name_Space.cpp

!IF  "$(CFG)" == "ACE static library - Win32 Debug"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Release"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Unicode Debug"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Unicode Release"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Alpha Debug"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Alpha Release"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Alpha Unicode Debug"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Alpha Unicode Release"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Remote_Tokens.cpp

!IF  "$(CFG)" == "ACE static library - Win32 Debug"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Release"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Unicode Debug"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Unicode Release"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Alpha Debug"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Alpha Release"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Alpha Unicode Debug"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Alpha Unicode Release"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Sched_Params.cpp

!IF  "$(CFG)" == "ACE static library - Win32 Debug"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Release"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Unicode Debug"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Unicode Release"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Alpha Debug"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Alpha Release"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Alpha Unicode Debug"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Alpha Unicode Release"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Select_Reactor.cpp

!IF  "$(CFG)" == "ACE static library - Win32 Debug"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Release"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Unicode Debug"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Unicode Release"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Alpha Debug"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Alpha Release"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Alpha Unicode Debug"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Alpha Unicode Release"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Select_Reactor_Base.cpp

!IF  "$(CFG)" == "ACE static library - Win32 Debug"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Release"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Unicode Debug"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Unicode Release"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Alpha Debug"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Alpha Release"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Alpha Unicode Debug"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Alpha Unicode Release"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Service_Config.cpp

!IF  "$(CFG)" == "ACE static library - Win32 Debug"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Release"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Unicode Debug"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Unicode Release"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Alpha Debug"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Alpha Release"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Alpha Unicode Debug"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Alpha Unicode Release"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Service_Manager.cpp

!IF  "$(CFG)" == "ACE static library - Win32 Debug"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Release"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Unicode Debug"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Unicode Release"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Alpha Debug"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Alpha Release"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Alpha Unicode Debug"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Alpha Unicode Release"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Service_Object.cpp

!IF  "$(CFG)" == "ACE static library - Win32 Debug"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Release"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Unicode Debug"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Unicode Release"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Alpha Debug"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Alpha Release"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Alpha Unicode Debug"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Alpha Unicode Release"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Service_Repository.cpp

!IF  "$(CFG)" == "ACE static library - Win32 Debug"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Release"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Unicode Debug"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Unicode Release"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Alpha Debug"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Alpha Release"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Alpha Unicode Debug"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Alpha Unicode Release"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Service_Types.cpp

!IF  "$(CFG)" == "ACE static library - Win32 Debug"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Release"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Unicode Debug"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Unicode Release"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Alpha Debug"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Alpha Release"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Alpha Unicode Debug"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Alpha Unicode Release"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Shared_Memory.cpp

!IF  "$(CFG)" == "ACE static library - Win32 Debug"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Release"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Unicode Debug"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Unicode Release"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Alpha Debug"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Alpha Release"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Alpha Unicode Debug"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Alpha Unicode Release"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Shared_Memory_MM.cpp

!IF  "$(CFG)" == "ACE static library - Win32 Debug"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Release"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Unicode Debug"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Unicode Release"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Alpha Debug"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Alpha Release"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Alpha Unicode Debug"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Alpha Unicode Release"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Shared_Memory_SV.cpp

!IF  "$(CFG)" == "ACE static library - Win32 Debug"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Release"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Unicode Debug"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Unicode Release"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Alpha Debug"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Alpha Release"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Alpha Unicode Debug"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Alpha Unicode Release"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Shared_Object.cpp

!IF  "$(CFG)" == "ACE static library - Win32 Debug"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Release"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Unicode Debug"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Unicode Release"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Alpha Debug"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Alpha Release"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Alpha Unicode Debug"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Alpha Unicode Release"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Signal.cpp

!IF  "$(CFG)" == "ACE static library - Win32 Debug"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Release"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Unicode Debug"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Unicode Release"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Alpha Debug"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Alpha Release"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Alpha Unicode Debug"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Alpha Unicode Release"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\SOCK.cpp

!IF  "$(CFG)" == "ACE static library - Win32 Debug"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Release"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Unicode Debug"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Unicode Release"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Alpha Debug"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Alpha Release"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Alpha Unicode Debug"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Alpha Unicode Release"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\SOCK_Acceptor.cpp

!IF  "$(CFG)" == "ACE static library - Win32 Debug"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Release"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Unicode Debug"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Unicode Release"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Alpha Debug"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Alpha Release"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Alpha Unicode Debug"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Alpha Unicode Release"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\SOCK_CODgram.cpp

!IF  "$(CFG)" == "ACE static library - Win32 Debug"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Release"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Unicode Debug"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Unicode Release"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Alpha Debug"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Alpha Release"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Alpha Unicode Debug"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Alpha Unicode Release"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\SOCK_Connector.cpp

!IF  "$(CFG)" == "ACE static library - Win32 Debug"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Release"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Unicode Debug"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Unicode Release"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Alpha Debug"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Alpha Release"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Alpha Unicode Debug"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Alpha Unicode Release"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\SOCK_Dgram.cpp

!IF  "$(CFG)" == "ACE static library - Win32 Debug"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Release"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Unicode Debug"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Unicode Release"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Alpha Debug"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Alpha Release"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Alpha Unicode Debug"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Alpha Unicode Release"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\SOCK_Dgram_Bcast.cpp

!IF  "$(CFG)" == "ACE static library - Win32 Debug"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Release"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Unicode Debug"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Unicode Release"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Alpha Debug"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Alpha Release"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Alpha Unicode Debug"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Alpha Unicode Release"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\SOCK_Dgram_Mcast.cpp

!IF  "$(CFG)" == "ACE static library - Win32 Debug"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Release"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Unicode Debug"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Unicode Release"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Alpha Debug"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Alpha Release"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Alpha Unicode Debug"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Alpha Unicode Release"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\SOCK_IO.cpp

!IF  "$(CFG)" == "ACE static library - Win32 Debug"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Release"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Unicode Debug"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Unicode Release"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Alpha Debug"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Alpha Release"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Alpha Unicode Debug"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Alpha Unicode Release"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\SOCK_Stream.cpp

!IF  "$(CFG)" == "ACE static library - Win32 Debug"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Release"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Unicode Debug"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Unicode Release"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Alpha Debug"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Alpha Release"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Alpha Unicode Debug"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Alpha Unicode Release"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\SPIPE.cpp

!IF  "$(CFG)" == "ACE static library - Win32 Debug"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Release"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Unicode Debug"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Unicode Release"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Alpha Debug"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Alpha Release"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Alpha Unicode Debug"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Alpha Unicode Release"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\SPIPE_Acceptor.cpp

!IF  "$(CFG)" == "ACE static library - Win32 Debug"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Release"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Unicode Debug"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Unicode Release"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Alpha Debug"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Alpha Release"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Alpha Unicode Debug"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Alpha Unicode Release"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\SPIPE_Addr.cpp

!IF  "$(CFG)" == "ACE static library - Win32 Debug"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Release"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Unicode Debug"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Unicode Release"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Alpha Debug"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Alpha Release"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Alpha Unicode Debug"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Alpha Unicode Release"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\SPIPE_Connector.cpp

!IF  "$(CFG)" == "ACE static library - Win32 Debug"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Release"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Unicode Debug"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Unicode Release"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Alpha Debug"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Alpha Release"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Alpha Unicode Debug"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Alpha Unicode Release"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\SPIPE_Stream.cpp

!IF  "$(CFG)" == "ACE static library - Win32 Debug"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Release"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Unicode Debug"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Unicode Release"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Alpha Debug"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Alpha Release"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Alpha Unicode Debug"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Alpha Unicode Release"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\SString.cpp

!IF  "$(CFG)" == "ACE static library - Win32 Debug"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Release"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Unicode Debug"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Unicode Release"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Alpha Debug"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Alpha Release"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Alpha Unicode Debug"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Alpha Unicode Release"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Stats.cpp

!IF  "$(CFG)" == "ACE static library - Win32 Debug"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Release"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Unicode Debug"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Unicode Release"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Alpha Debug"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Alpha Release"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Alpha Unicode Debug"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Alpha Unicode Release"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Strategies.cpp

!IF  "$(CFG)" == "ACE static library - Win32 Debug"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Release"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Unicode Debug"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Unicode Release"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Alpha Debug"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Alpha Release"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Alpha Unicode Debug"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Alpha Unicode Release"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\SV_Message.cpp

!IF  "$(CFG)" == "ACE static library - Win32 Debug"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Release"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Unicode Debug"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Unicode Release"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Alpha Debug"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Alpha Release"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Alpha Unicode Debug"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Alpha Unicode Release"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\SV_Message_Queue.cpp

!IF  "$(CFG)" == "ACE static library - Win32 Debug"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Release"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Unicode Debug"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Unicode Release"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Alpha Debug"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Alpha Release"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Alpha Unicode Debug"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Alpha Unicode Release"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\SV_Semaphore_Complex.cpp

!IF  "$(CFG)" == "ACE static library - Win32 Debug"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Release"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Unicode Debug"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Unicode Release"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Alpha Debug"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Alpha Release"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Alpha Unicode Debug"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Alpha Unicode Release"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\SV_Semaphore_Simple.cpp

!IF  "$(CFG)" == "ACE static library - Win32 Debug"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Release"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Unicode Debug"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Unicode Release"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Alpha Debug"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Alpha Release"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Alpha Unicode Debug"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Alpha Unicode Release"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\SV_Shared_Memory.cpp

!IF  "$(CFG)" == "ACE static library - Win32 Debug"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Release"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Unicode Debug"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Unicode Release"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Alpha Debug"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Alpha Release"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Alpha Unicode Debug"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Alpha Unicode Release"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Svc_Conf_l.cpp

!IF  "$(CFG)" == "ACE static library - Win32 Debug"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Release"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Unicode Debug"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Unicode Release"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Alpha Debug"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Alpha Release"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Alpha Unicode Debug"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Alpha Unicode Release"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Svc_Conf_y.cpp

!IF  "$(CFG)" == "ACE static library - Win32 Debug"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Release"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Unicode Debug"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Unicode Release"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Alpha Debug"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Alpha Release"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Alpha Unicode Debug"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Alpha Unicode Release"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Svc_Handler.cpp

!IF  "$(CFG)" == "ACE static library - Win32 Debug"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Release"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Unicode Debug"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Unicode Release"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Alpha Debug"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Alpha Release"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Alpha Unicode Debug"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Alpha Unicode Release"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Synch.cpp

!IF  "$(CFG)" == "ACE static library - Win32 Debug"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Release"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Unicode Debug"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Unicode Release"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Alpha Debug"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Alpha Release"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Alpha Unicode Debug"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Alpha Unicode Release"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Synch_Options.cpp

!IF  "$(CFG)" == "ACE static library - Win32 Debug"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Release"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Unicode Debug"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Unicode Release"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Alpha Debug"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Alpha Release"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Alpha Unicode Debug"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Alpha Unicode Release"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\System_Time.cpp

!IF  "$(CFG)" == "ACE static library - Win32 Debug"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Release"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Unicode Debug"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Unicode Release"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Alpha Debug"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Alpha Release"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Alpha Unicode Debug"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Alpha Unicode Release"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Task.cpp

!IF  "$(CFG)" == "ACE static library - Win32 Debug"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Release"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Unicode Debug"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Unicode Release"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Alpha Debug"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Alpha Release"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Alpha Unicode Debug"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Alpha Unicode Release"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Thread.cpp

!IF  "$(CFG)" == "ACE static library - Win32 Debug"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Release"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Unicode Debug"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Unicode Release"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Alpha Debug"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Alpha Release"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Alpha Unicode Debug"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Alpha Unicode Release"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Thread_Manager.cpp

!IF  "$(CFG)" == "ACE static library - Win32 Debug"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Release"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Unicode Debug"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Unicode Release"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Alpha Debug"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Alpha Release"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Alpha Unicode Debug"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Alpha Unicode Release"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Time_Request_Reply.cpp

!IF  "$(CFG)" == "ACE static library - Win32 Debug"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Release"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Unicode Debug"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Unicode Release"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Alpha Debug"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Alpha Release"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Alpha Unicode Debug"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Alpha Unicode Release"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Timeprobe.cpp

!IF  "$(CFG)" == "ACE static library - Win32 Debug"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Release"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Unicode Debug"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Unicode Release"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Alpha Debug"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Alpha Release"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Alpha Unicode Debug"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Alpha Unicode Release"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Timer_Hash.cpp

!IF  "$(CFG)" == "ACE static library - Win32 Debug"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Release"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Unicode Debug"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Unicode Release"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Alpha Debug"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Alpha Release"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Alpha Unicode Debug"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Alpha Unicode Release"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Timer_Heap.cpp

!IF  "$(CFG)" == "ACE static library - Win32 Debug"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Release"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Unicode Debug"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Unicode Release"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Alpha Debug"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Alpha Release"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Alpha Unicode Debug"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Alpha Unicode Release"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Timer_List.cpp

!IF  "$(CFG)" == "ACE static library - Win32 Debug"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Release"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Unicode Debug"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Unicode Release"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Alpha Debug"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Alpha Release"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Alpha Unicode Debug"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Alpha Unicode Release"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Timer_Queue.cpp

!IF  "$(CFG)" == "ACE static library - Win32 Debug"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Release"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Unicode Debug"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Unicode Release"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Alpha Debug"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Alpha Release"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Alpha Unicode Debug"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Alpha Unicode Release"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Timer_Queue_Adapters.cpp

!IF  "$(CFG)" == "ACE static library - Win32 Debug"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Release"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Unicode Debug"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Unicode Release"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Alpha Debug"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Alpha Release"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Alpha Unicode Debug"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Alpha Unicode Release"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Timer_Wheel.cpp

!IF  "$(CFG)" == "ACE static library - Win32 Debug"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Release"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Unicode Debug"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Unicode Release"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Alpha Debug"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Alpha Release"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Alpha Unicode Debug"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Alpha Unicode Release"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\TLI.cpp

!IF  "$(CFG)" == "ACE static library - Win32 Debug"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Release"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Unicode Debug"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Unicode Release"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Alpha Debug"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Alpha Release"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Alpha Unicode Debug"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Alpha Unicode Release"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\TLI_Acceptor.cpp

!IF  "$(CFG)" == "ACE static library - Win32 Debug"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Release"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Unicode Debug"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Unicode Release"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Alpha Debug"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Alpha Release"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Alpha Unicode Debug"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Alpha Unicode Release"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\TLI_Connector.cpp

!IF  "$(CFG)" == "ACE static library - Win32 Debug"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Release"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Unicode Debug"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Unicode Release"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Alpha Debug"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Alpha Release"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Alpha Unicode Debug"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Alpha Unicode Release"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\TLI_Stream.cpp

!IF  "$(CFG)" == "ACE static library - Win32 Debug"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Release"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Unicode Debug"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Unicode Release"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Alpha Debug"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Alpha Release"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Alpha Unicode Debug"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Alpha Unicode Release"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Token.cpp

!IF  "$(CFG)" == "ACE static library - Win32 Debug"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Release"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Unicode Debug"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Unicode Release"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Alpha Debug"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Alpha Release"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Alpha Unicode Debug"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Alpha Unicode Release"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Token_Collection.cpp

!IF  "$(CFG)" == "ACE static library - Win32 Debug"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Release"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Unicode Debug"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Unicode Release"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Alpha Debug"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Alpha Release"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Alpha Unicode Debug"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Alpha Unicode Release"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Token_Invariants.cpp

!IF  "$(CFG)" == "ACE static library - Win32 Debug"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Release"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Unicode Debug"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Unicode Release"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Alpha Debug"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Alpha Release"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Alpha Unicode Debug"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Alpha Unicode Release"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Token_Manager.cpp

!IF  "$(CFG)" == "ACE static library - Win32 Debug"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Release"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Unicode Debug"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Unicode Release"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Alpha Debug"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Alpha Release"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Alpha Unicode Debug"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Alpha Unicode Release"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Token_Request_Reply.cpp

!IF  "$(CFG)" == "ACE static library - Win32 Debug"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Release"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Unicode Debug"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Unicode Release"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Alpha Debug"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Alpha Release"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Alpha Unicode Debug"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Alpha Unicode Release"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\TP_Reactor.cpp

!IF  "$(CFG)" == "ACE static library - Win32 Debug"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Release"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Unicode Debug"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Unicode Release"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Alpha Debug"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Alpha Release"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Alpha Unicode Debug"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Alpha Unicode Release"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Trace.cpp

!IF  "$(CFG)" == "ACE static library - Win32 Debug"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Release"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Unicode Debug"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Unicode Release"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Alpha Debug"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Alpha Release"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Alpha Unicode Debug"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Alpha Unicode Release"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\TTY_IO.cpp

!IF  "$(CFG)" == "ACE static library - Win32 Debug"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Release"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Unicode Debug"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Unicode Release"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Alpha Debug"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Alpha Release"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Alpha Unicode Debug"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Alpha Unicode Release"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Typed_SV_Message.cpp

!IF  "$(CFG)" == "ACE static library - Win32 Debug"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Release"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Unicode Debug"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Unicode Release"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Alpha Debug"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Alpha Release"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Alpha Unicode Debug"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Alpha Unicode Release"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Typed_SV_Message_Queue.cpp

!IF  "$(CFG)" == "ACE static library - Win32 Debug"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Release"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Unicode Debug"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Unicode Release"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Alpha Debug"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Alpha Release"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Alpha Unicode Debug"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Alpha Unicode Release"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\UNIX_Addr.cpp

!IF  "$(CFG)" == "ACE static library - Win32 Debug"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Release"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Unicode Debug"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Unicode Release"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Alpha Debug"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Alpha Release"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Alpha Unicode Debug"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Alpha Unicode Release"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\UPIPE_Acceptor.cpp

!IF  "$(CFG)" == "ACE static library - Win32 Debug"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Release"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Unicode Debug"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Unicode Release"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Alpha Debug"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Alpha Release"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Alpha Unicode Debug"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Alpha Unicode Release"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\UPIPE_Connector.cpp

!IF  "$(CFG)" == "ACE static library - Win32 Debug"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Release"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Unicode Debug"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Unicode Release"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Alpha Debug"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Alpha Release"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Alpha Unicode Debug"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Alpha Unicode Release"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\UPIPE_Stream.cpp

!IF  "$(CFG)" == "ACE static library - Win32 Debug"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Release"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Unicode Debug"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Unicode Release"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Alpha Debug"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Alpha Release"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Alpha Unicode Debug"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Alpha Unicode Release"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\WFMO_Reactor.cpp

!IF  "$(CFG)" == "ACE static library - Win32 Debug"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Release"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Unicode Debug"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Unicode Release"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Alpha Debug"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Alpha Release"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Alpha Unicode Debug"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Alpha Unicode Release"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\WIN32_Asynch_IO.cpp

!IF  "$(CFG)" == "ACE static library - Win32 Debug"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Release"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Unicode Debug"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Unicode Release"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Alpha Debug"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Alpha Release"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Alpha Unicode Debug"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Alpha Unicode Release"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\WIN32_Proactor.cpp

!IF  "$(CFG)" == "ACE static library - Win32 Debug"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Release"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Unicode Debug"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Unicode Release"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Alpha Debug"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Alpha Release"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Alpha Unicode Debug"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Alpha Unicode Release"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\XtReactor.cpp

!IF  "$(CFG)" == "ACE static library - Win32 Debug"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Release"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Unicode Debug"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Unicode Release"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Alpha Debug"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Alpha Release"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Alpha Unicode Debug"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Alpha Unicode Release"

!ENDIF 

# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h"
# Begin Source File

SOURCE=.\Acceptor.h
# End Source File
# Begin Source File

SOURCE=.\ACE.h
# End Source File
# Begin Source File

SOURCE=.\Activation_Queue.h
# End Source File
# Begin Source File

SOURCE=.\Active_Map_Manager.h
# End Source File
# Begin Source File

SOURCE=.\Active_Map_Manager_T.h
# End Source File
# Begin Source File

SOURCE=.\Addr.h
# End Source File
# Begin Source File

SOURCE=.\ARGV.h
# End Source File
# Begin Source File

SOURCE=.\Array.h
# End Source File
# Begin Source File

SOURCE=.\Asynch_Acceptor.h
# End Source File
# Begin Source File

SOURCE=.\Asynch_IO.h
# End Source File
# Begin Source File

SOURCE=.\Asynch_IO_Impl.h
# End Source File
# Begin Source File

SOURCE=.\Auto_Ptr.h
# End Source File
# Begin Source File

SOURCE=.\Basic_Types.h
# End Source File
# Begin Source File

SOURCE=".\config-win32-common.h"
# End Source File
# Begin Source File

SOURCE=.\config.h
# End Source File
# Begin Source File

SOURCE=.\Connector.h
# End Source File
# Begin Source File

SOURCE=.\Containers.h
# End Source File
# Begin Source File

SOURCE=.\CORBA_Handler.h
# End Source File
# Begin Source File

SOURCE=.\CORBA_Ref.h
# End Source File
# Begin Source File

SOURCE=.\Date_Time.h
# End Source File
# Begin Source File

SOURCE=.\DEV.h
# End Source File
# Begin Source File

SOURCE=.\DEV_Addr.h
# End Source File
# Begin Source File

SOURCE=.\DEV_Connector.h
# End Source File
# Begin Source File

SOURCE=.\DEV_IO.h
# End Source File
# Begin Source File

SOURCE=.\Dirent.h
# End Source File
# Begin Source File

SOURCE=.\DLL.h
# End Source File
# Begin Source File

SOURCE=.\Dump.h
# End Source File
# Begin Source File

SOURCE=.\Dump_T.h
# End Source File
# Begin Source File

SOURCE=.\Dynamic.h
# End Source File
# Begin Source File

SOURCE=.\Dynamic_Service.h
# End Source File
# Begin Source File

SOURCE=.\Event_Handler.h
# End Source File
# Begin Source File

SOURCE=.\Event_Handler_T.h
# End Source File
# Begin Source File

SOURCE=.\FIFO.h
# End Source File
# Begin Source File

SOURCE=.\FIFO_Recv.h
# End Source File
# Begin Source File

SOURCE=.\FIFO_Recv_Msg.h
# End Source File
# Begin Source File

SOURCE=.\FIFO_Send.h
# End Source File
# Begin Source File

SOURCE=.\FIFO_Send_Msg.h
# End Source File
# Begin Source File

SOURCE=.\FILE.h
# End Source File
# Begin Source File

SOURCE=.\FILE_Addr.h
# End Source File
# Begin Source File

SOURCE=.\FILE_Connector.h
# End Source File
# Begin Source File

SOURCE=.\FILE_IO.h
# End Source File
# Begin Source File

SOURCE=.\Filecache.h
# End Source File
# Begin Source File

SOURCE=.\Free_List.h
# End Source File
# Begin Source File

SOURCE=.\Future.h
# End Source File
# Begin Source File

SOURCE=.\Get_Opt.h
# End Source File
# Begin Source File

SOURCE=.\Handle_Set.h
# End Source File
# Begin Source File

SOURCE=.\Hash_Map_Manager.h
# End Source File
# Begin Source File

SOURCE=.\Hash_Map_Manager_T.h
# End Source File
# Begin Source File

SOURCE=.\High_Res_Timer.h
# End Source File
# Begin Source File

SOURCE=.\INET_Addr.h
# End Source File
# Begin Source File

SOURCE=.\IO_Cntl_Msg.h
# End Source File
# Begin Source File

SOURCE=.\IO_SAP.h
# End Source File
# Begin Source File

SOURCE=.\IOStream.h
# End Source File
# Begin Source File

SOURCE=.\IOStream_T.h
# End Source File
# Begin Source File

SOURCE=.\IPC_SAP.h
# End Source File
# Begin Source File

SOURCE=.\Local_Name_Space.h
# End Source File
# Begin Source File

SOURCE=.\Local_Name_Space_T.h
# End Source File
# Begin Source File

SOURCE=.\Local_Tokens.h
# End Source File
# Begin Source File

SOURCE=.\Log_Msg.h
# End Source File
# Begin Source File

SOURCE=.\Log_Priority.h
# End Source File
# Begin Source File

SOURCE=.\Log_Record.h
# End Source File
# Begin Source File

SOURCE=.\LSOCK.h
# End Source File
# Begin Source File

SOURCE=.\LSOCK_Acceptor.h
# End Source File
# Begin Source File

SOURCE=.\LSOCK_CODgram.h
# End Source File
# Begin Source File

SOURCE=.\LSOCK_Connector.h
# End Source File
# Begin Source File

SOURCE=.\LSOCK_Dgram.h
# End Source File
# Begin Source File

SOURCE=.\LSOCK_Stream.h
# End Source File
# Begin Source File

SOURCE=.\Malloc.h
# End Source File
# Begin Source File

SOURCE=.\Malloc_T.h
# End Source File
# Begin Source File

SOURCE=.\Map.h
# End Source File
# Begin Source File

SOURCE=.\Map_Manager.h
# End Source File
# Begin Source File

SOURCE=.\Map_T.h
# End Source File
# Begin Source File

SOURCE=.\Mem_Map.h
# End Source File
# Begin Source File

SOURCE=.\Memory_Pool.h
# End Source File
# Begin Source File

SOURCE=.\Message_Block.h
# End Source File
# Begin Source File

SOURCE=.\Message_Queue.h
# End Source File
# Begin Source File

SOURCE=.\Method_Object.h
# End Source File
# Begin Source File

SOURCE=.\Module.h
# End Source File
# Begin Source File

SOURCE=.\Multiplexor.h
# End Source File
# Begin Source File

SOURCE=.\Name_Proxy.h
# End Source File
# Begin Source File

SOURCE=.\Name_Request_Reply.h
# End Source File
# Begin Source File

SOURCE=.\Name_Space.h
# End Source File
# Begin Source File

SOURCE=.\Naming_Context.h
# End Source File
# Begin Source File

SOURCE=.\NT_Service.h
# End Source File
# Begin Source File

SOURCE=.\Object_Manager.h
# End Source File
# Begin Source File

SOURCE=.\Obstack.h
# End Source File
# Begin Source File

SOURCE=.\OS.h
# End Source File
# Begin Source File

SOURCE=.\Pair.h
# End Source File
# Begin Source File

SOURCE=.\Pair_T.h
# End Source File
# Begin Source File

SOURCE=.\Parse_Node.h
# End Source File
# Begin Source File

SOURCE=.\Pipe.h
# End Source File
# Begin Source File

SOURCE=.\Priority_Reactor.h
# End Source File
# Begin Source File

SOURCE=.\Proactor.h
# End Source File
# Begin Source File

SOURCE=.\Proactor_Impl.h
# End Source File
# Begin Source File

SOURCE=.\Process.h
# End Source File
# Begin Source File

SOURCE=.\Process_Manager.h
# End Source File
# Begin Source File

SOURCE=.\Profile_Timer.h
# End Source File
# Begin Source File

SOURCE=.\Reactor.h
# End Source File
# Begin Source File

SOURCE=.\ReactorEx.h
# End Source File
# Begin Source File

SOURCE=.\Read_Buffer.h
# End Source File
# Begin Source File

SOURCE=.\Registry.h
# End Source File
# Begin Source File

SOURCE=.\Registry_Name_Space.h
# End Source File
# Begin Source File

SOURCE=.\Remote_Name_Space.h
# End Source File
# Begin Source File

SOURCE=.\Remote_Tokens.h
# End Source File
# Begin Source File

SOURCE=.\Sched_Params.h
# End Source File
# Begin Source File

SOURCE=.\Select_Reactor_Base.h
# End Source File
# Begin Source File

SOURCE=.\Select_Reactor_T.h
# End Source File
# Begin Source File

SOURCE=.\Service_Config.h
# End Source File
# Begin Source File

SOURCE=.\Service_Manager.h
# End Source File
# Begin Source File

SOURCE=.\Service_Object.h
# End Source File
# Begin Source File

SOURCE=.\Service_Record.h
# End Source File
# Begin Source File

SOURCE=.\Service_Repository.h
# End Source File
# Begin Source File

SOURCE=.\Shared_Memory.h
# End Source File
# Begin Source File

SOURCE=.\Shared_Memory_MM.h
# End Source File
# Begin Source File

SOURCE=.\Shared_Memory_SV.h
# End Source File
# Begin Source File

SOURCE=.\Shared_Object.h
# End Source File
# Begin Source File

SOURCE=.\Signal.h
# End Source File
# Begin Source File

SOURCE=.\Singleton.h
# End Source File
# Begin Source File

SOURCE=.\SOCK.h
# End Source File
# Begin Source File

SOURCE=.\SOCK_Acceptor.h
# End Source File
# Begin Source File

SOURCE=.\SOCK_CODgram.h
# End Source File
# Begin Source File

SOURCE=.\SOCK_Connector.h
# End Source File
# Begin Source File

SOURCE=.\SOCK_Dgram.h
# End Source File
# Begin Source File

SOURCE=.\SOCK_Dgram_Bcast.h
# End Source File
# Begin Source File

SOURCE=.\SOCK_Dgram_Mcast.h
# End Source File
# Begin Source File

SOURCE=.\SOCK_IO.h
# End Source File
# Begin Source File

SOURCE=.\SOCK_Stream.h
# End Source File
# Begin Source File

SOURCE=.\SPIPE.h
# End Source File
# Begin Source File

SOURCE=.\SPIPE_Acceptor.h
# End Source File
# Begin Source File

SOURCE=.\SPIPE_Addr.h
# End Source File
# Begin Source File

SOURCE=.\SPIPE_Connector.h
# End Source File
# Begin Source File

SOURCE=.\SPIPE_Stream.h
# End Source File
# Begin Source File

SOURCE=.\SString.h
# End Source File
# Begin Source File

SOURCE=.\stdcpp.h
# End Source File
# Begin Source File

SOURCE=.\Strategies.h
# End Source File
# Begin Source File

SOURCE=.\Strategies_T.h
# End Source File
# Begin Source File

SOURCE=.\Stream.h
# End Source File
# Begin Source File

SOURCE=.\Stream_Modules.h
# End Source File
# Begin Source File

SOURCE=.\SV_Message.h
# End Source File
# Begin Source File

SOURCE=.\SV_Message_Queue.h
# End Source File
# Begin Source File

SOURCE=.\SV_Semaphore_Complex.h
# End Source File
# Begin Source File

SOURCE=.\SV_Semaphore_Simple.h
# End Source File
# Begin Source File

SOURCE=.\SV_Shared_Memory.h
# End Source File
# Begin Source File

SOURCE=.\Svc_Conf.h
# End Source File
# Begin Source File

SOURCE=.\Svc_Conf_Tokens.h
# End Source File
# Begin Source File

SOURCE=.\Svc_Handler.h
# End Source File
# Begin Source File

SOURCE=.\Synch.h
# End Source File
# Begin Source File

SOURCE=.\Synch_Options.h
# End Source File
# Begin Source File

SOURCE=.\Synch_T.h
# End Source File
# Begin Source File

SOURCE=.\System_Time.h
# End Source File
# Begin Source File

SOURCE=.\Task.h
# End Source File
# Begin Source File

SOURCE=.\Task_T.h
# End Source File
# Begin Source File

SOURCE=.\Thread.h
# End Source File
# Begin Source File

SOURCE=.\Thread_Manager.h
# End Source File
# Begin Source File

SOURCE=.\Time_Request_Reply.h
# End Source File
# Begin Source File

SOURCE=.\Time_Value.h
# End Source File
# Begin Source File

SOURCE=.\Timer_Hash.h
# End Source File
# Begin Source File

SOURCE=.\Timer_Hash_T.h
# End Source File
# Begin Source File

SOURCE=.\Timer_Heap.h
# End Source File
# Begin Source File

SOURCE=.\Timer_Heap_T.h
# End Source File
# Begin Source File

SOURCE=.\Timer_List.h
# End Source File
# Begin Source File

SOURCE=.\Timer_List_T.h
# End Source File
# Begin Source File

SOURCE=.\Timer_Queue.h
# End Source File
# Begin Source File

SOURCE=.\Timer_Queue_T.h
# End Source File
# Begin Source File

SOURCE=.\Timer_Wheel.h
# End Source File
# Begin Source File

SOURCE=.\Timer_Wheel_T.h
# End Source File
# Begin Source File

SOURCE=.\TLI.h
# End Source File
# Begin Source File

SOURCE=.\TLI_Acceptor.h
# End Source File
# Begin Source File

SOURCE=.\TLI_Connector.h
# End Source File
# Begin Source File

SOURCE=.\TLI_Stream.h
# End Source File
# Begin Source File

SOURCE=.\Token.h
# End Source File
# Begin Source File

SOURCE=.\Token_Collection.h
# End Source File
# Begin Source File

SOURCE=.\Token_Invariants.h
# End Source File
# Begin Source File

SOURCE=.\Token_Manager.h
# End Source File
# Begin Source File

SOURCE=.\Token_Request_Reply.h
# End Source File
# Begin Source File

SOURCE=.\Trace.h
# End Source File
# Begin Source File

SOURCE=.\TTY_IO.h
# End Source File
# Begin Source File

SOURCE=.\Typed_SV_Message.h
# End Source File
# Begin Source File

SOURCE=.\UNIX_Addr.h
# End Source File
# Begin Source File

SOURCE=.\UPIPE_Acceptor.h
# End Source File
# Begin Source File

SOURCE=.\UPIPE_Addr.h
# End Source File
# Begin Source File

SOURCE=.\UPIPE_Connector.h
# End Source File
# Begin Source File

SOURCE=.\UPIPE_Stream.h
# End Source File
# Begin Source File

SOURCE=.\WIN32_Asynch_IO.h
# End Source File
# Begin Source File

SOURCE=.\WIN32_Proactor.h
# End Source File
# Begin Source File

SOURCE=.\ws2tcpip.h
# End Source File
# Begin Source File

SOURCE=.\XtReactor.h
# End Source File
# End Group
# Begin Group "Inline Files"

# PROP Default_Filter "i"
# Begin Source File

SOURCE=.\Acceptor.i
# End Source File
# Begin Source File

SOURCE=.\ACE.i
# End Source File
# Begin Source File

SOURCE=.\Active_Map_Manager.i
# End Source File
# Begin Source File

SOURCE=.\Active_Map_Manager_T.i
# End Source File
# Begin Source File

SOURCE=.\Addr.i
# End Source File
# Begin Source File

SOURCE=.\ARGV.i
# End Source File
# Begin Source File

SOURCE=.\Asynch_Acceptor.i
# End Source File
# Begin Source File

SOURCE=.\Asynch_IO.i
# End Source File
# Begin Source File

SOURCE=.\Asynch_IO_Impl.i
# End Source File
# Begin Source File

SOURCE=.\Atomic_Op.i
# End Source File
# Begin Source File

SOURCE=.\Auto_Ptr.i
# End Source File
# Begin Source File

SOURCE=.\Basic_Types.i
# End Source File
# Begin Source File

SOURCE=.\Connector.i
# End Source File
# Begin Source File

SOURCE=.\Containers.i
# End Source File
# Begin Source File

SOURCE=.\CORBA_Handler.i
# End Source File
# Begin Source File

SOURCE=.\CORBA_Ref.i
# End Source File
# Begin Source File

SOURCE=.\Date_Time.i
# End Source File
# Begin Source File

SOURCE=.\DEV.i
# End Source File
# Begin Source File

SOURCE=.\DEV_Addr.i
# End Source File
# Begin Source File

SOURCE=.\DEV_Connector.i
# End Source File
# Begin Source File

SOURCE=.\DEV_IO.i
# End Source File
# Begin Source File

SOURCE=.\Dirent.i
# End Source File
# Begin Source File

SOURCE=.\Dynamic.i
# End Source File
# Begin Source File

SOURCE=.\Event_Handler.i
# End Source File
# Begin Source File

SOURCE=.\FIFO.i
# End Source File
# Begin Source File

SOURCE=.\FIFO_Recv.i
# End Source File
# Begin Source File

SOURCE=.\FIFO_Recv_Msg.i
# End Source File
# Begin Source File

SOURCE=.\FIFO_Send.i
# End Source File
# Begin Source File

SOURCE=.\FIFO_Send_Msg.i
# End Source File
# Begin Source File

SOURCE=.\FILE.i
# End Source File
# Begin Source File

SOURCE=.\FILE_Addr.i
# End Source File
# Begin Source File

SOURCE=.\FILE_Connector.i
# End Source File
# Begin Source File

SOURCE=.\FILE_IO.i
# End Source File
# Begin Source File

SOURCE=.\Free_List.i
# End Source File
# Begin Source File

SOURCE=.\Get_Opt.i
# End Source File
# Begin Source File

SOURCE=.\Handle_Set.i
# End Source File
# Begin Source File

SOURCE=.\Hash_Map_Manager.i
# End Source File
# Begin Source File

SOURCE=.\Hash_Map_Manager_T.i
# End Source File
# Begin Source File

SOURCE=.\High_Res_Timer.i
# End Source File
# Begin Source File

SOURCE=.\INET_Addr.i
# End Source File
# Begin Source File

SOURCE=.\IO_SAP.i
# End Source File
# Begin Source File

SOURCE=.\IOStream_T.i
# End Source File
# Begin Source File

SOURCE=.\IPC_SAP.i
# End Source File
# Begin Source File

SOURCE=.\Local_Tokens.i
# End Source File
# Begin Source File

SOURCE=.\Log_Record.i
# End Source File
# Begin Source File

SOURCE=.\LSOCK.i
# End Source File
# Begin Source File

SOURCE=.\LSOCK_Acceptor.i
# End Source File
# Begin Source File

SOURCE=.\LSOCK_CODgram.i
# End Source File
# Begin Source File

SOURCE=.\LSOCK_Connector.i
# End Source File
# Begin Source File

SOURCE=.\LSOCK_Dgram.i
# End Source File
# Begin Source File

SOURCE=.\LSOCK_Stream.i
# End Source File
# Begin Source File

SOURCE=.\Malloc.i
# End Source File
# Begin Source File

SOURCE=.\Malloc_T.i
# End Source File
# Begin Source File

SOURCE=.\Map.i
# End Source File
# Begin Source File

SOURCE=.\Map_Manager.i
# End Source File
# Begin Source File

SOURCE=.\Map_T.i
# End Source File
# Begin Source File

SOURCE=.\Mem_Map.i
# End Source File
# Begin Source File

SOURCE=.\Memory_Pool.i
# End Source File
# Begin Source File

SOURCE=.\Message_Block.i
# End Source File
# Begin Source File

SOURCE=.\Message_Queue.i
# End Source File
# Begin Source File

SOURCE=.\Module.i
# End Source File
# Begin Source File

SOURCE=.\Multiplexor.i
# End Source File
# Begin Source File

SOURCE=.\NT_Service.i
# End Source File
# Begin Source File

SOURCE=.\Object_Manager.i
# End Source File
# Begin Source File

SOURCE=.\OS.i
# End Source File
# Begin Source File

SOURCE=.\Pair.i
# End Source File
# Begin Source File

SOURCE=.\Pair_T.i
# End Source File
# Begin Source File

SOURCE=.\Parse_Node.i
# End Source File
# Begin Source File

SOURCE=.\Pipe.i
# End Source File
# Begin Source File

SOURCE=.\Priority_Reactor.i
# End Source File
# Begin Source File

SOURCE=.\Proactor.i
# End Source File
# Begin Source File

SOURCE=.\Process.i
# End Source File
# Begin Source File

SOURCE=.\Process_Manager.i
# End Source File
# Begin Source File

SOURCE=.\Profile_Timer.i
# End Source File
# Begin Source File

SOURCE=.\Reactor.i
# End Source File
# Begin Source File

SOURCE=.\ReactorEx.i
# End Source File
# Begin Source File

SOURCE=.\Read_Buffer.i
# End Source File
# Begin Source File

SOURCE=.\Remote_Tokens.i
# End Source File
# Begin Source File

SOURCE=.\Sched_Params.i
# End Source File
# Begin Source File

SOURCE=.\Select_Reactor_Base.i
# End Source File
# Begin Source File

SOURCE=.\Select_Reactor_T.i
# End Source File
# Begin Source File

SOURCE=.\Service_Config.i
# End Source File
# Begin Source File

SOURCE=.\Service_Manager.i
# End Source File
# Begin Source File

SOURCE=.\Service_Object.i
# End Source File
# Begin Source File

SOURCE=.\Service_Record.i
# End Source File
# Begin Source File

SOURCE=.\Service_Repository.i
# End Source File
# Begin Source File

SOURCE=.\Shared_Memory_MM.i
# End Source File
# Begin Source File

SOURCE=.\Shared_Memory_SV.i
# End Source File
# Begin Source File

SOURCE=.\Shared_Object.i
# End Source File
# Begin Source File

SOURCE=.\Signal.i
# End Source File
# Begin Source File

SOURCE=.\Singleton.i
# End Source File
# Begin Source File

SOURCE=.\SOCK.i
# End Source File
# Begin Source File

SOURCE=.\SOCK_Acceptor.i
# End Source File
# Begin Source File

SOURCE=.\SOCK_CODgram.i
# End Source File
# Begin Source File

SOURCE=.\SOCK_Connector.i
# End Source File
# Begin Source File

SOURCE=.\SOCK_Dgram.i
# End Source File
# Begin Source File

SOURCE=.\SOCK_Dgram_Bcast.i
# End Source File
# Begin Source File

SOURCE=.\SOCK_Dgram_Mcast.i
# End Source File
# Begin Source File

SOURCE=.\SOCK_IO.i
# End Source File
# Begin Source File

SOURCE=.\SOCK_Stream.i
# End Source File
# Begin Source File

SOURCE=.\SPIPE.i
# End Source File
# Begin Source File

SOURCE=.\SPIPE_Addr.i
# End Source File
# Begin Source File

SOURCE=.\SPIPE_Connector.i
# End Source File
# Begin Source File

SOURCE=.\SPIPE_Stream.i
# End Source File
# Begin Source File

SOURCE=.\SString.i
# End Source File
# Begin Source File

SOURCE=.\Stream.i
# End Source File
# Begin Source File

SOURCE=.\Stream_Modules.i
# End Source File
# Begin Source File

SOURCE=.\SV_Message.i
# End Source File
# Begin Source File

SOURCE=.\SV_Message_Queue.i
# End Source File
# Begin Source File

SOURCE=.\SV_Semaphore_Complex.i
# End Source File
# Begin Source File

SOURCE=.\SV_Semaphore_Simple.i
# End Source File
# Begin Source File

SOURCE=.\SV_Shared_Memory.i
# End Source File
# Begin Source File

SOURCE=.\Svc_Handler.i
# End Source File
# Begin Source File

SOURCE=.\Synch.i
# End Source File
# Begin Source File

SOURCE=.\Synch_T.i
# End Source File
# Begin Source File

SOURCE=.\Task.i
# End Source File
# Begin Source File

SOURCE=.\Task_T.i
# End Source File
# Begin Source File

SOURCE=.\Thread.i
# End Source File
# Begin Source File

SOURCE=.\Thread_Manager.i
# End Source File
# Begin Source File

SOURCE=.\Timer_Queue_T.i
# End Source File
# Begin Source File

SOURCE=.\TLI.i
# End Source File
# Begin Source File

SOURCE=.\TLI_Acceptor.i
# End Source File
# Begin Source File

SOURCE=.\TLI_Connector.i
# End Source File
# Begin Source File

SOURCE=.\TLI_Stream.i
# End Source File
# Begin Source File

SOURCE=.\Token.i
# End Source File
# Begin Source File

SOURCE=.\Token_Collection.i
# End Source File
# Begin Source File

SOURCE=.\Token_Invariants.i
# End Source File
# Begin Source File

SOURCE=.\Token_Manager.i
# End Source File
# Begin Source File

SOURCE=.\Token_Request_Reply.i
# End Source File
# Begin Source File

SOURCE=.\Trace.i
# End Source File
# Begin Source File

SOURCE=.\Typed_SV_Message.i
# End Source File
# Begin Source File

SOURCE=.\Typed_SV_Message_Queue.i
# End Source File
# Begin Source File

SOURCE=.\UNIX_Addr.i
# End Source File
# Begin Source File

SOURCE=.\UPIPE_Acceptor.i
# End Source File
# Begin Source File

SOURCE=.\UPIPE_Connector.i
# End Source File
# Begin Source File

SOURCE=.\UPIPE_Stream.i
# End Source File
# Begin Source File

SOURCE=.\WIN32_Proactor.i
# End Source File
# End Group
# Begin Group "Template Files"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\Acceptor.cpp

!IF  "$(CFG)" == "ACE static library - Win32 Debug"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Release"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Unicode Debug"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Unicode Release"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Alpha Debug"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Alpha Release"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Alpha Unicode Debug"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Alpha Unicode Release"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Active_Map_Manager_T.cpp

!IF  "$(CFG)" == "ACE static library - Win32 Debug"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Release"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Unicode Debug"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Unicode Release"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Alpha Debug"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Alpha Release"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Alpha Unicode Debug"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Alpha Unicode Release"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Asynch_Acceptor.cpp

!IF  "$(CFG)" == "ACE static library - Win32 Debug"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Release"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Unicode Debug"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Unicode Release"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Alpha Debug"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Alpha Release"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Alpha Unicode Debug"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Alpha Unicode Release"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Auto_Ptr.cpp
# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\Connector.cpp

!IF  "$(CFG)" == "ACE static library - Win32 Debug"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Release"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Unicode Debug"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Unicode Release"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Alpha Debug"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Alpha Release"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Alpha Unicode Debug"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Alpha Unicode Release"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Dump_T.cpp
# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\Free_List.cpp
# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\Functor_T.cpp

!IF  "$(CFG)" == "ACE static library - Win32 Debug"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Release"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Unicode Debug"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Unicode Release"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Alpha Debug"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Alpha Release"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Alpha Unicode Debug"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Alpha Unicode Release"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Hash_Map_Manager_T.cpp

!IF  "$(CFG)" == "ACE static library - Win32 Debug"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Release"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Unicode Debug"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Unicode Release"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Alpha Debug"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Alpha Release"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Alpha Unicode Debug"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Alpha Unicode Release"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\IOStream_T.cpp
# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\Local_Name_Space_T.cpp
# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\Malloc_T.cpp
# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\Managed_Object.cpp

!IF  "$(CFG)" == "ACE static library - Win32 Debug"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Release"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Unicode Debug"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Unicode Release"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Alpha Debug"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Alpha Release"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Alpha Unicode Debug"

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Alpha Unicode Release"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Map_Manager.cpp
# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\Map_T.cpp
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\Message_Queue_T.cpp

!IF  "$(CFG)" == "ACE static library - Win32 Debug"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Release"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Unicode Debug"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Unicode Release"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Alpha Debug"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Alpha Release"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Alpha Unicode Debug"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Alpha Unicode Release"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Module.cpp
# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\Pair_T.cpp
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\RB_Tree.cpp
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\Select_Reactor_T.cpp

!IF  "$(CFG)" == "ACE static library - Win32 Debug"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Release"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Unicode Debug"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Unicode Release"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Alpha Debug"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Alpha Release"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Alpha Unicode Debug"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "ACE static library - Win32 Alpha Unicode Release"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Singleton.cpp
# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\Strategies_T.cpp
# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\Stream.cpp
# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\Stream_Modules.cpp
# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\Synch_T.cpp
# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\Task_T.cpp
# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\Timer_Hash_T.cpp
# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\Timer_Heap_T.cpp
# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\Timer_List_T.cpp
# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\Timer_Queue_T.cpp
# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\Timer_Wheel_T.cpp
# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1
# End Source File
# End Group
# Begin Group "Documentation"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\Readme
# End Source File
# Begin Source File

SOURCE=..\Version
# End Source File
# End Group
# End Target
# End Project
