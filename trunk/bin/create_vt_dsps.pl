# $Id$
# Script that creates the version_tests dsp files
# To use, go to the ACE_wrappers\tests\version_tests directory and
# run.

if ($^O ne "MSWin32") {
    die "Not running on Win32";
}

opendir DIR, ".." or die "Couldn't open dir: $!"; 

open DSW, ">version_tests.dsw" or die "Could not create dsw: $!";

@allfiles = readdir DIR;
closedir DIR;

print DSW "Microsoft Developer Studio Workspace File, Format Version 6.00\n";
print DSW "# WARNING: DO NOT EDIT OR DELETE THIS WORKSPACE FILE!\n";
print DSW "\n";

@data=<DATA>;

foreach $file (@allfiles) {
    if ($file =~ /\.dsp$/i) {
        ($shortname, $junk) = split /\./, $file;
        ### Don't do ACE_Init_Test since it uses static ACE and MFC, which is tricky and isn't
        ### covered by this script.
        if ($shortname ne "ACE_Init_Test") {
            print "Converting: $shortname\n";
            open NEW, ">$file" or die "Could not create $file: $!";

            my @mydata = @data;
            map {s/XXXXXXXX/$shortname/g;} @mydata;
            print NEW @mydata;

            open OLD, "<../$file" or die "Could not open ../$file: $!";
            while (<OLD>) {
                s/SOURCE=\.\\/SOURCE=\.\.\\/;
                print NEW "$_\n" if (/^\# Begin Group/ || /^\# Begin Source/);
                print NEW if (/^\# End/ || /^SOURCE=/ || /^\# PROP Default_Filter/);
            }

            close OLD;
	    close NEW;	

            print DSW "###############################################################################\n";
            print DSW "\n";
            print DSW "Project: \"$file\"=.\\$file - Package Owner=<4>\n";
            print DSW "\n";
            print DSW "Package=<5>\n";
            print DSW "{{{\n";
            print DSW "}}}\n";
            print DSW "\n";
            print DSW "Package=<4>\n";
            print DSW "{{{\n";
            print DSW "}}}\n";
            print DSW "\n";
        }        
    }
}

$junk = $junk;  # to get rid of a warning

print DSW "###############################################################################\n";
close DSW;

__DATA__
# Microsoft Developer Studio Project File - Name="XXXXXXXX" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Console Application" 0x0103

CFG=XXXXXXXX - Win32 Release
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "XXXXXXXX.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "XXXXXXXX.mak" CFG="XXXXXXXX - Win32 Release"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "XXXXXXXX - Win32 Static Debug" (based on "Win32 (x86) Console Application")
!MESSAGE "XXXXXXXX - Win32 Static Release" (based on "Win32 (x86) Console Application")
!MESSAGE "XXXXXXXX - Win32 Release" (based on "Win32 (x86) Console Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "XXXXXXXX - Win32 Static Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir ".\XXXXXXXX\Static Debug"
# PROP BASE Intermediate_Dir ".\XXXXXXXX\Static Debug"
# PROP BASE Target_Dir ".\XXXXXXXX"
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir ".\LIB\Debug"
# PROP Intermediate_Dir ".\LIB\Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ".\XXXXXXXX"
# ADD BASE CPP /nologo /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /YX /c
# ADD CPP /nologo /MDd /W3 /Gm /GX /Zi /Od /I "../../" /D "_DEBUG" /D ACE_AS_STATIC_LIBS /D "WIN32" /D "_CONSOLE" /FD /c
# SUBTRACT CPP /YX
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /debug /machine:I386
# ADD LINK32 acesd.lib advapi32.lib user32.lib /nologo /subsystem:console /debug /machine:I386 /libpath:"..\..\ace"

!ELSEIF  "$(CFG)" == "XXXXXXXX - Win32 Static Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir ".\XXXXXXXX\Static Release"
# PROP BASE Intermediate_Dir ".\XXXXXXXX\Static Release"
# PROP BASE Target_Dir ".\XXXXXXXX"
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir ".\LIB\Release"
# PROP Intermediate_Dir ".\LIB\Release"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ".\XXXXXXXX"
# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /YX /c
# ADD CPP /nologo /MD /W3 /GX /O2 /I "../../" /D "NDEBUG" /D ACE_AS_STATIC_LIBS /D "WIN32" /D "_CONSOLE" /FD /c
# SUBTRACT CPP /YX
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /machine:I386
# ADD LINK32 aces.lib advapi32.lib user32.lib /nologo /subsystem:console /machine:I386 /libpath:"..\..\ace"

!ELSEIF  "$(CFG)" == "XXXXXXXX - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir ".\XXXXXXXX\Release"
# PROP BASE Intermediate_Dir ".\XXXXXXXX\Release"
# PROP BASE Target_Dir ".\XXXXXXXX"
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir ".\DLL\Release"
# PROP Intermediate_Dir ".\DLL\Release"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ".\XXXXXXXX"
# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /YX /c
# ADD CPP /nologo /MD /W3 /GX /O2 /I "../../" /D "NDEBUG" /D "WIN32" /D "_CONSOLE" /FD /c
# SUBTRACT CPP /YX
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /machine:I386
# ADD LINK32 ace.lib /nologo /subsystem:console /machine:I386 /libpath:"..\..\ace"

!ENDIF 

# Begin Target

# Name "XXXXXXXX - Win32 Static Debug"
# Name "XXXXXXXX - Win32 Static Release"
# Name "XXXXXXXX - Win32 Release"
