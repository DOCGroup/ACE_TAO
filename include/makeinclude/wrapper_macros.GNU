#----------------------------------------------------------------------------
#  $Id$
#----------------------------------------------------------------------------
#
#  ACE supports the following flags.  They can be enabled either on
#  the command line, e.g., "make purify=1", or added to your
#  platform_macros.GNU.  To disable the option, set the flag to null,
#  e.g., "make debug=".  Some flags support setting to 0 disable,
#  e.g., "make debug=0".  debug=1 is enabled in the platform files
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
#  minimum_corba    Enable minimumCORBA support.
#  fast             Enable -fast option, e.g., with Sun C++.
#  inline           Enable ACE inlining.  Some platforms enable inlining by
#                     default, others do not.
#  optimize         Enable optimization; see OCFLAGS and OCCFLAGS.
#  orbix            Enable use of Orbix.
#  probe            Enable ACE_Timeprobes.
#  profile          Enable profiling; see PCFLAGS and PCCFLAGS.
#  purify           Purify all executables.  purify must be in user's PATH!
#  quantify         Quantify all executables.  quantify must be in user's PATH!
#  repo             Use GNU template repository (g++ with repo patches and
#                     egcs only).
#  rtti             Enable run-time type identification.  On some platforms,
#                     it is enabled by default, so this is ignored.
#  shared_libs      Build shared libraries. Ignored if static_libs_only is set.
#  static_libs      Build shared libraries. Ignored if shared_libs_only is set.
#  shared_libs_only Only build shared libraries.  Ignored if no SHLIBs are
#                     specified by the Makefile, as in performance-tests/Misc.
#  static_libs_only Only build static libraries.
#  threads          Build with thread support.
#  xt_reactor       Build the XtReactor.
#
#  Usually, users do not need to be concerned with make targets.
#  Just enter "make" on the command line to build.  A few notable
#  targets are listed below.
#
#  Target             Description
#  ------             -----------
#  show_statics       Lists all static objects in object files built for
#                       current directory.  Only supported for g++.
#  show_uninit        Lists all uninitialized in object files built for
#                       current directory.  Only supported for g++.
#
#  The following describes the ACE wrapper macros:
#
#  Variable     Description
#  --------     -----------
#  ACE_HAS_GNUG_PRE_2_8 CXX includes "g++", and the g++ version is
#                       prior to 2.8.0.  This is automatically determined
#                       below, but can be overridden in the user's
#                       platform_macros.GNU or environment.
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
#  PLATFORM_XT_CPPFLAGS   Platform CPP options for X11 (some require -I...)
#  PLATFORM_XT_LDFLAGS    Platform LD options for X11 (some require -L...)
#  PLATFORM_XT_LIBS       Platform libraries require with X11
#  PRELINK      Executable to precede linking, such as quantify
#  PURELINK     If set, contains purify executable as well as all options
#  PURE_CACHE_BASE_DIR
#               The base directory in which Pure products will produce
#               instrumented libraries.  If you don't want that to be below
#               /tmp, set this variable in your environment or on the make
#               command line.
#  PURE_CACHE_DIR
#               The full name of the directory in which Pure produces will
#               produce instrumented libraries.
#  PTDIRS       Pathnames of directories containing template code
#  RM           Name of program to use to remove files
#  SOFLAGS      Flags used to build a shared library
#  SOLINK.cc    Link line necessary to build a share library
#  VAR          Variant identifier suffix
#  VDIR         Directory for object code
#  VSHDIR       Directory for shared object code
#  ACE_ROOT     Pathname for the root of the build tree
#
#----------------------------------------------------------------------------
#      Platform-dependent macro definitions
#      (link to the appropriate platform-specific config file).
#----------------------------------------------------------------------------

#### Default to building shared libraries only.  Set it here to give the
#### platform a chance to override.
shared_libs = 1
static_libs = 0

#### Let platform just look at whether or not exceptions is defined.
#### With the following override, it doesn't have to check for 0.
ifeq ($(exceptions),0)
  override exceptions =
endif # exceptions

ifeq ($(fast),0)
  override fast =
endif # fast

ifeq ($(rtti),0)
  override rtti =
endif # rtti

#### Defined versioned_so = 1 if your platform requires version number
#### be appended after so library.
versioned_so = 0

