dnl
dnl An autoconf script to automatically configure ACE.
dnl Process this file with autoconf to produce a configure script.
dnl

dnl @todo PACKAGE and VERSION should be static, not computed by the
dnl       shell.  Or M4 should be used to compute if necessary.
dnl AC_INIT(["$PACKAGE"], ["$ACE_VERSION"], [ace-bugs@cs.wustl.edu],[ace])
AC_INIT([ACE],[TEST-VERSION],[ace-bugs@cs.wustl.edu],[ace])


AC_REVISION([$Id$])

AC_COPYRIGHT([ACE(TM) and TAO(TM) are copyrighted by Douglas C. Schmidt and
his research group at Washington University, University of California,
Irvine, andVanderbilt University Copyright (c) 1993-2003, all rights
reserved.  Since ACE and TAO are open-source, free software, you are
free to use, modify, and distribute the ACE and TAO source code and
object code produced from the source, as long as you include this
copyright statement along with code built using ACE and TAO.

Please see the file `COPYING' in the top level ACE directory for
additional details.])


dnl Require GNU Autoconf 2.57 or better.
AC_PREREQ([2.57])

AC_CONFIG_SRCDIR([ace/ACE.cpp])

dnl Check what platform we are running on.
AC_CANONICAL_TARGET([])

dnl Initialize GNU Automake, and require Automake 1.7 or better.
AM_INIT_AUTOMAKE([1.7])

changequote(, )dnl
ACE_VERSION=`head -1 ${srcdir}/VERSION | sed 's/.*version \([0-9.]*\).*/\1/'`
changequote([, ])dnl



dnl Add maintainer mode option to the option list.
dnl AM_MAINTAINER_MODE

dnl The maintainer of this configure script.
ACE_CONFIGURE_MAINTAINER='ace-users@cs.wustl.edu'


dnl Until autoconf support in ACE is complete, prevent this script
dnl from running unless the user explictly forces the configure script
dnl to run using the "--enable-maintainer-mode" configure script
dnl option.
dnl if test $USE_MAINTAINER_MODE != yes; then
dnl   AC_MSG_ERROR([
dnl     ACE autoconf support is currently disabled by default since it is
dnl     still under development.  Please use the stock ACE build procedure
dnl     detailed in the file \`ACE-INSTALL.html'.
dnl
dnl     If you wish to experiment with ACE's autoconf support then use the
dnl     \"--enable-maintainer-mode\" configure script option to enable
dnl     autoconf support.  For more details see the file
dnl     \`ACE-configuration.txt'.])
dnl fi  dnl  test $USE_MAINTAINER_MODE != yes

dnl Should we use "egrep" or "grep -E"?  This sets the "$EGREP" shell
dnl variable.
AC_PROG_EGREP

dnl If we are configuring in a CVS controlled directory then don't
dnl continue any further.  The idea is to prevent automatically
dnl generated files from being checked into the repository.  This
dnl will prevent accidental overwrites of ACE's current Makefiles by
dnl the automatically generated ones, for example.
dnl ACE_CHECK_FOR_CVS_DIR

dnl Prevent the configure script from continuing any further if
dnl configuration is being performed in the top-level directory.  The
dnl idea is to prevent files generated during configuration and build
dnl from overwriting the stock files of the same name.
ACE_CHECK_TOP_SRCDIR

dnl Prepare the `ace/config.h.in' header template.
ACE_PREP_CONFIG_HEADER

dnl Allow the standard program name transformations.
dnl We probably don't need AC_ARG_PROGRAM any longer since AM_INIT_AUTOMAKE
dnl handles this functionality.  -- Ossama
dnl AC_ARG_PROGRAM

dnl Generate a header file with all settings.
AC_CONFIG_HEADERS([ace/config.h])

dnl Move before the AC_ARG_ENABLE stuff to prevent autoconf complaints.
dnl This is a bit messy but it makes life easier for me.
dnl                          -Ossama
dnl
dnl SECTION: checks for programs
dnl

dnl Check if system supports "#! /bin/sh" line in scripts
AC_SYS_INTERPRETER

dnl Check the C compiler and preprocessor.
AC_PROG_CC
AC_PROG_CPP
AC_PROG_CC_C_O

dnl Check the C++ compiler and preprocessor.
AC_PROG_CXX
AC_PROG_CXXCPP

dnl Set the test language as C++
AC_LANG([C++])

dnl If we are cross compiling disable certain things in the Makefiles.
AM_CONDITIONAL([ACE_CROSS_COMPILED], [test X$cross_compiling = Xyes])


dnl This checks for a binary called gm4, gnum4 or m4, and assigns
dnl whatever was found (or "gonzo") to the M4 macro.
dnl AC_CHECK_PROGS(M4, gm4 gnum4 m4, gonzo)

dnl Look for the best awk-style program available.
AC_PROG_AWK

dnl Set some version information
ACE_CURRENT=0
ACE_AGE=0
ACE_REVISION=0

if test -n "$AWK"; then
  ACE_MAJOR=`echo "$ACE_VERSION" | sed 's/\./ /g' | $AWK '{ print \[$]1 }'`
  ACE_MINOR=`echo "$ACE_VERSION" | sed 's/\./ /g' | $AWK '{ print \[$]2 }'`
  ACE_BETA=`echo "$ACE_VERSION" | sed 's/\./ /g' | $AWK '{ print \[$]3 }'`

dnl Make sure all of the versions have a legitimate value, i.e. not an
dnl empty string.
  test -z "$ACE_MAJOR" && ACE_MAJOR=0
  test -z "$ACE_MINOR" && ACE_MINOR=0
  test -z "$ACE_BETA" && ACE_BETA=0

dnl This is a terrible kludge to fool libtool into making it produce a shared
dnl library with the "right" version information.
dnl We really should switch to a version scheme that is compatible with
dnl libtool.
dnl               -Ossama
ACE_CURRENT=`expr $ACE_MAJOR + $ACE_MINOR`
ACE_AGE=$ACE_MINOR
ACE_REVISION=$ACE_BETA

fi

AC_SUBST([ACE_CURRENT])
AC_SUBST([ACE_AGE])
AC_SUBST([ACE_REVISION])

dnl Do the usual install settings; don't forget to include a
dnl `install-sh' script, in case there is no BSD compatible `install'
dnl installed (no pun intended) in your machine.

dnl We don't need this anymore since AM_INIT_AUTOMAKE calls AC_PROG_INSTALL.
dnl       -- Ossama
dnl AC_PROG_INSTALL

dnl Special handling for some UNIX variants and Cygwin32
dnl AC_AIX

dnl AC_MINIX
AC_DIAGNOSE([obsolete],[AC_CYGWIN is obsolete: use AC_CANONICAL_HOST and $host_os])
case $host_os in
  *cygwin* ) CYGWIN=yes;;
         * ) CYGWIN=no;;
esac


dnl Check if we support symlinks
AC_PROG_LN_S

dnl Check if a lexical analyzer exists (lex, flex, etc.)
AM_PROG_LEX

dnl Check if some implementation of YACC exists (yacc, byacc, bison, etc.)
AC_PROG_YACC
dnl if test -z "$YACC"; then
dnl   ./missing yacc
dnl fi

dnl Check for perfect hash function generator
AC_CHECK_PROG([GPERF],[gperf],[gperf])

dnl Check for profiling progam
AC_CHECK_PROGS([PROF],[gprof prof],)

dnl The user's/default C++ flags are stored in "CXXFLAGS."  We use
dnl the variable "ACE_CXXFLAGS" to set the C++ flags we want.  At the end
dnl of the configuration process we combine ACE_CXXFLAGS and CXXFLAGS
dnl into CXXFLAGS (e.g., CXXFLAGS="$ACE_CXXFLAGS $CXXFLAGS").  CXXFLAGS
dnl goes after ACE_CXXFLAGS so that the user's C++ flag command line
dnl choices always override the configure script's choices.
ACE_CXXFLAGS=""
ACE_CFLAGS=""



dnl SECTION 2: Configure script command line options


dnl Determine which subsets to build
dnl This is done using the autoconf "--enable-foobar" mechanism.
ACE_CHECK_SUBSETS

dnl Add --{enable,disable,with,without}-feature options.
ACE_CONFIGURATION_OPTIONS
ACE_COMPILATION_OPTIONS

dnl Configure the TAO directory if it exists.
dnl TAO_CONFIGURATION_OPTIONS

# Autoconf's AC_OBJEXT and AC_EXEEXT macros only works for C compilers!
# Libtool's setup macro calls AC_OBJEXT and AC_EXEEXT without setting
# the test language to C.  We do it before any libtool setup macros are
# called so that the proper values are cached beforehand.  We also do
# it before any linker flags (LDFLAGS) are set so that C++ specific
# ones don't break the tests.
dnl AC_LANG_PUSH([C])
dnl AC_OBJEXT
dnl AC_EXEEXT
dnl AC_LANG_POP([C])

dnl Call ACE_SET_COMPILER_FLAGS before AC_PROG_LIBTOOL and after the
dnl AC_ARG_ENABLE and AC_ARG_WITH calls.
ACE_SET_COMPILER_FLAGS


dnl SECTION 3: check for programs <--- moved before section 2 (Ossama)


dnl Platform specific libraries needed for ACE's autoconf tests
dnl that currently do not have tests themselves.
dnl Platform specific flags
case "$host" in
  *freebsd*)
    LIBS="$LIBS -lcompat"
    ;;
  *osf3.2*)
    LIBS="$LIBS -lmach -lsys5 -lcxx -lc"
    ;;
  *osf4.0* | *osf5.0*)
    LIBS="$LIBS -lmach"
    ;;
  *psos*)
    LIBS="$LIBS -lm"
    ;;
esac


dnl SECTION 4: checks for libraries


dnl Additional X library checks
dnl We only check for these libraries if the user has
dnl enabled XtReactor support.

xt_reactor_go=no

if test "$ace_user_enable_xt_reactor" = yes; then
XTREACTOR_TEST_XLIBS=""
dnl Check for Motif if we have X
  T_LDFLAGS="$LDFLAGS"
  LDFLAGS="$LDFLAGS $X_LIBS"

dnl Note that ACE currently only needs -lX11 and -lXt for the XtReactor
dnl so we define another library variable that contains additional
dnl libraries for the XtReactor test since it needs either Motif or the
dnl Athena widget set.

  AC_CHECK_LIB([Xm],[XmCreateRowColumn],
    [
     AC_DEFINE([ACE_HAS_XT])
     XTREACTOR_TEST_XLIBS="-lXm"
     xt_reactor_go=yes
    ],
    [
     AC_DEFINE([ACE_LACKS_MOTIF])
     AC_CHECK_LIB([Xaw],[XawInitializeWidgetSet],
       [
        AC_DEFINE([ACE_HAS_XT])
        XTREACTOR_TEST_XLIBS="-lXaw -lXmu"
        xt_reactor_go=yes
       ],
       [
        xt_reactor_go=no
        AC_MSG_WARN([No usable X widget libraries were found.])
        AC_MSG_WARN([XtReactor support will be disabled.])
       ],[-lXmu])
    ],[-lXt])

  AC_SUBST([XTREACTOR_TEST_XLIBS])

dnl Restore pre-test linker flags
  LDFLAGS="$T_LDFLAGS"

fi dnl test "$ace_user_enable_xt_reactor"= yes

  AM_CONDITIONAL([COMPILE_XTREACTOR_TEST],[test X$xt_reactor_go = Xyes])

dnl End additional X library checks

dnl Some platforms do not have a dynamic linking library, however the
dnl dlopen, dlclose, etc., functions may exist in the C library.
dnl (e.g. Digital UNIX)
dnl Check for dynamic linking library
AC_SEARCH_LIBS([dlopen],[dl svld],[ace_has_svr4_dynamic_linking=yes],
  [
   ace_has_svr4_dynamic_linking=no
   AC_CHECK_LIB([dld],[shl_get],,)
  ])

dnl Check if the socket library is available
AC_SEARCH_LIBS([socket],[socket],,,[-lnsl])

dnl Check for gethostbyname in -lnsl since some platforms (e.g. Solaris)
dnl put it there.
AC_SEARCH_LIBS([gethostbyname],[nsl],,)

dnl Check for getservbyname in -lxnet since some platforms (e.g. Solaris)
dnl may put it there.
AC_SEARCH_LIBS([getservbyname],[socket xnet],,[AC_DEFINE([ACE_LACKS_GETSERVBYNAME])],[-lnsl])


dnl Check for compile() regex function in -lgen.  Solaris, for example,
dnl may put it there.
AC_SEARCH_LIBS([compile],[gen],,)

dnl Check for exception handling library (e.g. for Digital UNIX)
AC_SEARCH_LIBS([exc_continue],[exc],,)

dnl Check for ctime_r in -lc_r.  Some platforms, such as Digital UNIX,
dnl put reentrant functions such as asctime_r, ctime_r, gmtime_r, and
dnl localtime_r in -lc_r.
AC_SEARCH_LIBS([ctime_r],[c_r],,)

dnl TLI library check
AC_SEARCH_LIBS([t_accept],[xti tli_r tli nsl],
  [ace_has_tli_funcs=yes],[ace_has_tli_funcs=no])

dnl Check for all of the things we need to compile and link threads
dnl properly.
if test "$ace_user_enable_threads" = yes; then
  ACE_CHECK_THREADS
fi dnl test "$ace_user_enable_threads" = yes

dnl Setup Libtool

dnl This should be done in the "programs" section of this file but
dnl libtool may then be unaware of compiler flags set during the
dnl thread checks.

dnl Disable building of static libraries by default
AC_DISABLE_STATIC

dnl Enable Libtool module support
AC_LIBTOOL_DLOPEN

dnl FIXME: Temporary hack to make libtool work with g++.
dnl        Shared library support will only work with GNU g++ and GNU ld
dnl        right now.
dnl save_CC="$CC"
dnl CC="$CXX"

dnl Check for libtool and turn on Automake processing for Libtool
AC_PROG_LIBTOOL

dnl Enable C++ support in libtool
dnl AC_LIBTOOL_CXX

dnl Temporary hack until I get integrate libtool's new tag support
dnl into automake.
dnl This hack forces libtool to always use the C++ tag.
dnl LIBTOOL="$LIBTOOL --tag=CXX"

dnl Check for sched_yield() in posix4 library.
dnl Some platforms, such as Solaris, may define sched_yield() there.
dnl Later we run AC_CHECK_FUNC(sched_yield), which is redundant in this case
dnl but is needed if sched_yield() is defined in one of the other libraries
dnl we check for.
AC_SEARCH_LIBS([sched_yield],[rt posix4],[ace_has_sched_yield=yes],)

dnl Check for asynchronous IO calls (perform check *after* thread check!)
ACE_CHECK_ASYNCH_IO

