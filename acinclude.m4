dnl -------------------------------------------------------------------------
dnl       $Id$
dnl 
dnl       ACE M4 include file which contains ACE specific M4 macros
dnl       to be used by the ACE configure script.
dnl 
dnl -------------------------------------------------------------------------

dnl  Copyright (C) 1998  Ossama Othman
dnl
dnl  All Rights Reserved
dnl
dnl This library is free software; you can redistribute it and/or
dnl modify it under the current ACE distribution terms.
dnl 
dnl This library is distributed in the hope that it will be useful,
dnl but WITHOUT ANY WARRANTY; without even the implied warranty of
dnl MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.


dnl miscellaneous macros


dnl Macros to set flags for a given compiler on a given platform.
dnl The flags set here are generally only useful for _KNOWN_ compilers.

dnl ACE_SET_COMPILER_FLAGS
dnl Usage: ACE_SET_COMPILER_FLAGS
AC_DEFUN(ACE_SET_COMPILER_FLAGS, dnl
[
 AC_BEFORE([$0], [AM_PROG_LIBTOOL]) dnl

 dnl Make sure we know what C++ compiler and preprocessor we have!
 AC_REQUIRE([AC_PROG_CXX])
 AC_REQUIRE([AC_PROG_CXXCPP])
 AC_REQUIRE([AC_LANG_CPLUSPLUS])

 dnl Compiler Flag Key
 dnl    CXXFLAGS  - C++ flags to use during the configure script run and
 dnl                during ACE compilation.  The user may set this prior to
 dnl                running the configure script.  As such, it is important
 dnl                not to replace the existing value of CXXFLAGS; rather
 dnl                one should only add to it.
 dnl    ACE_CXXFLAGS - General C++ flags the configure script should set before
 dnl                   CXXFLAGS to allow the user override them.
 dnl    DCXXFLAGS - C++ debugging flags
 dnl    OCXXFLAGS - C++ optimization flags

 case "$target" in
   *aix4.2* | *aix4.3*)
     case "$CXX" in
       xlC*)
         CXXFLAGS="$CXXFLAGS"
         ACE_CXXFLAGS="$ACE_CXXFLAGS -qtempinc -qlanglvl=ansi -qflag=w:w -qinfo"
         DCXXFLAGS="-g -qcheck=nobounds:div:null"
         OCXXFLAGS="-qarch=com"
         ;;
       *)
         if test -n "$GXX"; then
           ACE_CXXFLAGS="-mcpu=common"
         fi
         ;;
     esac
     ;;
   *aix4.1*)
     case "$CXX" in
       xlC*)
         CXXFLAGS="$CXXFLAGS"
         ACE_CXXFLAGS="$ACE_CXXFLAGS -qxcall -qtempinc"
         DCXXFLAGS="-g"
         OCXXFLAGS="-qarch=ppc -qtune=604"
         ;;
       *)
         if test -n "$GXX"; then
           ACE_CXXFLAGS="-mcpu=common"
         fi
         ;;
     esac
     ;;
   *chorus*)
     ;;
   *cray*)
     ;;
   *dgux*)
     case "$CXX" in
       ec++)
         CXXFLAGS="$CXXFLAGS"
         ACE_CXXFLAGS="$ACE_CXXFLAGS -relax -v -eh"
         DCXXFLAGS="-g"
         OCXXFLAGS=""
         ;;
       *)
         if test -n "$GXX"; then
           ACE_CXXFLAGS="$ACE_CXXFLAGS"
         fi
         ;;
     esac
     ;;
   *freebsd*)
     case "$CXX" in
       *)
         if test -n "$GXX"; then       
           CXXFLAGS="$CXXFLAGS"
           ACE_CXXFLAGS="$ACE_CXXFLAGS -w -fno-strict-prototypes"
           DCXXFLAGS=""
           OCXXFLAGS=""
         fi
         ;;
     esac
     ;;
   *hpux*)
     case "$CXX" in
       CC)
         CXXFLAGS="$CXXFLAGS -pta -ti,/bin/true -tr,/bin/true"
         ACE_CXXFLAGS="$ACE_CXXFLAGS -Aa -z +a1"
         DCXXFLAGS="-g"
         OCXXFLAGS=""
         ;;
       aCC)
         CXXFLAGS="$CXXFLAGS"
         ACE_CXXFLAGS="$ACE_CXXFLAGS +W829,302"
         DCXXFLAGS="-g"
         OCXXFLAGS=""
         ;;
       *)
         if test -n "$GXX"; then
           ACE_CXXFLAGS="$ACE_CXXFLAGS -w"
         fi
         ;;
     esac
     ;;
   *irix*)

     ;;
   *linux*)
     case "$CXX" in
       *)
         if test -n "$GXX"; then
           CXXFLAGS="$CXXFLAGS"
           ACE_CXXFLAGS="$ACE_CXXFLAGS"
           DCXXFLAGS="$DCXXFLAGS"
           OCXXFLAGS="-O3"
         fi
         ;;
     esac
     ;;
   *lynxos*)
     ;;
   *m88k*)
     ;;
   *mvs*)
     ;;
   *netbsd*)
     ;;
   *osf*)
     ;;
   *psos*)
     ;;
   *sco*)
     ;;
   *sunos4*)
     ;;
   *solaris2*)
     ;;
   *tandem*)
     ;;
   *unixware*)
     ;;
   *vxworks*)
     ;;
   *)
     CXXFLAGS="$CXXFLAGS"
     ACE_CXXFLAGS="$ACE_CXXFLAGS"
     DCXXFLAGS="-g"
     OCXXFLAGS="-O"
     ;;
 esac
])

