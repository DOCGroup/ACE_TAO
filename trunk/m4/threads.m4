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

  dnl Sometimes thr_create is actually found with explicitly linking against
  dnl -lthread, so try a more "exotic" function.
  AC_CHECK_FUNC(rwlock_destroy, , AC_CHECK_LIB(thread, rwlock_destroy,,))dnl

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