dnl Additional `-lposix4' library check since it may not be added by the
dnl above checks on some platforms that may need it
AC_SEARCH_LIBS([clock_gettime],
  [rt posix4],[AC_DEFINE(ACE_HAS_CLOCK_GETTIME)],)


dnl SECTION 5: checks for header files

dnl Set known platform specific flags
ACE_SET_PLATFORM_MACROS

dnl Check for dirent headers
AC_HEADER_DIRENT

if test "$ac_cv_header_dirent_dirent_h" = yes ||
   test "$ac_cv_header_dirent_sys_ndir_h" = yes ||
   test "$ac_cv_header_dirent_sys_dir_h" = yes ||
   test "$ac_cv_header_dirent_ndir_h" = yes; then
  AC_DEFINE([ACE_HAS_DIRENT])
fi

dnl Check for sys/wait.h Posix.1 compliance
AC_HEADER_SYS_WAIT

if test "$ac_cv_cygwin" = yes; then
  AC_CHECK_HEADER([cygwin32/socket.h],
     [AC_DEFINE([ACE_HAS_CYGWIN32_SOCKET_H])],)
fi

AC_CHECK_HEADER([dlfcn.h],
  [
   dnl We already checked for dlopen in the previous library checks however,
   dnl it is possible that ac_cv_func_dlopen=yes if dlopen wasn't found before
   dnl the library test.  Hence we cannot use AC_CHECK_FUNC(dlopen) here
   dnl the previously cached value may prevent ACE_HAS_SVR4_DYNAMIC_LINKING
   dnl from being defined.
   dnl                          -Ossama
   if test "$ace_has_svr4_dynamic_linking" = yes; then
     AC_DEFINE([ACE_HAS_SVR4_DYNAMIC_LINKING])
   fi
  ],)

AC_CHECK_HEADER([malloc.h],
  ,
  [AC_DEFINE([ACE_LACKS_MALLOC_H])])

AC_CHECK_HEADER([memory.h],
  ,
  [AC_DEFINE([ACE_LACKS_MEMORY_H])])

AC_CHECK_HEADER([bytesex.h],
  [AC_DEFINE([ACE_HAS_BYTESEX_H])])

dnl We don't need to explicitly include this on linux.
dnl AC_CHECK_HEADER([features.h],
dnl  [AC_DEFINE([ACE_HAS_FEATURES_H])])

AC_CHECK_HEADER([sys/msg.h],
  [
   ACE_CACHE_CHECK([if _KERNEL is needed for msg prototypes],
     [ace_cv_lib_broken_msg_h],
     [
      AC_COMPILE_IFELSE([AC_LANG_PROGRAM([[
#ifdef UNIXWARE_7_1
#  define _KMEMUSER
#endif

#include <sys/msg.h>
        ]],[[
         struct msg ace_msg;
        ]])],[
         ace_cv_lib_broken_msg_h=no
        ],[
         AC_COMPILE_IFELSE([AC_LANG_PROGRAM([[
#ifndef _KERNEL
# define _KERNEL
# ifdef UNIXWARE_7_1
#   define _KMEMUSER
# endif
#endif
#include <sys/msg.h>
           ]],
           [[
            struct msg ace_msg;
           ]])],
           [
            ace_cv_lib_broken_msg_h=yes
           ],
           [
            dnl If we get here, then we have no idea if it is broken or not.
            ace_cv_lib_broken_msg_h=no
           ])
        ])
     ],
     [
      AC_DEFINE([ACE_HAS_BROKEN_MSG_H])
     ],)
  ],
  [AC_DEFINE([ACE_LACKS_SYSV_MSG_H])])

AC_CHECK_HEADER([sys/param.h],
  [],
  [AC_DEFINE([ACE_LACKS_PARAM_H])])

AC_CHECK_HEADER([sys/priocntl.h],[],[])

dnl Check for <ucontext.h> _before_ <sys/procfs.h>
AC_CHECK_HEADER([ucontext.h],
  [],
  [AC_DEFINE([ACE_LACKS_UCONTEXT_H])])

AC_CHECK_HEADER([sys/procfs.h],
  [
   dnl Check if <sys/procfs.h> conflicts with <ucontext.h>
   dnl Some (early?) versions of glibc2.1 define the same variables
   dnl in <sys/procfs.h> and <ucontext.h>.
   ACE_CACHE_CHECK([if sys/procfs.h conflicts with ucontext.h],
     [ace_has_procfs_conflict],
     [
      AC_COMPILE_IFELSE([AC_LANG_PROGRAM([[
#ifndef ACE_LACKS_UCONTEXT_H
# include <ucontext.h>
#endif

#include <sys/procfs.h>
        ]],[[
         int a = 0;
        ]])],[
         ace_has_procfs_conflict=no
        ],[
         ace_has_procfs_conflict=yes
        ])
     ],
     [
     ],
     [
      dnl If ace_has_procfs_conflict = no then define ACE_HAS_PROC_FS.
      AC_DEFINE([ACE_HAS_PROC_FS])
     ])
  ],)

AC_CHECK_HEADER([sys/select.h],
  [AC_DEFINE([ACE_HAS_SELECT_H])])

AC_CHECK_HEADER([netinet/tcp.h],
  [],
  [AC_DEFINE([ACE_LACKS_TCP_H])])

AC_CHECK_HEADER([sys/sockio.h],
  [AC_DEFINE([ACE_HAS_SOCKIO_H])])

AC_CHECK_HEADER([sysent.h],
  [AC_DEFINE([ACE_HAS_SYSENT_H])],)

AC_CHECK_HEADER([sys/systeminfo.h],[],[])

AC_CHECK_HEADER([sys/filio.h],
  [AC_DEFINE([ACE_HAS_SYS_FILIO_H])],)

AC_CHECK_HEADER([sys/uio.h],,)

AC_CHECK_HEADER([sys/ioctl.h],
   [AC_DEFINE([ACE_HAS_SYS_IOCTL_H])],)

AC_CHECK_HEADER([sys/types.h],
  ,
  [AC_DEFINE([ACE_LACKS_SYS_TYPES_H])])

AC_CHECK_HEADER([sys/termios.h],
  [
   AC_EGREP_CPP([ACE_WITH_TERM_IOCTLS],
      [
#include <sys/termios.h>

#if defined (ACE_HAS_SYS_IOCTL_H)
# include <sys/ioctl.h>
#endif

#if defined(TCGETS) && defined(TCSETS)
          ACE_WITH_TERM_IOCTLS
#elif defined(TCGETA) && defined(TCSETA)
          ACE_WITH_TERM_IOCTLS
#endif
      ],
      [
       AC_DEFINE([ACE_HAS_TERM_IOCTLS])
      ],)
  ],)

if test "$ace_has_tli_funcs" = yes; then
AC_CHECK_HEADER([xti.h],
  [
   ace_has_tli=yes
   AC_DEFINE([ACE_HAS_XTI])
   AC_DEFINE([ACE_HAS_TLI])
  ],)

AC_CHECK_HEADER([sys/xti.h],
  [
   ace_has_tli=yes
   AC_DEFINE([ACE_HAS_SYS_XTI_H])
   AC_DEFINE([ACE_HAS_XTI])
   AC_DEFINE([ACE_HAS_TLI])
  ],)

AC_CHECK_HEADER([sys/timod.h],
  [
   AC_DEFINE([ACE_HAS_TIMOD_H])
  ],
  [
   AC_CHECK_HEADER([tli/timod.h],
     [
      AC_DEFINE([ACE_HAS_OSF_TIMOD_H])
     ],)
  ])

AC_CHECK_HEADER([tiuser.h],
  [
   ace_has_tli=yes
   AC_DEFINE([ACE_HAS_TIUSER_H])
   AC_DEFINE([ACE_HAS_TLI])
  ],)

dnl Check if XTI headers are broken
if test "$ac_cv_header_xti_h" = yes ||
   test "$ac_cv_header_sys_xti_h"  = yes ||
   test "$ac_cv_header_tiuser_h" = yes; then
  ACE_CACHE_CHECK([if TCP macros are defined in XTI headers],
    [ace_cv_lib_has_broken_xti_macros],
    [
     AC_EGREP_CPP([ACE_BROKEN_XTI_MACROS],
       [
#ifndef ACE_LACKS_SYS_TYPES_H
# include <sys/types.h>
#endif

#if defined (ACE_HAS_TIUSER_H) || defined (ACE_HAS_XTI)
#  if defined (ACE_HAS_TIUSER_H)
#    include /**/ <tiuser.h>
#  elif defined (ACE_HAS_SYS_XTI_H)
#    include /**/ <sys/xti.h>
#  else
#    include /**/ <xti.h>
#  endif /* ACE_HAS_SYS_XTI_H */
#endif /* ACE_HAS_TIUSER_H || ACE_HAS_XTI */

#if defined (TCP_NODELAY) || defined (TCP_MAXSEG)
ACE_BROKEN_XTI_MACROS
#endif  /* TCP_NODELAY || TCPMAXSEG  */
       ],
       [
        ace_cv_lib_has_broken_xti_macros=yes
       ],
       [
        ace_cv_lib_has_broken_xti_macros=no
       ])
    ],
    [
     AC_DEFINE([ACE_HAS_BROKEN_XTI_MACROS])
    ],)
fi

if test "$ac_cv_header_tiuser_h" = yes; then
  ACE_CACHE_CHECK([if tiuser.h is protected by extern \"C\"],
      [ace_cv_lib_tiuser_with_extern_c],[
        AC_EGREP_HEADER([extern \"C\"],[tiuser.h],
          [
           ace_cv_lib_tiuser_with_extern_c=yes
          ],
          [
           ace_cv_lib_tiuser_with_extern_c=no
          ])
      ],,[AC_DEFINE([ACE_HAS_TIUSER_H_BROKEN_EXTERN_C])])
fi dnl test "$ac_cv_header_tiuser_h" = yes

AC_CHECK_HEADER([xliuser.h],
  [
   ace_has_tli=yes
   AC_DEFINE([ACE_HAS_XLI])
   AC_DEFINE([ACE_HAS_TLI])
  ],)


dnl Check for TLI prototypes
if test "$ace_has_tli" = yes; then
 ACE_CACHE_CHECK([for TLI prototypes],
  [ace_cv_lib_tli_prototypes],
  [
dnl We only check for t_accept.  This should hopefully be enough.
   AC_EGREP_CPP([t_accept],
     [
#ifndef ACE_LACKS_SYS_TYPES_H
# include <sys/types.h>
#endif

#if defined (ACE_HAS_TIMOD_H)
# include <sys/timod.h>
#endif

#if defined (ACE_HAS_OSF_TIMOD_H)
# include <tli/timod.h>
#endif

#if defined (ACE_HAS_TIUSER_H) || defined (ACE_HAS_XTI)
#  if defined (ACE_HAS_BROKEN_XTI_MACROS)
#    undef TCP_NODELAY
#    undef TCP_MAXSEG
#  endif /* ACE_HAS_BROKEN_XTI_MACROS */
#  if defined (ACE_HAS_TIUSER_H)
#    include /**/ <tiuser.h>
#  elif defined (ACE_HAS_SYS_XTI_H)
#    define class ace_xti_class
#    include /**/ <sys/xti.h>
#    undef class
#  else
#    include /**/ <xti.h>
#  endif /* ACE_HAS_TIUSER_H */
#endif /* ACE_HAS_TIUSER_H || ACE_HAS_XTI */

#if defined (ACE_HAS_XLI)
# include <xliuser.h>
#endif
     ],
     [
      ace_cv_lib_tli_prototypes=yes
     ],
     [
      ace_cv_lib_tli_prototypes=no
     ])
  ],[AC_DEFINE([ACE_HAS_TLI_PROTOTYPES])],)

dnl Check for t_errno type in TLI headers
 ACE_CACHE_CHECK([for t_errno in TLI headers],
  [ace_cv_lib_has_t_errno],
  [
   dnl Check if t_errno is declared in the TLI headers
   AC_EGREP_CPP([t_errno],
     [
#ifndef ACE_LACKS_SYS_TYPES_H
# include <sys/types.h>
#endif

#if defined (ACE_HAS_TIMOD_H)
# include <sys/timod.h>
#endif

#if defined (ACE_HAS_OSF_TIMOD_H)
# include <tli/timod.h>
#endif

#if defined (ACE_HAS_TIUSER_H) || defined (ACE_HAS_XTI)
#  if defined (ACE_HAS_BROKEN_XTI_MACROS)
#    undef TCP_NODELAY
#    undef TCP_MAXSEG
#  endif /* ACE_HAS_BROKEN_XTI_MACROS */
#  if defined (ACE_HAS_TIUSER_H)
#    include /**/ <tiuser.h>
#  elif defined (ACE_HAS_SYS_XTI_H)
#    define class ace_xti_class
#    include /**/ <sys/xti.h>
#    undef class
#  else
#    include /**/ <xti.h>
#  endif /* ACE_HAS_TIUSER_H */
#endif /* ACE_HAS_TIUSER_H || ACE_HAS_XTI */

#if defined (ACE_HAS_XLI)
# include <xliuser.h>
#endif
     ],
     [
      ace_cv_lib_has_t_errno=yes
     ],
     [
      ace_cv_lib_has_t_errno=no
     ])
  ],,[AC_DEFINE([ACE_LACKS_T_ERRNO])])

dnl Check if t_error incorrectly accepts char *
 ACE_CACHE_CHECK([if t_error incorrectly accepts char *],
  [ace_cv_lib_has_broken_t_error],
  [
   dnl Check if t_errno is declared in the TLI headers
   AC_COMPILE_IFELSE([AC_LANG_PROGRAM([[
#ifndef ACE_LACKS_SYS_TYPES_H
# include <sys/types.h>
#endif

#if defined (ACE_HAS_TIMOD_H)
# include <sys/timod.h>
#endif

#if defined (ACE_HAS_OSF_TIMOD_H)
# include <tli/timod.h>
#endif

#if defined (ACE_HAS_TIUSER_H) || defined (ACE_HAS_XTI)
#  if defined (ACE_HAS_BROKEN_XTI_MACROS)
#    undef TCP_NODELAY
#    undef TCP_MAXSEG
#  endif /* ACE_HAS_BROKEN_XTI_MACROS */
#  if defined (ACE_HAS_TIUSER_H)
#    include /**/ <tiuser.h>
#  elif defined (ACE_HAS_SYS_XTI_H)
#    define class ace_xti_class
#    include /**/ <sys/xti.h>
#    undef class
#  else
#    include /**/ <xti.h>
#  endif /* ACE_HAS_TIUSER_H */
#endif /* ACE_HAS_TIUSER_H || ACE_HAS_XTI */

#if defined (ACE_HAS_XLI)
# include <xliuser.h>
#endif
     ]],[[
      const char *ace_errmsg = "FOO";
      t_error (ace_errmsg);
     ]])],[
      ace_cv_lib_has_broken_t_error=no
     ],[
      ace_cv_lib_has_broken_t_error=yes
     ])
  ],
  [
   AC_DEFINE([ACE_HAS_BROKEN_T_ERROR])
  ],)

fi dnl test "$ace_has_tli" = yes
fi dnl test "$ace_has_tli_funcs" = yes

AC_CHECK_HEADER([sys/time.h],
  [
   dnl This is a weak test.
   AC_EGREP_HEADER([sys/time.h],[time.h],
     ,
      [
       AC_DEFINE([ACE_LACKS_SYSTIME_H])
      ])
  ],)

dnl See the notes about ACE_LACKS_MMAP in the functions section of this
dnl configure script.
dnl                       -Ossama
AC_CHECK_HEADER([sys/mman.h],
  [
   AC_EGREP_HEADER([extern \"C\"],[sys/mman.h],
     ,
      [
       AC_DEFINE([ACE_HAS_BROKEN_MMAP_H])
      ])
  ],
  [
   AC_DEFINE([ACE_LACKS_MMAP])
  ])

dnl Check for bzero() prototype if bstring.h exists.
AC_CHECK_HEADER([bstring.h],
  [
   AC_EGREP_HEADER([bzero],[bstring.h],
     [
      AC_DEFINE([ACE_HAS_BSTRING])
     ],)
  ],)

AC_CHECK_HEADER([strings.h],
  [
   AC_EGREP_HEADER([bzero],[strings.h],
     [
      AC_DEFINE([ACE_HAS_STRINGS])
     ],)
  ],)

AC_CHECK_HEADER([sys/syscall.h],
  [AC_DEFINE([ACE_HAS_SYSCALL_H])],)

AC_CHECK_HEADER([poll.h],
  [AC_DEFINE([ACE_HAS_POLL])],)

dnl We want AC_CHECK_HEADERS so that "HAVE_PWD_H" gets defined if we have it.
dnl "HAVE_PWD_H" is used in one of the tests later on.
AC_CHECK_HEADERS([pwd.h],,)

AC_CHECK_HEADER([regexpr.h],
  [AC_DEFINE([ACE_HAS_REGEX])],)

AC_CHECK_HEADER([stropts.h],
  [AC_DEFINE([ACE_HAS_STREAMS])],)

AC_CHECK_HEADER([siginfo.h],
  ,
  [AC_DEFINE([ACE_LACKS_SIGINFO_H])])

AC_CHECK_HEADER([unistd.h],
  ,
  [AC_DEFINE([ACE_LACKS_UNISTD_H])])

AC_CHECK_HEADER([utime.h],
  [AC_DEFINE([ACE_HAS_UTIME])],)

dnl This check will be disabled until we can get ACE to compile when it
dnl ACE_HAS_WCHAR is defined on UN*X systems
dnl AC_CHECK_HEADER([wchar.h],
dnl   [AC_DEFINE([ACE_HAS_WCHAR])],)

AC_CHECK_HEADER([libc.h],
  [AC_DEFINE([ACE_HAS_LIBC_H])])

AC_CHECK_HEADER([osfcn.h],
  [AC_DEFINE([ACE_HAS_OSFCN_H])])

AC_CHECK_HEADER([new],
  [AC_DEFINE([ACE_HAS_NEW_NO_H])],
  [
   AC_CHECK_HEADER([new.h],
    [AC_DEFINE([ACE_HAS_NEW_H])])
  ])

AC_CHECK_HEADER([stdexcept],
  [AC_DEFINE([ACE_HAS_STDEXCEPT_NO_H])],
  [
   AC_CHECK_HEADER([exception.h],
     [AC_DEFINE([ACE_HAS_EXCEPTION_H])])
  ])

dnl ace/OS.i can #include <cstring>,not #include <cstring.h>
dnl "cstring" is the correct form.
dnl TODO: Double check the above comment.
AC_CHECK_HEADER([cstring],
  [AC_DEFINE([ACE_HAS_GNU_CSTRING_H])],)

AC_CHECK_HEADER([memory],,)

dnl Check for availablity of "new style" C++ stream headers
AC_CHECK_HEADERS([iomanip ios iostream istream ostream fstream streambuf],
  ,
  [AC_DEFINE([ACE_USES_OLD_IOSTREAMS])])

AC_CHECK_HEADER([iostream.h],
  [AC_CHECK_HEADER([fstream.h],,[AC_DEFINE([ACE_LACKS_IOSTREAM_TOTALLY])])],
  [AC_DEFINE([ACE_LACKS_IOSTREAM_TOTALLY])])

dnl Check if platform needs to #include <regexpr.h> for
dnl regular expression support
ACE_CACHE_CHECK([regexpr.h for regular expression support],
   [ace_cv_needs_regexpr_h],
   [
    AC_EGREP_HEADER([compile],[regexpr.h],
     [
      ace_cv_needs_regexpr_h=yes
     ],
     [
      AC_EGREP_HEADER([step],[regexpr.h],
        [
         ace_cv_needs_regexpr_h=yes
        ],
        [
         ace_cv_needs_regexpr_h=no
        ])
     ])
   ],[AC_DEFINE([ACE_NEEDS_REGEXPR_H])],)

dnl Check if sprintf() returns char*
ACE_CONVERT_WARNINGS_TO_ERRORS([
ACE_CACHE_CHECK([if sprintf() returns char*],
  [ace_cv_lib_charptr_sprintf],
  [
   AC_COMPILE_IFELSE([AC_LANG_PROGRAM([[
#include <stdlib.h>
#include <stdio.h>
    ]],[[
     char *str = 0;
     const char *fmt = "%d";
     int num = 0;

     char *s = 0;

     s = sprintf(str, fmt, num);
    ]])],[
     ace_cv_lib_charptr_sprintf=yes
    ],[
     ace_cv_lib_charptr_sprintf=no
    ])
  ],[AC_DEFINE([ACE_HAS_CHARPTR_SPRINTF])],)
]) dnl ACE_CONVERT_WARNINGS_TO_ERRORS

dnl Check for getopt() prototype
ACE_CACHE_CHECK([for getopt() prototype],[ace_cv_lib_getopt_proto],[
  AC_EGREP_HEADER([getopt],[unistd.h],
    [
     ace_cv_lib_getopt_proto=yes
    ],
    [
     AC_EGREP_HEADER([[^_]+getopt],[stdlib.h],
       [
        ace_cv_lib_getopt_proto=yes
       ],
       [
        ace_cv_lib_getopt_proto=no
       ])
    ])
],,[AC_DEFINE([ACE_LACKS_GETOPT_PROTO])])

dnl Check for old malloc() prototype.
ACE_CONVERT_WARNINGS_TO_ERRORS([
ACE_CACHE_CHECK([for old malloc() prototype],
  [ace_cv_lib_old_malloc_proto],
  [
   AC_COMPILE_IFELSE([AC_LANG_PROGRAM([[
#include <stdlib.h>
#ifndef ACE_LACKS_MALLOC_H
# include <malloc.h>
#endif
    ]],[[
     char *s = 0;
     s = malloc(sizeof(int));
    ]])],[
     ace_cv_lib_old_malloc_proto=yes
    ],[
     ace_cv_lib_old_malloc_proto=no
    ])
  ],[AC_DEFINE([ACE_HAS_OLD_MALLOC])],)
]) dnl ACE_CONVERT_WARNINGS_TO_ERRORS

dnl Check for *_timedwait() prototypes
dnl TODO: We only check for one of the *_timedwait() prototypes.
dnl       Is this enough?
ACE_CACHE_CHECK([for *_timedwait() prototypes],[ace_cv_lib_timedwait_proto],[
  AC_EGREP_HEADER([recv_timedwait],[pthread.h],
    [
     ace_cv_lib_timedwait_proto=yes
    ],
    [
     AC_EGREP_HEADER([recv_timedwait],[fcntl.h],
       [
        ace_cv_lib_timedwait_proto=yes
       ],
       [
        ace_cv_lib_timedwait_proto=no
       ])
    ])
],,[AC_DEFINE([ACE_LACKS_TIMEDWAIT_PROTOTYPES])])

dnl Check for ualarm prototype in unistd.h.
dnl Most Unices seem to put it there (?)
ACE_CACHE_CHECK([for ualarm() prototype],[ace_cv_lib_ualarm_proto],[
   AC_EGREP_HEADER([ualarm],[unistd.h],
                   [
                    ace_cv_lib_ualarm_proto=yes
                   ],
                   [
                    ace_cv_lib_ualarm_proto=no
                   ])
],,[AC_DEFINE([ACE_LACKS_UALARM_PROTOTYPE])])

dnl Check for getrusage prototype in sys/resource.h
ACE_CACHE_CHECK([for getrusage() in sys/resource.h],
    [ace_cv_lib_has_getrusage_proto],[
      AC_EGREP_HEADER([getrusage],[sys/resource.h],
         [
          ace_cv_lib_has_getrusage_proto=yes
         ],
         [
          ace_cv_lib_has_getrusage_proto=no
         ])
],[AC_DEFINE([ACE_HAS_GETRUSAGE_PROTO])],)

dnl Check for {get,set}rlimit prototypes in sys/resource.h
ACE_CACHE_CHECK([for getrlimit() and setrlimit() in sys/resource.h],
    [ace_cv_lib_has_getsetrlimit_proto],[
      AC_EGREP_HEADER([getrlimit],[sys/resource.h],
         [
          AC_EGREP_HEADER([getrlimit],[sys/resource.h],
             [
              ace_cv_lib_has_getsetrlimit_proto=yes
             ],
             [
              ace_cv_lib_has_getsetrlimit_proto=no
             ])
         ],
         [
          ace_cv_lib_has_getsetrlimit_proto=no
         ])
],,[AC_DEFINE([ACE_LACKS_RLIMIT_PROTOTYPE])])

dnl Check for platforms that don't declare dl* functions
dnl as extern "C" in dlfcn.h.
dnl TODO: This check looks feeble to me...
dnl NOTE: Linux header files ARE protected with extern "C" by the
dnl       __BEGIN_DECLS macro, so don't be surprised!
ACE_CACHE_CHECK([if dlfcn.h is protected by extern \"C\"],
    [ace_cv_lib_dlfcn_with_extern_c],[
      AC_EGREP_HEADER([extern \"C\"],[dlfcn.h],
        [
         ace_cv_lib_dlfcn_with_extern_c=yes
        ],
        [
         ace_cv_lib_dlfcn_with_extern_c=no
        ])
],,[AC_DEFINE([ACE_HAS_DLFCN_H_BROKEN_EXTERN_C])])



dnl SECTION 6: Checks for typedefs


dnl dnl Standard typedef checks (All of them may not be needed)
dnl AC_TYPE_UID_T
dnl AC_TYPE_MODE_T
dnl AC_TYPE_OFF_T
dnl AC_TYPE_PID_T
dnl AC_TYPE_SIZE_T

dnl AC_CHECK_TYPE([off64_t],[long long])

dnl Specific typedef checks
dnl TODO: Check whether these typedefs can be defined somewhere else.
ACE_CHECK_TYPE([idtype_t],[signal.h],[AC_DEFINE([ACE_HAS_IDTYPE_T])],)
ACE_CHECK_TYPE([sem_t],[semaphore.h],,)
ACE_CHECK_TYPE([key_t],[sys/types.h],,[AC_DEFINE([ACE_LACKS_KEY_T])])
ACE_CHECK_TYPE([ssize_t],[sys/types.h],[AC_DEFINE([ACE_HAS_SSIZE_T])],)
ACE_CHECK_TYPE([u_longlong_t],[sys/types.h],,[AC_DEFINE([ACE_LACKS_U_LONGLONG_T])])
ACE_CHECK_TYPE([pri_t],[sys/types.h],,[AC_DEFINE([ACE_LACKS_PRI_T])])
ACE_CHECK_TYPE([ucontext_t],[ucontext.h],
  [
   AC_DEFINE([ACE_HAS_UCONTEXT_T])
  ],
  [
   dnl Some platforms define ucontext_t in <sys/ucontext.h>, but ACE
   dnl doesn't explicitly include that header.  However, it is very likely
   dnl that <signal.h> does, either directly or indirectly.
   ACE_CHECK_TYPE([ucontext_t],[signal.h],[AC_DEFINE([ACE_HAS_UCONTEXT_T])])
  ])
ACE_CHECK_TYPE([wchar_t],[wchar.h],,
  [
   ACE_CHECK_TYPE([wchar_t],[sys/types.h],,[AC_DEFINE([ACE_LACKS_WCHAR_T])])
  ])

ACE_CHECK_TYPE([socklen_t],[sys/socket.h],[AC_DEFINE([ACE_HAS_SOCKLEN_T])],
  [
   dnl The compiler in linux just issues a warning, and the test
   dnl passes!!!

   dnl FIXED by adding "-Werror" to compiler flags when using GNU C++
   dnl                                 -Ossama
   ACE_CONVERT_WARNINGS_TO_ERRORS(
   [
    dnl Check if socket size is denoted by size_t
    ACE_CACHE_CHECK([if socket size is denoted by size_t],
      [ace_cv_lib_posix_socket_len_size_t],[
        AC_COMPILE_IFELSE([AC_LANG_PROGRAM([[
#ifndef ACE_LACKS_SYS_TYPES_H
# include <sys/types.h>
#endif
#include <sys/socket.h>
         ]],[[
          int s = 0;
          struct sockaddr* addr = 0;
          int* addrlen = 0;
          accept(s, addr, addrlen);
         ]])],[
          ace_cv_lib_posix_socket_len_size_t=no
         ],[
          dnl Now see if it really does take a size_t socket size
          AC_COMPILE_IFELSE([AC_LANG_PROGRAM(
            [[
#ifndef ACE_LACKS_SYS_TYPES_H
# include <sys/types.h>
#endif
#include <sys/socket.h>
            ]],
            [[
             int s = 0;
             struct sockaddr* addr = 0;
             size_t* addrlen = 0;
             accept(s, addr, addrlen);
            ]])],
            [
             ace_cv_lib_posix_socket_len_size_t=yes
            ],
            [
             ace_cv_lib_posix_socket_len_size_t=no
            ])
         ])
      ],[AC_DEFINE([ACE_HAS_SIZET_SOCKET_LEN])],)
    ])
  ])

dnl Check for sig_atomic_t
ACE_CACHE_CHECK([for sig_atomic_t],
    [ace_cv_lib_posix_defines_sig_atomic_t],[
      AC_COMPILE_IFELSE([AC_LANG_PROGRAM([[
#ifndef ACE_LACKS_SYS_TYPES_H
# include <sys/types.h>
#endif
#include <signal.h>
         ]],[[
          sig_atomic_t sat;
         ]])],[
          ace_cv_lib_posix_defines_sig_atomic_t=yes
         ],[
          ace_cv_lib_posix_defines_sig_atomic_t=no
         ])
],[AC_DEFINE([ACE_HAS_SIG_ATOMIC_T])],)



dnl SECTION 7: checks for structures


dnl TODO: Check whether these structures can be defined somewhere else.
ACE_CHECK_STRUCT([dirent],[dirent.h],,[AC_DEFINE([ACE_LACKS_STRUCT_DIR])])
ACE_CHECK_STRUCT([flock],[fcntl.h],,[AC_DEFINE([ACE_LACKS_FILELOCKS])])
ACE_CHECK_STRUCT([rwlock_t],[synch.h],,[AC_DEFINE([ACE_LACKS_RWLOCK_T])])
ACE_CHECK_STRUCT([strbuf],[stropts.h],[AC_DEFINE([ACE_HAS_STRBUF_T])],)
ACE_CHECK_STRUCT([msgbuf],[sys/msg.h],,[AC_DEFINE([ACE_LACKS_MSGBUF_T])])
case "$host" in
*irix*)
  dnl IRIX prusage fields don't match what ACE currently supports.
  ;;
*)
  ACE_CHECK_STRUCT([prusage_t],[sys/procfs.h],[AC_DEFINE([ACE_HAS_PRUSAGE_T])],)
  ;;
esac
ACE_CHECK_STRUCT([strrecvfd],[stropts.h],,[AC_DEFINE([ACE_LACKS_STRRECVFD])])
ACE_CHECK_STRUCT([sigaction],[signal.h],,[AC_DEFINE([ACE_LACKS_SIGACTION])])
ACE_CHECK_STRUCT([sigset_t],[signal.h],,[AC_DEFINE([ACE_LACKS_SIGSET])])
ACE_CHECK_STRUCT([utsname],[sys/utsname.h],,[AC_DEFINE([ACE_LACKS_UTSNAME_T])])

ACE_CACHE_CHECK([for struct sembuf],[ace_cv_struct_sembuf],
  [
   dnl Some platforms may need to include some headers before <sys/sem.h>.
     AC_COMPILE_IFELSE([AC_LANG_PROGRAM([[
#ifndef ACE_LACKS_SYS_TYPES_H
# include <sys/types.h>
#endif
#include <sys/ipc.h>
#include <sys/sem.h>
       ]],[[
        struct sembuf ace_sembuf;
       ]])],[
        ace_cv_struct_sembuf=yes
       ],[
dnl Some compilers don't like the "struct" but we need the struct for
dnl some platforms to resolve ambiguities between functions and
dnl structures with with the same name.  So, we try the same test but
dnl without "struct" if the above test with "struct" fails.  If both
dnl tests fail, then we can be reasonably sure that we don't have the
dnl structure we are testing for.
        AC_COMPILE_IFELSE([AC_LANG_PROGRAM(
          [[
#ifndef ACE_LACKS_SYS_TYPES_H
# include <sys/types.h>
#endif
#include <sys/ipc.h>
#include <sys/sem.h>
          ]],
          [[
           sembuf ace_sembuf;
          ]])],
          [
           ace_cv_struct_sembuf=yes
          ],
          [
           ace_cv_struct_sembuf=no
          ])
       ])
  ],,[AC_DEFINE([ACE_LACKS_SEMBUF_T])])

dnl Thanks to Konstantinos Margaritis <kmargar@cc.uoa.gr> for pointing out
dnl that struct siginfo_t may also be defined in signal.h
ACE_CACHE_CHECK([for struct siginfo_t],[ace_cv_struct_siginfo_t],
  [
   dnl Since we are checking for siginfo_t in more than one header
   dnl we can't use the ACE_CHECK_STRUCT macro so we have to do things
   dnl manually.
   ACE_TRY_COMPILE_STRUCT([siginfo_t],[siginfo.h],
     [
      ace_cv_struct_siginfo_t=yes
     ],
     [
      ACE_TRY_COMPILE_STRUCT([siginfo_t],[signal.h],
        [
         ace_cv_struct_siginfo_t=yes
        ],
        [
         ace_cv_struct_siginfo_t=no
        ])
     ])
  ],
  [
   AC_DEFINE([ACE_HAS_SIGINFO_T])
  ],)

dnl Some platforms need to include sys/types.h before sys/socket.h
dnl in order for struct msghdr to work.
dnl Check for msghdr structure.
ACE_CACHE_CHECK([for struct msghdr],[ace_cv_struct_msghdr],
    [
     AC_COMPILE_IFELSE([AC_LANG_PROGRAM([[
#ifndef ACE_LACKS_SYS_TYPES_H
# include <sys/types.h>
#endif
#include <sys/socket.h>
       ]],[[
        struct msghdr ace_msghdr;
       ]])],[
        ace_cv_struct_msghdr=yes
       ],[
        ace_cv_struct_msghdr=no
       ])
    ], [AC_DEFINE([ACE_HAS_MSG])],)

ACE_CACHE_CHECK([for condition variable support],[ace_cv_struct_cond_t],
    [
     AC_COMPILE_IFELSE([AC_LANG_PROGRAM([[
#include <pthread.h>
       ]],[[
        pthread_cond_t ace_pthread_cond_t;
       ]])],[
        ace_cv_struct_cond_t=yes
       ],[
        AC_COMPILE_IFELSE([AC_LANG_PROGRAM(
          [[
#include <synch.h>
          ]],
          [[
           cond_t ace_cond_t;
          ]])],
          [
           ace_cv_struct_cond_t=yes
          ],
          [
           ace_cv_struct_cond_t=no
          ])
       ])
  ],,[AC_DEFINE([ACE_LACKS_COND_T])])

dnl Check for struct timespec
ACE_CACHE_CHECK([for POSIX timer structure],
 [ace_cv_lib_posix_timer_struct],
 [
  AC_COMPILE_IFELSE([AC_LANG_PROGRAM([[
#include <time.h>

#if defined(ACE_LACKS_SYSTIME_H)
# include <sys/time.h>
#endif
        ]],[[
         timespec sr;
        ]])],[
         ace_cv_lib_posix_timer_struct=yes
        ],[
         dnl Check if platform uses struct timestruc_t for POSIX timers
         dnl instead of struct timespec.
         AC_COMPILE_IFELSE([AC_LANG_PROGRAM(
           [[
#include <time.h>

#if defined(ACE_LACKS_SYSTIME_H)
# include <sys/time.h>
#endif
           ]],
           [[
            timestruc_t sr;
           ]])],
           [
            ace_cv_lib_posix_timer_struct=yes
            dnl Check for struct timespec in <sys/timers.h>
            ACE_CACHE_CHECK([for struct timespec in sys/timers.h],
              [ace_cv_lib_posix_struct_timespec_broken],[
                AC_COMPILE_IFELSE([AC_LANG_PROGRAM(
                  [[
#include <sys/timers.h>
                  ]],
                  [[
                   timespec sr;
                  ]])],
                  [
                   ace_cv_lib_posix_struct_timespec_broken=yes
                  ],
                  [
                   ace_cv_lib_posix_struct_timespec_broken=no
                  ])
                ],,)
           ],
           [
            ace_cv_lib_posix_timer_struct=no
           ])
        ])
  ],
  [
   AC_DEFINE([ACE_HAS_POSIX_TIME])
   if test "$ace_cv_lib_posix_struct_timespec_broken" = yes; then
     AC_DEFINE([ACE_HAS_BROKEN_POSIX_TIME])
   fi
  ],
  [
   dnl Check for struct timespec in <sys/timers.h>
   ACE_CACHE_CHECK([for struct timespec in sys/timers.h],
     [ace_cv_lib_posix_struct_timespec_broken],[
       AC_COMPILE_IFELSE([AC_LANG_PROGRAM([[
#include <sys/timers.h>
         ]],[[
          timespec sr;
         ]])],[
          ace_cv_lib_posix_struct_timespec_broken=yes
         ],[
          ace_cv_lib_posix_struct_timespec_broken=no
         ])
     ],[AC_DEFINE([ACE_HAS_BROKEN_POSIX_TIME])],)
  ])

dnl Check for typedef timespec_t
dnl TODO: Check whether this typedef can be defined somewhere else.
ACE_CACHE_CHECK([for timespec_t],
    [ace_cv_lib_posix_timespec_t],[
      AC_COMPILE_IFELSE([AC_LANG_PROGRAM([[
#include <time.h>

#if defined(ACE_LACKS_SYSTIME_H)
# include <sys/time.h>
#endif
        ]],[[
         timespec_t tt;
        ]])],[
         ace_cv_lib_posix_timespec_t=yes
        ],[
         ace_cv_lib_posix_timespec_t=no
        ])
],,[AC_DEFINE([ACE_LACKS_TIMESPEC_T])])

dnl Check for union semun
ACE_CACHE_CHECK([for union semun],
    [ace_cv_lib_posix_defines_union_semun],[
      AC_COMPILE_IFELSE([AC_LANG_PROGRAM([[
#ifndef ACE_LACKS_SYS_TYPES_H
# include <sys/types.h>
#endif
#include <sys/sem.h>
        ]],[[
/* We could also check if the macro _SEM_SEMUN_UNDEFINED is defined.
   No big deal. */

semun us;
        ]])],[
         ace_cv_lib_posix_defines_union_semun=yes
        ],[
         ace_cv_lib_posix_defines_union_semun=no
        ])
],[AC_DEFINE([ACE_HAS_SEMUN])],)



dnl SECTION 8: checks for variables


dnl Check for broken struct timespec members
ACE_CACHE_CHECK([for broken struct timespec members],
  [ace_cv_lib_broken_timespec],
  [
    AC_COMPILE_IFELSE([AC_LANG_PROGRAM([[
#include <time.h>

#if defined(ACE_LACKS_SYSTIME_H)
# include <sys/time.h>
#endif
        ]],[[
         timespec sr;
         sr.ts_sec  = 0;
         sr.ts_nsec = 0;
        ]])],[
         ace_cv_lib_broken_timespec=yes
        ],[
         ace_cv_lib_broken_timespec=no
        ])
  ],[AC_DEFINE([ACE_HAS_BROKEN_TIMESPEC_MEMBERS])],)

dnl Check for more than two fields in struct rusage
ACE_CACHE_CHECK([for limited struct rusage],
    [ace_cv_lib_limited_rusage],[
        AC_COMPILE_IFELSE([AC_LANG_PROGRAM([[
#include <sys/resource.h>
          ]],[[
           rusage ace_rusage;
           /*
             We just pick three (i.e. > 2) of the fields that
             ACE uses to see if we have a struct rusage that
             has more than two fields.
           */
           ace_rusage.ru_ixrss = 0;
           ace_rusage.ru_idrss = 0;
           ace_rusage.ru_isrss = 0;
          ]])],[
           ace_cv_lib_limited_rusage=no
          ],[
           ace_cv_lib_limited_rusage=yes
          ])
],[AC_DEFINE([ACE_HAS_LIMITED_RUSAGE_T])],)

if test "$ace_cv_struct_siginfo_t" = yes; then
  dnl Check for si_addr member in struct siginfo_t
  ACE_CACHE_CHECK([for si_addr member in struct siginfo_t],
    [ace_cv_lib_posix_si_addr],[
        AC_COMPILE_IFELSE([AC_LANG_PROGRAM([[
#ifndef ACE_LACKS_SIGINFO_H
# include <siginfo.h>
#endif
#include <signal.h>
          ]],[[
           siginfo_t acesig;
           acesig.si_addr = 0;
          ]])],[
           ace_cv_lib_posix_si_addr=yes
          ],[
           ace_cv_lib_posix_si_addr=no
          ])
  ],,[AC_DEFINE([ACE_LACKS_SI_ADDR])])
fi dnl test "$ace_cv_struct_siginfo_t" = yes

dnl Check for sin_len member in struct sockaddr_in
ACE_CACHE_CHECK([for sin_len member in struct sockaddr_in],
    [ace_cv_lib_has_sin_len],[
        AC_COMPILE_IFELSE([AC_LANG_PROGRAM([[
#ifndef ACE_LACKS_SYS_TYPES_H
# include <sys/types.h>
#endif
#include <netinet/in.h>
          ]],[[
           sockaddr_in ace_sin;
           ace_sin.sin_len = sizeof(ace_sin);
          ]])],[
           ace_cv_lib_has_sin_len=yes
          ],[
           ace_cv_lib_has_sin_len=no
          ])
],[AC_DEFINE([ACE_HAS_SIN_LEN])],)

dnl Check for sys_siglist
dnl TODO: Check whether this variable can be defined somewhere else.
dnl [OSSAMA:  Should we use autoconf's AC_CHECK_DECLS([sys_siglist])
dnl  test instead?]
ACE_CACHE_CHECK([for sys_siglist],
    [ace_cv_lib_posix_sys_siglist],[
      AC_COMPILE_IFELSE([AC_LANG_PROGRAM([[
#ifndef ACE_LACKS_UNISTD_H
# include <unistd.h>
#endif
#include <signal.h>
#if !defined (_sys_siglist)
# define _sys_siglist sys_siglist
#endif
        ]],[[
         void* vp = (void*) &_sys_siglist;
        ]])],[
         ace_cv_lib_posix_sys_siglist=yes
        ],[
         ace_cv_lib_posix_sys_siglist=no
        ])
],[AC_DEFINE([ACE_HAS_SYS_SIGLIST])],)

dnl Check for sys_errlist
dnl TODO: Check whether this variable can be defined somewhere else.
ACE_CACHE_CHECK([for sys_errlist],
    [ace_cv_lib_posix_sys_errlist],[
      AC_COMPILE_IFELSE([AC_LANG_PROGRAM([[
#include <stdio.h>
#include <errno.h>
#if !defined (_sys_errlist)
# define _sys_errlist sys_errlist
#endif
        ]],[[
         void* vp = (void*) &_sys_errlist;
        ]])],[
         ace_cv_lib_posix_sys_errlist=yes
        ],[
         dnl Check if sys_errlist is a global variable in a library
         AC_LINK_IFELSE([AC_LANG_PROGRAM(
           [[
#include <stdio.h>
#include <errno.h>

#if !defined (_sys_errlist)
# define _sys_errlist sys_errlist
#endif

extern const char * const _sys_errlist[];
           ]],
           [[
            void* vp = (void*) &_sys_errlist;
           ]])],
           [
            ace_cv_lib_posix_sys_errlist=yes
           ],
           [
            ace_cv_lib_posix_sys_errlist=no
           ])
        ])
    ],[AC_DEFINE([ACE_HAS_SYS_ERRLIST])],)

dnl Check for sys_nerr
dnl TODO: Check whether this variable can be defined somewhere else.
ACE_CACHE_CHECK([for sys_nerr],
  [ace_cv_lib_posix_sys_nerr],[
  AC_COMPILE_IFELSE([AC_LANG_PROGRAM([[
#include <stdio.h>
#include <errno.h>

#if !defined (_sys_nerr)
# define _sys_nerr sys_nerr
#endif
    ]],[[
     void* vp = (void*) &_sys_nerr;
    ]])],[
     ace_cv_lib_posix_sys_nerr=yes
    ],[
     dnl Check if sys_nerr is a global variable in a library
     AC_LINK_IFELSE([AC_LANG_PROGRAM(
       [[
#include <stdio.h>
#include <errno.h>

#if !defined (_sys_nerr)
# define _sys_nerr sys_nerr
#endif

extern int _sys_nerr;
       ]],
       [[
        void* vp = (void*) &_sys_nerr;
       ]])],
       [
        ace_cv_lib_posix_sys_nerr=yes
       ],
       [
        ace_cv_lib_posix_sys_nerr=no
       ])
    ])
  ],
  [
   dnl Do nothing
  ],
  [
   AC_DEFINE([ACE_LACKS_SYS_NERR])
  ])

dnl Save the cache for debugging purposes
AC_CACHE_SAVE


dnl SECTION 9: checks for compiler characteristics


dnl Check if compiler accepts "#pragma once" directive
ACE_CONVERT_WARNINGS_TO_ERRORS([
  ACE_CACHE_CHECK([if compiler accepts \"pragma once\" directive],
    [ace_cv_has_pragma_once],
    [
     AC_COMPILE_IFELSE([AC_LANG_PROGRAM([[
#pragma once
       ]],[[
        int a = 0; /* Put this here so we don't have an empty main(). */
       ]])],[
        ace_cv_has_pragma_once=yes
       ],[
        ace_cv_has_pragma_once=no
       ])
    ],,[AC_DEFINE([ACE_LACKS_PRAGMA_ONCE])])
]) dnl ACE_CONVERT_WARNINGS_TO_ERRORS

dnl If we are using GNU C++, see if it accepts the -pipe compiler flag.
dnl "-pipe" on cygwin32 doesn't seem to work, for example.
if test "$GXX" = yes; then
  PREPIPECXXFLAGS="$CXXFLAGS"
  CXXFLAGS="$CXXFLAGS -pipe"
  PREPIPECFLAGS="$CFLAGS"
  CFLAGS="$CFLAGS -pipe"
  ACE_CACHE_CHECK([if \"-pipe\" compiler flag is supported],
    [ace_cv_feature_gxx_has_pipe],
    [
     AC_COMPILE_IFELSE([AC_LANG_PROGRAM([[]],[[int a = 0;]])],[
        ace_cv_feature_gxx_has_pipe=yes
       ],[
        ace_cv_feature_gxx_has_pipe=no
       ])
    ],
    [
     dnl We don't need to add "-pipe" here since it was already added
     dnl for the test.
     dnl CXXFLAGS="$PREPIPECXXFLAGS -pipe"
     dnl CFLAGS="$PREPIPECFLAGS -pipe"
    ],
    [
     CXXFLAGS="$PREPIPECXXFLAGS"
     CFLAGS="$PREPIPECFLAGS"
    ])
fi

dnl Check to see if we are running on a big endian platform
dnl "ace/Basic_Types.h" should perhaps be modified to take advantage
dnl of the results of this test.
dnl Do not run this test if we are using a cross-compiler.
if test "$cross_compiling" != yes; then
  AC_C_BIGENDIAN
fi

dnl Check type sizes
dnl If we get a size of zero, then the type is unknown to the compiler.

dnl We don't need to check for sizeof(char) right now.  Also conflicts with
dnl ACE definition in Basic_Types.h, so we leave the test out.
if test "$cross_compiling" != yes; then
  dnl AC_CHECK_SIZEOF([char],[1])
  dnl if test "$ac_cv_sizeof_char" != "0"; then
  dnl   AC_DEFINE([ACE_SIZEOF_CHAR],[SIZEOF_CHAR])
  dnl fi
  AC_CHECK_SIZEOF([short])
  if test "$ac_cv_sizeof_short" != 0; then
    AC_DEFINE_UNQUOTED([ACE_SIZEOF_SHORT],[$ac_cv_sizeof_short])
  fi
  AC_CHECK_SIZEOF([int])
  if test $ac_cv_sizeof_int != 0; then
    AC_DEFINE_UNQUOTED([ACE_SIZEOF_INT],[$ac_cv_sizeof_int])
  fi
  AC_CHECK_SIZEOF([long])
  if test $ac_cv_sizeof_long != 0; then
    AC_DEFINE_UNQUOTED([ACE_SIZEOF_LONG],[$ac_cv_sizeof_long])
  fi
  AC_CHECK_SIZEOF([long long])
  if test $ac_cv_sizeof_long_long != 0; then
    AC_DEFINE_UNQUOTED([ACE_SIZEOF_LONG_LONG],[$ac_cv_sizeof_long_long])
  else
    AC_DEFINE([ACE_LACKS_LONGLONG_T])
  fi
  AC_CHECK_SIZEOF([void *])
  if test $ac_cv_sizeof_void_p != 0; then
    AC_DEFINE_UNQUOTED([ACE_SIZEOF_VOID_P],[$ac_cv_sizeof_void_p])
  fi
  AC_CHECK_SIZEOF([float])
  if test $ac_cv_sizeof_float != 0; then
    AC_DEFINE_UNQUOTED([ACE_SIZEOF_FLOAT],[$ac_cv_sizeof_float])
  else
    AC_DEFINE([ACE_LACKS_FLOATING_POINT])
  fi
  AC_CHECK_SIZEOF([double])
  if test $ac_cv_sizeof_double != 0; then
    AC_DEFINE_UNQUOTED([ACE_SIZEOF_DOUBLE],[$ac_cv_sizeof_double])
  else
    AC_DEFINE([ACE_LACKS_FLOATING_POINT])
  fi
  AC_CHECK_SIZEOF([long double])
  if test $ac_cv_sizeof_long_double != 0; then
    AC_DEFINE_UNQUOTED([ACE_SIZEOF_LONG_DOUBLE],[$ac_cv_sizeof_long_double])
  fi

  dnl Set the 64 bit unsigned int typedef
  ACE_UINT64=""
  dnl if test "$ace_cv_type_u_longlong_t" = yes; then
  dnl This doesn't work:  AC_CHECK_SIZEOF([u_longlong_t],[8])
  dnl   if test $ac_cv_sizeof_u_longlong_t = 8; then
  dnl     ACE_UINT64="u_longlong_t"
  dnl     ace_u_long_long_typedef_set=yes
  dnl   fi
  dnl elif test $ac_cv_sizeof_long = 8; then
  if test $ac_cv_sizeof_long = 8; then
    ACE_UINT64="unsigned long"
    ace_u_long_long_typedef_set=yes
  elif test $ac_cv_sizeof_long_long = 8; then
    ACE_UINT64="unsigned long long"
    ace_u_long_long_typedef_set=yes
  else
    ace_u_long_long_typedef_set=no
  fi

  dnl Check for broken "signed char"
  dnl If AC_CHECK_SIZEOF(signed char) returns zero then "signed char"
  dnl is broken.
  AC_CHECK_SIZEOF([signed char],[1])
  if test $ac_cv_sizeof_signed_char = 0; then
   AC_DEFINE([ACE_LACKS_SIGNED_CHAR])
  fi
else
  ace_u_long_long_typedef_set=no
fi dnl test "$cross_compiling" != yes

dnl Other checks


dnl Check for istream operator>> for char, unsigned char and signed char
ACE_CACHE_CHECK([for istream operator>> for char types],
  [ace_cv_feature_char_right_shifts],
  [
   AC_COMPILE_IFELSE([AC_LANG_PROGRAM([[
#include <iostream.h>
                  ]],[[
unsigned char a = 0;
cin >> a;

#ifndef ACE_LACKS_SIGNED_CHAR
signed char b = 0;
cin >> b;
#endif
                  ]])],[
                   ace_cv_feature_char_right_shifts=yes
                  ],[
                   ace_cv_feature_char_right_shifts=no
                  ])
  ],,[AC_DEFINE([ACE_LACKS_CHAR_RIGHT_SHIFTS])])


dnl Check for istream operator>> for char *, unsigned char * and signed char *
ACE_CACHE_CHECK([for istream operator>> for char * types],
  [ace_cv_feature_char_ptr_right_shifts],
  [
   AC_COMPILE_IFELSE([AC_LANG_PROGRAM([[
#include <iostream.h>
                  ]],[[
unsigned char * a = 0;
cin >> a;

#ifndef ACE_LACKS_SIGNED_CHAR
signed char * b = 0;
cin >> b;
#endif
                  ]])],[
                   ace_cv_feature_char_ptr_right_shifts=yes
                  ],[
                   ace_cv_feature_char_ptr_right_shifts=no
                  ])
  ],,[AC_DEFINE([ACE_LACKS_CHAR_STAR_RIGHT_SHIFTS])])

dnl Check if we have working C++ explicit template destructors
dnl Thanks to Nanbor Wang <nanbor@cs.wustl.edu> for providing this test.
ACE_CACHE_CHECK([for working C++ explicit template destructors],
  [ace_cv_feature_working_explicit_des],
  [
   AC_COMPILE_IFELSE([AC_LANG_PROGRAM([[

class dyn
{
 public:
   dyn () { }
   ~dyn () { }
};

template <class T>
class Base
{
 public:
   Base () { }
   virtual void f (void) { }
   ~Base () { }
};

template <class T>
class Derived
{
 public:
   Derived ()
     {
      x_ = new Base<T> ();
     }
   virtual void f (void) { }
   ~Derived () { x_->~Base (); }
 private:
   Base<T> *x_;
   T t_;
};
                  ]],[[
                   Derived<dyn> *x = new Derived<dyn> ();

                   x->f ();

                   delete x;
                   return 0;
                  ]])],[
                   ace_cv_feature_working_explicit_des=yes
                  ],[
                   ace_cv_feature_working_explicit_des=no
                  ])
  ],[AC_DEFINE([ACE_HAS_WORKING_EXPLICIT_TEMPLATE_DESTRUCTOR])],)

dnl Check for working C++ conversions
ACE_CACHE_CHECK([for working C++ conversions],
  [ace_cv_feature_cxx_working_conversions],
  [
   AC_COMPILE_IFELSE([AC_LANG_PROGRAM([[
                   template <class T>
                   class Foo
                   {
                    public:
                      Foo (void) { ace_ptr = 0; }
                      operator T * (void) { return ace_ptr; }

                    private:
                      T * ace_ptr;
                   };
                  ]],[[
                   Foo<char> Bar;

                   Foo<char> * Bar_ptr = &Bar;

                   char * p = Bar_ptr->operator char * ();
                  ]])],[
                   ace_cv_feature_cxx_working_conversions=yes
                  ],[
                   ace_cv_feature_cxx_working_conversions=no
                  ])
  ],,[AC_DEFINE([ACE_HAS_BROKEN_CONVERSIONS])])

dnl Check for C++ namespace support
ACE_CACHE_CHECK([for C++ namespace support],
  [ace_cv_feature_cxx_namespaces],
  [
   AC_COMPILE_IFELSE([AC_LANG_PROGRAM([[
                   namespace ACE
                   {
                     int x;
                   }
                  ]],[[
                   ACE::x = 0;
                  ]])],[
                   ace_cv_feature_cxx_namespaces=yes
                  ],[
                   ace_cv_feature_cxx_namespaces=no
                  ])
  ],,[AC_DEFINE([ACE_HAS_BROKEN_NAMESPACES])])

dnl Check for C++ standard namespace
ACE_CACHE_CHECK([for C++ standard namespace],
  [ace_cv_feature_posix_uses_std_namespace],[
    AC_COMPILE_IFELSE([AC_LANG_PROGRAM([[
#include <iostream.h>
                   ]],[[
                    std::cout << "FOO" << std::endl;
                   ]])],[
                    ace_cv_feature_posix_uses_std_namespace=yes
                   ],[
                    ace_cv_feature_posix_uses_std_namespace=no
                   ])
  ],[AC_DEFINE([ACE_USES_STD_NAMESPACE_FOR_STDCPP_LIB])],)

dnl Check for C++ "using" keyword support
ACE_CACHE_CHECK([for C++ \"using\" keyword support],
  [ace_cv_feature_cxx_using_keyword],
  [
   AC_COMPILE_IFELSE([AC_LANG_PROGRAM([[
#if defined (ACE_USES_STD_NAMESPACE_FOR_STDCPP_LIB)
# include <iostream.h>
#elif !defined (ACE_HAS_BROKEN_NAMESPACES)
  namespace ACE
  {
   int x;
  }
#endif
                  ]],[[
#if defined (ACE_USES_STD_NAMESPACE_FOR_STDCPP_LIB)
  using namespace std;

  cout << "FOO" << endl;
#elif !defined (ACE_HAS_BROKEN_NAMESPACES)
  using namespace ACE;

  x++;
#else
# error Namespaces are not supported on this platform.
Throw in some extraneous text since some compilers are stupid
enough to interpret "#error" as a warning.  Go figure.
This extraneous text should force an error.
#endif
                  ]])],[
                   ace_cv_feature_cxx_using_keyword=yes
                  ],[
                   ace_cv_feature_cxx_using_keyword=no
                  ])
  ],[AC_DEFINE([ACE_HAS_USING_KEYWORD])],)

dnl Check for new style C++ include file support
ACE_CACHE_CHECK([for new style C++ include file support],
  [ace_cv_lib_posix_standard_includes],[
    ace_cv_lib_posix_standard_includes=no
    if test "$ace_cv_feature_posix_uses_std_namespace" = yes; then
      AC_COMPILE_IFELSE([AC_LANG_PROGRAM([[
#include <string>
        ]],[[
#ifdef ACE_USES_STD_NAMESPACE_FOR_STDCPP_LIB
         std::string str;
#else
         string str;
#endif  /* ACE_USES_STD_NAMESPACE_FOR_STDCPP_LIB */
        ]])],[ace_cv_lib_posix_standard_includes=yes],[])
    fi
  ],
  [
   AC_DEFINE([ACE_HAS_STDCPP_STL_INCLUDES])
   AC_DEFINE([ACE_HAS_STRING_CLASS])
  ],)

AC_CHECK_HEADER([map],
[
  AC_CHECK_HEADER([net/if.h],
   [
    ACE_CACHE_CHECK([if STL map class conflicts with <net/if.h> map struct],
    [ace_cv_header_stl_map_conflict],
    [
     AC_COMPILE_IFELSE([AC_LANG_PROGRAM([[
#ifndef ACE_LACKS_SYS_TYPES_H
# include <sys/types.h>
#endif
#include <map>
#include <net/if.h>
       ]],[[
#ifdef ACE_USES_STD_NAMESPACE_FOR_STDCPP_LIB
        std::map<char, char> ace_map;
#else
        map<char, char> ace_map;
#endif  /* ACE_USES_STD_NAMESPACE_FOR_STDCPP_LIB */
       ]])],[
        ace_cv_header_stl_map_conflict=no
       ],[
        ace_cv_header_stl_map_conflict=yes
       ])
    ],[AC_DEFINE([ACE_HAS_STL_MAP_CONFLICT])],)
  ],)
],)

AC_CHECK_HEADER([queue],
[
  AC_CHECK_HEADER([netinet/in.h],
   [
    ACE_CACHE_CHECK([if STL queue class conflicts with <netinet/in.h> queue struct],
    [ace_cv_header_stl_queue_conflict],
    [
     AC_COMPILE_IFELSE([AC_LANG_PROGRAM([[
#ifndef ACE_LACKS_SYS_TYPES_H
# include <sys/types.h>
#endif
#include <queue>
#include <netinet/in.h>
       ]],[[
#ifdef ACE_USES_STD_NAMESPACE_FOR_STDCPP_LIB
        std::queue<char> ace_queue;
#else
        queue<char> ace_queue;
#endif  /* ACE_USES_STD_NAMESPACE_FOR_STDCPP_LIB */
       ]])],[
        ace_cv_header_stl_queue_conflict=no
       ],[
        ace_cv_header_stl_queue_conflict=yes
       ])
    ],[AC_DEFINE([ACE_HAS_STL_QUEUE_CONFLICT])],)
  ],)
],)

dnl Check whether platform supports the standard C++ library
dnl TODO: For now, check whether headers <new>, <iomanip>
dnl       and <memory> exist; is there a better way?
if test "$ac_cv_header_new" = yes &&
   test "$ac_cv_header_iomanip" = yes &&
   test "$ac_cv_header_memory" = yes; then

  dnl Check for auto_ptr class
  ACE_CACHE_CHECK([for C++ auto_ptr class],
    [ace_cv_lib_auto_ptr_class],
    [
     AC_COMPILE_IFELSE([AC_LANG_PROGRAM([[
#include <memory>
       ]],[[
        int *foo = new int;

#ifdef ACE_USES_STD_NAMESPACE_FOR_STDCPP_LIB
        std::auto_ptr<int> safe (foo);
#else
        auto_ptr<int> safe (foo);
#endif
        foo = safe.release ();

        delete foo;
       ]])],[
        ace_cv_lib_auto_ptr_class=yes
       ],[
        ace_cv_lib_auto_ptr_class=no
       ])
    ],
    [
     AC_DEFINE([ACE_HAS_STANDARD_CPP_LIBRARY])
    ],
    [
     AC_DEFINE([ACE_LACKS_AUTO_PTR])
    ])
fi

if test $ace_cv_lib_auto_ptr_class=yes; then
  dnl Check for auto_ptr reset method
  ACE_CACHE_CHECK([for C++ auto_ptr reset method],
    [ace_cv_lib_auto_ptr_reset],
    [
     AC_COMPILE_IFELSE([AC_LANG_PROGRAM([[
#include <memory>
       ]],[[
        int *foo = new int;

#ifdef ACE_USES_STD_NAMESPACE_FOR_STDCPP_LIB
        std::auto_ptr<int> safe (foo);
#else
        auto_ptr<Foo> safe (foo);
#endif
        int *bar = new int;

        safe.reset (bar);

        foo = safe.release ();
       ]])],[
        ace_cv_lib_auto_ptr_reset=yes
       ],[
        ace_cv_lib_auto_ptr_reset=no
       ])
    ],,[AC_DEFINE([ACE_AUTO_PTR_LACKS_RESET])])
fi  dnl test $ace_cv_lib_auto_ptr_class=yes

dnl Check for ANSI C++ cast support
ACE_CACHE_CHECK([for ANSI C++ cast support],
  [ace_cv_lib_posix_standard_casts],[
   ACE_CONVERT_WARNINGS_TO_ERRORS([
    AC_COMPILE_IFELSE([AC_LANG_PROGRAM([[/* No headers */]],[[
       class B { public: int b_; virtual int b() const = 0; };
       class D : public B { virtual int b() const { return b_; } };

       void* vp = 0;
       char* cp = 0;
       float f = 1.0;
       int i = 0;
       int* ip = 0;
       const int* jp = 0;

       i = static_cast<int> (f);
       ip = const_cast<int*> (jp);
       cp = reinterpret_cast<char*> (vp);

       B* bp = new D;
       D* dp = dynamic_cast<D*> (bp);
       delete bp;
      ]])],[
       ace_cv_lib_posix_standard_casts=yes
       AC_CACHE_VAL([ace_cv_lib_rtti],[ace_cv_lib_rtti=yes])
      ],[
       AC_COMPILE_IFELSE([AC_LANG_PROGRAM(
         [[/* No headers */]],
         [[
          class B { public: int b_; virtual int b() const = 0; };
          class D : public B { virtual int b() const { return b_; } };

          void* vp = 0;
          char* cp = 0;
          float f = 1.0;
          int i = 0;
          int* ip = 0;
          const int* jp = 0;

          i = static_cast<int> (f);
          ip = const_cast<int*> (jp);
          cp = reinterpret_cast<char*> (vp);
         ]])],
         [
          ace_cv_lib_posix_standard_casts=yes
          AC_CACHE_VAL([ace_cv_lib_rtti],[ace_cv_lib_rtti=no])
         ],
         [
          ace_cv_lib_posix_standard_casts=no
         ])
      ])
   ])
  ],
  [
   AC_DEFINE([ACE_HAS_ANSI_CASTS])
   if test "$ace_cv_lib_rtti" != yes; then
     AC_DEFINE([ACE_LACKS_RTTI])
   fi
  ],)

dnl Check for explicit C++ constructor support
ACE_CACHE_CHECK([for explicit C++ constructor support],
  [ace_cv_feature_explicit_constructor],
  [
   AC_COMPILE_IFELSE([AC_LANG_PROGRAM([[
class Foo
{
 public:
   explicit Foo (int i) { this->val_ = i; }
 private:
   int val_;
};
     ]],[[
Foo bar = 'a'; // error: no implicit char->Foo conversion
     ]])],[
dnl    The above test *should* fail!
      ace_cv_feature_explicit_constructor=no
     ],[
      AC_COMPILE_IFELSE([AC_LANG_PROGRAM(
        [[
class Foo
{
 public:
   explicit Foo (int i) { this->val_ = i; }
 private:
   int val_;
};
        ]],
        [[
Foo bar (5);
        ]])],
        [
         ace_cv_feature_explicit_constructor=yes
        ],
        [
         ace_cv_feature_explicit_constructor=no
        ])
     ])
  ],[AC_DEFINE([ACE_HAS_EXPLICIT_KEYWORD])],)

dnl Check for C++ mutable keyword
ACE_CACHE_CHECK([for C++ mutable keyword],
  [ace_cv_feature_cxx_mutable_keyword],
  [
   AC_COMPILE_IFELSE([AC_LANG_PROGRAM([[
class Foo
{
 public:
   Foo (void) : val_ (0) {}

   mutable int val_;
};
     ]],[[
const Foo bar;

bar.val_ = 3; // mutable declaration should allow this to be modified
     ]])],[
      ace_cv_feature_cxx_mutable_keyword=yes
     ],[
      ace_cv_feature_cxx_mutable_keyword=no
     ])
  ],[AC_DEFINE([ACE_HAS_MUTABLE_KEYWORD])],)

dnl Check if platform supports typename keyword
ACE_CACHE_CHECK([for C++ typename keyword],
  [ace_cv_feature_posix_typename_keyword],[
    AC_COMPILE_IFELSE([AC_LANG_PROGRAM([[
                     class Bar
                     {
                     public:
                       typedef int Y;
                       Bar(int bar) : bar_(bar) {}
                     private:
                       int bar_;
                     };

                     template <class T>
                     class Foo
                     {
                     public:
                       typedef typename T::Y Y;
                       Foo(T* foo) : foo_(foo) {}
                       void bilbo(typename T::Y y);
                     private:
                       T* foo_;
                     };

                     template <class T>
                     void Foo<T>::bilbo(typename T::Y y)
                     {
                     }
                   ]],[[
                     Bar bar(15);
                     Foo<Bar> foo(&bar);
                     foo.bilbo(10);
                   ]])],[
                    ace_cv_feature_posix_typename_keyword=yes
                   ],[
                    ace_cv_feature_posix_typename_keyword=no
                   ])
  ],[AC_DEFINE([ACE_HAS_TYPENAME_KEYWORD])],)

dnl Check if platform supports placement new operator
ACE_CACHE_CHECK([for C++ placement new operator],
  [ace_cv_feature_placement_new],[
    AC_COMPILE_IFELSE([AC_LANG_PROGRAM([[
#if defined (ACE_HAS_NEW_NO_H)
# include <new>
#elif defined (ACE_HAS_NEW_H)
# include <new.h>
#endif

class foo
{
public:
  void *operator new (size_t, void *p) { return p; }
};
                   ]],[[
int *x = 0;
foo *f = new (x) foo;
                   ]])],[
                    ace_cv_feature_placement_new=yes
                   ],[
                    ace_cv_feature_placement_new=no
                   ])
  ],,[AC_DEFINE([ACE_LACKS_PLACEMENT_OPERATOR_NEW])])

dnl Check if platform supports placement delete operator
ACE_CACHE_CHECK([for C++ placement delete operator],
  [ace_cv_feature_placement_delete],[
    AC_COMPILE_IFELSE([AC_LANG_PROGRAM([[
#if defined (ACE_HAS_NEW_NO_H)
# include <new>
#elif defined (ACE_HAS_NEW_H)
# include <new.h>
#endif

class foo
{
public:
  void *operator new (size_t, void *p) { return p; }
  void operator delete (void *p, void *) {}
};
                   ]],[[
int *x = 0;
foo *f = new (x) foo;

// delete f;  // Don't call delete for this test!
                   ]])],[
                    ace_cv_feature_placement_delete=yes
                   ],[
                    ace_cv_feature_placement_delete=no
                   ])
  ],,[AC_DEFINE([ACE_LACKS_PLACEMENT_OPERATOR_DELETE])])


dnl Check if const char * can be rvalue in conditional operator
ACE_CACHE_CHECK([if const char * can be rvalue in conditional operator],
  [ace_cv_feature_conditional_str_cast],
  [
   AC_COMPILE_IFELSE([AC_LANG_PROGRAM([[]],[[
                   const char * foo = 0;
                   const char * bar = 0;

                   const char * ace = ((1 < 2) ? foo : bar);
                  ]])],[
                   ace_cv_feature_conditional_str_cast=yes
                  ],[
                   ace_cv_feature_conditional_str_cast=no
                  ])
  ],,[AC_DEFINE([ACE_HAS_BROKEN_CONDITIONAL_STRING_CASTS])])

dnl Check if templates require source on platform
dnl FIXME:  This test may be broken.
ACE_CACHE_CHECK([if templates require source],
  [ace_cv_feature_templates_require_source],
  [
   dnl Create the common header file
   cat > ace_test.h <<EOF
#ifndef FOO_H
#define FOO_H
template <class T>
class Foo
{
 public:
   Foo (T val);
 private:
   T value_;
};

template <class T>
class Bar
{
 public:
   Bar (Foo<T> *);
 private:
   Foo<T> *foo_ptr;
};
#endif /* FOO_H */
EOF

   dnl Create template source test file
   cat > ace_test.$ac_ext <<EOF
#include "ace_test.h"
#ifndef FOO_CXX
#define FOO_CXX
template <class T>
Foo<T>::Foo (T val)
  : value_ (val)
{
 // Nothing else to do.
}

template <class T>
Bar<T>::Bar (Foo<T> *val)
  : foo_ptr (val)
{
 // Nothing else to do.
}
#endif /* FOO_CXX */
EOF

   dnl Add the ACE-specific compiler flags to the compiler flags for
   dnl the duration of this test.
   ace_cxx_template_save_CXXFLAGS="$CXXFLAGS"
   ace_cxx_template_save_CPPFLAGS="$CPPFLAGS"
   ace_cxx_template_save_LDFLAGS="$LDFLAGS"
   CXXFLAGS="$ACE_CXXFLAGS $CXXFLAGS"
   CPPFLAGS="$ACE_CPPFLAGS $CPPFLAGS"
   LDFLAGS="$ACE_LDFLAGS $LDFLAGS"

   dnl Remove any template repositories.
   rm -rf Templates.DB SunWS_cache ptrepository *.rpo

   dnl First try without explicit template instantiation.
   AC_LINK_IFELSE([AC_LANG_PROGRAM([[
#include "ace_test.h"
     ]],[[
Foo<int> foo (15);
Bar<char> bar (0);
     ]])],[
      dnl Template source is not required.
      ace_cv_feature_templates_require_source=no

      dnl Template source does not require pragma.
      AC_CACHE_VAL([ace_cv_feature_templates_require_pragma],
                   [ace_cv_feature_templates_require_pragma=no])

      dnl Explicit template instantiation is not required.
      AC_CACHE_VAL([ace_cv_feature_explicit_template_instantiation],
                   [ace_cv_feature_explicit_template_instantiation=no])

      dnl Pragma template instantiation is not required.
      AC_CACHE_VAL([ace_cv_feature_pragma_template_instantiation],
                   [ace_cv_feature_pragma_template_instantiation=no])
     ],[
      dnl Remove any template repositories.
      rm -rf Templates.DB SunWS_cache ptrepository *.rpo

      dnl Now try including the template source.
      AC_LINK_IFELSE([AC_LANG_PROGRAM(
        [[
#include "ace_test.h"
#include "ace_test.$ac_ext"
        ]],
        [[
Foo<int> foo (15);

Bar<char> bar (0);
        ]])],
        [
         dnl Template source is required!
         ace_cv_feature_templates_require_source=yes

         dnl Template source does not require pragma.
         AC_CACHE_VAL([ace_cv_feature_templates_require_pragma],
                      [ace_cv_feature_templates_require_pragma=no])

         dnl Explicit template instantiation is not required.
         AC_CACHE_VAL([ace_cv_feature_explicit_template_instantiation],
                      [ace_cv_feature_explicit_template_instantiation=no])

         dnl Pragma template instantiation is not required.
         AC_CACHE_VAL([ace_cv_feature_pragma_template_instantiation],
                      [ace_cv_feature_pragma_template_instantiation=no])
        ],
        [
dnl BEGIN OUTER REQUIRE SOURCE #########################################
         dnl Remove any generated template repositories.
         rm -rf Templates.DB SunWS_cache ptrepository *.rpo

         dnl Now try with explicit template instantiation.
         AC_LINK_IFELSE([AC_LANG_PROGRAM(
           [[
#include "ace_test.h"

template class Foo<int>;
template class Bar<char>;
           ]],
           [[
Foo<int> foo (15);
Bar<char> bar (0);
           ]])],
           [
            dnl Template source is not required.
            ace_cv_feature_templates_require_source=no

            dnl Template source does not require pragma.
            AC_CACHE_VAL([ace_cv_feature_templates_require_pragma],
                         [ace_cv_feature_templates_require_pragma=no])

            dnl Explicit template instantiation is required.
            AC_CACHE_VAL([ace_cv_feature_explicit_template_instantiation],
                         [ace_cv_feature_explicit_template_instantiation=yes])

            dnl Pragma template instantiation is not required.
            AC_CACHE_VAL([ace_cv_feature_pragma_template_instantiation],
                         [ace_cv_feature_pragma_template_instantiation=no])
           ],
           [
            dnl Remove any generated template repositories.
            rm -rf Templates.DB SunWS_cache ptrepository *.rpo

            dnl Don't set
            dnl ace_cv_feature_pragma_template_instantiation
            dnl to "no" here.  It should only be set to "no" if
            dnl explicit template instantiation works.

            dnl Now try including the template source.
            AC_LINK_IFELSE([AC_LANG_PROGRAM(
              [[
#include "ace_test.h"
#include "ace_test.$ac_ext"

template class Foo<int>;
template class Bar<char>;
              ]],
              [[
Foo<int> foo (15);
Bar<char> bar (0);
              ]])],
              [
               dnl Template source is required!
               ace_cv_feature_templates_require_source=yes

               dnl Template source does not require pragma.
               AC_CACHE_VAL([ace_cv_feature_templates_require_pragma],
                   [ace_cv_feature_templates_require_pragma=no])

               dnl Explicit template instantiation is required.
               AC_CACHE_VAL(
                 [ace_cv_feature_explicit_template_instantiation],
                 [ace_cv_feature_explicit_template_instantiation=yes])

               dnl Pragma template instantiation is not required.
               AC_CACHE_VAL(
                 [ace_cv_feature_pragma_template_instantiation],
                 [ace_cv_feature_pragma_template_instantiation=no])
              ],
              [
dnl BEGIN INNER REQUIRE SOURCE #########################################
               dnl Remove any generated template repositories.
               rm -rf Templates.DB SunWS_cache ptrepository *.rpo

               dnl Don't set
               dnl ace_cv_feature_explicit_template_instantiation
               dnl to "no" here.  It should only be set to "no" if
               dnl pragma template instantiation works.

               dnl Now try with pragma template instantiation.
               AC_LINK_IFELSE([AC_LANG_PROGRAM(
                 [[
#include "ace_test.h"

#pragma instantiate Foo<int>
#pragma instantiate Bar<char>
                 ]],
                 [[
Foo<int> foo (15);
Bar<char> bar (0);
                 ]])],
                 [
                  dnl Template source is not required.
                  ace_cv_feature_templates_require_source=no

                  dnl Template source does not require pragma.
                  AC_CACHE_VAL(
                    [ace_cv_feature_templates_require_pragma],
                    [ace_cv_feature_templates_require_pragma=no])

                  dnl Explicit template instantiation is not required.
                  AC_CACHE_VAL(
                    [ace_cv_feature_explicit_template_instantiation],
                    [ace_cv_feature_explicit_template_instantiation=no])

                  dnl Pragma template instantiation is required.
                  AC_CACHE_VAL(
                    [ace_cv_feature_pragma_template_instantiation],
                    [ace_cv_feature_pragma_template_instantiation=yes])
                 ],
                 [
                  dnl Remove any generated template repositories.
                  rm -rf Templates.DB SunWS_cache ptrepository *.rpo

                  dnl Don't set
                  dnl ace_cv_feature_explicit_template_instantiation
                  dnl to "no" here.  It should only be set to "no" if
                  dnl pragma template instantiation works.

                  dnl Now try including the template source.
                  AC_LINK_IFELSE([AC_LANG_PROGRAM(
                    [[
#include "ace_test.h"
#include "ace_test.$ac_ext"

#pragma instantiate Foo<int>
#pragma instantiate Bar<char>
                    ]],
                    [[
Foo<int> foo (15);
Bar<char> bar (0);
                    ]])],
                    [
                     dnl Template source is required!
                     ace_cv_feature_templates_require_source=yes

                     dnl Template source does not require pragma.
                     AC_CACHE_VAL(
                       [ace_cv_feature_templates_require_pragma],
                       [ace_cv_feature_templates_require_pragma=no])

                     dnl Explicit template instantiation is not required.
                     AC_CACHE_VAL(
                       [ace_cv_feature_explicit_template_instantiation],
                       [ace_cv_feature_explicit_template_instantiation=no])

                     dnl Pragma template instantiation is required.
                     AC_CACHE_VAL(
                       [ace_cv_feature_pragma_template_instantiation],
                       [ace_cv_feature_pragma_template_instantiation=yes])
                    ],
                    [
                     dnl If we get here, then we have no idea what is needed!
                     ace_cv_feature_templates_require_source=no
                    ])
                 ])
dnl END INNER REQUIRE SOURCE #########################################
              ])
          ])
dnl END OUTER REQUIRE SOURCE #########################################
        ])
     ])

   dnl Remove any generated template repositories.
   rm -rf Templates.DB SunWS_cache ptrepository *.rpo

   dnl Remove the test additional test files.
   rm -f ace_test*

   dnl Restore the compiler flags
   CXXFLAGS="$ace_cxx_template_save_CXXFLAGS"
   CPPFLAGS="$ace_cxx_template_save_CPPFLAGS"
   LDFLAGS="$ace_cxx_template_save_LDFLAGS"
  ],
  [
   AC_DEFINE([ACE_TEMPLATES_REQUIRE_SOURCE])
  ],
  [
   dnl Check if templates require pragma.
   ACE_CACHE_CHECK([if templates require pragma],
     [ace_cv_feature_templates_require_pragma],
     [
      dnl Create the common header file
      cat > ace_test.h <<EOF
#ifndef FOO_H
#define FOO_H
template <class T>
class Foo
{
 public:
   Foo (T val);
 private:
   T value_;
};

template <class T>
class Bar
{
 public:
   Bar (Foo<T> *);
 private:
   Foo<T> *foo_ptr;
};
#endif /* FOO_H */
EOF

      dnl Create template source test file
      cat > ace_test.$ac_ext <<EOF
#include "ace_test.h"
#ifndef FOO_CXX
#define FOO_CXX
template <class T>
Foo<T>::Foo (T val)
  : value_ (val)
{
 // Nothing else to do.
}

template <class T>
Bar<T>::Bar (Foo<T> *val)
  : foo_ptr (val)
{
 // Nothing else to do.
}
#endif /* FOO_CXX */
EOF

      dnl Add the ACE-specific compiler flags to the compiler flags for
      dnl the duration of this test.
      ace_cxx_template_save_CXXFLAGS="$CXXFLAGS"
      ace_cxx_template_save_CPPFLAGS="$CPPFLAGS"
      ace_cxx_template_save_LDFLAGS="$LDFLAGS"
      CXXFLAGS="$ACE_CXXFLAGS $CXXFLAGS"
      CPPFLAGS="$ACE_CPPFLAGS $CPPFLAGS"
      LDFLAGS="$ACE_LDFLAGS $LDFLAGS"

      dnl Remove any template repositories.
      rm -rf Templates.DB SunWS_cache ptrepository *.rpo

      dnl We already know that the simplest case doesn't work so go
      dnl straight to the "require pragma" test.

      dnl Now try including the template pragma.
      AC_LINK_IFELSE([AC_LANG_PROGRAM([[
#include "ace_test.h"

#pragma implementation ("ace_test.$ac_ext")
        ]],[[
Foo<int> foo (15);
Bar<char> bar (0);
        ]])],[
         dnl Template source is required!
         ace_cv_feature_templates_require_pragma=yes
        ],[
dnl BEGIN OUTER REQUIRE PRAGMA #########################################
         dnl Remove any generated template repositories.
         rm -rf Templates.DB SunWS_cache ptrepository *.rpo

         dnl Now try with explicit template instantiation.
         AC_LINK_IFELSE([AC_LANG_PROGRAM(
           [[
#include "ace_test.h"

#pragma implementation ("ace_test.$ac_ext")

template class Foo<int>;
template class Bar<char>;
           ]],
           [[
Foo<int> foo (15);
Bar<char> bar (0);
           ]])],
           [
            dnl Template pragma is required!
            ace_cv_feature_templates_require_pragma=yes

            dnl Explicit template instantiation is required.
            AC_CACHE_VAL(
              [ace_cv_feature_explicit_template_instantiation],
              [ace_cv_feature_explicit_template_instantiation=yes])

            dnl Pragma template instantiation is not required.
            AC_CACHE_VAL(
              [ace_cv_feature_pragma_template_instantiation],
              [ace_cv_feature_pragma_template_instantiation=no])
           ],
           [
dnl BEGIN INNER REQUIRE PRAGMA #########################################
            dnl Remove any generated template repositories.
            rm -rf Templates.DB SunWS_cache ptrepository *.rpo

            dnl Now try with pragma template instantiation.
            AC_LINK_IFELSE([AC_LANG_PROGRAM(
              [[
#include "ace_test.h"

#pragma implementation ("ace_test.$ac_ext")

#pragma instantiate Foo<int>
#pragma instantiate Bar<char>
              ]],
              [[
Foo<int> foo (15);
Bar<char> bar (0);
              ]])],
              [
               dnl Template pragma is required!
               ace_cv_feature_templates_require_pragma=yes

               dnl Explicit template instantiation is not required.
               AC_CACHE_VAL(
                 [ace_cv_feature_explicit_template_instantiation],
                 [ace_cv_feature_explicit_template_instantiation=no])

               dnl Pragma template instantiation is required.
               AC_CACHE_VAL(
                 [ace_cv_feature_pragma_template_instantiation],
                 [ace_cv_feature_pragma_template_instantiation=yes])
              ],
              [
               dnl If we get here, then we have no idea what is needed!
               ace_cv_feature_templates_require_pragma=no
              ])
dnl END INNER REQUIRE PRAGMA #########################################
           ])
dnl END OUTER REQUIRE PRAGMA #########################################
        ])

      dnl Remove any generated template repositories.
      rm -rf Templates.DB SunWS_cache ptrepository *.rpo

      dnl Remove the additional test files.
      rm -f ace_test*

      dnl Restore the compiler flags
      CXXFLAGS="$ace_cxx_template_save_CXXFLAGS"
      CPPFLAGS="$ace_cxx_template_save_CPPFLAGS"
      LDFLAGS="$ace_cxx_template_save_LDFLAGS"
     ],
     [
      AC_DEFINE([ACE_TEMPLATES_REQUIRE_PRAGMA])
     ],
     [
      dnl Do nothing.
     ])
  ])

ACE_CACHE_CHECK([if explicit template instantiation is needed],
  [ace_cv_feature_explicit_template_instantiation],
  [
   AC_MSG_ERROR([
Bug in configure script, check for need of explicit template
instantiation should have occurred during check for need of template
source.  Please e-mail the maintainer of this configure script:
  $ACE_CONFIGURE_MAINTAINER
this message.
    ])
  ],
  [
   AC_DEFINE([ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION])
  ],
  [
   ACE_CACHE_CHECK([if pragma template instantiation is needed],
     [ace_cv_feature_pragma_template_instantiation],
     [
      AC_MSG_FAILURE([
Bug in configure script, check for need of pragma template
instantiation should have occurred during check for need of template
source.  Please e-mail the maintainer of this configure script:
  $ACE_CONFIGURE_MAINTAINER
this message.
        ])
     ],
     [
      AC_DEFINE([ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA])
     ],
     [
      dnl Do nothing
     ])
  ])


dnl Check if platform supports template specialization
ACE_CACHE_CHECK([for template specialization],
  [ace_cv_feature_cxx_template_specialization],[
     AC_COMPILE_IFELSE([AC_LANG_PROGRAM([[
        template <class T>
        class Foo
        {
         public:
           Foo(T bar) : bar_(bar) {};
         private:
           T bar_;
        };

        class Foo<int>
        {
         public:
           Foo(int bar) : bar_(bar + 1) {};
         private:
           int bar_;
        };
       ]],[[
         Foo<int> foo(11);
       ]])],[
        ace_cv_feature_cxx_template_specialization=yes
       ],[
        ace_cv_feature_cxx_template_specialization=no
       ])
  ],[AC_DEFINE([ACE_HAS_TEMPLATE_SPECIALIZATION])],)

dnl Check if platform supports *standard *template specialization
ACE_CACHE_CHECK([for standard template specialization],
  [ace_cv_feature_cxx_std_template_specialization],[
    AC_COMPILE_IFELSE([AC_LANG_PROGRAM([[
// Some compilers have a hard time with this test since the syntax is
// too "new" for them.

// general vector type
template<class T>
class Vector
{
public:
  Vector (void);
  Vector (int);

  T& elem (int i) { return v[i]; }
  T& operator[] (int i);
private:
  T* v;
  int sz;
};

// void * specialization
template<>
class Vector<void *>
{
public:
  Vector (void);
  Vector (int);

  void*& elem (int i) { return p[i]; }
  void*& operator[] (int i);
private:
  void** p;
  int sz;
};
      ]],[[
Vector<int> vi;
Vector<void *> vpv;
      ]])],[
       ace_cv_feature_cxx_std_template_specialization=yes
      ],[
       ace_cv_feature_std_cxx_template_specialization=no
      ])
  ],[AC_DEFINE([ACE_HAS_STD_TEMPLATE_SPECIALIZATION])],)

if test "$ace_cv_feature_cxx_std_template_specialization" = yes; then

dnl Check if platform needs *standard* template method specialization
ACE_CACHE_CHECK([if platform needs standard template method specialization],
  [ace_cv_feature_cxx_std_template_method_specialization],[
    AC_COMPILE_IFELSE([AC_LANG_PROGRAM([[
// Some compilers have a hard time with this test since the syntax is
// too "new" for them.

// general vector type
template<class T>
class Vector
{
public:
  Vector (void);
  Vector (int);

  T& elem (int i) { return v[i]; }
  T& operator[] (int i);
private:
  T* v;
  int sz;
};

// void * specialization
template<>
class Vector<void *>
{
public:
  Vector (void);
  Vector (int);

  void*& elem (int i) { return p[i]; }
  void*& operator[] (int i);
private:
  void** p;
  int sz;
};

void *&
Vector<void *>::operator[] (int i)
{
 return p[i];
}
      ]],[[
       Vector <void *> vpv;
      ]])],[
dnl   template method specialization is *not* needed
       ace_cv_feature_cxx_std_template_method_specialization=no
      ],[
       AC_COMPILE_IFELSE([AC_LANG_PROGRAM(
         [[
// Some compilers have a hard time with this test since the syntax is
// too "new" for them.

// general vector type
template<class T>
class Vector
{
public:
  Vector (void);
  Vector (int);

  T& elem (int i) { return v[i]; }
  T& operator[] (int i);
private:
  T* v;
  int sz;
};

// void * specialization
template<>
class Vector<void *>
{
public:
  Vector (void);
  Vector (int);

  void*& elem (int i) { return p[i]; }
  void*& operator[] (int i);
private:
  void** p;
  int sz;
};

template <>
void *&
Vector<void *>::operator[] (int i)
{
 return p[i];
}
         ]],
         [[
          Vector<void *> vpv;
         ]])],
         [
dnl   template method specialization is needed
          ace_cv_feature_cxx_std_template_method_specialization=yes
         ],
         [
dnl   template method specialization is *not* needed
          ace_cv_feature_cxx_std_template_method_specialization=no
         ])
      ])
  ],[AC_DEFINE([ACE_HAS_STD_TEMPLATE_METHOD_SPECIALIZATION])],)
fi  dnl test $ace_cv_feature_cxx_std_template_specialization = yes

dnl Check if platform supports template typedefs
ACE_CACHE_CHECK([for template typedefs],
  [ace_cv_feature_posix_template_typedefs],[
    AC_COMPILE_IFELSE([AC_LANG_PROGRAM([[
#if defined (ACE_HAS_TYPENAME_KEYWORD)
#define TYPENAME typename
#else
#define TYPENAME
#endif

class Bar
{
public:
  typedef int Y;
  Bar(int bar) : bar_(bar) {}
  int value() const { return bar_; }
private:
  int bar_;
};

template <class T>
class Foo
{
public:
  typedef TYPENAME T::Y Y;
  Foo(T* foo) : foo_(foo) {}
  void print(Y);
private:
  T* foo_;
};

template <class T>
void Foo<T>::print(TYPENAME T::Y)
{
}
                   ]],[[
Bar bar(15);
Foo<Bar> foo(&bar);
foo.print(11);
                   ]])],[
                    ace_cv_feature_posix_template_typedefs=yes
                   ],[
                    ace_cv_feature_posix_template_typedefs=no
                   ])
  ],[AC_DEFINE([ACE_HAS_TEMPLATE_TYPEDEFS])],)

dnl Check if platform supports static data member templates
ACE_CACHE_CHECK([for static data member templates],
  [ace_cv_feature_posix_static_data_member_templates],[
    AC_COMPILE_IFELSE([AC_LANG_PROGRAM([[
template <class T>
class Foo
{
public:
  static T* sdm;
};

template <class T> T* Foo<T>::sdm = 0;
                   ]],[[
                    /* No body */
                   ]])],[
                    ace_cv_feature_posix_static_data_member_templates=yes
                   ],[
                    ace_cv_feature_posix_static_data_member_templates=no
                   ])
  ],,[AC_DEFINE([ACE_LACKS_STATIC_DATA_MEMBER_TEMPLATES])])

dnl Check if compiler needs definitions for hidden functions
ACE_CACHE_CHECK([if definition is needed for hidden functions],
  [ace_cv_feature_need_func_def],
  [
   AC_LINK_IFELSE([AC_LANG_PROGRAM([[
      class Foo
      {
       public:
         Foo (void) { a_ = 0; }
       private:
         Foo (const Foo &);
         void operator= (const Foo &);

         int a_;
      };
     ]],[[
       Foo Bar;
     ]])],[
      ace_cv_feature_need_func_def=no
     ],[
      AC_LINK_IFELSE([AC_LANG_PROGRAM(
        [[
         class Foo
         {
          public:
            Foo (void) { a_ = 0; }
          private:
            Foo (const Foo &);
            const Foo & operator= (const Foo &);

            int a_;
         };

         Foo::Foo (const Foo &)
         {
          a_ = 0;
         }

         const Foo &
         Foo::operator= (const Foo &)
         {
          a_ = 0;

          return *this;
         }
        ]],
        [[
         Foo Bar;
        ]])],
        [
         ace_cv_feature_need_func_def=yes
        ],
        [
         dnl If we get here then we don't know what is needed!
         ace_cv_feature_need_func_def=no
        ])
     ])
  ],
  [
   AC_DEFINE([ACE_NEEDS_FUNC_DEFINITIONS])
  ],)

dnl Check if platform supports C++ exceptions
if test "$ace_user_enable_exceptions" = yes; then
  ACE_CACHE_CHECK([for C++ exceptions],
    [ace_cv_feature_posix_exceptions],[
      AC_COMPILE_IFELSE([AC_LANG_PROGRAM([[]],[[
  int ret = 0;
  class ACE {};
  try
    {
      throw ACE();
    }
  catch (ACE)
    {
      ret = 1;
    }
                     ]])],[
                      ace_cv_feature_posix_exceptions=yes
                     ],[
                      ace_cv_feature_posix_exceptions=no
                     ])
    ],[AC_DEFINE([ACE_HAS_EXCEPTIONS])],[ace_user_enable_exceptions=no])

fi dnl test "$ace_user_enable_exceptions" = yes

dnl Check if we need a non-static object manager
dnl TODO / FIXME
dnl ACE_CACHE_CHECK([if we need a non-static object manager],
dnl   [ace_cv_feature_nonstatic_object_manager],[
dnl     ace_cv_feature_nonstatic_object_manager=yes
    dnl TODO: Should we check for this thing (and HOW), or
    dnl       should it be the user's choice?

    dnl For now, we will leave it as a user's choice.
    dnl                    -Ossama
dnl   ],
dnl   [
   dnl Don't define anything until we have a test for this.
   dnl AC_DEFINE([ACE_HAS_NONSTATIC_OBJECT_MANAGER])
dnl   ],)

dnl Save the cache for debugging purposes
AC_CACHE_SAVE


dnl SECTION 10: checks for library functions


AC_FUNC_ALLOCA
if test "$ac_cv_header_alloca_h" = yes; then
  AC_DEFINE([ACE_HAS_ALLOCA_H])
fi
if test "$ac_cv_func_alloca_works" = yes; then
  AC_DEFINE([ACE_HAS_ALLOCA])
fi

dnl ACE should really have something for both the sys/mman.h header
dnl and the mmap function since we need sys/mman.h for functions like
dnl mprotect and msync, but don't want to use mmap if it doesn't work.
dnl For now, we just check for the sys/mman.h header earlier in this
dnl configure script.

dnl AC_FUNC_MMAP
dnl if test "$ac_cv_func_mmap_fixed_mapped" = no; then
dnl Even if we have mmap, do not use if broken!
dnl  AC_DEFINE(ACE_LACKS_MMAP)
dnl fi

dnl Check if closedir() returns a meaningful value
AC_FUNC_CLOSEDIR_VOID

dnl Check for PWD functions
AC_CHECK_FUNC([getpwnam],,)
AC_CHECK_FUNC([setpwent],,)
AC_CHECK_FUNC([endpwent],,)
AC_CHECK_FUNC([getpwent],,)
AC_CHECK_FUNC([getpwuid],,)

if test "$ac_cv_func_getpwnam" != yes ||
  test "$ac_cv_func_setpwent" != yes ||
  test "$ac_cv_func_endpwent" != yes ||
  test "$ac_cv_func_getpwent" != yes ||
  test "$ac_cv_func_getpwuid" != yes; then
  AC_DEFINE([ACE_LACKS_PWD_FUNCTIONS])
else
  dnl The password file related functions above are required for ACE's
  dnl alternate implementation.

 ACE_CONVERT_WARNINGS_TO_ERRORS([
  dnl Check for functions necessary for ACE's alternate implementation
  dnl of the now obsolete cuserid() function.
  ACE_CACHE_CHECK([checking if ACE cuserid implementation should be used],
    [ace_cv_lib_use_alt_cuserid],
    [
     AC_LINK_IFELSE([AC_LANG_PROGRAM([[
/* Undefine _XOPEN_SOURCE since it may make the cuserid() prototype
   visible.  ACE should not rely on such feature test macros. */
#undef _XOPEN_SOURCE
#ifndef ACE_LACKS_UNISTD_H
# include <unistd.h>
#else
# error No unistd.h header.  Need header where cuserid() is located.
#endif  /*  ACE_LACKS_UNISTD_H */
       ]],[[
        char * foo = cuserid ((char *)0);
       ]])],[
        dnl If successful then use the system cuserid() implementation,
        dnl despite the fact that ACE's implementation may be safer.
        ace_cv_lib_use_alt_cuserid=no
       ],[
        AC_LINK_IFELSE([AC_LANG_PROGRAM(
          [[
#ifndef ACE_LACKS_SYS_TYPES_H
# include <sys/types.h>
#endif
#undef _XOPEN_SOURCE
#ifndef ACE_LACKS_UNISTD_H
# include <unistd.h>
#else
# error No unistd.h header.  Need header where geteuid() is located.
#endif
          ]],
          [[
           uid_t foo = geteuid ();
          ]])],
          [
           dnl All of the functions necessary for ACE's cuserid()
           dnl implementation exist.
           ace_cv_lib_use_alt_cuserid=yes
          ],
          [
           dnl If we get here, we're hosed!
           ace_cv_lib_use_alt_cuserid=no
          ])
       ])
    ],
    [
     AC_DEFINE([ACE_HAS_ALT_CUSERID])
    ],)
 ])
fi



dnl Check for `strftime' in the `intl' library, for SCO UNIX
AC_FUNC_STRFTIME