dnl add compiler flags to the CXXFLAGS and CFLAGS variables when doing an
dnl AC_TRY_COMPILE (not ACE_TRY_COMPILE).
dnl Use this macro when adding include directories to the compiler flags,
dnl for example.
dnl Usage: ACE_TRY_COMPILE(COMPILER-FLAGS, INCLUDES, FUNCTION-BODY,
dnl                        [ACTION-IF-FOUND [,ACTION-IF-NOT-FOUND]])
AC_DEFUN(ACE_TRY_COMPILE, dnl
[
 save_CXXFLAGS="$CXXFLAGS"
 CXXFLAGS="$CXXFLAGS $1"

 save_CFLAGS="$CFLAGS"
 CFLAGS="$CFLAGS $1"

 AC_TRY_COMPILE($2, $3, $4, $5)

 dnl Restore the C++ and C flags
 CXXFLAGS="$save_CXXFLAGS"
 CFLAGS="$save_CFLAGS"

])

dnl Create a temporary empty file and remove it after commands are done using
dnl it.  The directory in which the temporary file will be created in must
dnl exist.  Files will be created under the source directory, not the build
dnl directory.
dnl Use this macro when you need a particular file available but want it to be
dnl empty.  This is useful to prevent conflicts with autoconf's confdefs.h
dnl header when doing an AC_TRY_COMPILE.
dnl Usage: ACE_USE_TEMP_FILE(TEMP-FILE-TO-CREATE, COMMANDS-THAT-WILL-USE-IT)
AC_DEFUN(ACE_USE_TEMP_FILE, dnl
[
 if test -f ${srcdir}/$1; then
   mv ${srcdir}/$1 ${srcdir}/$1.conf
 fi

 touch ${srcdir}/$1

 $2

 if test -f ${srcdir}/$1.conf; then
   mv ${srcdir}/$1.conf ${srcdir}/$1
 else
   rm ${srcdir}/$1
 fi
])

dnl Run given test(s) with warnings converted to errors
dnl Usage: ACE_CONVERT_WARNINGS_TO_ERRORS(TEST-BLOCK)
AC_DEFUN(ACE_CONVERT_WARNINGS_TO_ERRORS, dnl
[
dnl If we are using GNU C++, add the "-Werror" compiler flag to the 
dnl current set of flags so that compiler warnings become errors.  We
dnl do this to cause certain tests to fail when they are supposed to
dnl fail.  Some of the tests pass because the GNU C++ compiler issues
dnl warnings instead of errors when errors should occur.
  TEMPCXXFLAGS=""
  if test -n "$GXX"; then
    TEMPCXXFLAGS="$CXXFLAGS"
    CXXFLAGS="$CXXFLAGS -Werror"
  fi

  $1

  if test -n "$TEMPCXXFLAGS"; then
    CXXFLAGS="$TEMPCXXFLAGS"
  fi

])

