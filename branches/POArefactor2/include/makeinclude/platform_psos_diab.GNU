# $Id$
# pSOS with Diab Data C++ 4.1a Compiler

# ================================================================
# pSOS sTUFF
# ================================================================

PSS_LIBC_DIR	= $(PSS_ROOT)/sys/libc
PSS_SYS_DIR	= $(PSS_ROOT)/sys/os

PSOS_LIBS = -L $(PSS_LIBC_DIR) -lprepc -lcxxsp \
            -L $(PSS_SYS_DIR) -lsysxx -lsys

PSS_BSP_LIB	= -L $(PSS_BSP) -lbsp

# DIAB_LIBS = -L $(DIABLIB)/MC60FS/psos -lpsos

# DIAB_LIBS = -L $(DIABLIB)/MC60FS -li -lchar -lg -limpl -lram -ld \
#            -L $(DIABLIB)/MC60FS -lcfp -lm -limpfp -lcomplex -lios \
#            -L $(DIABLIB)/MC60FS/psos -lpsos \
#            -L $(PSS_ROOT)/sys/libc -lprepc

# DIAB_LIBS = -L $(DIABLIB)/MC60FS -limpfp \
#            -L $(DIABLIB)/MC60F -limpl \
#            -L $(DIABLIB)/MC60F/psos -lpsos


#DIAB_LIBS = -L $(DIABLIB)/MC60F/psos -lc \
#            -L $(PSS_ROOT)/sys/libc -lprepc
#            -L $(DIABLIB)/MC60F -lchar -lg -limpl -lram -ld \
#            -L $(DIABLIB)/MC60FS -lcfp -lm -limpfp -lcomplex -lios -li

PSOSINCL = -I$(PSS_ROOT)/include -I$(PSS_BSP)
# PSOSINCL = -Y I,$(PSS_ROOT)/include -I$(PSS_BSP)

# hacks to resolve incompatibilities between the compiler and OS headers
DIAB_PSOS_HACKS = -D__Itypes -D_TIME_T_DEFINED -D_WCHAR_T_DEFINED -D__wchar_t
# DIAB_PSOS_HACKS = -D__Itypes -D_WCHAR_T_DEFINED -D__wchar_t

# ================================================================
# Compilation Settings and Flags
# ================================================================

# turn on debugging
debug = 1

# turn on optimization
optimize = 1

# turn off shared libraries
shared_libs =

# turn on static libraries
static_libs = 1


# C Compiler
CC              = dcc -tMC68060FS:psos
# -WDCDCONFIG=psos.conf

# C Debug Compiler Flags
# -D<name>[=<value>] defines symbol to preprocessor
# -U<name> undefine preprocessor symbol
# -I<path> adds a directory path to the include file search path
# -V[V] display the current version number of D-C++ (this could be
#       useful for crafting a single pSOS config.h file)
# -W is used to pass arguments to specific tools (compiler, linker, etc),
#    to change which tool is used, etc
# -o renames the output (we need to do this to avoid colliding with .i files)
# -X<name | number>[=<value>]gives detailed control of the compiler
#    -Xlint turns on a bunch of useful warning messages
#    -Xno-old-style disables the use of old style C function declarations
#                   and narrows the compiler error messages
#    -Xmnem-emb accepts only Motorola Embedded Mnemonics (assembler directive)
#    -Xkeywords=0x04 defines the inline keyword to be valid
#    -Xkill-opt=0x400 turns off the split optimization.  Per Jim Terman
#                     at DDI, this fixes a known problem
# -Y changes the default search path for include files and libraries
# -t<CPU><object format><floating point support>[:<environment>] is used to
#     describe the target board. The object format is F for ELF, E for COFF,
#     or N for GNU/VxWorks. The floating point support is H for hardware
#     support, S for software support, or N for no floating point support
CFLAGS += -D__DIAB -D__DIAB_42a -Xkill-opt=0x400 -Xno-old-style -Xlint $(PSOSINCL)