case "$host_os" in
  *win32*)
     AC_CHECK_FUNC([CancelIO],
       [AC_DEFINE([ACE_HAS_CANCEL_IO])],)

     AC_CHECK_FUNC([SignalObjectAndWait],
       [AC_DEFINE([ACE_HAS_SIGNAL_OBJECT_AND_WAIT])],)

     AC_CHECK_FUNC([TryEnterCriticalSection],
       [AC_DEFINE([ACE_HAS_WIN32_TRYLOCK])],)
     ;;
  *)
     ;;
esac

AC_CHECK_FUNC([access],
  ,
  [AC_DEFINE([ACE_LACKS_ACCESS])])

dnl Temporarily select C as the test language to work around a bug in
dnl some versions of Autoconf when C++ is selected as the test
dnl language.
AC_LANG_PUSH([C])
AC_CHECK_FUNC([bsearch],
  ,
  [AC_DEFINE([ACE_LACKS_BSEARCH])])
AC_LANG_POP([C])

dnl Temporarily select C as the test language to work around a bug in
dnl some versions of Autoconf when C++ is selected as the test
dnl language.
AC_LANG_PUSH([C])
AC_CHECK_FUNC([qsort],
  ,
  [AC_DEFINE([ACE_LACKS_QSORT])])
AC_LANG_POP([C])

