# Filename: VCAUTO.MAK
#   Top level makefile for MSVC.
#   $Id$

ACE_PLATFORM = Win32
#ACE_PLATFORM = Win32 Alpha

ACE_TARGET = Debug
#ACE_TARGET = Release
#ACE_TARGET = Unicode Debug
#ACE_TARGET = Unicode Release

#ACE_ACTION = clean

DIR_LIST = ace.dir netsvcs.dir tests.dir #apps.dir

!INCLUDE <$(ACE_ROOT)\include\makeinclude\VC50_DIR.MAK>