dnl Wrapper around AC_CACHE_VAL used to ensure "ACTION-IF" commands are run
dnl even if results have been previously cached.
dnl Usage: ACE_CACHE_CHECK(MESSAGE, CACHE-ID, COMMANDS-TO-SET-CACHE-VAL,
dnl                        ACTION-IF-CACHE-ID-IS-YES,
dnl                        ACTION-IF-CACHE-ID-IS-NO)
dnl The COMMANDS-TO-SET-CACHE-VAL should set the CACHE-ID to yes or "no,"
dnl otherwise the "ACTION-IF*" commands may not run.  The
dnl COMMANDS-TO-SET-CACHE-VAL should only set the CACHE value.  For example,
dnl no AC_DEFINES should be placed in the COMMANDS-TO-SET-CACHE-VAL.
AC_DEFUN(ACE_CACHE_CHECK,
[
  AC_MSG_CHECKING([$1])
  AC_CACHE_VAL([$2], [$3])
  AC_MSG_RESULT([$]$2)
  if test "[$]$2" = yes; then
    ace_just_a_place_holder=fixme
ifelse([$4], , :, [$4])
  else
    ace_just_a_place_holder=fixme
ifelse([$5], , , [$5
])dnl
  fi
])

dnl   checks for programs

dnl   checks for libraries

dnl Make sure thread library exists
dnl We need to be careful when tests for other thread libraries are
dnl added that we don't screw up handling of "ace_user_enable_threads"
dnl Tests should probably be more platform specific later on.