dnl ACE uses execv, execvp and execve, so we don't bother to check
dnl for the others (e.g. execl, execlp, execle)
AC_CHECK_FUNCS([execv execvp execve],
  ,
  [AC_DEFINE([ACE_LACKS_EXEC])])

AC_CHECK_FUNC([fgetwc],
  ,
  [AC_DEFINE([ACE_LACKS_FGETWC])])

AC_CHECK_FUNC([fcntl],
  ,
  [AC_DEFINE([ACE_LACKS_FCNTL])])

AC_CHECK_FUNC([fsync],
  ,
  [AC_DEFINE([ACE_LACKS_FSYNC])])

AC_CHECK_FUNC([getmsg],
  [
   AC_RUN_IFELSE([AC_LANG_SOURCE([[
#ifndef ACE_LACKS_UNISTD_H
# include <unistd.h>
#endif  /* !ACE_LACKS_UNISTD_H */

#ifdef ACE_HAS_SYS_IOCTL_H
# include <sys/ioctl.h>
#endif  /* ACE_HAS_SYS_IOCTL_H */

#ifdef ACE_HAS_STREAMS
#  include <stropts.h>
#endif  /* ACE_HAS_STREAMS */

int
main ()
{
  int fds[2];

  if (pipe (fds) != 0)
    return -1;

/*
 * Verify that we can actually set a STREAM option that ACE uses.
 * This is particularly necessary for platforms where compiling and
 * linking succeed but fail at run-time due to a missing actual
 * STREAMS implementation.  For example, Linux/glibc requires a
 * STREAMS patch/add-on.
 */

  int arg = RMSGN;

  if (ioctl (fds[0], I_SRDOPT, (void *) arg) != 0)
    return -1;

  return 0;
}
     ]])],[
      AC_DEFINE([ACE_HAS_STREAM_PIPES])
     ],[],[
      dnl action if cross-compiling
      AC_DEFINE([ACE_HAS_STREAM_PIPES])
     ])
  ],)

AC_CHECK_FUNC([gethostbyaddr],,)

AC_CHECK_FUNC([difftime],
  ,
  [AC_DEFINE([ACE_LACKS_DIFFTIME])])

dnl Check for 64 bit llseek() or lseek64()
case "$host" in
  *UnixWare7*)
    dnl Skip the check
    ;;
  *)
    ACE_CHECK_LSEEK64
    ;;
esac

AC_CHECK_FUNC([bind],
  [
   ACE_CACHE_CHECK([if bind() will select the port if it is zero],
     [ace_cv_have_wildcard_bind],
     [
      AC_RUN_IFELSE([AC_LANG_SOURCE([[
#ifndef ACE_LACKS_SYS_TYPES_H
# include <sys/types.h>
#endif
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>  /* for memset() */

         int main () {

           int sockfd = socket(AF_INET, SOCK_STREAM, 0);

           sockaddr_in serv_addr;

           (void) memset ((void *) &serv_addr, 0, sizeof(serv_addr));

           serv_addr.sin_family      = AF_INET;
           serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
           serv_addr.sin_port        = 0;

           return bind (sockfd, (struct sockaddr *) &serv_addr,
                        sizeof(serv_addr));
         }
        ]])],[
         ace_cv_have_wildcard_bind=yes
        ],[
         ace_cv_have_wildcard_bind=no
        ],[
         dnl action if cross-compiling
         ace_cv_have_wildcard_bind=yes
        ])
     ],,[AC_DEFINE([ACE_LACKS_WILDCARD_BIND])])
  ],)

AC_CHECK_FUNC([poll],
  [
   ACE_CACHE_CHECK([if poll is broken],
     [ace_cv_poll_is_broken],
     [
      AC_RUN_IFELSE([AC_LANG_SOURCE([[
#include <poll.h>
#include <time.h>

#ifdef ACE_LACKS_SYSTIME_H
# include <sys/time.h>
#endif

         int main () {

         int timeout = 2000; /* 2000 milliseconds */
         int timeout_arg = timeout;
                      /* poll() modifies timeout argument on some platforms */

         int result = 0;

         time_t *t = 0;

         time_t time1 = time(t);

         result = poll(0, 0, timeout_arg);

         time_t time2 = time(t);
         time_t diff = time2 - time1;

         /* If (diff * 1000) < timeout then poll() is broken! */
         if (result >= 0)
           return (((diff * 1000) < timeout) ? 1 : 0);
         else
           return 1; /* We had a problem with poll() so we don't
                        know what really happened. */
         }
        ]])],[
         ace_cv_poll_is_broken=no
        ],[
         ace_cv_poll_is_broken=yes
        ],[
         dnl action if cross-compiling
         ace_cv_poll_is_broken=no
        ])
     ],[AC_DEFINE([ACE_POLL_IS_BROKEN])],)
  ],)

AC_CHECK_FUNC([gethrtime],
  [
   ACE_CHECK_TYPE([hrtime_t],[sys/time.h],[AC_DEFINE([ACE_HAS_HI_RES_TIMER])],)
  ],)

AC_CHECK_FUNC([pread],
  [AC_CHECK_FUNC([pwrite],
    [
     AC_DEFINE([ACE_HAS_P_READ_WRITE])
     dnl Check if _XOPEN_SOURCE=500 macro is needed to make the pread() and
     dnl pwrite() prototypes visible.
     ACE_CACHE_CHECK([for pread prototype],
       [ace_cv_lib_has_pread_prototype],
       [
        ace_save_CPPFLAGS="$CPPFLAGS"
        ace_no_xopen="-U_XOPEN_SOURCE"
        CPPFLAGS="$CPPFLAGS $ace_no_xopen"
        AC_EGREP_HEADER([[^_]+pread], [unistd.h],
          [
           ace_cv_lib_has_pread_prototype=yes
          ],
          [
           ace_cv_lib_has_pread_prototype=no
          ])
        dnl Reset the compiler flags
        CPPFLAGS="$ace_save_CPPFLAGS"
       ],,[AC_DEFINE([ACE_LACKS_PREAD_PROTOTYPE])])
    ],)],)

AC_CHECK_FUNC([readv], , [AC_DEFINE([ACE_LACKS_READV])])

AC_CHECK_FUNC([writev], , [AC_DEFINE([ACE_LACKS_WRITEV])])

AC_CHECK_FUNC([set_t_errno], [AC_DEFINE([ACE_HAS_SET_T_ERRNO])],)

AC_CHECK_FUNC([sigsuspend],
  [AC_DEFINE([ACE_HAS_SIGSUSPEND])],)

AC_CHECK_FUNC([sigtimedwait],
  [AC_DEFINE([ACE_HAS_SIGTIMEDWAIT])],)

AC_CHECK_FUNC([socketpair],
  ,
  [AC_DEFINE([ACE_LACKS_SOCKETPAIR])])

AC_CHECK_FUNC([strcasecmp],
  ,
  [AC_DEFINE([ACE_LACKS_STRCASECMP])])

AC_CHECK_FUNC([strdup],
  ,
  [AC_DEFINE([ACE_HAS_STRDUP_EMULATION])])

AC_CHECK_FUNC([strrchr],
  ,
  [AC_DEFINE([ACE_LACKS_STRRCHR])])

AC_CHECK_FUNC([strptime],
  [
   AC_DEFINE([ACE_HAS_STRPTIME])
   dnl Check if _XOPEN_SOURCE macro is needed to make the strptime()
   dnl prototype visible.
   ACE_CACHE_CHECK([for strptime prototype],
     [ace_cv_lib_has_strptime_prototype],
     [
      ace_save_CPPFLAGS="$CPPFLAGS"
      ace_no_xopen="-U_XOPEN_SOURCE"
      CPPFLAGS="$CPPFLAGS $ace_no_xopen"
      AC_EGREP_HEADER([[^_]+strptime],[time.h],
        [
         ace_cv_lib_has_strptime_prototype=yes
        ],
        [
         ace_cv_lib_has_strptime_prototype=no
        ])
      dnl Reset the compiler flags
      CPPFLAGS="$ace_save_CPPFLAGS"
     ],,[AC_DEFINE([ACE_LACKS_STRPTIME_PROTOTYPE])])
  ],
  [
   AC_DEFINE([ACE_LACKS_NATIVE_STRPTIME])
  ])

AC_CHECK_FUNC([memchr],
  [AC_DEFINE([ACE_HAS_MEMCHR])],)

if test "$ace_cv_type_wchar_t" = yes; then
  AC_CHECK_FUNC([wcslen],
    [AC_DEFINE([ACE_HAS_XPG4_MULTIBYTE_CHAR])],)
fi

AC_CHECK_FUNC([syscall],
  ,
  [AC_DEFINE([ACE_LACKS_SYSCALL])])

AC_CHECK_FUNC([ualarm],
  [AC_DEFINE([ACE_HAS_UALARM])],)

AC_CHECK_FUNC([alarm],,)
AC_CHECK_FUNC([signal],,)

if test "$ac_cv_func_alarm" != yes &&
   test "$ac_cv_func_signal" != yes; then
  AC_DEFINE([ACE_LACKS_UNIX_SIGNALS])
fi

AC_CHECK_FUNC([fork],
  ,
  [AC_DEFINE([ACE_LACKS_FORK])])

AC_CHECK_FUNC([getrlimit],
  [
   AC_CHECK_FUNC([setrlimit],
     ,
     [AC_DEFINE([ACE_LACKS_RLIMIT])])
  ],
  [
   AC_DEFINE([ACE_LACKS_RLIMIT])
  ])

AC_CHECK_FUNC([sysinfo],
  [
   if test "$ac_cv_header_sys_systeminfo_h" = yes; then
     AC_DEFINE([ACE_HAS_SYSINFO])
   fi
  ],)

AC_CHECK_FUNC([strerror],
  [AC_DEFINE([ACE_HAS_STRERROR])],)

AC_CHECK_FUNC([lstat],
  ,
  [AC_DEFINE([ACE_LACKS_LSTAT])])

AC_CHECK_FUNC([readlink],
  ,
  [AC_DEFINE([ACE_LACKS_READLINK])])

AC_CHECK_FUNC([rename],
  ,
  [AC_DEFINE([ACE_LACKS_RENAME])])

AC_CHECK_FUNC([recvmsg],
  ,
  [AC_DEFINE([ACE_LACKS_RECVMSG])])

AC_CHECK_FUNC([sendmsg],
  ,
  [AC_DEFINE([ACE_LACKS_SENDMSG])])

if test "$ace_has_tli" = yes; then
  AC_CHECK_FUNC([t_getname],
    [AC_DEFINE([ACE_HAS_SVR4_TLI])],)
fi

AC_CHECK_FUNC([getrusage],
  [AC_DEFINE([ACE_HAS_GETRUSAGE])],)

AC_CHECK_FUNC([getpgid],
  [
   dnl Check if _XOPEN_SOURCE and _XOPEN_SOURCE_EXTENDED macros are
   dnl needed to make the getpgid() prototype visible.
   ACE_CACHE_CHECK([for getpgid prototype],
     [ace_cv_lib_has_getpgid_prototype],
     [
      ace_save_CPPFLAGS="$CPPFLAGS"
      ace_no_xopen="-U_XOPEN_SOURCE -U_XOPEN_SOURCE_EXTENDED"
      CPPFLAGS="$CPPFLAGS $ace_no_xopen"
      AC_EGREP_HEADER([[^_]+getpgid], [unistd.h],
        [
         ace_cv_lib_has_getpgid_prototype=yes
        ],
        [
         ace_cv_lib_has_getpgid_prototype=no
        ])
      dnl Reset the compiler flags
      CPPFLAGS="$ace_save_CPPFLAGS"
     ],, [AC_DEFINE([ACE_LACKS_GETPGID_PROTOTYPE])])
  ],
  [
   AC_DEFINE([ACE_LACKS_GETPGID])
  ])

AC_CHECK_FUNC([getppid],
  ,
  AC_DEFINE([ACE_LACKS_GETPPID]))

AC_CHECK_FUNC([setregid],
  [
   dnl Check if _XOPEN_SOURCE and _XOPEN_SOURCE_EXTENDED macros are
   dnl needed to make the setregid() prototype visible.
   ACE_CACHE_CHECK([for setregid prototype],
     [ace_cv_lib_has_setregid_prototype],
     [
      ace_save_CPPFLAGS="$CPPFLAGS"
      ace_no_xopen="-U_BSD_SOURCE -U_XOPEN_SOURCE -U_XOPEN_SOURCE_EXTENDED"
      CPPFLAGS="$CPPFLAGS $ace_no_xopen"
      AC_EGREP_HEADER([[^_]+setregid], [time.h],
        [
         ace_cv_lib_has_setregid_prototype=yes
        ],
        [
         ace_cv_lib_has_setregid_prototype=no
        ])
      dnl Reset the compiler flags
      CPPFLAGS="$ace_save_CPPFLAGS"
     ],, [AC_DEFINE([ACE_LACKS_SETREGID_PROTOTYPE])])
  ],
  [
   AC_DEFINE([ACE_LACKS_SETREGID])
  ])

AC_CHECK_FUNC([setreuid],
  [
   dnl Check if _XOPEN_SOURCE and _XOPEN_SOURCE_EXTENDED macros are
   dnl needed to make the setreuid() prototype visible.
   ACE_CACHE_CHECK([for setreuid prototype],
     [ace_cv_lib_has_setreuid_prototype],
     [
      ace_save_CPPFLAGS="$CPPFLAGS"
      ace_no_xopen="-U_BSD_SOURCE -U_XOPEN_SOURCE -U_XOPEN_SOURCE_EXTENDED"
      CPPFLAGS="$CPPFLAGS $ace_no_xopen"
      AC_EGREP_HEADER([[^_]+setreuid], [time.h],
        [
         ace_cv_lib_has_setreuid_prototype=yes
        ],
        [
         ace_cv_lib_has_setreuid_prototype=no
        ])
      dnl Reset the compiler flags
      CPPFLAGS="$ace_save_CPPFLAGS"
     ],, [AC_DEFINE([ACE_LACKS_SETREUID_PROTOTYPE])])
  ],
  [
   AC_DEFINE([ACE_LACKS_SETREUID])
  ])

AC_CHECK_FUNC([getpagesize],
  [AC_DEFINE([ACE_HAS_GETPAGESIZE])],
  [AC_DEFINE([ACE_PAGE_SIZE], [4096])])

AC_CHECK_FUNC([msync],
  ,
  [AC_DEFINE([ACE_LACKS_MSYNC])])

AC_CHECK_FUNC([mprotect],
  ,
  [AC_DEFINE([ACE_LACKS_MPROTECT])])

AC_CHECK_FUNC([mkfifo],
  ,
  [AC_DEFINE([ACE_LACKS_MKFIFO])])

dnl Temporarily select C as the test language to work around a bug in
dnl some versions of Autoconf when C++ is selected as the test
dnl language.
AC_LANG_PUSH([C])
AC_CHECK_FUNC([mktemp],
  ,
  [AC_DEFINE([ACE_LACKS_MKTEMP])])
AC_LANG_POP([C])

AC_CHECK_FUNC([madvise],
  ,
  [AC_DEFINE([ACE_LACKS_MADVISE])])

if test "$ac_cv_header_sys_priocntl_h" = yes; then
  AC_CHECK_FUNC([priocntl],
    [AC_DEFINE([ACE_HAS_PRIOCNTL])],)

dnl Some platforms define priocntl as a macro!
  if test "$ac_cv_func_priocntl" = no; then
    ACE_CACHE_CHECK([for priocntl macro],
      [ace_cv_lib_has_priocntl_macro],
      [
       AC_EGREP_CPP([ACE_PRIOCNTL_MACRO],
         [
#include <sys/priocntl.h>

#if defined (priocntl)
            ACE_PRIOCNTL_MACRO
#endif
         ],
         [
          ace_cv_lib_has_priocntl_macro=yes
         ],
         [
          ace_cv_lib_has_priocntl_macro=no
         ])
      ], [AC_DEFINE([ACE_HAS_PRIOCNTL])],)
  fi dnl test "$ac_cv_func_priocntl" = no
fi dnl test "$ac_cv_header_sys_priocntl_h" = yes

dnl FIXME:  How do we check for a working sbrk()?  Do we need to?
AC_CHECK_FUNC([sbrk],, [AC_DEFINE([ACE_LACKS_SBRK])])

dnl Check for SYSV IPC functions
AC_CHECK_FUNC([msgctl],,)

AC_CHECK_FUNC([msgget],,)

AC_CHECK_FUNC([msgrcv],,)

AC_CHECK_FUNC([semctl],,)

AC_CHECK_FUNC([semget],,)

AC_CHECK_FUNC([semop],,)

AC_CHECK_FUNC([shmat],,)

AC_CHECK_FUNC([shmctl],,)

AC_CHECK_FUNC([shmdt],,)

AC_CHECK_FUNC([shmget],,)
dnl End check for SYSV IPC functions

AC_CHECK_FUNC([read_real_time],
  [AC_DEFINE([ACE_HAS_AIX_HI_RES_TIMER])],)

