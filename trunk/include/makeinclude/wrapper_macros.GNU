#----------------------------------------------------------------------------
#	$Id$
#----------------------------------------------------------------------------
# 
# Please see the README file in this directory to get a complete listing
# of all the platform compilation macros that may be defined to port ACE.

#	The following describes the ACE wrapper macros
#
#	Variable	Description
#	--------	-----------
#	ARFLAGS		Flags for the archive utility (ar)
#	CC		C compiler command
#	CXX		C++ compiler command
#	COMPILE.c	Flags for compiling with C
#	COMPILE.cc	Flags for compiling with C++
#	CPPFLAGS	C pre-processor flags
#	CFLAGS		C compilation flags
#	CCFLAGS		C++ compilation flags
#	DCFLAGS		C compilation flags for debugging
#	DCCFLAGS	C++ compilation flags for debugging
#	DEFFLAGS	C++ preprocessor flag for defining symbols 
#	DLD		Name of dynamic linker
#	LD		Name of linker
#	IDL		Name of the CORBA IDL compiler
#	INSBIN		Binary (executable) installation directory
#	INSINC		Include file installation directory
#	INSMAN		Manpage installation directory
#	INSLIB		Library installation directory
#	LDFLAGS		ld linker flags
#	LINK.c		Flags for linking with C
#	LINK.cc		Flags for linking with C++
#	LINE_COMMAND	A hook for passing commands to the linker, e.g., for Quantify
#	MAKEFLAGS	Flags that are passed into the compilation from the commandline
#	OCFLAGS		Optimizing C compilation flags
#	OCCFLAGS	Optimizing C++ compilation flags
#	ORBIX_ROOT	Root of IONA's Orbix CORBA implementation
#	PCFLAGS		C compilation flags for profiling
#	PCCFLAGS	C++ compilation flags for profiling
#	PTDIRS		Pathnames of directories containing template code
#	RM		Name of program to use to remove files
#	SOFLAGS		Flags used to build a shared library
#	SOBUILD		Link line necessary to build a share library
#	VAR		Variant identifier suffix
#	VDIR		Directory for object code
#	VSHDIR		Directory for shared object code
#	WRAPPER_ROOT	Pathname for the root of the build tree
#
#----------------------------------------------------------------------------
#	Platform-dependent macro definitions 
#       (link to the appropriate platform-specific config file).
#----------------------------------------------------------------------------

include $(WRAPPER_ROOT)/include/makeinclude/platform_macros.GNU

#----------------------------------------------------------------------------
#	Platform-independent macro definitions
#----------------------------------------------------------------------------

CPPFLAGS	+= $(DEFFLAGS) $(INCLDIRS)
DCFLAGS		+= -g 
DCCFLAGS	+= -g 
IDL		= idl
IDLFLAGS	= -s S.cpp -c C.cpp
INCLDIRS	+= -I$(WRAPPER_ROOT) -I.
INSBIN		= $(WRAPPER_ROOT)/bin
INSINC		= $(WRAPPER_ROOT)/ace
INSLIB		= $(WRAPPER_ROOT)/ace
INSMAN		= $(WRAPPER_ROOT)/man
LDFLAGS		+= -L$(WRAPPER_ROOT)/ace -L./
LEX		= flex
# LINK_COMMAND = quantify -cache-dir=/tmp/quantify
OCFLAGS		+= -O
OCCFLAGS	+= -O
PCFLAGS		+= # -p
PCCFLAGS	+= # -p
RM		= /bin/rm
VAR		=
VDIR		= .obj/
VSHDIR		= .shobj/
ifdef SHLIBA
VLDLIBS :=$(VLDLIBS) -lACEshr $(LIBS)
else
VLDLIBS :=$(VLDLIBS) -lACE $(LIBS)
endif
YACC		= yacc

#----------------------------------------------------------------------------
#	Conditional macro definitions
#----------------------------------------------------------------------------

ifdef debug
CFLAGS 	+= $(DCFLAGS) 
CCFLAGS	+= $(DCCFLAGS)

else
ifdef profile
CFLAGS 	+= $(PCFLAGS)
CCFLAGS	+= $(PCCFLAGS)

else
ifdef optimize
CFLAGS  += $(OCFLAGS) #-DNDEBUG 
CCFLAGS += $(OCCFLAGS) #-DNDEBUG

else # default settings
CFLAGS  += $(DCFLAGS) #-DNDEBUG 
CCFLAGS += $(DCCFLAGS) #-DNDEBUG 
endif # optimize
endif # profile
endif # debug

COMPILE.c	=$(CC) $(CFLAGS) $(CPPFLAGS)  -c
COMPILE.cc	=$(CXX) $(CCFLAGS) $(CPPFLAGS) $(PTDIRS)  -c
LINK.c		=$(LD) $(CFLAGS) $(CPPFLAGS) $(LDFLAGS) $(LDLIBS) $(LIBS)
LINK.cc		=$(LD) $(CCFLAGS) $(CPPFLAGS) $(PTDIRS) $(LDFLAGS) $(LDLIBS) $(LIBS)
SOLINK.cc	=$(LINK_COMMAND) $(DLD) $(SOFLAGS)