#### The system has X Windows and we want to include the XtReactor in
#### the library
xt_reactor = 0

include $(ACE_ROOT)/include/makeinclude/platform_macros.GNU

#----------------------------------------------------------------------------
#      Platform-independent macro definitions
#----------------------------------------------------------------------------

CPPFLAGS        += $(DEFFLAGS) $(INCLDIRS)
IDL             = idl
IDLFLAGS        = -A -B -s S.cpp -c C.cpp
ifeq (,$(findstring -I$(ACE_ROOT),$(INCLDIRS)))
  INCLDIRS        += -I. -I$(ACE_ROOT)
endif
INSBIN          = $(ACE_ROOT)/bin
INSINC          = $(ACE_ROOT)/ace
INSLIB          = $(ACE_ROOT)/ace
INSMAN          = $(ACE_ROOT)/man
ifeq (,$(findstring -L$(ACE_ROOT)/ace,$(LDFLAGS)))
  LDFLAGS         += -L$(ACE_ROOT)/ace -L./
endif
LEX             = flex
PCFLAGS         += #### set in platform_macros.GNU file, e.g., -p
PCCFLAGS        += #### set in platform_macros.GNU file, e.g., -p
VAR             =
VDIR            = .obj/
ifndef ACELIB
  ACELIB = -lACE
endif
ifndef VSHDIR
  VSHDIR        = .shobj/
endif # VSHDIR
# NOTE: VLDLIBS is defined below, in case it is overridden for static-
#       or shared-only builds.
YACC            = yacc

# Define SOEXT trailing digits if platform needs it.
ifeq ($(versioned_so),1)
  ACE_LDSO_Version_Number := $(shell echo .`perl -ne  'if (/ACE version/) \
      { s/[^0-9]+(\d+\.\d+)(\.\d+)?.+/\1/ ; print }' ${ACE_ROOT}/VERSION `)
else
  ACE_LDSO_Version_Number =
endif

#----------------------------------------------------------------------------
#      Platform-dependent macros that require platform_macros.GNU.
#----------------------------------------------------------------------------

ifeq ($(findstring g++,$(CXX)),g++)
  ifndef ACE_HAS_GNUG_PRE_2_8
    ifeq ($(CXX),g++)
      ACE_HAS_GNUG_PRE_2_8 := \
        $(shell \
          if $(CXX) --version | egrep '^(cygnus-)?2\.[0-7]' > /dev/null; then \
          echo 1; else echo 0; fi)
    else
    ifeq ($(CXX),eg++)
      # Assume that eg++ is egcs.
      ACE_HAS_GNUG_PRE_2_8 := 0
    else  # ! plain g++ or eg++
      # CXX is something like g++386.  Assume, for now, that it's an old g++.
      ACE_HAS_GNUG_PRE_2_8 := 1
    endif # eg++
    endif # ! plain g++ or eg++
  endif # ! ACE_HAS_GNUG_PRE_2_8
else
  ACE_HAS_GNUG_PRE_2_8 =
endif # g++

#----------------------------------------------------------------------------
#      make flags
#----------------------------------------------------------------------------

ifeq ($(optimize),0)
  override optimize =
endif # optimize

ifneq ($(optimize),)
  CFLAGS  += $(OCFLAGS)
  CCFLAGS += $(OCCFLAGS)
endif # optimize

ifeq ($(debug),0)
  override debug =
endif # debug

ifeq ($(debug),)
  ifeq (,$(findstring $(CFLAGS),$(CCFLAGS)))
    CCFLAGS += -DACE_NDEBUG
  else
    CFLAGS  += -DACE_NDEBUG
  endif
else
  CFLAGS  += $(DCFLAGS)
  CCFLAGS += $(DCCFLAGS)
endif # debug

ifeq ($(threads),0)
  override threads =
endif # threads

ifeq ($(xt_reactor),0)
  override xt_reactor =
endif # xt_reactor

ifneq ($(xt_reactor),)
  override ace_with_x11=1
  CPPFLAGS += -DACE_HAS_XT $(PLATFORM_XT_CPPFLAGS)
  LIBS += $(PLATFORM_XT_LIBS)
  LDFLAGS += $(PLATFORM_XT_LDFLAGS)
endif # xt_reactor

ifeq ($(fl_reactor),0)
  override fl_reactor =
endif # fl_reactor