AC_CHECK_FUNC([shm_open],
  [
   AC_MSG_WARN([platform supports shm_open but shm_open support will not be])
   AC_MSG_WARN([enabled since ACE currently supports it only on LynxOS])
dnl  AC_DEFINE([ACE_HAS_SHM_OPEN])
  ],)

AC_CHECK_FUNC([snprintf],
  [AC_DEFINE([ACE_HAS_SNPRINTF])],)

AC_CHECK_FUNC([tempnam],
  ,
  [AC_DEFINE([ACE_LACKS_TEMPNAM])])

AC_CHECK_FUNC([truncate],
  ,
  [AC_DEFINE([ACE_LACKS_TRUNCATE])])

dnl Check for MIT pthreads APIs for timed send/recv operations
AC_CHECK_FUNC([recv_timedwait],
  [AC_DEFINE([ACE_HAS_RECV_TIMEDWAIT])],)

AC_CHECK_FUNC([recvfrom_timedwait],
  [AC_DEFINE([ACE_HAS_RECVFROM_TIMEDWAIT])],)

AC_CHECK_FUNC([recvmsg_timedwait],
  [AC_DEFINE([ACE_HAS_RECVMSG_TIMEDWAIT])],)

AC_CHECK_FUNC([send_timedwait],
  [AC_DEFINE([ACE_HAS_SEND_TIMEDWAIT])],)

AC_CHECK_FUNC([sendto_timedwait],
  [AC_DEFINE([ACE_HAS_SENDTO_TIMEDWAIT])],)

AC_CHECK_FUNC([sendmsg_timedwait],
  [AC_DEFINE([ACE_HAS_SENDMSG_TIMEDWAIT])],)

AC_CHECK_FUNC([read_timedwait],
  [AC_DEFINE([ACE_HAS_READ_TIMEDWAIT])],)

AC_CHECK_FUNC([readv_timedwait],
  [AC_DEFINE([ACE_HAS_READV_TIMEDWAIT])],)

AC_CHECK_FUNC([write_timedwait],
  [AC_DEFINE([ACE_HAS_WRITE_TIMEDWAIT])],)

AC_CHECK_FUNC([writev_timedwait],
  [AC_DEFINE([ACE_HAS_WRITEV_TIMEDWAIT])],)

dnl Save the cache for debugging purposes
AC_CACHE_SAVE

dnl Check for POSIX Semaphore functions
dnl We only check for a few of them since some platforms don't have these.
AC_CHECK_FUNC([sem_init],,)
AC_CHECK_FUNC([sem_destroy],,)

dnl We only enable POSIX semaphores if we are not using UNIX International
dnl threads.
dnl Enabling POSIX semaphores on Solaris seems to cause problems for
dnl ACE. (NOTE: This may no longer be true after the updates/fixes
dnl I made.  The fixes corrected broken code in ACE_OS::sema_init().
dnl -Ossama)
if test "$ac_cv_func_sem_init" = yes &&
   test "$ac_cv_func_sem_destroy" = yes &&
   test "$ace_cv_type_sem_t" = yes &&
   test "$ace_has_sthreads" != yes; then

dnl Only enable POSIX semaphore support if process shared semaphores
dnl are supported.  Presumably process shared semaphores are only
dnl available if the _POSIX_THREAD_PROCESS_SHARED macro is defined by
dnl the platform.
  AC_EGREP_CPP([WE_HAVE_SHARED_POSIX_SEMAPHORES],
    [
#ifndef _REENTRANT
#define _REENTRANT
#endif

#ifndef _THREAD_SAFE
#define _THREAD_SAFE
#endif

#ifndef ACE_LACKS_UNISTD_H
# include <unistd.h>  /* needed for _POSIX_THREAD_PROCESS_SHARED */
#endif

#include <pthread.h>
#include <semaphore.h>

#if defined (_POSIX_THREAD_PROCESS_SHARED)
WE_HAVE_SHARED_POSIX_SEMAPHORES
#endif
    ],
    [
     AC_DEFINE([ACE_HAS_POSIX_SEM])

     AC_CHECK_FUNC([sem_open],
       [
        dnl Check if it works!  For example, in glibc 2.x sem_open exists
        dnl but it appears to be a stub.  However, it isn't listed as a
        dnl stub in <gnu/stubs.h> so the configure script thinks it is
        dnl implemented!
        ACE_CACHE_CHECK([if sem_open works],
          [ace_cv_sem_open_works],
          [
           AC_RUN_IFELSE([AC_LANG_SOURCE([[
#ifndef ACE_LACKS_SYS_TYPES_H
#include <sys/types.h>
#endif

#include <sys/stat.h>
#include <fcntl.h>

#include <stddef.h>  /* for definition of "NULL" */

#include <semaphore.h>

#ifndef SEM_FAILED
# define SEM_FAILED ((sem_t *) -1)
#endif

int
main ()
{
  sem_t *s = 0;

  s = sem_open ("ace_semaphore_foo", O_CREAT | O_EXCL, 0600, 1);

  if (s == SEM_FAILED)
    return -1;  /* FAILURE */

  if (sem_close (s) != 0)
    return -1;  /* Something went wrong! */

  return 0;
}
             ]])],[
              ace_cv_sem_open_works=yes
             ],[
              ace_cv_sem_open_works=no
             ],[
              dnl action if cross-compiling
              ace_cv_sem_open_works=yes
             ])
          ],, [AC_DEFINE([ACE_LACKS_NAMED_POSIX_SEM])])
       ],
       [
        AC_DEFINE([ACE_LACKS_NAMED_POSIX_SEM])
       ])
    ],)

fi dnl check for POSIX Semaphore functions


dnl The following tests are performed only when the user has enabled
dnl support for threads.

dnl NOTE: Make sure the thread library is in "LIBS"
dnl           (e.g.: LIBS="$LIBS -lpthread")
dnl       otherwise the below thread "CHECK_FUNCs"
dnl       will not work correctly.
if test "$ace_user_enable_threads" = yes; then

 if test "$ace_has_pthreads" = yes; then
dnl Digital UNIX 4.0 "mangles" the following pthread functions:
dnl      pthread_attr_getguardsize_np
dnl      pthread_attr_getinheritsched
dnl      pthread_attr_getstacksize
dnl      pthread_attr_setguardsize_np
dnl      pthread_attr_setinheritsched
dnl      pthread_attr_setstacksize
dnl      pthread_cancel
dnl      pthread_cond_broadcast
dnl      pthread_cond_destroy
dnl      pthread_cond_init
dnl      pthread_cond_sig_preempt_int_np
dnl      pthread_cond_signal
dnl      pthread_cond_signal_int_np
dnl      pthread_cond_timedwait
dnl      pthread_cond_wait
dnl      pthread_create
dnl      pthread_delay_np
dnl      pthread_detach
dnl      pthread_equal
dnl      pthread_exit
dnl      pthread_get_expiration_np
dnl      pthread_getspecific
dnl      pthread_join
dnl      pthread_lock_global_np
dnl      pthread_mutex_destroy
dnl      pthread_mutex_init
dnl      pthread_mutex_lock
dnl      pthread_mutex_trylock
dnl      pthread_mutex_unlock
dnl      pthread_once
dnl      pthread_self
dnl      pthread_setspecific
dnl      pthread_testcancel
dnl      pthread_unlock_global_np
dnl These functions have a double underscore "__" prepended to maintain
dnl backwards compatibility with Pthread Draft 4 functions of the same
dnl name.

  AC_CHECK_FUNC([pthread_sigmask],
    ,
    [AC_DEFINE([ACE_LACKS_PTHREAD_SIGMASK])])

  AC_CHECK_FUNC([pthread_key_create],
    [AC_DEFINE([ACE_HAS_THREAD_SPECIFIC_STORAGE])],
    [
     AC_CHECK_FUNC([pthread_keycreate],
       [AC_DEFINE(ACE_HAS_THREAD_SPECIFIC_STORAGE)],
       [AC_DEFINE(ACE_HAS_TSS_EMULATION)])
    ])

  AC_CHECK_FUNC([pthread_condattr_setkind_np],
    [AC_DEFINE([ACE_HAS_PTHREAD_CONDATTR_SETKIND_NP])],)

  AC_CHECK_FUNC([pthread_mutexattr_setkind_np],
    [AC_DEFINE([ACE_HAS_PTHREAD_MUTEXATTR_SETKIND_NP])],)

  AC_CHECK_FUNC([pthread_condattr_setpshared],
    ,
    [AC_DEFINE([ACE_LACKS_CONDATTR_PSHARED])])

  AC_CHECK_FUNC([pthread_attr_setstackaddr],
    ,
    [AC_DEFINE([ACE_LACKS_THREAD_STACK_ADDR])])

  ACE_CHECK_FUNC([pthread_attr_setstacksize], [pthread.h],
    ,
    [AC_DEFINE([ACE_LACKS_THREAD_STACK_SIZE])])

  ACE_CHECK_FUNC([pthread_cancel], [pthread.h],
    [
     dnl Make sure the prototype actually exists.  Some platforms,
     dnl such as FreeBSD 4, appear to have a missing prototype.  If
     dnl the prototype is missing, then don't use pthread_cancel.
     dnl Creating a prototype for it in ACE is probably a bad idea.

     ace_save_CPPFLAGS="$CPPFLAGS"
     CPPFLAGS="$CPPFLAGS $ACE_THR_CPPFLAGS"

     AC_EGREP_HEADER([pthread_cancel], [pthread.h],,
       [
        AC_DEFINE([ACE_LACKS_PTHREAD_CANCEL])
       ])

     dnl Reset the preprocessor flags
     CPPFLAGS="$ace_save_CPPFLAGS"
    ],
    [
     AC_DEFINE([ACE_LACKS_PTHREAD_CANCEL])
    ])

  AC_CHECK_FUNC([pthread_yield],
    ,
    [AC_DEFINE([ACE_LACKS_PTHREAD_YIELD])])

  AC_CHECK_FUNC([pthread_thr_sigsetmask],
    ,
    [AC_DEFINE([ACE_LACKS_PTHREAD_THR_SIGSETMASK])])

  AC_CHECK_FUNC([pthread_attr_setdetachstate],
    ,
    [AC_DEFINE([ACE_LACKS_SETDETACH])])

  dnl ACE currently doesn't provide enough fine grained control over
  dnl these functions so both must be present in order to prevent
  dnl ACE_LACKS_SETSCHED from being defined.
  AC_CHECK_FUNC([sched_setscheduler],
    [
     AC_CHECK_FUNC([pthread_attr_setschedpolicy],,
      [AC_CHECK_FUNC([pthread_attr_setsched],,
        [AC_DEFINE([ACE_LACKS_SETSCHED])])])
    ],
    [
     AC_DEFINE([ACE_LACKS_SETSCHED])
    ])

  AC_CHECK_FUNC([pthread_attr_setscope],
    ,
    [AC_DEFINE([ACE_LACKS_THREAD_PROCESS_SCOPING])])

  AC_CHECK_FUNC([pthread_mutexattr_setpshared],
    ,
    [AC_DEFINE([ACE_LACKS_MUTEXATTR_PSHARED])])

dnl Check for POSIX Threads Draft 4 functions
  AC_CHECK_FUNC([pthread_mutexattr_create],,)
  AC_CHECK_FUNC([pthread_mutexattr_delete],,)
  AC_CHECK_FUNC([pthread_condattr_delete],,)
  AC_CHECK_FUNC([pthread_condattr_create],,)
  AC_CHECK_FUNC([pthread_setprio],,)
  AC_CHECK_FUNC([pthread_getprio],,)
  AC_CHECK_FUNC([pthread_setcancel],,)
  AC_CHECK_FUNC([pthread_setasynccancel],,)
  AC_CHECK_FUNC([pthread_kill],,)
dnl Check for POSIX Threads Draft 6 functions
  AC_CHECK_FUNC([pthread_attr_setprio],,)
  AC_CHECK_FUNC([pthread_attr_getprio],,)
  AC_CHECK_FUNC([pthread_setintr],,)
  AC_CHECK_FUNC([pthread_setintrtype],,)
dnl Check for POSIX threads Draft 6, 7 and Standard common functions
  AC_CHECK_FUNC([pthread_mutexattr_init],,)
  AC_CHECK_FUNC([pthread_mutexattr_destroy],,)
  AC_CHECK_FUNC([pthread_condattr_init],,)
  AC_CHECK_FUNC([pthread_condattr_destroy],,)
dnl Check for POSIX Threads Draft 7 and Draft Standard common functions
  AC_CHECK_FUNC([pthread_setschedparam],,)
  AC_CHECK_FUNC([pthread_getschedparam],,)
  AC_CHECK_FUNC([pthread_setcancelstate],,)
  AC_CHECK_FUNC([pthread_setcanceltype],,)
dnl Check for POSIX Threads Draft Standard functions
dnl sched_yield() is in the C library or perhaps in "-lposix4."
dnl We need to add other library checks in this script's "check libraries"
dnl section if it is in another library.
dnl  AC_CHECK_FUNC(sched_yield,,)
dnl We already check for this during the library checks.

dnl Check for Unix98 pthreads extensions
  ACE_CACHE_CHECK([for struct pthread_rwlock_t],
    [ace_cv_struct_pthread_rwlock_t],
    [
     dnl Since we are checking for siginfo_t in more than one header
     dnl we can't use the ACE_CHECK_STRUCT macro so we have to do things
     dnl manually.
     ACE_TRY_COMPILE_STRUCT([pthread_rwlock_t], [pthread.h],
       [
        ace_cv_struct_pthread_rwlock_t=yes
       ],
       [
        ACE_TRY_COMPILE_STRUCT([pthread_rwlock_t], [sys/types.h],
          [
           ace_cv_struct_pthread_rwlock_t=yes
          ],
          [
           ace_cv_struct_pthread_rwlock_t=no
          ])
       ])
    ],,)

  ACE_CACHE_CHECK([for struct pthread_rwlockattr_t],
    [ace_cv_struct_pthread_rwlockattr_t],
    [
     dnl Since we are checking for siginfo_t in more than one header
     dnl we can't use the ACE_CHECK_STRUCT macro so we have to do things
     dnl manually.
     ACE_TRY_COMPILE_STRUCT([pthread_rwlockattr_t], [pthread.h],
       [
        ace_cv_struct_pthread_rwlockattr_t=yes
       ],
       [
        ACE_TRY_COMPILE_STRUCT([pthread_rwlockattr_t], [sys/types.h],
          [
           ace_cv_struct_pthread_rwlockattr_t=yes
          ],
          [
           ace_cv_struct_pthread_rwlockattr_t=no
          ])
       ])
    ],,)

  AC_CHECK_FUNC([pthread_continue],,)
  AC_CHECK_FUNC([pthread_suspend],,)
  AC_CHECK_FUNC([pthread_rwlock_init],,)
  AC_CHECK_FUNC([pthread_rwlock_destroy],,)
  AC_CHECK_FUNC([pthread_rwlock_rdlock],,)
  AC_CHECK_FUNC([pthread_rwlock_wrlock],,)
  AC_CHECK_FUNC([pthread_rwlock_unlock],,)
  AC_CHECK_FUNC([pthread_rwlock_tryrdlock],,)
  AC_CHECK_FUNC([pthread_rwlock_trywrlock],,)
  AC_CHECK_FUNC([pthread_rwlockattr_init],,)
  AC_CHECK_FUNC([pthread_rwlockattr_destroy],,)
  AC_CHECK_FUNC([pthread_rwlockattr_setpshared],,)

  if test "$ace_cv_struct_pthread_rwlock_t" = yes &&
     test "$ace_cv_struct_pthread_rwlockattr_t" = yes &&
     test "$ac_cv_func_pthread_continue" = yes &&
     test "$ac_cv_func_pthread_continue" = yes &&
     test "$ac_cv_func_pthread_suspend" = yes &&
     test "$ac_cv_func_pthread_rwlock_init" = yes &&
     test "$ac_cv_func_pthread_rwlock_destroy" = yes &&
     test "$ac_cv_func_pthread_rwlock_rdlock" = yes &&
     test "$ac_cv_func_pthread_rwlock_wrlock" = yes &&
     test "$ac_cv_func_pthread_rwlock_unlock" = yes &&
     test "$ac_cv_func_pthread_rwlock_tryrdlock" = yes &&
     test "$ac_cv_func_pthread_rwlock_trywrlock" = yes &&
     test "$ac_cv_func_pthread_rwlockattr_init" = yes &&
     test "$ac_cv_func_pthread_rwlockattr_destroy" = yes &&
     test "$ac_cv_func_pthread_rwlockattr_setpshared" = yes; then
    AC_DEFINE([ACE_HAS_PTHREADS_UNIX98_EXT])
  fi  dnl Unix98 pthreads extensions

dnl Check if platform has thread_self() rather than pthread_self()
  ACE_CHECK_FUNC([pthread_self], [pthread.h],
                 ,
                 [
                  AC_CHECK_FUNC([thread_self],
                    [
                     AC_DEFINE([ACE_HAS_THREAD_SELF])
                    ],)
                 ])

dnl Check if pthread.h declares an enum with PTHREAD_PROCESS_PRIVATE and
dnl PTHREAD_PROCESS_SHARED values.
  ACE_CACHE_CHECK([for PTHREAD_PROCESS_* enumeration in pthread.h],
                  [ace_cv_lib_pthread_process_enum],
                  [
                   AC_COMPILE_IFELSE([AC_LANG_PROGRAM([[
#include <pthread.h>
                     ]], [[
/* Undefine PTHREAD_PROCESS_SHARED in case some platforms #define it */
#undef PTHREAD_PROCESS_SHARED
int foo = PTHREAD_PROCESS_SHARED;
                     ]])],[
                      ace_cv_lib_pthread_process_enum=yes
                     ],[
                      ace_cv_lib_pthread_process_enum=no
                     ])
                  ],
                  [
                   AC_DEFINE([ACE_HAS_PTHREAD_PROCESS_ENUM])
                  ],)