dnl Check for thread related libraries and compiler flags
dnl Usage: ACE_CHECK_THREADS
AC_DEFUN(ACE_CHECK_THREADS, dnl
[
dnl  AC_REQUIRE([AC_PROG_CXX])
dnl  AC_REQUIRE([AC_PROG_CXXCPP])
dnl  AC_REQUIRE([AC_LANG_CPLUSPLUS])

dnl Check if compiler accepts specific flag to enable threads
 ACE_CACHE_CHECK(if compiler may need a thread flag,
   ace_cv_feature_may_need_thread_flag,
   [
    ACE_CHECK_THREAD_FLAGS(
      [
       ace_cv_feature_may_need_thread_flag=no
      ],
      [
       ace_cv_feature_may_need_thread_flag=yes
      ])
   ],
   [
    ace_cv_feature_thread_flag_set=no

    save_CXXFLAGS="$CXXFLAGS"
    save_CFLAGS="$CFLAGS"

    CXXFLAGS="$CXXFLAGS -mt"
    CFLAGS="$CFLAGS -mt"

    ACE_CACHE_CHECK(if compiler can use -mt flag,
      ace_cv_feature_has_mt_flag,
      [
       ACE_CHECK_THREAD_FLAGS(
         [
          ace_cv_feature_has_mt_flag=yes
         ],
         [
          ace_cv_feature_has_mt_flag=no
         ])
      ],
      [
       ace_cv_feature_thread_flag_set=yes
      ],
      [
       CXXFLAGS="$save_CXXFLAGS"
       CFLAGS="$save_CFLAGS"
      ])

    if test "$ace_cv_feature_thread_flag_set" = no; then
 
      CXXFLAGS="$CXXFLAGS -pthread"
      CFLAGS="$CFLAGS -pthread"

      ACE_CACHE_CHECK(if compiler can use -pthread flag,
        ace_cv_feature_has_pthread_flag,
        [
         ACE_CHECK_THREAD_FLAGS(
           [
            ace_cv_feature_has_pthread_flag=yes
           ],
           [
            ace_cv_feature_has_pthread_flag=no
           ])
        ],
        [
         ace_cv_feature_thread_flag_set=yes
        ],
        [
         CXXFLAGS="$save_CXXFLAGS"
         CFLAGS="$save_CFLAGS"
        ])

    fi dnl test "$ace_cv_feature_thread_flag_set" = no

    if test "$ace_cv_feature_thread_flag_set" = no; then

      CXXFLAGS="$CXXFLAGS -pthreads"
      CFLAGS="$CFLAGS -pthreads"

      ACE_CACHE_CHECK(if compiler can use -pthreads flag,
        ace_cv_feature_has_pthreads_flag,
        [
         ACE_CHECK_THREAD_FLAGS(
           [
            ace_cv_feature_has_pthreads_flag=yes
           ],
           [
            ace_cv_feature_has_pthreads_flag=no
           ])
        ],
        [
         ace_cv_feature_thread_flag_set=yes
        ],
        [
         CXXFLAGS="$save_CXXFLAGS"
         CFLAGS="$save_CFLAGS"
        ])

    fi dnl test "$ace_cv_feature_thread_flag_set" = no

    if test "$ace_cv_feature_thread_flag_set" = no; then

      CXXFLAGS="$CXXFLAGS -mthreads"
      CFLAGS="$CXXFLAGS -mthreads"

      ACE_CACHE_CHECK(if compiler can use -mthreads flag,
        ace_cv_feature_has_mthreads_flag,
        [
         ACE_CHECK_THREAD_FLAGS(
           [
            ace_cv_feature_has_mthreads_flag=yes
           ],
           [
            ace_cv_feature_has_mthreads_flag=no
           ])
        ],
        [
         ace_cv_feature_thread_flag_set=yes
        ],
        [
         CXXFLAGS="$save_CXXFLAGS"
         CFLAGS="$save_CFLAGS"
        ])

    fi dnl test "$ace_cv_feature_thread_flag_set" = no

    if test "$ace_cv_feature_thread_flag_set" = no; then

      CXXFLAGS="$CXXFLAGS -threads"
      CFLAGS="$CXXFLAGS -threads"

      ACE_CACHE_CHECK(if compiler can use -threads flag,
        ace_cv_feature_has_dash_threads_flag,
        [
         ACE_CHECK_THREAD_FLAGS(
           [
            ace_cv_feature_has_dash_threads_flag=yes
           ],
           [
            ace_cv_feature_has_dash_threads_flag=no
           ])
        ],
        [
         ace_cv_feature_thread_flag_set=yes
        ],
        [
         CXXFLAGS="$save_CXXFLAGS"
         CFLAGS="$save_CFLAGS"
        ])

    fi dnl test "$ace_cv_feature_thread_flag_set" = no
   ],
   [
    dnl Do nothing
   ])

dnl Check for UNIX International Threads -- STHREADS

  ace_has_sthreads=no

  AC_CHECK_FUNC(thr_create,
    [
     ace_has_sthreads=yes
     AC_DEFINE(ACE_HAS_STHREADS)
    ],
    [
     AC_CHECK_LIB(thread, thr_create, dnl
                  [
                   ace_has_sthreads=yes
dnl Since we AC_DEFINE(ACE_HAS_STHREADS), the default behavior of
dnl of adding "-lthread" to the "LIBS" variable no longer works.
dnl So, we have to add it manually.
                   LIBS="$LIBS -lthread"
                   AC_DEFINE(ACE_HAS_STHREADS)
                  ],)
    ])

dnl Check for POSIX threads

    dnl Check if platform provides pthreads backward compatibility macros
    dnl (Some platforms may define some pthread functions such as
    dnl  pthread_create() as macros when using a later implementation of
    dnl  pthreads.  For example, Digital Unix 4.0 #defines a pthread_create
    dnl  macro as "__pthread_create" to allow the new implemenation of
    dnl  pthread_create() to co-exist with the old implementation of
    dnl  of pthread_create().)

    AC_CACHE_CHECK(for pthreads backward compatibility macros,
      ace_cv_lib_pthread_compat_macros,
      [
       AC_EGREP_CPP(ACE_PTHREAD_MACROS,
         [
#include <pthread.h>

#if defined (pthread_create)
             ACE_PTHREAD_MACROS
#endif
         ],
         [
          ace_cv_lib_pthread_compat_macros=yes
         ],
         [
          ace_cv_lib_pthread_compat_macros=no
         ])
      ])

    ace_has_pthreads=no

dnl Check for POSIX threads -- PTHREADS
    ACE_CHECK_FUNC(pthread_create, pthread.h,
      [
       ace_has_pthreads=yes
       AC_DEFINE(ACE_HAS_PTHREADS)
      ],
      [
       AC_CHECK_LIB(pthread, pthread_create, dnl
                    [
                     ace_has_pthreads=yes
dnl Since we AC_DEFINE(ACE_HAS_PTHREADS), the default behavior of
dnl of adding "-lpthread" to the "LIBS" variable no longer works.
dnl So, we have to add it manually.
                     LIBS="$LIBS -lpthread"
                     AC_DEFINE(ACE_HAS_PTHREADS)
                    ],)

dnl Check if pthread function names are mangled (e.g. DU 4.0) to maintain
dnl older Pthread Draft compatibility.
       if test "$ace_has_pthreads" = no &&
          test "$ace_cv_lib_pthread_compat_macros" = yes; then
         ACE_CHECK_LIB(pthread, pthread_create, pthread.h, dnl
                      [
                       ace_has_pthreads=yes
dnl Since we AC_DEFINE(ACE_HAS_PTHREADS), the default behavior of
dnl of adding "-lpthread" to the "LIBS" variable no longer works.
dnl So, we have to add it manually.
                       LIBS="$LIBS -lpthread"
                       AC_DEFINE(ACE_HAS_PTHREADS)
                      ],)
       fi dnl test "$ace_has_pthreads" = no && have compatibilty macros

dnl Check if we need to use -lpthreads instead (e.g. AIX 4.2)
       if test "$ace_has_pthreads" = no; then
         AC_CHECK_LIB(pthreads, pthread_create, dnl
                      [
                       ace_has_pthreads=yes
dnl Since we AC_DEFINE(ACE_HAS_PTHREADS), the default behavior of
dnl of adding "-lpthread" to the "LIBS" variable no longer works.
dnl So, we have to add it manually.
                       LIBS="$LIBS -lpthreads"
                       AC_DEFINE(ACE_HAS_PTHREADS)
                      ],)
       fi dnl test "$ace_has_pthreads" = no

       if test "$ace_has_pthreads" = no; then  
dnl Check for POSIX threads in -lc_r
dnl Check if we already have the necessary library, first
         AC_CHECK_LIB(c_r, pthread_create, dnl
                      [
                       ace_has_pthreads=yes
dnl Since we AC_DEFINE(ACE_HAS_PTHREADS), the default behavior of
dnl of adding "-lc_r" to the "LIBS" variable no longer works.
dnl So, we have to add it manually.
                       LIBS="$LIBS -lc_r"
                       AC_DEFINE(ACE_HAS_PTHREADS)
                      ],)
       fi dnl  test "$ace_has_pthreads" = no

       if test "$ace_has_pthreads" = no; then  
dnl Check for POSIX threads in -lgthreads, i.e. FSU Pthreads
         AC_CHECK_LIB(gthreads, pthread_create, dnl
                      [
                       ace_has_pthreads=yes
dnl Since we AC_DEFINE(ACE_HAS_PTHREADS), the default behavior of
dnl of adding "-lgthreads" to the "LIBS" variable no longer works.
dnl So, we have to add it manually.
                       LIBS="$LIBS -lgthreads"
                       AC_DEFINE(ACE_HAS_PTHREADS)
                      ],)
       fi dnl  test "$ace_has_pthreads" = no
      ])

dnl If we don't have any thread library, then disable threading altogether!
  if test "$ace_has_pthreads" != yes && 
     test "$ace_has_sthreads" != yes; then
       ace_user_enable_threads=no
  fi

])