ifneq ($(fl_reactor),)
  override ace_with_x11=1
  override ace_with_gl=1
  CPPFLAGS += -DACE_HAS_FL $(PLATFORM_FL_CPPFLAGS)
  LIBS += $(PLATFORM_FL_LIBS)
  LDFLAGS += $(PLATFORM_FL_LDFLAGS)
endif # fl_reactor

ifeq ($(ace_with_gl),0)
  override ace_with_gl =
endif # ace_with_gl

ifneq ($(ace_with_gl),)
  override ace_with_x11 = 1
  CPPFLAGS += $(PLATFORM_GL_CPPFLAGS)
  LIBS += $(PLATFORM_GL_LIBS)
  LDFLAGS += $(PLATFORM_GL_LDFLAGS)
endif # ace_with_gl

ifeq ($(ace_with_x11),0)
  override ace_with_x11 =
endif # ace_with_x11

ifneq ($(ace_with_x11),)
  CPPFLAGS += $(PLATFORM_X11_CPPFLAGS)
  LIBS += $(PLATFORM_X11_LIBS)
  LDFLAGS += $(PLATFORM_X11_LDFLAGS)
endif # ace_with_x11


ifdef exceptions
  CPPFLAGS += -DACE_HAS_EXCEPTIONS
  ifeq ($(findstring g++,$(CXX)),g++)
    ifeq ($(ACE_HAS_GNUG_PRE_2_8),1)
      #### Enable exception handling with g++ < 2.8.0.  Not recommended,
      #### but provided here for completeness.
      CCFLAGS += -fhandle-exceptions
    endif # ACE_HAS_GNUG_PRE_2_8
  endif # g++
else  # ! exceptions
  ifeq ($(ACE_HAS_GNUG_PRE_2_8),0)
    #### Disable the default exception handling of g++ >= 2.8.0.
    CCFLAGS += -fno-exceptions
  endif # ! ACE_HAS_GNUG_PRE_2_8
endif # ! exceptions

ifeq ($(minimum_corba),1)
  CPPFLAGS += -DTAO_HAS_MINIMUM_CORBA
endif # minimum_corba

ifeq ($(profile),0)
  override profile =
endif # profile

ifneq ($(profile),)
  CFLAGS  += $(PCFLAGS)
  CCFLAGS += $(PCCFLAGS)
endif # profile

ifeq ($(inline),0)
  CCFLAGS += -DACE_NO_INLINE
else
ifeq ($(inline),1)
  CCFLAGS += -D__ACE_INLINE__
endif # inline eq 1
endif # inline eq 0

ifeq ($(shared_libs),0)
  override shared_libs =
endif # shared_libs
ifeq ($(static_libs),0)
  override static_libs =
endif # static_libs

ifeq ($(shared_libs_only),0)
  override shared_libs_only =
endif # shared_libs_only
ifeq ($(static_libs_only),0)
  override static_libs_only =
endif # static_libs_only

ifdef shared_libs_only
  override shared_libs = 1
  override static_libs =
endif # shared_libs_only
ifdef static_libs_only
  override shared_libs =
  override static_libs = 1
endif # static_libs_only

#### We need to set VBIN here, because it's used in the BUILD
#### definitions below.
VBIN = $(BIN:%=%$(VAR))

ifdef shared_libs
  ifdef static_libs
    #### Build both shared and static libs.
    BUILD  = $(VSHLIB) $(SHLIBA) $(VLIB) $(VBIN)
  else # ! static_libs
    #### Build shared libs only.
    ifeq ($(SHLIB),)
      #### Makefile only specifies static libs, so only build static libs.
      BUILD         = $(VLIB) $(VBIN)
    else # ! SHLIB
      #### Don't build static libs.
      BUILD         = $(VSHLIB) $(SHLIBA) $(VBIN)
      LIB           =
      OBJEXT        = so
      ifndef VXWORKS
        VDIR        = .shobj/
      endif # ! VXWORKS
      ifeq ($(BIN),)
        #### No executables are being built, so don't create .obj/.
        OBJDIRS     = .shobj
      endif # BIN
    endif # ! SHLIB
  endif # ! static_libs