dnl Check if pthread_create requires an extern "C" start routine
ACE_CONVERT_WARNINGS_TO_ERRORS(
[
ACE_CACHE_CHECK([if pthread_create requires an extern \"C\" start routine],
  [ace_cv_lib_pthread_c_func],[
    AC_COMPILE_IFELSE([AC_LANG_PROGRAM([[
#include <pthread.h>

void *ace_start_routine(void *);
      ]], [[
pthread_create(0, 0, ace_start_routine, 0);
      ]])],[
       ace_cv_lib_pthread_c_func=no
      ],[
       dnl Check if extern "C" start routine is required.
       AC_COMPILE_IFELSE([AC_LANG_PROGRAM(
         [[
#include <pthread.h>

extern "C" void *ace_start_routine(void *);
         ]],
         [[
pthread_create(0, 0, ace_start_routine, 0);
         ]])],
         [
          ace_cv_lib_pthread_c_func=yes
         ],
         [
          ace_cv_lib_pthread_c_func=no
         ])
      ])
  ],
  [
   AC_DEFINE([ACE_HAS_THR_C_FUNC])
  ],)
])  dnl ACE_CONVERT_WARNINGS_TO_ERRORS

dnl Check if pthread_key_create has a standard arg thread destructor
ACE_CACHE_CHECK([if pthread_key_create has std arg thread destructor],
  [ace_cv_lib_pthread_stdarg_dest],[
    if test "$ac_cv_func_pthread_key_create" = yes; then
      ace_pthread_key_create=pthread_key_create
    else
      ace_pthread_key_create=pthread_keycreate
    fi
    AC_COMPILE_IFELSE([AC_LANG_PROGRAM([[
#include <pthread.h>

void ace_destructor(void *);
      ]], [[
${ace_pthread_key_create}(0, ace_destructor);
      ]])],[
       ace_cv_lib_pthread_stdarg_dest=no
      ],[
       AC_COMPILE_IFELSE([AC_LANG_PROGRAM(
         [[
#include <pthread.h>

void ace_destructor(...);
         ]],
         [[
${ace_pthread_key_create}(0, ace_destructor);
         ]])],
         [
          ace_cv_lib_pthread_stdarg_dest=yes
         ],
         [
          ace_cv_lib_pthread_stdarg_dest=no
         ])
      ])
  ],
  [
   AC_DEFINE([ACE_HAS_STDARG_THR_DEST])
  ],)

dnl Check if pthread_key_create requires an extern "C" start routine
ACE_CONVERT_WARNINGS_TO_ERRORS([
ACE_CACHE_CHECK([if pthread_key_create requires an extern \"C\" start routine],
  [ace_cv_lib_pthread_c_dest],[
    if test "$ac_cv_func_pthread_key_create" = yes; then
      ace_pthread_key_create=pthread_key_create
    else
      ace_pthread_key_create=pthread_keycreate
    fi
    AC_COMPILE_IFELSE([AC_LANG_PROGRAM([[
#include <pthread.h>

void ace_destructor(void *);
      ]], [[
${ace_pthread_key_create}(0, ace_destructor);
      ]])],[
       ace_cv_lib_pthread_c_dest=no
      ],[
       dnl Check if extern "C" start routine is required.
       AC_COMPILE_IFELSE([AC_LANG_PROGRAM(
         [[
#include <pthread.h>

extern "C" void ace_destructor(void *);
         ]],
         [[
${ace_pthread_key_create}(0, ace_destructor);
         ]])],
         [
          ace_cv_lib_pthread_c_dest=yes
         ],
         [
          ace_cv_lib_pthread_c_dest=no
         ])
      ])
  ],
  [
   AC_DEFINE([ACE_HAS_THR_C_DEST])
  ],)
])  dnl ACE_CONVERT_WARNINGS_TO_ERRORS


AC_CHECK_FUNC([sched_get_priority_min],,
  [
dnl Check if the PTHREAD_MIN_PRIORITY constant exists.
   AC_COMPILE_IFELSE([AC_LANG_PROGRAM([[
#include <pthread.h>
       ]], [[
 int p = (int) PTHREAD_MIN_PRIORITY;
       ]])
     ],
     [
      dnl Since we have PTHREAD_MIN_PRIORITY, denote that PX_PRIO_MIN
      dnl should not be used.
      ace_has_px_prio_min=no
     ],
     [
      dnl PTHREAD_MIN_PRIORITY doesn't appear to be defined, so
      dnl check if the platform defines PX_PRIO_MIN, instead.

      AC_COMPILE_IFELSE([AC_LANG_PROGRAM([[
#include <pthread.h>
             ]], [[
  int p = (int) PX_PRIO_MIN;
             ]])],
           [
            ace_has_px_prio_min=yes
           ],
           [
            ace_has_px_prio_min=no
           ])
     ])
  ])

 if test "$ace_has_px_prio_min" = yes; then
   AC_DEFINE([PTHREAD_MIN_PRIORITY],
             [PX_PRIO_MIN],
             [Minimum thread priority])
 fi

AC_CHECK_FUNC([sched_get_priority_max],,
  [
dnl Check if the PTHREAD_MAX_PRIORITY constant exists.
   AC_COMPILE_IFELSE([AC_LANG_PROGRAM([[
#include <pthread.h>
       ]], [[
 int p = (int) PTHREAD_MAX_PRIORITY;
       ]])
     ],
     [
      dnl Since we have PTHREAD_MAX_PRIORITY, denote that PX_PRIO_MAX
      dnl should not be used.
      ace_has_px_prio_max=no
     ],
     [
      dnl PTHREAD_MAX_PRIORITY doesn't appear to be defined, so
      dnl check if the platform defines PX_PRIO_MAX, instead.

      AC_COMPILE_IFELSE([AC_LANG_PROGRAM([[
#include <pthread.h>
             ]], [[
  int p = (int) PX_PRIO_MAX;
             ]])],
           [
            ace_has_px_prio_max=yes
           ],
           [
            ace_has_px_prio_max=no
           ])
     ])
  ])

 if test "$ace_has_px_prio_max" = yes; then
   AC_DEFINE([PTHREAD_MAX_PRIORITY],
             [PX_PRIO_MAX],
             [Maximum thread priority])
 fi

 fi dnl test "$ace_has_pthreads" = yes

 if test "$ace_has_sthreads" = yes; then
dnl Only check for these functions if we have the UNIX International
dnl Threads library "thread."
  AC_CHECK_FUNC([thr_keycreate],
    [AC_DEFINE([ACE_HAS_THREAD_SPECIFIC_STORAGE])],
    [AC_DEFINE([ACE_HAS_TSS_EMULATION])])

  AC_CHECK_FUNC([thr_yield],
    [AC_DEFINE([ACE_HAS_THR_YIELD])],)

  AC_CHECK_FUNC([thr_keydelete],
    [AC_DEFINE([ACE_HAS_THR_KEYDELETE])],)

  AC_CHECK_FUNC([thr_min_stack],[],
    [
     AC_CHECK_FUNC([thr_minstack],
       [AC_DEFINE([ACE_HAS_THR_MINSTACK])],)
    ])

 fi dnl test "$ace_has_sthreads" = yes

fi  dnl test "$ace_user_enable_threads" = yes

dnl
dnl By Eric:
dnl ACE will define a sigwait function if we lie and say we don't have
dnl one.  Unfortunately, the ACE function may conflict with our
dnl function, so we'll go ahead and turn this on, even if we are
dnl ignoring threads.
AC_CHECK_FUNC([sigwait],
  [AC_DEFINE([ACE_HAS_SIGWAIT])],)


dnl Check for reentrant functions
if test "$ace_user_enable_reentrant_funcs" = yes; then
  dnl Temporarily select C as the test language to work around a bug in
  dnl some versions of Autoconf when C++ is selected as the test
  dnl language.
  AC_LANG_PUSH([C])
  AC_CHECK_FUNC([rand_r],,)
  AC_LANG_POP([C])

  AC_CHECK_FUNC([strtok_r],
    [
     dnl Check if _POSIX_SOURCE macro is needed to make the strtok_r()
     dnl prototype visible.
     ACE_CACHE_CHECK([for strtok_r prototype],
     [ace_cv_lib_has_strtok_r_prototype],
        [
         ace_save_CPPFLAGS="$CPPFLAGS"
         ace_no_posix="-U_POSIX_SOURCE $ACE_THR_CPPFLAGS"
         CPPFLAGS="$CPPFLAGS $ace_no_posix"
         AC_EGREP_HEADER([[^_]+strtok_r], [string.h],
           [
            ace_cv_lib_has_strtok_r_prototype=yes
           ],
           [
            ace_cv_lib_has_strtok_r_prototype=no
           ])
           dnl Reset the preprocessor flags
           CPPFLAGS="$ace_save_CPPFLAGS"
        ],, [AC_DEFINE([ACE_LACKS_STRTOK_R_PROTOTYPE])])
    ],)

  AC_CHECK_FUNC([getpwnam_r],
                [
                 AC_COMPILE_IFELSE([AC_LANG_PROGRAM([[
#ifndef _REENTRANT
# define _REENTRANT
#endif
#ifdef HAVE_PWD_H
# include <pwd.h>
#endif
#ifndef ACE_LACKS_SYS_TYPES_H
# include <sys/types.h>
#endif
                 ]],
                 [[
  const char * name;
  struct passwd * pwent;
  char * buffer;
  int buflen;
  struct passwd * result;

  int status = getpwnam_r (name, pwent, buffer, buflen, &result);
                 ]])],
                 [
                  if test "$ace_user_enable_reentrant_funcs" = yes; then
                    AC_DEFINE([ACE_HAS_POSIX_GETPWNAM_R])
                  fi
                 ],
                 [
                  dnl Nothing to do!
                 ])
                ],
                [AC_DEFINE([ACE_LACKS_PWD_REENTRANT_FUNCTIONS])])

  AC_CHECK_FUNC([ctime_r],,)

  AC_CHECK_FUNC([localtime_r],,)

  AC_CHECK_FUNC([gmtime_r],,)

  AC_CHECK_FUNC([asctime_r],,)

  AC_CHECK_FUNC([getprotobyname_r],,)

  AC_CHECK_FUNC([getprotobynumber_r],,)

  AC_CHECK_FUNC([gethostbyaddr_r],,)

  AC_CHECK_FUNC([gethostbyname_r],,)

  AC_CHECK_FUNC([getservbyname_r],,)
fi dnl End checks for reentrant functions

AC_CHECK_FUNC([readdir_r],
  ,
  [AC_DEFINE([ACE_LACKS_READDIR_R])])

dnl Disabled until we figure out what to do with the comparator
dnl function argument inconsistencies between different platforms.
dnl For example:
dnl    int comparator (const void * d1, const void * d2)
dnl instead of:
dnl    int comparator (const dirent ** d1, const dirent ** d2)
dnl
dnl AC_CHECK_FUNC([scandir], [AC_DEFINE([ACE_HAS_SCANDIR])], [])

AC_CHECK_FUNC([seekdir],
  ,
  [AC_DEFINE([ACE_LACKS_SEEKDIR])])

AC_CHECK_FUNC([telldir],
  ,
  [AC_DEFINE([ACE_LACKS_TELLDIR])])


dnl
dnl SECTION 11: checks for function characteristics
dnl

ACE_CONVERT_WARNINGS_TO_ERRORS([
dnl Check if dlopen takes a char * arg instead of const char *
 if test "$ace_has_svr4_dynamic_linking" = yes; then
  ACE_CACHE_CHECK([if dlopen takes a char *],
    [ace_cv_lib_charptr_dl],
    [
     dnl Check if it takes a const char *, first.
     AC_COMPILE_IFELSE([AC_LANG_PROGRAM([[
#include <dlfcn.h>
      ]], [[
       const char *filename = 0;
       int flag = 0;
       void *ptr = dlopen(filename, flag);
      ]])],[
       ace_cv_lib_charptr_dl=no
      ],[
       dnl Now check if it takes a non-const char *.
       AC_COMPILE_IFELSE([AC_LANG_PROGRAM(
        [[
#include <dlfcn.h>
        ]],
        [[
         char *filename = 0;
         int flag = 0;
         void *ptr = dlopen(filename, flag);
        ]])],
        [
         ace_cv_lib_charptr_dl=yes
        ],
        [
         ace_cv_lib_charptr_dl=no
        ])
      ])
    ], [AC_DEFINE([ACE_HAS_CHARPTR_DL])],)
 fi dnl test "$ace_has_svr4_dynamic_linking" = yes
]) dnl ACE_CONVERT_WARNINGS_TO_ERRORS

ACE_CONVERT_WARNINGS_TO_ERRORS([
dnl Check if "getby" functions use a non-const  char * argument
 if test "$ac_cv_func_gethostbyaddr" = yes; then
  ACE_CACHE_CHECK([\"getby\" functions take a non-const char *],
    [ace_cv_lib_nonconst_getby],
    [
     AC_COMPILE_IFELSE([AC_LANG_PROGRAM([[
#include <sys/socket.h>
      ]], [[
       char *addr = 0;
       int len = 0;
       int type = 0;
       struct hostent *mystruct = 0;

       mystruct = gethostbyaddr(name, len, type);
      ]])],[
       ace_cv_lib_nonconst_getby=yes
      ],[
       ace_cv_lib_nonconst_getby=no
      ])
    ], [AC_DEFINE([ACE_HAS_NONCONST_GETBY])],)
 fi dnl test "$ac_cv_func_gethostbyaddr" = yes
]) dnl ACE_CONVERT_WARNINGS_TO_ERRORS

dnl Check if new throws exception upon failure
if test "$ace_user_enable_exceptions" = yes; then
  ACE_CACHE_CHECK([if new throws std::bad_alloc exception on failure],
    [ace_cv_new_throws_bad_alloc_exception],
    [
     AC_RUN_IFELSE([AC_LANG_SOURCE([[
#if defined (ACE_HAS_NEW_NO_H)
# include <new>
#elif defined (ACE_HAS_NEW_H)
# include <new.h>
#endif

#if defined (ACE_HAS_STDEXCEPT_NO_H)
# include <stdexcept>
#elif defined (ACE_HAS_EXCEPTION_H)
# include <exception.h>
#endif

        int main(int, char *[]) {
          while (1) {
            try {
              char *a = new char[1024*1024];
              if (a == 0) {
                return 1; /* new() does NOT throw exceptions */
              }
            }

#ifdef ACE_USES_STD_NAMESPACE_FOR_STDCPP_LIB
            catch (std::bad_alloc)
#else
            catch (bad_alloc)
#endif  /* ACE_USES_STD_NAMESPACE_FOR_STDCPP_LIB */
            {
             return 0; /* new() does throw exceptions */
            }
         };

         return 1;  /* ERROR: We shouldn't get this far! */
        }
       ]])],[
        ace_cv_new_throws_bad_alloc_exception=yes
       ],[
        ace_cv_new_throws_bad_alloc_exception=no
       ],[
        ace_cv_new_throws_bad_alloc_exception=no
       ])
    ], [AC_DEFINE([ACE_NEW_THROWS_EXCEPTIONS])],)

  if test "$ace_cv_new_throws_bad_alloc_exception" != yes; then
   ACE_CACHE_CHECK([if new throws xalloc exception on failure],
    [ace_cv_new_throws_xalloc_exception],
    [
     AC_RUN_IFELSE([AC_LANG_SOURCE([[
#if defined (ACE_HAS_NEW_NO_H)
# include <new>
#elif defined (ACE_HAS_NEW_H)
# include <new.h>
#endif

#if defined (ACE_HAS_STDEXCEPT_NO_H)
# include <stdexcept>
#elif defined (ACE_HAS_EXCEPTION_H)
# include <exception.h>
#endif

        int main(int, char *[]) {
          while (1) {
            try {
              char *a = new char[1024*1024];
              if (a == 0) {
                return 1; /* new() does NOT throw exceptions */
              }
            }

            catch (xalloc)
            {
             return 0; /* new() does throw exceptions */
            }
          };

         return 1;  /* ERROR: We shouldn't get this far! */
        }
       ]])],[
        ace_cv_new_throws_xalloc_exception=yes
       ],[
        ace_cv_new_throws_xalloc_exception=no
       ],[
        ace_cv_new_throws_xalloc_exception=no
       ])
     ], [AC_DEFINE([ACE_NEW_THROWS_EXCEPTIONS])],)
  fi dnl ace_cv_new_throws_bad_alloc_exceptions = no
fi dnl $ace_user_enable_exceptions = yes

ACE_CONVERT_WARNINGS_TO_ERRORS([
dnl Check if putmsg takes a const struct strbuf *
dnl If we have getmsg() we can be pretty sure that we have putmsg()
 if test "$ac_cv_func_getmsg" = yes ||
    test "$ac_cv_header_stropts_h" = yes; then
  ACE_CACHE_CHECK([if putmsg takes a const struct strbuf*],
    [ace_cv_lib_const_strbufptr],
    [
     dnl Check if it takes a const struct strbuf *, first.
     AC_COMPILE_IFELSE([AC_LANG_PROGRAM([[
#include <stropts.h>
      ]], [[
       int result = 0;
       int fd = 0;
       const struct strbuf * ace_str = 0;
       int flags = 0;

       result = putmsg(fd, ace_str, ace_str, flags);
      ]])],[
       ace_cv_lib_const_strbufptr=yes
      ],[
       ace_cv_lib_const_strbufptr=no
      ])
    ],, [AC_DEFINE([ACE_LACKS_CONST_STRBUF_PTR])])
 fi dnl "$ac_cv_func_getmsg" = yes || "$ac_cv_header_stropts_h" = yes
]) dnl ACE_CONVERT_WARNINGS_TO_ERRORS

dnl Check if select takes int instead of fd_set
ACE_CONVERT_WARNINGS_TO_ERRORS([
 ACE_CACHE_CHECK([if select takes int instead of fd_set],
  [ace_cv_lib_select_takes_int],
  [
   AC_COMPILE_IFELSE([AC_LANG_PROGRAM([[
#ifndef ACE_LACKS_SYS_TYPES_H
# include <sys/types.h>
#endif
#include <sys/time.h>
#ifndef ACE_LACKS_UNISTD_H
# include <unistd.h>
#endif
#ifdef ACE_HAS_SELECT_H
# include <sys/select.h>
#endif
     ]], [[
      int n = 0;
      fd_set* readfds = 0;
      fd_set* writefds = 0;
      fd_set* exceptfds = 0;
      timeval* timeout = 0;
      int result = 0;

      result = select(n, readfds, writefds, exceptfds, timeout);
     ]])],[
      ace_cv_lib_select_takes_int=no
     ],[
dnl Now check if select will actually take int* arguments
      AC_COMPILE_IFELSE([AC_LANG_PROGRAM(
        [[
#ifndef ACE_LACKS_SYS_TYPES_H
# include <sys/types.h>
#endif
#include <sys/time.h>
#ifndef ACE_LACKS_UNISTD_H
# include <unistd.h>
#endif
#ifdef ACE_HAS_SELECT_H
# include <sys/select.h>
#endif
        ]],
        [[
         int n = 0;
         int* readfds = 0;
         int* writefds = 0;
         int* exceptfds = 0;
         timeval* timeout = 0;
         int result = 0;

         result = select(n, readfds, writefds, exceptfds, timeout);
        ]])],
        [
         ace_cv_lib_select_takes_int=yes
        ],
        [
         ace_cv_lib_select_takes_int=no
         dnl If we get here we don't know what select() takes.
        ])
     ])
  ], [AC_DEFINE([ACE_SELECT_USES_INT])],)
]) dnl ACE_CONVERT_WARNINGS_TO_ERRORS


dnl Check if setrlimit() takes an enum as 1st argument
ACE_CHECK_SETRLIMIT_ENUM

dnl This test fails (i.e. passes when it shouldn't) when compiling with
dnl GCC/G++ since the compiler treats passing a const to a non-const
dnl argument as a warning and not as an error since the const is
dnl simply discarded.  To correct this problem, we use "-Werror" which
dnl converts all warnings to errors, whenever we are compiling with
dnl G++.
dnl        -Ossama

ACE_CONVERT_WARNINGS_TO_ERRORS(
[
dnl Check if setrlimit() takes a const pointer as 2nd argument
ACE_CACHE_CHECK([if setrlimit() takes a const pointer as 2nd argument],
  [ace_cv_lib_posix_setrlimit_const_2],[
    AC_COMPILE_IFELSE([AC_LANG_PROGRAM([[
#include <sys/resource.h>
      ]], [[
       const struct rlimit* rlp = 0;
       setrlimit(RLIMIT_CPU, rlp);
      ]])],[
       ace_cv_lib_posix_setrlimit_const_2=yes
      ],[
       ace_cv_lib_posix_setrlimit_const_2=no
      ])
  ],, [AC_DEFINE([ACE_HAS_BROKEN_SETRLIMIT])])
]) dnl ACE_CONVERT_WARNINGS_TO_ERRORS

dnl Check if getrusage() takes an enum as 1st argument
ACE_CHECK_GETRUSAGE_ENUM

dnl TODO: This doesn't work.
dnl The compiler in linux just issues a warning, and the test passes!!!
dnl
dnl FIXED by adding "-Werror" to compiler flags when using GNU C++
dnl                                 -Ossama
ACE_CONVERT_WARNINGS_TO_ERRORS(
[
dnl Check if select takes a const fifth argument
ACE_CACHE_CHECK([if select takes a const fifth argument],
  [ace_cv_lib_posix_select_const_5],[
    AC_COMPILE_IFELSE([AC_LANG_PROGRAM([[
#ifndef ACE_LACKS_SYS_TYPES_H
# include <sys/types.h>
#endif
#include <sys/time.h>
#ifndef ACE_LACKS_UNISTD_H
# include <unistd.h>
#endif
#ifdef ACE_HAS_SELECT_H
# include <sys/select.h>
#endif
      ]], [[
       int n = 0;
       fd_set *readfds = 0;
       fd_set *writefds = 0;
       fd_set *exceptfds = 0;
       const struct timeval* timeout = 0;
       select(n, readfds, writefds, exceptfds, timeout);
      ]])],[
       ace_cv_lib_posix_select_const_5=yes
      ],[
       ace_cv_lib_posix_select_const_5=no
      ])
  ], , [AC_DEFINE([ACE_HAS_NONCONST_SELECT_TIMEVAL])])
]) dnl ACE_CONVERT_WARNINGS_TO_ERRORS

dnl Only run the following tests if the msghdr structure exists.
if test "$ace_cv_struct_msghdr" = yes &&
   test "$ac_cv_func_sendmsg" = yes; then
 ACE_CONVERT_WARNINGS_TO_ERRORS(
 [
dnl Check if sendmsg takes a const 2nd argument
 ACE_CACHE_CHECK([if sendmsg omits the const from the second argument],
  [ace_cv_lib_broken_sendmsg],[
    AC_COMPILE_IFELSE([AC_LANG_PROGRAM([[
#ifndef ACE_LACKS_SYS_TYPES_H
# include <sys/types.h>
#endif
#include <sys/socket.h>
      ]], [[
       int s = 0;
       const struct msghdr *msg = 0;
       unsigned int flags = 0;

       int result = 0;

       result = (int) sendmsg(s, msg, flags);
      ]])],[
       ace_cv_lib_broken_sendmsg=no
      ],[
       ace_cv_lib_broken_sendmsg=yes
      ])
   ], [AC_DEFINE([ACE_HAS_BROKEN_SENDMSG])],)
 ]) dnl ACE_CONVERT_WARNINGS_TO_ERRORS
fi dnl test "$ace_cv_struct_msghdr" = yes

dnl Only run the following tests if the writev function exists
if test "$ac_cv_header_sys_uio_h" = yes &&
   test "$ac_cv_func_writev" = yes; then
 ACE_CONVERT_WARNINGS_TO_ERRORS(
 [
dnl Check if writev omits the const from the iovec argument
 ACE_CACHE_CHECK([if writev omits the const from the iovec argument],
  [ace_cv_lib_broken_writev],[
    AC_COMPILE_IFELSE([AC_LANG_PROGRAM([[
#ifdef ACE_HAS_UNISTD_H
# include <unistd.h>
#endif

#include <sys/uio.h>
      ]], [[
       int filedes = 0;
       const struct iovec *vector = 0;
       size_t count = 0;

       int result = 0;

       result = (int) writev(filedes, vector, count);
      ]])],[
       ace_cv_lib_broken_writev=no
      ],[
       ace_cv_lib_broken_writev=yes
      ])
   ], [AC_DEFINE([ACE_HAS_BROKEN_WRITEV])],)
 ]) dnl ACE_CONVERT_WARNINGS_TO_ERRORS
fi dnl $ac_cv_header_sys_uio_h = yes && $ac_cv_func_writev = yes


ACE_CONVERT_WARNINGS_TO_ERRORS(
[
ACE_CACHE_CHECK([for (struct sockaddr *) msg_name field in msghdr],
  [ace_cv_lib_sockaddr_msg_name],[
    AC_COMPILE_IFELSE([AC_LANG_PROGRAM([[
#ifndef ACE_LACKS_SYS_TYPES_H
# include <sys/types.h>
#endif
#include <sys/socket.h>
      ]], [[
       msghdr ace_msghdr;
       struct sockaddr *addr = 0;

       /*
        * Note that some platforms declare msg_name to be a void*,
        * in which case this assignment will work.
        * Should we _not_ define ACE_HAS_SOCKADDR_MSG_NAME in that
        * case?  I tend to think it is more appropriate to define
        * ACE_HAS_SOCKADDR_MSG_NAME rather than cast addr to a char*,
        * as is done in ACE when the macro is not defined.
        *      -Ossama
        */
       ace_msghdr.msg_name = (struct sockaddr *)addr;
      ]])],[
       ace_cv_lib_sockaddr_msg_name=yes
      ],[
       ace_cv_lib_sockaddr_msg_name=no
      ])
  ], [AC_DEFINE([ACE_HAS_SOCKADDR_MSG_NAME])],)
]) dnl ACE_CONVERT_WARNINGS_TO_ERRORS


ACE_CONVERT_WARNINGS_TO_ERRORS(
[
ACE_CACHE_CHECK([if setsockopt() takes a void* fourth argument],
  [ace_cv_lib_posix_setsockopt_voidp_4],
  [
   AC_COMPILE_IFELSE([AC_LANG_PROGRAM([[
#ifndef ACE_LACKS_SYS_TYPES_H
# include <sys/types.h>
#endif
#include <sys/socket.h>
     ]], [[
      int s = 0;
      int level = 0;
      int optname = 0;
      void* optval = 0;

#if defined (ACE_HAS_SOCKLEN_T)
      socklen_t optlen = 0;
#elif defined (ACE_HAS_SIZET_SOCKET_LEN)
      size_t optlen = 0;
#else
      int optlen = 0;
#endif

      setsockopt (s, level, optname, optval, optlen);
     ]])],[
      ace_cv_lib_posix_setsockopt_voidp_4=yes
     ],[
      ace_cv_lib_posix_setsockopt_voidp_4=no
     ])
  ],
  [
   AC_DEFINE([ACE_HAS_VOIDPTR_SOCKOPT])
  ],
  [
   ACE_CACHE_CHECK([if setsockopt() takes a char* fourth argument],
     [ace_cv_lib_posix_setsockopt_charp_4],
     [
      AC_COMPILE_IFELSE([AC_LANG_PROGRAM([[
#ifndef ACE_LACKS_SYS_TYPES_H
# include <sys/types.h>
#endif
#include <sys/socket.h>
        ]], [[
         int s = 0;
         int level = 0;
         int optname = 0;
         char* optval = 0;

#if defined (ACE_HAS_SOCKLEN_T)
         socklen_t optlen = 0;
#elif defined (ACE_HAS_SIZET_SOCKET_LEN)
         size_t optlen = 0;
#else
         int optlen = 0;
#endif

         setsockopt (s, level, optname, optval, optlen);
        ]])],[
         ace_cv_lib_posix_setsockopt_charp_4=yes
        ],[
         ace_cv_lib_posix_setsockopt_charp_4=no
         ])
     ],
     [
      AC_DEFINE([ACE_HAS_CHARPTR_SOCKOPT])
     ],)
  ])
]) dnl ACE_CONVERT_WARNINGS_TO_ERRORS

ACE_CONVERT_WARNINGS_TO_ERRORS(
[
ACE_CACHE_CHECK([if mmap() takes a void* first argument],
  [ace_cv_lib_posix_voidptr_mmap],[
    AC_COMPILE_IFELSE([AC_LANG_PROGRAM([[
#ifndef ACE_LACKS_SYS_TYPES_H
# include <sys/types.h>
#endif
#ifndef ACE_LACKS_UNISTD_H
# include <unistd.h>
#endif
#include <sys/mman.h>
      ]], [[
       void *start = 0;
       size_t length = 0;
       int prot = 0;
       int flags = 0;
       int fd = 0;
       off_t offset = 0;

       void *result = 0;

       result = (void *)mmap(start, length, prot, flags, fd, offset);
      ]])],[
       ace_cv_lib_posix_voidptr_mmap=yes
      ],[
       ace_cv_lib_posix_voidptr_mmap=no
      ])
  ], [AC_DEFINE([ACE_HAS_VOIDPTR_MMAP])],)
]) dnl ACE_CONVERT_WARNINGS_TO_ERRORS

dnl Check if msync is broken, i.e. doesn't accept third "sync" argument.
dnl This test should come after the test for checking if mmap takes a void*
dnl argument since we need the ACE_MMAP_TYPE type for this test.
if test "$ac_cv_func_msync" = yes; then
ACE_CACHE_CHECK([if msync accepts a third \"sync\" argument],
  [ace_cv_lib_broken_msync],
  [
   AC_COMPILE_IFELSE([AC_LANG_PROGRAM([[
#include <sys/mman.h>

#if defined (ACE_HAS_VOIDPTR_MMAP)
      /* Needed for some odd OS's (e.g., SGI). */
      typedef void *ACE_MMAP_TYPE;
#else
      typedef char *ACE_MMAP_TYPE;
#endif /* ACE_HAS_VOIDPTR_MMAP */
     ]], [[
      ACE_MMAP_TYPE start = 0;
      size_t length = 0;
      int flags = MS_SYNC;

      int result;

      result = msync(start, length, flags);
     ]])],[
      ace_cv_lib_broken_msync=no
     ],[
      dnl Now see if it works with just two arguments
      AC_COMPILE_IFELSE([AC_LANG_PROGRAM(
        [[
#include <sys/mman.h>

#if defined (ACE_HAS_VOIDPTR_MMAP)
         /* Needed for some odd OS's (e.g., SGI). */
         typedef void *ACE_MMAP_TYPE;
#else
         typedef char *ACE_MMAP_TYPE;
#endif /* ACE_HAS_VOIDPTR_MMAP */
        ]],
        [[
         ACE_MMAP_TYPE start = 0;
         size_t length = 0;

         int result;

         result = msync(start, length);
        ]])],
        [
         ace_cv_lib_broken_msync=yes
        ],
        [
         dnl If we get to this point then we don't know if msync is
         dnl is broken or not.
         ace_cv_lib_broken_msync=no
        ])
     ])
  ], [AC_DEFINE([ACE_HAS_BROKEN_NETBSD_MSYNC])],)
fi  dnl test "$ac_cv_func_msync" = yes

dnl Check if platform has iostream method ipfx()
ACE_CACHE_CHECK([for iostream method ipfx()],
  [ace_cv_feature_has_iostream_ipfx],[
    AC_COMPILE_IFELSE([AC_LANG_PROGRAM([[
#include <iostream.h>
      ]], [[
       cin.ipfx();
      ]])],[
       ace_cv_feature_has_iostream_ipfx=yes
      ],[
       ace_cv_feature_has_iostream_ipfx=no
      ])
  ], , [AC_DEFINE([ACE_LACKS_IOSTREAM_FX])])

dnl Check if platform has line-buffered streambufs
ACE_CACHE_CHECK([for line-buffered streambufs],
  [ace_cv_feature_has_linebuffered_streambuf],[
    AC_COMPILE_IFELSE([AC_LANG_PROGRAM([[
#include <iostream.h>
      ]], [[
       cin.rdbuf()->linebuffered(1);
      ]])],[
       ace_cv_feature_has_linebuffered_streambuf=yes
      ],[
       ace_cv_feature_has_linebuffered_streambuf=no
      ])
  ], , [AC_DEFINE([ACE_LACKS_LINEBUFFERED_STREAMBUF])])

dnl Check if platform has unbuffered streambufs
ACE_CACHE_CHECK([for unbuffered streambufs],
  [ace_cv_feature_has_unbuffered_streambuf],[
    AC_COMPILE_IFELSE([AC_LANG_PROGRAM([[
#include <iostream.h>
      ]], [[
       cin.rdbuf()->unbuffered(1);
      ]])],[
       ace_cv_feature_has_unbuffered_streambuf=yes
      ],[
       ace_cv_feature_has_unbuffered_streambuf=no
      ])
  ], , [AC_DEFINE([ACE_LACKS_UNBUFFERED_STREAMBUF])])


dnl Check if signal takes a void (*)(int) as second argument
ACE_CONVERT_WARNINGS_TO_ERRORS(
[
ACE_CACHE_CHECK([if signal takes a void (*)(int) as second argument],
  [ace_cv_lib_signal_vi1_2],[
    AC_COMPILE_IFELSE([AC_LANG_PROGRAM([[
#include <signal.h>
       typedef void (*SA)(int);
       void handler(int) { }
      ]], [[
       SA nn = handler;
       signal(SIGINT, nn);
      ]])],[
       ace_cv_lib_signal_vi1_2=yes
      ],[
       dnl Check if extern "C" signal handler is required.
       AC_COMPILE_IFELSE([AC_LANG_PROGRAM(
         [[
#include <signal.h>
extern "C"
  {
    typedef void (*SA)(int);
    void handler(int) { }
  }
         ]],
         [[
          SA nn = handler;
          signal(SIGINT, nn);
         ]])],
         [
          ace_cv_lib_signal_vi1_2=yes
         ],
         [
          ace_cv_lib_signal_vi1_2=no
         ])
      ])
  ],
  [
   AC_DEFINE([ACE_HAS_SIG_C_FUNC])
  ],)
])  dnl ACE_CONVERT_WARNINGS_TO_ERRORS

dnl Check if signal takes a void (*)(void) as second argument
ACE_CONVERT_WARNINGS_TO_ERRORS(
[
ACE_CACHE_CHECK([if signal takes a void (*)(void) as second argument],
  [ace_cv_lib_signal_vv1_2],[
    AC_COMPILE_IFELSE([AC_LANG_PROGRAM([[
#include <signal.h>
       typedef void (*SA)(void);
       void handler(void) { }
      ]], [[
       SA nn = handler;
       signal(SIGINT, nn);
      ]])],[
       ace_cv_lib_signal_vv1_2=yes
      ],[
       dnl Check if extern "C" signal handler is required.
       AC_COMPILE_IFELSE([AC_LANG_PROGRAM(
         [[
#include <signal.h>
extern "C"
  {
    typedef void (*SA)(void);
    void handler(void) { }
  }
         ]],
         [[
          SA nn = handler;
          signal(SIGINT, nn);
         ]])],
         [
          ace_cv_lib_signal_vv1_2=yes
         ],
         [
          ace_cv_lib_signal_vv1_2=no
         ])
      ])
  ],
  [
   AC_DEFINE([ACE_HAS_SIG_C_FUNC])
  ])
])  dnl ACE_CONVERT_WARNINGS_TO_ERRORS

dnl Check if signal takes a void (*)(int, ...) as second argument
ACE_CONVERT_WARNINGS_TO_ERRORS(
[
ACE_CACHE_CHECK([if signal takes a void (*)(int, ...) as second argument],
  [ace_cv_lib_signal_vi1a2_2],[
    AC_COMPILE_IFELSE([AC_LANG_PROGRAM([[
#include <signal.h>
       typedef void (*SA)(int, ...);
       void handler(int, ...) { }
      ]], [[
       SA nn = handler;
       signal(SIGINT, nn);
      ]])],[
       ace_cv_lib_signal_vi1a2_2=yes
      ],[
       dnl Check if extern "C" signal handler is required.
       AC_COMPILE_IFELSE([AC_LANG_PROGRAM(
         [[
#include <signal.h>
extern "C"
  {
    typedef void (*SA)(int, ...);
    void handler(int, ...) { }
  }
         ]],
         [[
          SA nn = handler;
          signal(SIGINT, nn);
         ]])],
         [
          ace_cv_lib_signal_vi1a2_2=yes
         ],
         [
          ace_cv_lib_signal_vi1a2_2=no
         ])
      ])
  ],
  [
   AC_DEFINE([ACE_HAS_SIG_C_FUNC])
  ],)
])  dnl ACE_CONVERT_WARNINGS_TO_ERRORS

dnl Check if signal takes a void (*)(...) as second argument
ACE_CONVERT_WARNINGS_TO_ERRORS(
[
ACE_CACHE_CHECK([if signal takes a void (*)(...) as second argument],
  [ace_cv_lib_signal_va1_2],[
    AC_COMPILE_IFELSE([AC_LANG_PROGRAM([[
#include <signal.h>
       typedef void (*SA)(...);
       void handler(...) { }
      ]], [[
       SA nn = handler;
       signal(SIGINT, nn);
      ]])],[
       ace_cv_lib_signal_va1_2=yes
      ],[
       dnl Check if extern "C" signal handler is required.
       AC_COMPILE_IFELSE([AC_LANG_PROGRAM(
         [[
#include <signal.h>
extern "C"
  {
    typedef void (*SA)(...);
    void handler(...) { }
  }
         ]],
         [[
          SA nn = handler;
          signal(SIGINT, nn);
         ]])],
         [
          ace_cv_lib_signal_va1_2=yes
         ],
         [
          ace_cv_lib_signal_va1_2=no
         ])
      ])
  ],
  [
   AC_DEFINE([ACE_HAS_SIG_C_FUNC])
  ],)
])  dnl ACE_CONVERT_WARNINGS_TO_ERRORS

dnl Check if signal returns a void (*)(int)
AC_CACHE_CHECK([if signal returns a void (*)(int)],
  [ace_cv_lib_signal_vi1_ret],[
    AC_COMPILE_IFELSE([AC_LANG_PROGRAM([[
#include <signal.h>

typedef void (*SA)(int);
void foo(SA nn) { }
      ]], [[
SA nn = SIG_DFL;
nn = signal(SIGINT, 0);
foo(nn);
      ]])],[
       ace_cv_lib_signal_vi1_ret=yes
      ],[
       ace_cv_lib_signal_vi1_ret=no
      ])
  ])

dnl Check if signal returns a void (*)(void)
AC_CACHE_CHECK([if signal returns a void (*)(void)],
  [ace_cv_lib_signal_vv1_ret],[
    AC_COMPILE_IFELSE([AC_LANG_PROGRAM([[
#include <signal.h>

typedef void (*SA)(void);
void foo(SA nn) { }
      ]], [[
SA nn = SIG_DFL;
nn = signal(SIGINT, 0);
foo(nn);
      ]])],[
       ace_cv_lib_signal_vv1_ret=yes
      ],[
       ace_cv_lib_signal_vv1_ret=no
      ])
  ])

dnl Check if signal returns a void (*)(int, ...)
AC_CACHE_CHECK([if signal returns a void (*)(int, ...)],
  [ace_cv_lib_signal_vi1a2_ret],[
    AC_COMPILE_IFELSE([AC_LANG_PROGRAM([[
#include <signal.h>

typedef void (*SA)(int, ...);
      ]], [[
       SA oo = signal(SIGINT, 0);
      ]])],[
       ace_cv_lib_signal_vi1a2_ret=yes
      ],[
       ace_cv_lib_signal_vi1a2_ret=no
      ])
  ])

dnl Check if signal returns a void (*)(...)
AC_CACHE_CHECK([if signal returns a void (*)(...)],
  [ace_cv_lib_signal_va1_ret],[
    AC_COMPILE_IFELSE([AC_LANG_PROGRAM([[
#include <signal.h>

typedef void (*SA)(...);
      ]], [[
       SA oo = signal(SIGINT, 0);
      ]])],[
        ace_cv_lib_signal_va1_ret=yes
      ],[
       ace_cv_lib_signal_va1_ret=no
      ])
  ])

if test "$ace_cv_struct_sigaction" = yes; then
dnl Check if struct sigaction takes a void (*)(int) handler
  AC_CACHE_CHECK([if struct sigaction takes a void (*)(int) handler],
    [ace_cv_lib_struct_sigaction_vi1_handler],[
      AC_COMPILE_IFELSE([AC_LANG_PROGRAM([[
#include <signal.h>
         typedef void (*SA)(int);
         void foo(struct sigaction* sa, SA nn) { }
        ]], [[
         struct sigaction sa;
         SA nn = SIG_DFL;
         sa.sa_handler = nn;
         foo(&sa, nn);
        ]])],[
         ace_cv_lib_struct_sigaction_vi1_handler=yes
        ],[
         ace_cv_lib_struct_sigaction_vi1_handler=no
        ])
    ])

dnl Check if struct sigaction takes a void (*)(void) handler
  AC_CACHE_CHECK([if struct sigaction takes a void (*)(void) handler],
    [ace_cv_lib_struct_sigaction_vv1_handler],[
      AC_COMPILE_IFELSE([AC_LANG_PROGRAM([[
#include <signal.h>
         typedef void (*SA)(void);
         void foo(struct sigaction* sa, SA nn) { }
        ]], [[
         struct sigaction sa;
         SA nn = SIG_DFL;
         sa.sa_handler = nn;
         foo(&sa, nn);
        ]])],[
         ace_cv_lib_struct_sigaction_vv1_handler=yes
        ],[
         ace_cv_lib_struct_sigaction_vv1_handler=no
        ])
    ])

dnl Check if struct sigaction takes a void (*)(int, ...) handler
  AC_CACHE_CHECK([if struct sigaction takes a void (*)(int, ...) handler],
    [ace_cv_lib_struct_sigaction_vi1a2_handler],[
      AC_COMPILE_IFELSE([AC_LANG_PROGRAM([[
#include <signal.h>
         typedef void (*SA)(int, ...);
         void foo(struct sigaction* sa, SA nn) { }
        ]], [[
         struct sigaction sa;
         SA nn = SIG_DFL;
         sa.sa_handler = nn;
         foo(&sa, nn);
        ]])],[
         ace_cv_lib_struct_sigaction_vi1a2_handler=yes
        ],[
         ace_cv_lib_struct_sigaction_vi1a2_handler=no
        ])
    ])

dnl Check if struct sigaction takes a void (*)(...) handler
  AC_CACHE_CHECK([if struct sigaction takes a void (*)(...) handler],
    [ace_cv_lib_struct_sigaction_va1_handler],[
      AC_COMPILE_IFELSE([AC_LANG_PROGRAM([[
#include <signal.h>
         typedef void (*SA)(...);
         void foo(struct sigaction* sa, SA nn) { }
        ]], [[
         struct sigaction sa;
         SA nn = SIG_DFL;
         sa.sa_handler = nn;
         foo(&sa, nn);
        ]])],[
         ace_cv_lib_struct_sigaction_va1_handler=yes
        ],[
         ace_cv_lib_struct_sigaction_va1_handler=no
        ])
    ])
fi dnl test "$ace_cv_struct_sigaction" = yes

dnl TODO: This doesn't work.
dnl The linux compiler issues a warning regarding the invalid void*
dnl conversion.
dnl
dnl FIXED by adding "-Werror" to compiler flags when using GNU C++
dnl                                 -Ossama
ACE_CONVERT_WARNINGS_TO_ERRORS(
[
dnl Check if msgsnd() takes a struct msgbuf* second argument
ACE_CACHE_CHECK([if msgsnd() takes a struct msgbuf* second argument],
  [ace_cv_lib_posix_msgsnd_msgbufp_2],[
    AC_COMPILE_IFELSE([AC_LANG_PROGRAM([[
#ifndef ACE_LACKS_SYS_TYPES_H
# include <sys/types.h>
#endif
# include <sys/ipc.h>
#ifndef ACE_LACKS_SYSV_MSG_H
# include <sys/msg.h>
#endif
      ]], [[
       int msqid = 0;
       struct msgbuf* msgp = 0;
       int msgsz = 0;
       int msgflg = 0;
       msgsnd(msqid, msgp, msgsz, msgflg);
      ]])],[
       ace_cv_lib_posix_msgsnd_msgbufp_2=yes
      ],[
       ace_cv_lib_posix_msgsnd_msgbufp_2=no
      ])
  ],
  [
   dnl "ACTIONS-IF-SUCCESSFUL" handled later in configure.in
  ],
  [
   dnl Check if msgsnd() takes a const void* second argument
   ACE_CACHE_CHECK([if msgsnd() takes a const void* second argument],
     [ace_cv_lib_posix_msgsnd_cvoidp_2],[
       AC_COMPILE_IFELSE([AC_LANG_PROGRAM([[
#ifndef ACE_LACKS_SYS_TYPES_H
# include <sys/types.h>
#endif
# include <sys/ipc.h>
#ifndef ACE_LACKS_SYSV_MSG_H
# include <sys/msg.h>
#endif
         ]], [[
          int msqid = 0;
          const void* msgp = 0;
          int msgsz = 0;
          int msgflg = 0;
          msgsnd(msqid, msgp, msgsz, msgflg);
         ]])],[
          ace_cv_lib_posix_msgsnd_cvoidp_2=yes
         ],[
          ace_cv_lib_posix_msgsnd_cvoidp_2=no
         ])
     ],
     [
      dnl Do nothing if msgsnd takes a const void* second argument
     ],
     [
      dnl If we get this far we presumably have a non-const void* second param
      AC_DEFINE([ACE_HAS_NONCONST_MSGSND])
     ])
  ])
]) dnl ACE_CONVERT_WARNINGS_TO_ERRORS

dnl TODO: This doesn't work.
dnl The linux compiler issues a warning regarding the invalid void*
dnl conversion.
dnl
dnl FIXED by adding "-Werror" to compiler flags when using GNU C++
dnl                                 -Ossama
ACE_CONVERT_WARNINGS_TO_ERRORS(
[
dnl Check if msgrcv() takes a void* second argument
AC_CACHE_CHECK([if msgrcv() takes a void* second argument],
  [ace_cv_lib_posix_msgrcv_voidp_2],[
    AC_COMPILE_IFELSE([AC_LANG_PROGRAM([[
#ifndef ACE_LACKS_SYS_TYPES_H
# include <sys/types.h>
#endif
# include <sys/ipc.h>
#ifndef ACE_LACKS_SYSV_MSG_H
# include <sys/msg.h>
#endif
      ]], [[
       int msqid = 0;
       void* msgp = 0;
       int msgsz = 0;
       long msgtyp = 0;
       int msgflg = 0;
       msgrcv(msqid, msgp, msgsz, msgtyp, msgflg);
      ]])],[
       ace_cv_lib_posix_msgrcv_voidp_2=yes
      ],[
       ace_cv_lib_posix_msgrcv_voidp_2=no
      ])
  ])
]) dnl ACE_CONVERT_WARNINGS_TO_ERRORS

dnl TODO: This doesn't work.
dnl The linux compiler issues a warning regarding the invalid void*
dnl conversion.
dnl
dnl FIXED by adding "-Werror" to compiler flags when using GNU C++
dnl                                 -Ossama
ACE_CONVERT_WARNINGS_TO_ERRORS(
[
dnl Check if shmat() takes a void* second argument
AC_CACHE_CHECK([if shmat() takes a void* second argument],
  [ace_cv_lib_posix_shmat_voidp_2],[
    AC_COMPILE_IFELSE([AC_LANG_PROGRAM([[
#ifndef ACE_LACKS_SYS_TYPES_H
# include <sys/types.h>
#endif
# include <sys/ipc.h>
# include <sys/shm.h>
      ]], [[
       int shmid = 0;
       void* shmaddr = 0;
       int shmflg = 0;
       shmat(shmid, shmaddr, shmflg);
      ]])],[
       ace_cv_lib_posix_shmat_voidp_2=yes
      ],[
       ace_cv_lib_posix_shmat_voidp_2=no
      ])
  ])
]) dnl ACE_CONVERT_WARNINGS_TO_ERRORS

dnl TODO: This doesn't work.
dnl The linux compiler issues a warning regarding the invalid void*
dnl conversion.
dnl
dnl FIXED by adding "-Werror" to compiler flags when using GNU C++
dnl                                 -Ossama
ACE_CONVERT_WARNINGS_TO_ERRORS(
[
dnl Check if sigaction() takes a const* second argument
AC_CACHE_CHECK([if sigaction() takes a const* second argument],
  [ace_cv_lib_posix_sigaction_constp_2],[
    AC_COMPILE_IFELSE([AC_LANG_PROGRAM([[
#include <signal.h>
      ]], [[
       int signum = 0;
       const struct sigaction* act = 0;
       struct sigaction* oldact = 0;
       sigaction(signum, act, oldact);
      ]])],[
       ace_cv_lib_posix_sigaction_constp_2=yes
      ],[
       ace_cv_lib_posix_sigaction_constp_2=no
      ])
  ])
]) dnl ACE_CONVERT_WARNINGS_TO_ERRORS

dnl Check if the wait() system call takes a (union wait *) rather than int *
ACE_CACHE_CHECK([if wait() takes a (union wait *) rather than int *],
  [ace_cv_lib_wait_takes_union_wait_ptr],
   [
    AC_COMPILE_IFELSE([AC_LANG_PROGRAM([[
#include <sys/wait.h>
       ]], [[
        int* status = 0;
        int result = 0;
        result=wait(status);
       ]])],[
        ace_cv_lib_wait_takes_union_wait_ptr=no
       ],[
        AC_COMPILE_IFELSE([AC_LANG_PROGRAM(
          [[
#include <sys/wait.h>
          ]],
          [[
           union wait* status = 0;
           int result = 0;
           result=wait(status);
          ]])],
          [
           ace_cv_lib_wait_takes_union_wait_ptr=yes
          ],
          [
dnl If we get here then we're screwed! We can't use int* or union wait*.
dnl It is very unlikely that we will ever get this far.  For the sake
dnl of setting some result, we'll assume that we can actually use int*.
           ace_cv_lib_wait_takes_union_wait_ptr=no
           AC_MSG_WARN([wait() doesn't seem to take either int* or union wait*])
           AC_MSG_WARN([as an argument.  Going to assume that int* will work.])
          ])
       ])
  ], [AC_DEFINE([ACE_HAS_UNION_WAIT])],)

dnl We need to use the ACE_CONVERT_WARNINGS_TO_ERRORS() macro since
dnl passing a void * just caused implicit conversion warnings when
dnl using GNU C++, for example.
ACE_CONVERT_WARNINGS_TO_ERRORS(
[
dnl Check for SVR4 style gettimeofday()
AC_CACHE_CHECK([if gettimeofday() takes a void * second argument],
  [ace_cv_lib_voidptr_gettimeofday],
    [
     AC_COMPILE_IFELSE([AC_LANG_PROGRAM([[
#include <sys/time.h>
#ifndef ACE_LACKS_UNISTD_H
# include <unistd.h>
#endif
       ]], [[
        struct timeval *tv = 0;
        void *tzp = 0;

        gettimeofday(tv, tzp);
       ]])],[
        ace_cv_lib_voidptr_gettimeofday=yes
       ],[
        ace_cv_lib_voidptr_gettimeofday=no
       ])
    ])
]) dnl ACE_CONVERT_WARNINGS_TO_ERRORS

if test "$ace_cv_lib_voidptr_gettimeofday" = no; then
ACE_CONVERT_WARNINGS_TO_ERRORS(
[
dnl Check for old OSF1 style gettimeofday()
  AC_CACHE_CHECK([if gettimeofday() takes a timezone * second argument],
    [ace_cv_lib_timezone_gettimeofday],
    [
     AC_COMPILE_IFELSE([AC_LANG_PROGRAM([[
#include <sys/time.h>
#ifndef ACE_LACKS_UNISTD_H
# include <unistd.h>
#endif
       ]], [[
        struct timeval *tv = 0;
        struct timezone *tzp = 0;

        gettimeofday(tv, tzp);
       ]])],[
        ace_cv_lib_timezone_gettimeofday=yes
       ],[
        ace_cv_lib_timezone_gettimeofday=no
       ])
    ])
]) dnl ACE_CONVERT_WARNINGS_TO_ERRORS
fi dnl test "$ace_cv_lib_voidptr_gettimeofday" = no

dnl Check for gettimeofday() protoype
if test "$ace_cv_lib_voidptr_gettimeofday" = yes ||
   test "$ace_cv_lib_timezone_gettimeofday" = yes; then
  ACE_CACHE_CHECK([for gettimeofday() protoype],
    [ace_cv_lib_has_gettimeofday_prototype],
    [
     AC_EGREP_CPP([gettimeofday],
       [
#include <time.h>

#ifdef ACE_LACKS_SYSTIME_H
# include <sys/time.h>
#endif
       ],
       [
dnl We have a gettimeofday prototype
        ace_cv_lib_has_gettimeofday_prototype=yes
       ],
       [
dnl We don't have a gettimeofday prototype
        ace_cv_lib_has_gettimeofday_prototype=no
       ])
    ],
    [
     if test "$ace_cv_lib_voidptr_gettimeofday" = yes; then
       AC_DEFINE([ACE_HAS_SUNOS4_GETTIMEOFDAY])
     else
       AC_DEFINE([ACE_HAS_TIMEZONE_GETTIMEOFDAY])
     fi
    ],
    [
     if test "$ace_cv_lib_voidptr_gettimeofday" = yes; then
       AC_DEFINE([ACE_HAS_SVR4_GETTIMEOFDAY])
     else
       AC_DEFINE([ACE_HAS_OSF1_GETTIMEOFDAY])
     fi
    ])
fi dnl Check for gettimeofday() protoype


dnl Check if ctime_r() takes two arguments
if test "$ac_cv_func_ctime_r" = yes; then
  ACE_CACHE_CHECK([if ctime_r() takes two arguments],
    [ace_cv_lib_posix_ctime_r_2_params],
    [
     AC_COMPILE_IFELSE([AC_LANG_PROGRAM([[
#ifndef _REENTRANT
# define _REENTRANT
#endif

#include <time.h>

#if defined(ACE_LACKS_SYSTIME_H)
# include <sys/time.h>
#endif
       ]], [[
        const time_t *t = 0;
        char *buf;
        ctime_r(t, buf);
       ]])],[
        ace_cv_lib_posix_ctime_r_2_params=yes
       ],[
        ace_cv_lib_posix_ctime_r_2_params=no
       ])
    ], [AC_DEFINE([ACE_HAS_2_PARAM_ASCTIME_R_AND_CTIME_R])],)
fi dnl test "$ac_cv_func_ctime_r" = yes


dnl
dnl SECTION 12: checks for type characteristics
dnl

dnl struct msghdr stuff
dnl Only run the following tests if the msghdr structure exists.
if test "$ace_cv_struct_msghdr" = yes; then

  ACE_CACHE_CHECK([if struct msghdr has a msg_accrights member],
    [ace_cv_lib_posix_struct_msghdr_has_msg_accrights],[
      AC_COMPILE_IFELSE([AC_LANG_PROGRAM([[
#ifndef ACE_LACKS_SYS_TYPES_H
# include <sys/types.h>
#endif
#include <sys/socket.h>
        ]], [[
         msghdr mh;
         mh.msg_accrights = 0;
        ]])],[
         ace_cv_lib_posix_struct_msghdr_has_msg_accrights=yes
        ],[
         ace_cv_lib_posix_struct_msghdr_has_msg_accrights=no
        ])
  ])

  ACE_CACHE_CHECK([if struct msghdr has a msg_accrightslen member],
    [ace_cv_lib_posix_struct_msghdr_has_msg_accrightslen],[
      AC_COMPILE_IFELSE([AC_LANG_PROGRAM([[
#ifndef ACE_LACKS_SYS_TYPES_H
# include <sys/types.h>
#endif
#include <sys/socket.h>
        ]], [[
         msghdr mh;
         mh.msg_accrightslen = 0;
        ]])],[
         ace_cv_lib_posix_struct_msghdr_has_msg_accrightslen=yes
        ],[
         ace_cv_lib_posix_struct_msghdr_has_msg_accrightslen=no
        ])
  ])

dnl Check for 4.4 BSD style struct msghdr members
dnl The following test should only be run if the above two testsfail.
  if test "$ace_cv_lib_posix_struct_msghdr_has_msg_accrights" = no &&
     test "$ace_cv_lib_posix_struct_msghdr_has_msg_accrightslen" = no; then
    AC_DEFINE([ACE_LACKS_MSG_ACCRIGHTS])
    ACE_CACHE_CHECK([for 4.4 BSD style struct msghdr],
      [ace_cv_lib_4_4bsd_msghdr],[
      AC_EGREP_HEADER([msg_control], [sys/socket.h],
        [
         ace_cv_lib_4_4bsd_msghdr=yes
        ],
        [
         ace_cv_lib_4_4bsd_msghdr=no
        ])
    ],
    [
     AC_DEFINE([ACE_HAS_4_4BSD_SENDMSG_RECVMSG])
    ],
    [
     AC_MSG_WARN([No supported msghdr structure was found.  ACE may not compile or function properly.])
    ])
  fi

dnl Now check if CMSG_DATA is defined if and only if we have 4.4 BSD
dnl style struct msghdr members.  If CMSG_DATA isn't defined, then
dnl we try to figure out what it should be defined to.
  if test "$ace_cv_lib_4_4bsd_msghdr" = yes; then
    ACE_CACHE_CHECK([if CMSG_DATA is defined],
      [ace_cv_lib_cmsg_data],
      [
       AC_COMPILE_IFELSE([AC_LANG_PROGRAM(
         [[
#ifndef ACE_LACKS_SYS_TYPES_H
# include <sys/types.h>
#endif
#include <sys/socket.h>
#include <sys/un.h>  /* @@ Is this header necessary? */
         ]],
         [[
           cmsghdr ace_cmsg;
           void * c = CMSG_DATA (&ace_cmsg);
         ]])],
         [
          ace_cv_lib_cmsg_data=yes
         ],
         [
          ace_cv_lib_cmsg_data=no
dnl Now check which type of CMSG_DATA #define we need.
          AC_COMPILE_IFELSE([AC_LANG_PROGRAM([[
#ifndef ACE_LACKS_SYS_TYPES_H
# include <sys/types.h>
#endif
#include <sys/socket.h>
#include <sys/un.h>
            ]], [[
             /* This is sloppy.  Oh well. */
             unsigned char ace_cmsg_data[0];

             cmsghdr ace_cmsg;
             ace_cmsg.cmsg_data = ace_cmsg_data;
            ]])],,[
             AC_COMPILE_IFELSE([AC_LANG_PROGRAM(
               [[
#ifndef ACE_LACKS_SYS_TYPES_H
# include <sys/types.h>
#endif
#include <sys/socket.h>
#include <sys/un.h>
               ]],
               [[
                /* This is sloppy.  Oh well. */
                unsigned char ace_cmsg_data[0];

                cmsghdr ace_cmsg;
                ace_cmsg.__cmsg_data = ace_cmsg_data;
               ]])],
               [
                AC_MSG_ERROR([cmsgmdr member '__cmsg_data[0]' not supported by ACE's CMSG_DATA macro])
               ],
               [
                AC_COMPILE_IFELSE([AC_LANG_PROGRAM(
                  [[
#ifndef ACE_LACKS_SYS_TYPES_H
# include <sys/types.h>
#endif
#include <sys/socket.h>
#include <sys/un.h>
                  ]],
                  [[
                   cmsghdr ace_cmsg;
                  ]])],
                  [
                   AC_CACHE_VAL([ace_cv_lib_cmsg_data_member],
                                [ace_cv_lib_cmsg_data_member=no])
                  ],
                  [
                   AC_MSG_ERROR([No supported cmsghdr structure exists.  ACE cannot be built without one.])
                  ])
               ])
            ])
         ])
      ], ,
      [
       AC_DEFINE([ACE_LACKS_CMSG_DATA_MACRO])
       if test "$ace_cv_lib_cmsg_data_member"=no; then
         AC_DEFINE([ACE_LACKS_CMSG_DATA_MEMBER])
       fi

      ])
  fi dnl test "$ace_cv_lib_4_4bsd_msghdr" = yes

fi dnl End struct msghdr_stuff

dnl
dnl SECTION 13: checks for system services
dnl

dnl Do not run this test if we are using a cross-compiler.
if test "$cross_compiling" != yes; then
  AC_SYS_RESTARTABLE_SYSCALLS
  if test "$ac_cv_sys_restartable_syscalls" = yes; then
    AC_DEFINE([ACE_HAS_SIGNAL_SAFE_OS_CALLS])
  fi
fi

dnl Check if platform defines ctime() as a macro
ACE_CACHE_CHECK([for ctime() macro],
  [ace_cv_feature_have_ctime_macro],
  [
   AC_EGREP_CPP([ACE_CTIME_MACRO],
     [
#include <time.h>

#if defined(ACE_LACKS_SYSTIME_H)
# include <sys/time.h>
#endif

#if defined (ctime)
  ACE_CTIME_MACRO
#endif
     ],
     [
      ace_cv_feature_have_ctime_macro=yes
     ],
     [
      ace_cv_feature_have_ctime_macro=no
     ])
  ], [AC_DEFINE([ACE_HAS_BROKEN_CTIME])],)

dnl Check if platform defines ctime_r, asctime_r, rand_r or getpwnam_r
dnl as macros.
ACE_CACHE_CHECK([for reentrant function macros],
  [ace_cv_feature_has_broken_r_routines],
  [
   AC_EGREP_CPP([ACE_R_MACROS],
     [
#ifndef _REENTRANT
# define _REENTRANT
#endif

#include <time.h>

#if defined(ACE_LACKS_SYSTIME_H)
# include <sys/time.h>
#endif

#if defined (HAVE_PWD_H)
# include <pwd.h>
#endif

#if defined (ctime_r) || \
    defined (asctime_r) || \
    defined (rand_r) || \
    defined (getpwnam_r)
  ACE_R_MACROS
#endif
     ],
     [
      ace_cv_feature_has_broken_r_routines=yes
     ],
     [
      ace_cv_feature_has_broken_r_routines=no
     ])
  ], [AC_DEFINE([ACE_HAS_BROKEN_R_ROUTINES])],)

dnl Check if platform defines sig{empty,fill,add,del}set as macros
ACE_CACHE_CHECK([for sig{empty fill add del}set macros],
  [ace_cv_feature_have_sig_macros],
  [
   AC_EGREP_CPP([ACE_SIG_MACROS],
     [
#ifndef ACE_LACKS_SYS_TYPES_H
# include <sys/types.h>
#endif
#include <signal.h>

#if defined (sigemptyset) && \
    defined (sigfillset) && \
    defined (sigaddset) && \
    defined (sigdelset) && \
    defined (sigismember)
  ACE_SIG_MACROS
#endif
     ],
     [
      ace_cv_feature_have_sig_macros=yes
     ],
     [
      ace_cv_feature_have_sig_macros=no
     ])
  ], [AC_DEFINE([ACE_HAS_SIG_MACROS])],)

dnl Check for open() mode masks
ACE_CACHE_CHECK([for open() mode masks],
  [ace_cv_feature_have_open_mode_masks],[
    AC_EGREP_CPP([ACE_OPEN_MODE_MASKS_EXIST],
      [
#ifndef ACE_LACKS_SYS_TYPES_H
# include <sys/types.h>
#endif
#include <sys/stat.h>
#include <fcntl.h>

/* These are ORed so that ACE will not redefine any of them if any of
   them exist. */
#if defined (S_IRWXU) || \
    defined (S_IRUSR) || \
    defined (S_IWUSR) || \
    defined (S_IXUSR) || \
    defined (S_IRWXG) || \
    defined (S_IRGRP) || \
    defined (S_IWGRP) || \
    defined (S_IXGRP) || \
    defined (S_IRWXO) || \
    defined (S_IROTH) || \
    defined (S_IWOTH) || \
    defined (S_IXOTH)
  ACE_OPEN_MODE_MASKS_EXIST
#endif
      ],
      [
       ace_cv_feature_have_open_mode_masks=yes
      ],
      [
       ace_cv_feature_have_open_mode_masks=no
      ])
  ], , [AC_DEFINE([ACE_LACKS_MODE_MASKS])])


dnl Check if platform supports POSIX O_NONBLOCK semantics
ACE_CACHE_CHECK([for POSIX O_NONBLOCK semantics],
  [ace_cv_feature_posix_o_nonblock],[
    AC_EGREP_CPP([ACE_POSIX_O_NONBLOCK],
      [
#ifndef ACE_LACKS_SYS_TYPES_H
# include <sys/types.h>
#endif
#include <sys/stat.h>
#include <fcntl.h>

#if defined (O_NONBLOCK)
  ACE_POSIX_O_NONBLOCK
#endif
      ],
      [
       ace_cv_feature_posix_o_nonblock=yes
      ],
      [
       ace_cv_feature_posix_o_nonblock=no
      ])
  ], [AC_DEFINE([ACE_HAS_POSIX_NONBLOCK])],)

dnl Check for MAP_FAILED constant
ACE_CACHE_CHECK([for MAP_FAILED constant],
  [ace_cv_lib_have_map_failed],[
    dnl We need the square brackets around "ACEMAPFAILED.+[0-9]" to
    dnl prevent the character class "[0-9]" from becoming "0-9" due to
    dnl M4 quoting.
    AC_EGREP_CPP([ACEMAPFAILED.+[0-9]],
      [
#include <sys/mman.h>
       ACEMAPFAILED MAP_FAILED
      ],
      [
       ace_cv_lib_have_map_failed=yes
      ],
      [
       ace_cv_lib_have_map_failed=no
      ])
  ],
  [
   dnl Check if platform defines MAP_FAILED as a long constant
   ACE_CACHE_CHECK([if MAP_FAILED is a long constant],
     [ace_cv_feature_long_map_failed],[
       dnl We need the square brackets around "ACEMAPFAILED.+[0-9]L" to
       dnl prevent the character class "[0-9]" from becoming "0-9" due to
       dnl M4 quoting.
       AC_EGREP_CPP([ACEMAPFAILED.+[0-9]L],
         [
#include <sys/mman.h>
          ACEMAPFAILED MAP_FAILED
         ],
         [
          ace_cv_feature_long_map_failed=yes
         ],
         [
          ace_cv_feature_long_map_failed=no
         ])
     ], [AC_DEFINE([ACE_HAS_LONG_MAP_FAILED])],
     [
      dnl Check if MAP_FAILED is _not_ cast to void *
      ACE_CACHE_CHECK([if MAP_FAILED is not cast to void *],
        [ace_cv_have_broken_map_failed],[
          AC_COMPILE_IFELSE([AC_LANG_PROGRAM([[
#include <sys/mman.h>
            ]], [[
             void * foo = MAP_FAILED;
            ]])],[
             ace_cv_have_broken_map_failed=no
            ],[
             AC_COMPILE_IFELSE([AC_LANG_PROGRAM(
               [[
#include <sys/mman.h>
               ]],
               [[
                void * foo = (void *) MAP_FAILED;
               ]])],
               [
                ace_cv_have_broken_map_failed=yes
               ],
               [
                dnl If we get here then we have no idea what is wrong!
                ace_cv_have_broken_map_failed=no
               ])
            ])
        ], [AC_DEFINE([ACE_HAS_BROKEN_MAP_FAILED])],)
     ])
  ],)

dnl Check if platform supports TCP_NODELAY support
ACE_CACHE_CHECK([for TCP_NODELAY support],
  [ace_cv_feature_tcp_nodelay],[
    AC_EGREP_CPP([ACE_TCPNODELAY],
      [
#ifndef ACE_LACKS_SYS_TYPES_H
# include <sys/types.h>
#endif
#include <netinet/tcp.h>

#if defined (TCP_NODELAY)
       ACE_TCPNODELAY
#endif
      ],
      [
       ace_cv_feature_tcp_nodelay=yes
      ],
      [
       ace_cv_feature_tcp_nodelay=no
      ])
  ], , [AC_DEFINE([ACE_LACKS_TCP_NODELAY])])

dnl Check if platform supports SO_SNDBUF/SO_RCVBUF socket options
ACE_CACHE_CHECK([for SO_SNDBUF/SO_RCVBUF socket options],
  [ace_cv_feature_so_sndbuf_rcvbuf],[
    AC_EGREP_CPP([ACE_SO_BUF],
      [
#ifndef ACE_LACKS_SYS_TYPES_H
# include <sys/types.h>
#endif
#include <sys/socket.h>

#if defined (SO_SNDBUF) && \
    defined (SO_RCVBUF)
       ACE_SO_BUF
#endif
      ],
      [
       ace_cv_feature_so_sndbuf_rcvbuf=yes
      ],
      [
       ace_cv_feature_so_sndbuf_rcvbuf=no
      ])
  ], , [AC_DEFINE([ACE_LACKS_SOCKET_BUFSIZ])])

dnl TODO: We only check for ACE_HAS_AUTOMATIC_INIT_FINI on platforms that
dnl have SVR4 dynamic linking since ACE doesn't support it otherwise.
if test "$ac_cv_header_dlfcn_h" = yes &&
   test "$ace_has_svr4_dynamic_linking" = yes; then
dnl Check if platform calls init/fini automatically
 ACE_CACHE_CHECK([for automatic init/fini calls],
  [ace_cv_feature_auto_init_fini],[
    ace_cv_feature_auto_init_fini=yes
    # TODO: We know how to check for this, but we need to:
    #
    #   1. Compile one file.
    #   2. Compile and link another file.
    #   3. Run file in point (2); it returns what we need.
    #
    # How do we do all that?
   ], [AC_DEFINE([ACE_HAS_AUTOMATIC_INIT_FINI])],)
fi dnl test "$ac_cv_header_dlfcn_h" = yes &&
   dnl      "$ace_has_svr4_dynamic_linking" = yes

dnl Check for recursive thread exit semantics
if test "$ace_user_enable_threads" = yes; then
  ACE_CACHE_CHECK([for recursive thread exit semantics],
    [ace_cv_feature_recursive_thr_exit],[
      ace_cv_feature_recursive_thr_exit=yes
      # TODO: How do we check for recursive thread exit semantics
    ], [AC_DEFINE([ACE_HAS_RECURSIVE_THR_EXIT_SEMANTICS])],)
fi

dnl Check for UNIX domain sockets
ACE_CACHE_CHECK([for UNIX domain sockets],
  [ace_cv_feature_unix_sockets],
   [
    AC_COMPILE_IFELSE([AC_LANG_PROGRAM([[
#include <sys/un.h>
      ]], [[
       sockaddr_un su;
      ]])],[
       ace_cv_feature_unix_sockets=yes
      ],[
       ace_cv_feature_unix_sockets=no
      ])
   ], , [AC_DEFINE([ACE_LACKS_UNIX_DOMAIN_SOCKETS])])

dnl Check for ACE_Handle_Set optimized for select()
ACE_CACHE_CHECK([for ACE_Handle_Set optimized for select()],
  [ace_cv_feature_handle_set_optimized_for_select],[
    ace_cv_feature_handle_set_optimized_for_select=yes
    # TODO: We know how to check this. We need to:
    #
    #   1. Compile and link a file.
    #   2. Run nm on that file.
    #
    # How do we do that?
  ], [AC_DEFINE([ACE_HAS_HANDLE_SET_OPTIMIZED_FOR_SELECT])],)

dnl Even we if we don't have IP multicasting, we still need to define
dnl "ACE_HAS_IP_MULTICAST" since struct ip_mreq gets redefined by ACE.
dnl What do we do about this problem?
dnl             -Ossama
dnl Check for IP multicast support
ACE_CACHE_CHECK([for IP multicast support],
  [ace_cv_feature_ip_multicast],[
    AC_COMPILE_IFELSE([AC_LANG_PROGRAM([[
#ifndef ACE_LACKS_SYS_TYPES_H
# include <sys/types.h>
#endif
#include <netinet/in.h>
      ]], [[
       ip_mreq im;
      ]])],[
       ace_cv_feature_ip_multicast=yes
      ],[
dnl Some platforms define ip_mreq in <sys/netinet/in.h>.
       AC_COMPILE_IFELSE([AC_LANG_PROGRAM(
         [[
#ifndef ACE_LACKS_SYS_TYPES_H
# include <sys/types.h>
#endif
#include <sys/netinet/in.h>
         ]],
         [[
          ip_mreq im;
         ]])],
         [
          ace_cv_feature_ip_multicast=yes
         ],
         [
          ace_cv_feature_ip_multicast=no
         ])
      ])
  ], [AC_DEFINE([ACE_HAS_IP_MULTICAST])],)

ACE_CACHE_CHECK([if running on an Alpha],
  [ace_cv_feature_alpha],[
    case "$host" in
      alpha*)
        ace_cv_feature_alpha=yes
        ;;
      *)
        ace_cv_feature_alpha=no
        ;;
    esac
  ],
  [
   case "$host" in
     *linux*)
       dnl We only define ACE_HAS_ALPHA_TIMER if we are running Linux
       dnl on an Alpha and are using GNU C++!
       if test "$GXX" = yes; then
         AC_DEFINE([ACE_HAS_ALPHA_TIMER])
       fi
       ;;
   esac
  ],)

ACE_CACHE_CHECK([if running on a Power PC],
  [ace_cv_feature_powerpc],[
    case "$host" in
      powerpc*)
        ace_cv_feature_powerpc=yes
        ;;
      *)
        ace_cv_feature_powerpc=no
        ;;
    esac
  ],
  [
   case "$host" in
     *aix*)
       dnl We don't do anything for AIX since AIX already has a
       dnl hi-res timer function!
       ;;
     *)
       dnl Only define ACE_HAS_POWERPC_TIMER when using GNU C++!
       if test "$GXX" = yes; then
         AC_DEFINE([ACE_HAS_POWERPC_TIMER])
       fi
       ;;
   esac
  ],)

ACE_CACHE_CHECK([if running on a Pentium(tm) processor],
  [ace_cv_feature_pentium],[
    case "$host" in
      i386-* | i486-* |i586-* | i686-*)
dnl If we do have a pentium, than define ACE_HAS_PENTIUM and add
dnl gethrtime.cpp to the source list, but only if we're using GNU C++
dnl since gethrtime.cpp uses assembler code specific to that compiler.
	if test "$GXX" = yes; then
          ace_cv_feature_pentium=yes
        else
          ace_cv_feature_pentium=no
	fi
        ;;
      *)
        ace_cv_feature_pentium=no
        ;;
    esac
  ],
  [
   AC_DEFINE([ACE_HAS_PENTIUM])
  ],)
