#----------------------------------------------------------------------------
#  $Id$
#----------------------------------------------------------------------------
#
#  ACE supports the following flags.  They can be enabled either on
#  the command line, e.g., make purify=1, or added to your
#  platform_macros.GNU.  debug=1 is enabled in the platform files
#  that are released with ACE.
#
#  Please note that the effects of a flag may be platform specific.
#  Also, combinations of certain flags may or may not be allowed on
#  specific platforms, e.g., debug=1 opt=1 is supported by g++ but
#  not all other C++ compilers.
#
#  Flag             Description
#  ----             -----------
#  debug            Enable debugging; see DCFLAGS and DCCFLAGS.
#  exceptions       Enable exception handling (not supported by all platforms).
#  fast             Enable -fast option, e.g., with Sun C++.
#  optimize         Enable optimization; see OCFLAGS and OCCFLAGS.
#  orbix            Enable use of Orbix.
#  profile          Enable profiling; see PCFLAGS and PCCFLAGS.
#  purify           Purify all executables.
#  quantify         Quantify all executables.
#  static_libs_only Only build static libraries.
#  shared_libs_only Only build shared libraries.  Ignore if no SHLIBs are
#                     specified by the  Makefile, as in performance-tests/Misc.
#
#  The following describes the ACE wrapper macros:
#
#  Variable     Description
#  --------     -----------
#  ARFLAGS      Flags for the archive utility (ar)
#  CC           C compiler command
#  CXX          C++ compiler command
#  COMPILE.c    Flags for compiling with C
#  COMPILE.cc   Flags for compiling with C++
#  CPPFLAGS     C pre-processor flags
#  CFLAGS       C compilation flags
#  CCFLAGS      C++ compilation flags
#  DCFLAGS      C compilation flags for debugging
#  DCCFLAGS     C++ compilation flags for debugging
#  DEFFLAGS     C++ preprocessor flag for defining symbols
#  DLD          Name of dynamic linker
#  LD           Name of linker
#  IDL          Name of the CORBA IDL compiler
#  INSBIN       Binary (executable) installation directory
#  INSINC       Include file installation directory
#  INSMAN       Manpage installation directory
#  INSLIB       Library installation directory
#  LDFLAGS      ld linker flags
#  LINK.c       Flags for linking with C
#  LINK.cc      Flags for linking with C++
#  MAKEFLAGS    Flags that are passed into the compilation from the commandline
#  OCFLAGS      Optimizing C compilation flags
#  OCCFLAGS     Optimizing C++ compilation flags
#  ORBIX_ROOT   Root of IONA's Orbix CORBA implementation
#  PCFLAGS      C compilation flags for profiling
#  PCCFLAGS     C++ compilation flags for profiling
#  PRELINK      Executable to precede linking, such as purify or quantify
#  PTDIRS       Pathnames of directories containing template code
#  RM           Name of program to use to remove files
#  SOFLAGS      Flags used to build a shared library
#  SOBUILD      Link line necessary to build a share library
#  VAR          Variant identifier suffix
#  VDIR         Directory for object code
#  VSHDIR       Directory for shared object code
#  ACE_ROOT     Pathname for the root of the build tree
#
#----------------------------------------------------------------------------
#      Platform-dependent macro definitions
#      (link to the appropriate platform-specific config file).
#----------------------------------------------------------------------------

include $(ACE_ROOT)/include/makeinclude/platform_macros.GNU

#----------------------------------------------------------------------------
#      Platform-independent macro definitions
#----------------------------------------------------------------------------

CPPFLAGS        += $(DEFFLAGS) $(INCLDIRS)
IDL             = idl
IDLFLAGS        = -s S.cpp -c C.cpp
INCLDIRS        += -I. -I$(ACE_ROOT)
INSBIN          = $(ACE_ROOT)/bin
INSINC          = $(ACE_ROOT)/ace
INSLIB          = $(ACE_ROOT)/ace
INSMAN          = $(ACE_ROOT)/man
LDFLAGS         += -L$(ACE_ROOT)/ace -L./
LEX             = flex
OCFLAGS         += -O
OCCFLAGS        += -O
PCFLAGS         += #### set in platform_macros.GNU file, e.g., -p
PCCFLAGS        += #### set in platform_macros.GNU file, e.g., -p
RM              = /bin/rm
VAR             =
VDIR            = .obj/
ifndef ACELIB
ACELIB = -lACE
endif
ifndef  VSHDIR
VSHDIR          = .shobj/
endif # VSHDIR
# NOTE: VLDLIBS is defined below, in case it is overridden for static-
#       or shared-only builds.
YACC            = yacc