dnl This macro will check that the current compiler flags do something
dnl useful in terms of thread libraries and/or functions.
dnl Usage: ACE_CHECK_THREAD_FLAGS(ACTION-IF-USABLE [, ACTION-IF-NOT-USABLE]])
AC_DEFUN(ACE_CHECK_THREAD_FLAGS, dnl
[
ACE_CONVERT_WARNINGS_TO_ERRORS([
AC_TRY_LINK(
ifelse(AC_LANG, CPLUSPLUS, [#ifdef __cplusplus
extern "C"
#endif

#ifndef _REENTRANT
#error _REENTRANT was not defined
THROW ME AN ERROR!
#endif
])dnl
[
char thr_create();
], [
thr_create();
], [$1],
[
dnl
  AC_REQUIRE([AC_PROG_AWK])

  AC_TRY_CPP(
    [
#include <pthread.h>
    ], ace_header_exists=yes, ace_header_exists=no)

  cat > conftest.$ac_ext <<EOF

#include <pthread.h>
  ACE_REAL_FUNCTION pthread_create

EOF

  if test "$ace_header_exists" = yes; then
    if (eval "$ac_cpp conftest.$ac_ext") 2>&5 |
       egrep "ACE_REAL_FUNCTION" | 
       (eval "$AWK '{print \[$]2}' > conftest.awk 2>&1"); then
         rm -f conftest.$ac_ext
         ace_real_function=`cat conftest.awk`
         rm -f conftest.awk
    fi
  else
    ace_real_function="pthread_create"
  fi dnl test "$ace_header_not_exist" != yes

AC_TRY_LINK(
ifelse(AC_LANG, CPLUSPLUS, [#ifdef __cplusplus
extern "C"
#endif

#ifndef _REENTRANT
#error _REENTRANT was not defined
THROW ME AN ERROR!
#endif
])dnl
[
char $ace_real_function();
], [
$ace_real_function();
], [$1],[$2])

dnl
  ])
 ])
])

