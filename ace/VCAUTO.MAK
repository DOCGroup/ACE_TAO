# Directory: ace/
# $Id$
#   Auto makefile for ace library.
#   This file is specialized.

MYMAKEFLAGS = /$(MAKEFLAGS)

ace_dll: DLL
	@NMake $(MYMAKEFLAGS) CFG="ACE dynamic library - $(ACE_PLATFORM) $(ACE_TARGET)" /NOLOGO /F $@.mak $(ACE_ACTION)

#ace_lib: LIB
#	@NMake $(MYMAKEFLAGS) CFG="ACE static library - $(ACE_PLATFORM) $(ACE_TARGET)" /NOLOGO /F $@.mak $(ACE_ACTION)

DLL :
	@mkdir DLL

LIB : 
	@mkdir LIB