#----------------------------------------------------------------------------
#      make flags
#----------------------------------------------------------------------------

ifeq ($(optimize),0)
  override optimize =
endif # optimize

ifneq ($(optimize),)
  #### Disable debug, unconditionally.
  override debug =
  CFLAGS  += $(OCFLAGS)
  CCFLAGS += $(OCCFLAGS)
endif # optimize

ifeq ($(debug),0)
  override debug =
endif # debug

ifeq ($(debug),)
  CFLAGS  += -DACE_NDEBUG
  CCFLAGS += -DACE_NDEBUG
else
  CFLAGS  += $(DCFLAGS)
  CCFLAGS += $(DCCFLAGS)
endif # debug

ifeq ($(profile),0)
  override profile =
endif # profile

ifneq ($(profile),)
  CFLAGS  += $(PCFLAGS)
  CCFLAGS += $(PCCFLAGS)
endif # profile


ifeq ($(shared_libs_only),0)
  override shared_libs_only =
endif # shared_libs_only
ifeq ($(static_libs_only),0)
  override static_libs_only =
endif # static_libs_only

ifdef shared_libs_only
  ifeq ($(SHLIB),)
    #### Makefile only specifies static libs, so only build static libs.
  else
    #### don't build static objects:
    BUILD           = $(VSHLIB) $(SHLIBA) $(VBIN)
    LIB             =
    OBJDIRS         = .shobj
    SHOBJ           = $(addsuffix .$(SOEXT),$(FILES))
    VDIR            =
  endif # SHLIB
else
ifdef static_libs_only
  #### don't build shared objects:
  BUILD           = $(VLIB) $(VBIN)
  OBJDIRS         = .obj
  PIC             =
  SHLIB           =
  SHOBJ           = $(addsuffix .o,$(FILES))
  SOEXT           = o
  VSHDIR          = .obj/
  VLDLIBS         = $(LDLIBS:%.so=%.o)
endif # static_libs_only
endif # shared_libs_only

ifdef SHLIBA
  VLDLIBS := $(LDLIBS) -lACEshr $(LIBS)
else
  VLDLIBS := $(LDLIBS) $(ACELIB) $(LIBS)
endif # SHLIBA


ifeq ($(purify),0)
  override purify =
endif # purify

ifdef purify
  #### You might want to adjust the Purify options below.
  #### -best-effort is undocumented but supported, and seems
  #### to help avoid occasional link failure.
  #### SIGINT is used by the ACE Thread_Manager_Test.
  PRELINK += purify -best-effort -cache-dir=/tmp/purifycache -chain-length=20 \
             -fds-inuse-at-exit=no -ignore-signals=SIGINT -inuse-at-exit \
             -max_threads=100
endif # purify

ifeq ($(quantify),0)
  override quantify =
endif # quantify

ifdef quantify
  #### You might want to adjust the Quantify options below.
  #### -best-effort is undocumented but supported, and seems
  #### to help avoid occasional link failure.
  PRELINK += quantify -best-effort -cache-dir=/tmp/purifycache -max_threads=100
endif # quantify


#----------------------------------------------------------------------------
#      Conditional macro definitions
#----------------------------------------------------------------------------

COMPILE.c       = $(CC) $(CFLAGS) $(CPPFLAGS) -c
COMPILE.cc      = $(CXX) $(CCFLAGS) $(CPPFLAGS) $(PTDIRS) -c
# 960905 Marius Kjeldahl <marius@funcom.com>
# Added the line below to be used for compiling executable shared objects
COMPILEESO.cc   = $(CXX) $(CCFLAGS) $(CPPFLAGS) $(PTDIRS)

ifeq ($(LINK.c),override)
  LINK.c        = $(LINK.c.override)
else
  LINK.c        = $(PRELINK) $(LD) $(CFLAGS) $(CPPFLAGS) $(LDFLAGS) $(LDLIBS) \
                  $(LIBS)
endif # LINK.c.override

ifeq ($(LINK.cc),override)
  LINK.cc       = $(LINK.cc.override)
else
  LINK.cc       = $(PRELINK) $(LD) $(CCFLAGS) $(CPPFLAGS) $(PTDIRS)
endif # LINK.cc.override

SOLINK.cc       = $(PRELINK) $(DLD) $(SOFLAGS)