dnl   checks for header files

dnl   checks for typedefs

dnl Check for specific typedef in given header file
dnl Usage: ACE_CHECK_TYPE(TYPEDEF, INCLUDE,
dnl                       [ACTION-IF-FOUND [, ACTION-IF-NOT-FOUND]])
dnl This macro can only check for one typedef in one header file at a time!!
AC_DEFUN(ACE_CHECK_TYPE, dnl
[
dnl  AC_REQUIRE([AC_PROG_CXX])
dnl  AC_REQUIRE([AC_PROG_CXXCPP])
dnl  AC_REQUIRE([AC_LANG_CPLUSPLUS])

  ACE_CACHE_CHECK(for $1 in $2, ace_cv_type_$1,
    [
     AC_TRY_COMPILE(
       [
#include <$2>
       ],
       [
        $1 ace_$1;
       ],
       [
        ace_cv_type_$1=yes
       ],
       [
        ace_cv_type_$1=no
       ])
    ], $3, $4)
])


dnl   checks for structures

dnl Check for specific struct in given header file
dnl Usage: ACE_CHECK_STRUCT(STRUCTURE, INCLUDE,
dnl                        [ACTION-IF-FOUND [, ACTION-IF-NOT-FOUND]])
dnl This macro can only check for one struct in one header file at a time!!
AC_DEFUN(ACE_CHECK_STRUCT, dnl
[
dnl  AC_REQUIRE([AC_PROG_CXX])
dnl  AC_REQUIRE([AC_PROG_CXXCPP])
dnl  AC_REQUIRE([AC_LANG_CPLUSPLUS])

dnl Do the transliteration at runtime so arg 1 can be a shell variable.
dnl  ac_safe=`echo "$1" | sed 'y%./+-%__p_%'`

  ACE_CACHE_CHECK(for struct $1 in $2, ace_cv_struct_$1,
    [
     ACE_TRY_COMPILE_STRUCT($1, $2,
       [
        ace_cv_struct_$1=yes
       ],
       [
        ace_cv_struct_$1=no
       ])
    ], $3, $4)
])

dnl Check for specific struct in given header file by compiling a test
dnl program.  This macro is used by ACE_CHECK_STRUCT.
dnl Usage: ACE_TRY_COMPILE_STRUCT(STRUCTURE, INCLUDE,
dnl                         [ACTION-IF-SUCCESSFUL[, ACTION-IF-NOT-SUCCESSFUL]])
dnl This macro can only check for one struct in one header file at a time!!
AC_DEFUN(ACE_TRY_COMPILE_STRUCT, dnl
[
dnl  AC_REQUIRE([AC_PROG_CXX])
dnl  AC_REQUIRE([AC_PROG_CXXCPP])
dnl  AC_REQUIRE([AC_LANG_CPLUSPLUS])

     AC_TRY_COMPILE(
       [
#include <$2>
       ],
       [
        struct $1 ace_$1;
       ],
       [
        $3
       ],
       [
dnl Some compilers don't like the "struct" but we need the struct for some
dnl platforms to resolve ambiguities between functions and structures with
dnl with the same name.  So, we try the same test but without "struct" if
dnl the above test with "struct" fails.  If both tests fail, then we can
dnl be reasonably sure that we don't have the structure we are testing for.
        AC_TRY_COMPILE(
          [
#include <$2>
          ],
          [
           $1 ace_$1;
          ],
          [
           $3
          ],
          [
           $4
          ])
       ])
])