AM_CONDITIONAL([ACE_ON_PENTIUM], [test X$ace_cv_feature_pentium = Xyes])


dnl
dnl SECTION 14: checks for aggregated features
dnl TODO: Little by little, get rid of these...
dnl


dnl Macro ACE_HAS_REENTRANT_FUNCTIONS means the following functions
dnl are usable:
dnl
dnl   rand_r
dnl   strtok_r
dnl   getpwnam_r (if we don't have, define ACE_LACKS_PWD_REENTRANT_FUNCTIONS)
dnl   ctime_r
dnl   localtime_r
dnl   gmtime_r
dnl   asctime_r
dnl * getprotobyname_r
dnl * getprotobynumber_r
dnl * gethostbyaddr_r
dnl * gethostbyname_r
dnl * getservbyname_r
dnl
dnl Those marked with '*' are NOT usable if
dnl ACE_LACKS_NETDB_REENTRANT_FUNCTIONS) is defined.
dnl
dnl The time has come to create feature macros for each of these...
dnl With the separate feature macros, we will define (for now)
dnl ACE_HAS_REENTRANT_FUNCTIONS only when all of those WITHOUHT a '*'
dnl are defined. Also, if any of those with '*' are missing, we will
dnl define ACE_LACKS_NETDB_REENTRANT_FUNCTIONS.

dnl Don't bother with reentrant functions if they are disabled by the user.
if test "$ace_user_enable_reentrant_funcs" = yes &&
   test "$ac_cv_func_rand_r" = yes &&
   test "$ac_cv_func_strtok_r" = yes &&
   test "$ac_cv_func_ctime_r" = yes &&
   test "$ac_cv_func_localtime_r" = yes &&
   test "$ac_cv_func_gmtime_r" = yes &&
   test "$ac_cv_func_asctime_r" = yes; then
  AC_DEFINE([ACE_HAS_REENTRANT_FUNCTIONS])
  # Explicitly enable reentrant functions if thread support is not enabled.
  if test "$ace_user_enable_threads" = no; then
    ACE_CPPFLAGS="$ACE_CPPFLAGS -D_REENTRANT"
  fi
fi

dnl Don't bother with reentrant functions if they are disabled by the user.
if test "$ace_user_enable_reentrant_funcs" = no ||
   test "$ac_cv_func_getprotobyname_r" = no ||
   test "$ac_cv_func_getprotobynumber_r" = no ||
   test "$ac_cv_func_gethostbyaddr_r" = no ||
   test "$ac_cv_func_gethostbyname_r" = no ||
   test "$ac_cv_func_getservbyname_r" = no; then
  AC_DEFINE([ACE_LACKS_NETDB_REENTRANT_FUNCTIONS])
fi

dnl FIXME!!!
dnl The following is a kludge until the netdb reentrant function
dnl number of arguments is handled.
case "$host" in
  *linux*) AC_DEFINE([ACE_LACKS_NETDB_REENTRANT_FUNCTIONS]);;
  *) ;;
esac


dnl Macro ACE_LACKS_SOME_POSIX_PROTOTYPES implies any of the following
dnl features:
dnl
dnl    ! ACE_HAS_MSGSND_MSGBUFP_2
dnl    ! ACE_LACKS_MSGRCV_VOIDP_2
dnl    ! ACE_LACKS_SHMAT_VOIDP_2
dnl    ! ACE_LACKS_SIGACTION_CONSTP_2
dnl
dnl So, for now, we define it if any of those were autoconf'ed.

if test "$ace_cv_lib_posix_msgsnd_msgbufp_2" = yes ||
   test "$ace_cv_lib_posix_msgrcv_voidp_2" != yes ||
   test "$ace_cv_lib_posix_shmat_voidp_2" != yes ||
   test "$ace_cv_lib_posix_sigaction_constp_2" != yes; then
  AC_DEFINE([ACE_LACKS_SOME_POSIX_PROTOTYPES])
fi

if test "$ac_cv_func_msgctl" = yes &&
   test "$ac_cv_func_msgget" = yes &&
   test "$ac_cv_func_msgrcv" = yes &&
   test "$ac_cv_func_semctl" = yes &&
   test "$ac_cv_func_semget" = yes &&
   test "$ac_cv_func_semop" = yes &&
   test "$ac_cv_func_shmat" = yes &&
   test "$ac_cv_func_shmctl" = yes &&
   test "$ac_cv_func_shmdt" = yes &&
   test "$ac_cv_func_shmget" = yes; then
  AC_DEFINE([ACE_HAS_SYSV_IPC])
fi

if test "$ac_cv_func_shmat" != yes ||
   test "$ac_cv_func_shmctl" != yes ||
   test "$ac_cv_func_shmdt" != yes ||
   test "$ac_cv_func_shmget" != yes; then
  AC_DEFINE([ACE_LACKS_SYSV_SHMEM])
fi

dnl Check for what POSIX threads draft we have
AC_MSG_CHECKING([which POSIX thread library was found])

ace_has_pthreads=no

