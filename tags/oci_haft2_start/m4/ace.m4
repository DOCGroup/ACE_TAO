dnl -------------------------------------------------------------------------
dnl       $Id$
dnl 
dnl       ace.m4
dnl
dnl       ACE M4 include file which contains ACE specific M4 macros
dnl       for enabling/disabling certain ACE features.
dnl 
dnl -------------------------------------------------------------------------

dnl  Copyright (C) 1998, 1999, 2000, 2002  Ossama Othman
dnl
dnl  All Rights Reserved
dnl
dnl This library is free software; you can redistribute it and/or
dnl modify it under the current ACE distribution terms.
dnl 
dnl This library is distributed in the hope that it will be useful,
dnl but WITHOUT ANY WARRANTY; without even the implied warranty of
dnl MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.


dnl Macros that add ACE configuration options to a `configure' script.
dnl ACE_CONFIGURATION_OPTIONS
AC_DEFUN([ACE_CONFIGURATION_OPTIONS],
[
 AC_ARG_ENABLE([alloca],
  AC_HELP_STRING([--enable-alloca],[compile with alloca() support [[no]]]),
  [
   case "${enableval}" in
    yes)
      ace_user_enable_alloca=yes
      ;;
    no)
      ace_user_enable_alloca=no
      ;;
    *)
      AC_MSG_ERROR([bad value ${enableval} for --enable-alloca])
      ;;
   esac
  ],
  [
   dnl Disable alloca() support by default since its use is generally
   dnl not recommended.
   ace_user_enable_alloca=no
  ])

 AC_ARG_ENABLE([log-msg-prop],
  AC_HELP_STRING([--enable-log-msg-prop],[enable threads inheriting ACE_Log_Msg properties from parent thread [[yes]]]),
  [
   case "${enableval}" in
    yes)
      dnl nothing to do
      ;;
    no)
      AC_DEFINE(ACE_THREADS_DONT_INHERIT_LOG_MSG)
      ;;
    *)
      AC_MSG_ERROR([bad value ${enableval} for --enable-log-msg-prop])
      ;;
   esac
  ],)

 AC_ARG_ENABLE([logging],
  AC_HELP_STRING([--enable-logging],[enable ACE logging macros [[yes]]]),
  [
   case "${enableval}" in
    yes)
      dnl nothing to do
      ;;
    no)
      AC_DEFINE([ACE_NLOGGING])
      ;;
    *)
      AC_MSG_ERROR([bad value ${enableval} for --enable-logging])
      ;;
   esac
  ],)

 AC_ARG_ENABLE([malloc-stats],
  AC_HELP_STRING([--enable-malloc-stats],[enable malloc statistics collection [[no]]]),
  [
   case "${enableval}" in
    yes)
      AC_DEFINE([ACE_HAS_MALLOC_STATS])
      ;;
    no)
      dnl nothing to do
      ;;
    *)
      AC_MSG_ERROR([bad value ${enableval} for --enable-malloc-stats])
      ;;
   esac
  ],)

 AC_ARG_ENABLE([pi-pointers],
  AC_HELP_STRING([--enable-pi-pointers],[enable pos. indep. pointers [[yes]]]),
  [
   case "${enableval}" in
    yes)
      AC_DEFINE([ACE_HAS_POSITION_INDEPENDENT_POINTERS])
      ;;
    no)
      ;;
    *)
      AC_MSG_ERROR([bad value ${enableval} for --enable-pi-pointers])
      ;;
   esac
  ],
  [
   AC_DEFINE([ACE_HAS_POSITION_INDEPENDENT_POINTERS])
  ])

 AC_ARG_ENABLE([probe],
  AC_HELP_STRING([--enable-probe],[enable ACE_Timeprobes [[no]]]),
  [
   case "${enableval}" in
    yes)
      AC_DEFINE([ACE_COMPILE_TIMEPROBES])
      ;;
    no)
      ;;
    *)
      AC_MSG_ERROR([bad value ${enableval} for --enable-probe])
      ;;
   esac
  ],)

 AC_ARG_ENABLE([static-obj-mgr],
  AC_HELP_STRING([--enable-static-obj-mgr],[enable static Object_Manager [[yes]]]),
  [
   case "${enableval}" in
    yes)
      dnl nothing to do
      ;;
    no)
      AC_DEFINE([ACE_HAS_NONSTATIC_OBJECT_MANAGER])
      ;;
    *)
      AC_MSG_ERROR([bad value ${enableval} for --enable-static-obj-mgr])
      ;;
   esac
  ],)


 AC_ARG_ENABLE([threads],
  AC_HELP_STRING([--enable-threads],[enable thread support [[yes]]]),
  [
   case "${enableval}" in
    yes)
      ace_user_enable_threads=yes
      ;;
    no)
      ace_user_enable_threads=no
      ;;
    *)
      AC_MSG_ERROR([bad value ${enableval} for --enable-threads])
      ;;
   esac
  ],
  [
    ace_user_enable_threads=yes
  ])

 AC_ARG_ENABLE([verb-not-sup],
  AC_HELP_STRING([--enable-verb-not-sup],[enable verbose ENOTSUP reports [[no]]]),
  [
   case "${enableval}" in
    yes)
      AC_DEFINE([ACE_HAS_VERBOSE_NOTSUP])
      ;;
    no)
      dnl Do nothing
      ;;
    *)
      AC_MSG_ERROR([bad value ${enableval} for --enable-verb-not-sup])
      ;;
   esac
  ],)

 AC_ARG_ENABLE([trace],
  AC_HELP_STRING([--enable-trace],[enable ACE tracing [[no]]]),
  [
   case "${enableval}" in
    yes)
      ;;
    no)
      AC_DEFINE([ACE_NTRACE])
      ;;
    *)
      AC_MSG_ERROR([bad value ${enableval} for --enable-trace])
      ;;
   esac
  ],
  [
   AC_DEFINE([ACE_NTRACE])
  ])

 AC_ARG_ENABLE([xt-reactor],
  AC_HELP_STRING([--enable-xt-reactor],[build support for the XtReactor [[no]]]),
  [
   case "${enableval}" in
    yes)
      AC_PATH_XTRA
dnl Here, if X isn't found or the user sets "--without-x" on the command
dnl line, then "no_x" is set to "yes."
      if test "$no_x" != yes; then
        ACE_XLIBS="-lX11 -lXt"
        ace_user_enable_xt_reactor=yes
      else
        ACE_XLIBS=""
        ace_user_enable_xt_reactor=no
        AC_MSG_WARN([X was not found or it was disabled.])
        AC_MSG_WARN([ACE_XtReactor will not be enabled.])
      fi
      ;;
    no)
      ACE_XLIBS=""
      ace_user_enable_xt_reactor=no
      ;;
    *)
      AC_MSG_ERROR([bad value ${enableval} for --enable-xt-reactor])
      ;;
   esac
  ],)

 AC_ARG_ENABLE([fl-reactor],
  AC_HELP_STRING([--enable-fl-reactor],[build support for the FlReactor [[no]]]),
  [
   case "${enableval}" in
    yes)
      AC_MSG_ERROR([--enable-fl-reactor currently unimplemented])
      ace_user_enable_fl_reactor=yes
      ;;
    no)
      AC_MSG_ERROR([--enable-fl-reactor currently unimplemented])
      ace_user_enable_fl_reactor=no
      ;;
    *)
      AC_MSG_ERROR([bad value ${enableval} for --enable-fl-reactor])
      ;;
   esac
  ],)

 AC_ARG_WITH([gperf],
  AC_HELP_STRING([--with-gperf],[compile the gperf program [[yes]]]),
  [
   case "${withval}" in
    yes)
      ace_user_with_gperf=yes
      AC_DEFINE([ACE_HAS_GPERF])
      if test -n "$GPERF"; then
        AC_MSG_WARN([gperf program already exists])
        AC_MSG_WARN([existing gperf may be overwritten during installation])
      fi
      ;;
    no)
      ace_user_with_gperf=no
      ;;
    *)
      AC_MSG_ERROR([bad value ${withval} for --with-gperf])
      ;;
   esac
  ],
  [
   ace_user_with_gperf=yes
   AC_DEFINE([ACE_HAS_GPERF])
   if test -n "$GPERF"; then
     AC_MSG_WARN([gperf program already exists])
     AC_MSG_WARN([existing gperf may be overwritten during installation])
   fi
  ])
 AM_CONDITIONAL([COMPILE_GPERF], [test X$ace_user_with_gperf = Xyes])

 AC_ARG_WITH([rmcast],
  AC_HELP_STRING([--with-rmcast],[compile the ACE_RMCast library [[yes]]]),
  [
   case "${withval}" in
    yes)
      ace_user_with_rmcast=yes
      ;;
    no)
      ace_user_with_rmcast=no
      ;;
    *)
      AC_MSG_ERROR([bad value ${withval} for --with-rmcast])
      ;;
   esac
  ],
  [
   ace_user_with_rmcast=yes
  ])
 AM_CONDITIONAL([BUILD_RMCAST], [test X$ace_user_with_rmcast = Xyes])

 AC_ARG_WITH([qos],
  AC_HELP_STRING([--with-qos],[compile the ACE_QoS library [[no]]]),
  [
   case "${withval}" in
    yes)
      ace_user_with_qos=yes
      ;;
    no)
      ace_user_with_qos=no
      ;;
    *)
      AC_MSG_ERROR([bad value ${withval} for --with-qos])
      ;;
   esac
  ],
  [
   ace_user_with_qos=no
  ])
 AM_CONDITIONAL([BUILD_QOS], [test X$ace_user_with_qos = Xyes])

 AC_ARG_WITH([ssl],
  AC_HELP_STRING([--with-ssl],[compile the ACE_SSL library [[no]]]),
  [
   case "${withval}" in
    yes)
      ace_user_with_ssl=yes
      ;;
    no)
      ace_user_with_ssl=no
      ;;
    *)
      AC_MSG_ERROR([bad value ${withval} for --with-ssl])
      ;;
   esac
  ],
  [
   ace_user_with_ssl=no
  ])
 AM_CONDITIONAL([BUILD_SSL], [test X$ace_user_with_ssl = Xyes])

#AC_ARG_WITH([tao],
# AC_HELP_STRING([--with-tao],[build TAO (the ACE ORB) [[yes]]]),
#              [
#               case "${withval}" in
#                yes)
#                  ace_user_with_tao=yes
#                  ;;
#                no)
#                  ;;
#                *)
#                  AC_MSG_ERROR([bad value ${withval} for --with-tao])
#                  ;;
#               esac
#              ],
#              [
#               ace_user_with_tao=yes
#              ])

 AC_ARG_WITH([tli-device],
  AC_HELP_STRING([--with-tli-device(=DEV)],[device for TCP on TLI [[/dev/tcp]]]),
  [
   case "${withval}" in
    yes)
      AC_MSG_ERROR([Specify the TLI/TCP device if you use this option.])
      ;;
    no)
      ;;
    *)
      if test -e "${withval}"; then
        AC_DEFINE_UNQUOTED([ACE_TLI_TCP_DEVICE], ["${withval}"])
      else
        AC_MSG_ERROR([TLI/TCP device ${withval} does not exist.])
      fi
      ;;
   esac
  ],)

 AC_ARG_ENABLE([reentrant],
  AC_HELP_STRING([--enable-reentrant],[enable reentrant functions [[yes]]]),
  [
   case "${enableval}" in
    yes)
      ace_user_enable_reentrant_funcs=yes
      ;;
    no)
      ace_user_enable_reentrant_funcs=no
      ;;
    *)
      AC_MSG_ERROR([bad value ${enableval} for --enable-reentrant])
      ;;
   esac
  ],
  [
   ace_user_enable_reentrant_funcs=yes
  ])


])


dnl Macros that add ACE compilation options to a `configure' script.
dnl ACE_COMPILATION_OPTIONS
AC_DEFUN([ACE_COMPILATION_OPTIONS],
[
 AC_ARG_ENABLE([debug],
  AC_HELP_STRING([--enable-debug],[enable debugging [[yes]]]),
  [
   case "${enableval}" in
    yes)
      ACE_CXXFLAGS="$ACE_CXXFLAGS $DCXXFLAGS"
      ;;
    no)
      AC_DEFINE([ACE_NDEBUG])
      ;;
    *)
      AC_MSG_ERROR([bad value ${enableval} for --enable-debug])
      ;;
   esac
  ],)

 AC_ARG_ENABLE([exceptions],
  AC_HELP_STRING([--enable-exceptions],[enable C++ exception handling [[yes]]]),
  [
   case "${enableval}" in
    yes)
      ace_user_enable_exceptions=yes
      ;;
    no)
      ace_user_enable_exceptions=no
      if test "$GXX" = yes; then