dnl   checks for variables

dnl   checks for compiler characteristics

dnl   checks for library functions

dnl Check for function using prototype in header
dnl This macro is used if a function is called a different name in a given
dnl library than what is in the header file but the difference is made
dnl transparent to the user since the header may provide a macro to make
dnl things "transparent."  If the given header does not exist then this
dnl macro acts just like the standard AC_CHECK_FUNC macro.
dnl Usage: ACE_CHECK_FUNC(FUNCTION, HEADER,
dnl                        [ACTION-IF-FOUND [, ACTION-IF-NOT-FOUND]])
AC_DEFUN(ACE_CHECK_FUNC, dnl
[
dnl  AC_REQUIRE([AC_PROG_CXX])
dnl  AC_REQUIRE([AC_PROG_CXXCPP])
dnl  AC_REQUIRE([AC_LANG_CPLUSPLUS])
  AC_REQUIRE([AC_PROG_AWK])

  AC_TRY_CPP(
    [
#include <$2>
    ], ace_header_exists=yes, ace_header_exists=no)

  cat > conftest.$ac_ext <<EOF

#include <$2>
  ACE_REAL_FUNCTION $1

EOF

  if test "$ace_header_exists" = yes; then
    if test -z "$AWK"; then
      AC_MSG_WARN(No awk program found.  "Real" function may not be found.)
    fi

    if (eval "$ac_cpp conftest.$ac_ext") 2>&5 |
       egrep "ACE_REAL_FUNCTION" | 
       (eval "$AWK '{print \[$]2}' > conftest.awk 2>&1"); then
         rm -f conftest.$ac_ext
         ace_real_function=`cat conftest.awk`
         rm -f conftest.awk
    fi

    if test $1 != "$ace_real_function"; then
      AC_MSG_CHECKING(for real $1 from $2)
      AC_MSG_RESULT($ace_real_function)
    fi
  else
    ace_real_function=$1
  fi dnl test "$ace_header_not_exist" != yes

  AC_CHECK_FUNC($ace_real_function, $3, $4)
])

dnl Check for function in library using prototype in header
dnl This macro is used if a function is called a different name in a given
dnl library than what is in the header file but the difference is made
dnl transparent to the user since the header may provide a macro to make
dnl things "transparent."  If the given header does not exist then this
dnl macro acts just like the standard AC_CHECK_LIB macro.
dnl Usage: ACE_CHECK_LIB(LIBRARY, FUNCTION, HEADER,
dnl                        [ACTION-IF-FOUND [, ACTION-IF-NOT-FOUND]])
AC_DEFUN(ACE_CHECK_LIB, dnl
[
dnl  AC_REQUIRE([AC_PROG_CXX])
dnl  AC_REQUIRE([AC_PROG_CXXCPP])
dnl  AC_REQUIRE([AC_LANG_CPLUSPLUS])
  AC_REQUIRE([AC_PROG_AWK])

  AC_TRY_CPP(
    [
#include <$3>
    ], ace_header_exists=yes, ace_header_exists=no)

  cat > conftest.$ac_ext <<EOF

#include <$3>
  ACE_REAL_FUNCTION $2

EOF

  if test "$ace_header_exists" = yes; then
    if test -z "$AWK"; then
      AC_MSG_WARN(No awk program found.  "Real" function in library may not be found.)
    fi

    if (eval "$ac_cpp conftest.$ac_ext") 2>&5 |
       egrep "ACE_REAL_FUNCTION" |
       eval "$AWK '{print \[$]2}'" > conftest.awk 2>&1; then
        rm -f conftest.$ac_ext
        ace_real_function=`cat conftest.awk`
        rm -f conftest.awk
    fi

    if test $2 != "$ace_real_function"; then
      AC_MSG_CHECKING(for real $2 from $3)
      AC_MSG_RESULT($ace_real_function)
    fi
  else
    ace_real_function=$2
  fi dnl test "$ace_header_not_exist" != yes

  AC_CHECK_LIB($1, $ace_real_function, $4, $5)
])


dnl   checks for structures

dnl   checks for system services