# C Compiler Debug Flags (passed iff debug == 1)
# -# prints subprograms with arguments as they are executed
# -## prints subprograms with arguments without executing them
# -### prints subprograms with arguments (in quotes) without executing them
# -g embeds debug info but turns off a bunch of optimizations, including inlining
# -g1 embeds debug info but not line numbers, and preserves inlining
# -v runs the compiler in verbose mode
# -w suppresses warnings
# -H prints path names of all include files to stderr
# -E stops compilation after preprocessing and sends
#    results to stdout
# -P same as -E except it sends it to the source file specified by the -o flag
#    (IMPORTANT: for ACE, some *other* file name must be specified, as the
#     default behavior for -P without the -o option will overwrite *our*
#     .i (inline) file)
# -X<name | number>[=<value>]gives detailed control of the compiler
#    -Xlint turns on a bunch of useful warning messages
#    -Xstop-on-warning treats warnings like errors and stops compilation
#    -Xinit-locals[=<mask>] initializes memory pattern for local variables
#    -Xno-optimized-debug turns off optimizations for debugging
DCFLAGS         += -g -w -Ws
#DCFLAGS         += -g -w -Ws -P -o precomp.out
#DCFLAGS         += -g -w -Ws -H

# C Compiler Optimization Flags (passed iff optimize == 1)
# -O
# -XO
# OCFLAGS         += -XO

# C++ Compiler
CXX             = dplus -tMC68060FS:psos
### This (without the above -t option cannot resolve DOBJECT, etc.)
# CXX             = dplus

# C++ Compiler Flags
# -o renames the output (we need to do this to avoid colliding with .i files)
# -W is used to pass arguments to specific tools (compiler, linker, etc),
#    to change which tool is used, etc
# -X<name | number>[=<value>]gives detailed control of the compiler
#    -Xexception enables exception handling
#    -Xno-implicit-templates produces smaller code, but requires explicit
#                            template instantiation (see pp 91 of D-C++ user guide)
# -Xcomdat (per Jim Terman) is the "preferred" way of instantiating templates as of 4.1a (but, there are problems there with Synch_T.cpp and Synch_T.h)

#CCFLAGS         += $(CFLAGS) $(DIAB_PSOS_HACKS) -Xno-implicit-templates
#CCFLAGS         += $(CFLAGS) $(DIAB_PSOS_HACKS) -Xno-implicit-templates=3
#CCFLAGS         += $(CFLAGS) $(DIAB_PSOS_HACKS) -Xno-implicit-templates=2
#CCFLAGS         += $(CFLAGS) $(DIAB_PSOS_HACKS) -Xcomdat
CCFLAGS         += $(CFLAGS) $(DIAB_PSOS_HACKS)

# C++ Compiler Debug Flags  (passed iff debug == 1)
DCCFLAGS         +=  $(DCFLAGS)

# Dynamic Linker
# DLD             = dld
DLD             = dplus -tMC68060FS:psos -Wm$(PSS_BSP)/ram.lnk

# Static Linker
# LD              = dld
LD              = dplus -tMC68060FS:psos -Wm$(PSS_BSP)/ram.lnk

# Static Linker Flags
# -L<path> adds a directory path to the linker search path
# -l<name> adds a library name to the list the linker uses
# -f<val> fills in holes in an output section with the given 16 bit pattern
# -m generates a link map of the input and output sections on stdout
# -m2 generates a more detailed link map of the input and output sections on stdout
# -r performs incremental link
# -r2 add relocation tables, treat unresolved symbols as errors
# -r3 add relocation tables, treat unresolved symbols as warnings
# -Xcheck-overlap checks for overlap of output sections and sections that wrap around
#     32 bit address boundary
# -Xdont-die forces the linker to continue after errors
# -Xexpl-instantiations causes linker to write source lines of explicit template
#    instantiations to stdout
#LDFLAGS         += -Xdont-die -Xexpl-instantiations
#LDFLAGS         += -Xdont-die
#LDFLAGS         += -Xdont-die
#LDFLAGS         += -Xdont-die -Xexpl-instantiations

# Additional libraries used by the linker
LIBS            +=  $(PSOS_LIBS) $(PSS_BSP_LIB)
#LIBS            +=  $(PSOS_LIBS) $(PSS_BSP_LIB) $(DIAB_LIBS)

# position independent code flag (do we need this ?)
# PIC             = -fpic

# Archive Utility
AR              = dar

# Archive Utility Flags
# -d[lv] delete the named files from the archive
# -m[abiv] move the named files
# -r[abciluv] replace the named files in the archive
ARFLAGS         = -r

RANLIB          = echo

# Shared Object Library Stuff (do we need this ?)
# SOFLAGS         = -G $(CPPFLAGS)
# SOBUILD - compile into .so directly
# SOBUILD         = $(COMPILE.cc) $(PIC) -o $(VSHDIR)$*.so $<

# Unidentified Stuff
# PRELIB          = @true