else # ! shared_libs
  ifdef static_libs
    #### Don't build shared libs.
    BUILD           = $(VLIB) $(VBIN)
    OBJDIRS         = .obj
    PIC             =
    SHLIB           =
    SOEXT           = o
    VSHDIR          = .obj/
    VLDLIBS         = $(LDLIBS:%.so=%.o)
  else # ! static_libs
    #### Don't build any libs.
    BUILD           = $(VBIN)
  endif # ! static_libs
endif # ! shared_libs

ifdef SHLIBA
  VLDLIBS := $(LDLIBS) -lACEshr $(LIBS)
else
  VLDLIBS := $(LDLIBS) $(ACELIB) $(LIBS)
endif # SHLIBA

ifeq ($(probe),0)
  override probe =
endif # probe

ifeq ($(probe),1)
    CCFLAGS += -DACE_COMPILE_TIMEPROBES
endif # probe

ifeq ($(purify),0)
  override purify =
endif # purify

PURE_CACHE_BASE_DIR = /tmp/purifycache
PURE_CACHE_DIR = $(PURE_CACHE_BASE_DIR)-$(LOGNAME)-$(notdir $(CXX) )

ifdef purify
  #### Pick up Purify directory from the users PATH.
  ACE_PURIFY_DIR := \
    $(shell type purify | sed -e 's/.* is //' -e 's%/purify$$%%')

  #### You might want to adjust the Purify options below.
  #### -best-effort is undocumented but supported, and seems
  #### to help avoid occasional link failure.
  #### SIGINT is used by the ACE Thread_Manager_Test.
  PURELINK += purify -best-effort -cache-dir=$(PURE_CACHE_DIR) \
              -chain-length=20 -fds-inuse-at-exit=no -ignore-signals=SIGINT \
              -inuse-at-exit -max_threads=100
  CCFLAGS += -DACE_HAS_PURIFY
  CPPFLAGS += -I$(ACE_PURIFY_DIR)
endif # purify

ifeq ($(quantify),0)
  override quantify =
endif # quantify

ifdef quantify
  #### Pick up Quantify directory from the users PATH.
  ACE_QUANTIFY_DIR := \
    $(shell type quantify | sed -e 's/.* is //' -e 's%/quantify$$%%')

  #### You might want to adjust the Quantify options below.
  #### -best-effort is undocumented but supported, and seems
  #### to help avoid occasional link failure.
  PRELINK += quantify -best-effort -cache-dir=$(PURE_CACHE_DIR) \
             -max_threads=100
  CCFLAGS += -DACE_HAS_QUANTIFY
  CPPFLAGS += -I$(ACE_QUANTIFY_DIR)

  ifeq ($(shell uname -p),sparc)
    PRELINK += -record-register-window-traps
  endif # sparc
endif # quantify

ifeq ($(repo),0)
  override repo =
endif # repo

ifdef repo
  #### Remove -fno-implicit-templates from, and add -frepo to, CCFLAGS.
  CCFLAGS := $(strip $(subst -fno-implicit-templates,,$(CCFLAGS)))
  CCFLAGS += -frepo -DACE_HAS_GNU_REPO
endif # repo

#----------------------------------------------------------------------------
#      Conditional macro definitions
#----------------------------------------------------------------------------

COMPILE.c       = $(CC) $(CFLAGS) $(CPPFLAGS) -c
COMPILE.cc      = $(CXX) $(CCFLAGS) $(CPPFLAGS) $(PTDIRS) -c
COMPILE-NO_DASH_G.cc = `echo $(COMPILE.cc) | sed 's/-g //'`
# 960905 Marius Kjeldahl <marius@funcom.com>
# Added the line below to be used for compiling executable shared objects
COMPILEESO.cc   = $(CXX) $(CCFLAGS) $(CPPFLAGS) $(PTDIRS)

ifeq ($(LINK.c),override)
  LINK.c  = $(LINK.c.override)
else
  LINK.c  = $(PURELINK) $(PRELINK) $(LD) $(CFLAGS) $(CPPFLAGS) $(LDFLAGS) \
            $(LDLIBS) $(LIBS)
endif # LINK.c.override

ifeq ($(LINK.cc),override)
  LINK.cc = $(LINK.cc.override)
else
  LINK.cc = $(PURELINK) $(PRELINK) $(LD) $(CCFLAGS) $(CPPFLAGS) $(PTDIRS)
endif # LINK.cc.override

SOLINK.cc = $(PRELINK) $(DLD) $(SOFLAGS)