dnl Check if we have Pthreads Draft 4
dnl if test "$ac_cv_func_pthread_delay_np" = yes &&
if test "$ac_cv_func_pthread_mutexattr_create" = yes &&
dnl   test "$ac_cv_func_pthread_mutexattr_setkind_np" = yes &&
   test "$ac_cv_func_pthread_mutexattr_delete" = yes &&
   test "$ac_cv_func_pthread_condattr_delete" = yes &&
   test "$ac_cv_func_pthread_condattr_create" = yes &&
   test "$ac_cv_func_pthread_setprio" = yes &&
   test "$ac_cv_func_pthread_getprio" = yes &&
dnl   test "$ac_cv_func_pthread_getspecific" = yes &&
   test "$ac_cv_func_pthread_setcancel" = yes &&
   test "$ac_cv_func_pthread_setasynccancel" = yes &&
   test "$ac_cv_func_pthread_kill" = yes; then
  ace_has_pthreads=yes
  AC_MSG_RESULT([POSIX Threads Draft 4])
  AC_DEFINE([ACE_HAS_PTHREADS_DRAFT4])
dnl Check if we have Pthreads Draft 6
elif test "$ac_cv_func_pthread_mutexattr_init" = yes &&
   test "$ac_cv_func_pthread_mutexattr_destroy" = yes &&
   test "$ac_cv_func_pthread_condattr_destroy" = yes &&
   test "$ac_cv_func_pthread_condattr_init" = yes &&
   test "$ac_cv_func_pthread_attr_setprio" = yes &&
   test "$ac_cv_func_pthread_attr_getprio" = yes &&
   test "$ac_cv_func_pthread_setintr" = yes &&
   test "$ac_cv_func_pthread_setintrtype" = yes; then
  ace_has_pthreads=yes
  AC_MSG_RESULT([POSIX Threads Draft 6])
  AC_DEFINE([ACE_HAS_PTHREADS_DRAFT6])
dnl Check if we have Pthreads Draft 7
elif test "$ac_cv_func_pthread_mutexattr_init" = yes &&
   test "$ac_cv_func_pthread_mutexattr_destroy" = yes &&
   test "$ac_cv_func_pthread_condattr_destroy" = yes &&
   test "$ac_cv_func_pthread_condattr_init" = yes &&
   test "$ac_cv_func_pthread_setschedparam" = yes &&
   test "$ac_cv_func_pthread_getschedparam" = yes &&
   test "$ac_cv_func_pthread_setcancelstate" = yes &&
   test "$ac_cv_func_pthread_setcanceltype" = yes &&
   test "$ace_has_sched_yield" != yes; then
  ace_has_pthreads=yes
  AC_MSG_RESULT([POSIX Threads Draft 7])
  AC_DEFINE([ACE_HAS_PTHREADS_DRAFT7])
dnl Check if we have Pthreads Draft Standard
elif test "$ac_cv_func_pthread_mutexattr_init" = yes &&
   test "$ac_cv_func_pthread_mutexattr_destroy" = yes &&
   test "$ac_cv_func_pthread_condattr_destroy" = yes &&
   test "$ac_cv_func_pthread_condattr_init" = yes &&
   test "$ac_cv_func_pthread_setschedparam" = yes &&
   test "$ac_cv_func_pthread_getschedparam" = yes &&
   test "$ac_cv_func_pthread_setcancelstate" = yes &&
   test "$ac_cv_func_pthread_setcanceltype" = yes &&
   test "$ace_has_sched_yield" = yes; then
  ace_has_pthreads=yes
  AC_MSG_RESULT([POSIX Threads Draft Standard])
  AC_DEFINE([ACE_HAS_PTHREADS_STD])
else
  ace_has_pthreads=no
  AC_MSG_RESULT([none])
fi dnl PTHREAD DRAFT CHECKS

dnl Check if we have UNIX International threads
AC_MSG_CHECKING([if a UNIX International thread library was found])
if test "$ace_has_sthreads" = yes; then
  AC_MSG_RESULT([yes])
else
  AC_MSG_RESULT([no])
fi

if test "$ace_user_enable_threads" != yes ||
   test "$ace_has_pthreads" != yes &&
   test "$ace_has_sthreads" != yes; then
  dnl We don't have a usable thread library!
  ace_user_enable_threads=no
  dnl Make sure _REENTRANT and _THREAD_SAFE are not in the
  dnl preprocessor flags since thread support is being disabled.
  dnl Removal of these flags is only performed if the configure script
  dnl added them.
  if test -n "$ACE_THR_CPPFLAGS"; then
changequote(, )dnl
  CPPFLAGS=`eval "echo $CPPFLAGS | sed -e 's/$ACE_THR_CPPFLAGS//' -e 's/-D_THREAD_SAFE\(=[0-9]*\)\?//'"`
changequote([, ])dnl
  fi

  AC_MSG_WARN([It appears that you do NOT have any usable thread libraries])
  AC_MSG_WARN([or thread support was explicitly disabled.])
  AC_MSG_WARN([Disabling thread support.])
  dnl ACE uses different versions of readdir_r depending on the thread
  dnl library being used, i.e. on the ACE_HAS_*THREADS* macros.  Since
  dnl it doesn't seem like a good idea to define any ACE_HAS_*THREADS*
  dnl macro if ACE won't be supporting threads, define ACE_LACKS_READDIR_R
  dnl regardless if readdir_r() exists.
  if test "$ac_cv_func_readdir_r" = yes; then
    AC_MSG_WARN([Disabling support for readdir_r() since thread support])
    AC_MSG_WARN([is being disabled.])
    AC_DEFINE([ACE_LACKS_READDIR_R])
  fi dnl test "$ac_cv_func_readdir_r" = yes
fi dnl

if test "$ace_user_enable_threads" = yes; then
dnl If we get this far then we have threads.
dnl FIXME: The "_POSIX" macros may need to be defined _before_ the checks for
dnl        reentrant functions!  However, we don't want to define them if
dnl        the UNIX International threads library was detected.
  AC_DEFINE([ACE_HAS_THREADS])
  AC_DEFINE([ACE_MT_SAFE])
  ACE_CPPFLAGS="$ACE_CPPFLAGS $ACE_THR_CPPFLAGS"
  if test "$ace_has_pthreads" = yes &&
     test "$ace_has_sthreads" != yes; then
    ACE_CPPFLAGS="$ACE_CPPFLAGS -D_POSIX_PTHREAD_SEMANTICS"
  fi dnl test "$ace_has_pthreads" = yes

  if test "$ace_has_pthreads" = yes; then

    dnl Check if OS requires non-null status pointer for ::pthread_join ()
    dnl
    dnl This test must be performed after the POSIX threads implementation
    dnl that the platform supports has been determined.
    ACE_CACHE_CHECK([for pthread_join null status pointer support],
      [ace_cv_have_null_status_pthread_join],[
        AC_EGREP_CPP([WE_HAVE_PTHREADS_D4],
          [
#if defined (ACE_HAS_PTHREADS) && defined (ACE_HAS_PTHREADS_DRAFT4)
/* This test is only valid for Pthreads Draft 4 */
WE_HAVE_PTHREADS_D4
#endif
          ],
          [
           AC_RUN_IFELSE([AC_LANG_SOURCE([[
#ifndef _REENTRANT
#define _REENTRANT
#endif

#include <pthread.h>

/* _THREAD_SAFE is defined in <pthread.h> on some platforms. */
#ifndef _THREAD_SAFE
#define _THREAD_SAFE
#endif

#include <stdio.h>

#ifdef __cplusplus
extern "C"
#endif
void *
nothing (void *unused)
{
  return (void *) 34;
};

int
main ()
{
  pthread_attr_t attr;
  pthread_t id;
  void *status;
  int retval = 0;

  /* ----- */
  /* We return 0 on error for these calls since we only want to
     return an error status if pthread_join fails.  If these calls
     fail then we've got other problems! */
  if (pthread_attr_create (&attr) != 0) return 0 /*1*/;

  if (pthread_create (&id, attr, nothing, 0) != 0) return 0 /*2*/;

  if (pthread_attr_delete (&attr) != 0) return /*3*/;
  /* ----- */

  /* With a second (status) arg of 0, LynxOS 3.0.0 pthread_join ()
     will fail with errno 14 (address fault detected). */
  if (pthread_join (id, 0) == -1) {
    fprintf (stderr, "%s: %d; ", __FILE__, __LINE__);
    perror ("pthread_join");
    retval = 1;
  }

  if (pthread_join (id, &status) == -1) {
    fprintf (stderr, "%s: %d; ", __FILE__, __LINE__);
    perror ("pthread_join");
    retval = 2;
  }

  return retval;
}
             ]])],[
              ace_cv_have_null_status_pthread_join=yes
             ],[
              ace_cv_have_null_status_pthread_join=no
             ],[
              dnl If we are cross-compiling let's hope that
              dnl that we have a working null status pointer
              dnl for pthread_join.
              ace_cv_have_null_status_pthread_join=yes
             ])
          ],
          [
           ace_cv_have_null_status_pthread_join=yes
          ])
      ], , [AC_DEFINE([ACE_LACKS_NULL_PTHREAD_STATUS])])

    dnl Check if OS supports mutex timeouts
    dnl (e.g. pthread_mutex_timedlock()).
    ACE_CACHE_CHECK([for mutex timeouts],
      [ace_cv_have_mutex_timeouts],[
         AC_RUN_IFELSE([AC_LANG_SOURCE([[
#ifndef _REENTRANT
#define _REENTRANT
#endif

#include <pthread.h>

/* _THREAD_SAFE is defined in <pthread.h> on some platforms. */
#ifndef _THREAD_SAFE
#define _THREAD_SAFE
#endif

#include <stdio.h>

#include <errno.h>
#include <sys/time.h>

#ifndef ACE_LACKS_UNISTD_H
# include <unistd.h>
#endif


pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

#ifdef __cplusplus
extern "C"
#endif
void *threadFunc (void *parm)
{
  int rc;
  int i;
  struct timespec deltatime;
  struct timeval tv;

  if (gettimeofday (&tv, 0) != 0)
    {
      return 0;
    }

  deltatime.tv_sec = tv.tv_sec + 5;
  deltatime.tv_nsec = 0;

  rc = pthread_mutex_timedlock (&mutex, &deltatime);

  if (rc != ETIMEDOUT)
    {
      /* printf("Got an incorrect return code from pthread_mutex_timedlock\n"); */
    }

  return 0;
}

int main (void)
{
  int rc  =0;
  pthread_t thread;

  rc = pthread_mutex_lock (&mutex);
  if (rc != 0)
    {
      exit (-1);
    }

  rc = pthread_create (&thread, NULL, threadFunc, NULL);
  if (rc != 0)
    {
      exit (-1);
    }

  rc = pthread_join (thread, NULL);
  if (rc != 0)
    {
      exit (-1);
    }

  pthread_mutex_destroy (&mutex);

  return 0;
}
           ]])],[
            ace_cv_have_mutex_timeouts=yes
           ],[
            ace_cv_have_mutex_timeouts=no
           ],[
              dnl Cross-compiled case
              AC_CHECK_FUNC([pthread_mutex_timedlock],
                [ace_cv_have_mutex_timeouts=yes],
                [ace_cv_have_mutex_timeouts=no])
           ])
      ], [AC_DEFINE([ACE_HAS_MUTEX_TIMEOUTS])],)

    dnl Check if platform needs to #include <sched.h> to get thread
    dnl scheduling defs.
    ACE_CACHE_CHECK([if sched.h is needed for thread scheduling definitions],
      [ace_cv_needs_sched_h],
      [
       AC_COMPILE_IFELSE([AC_LANG_PROGRAM([[
#ifdef ACE_HAS_STHREADS
#include <thread.h>
#endif

#ifdef ACE_HAS_PTHREADS
#include <pthread.h>
#endif
         ]], [[
int foo = SCHED_OTHER;
         ]])],[
          ace_cv_needs_sched_h=no
         ],[
          AC_COMPILE_IFELSE([AC_LANG_PROGRAM(
            [[
#ifdef ACE_HAS_STHREADS
#include <thread.h>
#endif

#ifdef ACE_HAS_PTHREADS
#include <pthread.h>
#endif

#include <sched.h>
            ]],
            [[
int foo = SCHED_OTHER;
            ]])],
            [
             ace_cv_needs_sched_h=yes
            ],
            [
             dnl We're hosed if we get here!
             ace_cv_needs_sched_h=no
            ])
         ])
      ],
      [
       AC_DEFINE([ACE_NEEDS_SCHED_H])
      ],)

    dnl Check if platform only supports SCHED_OTHER scheduling policy
    dnl
    dnl This test must be performed after the POSIX threads implementation
    dnl that the platform supports has been determined.
    ACE_CACHE_CHECK([if SCHED_OTHER is only scheduling policy],
      [ace_cv_feature_only_have_sched_other],
      [
        AC_EGREP_CPP([WE_ONLY_HAVE_SCHED_OTHER],
          [
#ifdef ACE_HAS_STHREADS
# include <thread.h>
#endif

#ifdef ACE_HAS_PTHREADS
# include <pthread.h>
#endif

#if defined (ACE_NEEDS_SCHED_H)
# include <sched.h>
#endif

       /* These are ORed so that ACE will not redefine
          any of them if any of them exist. */
#if !defined (SCHED_FIFO) && \
    !defined (SCHED_RR) && \
    defined (SCHED_OTHER)
          WE_ONLY_HAVE_SCHED_OTHER
#endif
          ],
          [
           ace_cv_feature_only_have_sched_other=yes
          ],
          [
           AC_RUN_IFELSE([AC_LANG_SOURCE([[
#ifndef _REENTRANT
#define _REENTRANT
#endif

#include <pthread.h>

/* _THREAD_SAFE is defined in <pthread.h> on some platforms. */
#ifndef _THREAD_SAFE
#define _THREAD_SAFE
#endif

#include <stdio.h>
#include <errno.h>

int main ()
{
  pthread_attr_t ace_attr;

#if defined (ACE_HAS_PTHREADS_DRAFT4)
  if (pthread_attr_create (&ace_attr) != 0)
#else
  if (pthread_attr_init (&ace_attr) != 0)
#endif
    {
     perror ("pthread_attr_init");
     return 0; /* Return "successfully" since only the policy call
                  will return with an error for this test. */
    }

#if defined (ACE_HAS_PTHREADS_DRAFT4)
  if (pthread_attr_setsched (&ace_attr, SCHED_FIFO) != 0)
#else
  if (pthread_attr_setschedpolicy (&ace_attr, SCHED_FIFO) != 0)
#endif
    {
     perror ("pthread_attr_setschedpolicy");
     return -1;
    }

#if defined (ACE_HAS_PTHREADS_DRAFT4)
  if (pthread_attr_delete (&ace_attr) != 0)
#else
  if (pthread_attr_destroy (&ace_attr) != 0)
#endif
    {
     perror ("pthread_attr_destroy");
     return 0; /* Return "successfully" since only the policy call
                  will return with an error for this test. */
    }

  return 0;
}
             ]])],[
              ace_cv_feature_only_have_sched_other=no
             ],[
              ace_cv_feature_only_have_sched_other=yes
             ],[
              dnl We only get here if polices other than SCHED_OTHER
              dnl were found in the headers and we are cross-compiling.
              dnl
              dnl If we are cross-compiling let's hope that the
              dnl scheduling policies found in the headers
              dnl besides SCHED_OTHER (e.g. SCHED_FIFO, SCHED_RR)
              dnl are supported.
              ace_cv_feature_only_have_sched_other=no
             ])
          ])
      ], [AC_DEFINE([ACE_HAS_ONLY_SCHED_OTHER])],)
  fi dnl test "$ace_has_pthreads" = yes
fi dnl test "$ace_user_enable_threads" = yes


if test "$ac_cv_header_libc_h" != yes ||
   test "$ac_cv_header_osfcn_h" != yes; then
  AC_DEFINE([ACE_HAS_CPLUSPLUS_HEADERS])
fi



if test "$ace_cv_lib_signal_vi1_2" = yes &&
   test "$ace_cv_lib_signal_vi1_ret" = yes &&
   test "$ace_cv_lib_struct_sigaction_vi1_handler" = yes; then
  AC_DEFINE([ACE_HAS_CONSISTENT_SIGNAL_PROTOTYPES])
elif test "$ace_cv_lib_signal_vi1_2" != yes &&
   test "$ace_cv_lib_signal_vv1_2" != yes &&
   test "$ace_cv_lib_signal_vi1a2_2" != yes &&
   test "$ace_cv_lib_signal_va1_2" = yes &&
   test "$ace_cv_lib_signal_vi1_ret" != yes &&
   test "$ace_cv_lib_signal_vv1_ret" != yes &&
   test "$ace_cv_lib_signal_vi1a2_ret" != yes &&
   test "$ace_cv_lib_signal_va1_ret" = yes &&
   test "$ace_cv_lib_struct_sigaction_vi1_handler" != yes &&
   test "$ace_cv_lib_struct_sigaction_vv1_handler" != yes &&
   test "$ace_cv_lib_struct_sigaction_vi1a2_handler" != yes &&
   test "$ace_cv_lib_struct_sigaction_va1_handler" = yes; then
  AC_DEFINE([ACE_HAS_LYNXOS_SIGNALS])
  AC_DEFINE([ACE_HAS_TANDEM_SIGNALS])
  AC_DEFINE([ACE_HAS_IRIX_53_SIGNALS])
  AC_DEFINE([ACE_HAS_SUNOS4_SIGNAL_T])
elif test "$ace_cv_lib_signal_vi1a2_2" = yes &&
   test "$ace_cv_lib_signal_vi1a2_ret" = yes &&
   test "$ace_cv_lib_struct_sigaction_vi1a2_handler" = yes; then
  AC_DEFINE([ACE_HAS_SPARCWORKS_401_SIGNALS])
elif test "$ace_cv_lib_signal_vi1_2" = yes &&
   test "$ace_cv_lib_signal_vi1_ret" = yes &&
   test "$ace_cv_lib_struct_sigaction_vi1_handler" != yes; then
  AC_DEFINE([ACE_HAS_SVR4_SIGNAL_T])
elif test "$ace_cv_lib_signal_vi1_2" = yes &&
   test "$ace_cv_lib_signal_vv1_ret" = yes &&
   test "$ace_cv_lib_struct_sigaction_vv1_handler" = yes; then
  AC_DEFINE([ACE_HAS_SVR4_SIGNAL_T])
elif test "$ace_cv_lib_signal_vi1_2" = yes &&
   test "$ace_cv_lib_signal_vi1_ret" != yes &&
   test "$ace_cv_lib_signal_vv1_ret" != yes &&
   test "$ace_cv_lib_signal_vi1a2_ret" != yes &&
   test "$ace_cv_lib_signal_va1_ret" = yes &&
   test "$ace_cv_lib_struct_sigaction_vi1_handler" != yes &&
   test "$ace_cv_lib_struct_sigaction_vv1_handler" != yes &&
   test "$ace_cv_lib_struct_sigaction_vi1a2_handler" != yes &&
   test "$ace_cv_lib_struct_sigaction_va1_handler" = yes; then
  AC_DEFINE([ACE_HAS_UNIXWARE_SVR4_SIGNAL_T])
fi dnl ACE_HAS_CONSISTENT_SIGNAL_PROTOTYPES

dnl
dnl SECTION 15: Final checks
dnl

dnl Make final substitutions and defines
if test "$ace_u_long_long_typedef_set" = yes; then
  AC_DEFINE_UNQUOTED([ACE_UINT64_TYPEDEF], [$ACE_UINT64])
fi

dnl Combine package set flags with user's flags.
dnl User's flags go after package flags to allow user to override
dnl package defaults.
dnl X_CFLAGS comes from AC_PATH_XTRA.  It may include, for example,
dnl additional include file paths or macros that need to be defined
dnl in order for X11 related files to be compiled properly.
if test "$ace_user_enable_optimize"; then
  dnl We want OCXXFLAGS to be on the end, so we use CXXFLAGS,
  dnl not ACE_CXXFLAGS!
  CXXFLAGS="$OCXXFLAGS $CXXFLAGS"
  CFLAGS="$OCFLAGS $CFLAGS"
fi
CXXFLAGS="$ACE_CXXFLAGS $X_CFLAGS $CXXFLAGS"
CFLAGS="$ACE_CFLAGS $X_CFLAGS $CFLAGS"
CPPFLAGS="$ACE_CPPFLAGS $CPPFLAGS"
LDFLAGS="$ACE_LDFLAGS $LDFLAGS"

dnl The following tests should be performed _after_ the bulk of the
dnl ACE macros have been defined.

dnl Flush the cache so that it is easier to debug the configure script
dnl if the following integrity check fails.
AC_CACHE_SAVE

dnl Verify the integrity of the current configuration.
ACE_CACHE_CHECK([if generated ACE configuration is usable],
  [ace_cv_configuration_is_usable],
  [
   dnl We want an empty ace/config.h to prevent multiple defines
   dnl with Autoconf's confdefs.h
   ACE_USE_TEMP_FILE([ace/config.h],
     [
      dnl Now run the compilation test
      ACE_TRY_COMPILE([-I. -I${srcdir}],
        [
#if defined(ACE_UINT64_TYPEDEF)
  typedef ACE_UINT64_TYPEDEF ACE_UINT64;
#endif /* ACE_UINT64_TYPEDEF */

#include "ace/OS.cpp"
        ],
        [
         ACE_Time_Value t = ACE_OS::gettimeofday ();
         t++;
        ],
        [
         ace_cv_configuration_is_usable=yes
        ],
        [
         ace_cv_configuration_is_usable=no
        ])
     ])
  ],
  [
   dnl Looks good! Do nothing.
   dnl It appears that ace/OS.cpp compiled.  If it didn't compile then
   dnl there would be no chance that the rest of ACE would compile.
  ],
  [
   AC_MSG_ERROR(
     [
The generated configuration appears to be unusable.  Please verify
that your system path and environment variables are correct.  If they
appear to be correct then please send the maintainer of this configure
script $ACE_CONFIGURE_MAINTAINER the *COMPRESSED* 'config.log' file
and the following information:

  ACE 'configure' Script Information
  ==================================
  [RCS] translit([$Id$], [$\"])


  ACE Version:        $ACE_VERSION
  C++ Compiler:       $CXX
  C++ Preprocessor:   $CXXCPP
  C++ Flags:          $CXXFLAGS
  Preprocessor Flags: $CPPFLAGS
  Linker:             $LD
  Linker Flags:       $LDFLAGS
  Libraries:          $LIBS
  System type information:
    Build: $build  Host: $host

In the meantime, please use the stock ACE build procedure detailed in
the file 'ACE-INSTALL.html'.
     ])
  ])

dnl Check for ACE_IOStream support
ACE_CACHE_CHECK([for ACE_IOStream support],
  [ace_cv_feature_ace_iostream],
  [
   dnl We want an empty ace/config.h to prevent multiple defines
   dnl with Autoconf's confdefs.h
   ACE_USE_TEMP_FILE([ace/config.h],
     [
      dnl Now run the compilation test
      ACE_TRY_COMPILE([-I. -I${srcdir}],
        [
#if defined(ACE_UINT64_TYPEDEF)
  typedef ACE_UINT64_TYPEDEF ACE_UINT64;
#endif /* ACE_UINT64_TYPEDEF */

#include "ace/IOStream.cpp"
        ],
        [
         int a=0; a += 1;
        ],
        [
         ace_cv_feature_ace_iostream=yes
        ],
        [
         ace_cv_feature_ace_iostream=no
        ])
     ])
  ], , [AC_DEFINE([ACE_LACKS_ACE_IOSTREAM])])

dnl Check if ACE needs minimum iostream header inclusion
ACE_CACHE_CHECK([if ACE needs minimum iostream header inclusion],
  [ace_cv_lib_minimum_iostream],
  [
   dnl We want an empty ace/config.h to prevent multiple defines
   dnl with Autoconf's confdefs.h
   ACE_USE_TEMP_FILE([ace/config.h],
     [
      dnl Now run the compilation test
      ACE_TRY_COMPILE([-I. -I${srcdir}],
        [
#if defined(ACE_UINT64_TYPEDEF)
  typedef ACE_UINT64_TYPEDEF ACE_UINT64;
#endif /* ACE_UINT64_TYPEDEF */

#include "ace/OS.cpp"
        ],
        [
         int a=0; a += 1;
        ],
        [
         ace_cv_lib_minimum_iostream=no
        ],
        [
         dnl Now check if ACE_HAS_MINIMUM_IOSTREAMH_INCLUSION makes
         dnl compilation work!
         ACE_TRY_COMPILE([-I. -I${srcdir}],
           [
#if defined(ACE_UINT64_TYPEDEF)
  typedef ACE_UINT64_TYPEDEF ACE_UINT64;
#endif /* ACE_UINT64_TYPEDEF */

#define ACE_HAS_MINIMUM_IOSTREAMH_INCLUSION
#include "ace/OS.cpp"
           ],
           [
            int a=0; a += 1;
           ],
           [
            ace_cv_lib_minimum_iostream=yes
           ],
           [
            dnl If we get here, then we have no idea what is wrong!
            ace_cv_lib_minimum_iostream=no
           ])
        ])
     ])
  ], [AC_DEFINE([ACE_HAS_MINIMUM_IOSTREAMH_INCLUSION])],)

dnl @@ I don't think that this test is correct.  The only way I could
dnl    get ACE_Addr::sap_any to work was to explicitly cast it to the
dnl    a constant reference of the derived type.
dnl       -Ossama
dnl Check if compiler can't handle the static ACE_Addr::sap_any construct
ACE_CACHE_CHECK([for ACE_addr::sap_any support],
  [ace_cv_lib_sap_any_support],
  [
   dnl We want an empty ace/config.h to prevent multiple defines
   dnl with Autoconf's confdefs.h
   ACE_USE_TEMP_FILE([ace/config.h],
     [
      dnl Now run the compilation test
      ACE_TRY_COMPILE([-I. -I${srcdir}],
        [
#if defined(ACE_UINT64_TYPEDEF)
  typedef ACE_UINT64_TYPEDEF ACE_UINT64;
#endif /* ACE_UINT64_TYPEDEF */

#include "ace/FILE_Addr.h"
        ],
        [
  ACE_FILE_Addr ace_file ((const ACE_FILE_Addr &) ACE_Addr::sap_any);
        ],
        [
         ace_cv_lib_sap_any_support=yes
        ],
        [
         dnl Now check if ACE_HAS_BROKEN_SAP_ANY makes
         dnl compilation work!
         ACE_TRY_COMPILE([-I. -I${srcdir}],
           [
#if defined(ACE_UINT64_TYPEDEF)
  typedef ACE_UINT64_TYPEDEF ACE_UINT64;
#endif /* ACE_UINT64_TYPEDEF */

#define ACE_HAS_BROKEN_SAP_ANY
#include "ace/FILE_Addr.h"
           ],
           [
  ACE_FILE_Addr ace_file ((const ACE_FILE_Addr &) ACE_Addr::sap_any);
           ],
           [
            ace_cv_lib_sap_any_support=no
           ],
           [
            dnl If we get here, then we have no idea what is wrong!
            ace_cv_lib_sap_any_support=yes
          ])
        ])
     ])
  ],, [AC_DEFINE([ACE_HAS_BROKEN_SAP_ANY])])

dnl Check if ACE needs conversion to pass ACE_TTY_IO to DEV_Connector
ACE_CACHE_CHECK([if ACE needs conversion to pass ACE_TTY_IO to DEV_Connector],
  [ace_cv_lib_need_dev_io_conv],
  [
   dnl We want an empty ace/config.h to prevent multiple defines
   dnl with Autoconf's confdefs.h
   ACE_USE_TEMP_FILE([ace/config.h],
     [
      dnl Now run the compilation test
      ACE_TRY_COMPILE([-I. -I${srcdir}],
        [
#if defined(ACE_UINT64_TYPEDEF)
  typedef ACE_UINT64_TYPEDEF ACE_UINT64;
#endif /* ACE_UINT64_TYPEDEF */

#include "ace/OS.cpp"
        ],
        [
         int a=0; a += 1;
        ],
        [
         ace_cv_lib_need_dev_io_conv=no
        ],
        [
         dnl Now check if ACE_NEEDS_DEV_IO_CONVERSION makes
         dnl compilation work!
         ACE_TRY_COMPILE([-I. -I${srcdir}],
           [
#if defined(ACE_UINT64_TYPEDEF)
  typedef ACE_UINT64_TYPEDEF ACE_UINT64;
#endif /* ACE_UINT64_TYPEDEF */

#define ACE_NEEDS_DEV_IO_CONVERSION
#include "ace/DEV_Connector.cpp"
           ],
           [
            int a=0; a += 1;
           ],
           [
            ace_cv_lib_need_dev_io_conv=yes
           ],
           [
            dnl If we get here, then we have no idea what is wrong!
            ace_cv_lib_need_dev_io_conv=no
           ])
        ])
     ])
  ], [AC_DEFINE([ACE_NEEDS_DEV_IO_CONVERSION])],)

dnl End ACE macro tests!

dnl Substitute whatever X libraries ACE needs, if any.
AC_SUBST([ACE_XLIBS])

dnl Prepend purify and quantify command lines if purify and quantify are
dnl enabled.  Otherwise, PURELINK and PRELINK will just be "blank."
LD="$PURELINK $PRELINK $LD"
dnl LDFLAGS="$ACE_LDFLAGS $LDFLAGS"

dnl AC_SUBST(LDFLAGS)
dnl AC_SUBST(LIBOBJS)

dnl These definitions are expanded in the makefile and ace-config
if test "$no_x" != yes; then
  ACE_LIBDIR="${X_LIBS} -L\${libdir}"
else
  ACE_LIBDIR='-L${libdir}'
fi
ACE_LIBS="${X_PRE_LIBS} ${ACE_XLIBS} ${X_EXTRA_LIBS} ${LIBS}"
ACE_INCLUDEDIR='-I${includedir}'

AC_SUBST([ACE_LIBDIR])
AC_SUBST([ACE_LIBS])
AC_SUBST([ACE_INCLUDEDIR])

dnl These definitions are expanded in the makefile and ace-config
#TAO_LIBDIR='-L${libdir}'
#TAO_LIBS="-lACE ${LIBS}"
#TAO_INCLUDEDIR='-I${includedir}'
#
#AC_SUBST([TAO_LIBDIR])
#AC_SUBST([TAO_LIBS])
#AC_SUBST([TAO_INCLUDEDIR])

dnl Force CXXFLAGS to be substituted in Makefiles that don't "need" them.
AC_SUBST([CXXFLAGS])

dnl
dnl SECTION 16: AC_CONFIG_FILES([FILE...])
dnl
dnl
dnl We can finally create all the files listed here; Makefile is
dnl created from Makefile.in, etc.  Top-level Makefiles should be
dnl created first.

dnl   Makefile
dnl   ace/Makefile
dnl   apps/Makefile
dnl   apps/gperf/Makefile
dnl   apps/gperf/src/Makefile
dnl   man/Makefile
dnl   man/man3/Makefile
dnl   netsvcs/Makefile
dnl   netsvcs/clients/Makefile
dnl   netsvcs/clients/Logger/Makefile
dnl   netsvcs/clients/Naming/Makefile
dnl   netsvcs/clients/Naming/Client/Makefile
dnl   netsvcs/clients/Naming/Dump_Restore/Makefile
dnl   netsvcs/clients/Tokens/Makefile
dnl   netsvcs/clients/Tokens/collection/Makefile
dnl   netsvcs/clients/Tokens/deadlock/Makefile
dnl   netsvcs/clients/Tokens/invariant/Makefile
dnl   netsvcs/clients/Tokens/manual/Makefile
dnl   netsvcs/clients/Tokens/mutex/Makefile
dnl   netsvcs/clients/Tokens/rw_lock/Makefile
dnl   netsvcs/lib/Makefile
dnl   netsvcs/servers/Makefile
dnl   tests/Makefile

AC_CONFIG_FILES([
  ace-config
  ace-config.1
  aceConf.sh
  Makefile
  ace/Makefile
  ace/RMCast/Makefile
  ace/QoS/Makefile
  ace/SSL/Makefile
  apps/Makefile
  apps/gperf/Makefile
  apps/gperf/src/Makefile
  tests/Makefile
])

AC_CONFIG_COMMANDS([default],[
  echo ""
  echo "Configuration of ACE is now complete."
  echo ""
 ],[])
AC_OUTPUT



dnl Configure the TAO directory if it exists.
dnl if test -d $srcdir/TAO; then
dnl   AC_CONFIG_SUBDIRS([TAO])
dnl fi
