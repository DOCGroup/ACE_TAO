# Microsoft Developer Studio Project File - Name="PACE LIB" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Static Library" 0x0104

CFG=PACE LIB - Win32 Static Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "pace_lib.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "pace_lib.mak" CFG="PACE LIB - Win32 Static Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "PACE LIB - Win32 Static Release" (based on "Win32 (x86) Static Library")
!MESSAGE "PACE LIB - Win32 Static Debug" (based on "Win32 (x86) Static Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "PACE LIB - Win32 Static Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir ""
# PROP Intermediate_Dir "LIB\Release"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_MBCS" /D "_LIB" /YX /FD /c
# ADD CPP /nologo /MD /W3 /GX /O2 /I "../../" /D "NDEBUG" /D "PACE_AS_STATIC_LIBS" /D "WIN32" /D "PACE_HAS_ALL_POSIX_FUNCS" /V"ERBOSE:LIB" /FD /c
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo /out:".\paces.lib"

!ELSEIF  "$(CFG)" == "PACE LIB - Win32 Static Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "pace_lib___Win32_Debug"
# PROP BASE Intermediate_Dir "pace_lib___Win32_Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir ""
# PROP Intermediate_Dir "LIB\Debug"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_MBCS" /D "_LIB" /YX /FD /c
# ADD CPP /nologo /MDd /W3 /Gm /GX /Zi /Od /I "../../" /D "_DEBUG" /D "WIN32" /D "PACE_HAS_ALL_POSIX_FUNCS" /D "PACE_AS_STATIC_LIBS" /FD /c
# SUBTRACT CPP /YX
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo /out:".\pacesd.lib"

!ENDIF 

# Begin Target

# Name "PACE LIB - Win32 Static Release"
# Name "PACE LIB - Win32 Static Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\aio.c
# End Source File
# Begin Source File

SOURCE=.\assert.c
# End Source File
# Begin Source File

SOURCE=.\ctype.c
# End Source File
# Begin Source File

SOURCE=.\dirent.c
# End Source File
# Begin Source File

SOURCE=.\fcntl.c
# End Source File
# Begin Source File

SOURCE=.\grp.c
# End Source File
# Begin Source File

SOURCE=.\locale.c
# End Source File
# Begin Source File

SOURCE=.\math.c
# End Source File
# Begin Source File

SOURCE=.\mman.c
# End Source File
# Begin Source File

SOURCE=.\mqueue.c
# End Source File
# Begin Source File

SOURCE=.\pthread.c
# End Source File
# Begin Source File

SOURCE=.\pwd.c
# End Source File
# Begin Source File

SOURCE=.\sched.c
# End Source File
# Begin Source File

SOURCE=.\semaphore.c
# End Source File
# Begin Source File

SOURCE=.\setjmp.c
# End Source File
# Begin Source File

SOURCE=.\signal.c
# End Source File
# Begin Source File

SOURCE=.\stat.c
# End Source File
# Begin Source File

SOURCE=.\stdio.c
# End Source File
# Begin Source File

SOURCE=.\stdlib.c
# End Source File
# Begin Source File

SOURCE=.\string.c
# End Source File
# Begin Source File

SOURCE=.\termios.c
# End Source File
# Begin Source File

SOURCE=.\time.c
# End Source File
# Begin Source File

SOURCE=.\times.c
# End Source File
# Begin Source File

SOURCE=.\unistd.c
# End Source File
# Begin Source File

SOURCE=.\utime.c
# End Source File
# Begin Source File

SOURCE=.\utsname.c
# End Source File
# Begin Source File

SOURCE=.\wait.c
# End Source File
# End Group
# Begin Group "Header Files (Prototypes)"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=..\aio.h
# End Source File
# Begin Source File

SOURCE=..\assert.h
# End Source File
# Begin Source File

SOURCE=..\ctype.h
# End Source File
# Begin Source File

SOURCE=..\dirent.h
# End Source File
# Begin Source File

SOURCE=..\errno.h
# End Source File
# Begin Source File

SOURCE=..\fcntl.h
# End Source File
# Begin Source File

SOURCE=..\grp.h
# End Source File
# Begin Source File

SOURCE=..\limits.h
# End Source File
# Begin Source File

SOURCE=..\locale.h
# End Source File
# Begin Source File

SOURCE=..\math.h
# End Source File
# Begin Source File

SOURCE=..\sys\mman.h
# End Source File
# Begin Source File

SOURCE=..\mqueue.h
# End Source File
# Begin Source File

SOURCE=..\pthread.h
# End Source File
# Begin Source File

SOURCE=..\pwd.h
# End Source File
# Begin Source File

SOURCE=..\sched.h
# End Source File
# Begin Source File

SOURCE=..\semaphore.h
# End Source File
# Begin Source File

SOURCE=..\setjmp.h
# End Source File
# Begin Source File

SOURCE=..\signal.h
# End Source File
# Begin Source File

SOURCE=..\sys\stat.h
# End Source File
# Begin Source File

SOURCE=..\stddef.h
# End Source File
# Begin Source File

SOURCE=..\stdio.h
# End Source File
# Begin Source File

SOURCE=..\stdlib.h
# End Source File
# Begin Source File

SOURCE=..\string.h
# End Source File
# Begin Source File

SOURCE=..\termios.h
# End Source File
# Begin Source File

SOURCE=..\time.h
# End Source File
# Begin Source File

SOURCE=..\sys\times.h
# End Source File
# Begin Source File

SOURCE=..\sys\types.h
# End Source File
# Begin Source File

SOURCE=..\unistd.h
# End Source File
# Begin Source File

SOURCE=..\utime.h
# End Source File
# Begin Source File

SOURCE=..\sys\utsname.h
# End Source File
# Begin Source File

SOURCE=..\sys\wait.h
# End Source File
# End Group
# Begin Group "Header Files (Win32 Specific)"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\aio.h
# End Source File
# Begin Source File

SOURCE=.\assert.h
# End Source File
# Begin Source File

SOURCE=.\ctype.h
# End Source File
# Begin Source File

SOURCE=.\dirent.h
# End Source File
# Begin Source File

SOURCE=.\errno.h
# End Source File
# Begin Source File

SOURCE=.\fcntl.h
# End Source File
# Begin Source File

SOURCE=.\grp.h
# End Source File
# Begin Source File

SOURCE=.\limits.h
# End Source File
# Begin Source File

SOURCE=.\locale.h
# End Source File
# Begin Source File

SOURCE=.\math.h
# End Source File
# Begin Source File

SOURCE=.\mman.h
# End Source File
# Begin Source File

SOURCE=.\mqueue.h
# End Source File
# Begin Source File

SOURCE=.\pthread.h
# End Source File
# Begin Source File

SOURCE=.\pthreadtypes.h
# End Source File
# Begin Source File

SOURCE=.\pwd.h
# End Source File
# Begin Source File

SOURCE=.\sched.h
# End Source File
# Begin Source File

SOURCE=.\schedtypes.h
# End Source File
# Begin Source File

SOURCE=.\semaphore.h
# End Source File
# Begin Source File

SOURCE=.\setjmp.h
# End Source File
# Begin Source File

SOURCE=.\signal.h
# End Source File
# Begin Source File

SOURCE=.\stat.h
# End Source File
# Begin Source File

SOURCE=.\stdio.h
# End Source File
# Begin Source File

SOURCE=.\stdlib.h
# End Source File
# Begin Source File

SOURCE=.\string.h
# End Source File
# Begin Source File

SOURCE=.\termios.h
# End Source File
# Begin Source File

SOURCE=.\time.h
# End Source File
# Begin Source File

SOURCE=.\times.h
# End Source File
# Begin Source File

SOURCE=.\types.h
# End Source File
# Begin Source File

SOURCE=.\unistd.h
# End Source File
# Begin Source File

SOURCE=.\utime.h
# End Source File
# Begin Source File

SOURCE=.\utsname.h
# End Source File
# Begin Source File

SOURCE=.\wait.h
# End Source File
# End Group
# Begin Group "Inline Files"

# PROP Default_Filter "inl"
# Begin Source File

SOURCE=.\aio.inl
# End Source File
# Begin Source File

SOURCE=.\assert.inl
# End Source File
# Begin Source File

SOURCE=.\ctype.inl
# End Source File
# Begin Source File

SOURCE=.\dirent.inl
# End Source File
# Begin Source File

SOURCE=.\fcntl.inl
# End Source File
# Begin Source File

SOURCE=.\grp.inl
# End Source File
# Begin Source File

SOURCE=.\locale.inl
# End Source File
# Begin Source File

SOURCE=.\math.inl
# End Source File
# Begin Source File

SOURCE=.\mman.inl
# End Source File
# Begin Source File

SOURCE=.\mqueue.inl
# End Source File
# Begin Source File

SOURCE=.\pthread.inl
# End Source File
# Begin Source File

SOURCE=.\pwd.inl
# End Source File
# Begin Source File

SOURCE=.\sched.inl
# End Source File
# Begin Source File

SOURCE=.\semaphore.inl
# End Source File
# Begin Source File

SOURCE=.\setjmp.inl
# End Source File
# Begin Source File

SOURCE=.\signal.inl
# End Source File
# Begin Source File

SOURCE=.\stat.inl
# End Source File
# Begin Source File

SOURCE=.\stdio.inl
# End Source File
# Begin Source File

SOURCE=.\stdlib.inl
# End Source File
# Begin Source File

SOURCE=.\string.inl
# End Source File
# Begin Source File

SOURCE=.\termios.inl
# End Source File
# Begin Source File

SOURCE=.\time.inl
# End Source File
# Begin Source File

SOURCE=.\times.inl
# End Source File
# Begin Source File

SOURCE=.\unistd.inl
# End Source File
# Begin Source File

SOURCE=.\utime.inl
# End Source File
# Begin Source File

SOURCE=.\utsname.inl
# End Source File
# Begin Source File

SOURCE=.\wait.inl
# End Source File
# End Group
# Begin Group "Config Files"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\config\compiler.h
# End Source File
# Begin Source File

SOURCE=..\config\config.h
# End Source File
# Begin Source File

SOURCE=..\config\constants.h
# End Source File
# Begin Source File

SOURCE=..\config\defaults.h
# End Source File
# Begin Source File

SOURCE=..\config\defines.h
# End Source File
# Begin Source File

SOURCE=..\config\features.h
# End Source File
# Begin Source File

SOURCE=..\config\platform.h
# End Source File
# Begin Source File

SOURCE=..\config\temp.h
# End Source File
# Begin Source File

SOURCE=..\config\utility.h
# End Source File
# End Group
# End Target
# End Project