dnl Temporarily change M4 quotes to prevent "regex []" from being eaten
changequote(, )dnl
        if $CXX --version | $EGREP -v '^2\.[0-7]' > /dev/null; then
changequote([, ])dnl
          ACE_CXXFLAGS="$ACE_CXXFLAGS -fno-exceptions"
        fi
      fi
      ;;
    *)
      AC_MSG_ERROR([bad value ${enableval} for --enable-exceptions])
      ;;
   esac
  ],
  [
   ace_user_enable_exceptions=yes

dnl THE FOLLOWING WAS ONLY USED WHEN DISABLING EXCEPTION SUPPORT BY
dnl DEFAULT.
dnl
dnl    if test "$GXX" = yes; then
dnl dnl Temporarily change M4 quotes to prevent "regex []" from being eaten
dnl changequote(, )dnl
dnl      if $CXX --version | $EGREP -v '^2\.[0-7]' > /dev/null; then
dnl changequote([, ])dnl
dnl        ACE_CXXFLAGS="$ACE_CXXFLAGS -fno-exceptions"
dnl      fi
dnl    fi
  ])


 AC_ARG_ENABLE([fast],
  AC_HELP_STRING([--enable-fast],[enable -fast flag, e.g. Sun C++ [[no]]]),
  [
   case "${enableval}" in
    yes)
      ACE_CXXFLAGS="$ACE_CXXFLAGS -fast"
      ACE_CFLAGS="$ACE_CFLAGS -fast"
      ;;
    no)
      ;;
    *)
      AC_MSG_ERROR([bad value ${enableval} for --enable-fast])
      ;;
   esac
  ],)

 AC_ARG_ENABLE([inline],
  AC_HELP_STRING([--enable-inline],[enable code inlining [[yes]]]),
  [
   case "${enableval}" in
    yes)
      AC_DEFINE([__ACE_INLINE__])
      ;;
    no)
      AC_DEFINE([ACE_NO_INLINE])
      ;;
    *)
      AC_MSG_ERROR([bad value ${enableval} for --enable-inline])
      ;;
   esac
  ],
  [
   AC_DEFINE([__ACE_INLINE__])
  ])

 AC_ARG_ENABLE([optimize],
  AC_HELP_STRING([--enable-optimize],[enable additional optimizations [[yes]]]),
  [
   case "${enableval}" in
    yes)
      ace_user_enable_optimize=yes
      ;;
    no)
      AC_MSG_WARN([Optimization configure support not fully implemented yet.])
      ;;
    *)
      AC_MSG_ERROR([bad value ${enableval} for --enable-optimize])
      ;;
   esac
  ],
  [
   ace_user_enable_optimize=yes
  ])


 AC_ARG_ENABLE([profile],
  AC_HELP_STRING([--enable-profile],[enable profiling [[no]]]),
  [
   case "${enableval}" in
    yes)
      if test -z "$PROF"; then
        AC_MSG_WARN([No profiling program found.  Assuming 'prof' exists.])
        ACE_CXXFLAGS="$ACE_CXXFLAGS -p"
        ACE_CFLAGS="$ACE_CFLAGS -p"
      else
        case "$PROF" in
        gprof)
          echo "Building with 'gprof' support"
          ACE_CXXFLAGS="$ACE_CXXFLAGS -pg"
          ACE_CFLAGS="$ACE_CFLAGS -pg"
          ;;
        prof)
          echo "Building with 'prof' support"
          ACE_CXXFLAGS="$ACE_CXXFLAGS -p"
          ACE_CFLAGS="$ACE_CFLAGS -p"
          ;;
        *)
          dnl We shouldn't get here.
          AC_MSG_WARN([Assuming 'prof' exists.])
          ACE_CXXFLAGS="$ACE_CXXFLAGS -p"
          ACE_CFLAGS="$ACE_CFLAGS -p"
          ;;
        esac
      fi
      ;;
    no)
      dnl Do nothing
      ;;
    *)
      AC_MSG_ERROR([bad value ${enableval} for --enable-profile])
      ;;
   esac
  ],)

 AC_ARG_ENABLE([purify],
  AC_HELP_STRING([--enable-purify],[Purify all executables [[no]]]),
  [
   case "${enableval}" in
    yes)
      AC_CHECK_PROG([PURIFY], [purify], [purify],[])
      if test -n "$PURIFY"; then
        PURE_CACHE_BASE_DIR=/tmp/purifycache
        PURE_CACHE_DIR="${PURE_CACHE_BASE_DIR}-${LOGNAME}"
        PURE_CACHE_DIR="${PURE_CACHE_DIR}-"`basename $CXX`
        PURELINK="$PURIFY -best-effort -chain-length=20 -cache-dir=$PURE_CACHE_DIR -fds-inuse-at-exit=no -inuse-at-exit -max_threads=100"
        dnl Pick up Quantify directory from the users PATH.
		    ACE_PURIFY_DIR=`type purify | sed -e 's/.* is //' -e 's%/purify'`
        ACE_CPPFLAGS="-DACE_HAS_PURIFY -I$ACE_PURIFY_DIR"
      else
        AC_MSG_WARN([Purify program was not found.])
        AC_MSG_WARN([Disabling purify support.])
      fi
      ;;
    no)
      PURELINK=""
      ;;
    *)
      AC_MSG_ERROR([bad value ${enableval} for --enable-purify])
      ;;
   esac
  ], PURELINK="")

 AC_ARG_ENABLE([quantify],
  AC_HELP_STRING([--enable-quantify],[Quantify all executables [[no]]]),
  [
   case "${enableval}" in
    yes)
      AC_CHECK_PROG([QUANTIFY], [quantify], [quantify],[])
      if test -n "$QUANTIFY"; then
        PURE_CACHE_BASE_DIR=/tmp/purifycache
        PURE_CACHE_DIR="${PURE_CACHE_BASE_DIR}-${LOGNAME}"
        PURE_CACHE_DIR="${PURE_CACHE_DIR}-"`basename $CXX`

        PRELINK="$QUANTIFY -best-effort -max_threads=100 -cache-dir=$PURE_CACHE_DIR"
        dnl Pick up Quantify directory from the users PATH.
		    ACE_QUANTIFY_DIR=`type quantify | sed -e 's/.* is //' -e 's%/quantify$$%%'`
        ACE_CPPFLAGS="-DACE_HAS_QUANTIFY -I$ACE_QUANTIFY_DIR"
      else
        AC_MSG_WARN([Quantify program was not found.])
        AC_MSG_WARN([Disabling quantify support.])
      fi
      ;;
    no)
      PRELINK=""
      ;;
    *)
      AC_MSG_ERROR([bad value ${enableval} for --enable-quantify])
      ;;
   esac
  ], PRELINK="")

 AC_ARG_ENABLE([repo],
  AC_HELP_STRING([--enable-repo],[use GNU template repository GNU C++ with repo patches and EGCS only [[no]]]),
  [
   case "${enableval}" in
    yes)
      if test "$GXX" = yes; then
        ace_user_enable_repo=yes
        ACE_CXXFLAGS="$ACE_CXXFLAGS -frepo"
        AC_DEFINE(ACE_HAS_GNU_REPO)
      else
        ace_user_enable_repo=no
        AC_MSG_WARN([Not using GNU C++! GNU template respository disabled.])
      fi
      ;;
    no)
      ace_user_enable_repo=no
      ;;
    *)
      AC_MSG_ERROR([bad value ${enableval} for --enable-repo])
      ;;
   esac
  ],
  [
   ace_user_enable_repo=no
  ])

 AC_ARG_ENABLE([rtti],
  AC_HELP_STRING([--enable-rtti],[enable run-time type identification *Currently only for Sun C++* [[no]]]),
  [
   case "${enableval}" in
    yes)
      if test "$GXX" = no; then
        case "$host" in
          *solaris*)
               ace_user_enable_rtti=yes
               ;;
          *)
               ;;
        esac
      else
        AC_MSG_WARN([Not using Sun C++. RTTI will not be enabled.])
      fi
      ;;
    no)
      ;;
    *)
      AC_MSG_ERROR([bad value ${enableval} for --enable-rtti])
      ;;
   esac
  ],)

])
