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
    ifelse(AC_LANG, [CPLUSPLUS],
      [ace_save_CXXFLAGS="$CXXFLAGS"],
      [ace_save_CFLAGS="$CFLAGS"])

    ACE_CHECK_THREAD_FLAGS(
      [
       ace_cv_feature_may_need_thread_flag=no
      ],
      [
       ace_cv_feature_may_need_thread_flag=yes
      ])
    dnl Reset the flags to a consistent state.
    dnl This prevents duplicate flags from being added to
    dnl the C/CXXFLAGS variable.
    ifelse(AC_LANG, [CPLUSPLUS],
           [CXXFLAGS="$ace_save_CXXFLAGS"],[CFLAGS="$ace_save_CFLAGS"])
   ],
   [
    dnl The compiler/platform has no thread support linked in by default
    dnl so search for a usable compiler flag to enable thread support.
    dnl If no thread flag is found then the remaining tests should still
    dnl figure out how to enable thread support via library checks.
    ACE_SEARCH_THREAD_FLAGS(
      [mt pthread pthreads mthreads threads Kthread kthread -thread_safe],,)
      dnl NOTE: "-thread_safe" is correct, not "thread_safe."
      dnl       KAI C++ uses the flag "--thread_safe" which is why
      dnl       "-thread_safe" is passed as the flag to test.
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

 dnl Check if any thread related preprocessor flags are needed.
 ACE_CHECK_THREAD_CPPFLAGS

 dnl Check for POSIX threads
 dnl
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
    dnl Add thread preprocessor flags, if any.
    ace_save_CPPFLAGS="$CPPFLAGS"
    CPPFLAGS="$ACE_THR_CPPFLAGS $CPPFLAGS" dnl User's CPPFLAGS go last

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

    dnl Reset the preprocessor flags
    CPPFLAGS="$ace_save_CPPFLAGS"
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
    ACE_SEARCH_LIBS(pthread_create, pthread pthreads c_r gthreads,
      [
       ace_has_pthreads=yes
       AC_DEFINE(ACE_HAS_PTHREADS)

       dnl This is ugly but some platforms appear to implement stubs
       dnl in the C library, so it is possible that a no-op function
       dnl may be found.  Here we check for a few more functions in
       dnl case this is so.  This may not be fool proof since the
       dnl additional functions themselves may be implemented as
       dnl stubs, in which case the same problem will occur!
       dnl Another solution is to check for the function using
       dnl AC_CHECK_LIB but that will force the library to be added
       dnl to the LIBS variable, which may not even be necessary.  In
       dnl any case, it may be the better solution.  If problems arise
       dnl in the future regarding this issue, then we should probably
       dnl switch to doing an AC_CHECK_LIB before each ACE_SEARCH_LIBS
       dnl below.

       dnl Search for functions in more recent standards first.

       dnl Note that the functions were chosen since they appear to be
       dnl more "exotic" than the less "interesting" functions such as
       dnl pthread_mutexattr_init.

       dnl Draft 7 and Standard
       ACE_SEARCH_LIBS([pthread_setschedparam],
                       [pthread pthreads c_r gthreads],,
          [
           dnl Draft 6
           ACE_SEARCH_LIBS([pthread_attr_setprio],
                           [pthread pthreads c_r gthreads],,
              [
               dnl Draft 4
               ACE_SEARCH_LIBS([pthread_setprio],
                               [pthread pthreads c_r gthreads],,)
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
 AC_REQUIRE([AC_PROG_AWK])

 ACE_CONVERT_WARNINGS_TO_ERRORS([

dnl Check for UI thread support first.

 dnl Because some platforms are brain damaged enough to provide
 dnl useless thread function stubs, link tests may succeed despite the
 dnl fact the stubs are no-ops.  This forces us to use a run-time test
 dnl to get around this nuisance by checking the return value of
 dnl thr_create().  The cross-compiled case will use a link-time
 dnl test, instead.

 AC_TRY_RUN(
   [
#include <thread.h>
   ]
ifelse(AC_LANG, CPLUSPLUS, [#ifdef __cplusplus
extern "C"
#endif
])
   [
void * ace_start_func(void *arg)
{
 return arg;
};

int main (int argc, char **argv)
{
 thread_t tid = 0;

 return thr_create (0, 0, ace_start_func, 0, 0, &tid);
}
   ],
   [$1],
   [
 dnl Now check for POSIX thread support.

 dnl Because some platforms are brain damaged enough to provide
 dnl useless thread function stubs, link tests may succeed despite the
 dnl fact the stubs are no-ops.  This forces us to use a run-time test
 dnl to get around this nuisance by checking the return value of
 dnl pthread_create().  The cross-compiled case will use a link-time
 dnl test, instead.
    AC_TRY_RUN(
      [
#include <pthread.h>
      ]
ifelse(AC_LANG, CPLUSPLUS, [#ifdef __cplusplus
extern "C"
#endif
])
      [
void * ace_start_func(void *arg)
{
 return arg;
};

int main (int argc, char **argv)
{
 pthread_t tid = 0;

 return pthread_create (&tid, 0, ace_start_func, 0);
}
      ],
      [$1],
      [$2],
      [
       dnl POSIX threads cross-compiled case

       AC_TRY_CPP(
         [
#include <pthread.h>
         ],
         [
          cat > conftest.$ac_ext <<EOF

#include <pthread.h>
  ACE_REAL_FUNCTION pthread_create

EOF

          if (eval "$ac_cpp conftest.$ac_ext") 2>&5 |
             egrep "ACE_REAL_FUNCTION" | 
             (eval "$AWK '{print \[$]2}' > conftest.awk 2>&1"); then
               rm -f conftest.$ac_ext
               ace_real_function=`cat conftest.awk`
               rm -f conftest.awk
          fi
         ],
         [
          ace_real_function="pthread_create"
         ])

       AC_TRY_LINK(
         [
/*
 * Don't use definition of specific preprocessor macros as criterion
 * for determining if thread support is found.
 *
 * #if !defined (_REENTRANT) && !defined (_THREAD_SAFE)
 * #error Neither _REENTRANT nor _THREAD_SAFE were defined.
 * THROW ME AN ERROR!
 * #endif
 *
 */
          ]
ifelse(AC_LANG, CPLUSPLUS, [#ifdef __cplusplus
extern "C"
#endif
])dnl
         [
char $ace_real_function();
         ],
         [
$ace_real_function();
         ],
         [$1],
         [$2])
      ])
   ],
   [
    dnl UI threads cross-compiled case
    AC_TRY_LINK(
      [
/*
 * Don't use definition of specific preprocessor macros as criterion
 * for determining if thread support is found.
 *
 * #if !defined (_REENTRANT) && !defined (_THREAD_SAFE)
 * #error Neither _REENTRANT nor _THREAD_SAFE were defined.
 * THROW ME AN ERROR!
 * #endif
 *
 */
      ]
ifelse(AC_LANG, CPLUSPLUS, [#ifdef __cplusplus
extern "C"
#endif
])
      [
char thr_create();
      ],
      [
thr_create();
      ],
      [$1],
      [$2])
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
    ifelse(AC_LANG, [CPLUSPLUS],
           [ace_save_CXXFLAGS="$CXXFLAGS"],[ace_save_CFLAGS="$CFLAGS"])

    for i in $1; do
      ifelse(AC_LANG, [CPLUSPLUS],
             [CXXFLAGS="$CXXFLAGS -$i"],[CFLAGS="$CFLAGS -$i"])

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
      ifelse(AC_LANG, [CPLUSPLUS],
             [CXXFLAGS="$ace_save_CXXFLAGS"],[CFLAGS="$ace_save_CFLAGS"])
    done

    dnl Reset the flags to a consistent state.
    dnl This prevents duplicate flags from being added to
    dnl the C/CXXFLAGS variable.
    ifelse(AC_LANG, [CPLUSPLUS],
           [CXXFLAGS="$ace_save_CXXFLAGS"],[CFLAGS="$ace_save_CFLAGS"])
   ],
   [
    dnl Add the found/cached thread flag to the C/CXXFLAGS variables
    ifelse(AC_LANG, [CPLUSPLUS],
           [CXXFLAGS="$CXXFLAGS $ace_cv_thread_flag_search"],
           [CFLAGS="$CFLAGS $ace_cv_thread_flag_search"])

    $2
   ],
   [
    $3
   ])
])


dnl Check if the compiler defines thread related preprocessor flags.
dnl If not, then provide them.
dnl Usage: ACE_CHECK_THREAD_CPPFLAGS
AC_DEFUN(ACE_CHECK_THREAD_CPPFLAGS, dnl
[
 dnl A compile-time test is used instead of a preprocessor-time test
 dnl because compiler thread flags defined in CFLAGS or CXXFLAGS
 dnl should be used for this test.
 AC_TRY_COMPILE(
   [
#if !defined (_REENTRANT) && !defined (_THREAD_SAFE)
#error Neither _REENTRANT nor _THREAD_SAFE were defined.
THROW ME AN ERROR!
#endif
   ],
   [
    int a = 0; a++;
   ],
   [
    ACE_THR_CPPFLAGS=
   ],
   [
    ACE_THR_CPPFLAGS="-D_REENTRANT -D_THREAD_SAFE"
   ])
])
