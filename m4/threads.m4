dnl -------------------------------------------------------------------------
dnl       $Id$
dnl 
dnl       threads.m4
dnl 
dnl       ACE M4 include file which contains ACE specific M4 macros
dnl       for configuring thread support.  This file is to be used
dnl       with the configure script.
dnl 
dnl -------------------------------------------------------------------------

dnl  Copyright (C) 1998, 1999  Ossama Othman
dnl
dnl  All Rights Reserved
dnl
dnl This library is free software; you can redistribute it and/or
dnl modify it under the current ACE distribution terms.
dnl 
dnl This library is distributed in the hope that it will be useful,
dnl but WITHOUT ANY WARRANTY; without even the implied warranty of
dnl MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

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
    dnl The compiler/platform has no thread support linked in by default
    dnl so search for a usable compiler flag to enable thread support.
    dnl If no thread flag is found then the remaining tests should still
    dnl figure out how to enable thread support via library checks.
    ACE_SEARCH_THREAD_FLAGS(mt pthread pthreads mthreads threads Kthread,,)
   ],
   [
    dnl Do nothing
   ])

 dnl Check for UNIX International Threads -- STHREADS
 ACE_SEARCH_LIBS(thr_create, thread,
   [
    ace_has_sthreads=yes
    AC_DEFINE(ACE_HAS_STHREADS)
   ],
   [
    ace_has_sthreads=no
   ])

 dnl Sometimes thr_create is actually found with explicitly linking against
 dnl -lthread, so try a more "exotic" function.
 ACE_SEARCH_LIBS(rwlock_destroy, thread,,)

 dnl Check for POSIX threads
 ACE_SEARCH_LIBS(pthread_create, pthread pthreads c_r gthreads,
   [
    ace_has_pthreads=yes
    AC_DEFINE(ACE_HAS_PTHREADS)
   ],
   [
    dnl Check if platform provides pthreads backward compatibility macros
    dnl (Some platforms may define some pthread functions such as
    dnl  pthread_create() as macros when using a later implementation of
    dnl  pthreads.  For example, Digital Unix 4.0 #defines a pthread_create
    dnl  macro as "__pthread_create" to allow the new implemenation of
    dnl  pthread_create() to co-exist with the old implementation of
    dnl  of pthread_create().)

    ACE_CACHE_CHECK(for pthreads backward compatibility macros,
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
      ],
      [
       dnl Check if pthread function names are mangled (e.g. DU 4.0)
       dnl to maintain older Pthread Draft compatibility.
       ACE_CHECK_FUNC(pthread_create, pthread.h,
         [
          ace_has_pthreads=yes
          AC_DEFINE(ACE_HAS_PTHREADS)
         ],
         [
          ACE_CHECK_LIB(pthread, pthread_create, pthread.h, dnl
            [
             ace_has_pthreads=yes
             dnl Since we AC_DEFINE(ACE_HAS_PTHREADS), the default behavior
             dnl of adding "-lpthread" to the "LIBS" variable no longer
             dnl works, so we have to add it manually.
             LIBS="$LIBS -lpthread"
             AC_DEFINE(ACE_HAS_PTHREADS)
            ],
            [
             ace_has_pthreads=yes
            ])
         ])
      ],
      [
       ace_has_pthreads=no
      ])
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
[
#ifndef _REENTRANT
#error _REENTRANT was not defined
THROW ME AN ERROR!
#endif
]
ifelse(AC_LANG, CPLUSPLUS, [#ifdef __cplusplus
extern "C"
#endif
])
[
char thr_create();
], [
thr_create();
], [$1],
[
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
[
#ifndef _REENTRANT
#error _REENTRANT was not defined
THROW ME AN ERROR!
#endif
]
ifelse(AC_LANG, CPLUSPLUS, [#ifdef __cplusplus
extern "C"
#endif
])dnl
[
char $ace_real_function();
], [
$ace_real_function();
], [$1],[$2])

  ])
 ])
])

dnl Check what compiler thread flag may be used, if any, from the given list.
dnl The flag list is separated by white space.
dnl Usage: ACE_SEARCH_THREAD_FLAGS(THREAD-FLAG-LIST,
dnl                                [ACTION-IF-FOUND], [ACTION-IF-NOT-FOUND])
AC_DEFUN(ACE_SEARCH_THREAD_FLAGS, dnl
[
 ACE_CACHE_CHECK(for compiler thread flag,
   ace_cv_thread_flag_search,
   [
    ace_save_CXXFLAGS="$CXXFLAGS"
    ace_save_CFLAGS="$CFLAGS"

    for i in $1; do
      CXXFLAGS="$CXXFLAGS -$i"
      CFLAGS="$CFLAGS -$i"

      ACE_CHECK_THREAD_FLAGS(
        [
         ace_cv_thread_flag_search="-$i"

         dnl A usable flag was found so break out of the loop.
         break;
        ],
        [
         ace_cv_thread_flag_search=no
        ])

      dnl Reset the flags for the next flag check.
      CXXFLAGS="$ace_save_CXXFLAGS"
      CFLAGS="$ace_save_CFLAGS"
    done

    dnl Reset the flags to a consistent state.
    dnl This prevents duplicate flags from being added to
    dnl the C/CXXFLAGS variable.
    CXXFLAGS="$ace_save_CXXFLAGS"
    CFLAGS="$ace_save_CFLAGS"
   ],
   [
    dnl Add the found/cached thread flag to the C/CXXFLAGS variables
    CXXFLAGS="$CXXFLAGS $ace_cv_thread_flag_search"
    CFLAGS="$CFLAGS $ace_cv_thread_flag_search"

    $2
   ],
   [
    $3
   ])
])
