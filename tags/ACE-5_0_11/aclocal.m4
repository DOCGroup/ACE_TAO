dnl aclocal.m4 generated automatically by aclocal 1.4

dnl Copyright (C) 1994, 1995-8, 1999 Free Software Foundation, Inc.
dnl This file is free software; the Free Software Foundation
dnl gives unlimited permission to copy and/or distribute it,
dnl with or without modifications, as long as this notice is preserved.

dnl This program is distributed in the hope that it will be useful,
dnl but WITHOUT ANY WARRANTY, to the extent permitted by law; without
dnl even the implied warranty of MERCHANTABILITY or FITNESS FOR A
dnl PARTICULAR PURPOSE.

# Add --enable-maintainer-mode option to configure.
# From Jim Meyering

# serial 1

AC_DEFUN(AM_MAINTAINER_MODE,
[AC_MSG_CHECKING([whether to enable maintainer-specific portions of Makefiles])
  dnl maintainer-mode is disabled by default
  AC_ARG_ENABLE(maintainer-mode,
[  --enable-maintainer-mode enable make rules and dependencies not useful
                          (and sometimes confusing) to the casual installer],
      USE_MAINTAINER_MODE=$enableval,
      USE_MAINTAINER_MODE=no)
  AC_MSG_RESULT($USE_MAINTAINER_MODE)
  AM_CONDITIONAL(MAINTAINER_MODE, test $USE_MAINTAINER_MODE = yes)
  MAINT=$MAINTAINER_MODE_TRUE
  AC_SUBST(MAINT)dnl
]
)

# Define a conditional.

AC_DEFUN(AM_CONDITIONAL,
[AC_SUBST($1_TRUE)
AC_SUBST($1_FALSE)
if $2; then
  $1_TRUE=
  $1_FALSE='#'
else
  $1_TRUE='#'
  $1_FALSE=
fi])

dnl -------------------------------------------------------------------------
dnl       $Id$
dnl 
dnl       ACE M4 include file which contains general M4 macros
dnl       to be used by the ACE configure script.
dnl
dnl       The macros in this file were designed for ACE but should be
dnl       general enough for general use.
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


dnl miscellaneous macros

dnl Prevent the configure script continuing any further if a CVS control
dnl directory is found.  The idea is to prevent files generated during
dnl configuration and build from be checked in to the CVS repository that
dnl the sources are checked into.  This should only be an issue for
dnl maintainers, not end-users.  Maintainers should configure and build in
dnl a directory that doesn't contain any CVS controlled sources and files,
dnl i.e. that doesn't contain a CVS directory.
dnl
dnl Usage: ACE_CHECK_FOR_CVS_DIR
AC_DEFUN(ACE_CHECK_FOR_CVS_DIR,
[
 if test -d CVS; then
   AC_MSG_ERROR(
     [
      This error is meant for maintainers:

      Please configure and build in a non-CVS controlled directory.
      Doing so will prevent accidentally committing automatically
      generated files into the CVS repository and help ensure that
      the generated files and build scheme are correct.

      For example, try the following from the top-level source
      directory:

          mkdir objdir
          cd objdir
          ../configure
          make

      This will create a build space in the directory \`objdir' and
      start a build in that directory.
     ])
 fi
])


dnl Prevent the configure script from continuing any further if
dnl configuration is being performed in the top-level directory.  The
dnl idea is to prevent files generated during configuration and build
dnl from overwriting the stock files of the same name.
dnl Usage: ACE_CHECK_TOP_SRCDIR
AC_DEFUN(ACE_CHECK_TOP_SRCDIR,
[
 if test $srcdir = "." && test $USE_MAINTAINER_MODE != yes; then
   AC_MSG_ERROR(
     [
      Please configure and build in a directory other than the
      top-level source directory.  Doing so will prevent files
      distributed with the package from being overwritten.  This
      currently necessary since autoconf support is still
      experimental.  If you encounter problems please use the stock
      build procedure.

      For example, try the following from the top-level source
      directory:

          mkdir objdir
          cd objdir
          ../configure
          make

      This will create a build space in the directory \`objdir' and
      start a build in that directory.
     ])
 fi
])

dnl Add compiler flags to the CXXFLAGS and CFLAGS variables when doing an
dnl AC_TRY_COMPILE (not ACE_TRY_COMPILE).
dnl Use this macro when adding include directories to the compiler flags,
dnl for example.
dnl Usage: ACE_TRY_COMPILE(COMPILER-FLAGS, INCLUDES, FUNCTION-BODY,
dnl                        [ACTION-IF-FOUND [,ACTION-IF-NOT-FOUND]])
AC_DEFUN(ACE_TRY_COMPILE, dnl
[
 ifelse(AC_LANG, [CPLUSPLUS],
   [
    ace_pre_try_CXXFLAGS="$CXXFLAGS"
    CXXFLAGS="$CXXFLAGS $1"
   ],
   [
    ace_pre_try_CFLAGS="$CFLAGS"
    CFLAGS="$CFLAGS $1"
   ])

 AC_TRY_COMPILE([$2],[$3],[$4],[$5])

 dnl Restore the C++ and C flags
 ifelse(AC_LANG, [CPLUSPLUS],
   [CXXFLAGS="$ace_pre_try_CXXFLAGS"],[CFLAGS="$ace_pre_try_CFLAGS"])

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
 test -d ./$1 && AC_MSG_ERROR([cannot create file: $acetmp is a directory])

 test -f ${srcdir}/$1 && mv ${srcdir}/$1 ${srcdir}/$1.conf
 touch ${srcdir}/$1

 if test ${srcdir}/$1 != "./$1"; then
   test -f ./$1 && mv ./$1 ./$1.conf
   dnl Create all of the sub-directories (assume "mkdir -p" is not portable).
   acetmp="."
changequote(, )dnl
   for ace_dir in `echo $1 | sed -e 's,/[^/][^/]*\$,,' -e 's,/, ,g'`; do
changequote([, ])dnl
     acetmp="$acetmp/$ace_dir"
     test -f $acetmp && AC_MSG_ERROR([cannot create directory: $acetmp])
     test -d $acetmp || mkdir $acetmp
   done
   touch ./$1
 fi

 $2

 if test -f ${srcdir}/$1.conf; then
   mv ${srcdir}/$1.conf ${srcdir}/$1
 else
   rm ${srcdir}/$1
 fi

 if test ${srcdir}/$1 != "./$1"; then
   if test -f ./$1.conf; then
     mv ./$1.conf ./$1
   else
     dnl Remove the file.  Any sub-directories will not be removed
     dnl since we have no way to tell if they existed prior to the
     dnl creation of this file.
     rm ./$1
   fi
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
dnl Other "treat warnings as errors" flags for other compilers should
dnl be added if possible.
  ace_pre_warning_CXXFLAGS="$CXXFLAGS"

  CXXFLAGS="$CXXFLAGS $WERROR"

  $1

  CXXFLAGS="$ace_pre_warning_CXXFLAGS"
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
  if test "[$]$2" != no; then
    ace_just_a_place_holder=fixme
ifelse([$4], , :, [$4])
  else
    ace_just_a_place_holder=fixme
ifelse([$5], , , [$5
])
  fi
])

dnl   checks for programs

dnl   checks for libraries

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

  ACE_CACHE_CHECK([for $1 in $2], [ace_cv_type_$1],
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
    ],[$3],[$4])
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

  ACE_CACHE_CHECK([for struct $1 in $2], [ace_cv_struct_$1],
    [
     ACE_TRY_COMPILE_STRUCT([$1], [$2],
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
     ],
     [ace_header_exists=yes],
     [ace_header_exists=no])

  cat > conftest.$ac_ext <<EOF

#include <$2>
  ACE_REAL_FUNCTION $1

EOF

  if test "$ace_header_exists" = yes; then
    if test -z "$AWK"; then
      AC_MSG_WARN([No awk program found.  "Real" function may not be found.])
    fi

    if (eval "$ac_cpp conftest.$ac_ext") 2>&5 |
       egrep "ACE_REAL_FUNCTION" | 
       (eval "$AWK '{print \[$]2}' > conftest.awk 2>&1"); then
         rm -f conftest.$ac_ext
         ace_real_function=`cat conftest.awk`
         rm -f conftest.awk
    fi

    if test $1 != "$ace_real_function"; then
      AC_MSG_CHECKING([for real $1 from $2])
      AC_MSG_RESULT([$ace_real_function])
    fi
  else
    ace_real_function=$1
  fi dnl test "$ace_header_not_exist" != yes

  AC_CHECK_FUNC([$ace_real_function],[$3],[$4])
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


dnl Check if getrlimit() takes an enum as 1st argument
dnl Usage: ACE_CHECK_SETRLIMIT_ENUM
AC_DEFUN(ACE_CHECK_SETRLIMIT_ENUM, dnl
[
if test "$ac_cv_func_setrlimit" = yes; then
  AC_MSG_CHECKING([if setrlimit() takes an enum as 1st argument])
  AC_EGREP_HEADER([setrlimit.*\(.*[^,]*enum], sys/resource.h,
    [
     cat > conftest.$ac_ext <<EOF
#include "confdefs.h"
#include <sys/resource.h>
EOF

changequote(, )dnl
dnl Here we attempt to determine the type of the first argument of
dnl getrusage from its prototype.  It should either be an int or an
dnl enum.  If it is an enum, determine the enum type.
     ace_setrlimit_enum=`eval "$ac_cpp conftest.$ac_ext" | \
       egrep '[ ]+setrlimit.*\(.*[^,]*enum' | \
       sed -e 's/^.*setrlimit.*(.*enum//' -e 's/[^ ]*,.*$//'`
changequote([, ])dnl

     ace_setrlimit_enum="enum $ace_setrlimit_enum" 

     AC_MSG_RESULT([$ace_setrlimit_enum])

if test -n "$ace_setrlimit_enum"; then
     AC_DEFINE_UNQUOTED([ACE_HAS_RLIMIT_RESOURCE_ENUM], [$ace_setrlimit_enum])
fi

     rm -rf conftest*

dnl Do not remove this parenthesis --> )
dnl It's only purpose to keep Emacs from getting confused about mismatched
dnl parentheses.
    ],
    [
     AC_MSG_RESULT([no])
    ])

fi  dnl test "$ac_cv_func_setrlimit" = yes
])

dnl Check if getrusage() takes an enum as 1st argument
dnl Usage: ACE_CHECK_GETRUSAGE_ENUM
AC_DEFUN(ACE_CHECK_GETRUSAGE_ENUM, dnl
[
if test "$ac_cv_func_getrusage" = yes; then
  AC_MSG_CHECKING([if getrusage() takes an enum as 1st argument])
  AC_EGREP_HEADER([getrusage.*\(.*[^,]*enum], sys/resource.h,
    [
     cat > conftest.$ac_ext <<EOF
#include "confdefs.h"
#include <sys/resource.h>
EOF

changequote(, )dnl
dnl Here we attempt to determine the type of the first argument of
dnl getrusage from its prototype.  It should either be an int or an
dnl enum.  If it is an enum, determine the enum type.
     ace_rusage_who=`eval "$ac_cpp conftest.$ac_ext" | \
       egrep '[ ]+getrusage.*\(.*[^,]*enum' | \
       sed -e 's/^.*getrusage.*(.*enum//' -e 's/[^ ]*,.*$//'`
changequote([, ])dnl

     ace_rusage_who="enum $ace_rusage_who" 

     AC_MSG_RESULT([$ace_rusage_who])

if test -n "$ace_rusage_who"; then
     AC_DEFINE_UNQUOTED([ACE_HAS_RUSAGE_WHO_ENUM], [$ace_rusage_who])
fi

     rm -rf conftest*

dnl Do not remove this parenthesis --> )
dnl It's only purpose to keep Emacs from getting confused about mismatched
dnl parentheses.
    ],
    [
     AC_MSG_RESULT([no])
    ])

fi  dnl test "$ac_cv_func_getrusage" = yes
])

dnl   checks for structures

dnl   checks for system services


dnl *********************** SPECIAL SECTION *******************************
dnl
dnl This section contains my own *re*implementation of the functionality
dnl provided by some tests/macros found in GNU Autoconf since the ones found
dnl in Autoconf don't appear to work as expected.
dnl
dnl                -Ossama Othman <ossama@debian.org>
dnl
dnl The copyright for the following macros is listed below.
dnl Note that all macros listed prior to this section are copyrighted
dnl by Ossama Othman, not the Free Software Foundation.  Nevertheless,
dnl all software found in this file is free software.  Please read the
dnl distribution terms found at the top of this file and the ones below.

dnl Parameterized macros.
dnl Requires GNU m4.
dnl This file is part of Autoconf.
dnl Copyright (C) 1992, 93, 94, 95, 96, 1998 Free Software Foundation, Inc.
dnl
dnl This program is free software; you can redistribute it and/or modify
dnl it under the terms of the GNU General Public License as published by
dnl the Free Software Foundation; either version 2, or (at your option)
dnl any later version.
dnl
dnl This program is distributed in the hope that it will be useful,
dnl but WITHOUT ANY WARRANTY; without even the implied warranty of
dnl MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
dnl GNU General Public License for more details.
dnl
dnl You should have received a copy of the GNU General Public License
dnl along with this program; if not, write to the Free Software
dnl Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA
dnl 02111-1307, USA.
dnl
dnl As a special exception, the Free Software Foundation gives unlimited
dnl permission to copy, distribute and modify the configure scripts that
dnl are the output of Autoconf.  You need not follow the terms of the GNU
dnl General Public License when using or distributing such scripts, even
dnl though portions of the text of Autoconf appear in them.  The GNU
dnl General Public License (GPL) does govern all other use of the material
dnl that constitutes the Autoconf program.
dnl
dnl Certain portions of the Autoconf source text are designed to be copied
dnl (in certain cases, depending on the input) into the output of
dnl Autoconf.  We call these the "data" portions.  The rest of the Autoconf
dnl source text consists of comments plus executable code that decides which
dnl of the data portions to output in any given case.  We call these
dnl comments and executable code the "non-data" portions.  Autoconf never
dnl copies any of the non-data portions into its output.
dnl
dnl This special exception to the GPL applies to versions of Autoconf
dnl released by the Free Software Foundation.  When you make and
dnl distribute a modified version of Autoconf, you may extend this special
dnl exception to the GPL to apply to your modified version as well, *unless*
dnl your modified version has the potential to copy into its output some
dnl of the text that was the non-data portion of the version that you started
dnl with.  (In other words, unless your change moves or copies text from
dnl the non-data portions to the data portions.)  If your modification has
dnl such potential, you must delete any notice of this special exception
dnl to the GPL from your modified version.
dnl
dnl Written by David MacKenzie, with help from
dnl Franc,ois Pinard, Karl Berry, Richard Pixley, Ian Lance Taylor,
dnl Roland McGrath, Noah Friedman, david d zuhn, and many others.


dnl Usage: ACE_SEARCH_LIBS(FUNCTION, SEARCH-LIBS [, ACTION-IF-FOUND
dnl                        [, ACTION-IF-NOT-FOUND [, OTHER-LIBRARIES]]])
dnl Search for a library defining FUNCTION, if it's not already available.
AC_DEFUN(ACE_SEARCH_LIBS, dnl
[
 AC_CACHE_CHECK([for library containing $1], [ac_cv_search_$1],
   [
    ac_func_search_save_LIBS="$LIBS"

    ac_cv_search_$1="no"

    ACE_TRY_LINK_FUNC([$1], [ac_cv_search_$1="none required"])

    test "$ac_cv_search_$1" = "no" && for i in $2; do
      LIBS="-l$i $5 $ac_func_search_save_LIBS"
      ACE_TRY_LINK_FUNC([$1],
        [
         ac_cv_search_$1="-l$i"
         break
        ])
    done

    LIBS="$ac_func_search_save_LIBS"
   ])

  if test "$ac_cv_search_$1" != "no"; then
    test "$ac_cv_search_$1" = "none required" || LIBS="$ac_cv_search_$1 $LIBS"
    $3
  else :
    $4
  fi
])

dnl Usage: ACE_TRY_LINK_FUNC(FUNCTION,[, ACTION-IF-FOUND
dnl                          [, ACTION-IF-NOT-FOUND])
dnl Search for a library defining FUNCTION, if it's not already available.
AC_DEFUN(ACE_TRY_LINK_FUNC, dnl
[
AC_TRY_LINK(
dnl Don't include <ctype.h> because on OSF/1 3.0 it includes <sys/types.h>
dnl which includes <sys/select.h> which contains a prototype for
dnl select.  Similarly for bzero.
[/* System header to define __stub macros and hopefully few prototypes,
    which can conflict with char $1(); below.  */
#include <assert.h>
/* Override any gcc2 internal prototype to avoid an error.  */
]ifelse(AC_LANG, CPLUSPLUS, [#ifdef __cplusplus
extern "C"
#endif
])dnl
[/* We use char because int might match the return type of a gcc2
    builtin and then its argument prototype would still apply.  */
char $1();
], [
/* The GNU C library defines this for functions which it implements
    to always fail with ENOSYS.  Some functions are actually named
    something starting with __ and the normal name is an alias.  */
#if defined (__stub_$1) || defined (__stub___$1)
choke me
#else
$1();
#endif
],[$2],[$3])
])

AC_DEFUN(ACE_SYS_RESTARTABLE_SYSCALLS,
[AC_REQUIRE([AC_HEADER_SYS_WAIT])
AC_CHECK_HEADERS(unistd.h)
AC_CACHE_CHECK(for restartable system calls, ac_cv_sys_restartable_syscalls,
[AC_TRY_RUN(
[/* Exit 0 (true) if wait returns something other than -1,
   i.e. the pid of the child, which means that wait was restarted
   after getting the signal.  */
#include <sys/types.h>
#include <signal.h>
#if HAVE_UNISTD_H
# include <unistd.h>
#endif
#if HAVE_SYS_WAIT_H
# include <sys/wait.h>
#endif

/* Some platforms explicitly require an extern "C" signal handler
   when using C++. */
#ifdef __cplusplus
extern "C"
#endif
void ucatch (int) { }

main () {
  int i = fork (), status;
  if (i == 0) { sleep (3); kill (getppid (), SIGINT); sleep (3); exit (0); }
  signal (SIGINT, ucatch);
  status = wait(&i);
  if (status == -1) wait(&i);
  exit (status == -1);
}
], ac_cv_sys_restartable_syscalls=yes, ac_cv_sys_restartable_syscalls=no)])
if test $ac_cv_sys_restartable_syscalls = yes; then
  AC_DEFINE(HAVE_RESTARTABLE_SYSCALLS)
fi
])


# Do all the work for Automake.  This macro actually does too much --
# some checks are only needed if your package does certain things.
# But this isn't really a big deal.

# serial 1

dnl Usage:
dnl AM_INIT_AUTOMAKE(package,version, [no-define])

AC_DEFUN(AM_INIT_AUTOMAKE,
[AC_REQUIRE([AC_PROG_INSTALL])
PACKAGE=[$1]
AC_SUBST(PACKAGE)
VERSION=[$2]
AC_SUBST(VERSION)
dnl test to see if srcdir already configured
if test "`cd $srcdir && pwd`" != "`pwd`" && test -f $srcdir/config.status; then
  AC_MSG_ERROR([source directory already configured; run "make distclean" there first])
fi
ifelse([$3],,
AC_DEFINE_UNQUOTED(PACKAGE, "$PACKAGE", [Name of package])
AC_DEFINE_UNQUOTED(VERSION, "$VERSION", [Version number of package]))
AC_REQUIRE([AM_SANITY_CHECK])
AC_REQUIRE([AC_ARG_PROGRAM])
dnl FIXME This is truly gross.
missing_dir=`cd $ac_aux_dir && pwd`
AM_MISSING_PROG(ACLOCAL, aclocal, $missing_dir)
AM_MISSING_PROG(AUTOCONF, autoconf, $missing_dir)
AM_MISSING_PROG(AUTOMAKE, automake, $missing_dir)
AM_MISSING_PROG(AUTOHEADER, autoheader, $missing_dir)
AM_MISSING_PROG(MAKEINFO, makeinfo, $missing_dir)
AC_REQUIRE([AC_PROG_MAKE_SET])])

#
# Check to make sure that the build environment is sane.
#

AC_DEFUN(AM_SANITY_CHECK,
[AC_MSG_CHECKING([whether build environment is sane])
# Just in case
sleep 1
echo timestamp > conftestfile
# Do `set' in a subshell so we don't clobber the current shell's
# arguments.  Must try -L first in case configure is actually a
# symlink; some systems play weird games with the mod time of symlinks
# (eg FreeBSD returns the mod time of the symlink's containing
# directory).
if (
   set X `ls -Lt $srcdir/configure conftestfile 2> /dev/null`
   if test "[$]*" = "X"; then
      # -L didn't work.
      set X `ls -t $srcdir/configure conftestfile`
   fi
   if test "[$]*" != "X $srcdir/configure conftestfile" \
      && test "[$]*" != "X conftestfile $srcdir/configure"; then

      # If neither matched, then we have a broken ls.  This can happen
      # if, for instance, CONFIG_SHELL is bash and it inherits a
      # broken ls alias from the environment.  This has actually
      # happened.  Such a system could not be considered "sane".
      AC_MSG_ERROR([ls -t appears to fail.  Make sure there is not a broken
alias in your environment])
   fi

   test "[$]2" = conftestfile
   )
then
   # Ok.
   :
else
   AC_MSG_ERROR([newly created file is older than distributed files!
Check your system clock])
fi
rm -f conftest*
AC_MSG_RESULT(yes)])

dnl AM_MISSING_PROG(NAME, PROGRAM, DIRECTORY)
dnl The program must properly implement --version.
AC_DEFUN(AM_MISSING_PROG,
[AC_MSG_CHECKING(for working $2)
# Run test in a subshell; some versions of sh will print an error if
# an executable is not found, even if stderr is redirected.
# Redirect stdin to placate older versions of autoconf.  Sigh.
if ($2 --version) < /dev/null > /dev/null 2>&1; then
   $1=$2
   AC_MSG_RESULT(found)
else
   $1="$3/missing $2"
   AC_MSG_RESULT(missing)
fi
AC_SUBST($1)])

# Like AC_CONFIG_HEADER, but automatically create stamp file.

AC_DEFUN(AM_CONFIG_HEADER,
[AC_PREREQ([2.12])
AC_CONFIG_HEADER([$1])
dnl When config.status generates a header, we must update the stamp-h file.
dnl This file resides in the same directory as the config header
dnl that is generated.  We must strip everything past the first ":",
dnl and everything past the last "/".
AC_OUTPUT_COMMANDS(changequote(<<,>>)dnl
ifelse(patsubst(<<$1>>, <<[^ ]>>, <<>>), <<>>,
<<test -z "<<$>>CONFIG_HEADERS" || echo timestamp > patsubst(<<$1>>, <<^\([^:]*/\)?.*>>, <<\1>>)stamp-h<<>>dnl>>,
<<am_indx=1
for am_file in <<$1>>; do
  case " <<$>>CONFIG_HEADERS " in
  *" <<$>>am_file "*<<)>>
    echo timestamp > `echo <<$>>am_file | sed -e 's%:.*%%' -e 's%[^/]*$%%'`stamp-h$am_indx
    ;;
  esac
  am_indx=`expr "<<$>>am_indx" + 1`
done<<>>dnl>>)
changequote([,]))])


dnl AM_PROG_LEX
dnl Look for flex, lex or missing, then run AC_PROG_LEX and AC_DECL_YYTEXT
AC_DEFUN(AM_PROG_LEX,
[missing_dir=ifelse([$1],,`cd $ac_aux_dir && pwd`,$1)
AC_CHECK_PROGS(LEX, flex lex, "$missing_dir/missing flex")
AC_PROG_LEX
AC_DECL_YYTEXT])

dnl -------------------------------------------------------------------------
dnl       $Id$
dnl 
dnl       subsets.m4
dnl
dnl       ACE M4 include file which contains ACE specific M4 macros
dnl       that set/determine which ACE subsets to build.
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


dnl Check for thread related libraries and compiler flags
dnl Usage: ACE_CHECK_SUBSETS
AC_DEFUN(ACE_CHECK_SUBSETS, dnl
[

dnl Begin ACE_CHECK_SUBSETS

dnl Assume all subsets will be built, including the full ACE library.
dnl If any of the components is explicitly enabled or disabled by the user
dnl then do NOT build the full ACE library.
AC_ARG_ENABLE(lib-all,
              [  --enable-lib-all        build all ACE components        [default=no]],
              [
               case "${enableval}" in
                yes)
                  ace_user_enable_lib_all=yes
                  ;;
                no)
                  ace_user_enable_lib_all=no
                  ;;
                *)
                  AC_MSG_ERROR(bad value ${enableval} for --enable-lib-all)
                  ;;
               esac
              ],
              [
               ace_user_enable_lib_all=no
              ])

AC_ARG_ENABLE(lib-full,
              [  --enable-lib-full       build the full ACE library      [default=yes]],
              [
               case "${enableval}" in
                yes)
                  ace_user_enable_lib_full=yes
                  ;;
                no)
                  ace_user_enable_lib_full=no
                  ;;
                *)
                  AC_MSG_ERROR(bad value ${enableval} for --enable-lib-full)
                  ;;
               esac
              ],
              [
               ace_user_enable_lib_full=yes
              ])

AC_ARG_ENABLE(lib-os,
              [  --enable-lib-os         build libACE_OS library         ],
              [
               case "${enableval}" in
                yes)
                  ACE_CREATE_LIBACE_OS
                  ;;
                no)
                  ace_user_enable_lib_os=no
                  ;;
                *)
                  AC_MSG_ERROR(bad value ${enableval} for --enable-lib-os)
                  ;;
               esac

               dnl Disable full ACE library build
               ace_user_enable_lib_full=no
              ],)

AC_ARG_ENABLE(lib-utils,
              [  --enable-lib-utils      build libACE_Utils library      ],
              [
               case "${enableval}" in
                yes)
                  ACE_CREATE_LIBACE_UTILS
                  ;;
                no)
                  ace_user_enable_lib_utils=no
                  ;;
                *)
                  AC_MSG_ERROR(bad value ${enableval} for --enable-lib-utils)
                  ;;
               esac

               dnl Disable full ACE library build
               ace_user_enable_lib_full=no
              ],)

AC_ARG_ENABLE(lib-logging,
              [  --enable-lib-logging    build libACE_Logging library    ],
              [
               case "${enableval}" in
                yes)
                  ACE_CREATE_LIBACE_LOGGING
                  ;;
                no)
                  ace_user_enable_lib_logging=no
                  ;;
                *)
                  AC_MSG_ERROR(bad value ${enableval} for --enable-lib-logging)
                  ;;
               esac

               dnl Disable full ACE library build
               ace_user_enable_lib_full=no
              ],)

AC_ARG_ENABLE(lib-threads,
              [  --enable-lib-threads    build libACE_Threads library    ],
              [
               case "${enableval}" in
                yes)
                  ACE_CREATE_LIBACE_THREADS
                  ;;
                no)
                  ace_user_enable_lib_threads=no
                  ;;
                *)
                  AC_MSG_ERROR(bad value ${enableval} for --enable-lib-threads)
                  ;;
               esac

               dnl Disable full ACE library build
               ace_user_enable_lib_full=no
              ],)

AC_ARG_ENABLE(lib-demux,
              [  --enable-lib-demux      build libACE_Demux library      ],
              [
               case "${enableval}" in
                yes)
                  ACE_CREATE_LIBACE_DEMUX
                  ;;
                no)
                  ace_user_enable_lib_demux=no
                  ;;
                *)
                  AC_MSG_ERROR(bad value ${enableval} for --enable-lib-demux)
                  ;;
               esac

               dnl Disable full ACE library build
               ace_user_enable_lib_full=no
              ],)

AC_ARG_ENABLE(lib-connection,
              [  --enable-lib-connection build libACE_Connection library ],
              [
               case "${enableval}" in
                yes)
                  ACE_CREATE_LIBACE_CONNECTION
                  ;;
                no)
                  ace_user_enable_lib_connection=no
                  ;;
                *)
                  AC_MSG_ERROR(bad value ${enableval} for --enable-lib-connection)
                  ;;
               esac

               dnl Disable full ACE library build
               ace_user_enable_lib_full=no
              ],)

AC_ARG_ENABLE(lib-sockets,
              [  --enable-lib-sockets    build libACE_Sockets library    ],
              [
               case "${enableval}" in
                yes)
                  ACE_CREATE_LIBACE_SOCKETS
                  ;;
                no)
                  ace_user_enable_lib_sockets=no
                  ;;
                *)
                  AC_MSG_ERROR(bad value ${enableval} for --enable-lib-sockets)
                  ;;
               esac

               dnl Disable full ACE library build
               ace_user_enable_lib_full=no
              ],)

AC_ARG_ENABLE(lib-ipc,
              [  --enable-lib-ipc        build libACE_IPC library        ],
              [
               case "${enableval}" in
                yes)
                  ACE_CREATE_LIBACE_IPC
                  ;;
                no)
                  ace_user_enable_lib_ipc=no
                  ;;
                *)
                  AC_MSG_ERROR(bad value ${enableval} for --enable-lib-ipc)
                  ;;
               esac

               dnl Disable full ACE library build
               ace_user_enable_lib_full=no
              ],)

AC_ARG_ENABLE(lib-svcconf,
              [  --enable-lib-svcconf    build libACE_Svcconf library    ],
              [
               case "${enableval}" in
                yes)
                  ACE_CREATE_LIBACE_SVCCONF
                  ;;
                no)
                  ace_user_enable_lib_svcconf=no
                  ;;
                *)
                  AC_MSG_ERROR(bad value ${enableval} for --enable-lib-svcconf)
                  ;;
               esac

               dnl Disable full ACE library build
               ace_user_enable_lib_full=no
              ],)

AC_ARG_ENABLE(lib-streams,
              [  --enable-lib-streams    build libACE_Streams library    ],
              [
               case "${enableval}" in
                yes)
                  ACE_CREATE_LIBACE_STREAMS
                  ;;
                no)
                  ace_user_enable_lib_streams=no
                  ;;
                *)
                  AC_MSG_ERROR(bad value ${enableval} for --enable-lib-streams)
                  ;;
               esac

               dnl Disable full ACE library build
               ace_user_enable_lib_full=no
              ],)

AC_ARG_ENABLE(lib-memory,
              [  --enable-lib-memory     build libACE_Memory library     ],
              [
               case "${enableval}" in
                yes)
                  ACE_CREATE_LIBACE_MEMORY
                  ;;
                no)
                  ace_user_enable_lib_memory=no
                  ;;
                *)
                  AC_MSG_ERROR(bad value ${enableval} for --enable-lib-memory)
                  ;;
               esac

               dnl Disable full ACE library build
               ace_user_enable_lib_full=no
              ],)

AC_ARG_ENABLE(lib-token,
              [  --enable-lib-token      build libACE_Token library      ],
              [
               case "${enableval}" in
                yes)
                  ACE_CREATE_LIBACE_TOKEN
                  ;;
                no)
                  ace_user_enable_lib_token=no
                  ;;
                *)
                  AC_MSG_ERROR(bad value ${enableval} for --enable-lib-token)
                  ;;
               esac

               dnl Disable full ACE library build
               ace_user_enable_lib_full=no
              ],)

AC_ARG_ENABLE(lib-other,
              [  --enable-lib-other      build libACE_Other library      ],
              [
               case "${enableval}" in
                yes)
                  ACE_CREATE_LIBACE_OTHER
                  ;;
                no)
                  ace_user_enable_lib_other=no
                  ;;
                *)
                  AC_MSG_ERROR(bad value ${enableval} for --enable-lib-other)
                  ;;
               esac

               dnl Disable full ACE library build
               ace_user_enable_lib_full=no
              ],)


dnl If no ACE subsets were explicitly enabled or disabled then build
dnl all of them.
if test $ace_user_enable_lib_all = yes; then

  ACE_CREATE_ALL_COMPONENTS

elif test $ace_user_enable_lib_all = no; then

  ACE_DISABLE_ALL_COMPONENTS

fi

if test $ace_user_enable_lib_full = no &&
   test $ace_user_enable_lib_os = no &&
   test $ace_user_enable_lib_utils = no &&
   test $ace_user_enable_lib_logging = no &&
   test $ace_user_enable_lib_threads = no &&
   test $ace_user_enable_lib_demux = no &&
   test $ace_user_enable_lib_connection = no &&
   test $ace_user_enable_lib_sockets = no &&
   test $ace_user_enable_lib_ipc = no &&
   test $ace_user_enable_lib_svcconf = no &&
   test $ace_user_enable_lib_streams = no &&
   test $ace_user_enable_lib_memory = no &&
   test $ace_user_enable_lib_token = no &&
   test $ace_user_enable_lib_other = no; then

  dnl If we get here then no ACE libraries will be built!
  AC_MSG_ERROR(no ACE components will be built.  Specify which components to build)

fi  dnl No components will be built!

dnl Set which ACE subsets to build
AM_CONDITIONAL(BUILD_OS_FILES,
               test X$ace_user_enable_lib_os = Xyes)

AM_CONDITIONAL(BUILD_UTILS_FILES,
               test X$ace_user_enable_lib_utils = Xyes)

AM_CONDITIONAL(BUILD_LOGGING_FILES,
               test X$ace_user_enable_lib_logging = Xyes)

AM_CONDITIONAL(BUILD_THREADS_FILES,
               test X$ace_user_enable_lib_threads = Xyes)

AM_CONDITIONAL(BUILD_DEMUX_FILES,
               test X$ace_user_enable_lib_demux = Xyes)

AM_CONDITIONAL(BUILD_CONNECTION_FILES,
               test X$ace_user_enable_lib_connection = Xyes)

AM_CONDITIONAL(BUILD_SOCKETS_FILES,
               test X$ace_user_enable_lib_sockets = Xyes)

AM_CONDITIONAL(BUILD_IPC_FILES,
               test X$ace_user_enable_lib_ipc = Xyes)

AM_CONDITIONAL(BUILD_SVCCONF_FILES,
               test X$ace_user_enable_lib_svcconf = Xyes)

AM_CONDITIONAL(BUILD_STREAMS_FILES,
               test X$ace_user_enable_lib_streams = Xyes)

AM_CONDITIONAL(BUILD_MEMORY_FILES,
               test X$ace_user_enable_lib_memory = Xyes)

AM_CONDITIONAL(BUILD_TOKEN_FILES,
               test X$ace_user_enable_lib_token = Xyes)

AM_CONDITIONAL(BUILD_OTHER_FILES,
               test X$ace_user_enable_lib_other = Xyes)

AM_CONDITIONAL(BUILD_FULL_LIBRARY,
               test X$ace_user_enable_lib_full = Xyes)

dnl End ACE_CHECK_SUBSETS
])

dnl Set the component dependencies for the libACE_OS library
dnl Usage: ACE_CREATE_LIBACE_OS
AC_DEFUN(ACE_CREATE_LIBACE_OS,
[
 ace_user_enable_lib_os=yes
])

dnl Set the component dependencies for the libACE_Utils library
dnl Usage: ACE_CREATE_LIBACE_UTILS
AC_DEFUN(ACE_CREATE_LIBACE_UTILS,
[
 ace_user_enable_lib_utils=yes

 dnl Be careful not to go into a circular/recursive loop with these macros!
 ACE_CREATE_LIBACE_OS
])

dnl Set the component dependencies for the libACE_Logging library
dnl Usage: ACE_CREATE_LIBACE_Logging
AC_DEFUN(ACE_CREATE_LIBACE_LOGGING,
[
 ace_user_enable_lib_logging=yes

 dnl Be careful not to go into a circular/recursive loop with these macros!
 ACE_CREATE_LIBACE_OS
])

dnl Set the component dependencies for the libACE_Threads library
dnl Usage: ACE_CREATE_LIBACE_THREADS
AC_DEFUN(ACE_CREATE_LIBACE_THREADS,
[
 ace_user_enable_lib_threads=yes

 dnl Be careful not to go into a circular/recursive loop with these macros!
 ACE_CREATE_LIBACE_OS
])

dnl Set the component dependencies for the libACE_Demux library
dnl Usage: ACE_CREATE_LIBACE_DEMUX
AC_DEFUN(ACE_CREATE_LIBACE_DEMUX,
[
 ace_user_enable_lib_demux=yes

 dnl Be careful not to go into a circular/recursive loop with these macros!
 ACE_CREATE_LIBACE_OS
 ACE_CREATE_LIBACE_THREADS
])

dnl Set the component dependencies for the libACE_Connection library
dnl Usage: ACE_CREATE_LIBACE_CONNECTION
AC_DEFUN(ACE_CREATE_LIBACE_CONNECTION,
[
 ace_user_enable_lib_connection=yes

 dnl Be careful not to go into a circular/recursive loop with these macros!
 ACE_CREATE_LIBACE_OS
 ACE_CREATE_LIBACE_THREADS
 ACE_CREATE_LIBACE_DEMUX
])

dnl Set the component dependencies for the libACE_Sockets library
dnl Usage: ACE_CREATE_LIBACE_SOCKETS
AC_DEFUN(ACE_CREATE_LIBACE_SOCKETS,
[
 ace_user_enable_lib_sockets=yes

 dnl Be careful not to go into a circular/recursive loop with these macros!
 ACE_CREATE_LIBACE_OS
])

dnl Set the component dependencies for the libACE_IPC library
dnl Usage: ACE_CREATE_LIBACE_IPC
AC_DEFUN(ACE_CREATE_LIBACE_IPC,
[
 ace_user_enable_lib_ipc=yes

 dnl Be careful not to go into a circular/recursive loop with these macros!
 ACE_CREATE_LIBACE_OS
 ACE_CREATE_LIBACE_SOCKETS
])

dnl Set the component dependencies for the libACE_Svcconf library
dnl Usage: ACE_CREATE_LIBACE_SVCCONF
AC_DEFUN(ACE_CREATE_LIBACE_SVCCONF,
[
 ace_user_enable_lib_svcconf=yes

 dnl Be careful not to go into a circular/recursive loop with these macros!
 ACE_CREATE_LIBACE_OS
 ACE_CREATE_LIBACE_THREADS
 ACE_CREATE_LIBACE_DEMUX
 ACE_CREATE_LIBACE_SOCKETS
])

dnl Set the component dependencies for the libACE_Streams library
dnl Usage: ACE_CREATE_LIBACE_STREAMS
AC_DEFUN(ACE_CREATE_LIBACE_STREAMS,
[
 ace_user_enable_lib_streams=yes

 dnl Be careful not to go into a circular/recursive loop with these macros!
 ACE_CREATE_LIBACE_OS
 ACE_CREATE_LIBACE_THREADS
 ACE_CREATE_LIBACE_DEMUX
])

dnl Set the component dependencies for the libACE_Memory library
dnl Usage: ACE_CREATE_LIBACE_MEMORY
AC_DEFUN(ACE_CREATE_LIBACE_MEMORY,
[
 ace_user_enable_lib_memory=yes

 dnl Be careful not to go into a circular/recursive loop with these macros!
 ACE_CREATE_LIBACE_OS
])

dnl Set the component dependencies for the libACE_Token library
dnl Usage: ACE_CREATE_LIBACE_TOKEN
AC_DEFUN(ACE_CREATE_LIBACE_TOKEN,
[
 ace_user_enable_lib_token=yes

 dnl Be careful not to go into a circular/recursive loop with these macros!
 ACE_CREATE_LIBACE_OS
 ACE_CREATE_LIBACE_UTILS
 ACE_CREATE_LIBACE_LOGGING
 ACE_CREATE_LIBACE_THREADS
 ACE_CREATE_LIBACE_DEMUX
 ACE_CREATE_LIBACE_CONNECTION
 ACE_CREATE_LIBACE_SOCKETS
 ACE_CREATE_LIBACE_IPC
 ACE_CREATE_LIBACE_SVCCONF
 ACE_CREATE_LIBACE_STREAMS
 ACE_CREATE_LIBACE_MEMORY
 dnl ACE_CREATE_LIBACE_OTHER
])

dnl Set the component dependencies for the libACE_Utils library
dnl Usage: ACE_CREATE_LIBACE_OTHER
AC_DEFUN(ACE_CREATE_LIBACE_OTHER,
[
 ace_user_enable_lib_other=yes

 dnl Be careful not to go into a circular/recursive loop with these macros!
 ACE_CREATE_LIBACE_OS
 ACE_CREATE_LIBACE_UTILS
 ACE_CREATE_LIBACE_LOGGING
 ACE_CREATE_LIBACE_THREADS
 ACE_CREATE_LIBACE_DEMUX
 ACE_CREATE_LIBACE_CONNECTION
 ACE_CREATE_LIBACE_SOCKETS
 ACE_CREATE_LIBACE_IPC
 ACE_CREATE_LIBACE_SVCCONF
 ACE_CREATE_LIBACE_STREAMS
 ACE_CREATE_LIBACE_MEMORY
 dnl ACE_CREATE_LIBACE_TOKEN
])

dnl Build all ACE component libraries
dnl Usage: ACE_CREATE_ALL_COMPONENTS
AC_DEFUN(ACE_CREATE_ALL_COMPONENTS,
[
 ace_user_enable_lib_os=yes
 ace_user_enable_lib_utils=yes
 ace_user_enable_lib_logging=yes
 ace_user_enable_lib_threads=yes
 ace_user_enable_lib_demux=yes
 ace_user_enable_lib_connection=yes
 ace_user_enable_lib_sockets=yes
 ace_user_enable_lib_ipc=yes
 ace_user_enable_lib_svcconf=yes
 ace_user_enable_lib_streams=yes
 ace_user_enable_lib_memory=yes
 ace_user_enable_lib_token=yes
 ace_user_enable_lib_other=yes
])

dnl Disable all ACE component libraries
dnl Usage: ACE_CREATE_ALL_COMPONENTS
AC_DEFUN(ACE_DISABLE_ALL_COMPONENTS,
[
 ace_user_enable_lib_os=no
 ace_user_enable_lib_utils=no
 ace_user_enable_lib_logging=no
 ace_user_enable_lib_threads=no
 ace_user_enable_lib_demux=no
 ace_user_enable_lib_connection=no
 ace_user_enable_lib_sockets=no
 ace_user_enable_lib_ipc=no
 ace_user_enable_lib_svcconf=no
 ace_user_enable_lib_streams=no
 ace_user_enable_lib_memory=no
 ace_user_enable_lib_token=no
 ace_user_enable_lib_other=no
])

dnl -------------------------------------------------------------------------
dnl       $Id$
dnl 
dnl       compiler.m4
dnl
dnl       ACE M4 include file which contains ACE specific M4 macros
dnl       that set/determine compiler configurations for ACE.
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
 dnl    WERROR    - Compiler flag that converts warnings to errors

 if test -n "$GXX"; then
    WERROR="-Werror"
 fi

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
   *irix5*)
     case "$CXX" in
       CC)
         CXXFLAGS="$CXXFLAGS -ptused -prelink +pp -woff 3203,3209,3161,3262,3665"
         ACE_CXXFLAGS="$ACE_CXXFLAGS "
         DCXXFLAGS="-g"
         OCXXFLAGS=""
         ;;
       *)
         ;;
     esac
     ;;
   *irix6*)
     case "$CXX" in
       CC)
         CPPFLAGS="$CPPFLAGS -D_SGI_MP_SOURCE"
         CXXFLAGS="$CXXFLAGS -exceptions -ptnone -no_prelink -Wl,-woff,15 -Wl,-woff,84 -Wl,-woff,85 -Wl,-woff,133"
         ACE_CXXFLAGS="$ACE_CXXFLAGS "
         DCXXFLAGS="-g"
         OCXXFLAGS="-O -OPT:Olimit=0"
       ;;
     esac
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
     case "$CXX" in
       CC)
         WERROR="-xwe"

         if test "$ace_user_enable_exceptions" != yes; then
           CXXFLAGS="$CXXFLAGS -noex"
         fi

         dnl Some flags only work with Sun C++ 4.2
         if (CC -V 2>&1 | egrep 'Compilers 4\.2' > /dev/null); then
           CXXFLAGS="$CXXFLAGS -features=castop"
           if test "$ace_user_enable_rtti" = yes; then
             CXXFLAGS="$CXXFLAGS -features=rtti"
           fi 
         fi

         dnl Sun C++ 5.0 weirdness
         if (CC -V 2>&1 | egrep 'Compilers 5\.0' > /dev/null); then
           CXXFLAGS="$CXXFLAGS -library=iostream,no%Cstd -instances=explicit"

           if test "$ace_user_enable_exceptions" != yes; then
             dnl See /opt/SUNWspro_5.0/SC5.0/include/CC/stdcomp.h.
             AC_DEFINE(_RWSTD_NO_EXCEPTIONS)
           fi
         fi

         CXXFLAGS="$CXXFLAGS"
         ACE_CXXFLAGS="$ACE_CXXFLAGS"
         DCXXFLAGS="$DCXXFLAGS -g"
         OCXXFLAGS="$OCXXFLAGS -O"
         LDFLAGS="$LDFLAGS -xildoff"
         ;;
     esac
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


# serial 42 AC_PROG_LIBTOOL
AC_DEFUN(AC_PROG_LIBTOOL,
[AC_REQUIRE([AC_LIBTOOL_SETUP])dnl

# Save cache, so that ltconfig can load it
AC_CACHE_SAVE

# Actually configure libtool.  ac_aux_dir is where install-sh is found.
AR="$AR" LTCC="$CC" CC="$CC" CFLAGS="$CFLAGS" CPPFLAGS="$CPPFLAGS" \
FILE="$FILE" LD="$LD" LDFLAGS="$LDFLAGS" LIBS="$LIBS" \
LN_S="$LN_S" NM="$NM" RANLIB="$RANLIB" STRIP="$STRIP" \
AS="$AS" DLLTOOL="$DLLTOOL" OBJDUMP="$OBJDUMP" \
objext="$OBJEXT" exeext="$EXEEXT" reload_flag="$reload_flag" \
deplibs_check_method="$deplibs_check_method" file_magic_cmd="$file_magic_cmd" \
${CONFIG_SHELL-/bin/sh} $ac_aux_dir/ltconfig --no-reexec \
$libtool_flags --no-verify --build="$build" $ac_aux_dir/ltmain.sh $lt_target \
|| AC_MSG_ERROR([libtool configure failed])

# Reload cache, that may have been modified by ltconfig
AC_CACHE_LOAD

# This can be used to rebuild libtool when needed
LIBTOOL_DEPS="$ac_aux_dir/ltconfig $ac_aux_dir/ltmain.sh"

# Always use our own libtool.
LIBTOOL='$(SHELL) $(top_builddir)/libtool'
AC_SUBST(LIBTOOL)dnl

# Redirect the config.log output again, so that the ltconfig log is not
# clobbered by the next message.
exec 5>>./config.log
])

AC_DEFUN(AC_LIBTOOL_SETUP,
[AC_PREREQ(2.13)dnl
AC_REQUIRE([AC_ENABLE_SHARED])dnl
AC_REQUIRE([AC_ENABLE_STATIC])dnl
AC_REQUIRE([AC_ENABLE_FAST_INSTALL])dnl
AC_REQUIRE([AC_CANONICAL_HOST])dnl
AC_REQUIRE([AC_CANONICAL_BUILD])dnl
AC_REQUIRE([AC_PROG_CC])dnl
AC_REQUIRE([AC_PROG_LD])dnl
AC_REQUIRE([AC_PROG_LD_RELOAD_FLAG])dnl
AC_REQUIRE([AC_PROG_NM])dnl
AC_REQUIRE([AC_PROG_LN_S])dnl
AC_REQUIRE([AC_DEPLIBS_CHECK_METHOD])dnl
# Autoconf's AC_OBJEXT and AC_EXEEXT macros only works for C compilers!
AC_REQUIRE([AC_LANG_SAVE])dnl
AC_REQUIRE([AC_LANG_C])dnl
AC_REQUIRE([AC_OBJEXT])dnl
AC_REQUIRE([AC_EXEEXT])dnl
AC_REQUIRE([AC_LANG_RESTORE])dnl
dnl

# Only perform the check for file, if the check method requires it
case "$deplibs_check_method" in
file_magic*)
  if test "$file_magic_cmd" = '$FILE'; then
    AC_PATH_FILE
  fi
  ;;
esac

case "$target" in
NONE) lt_target="$host" ;;
*) lt_target="$target" ;;
esac

AC_CHECK_TOOL(RANLIB, ranlib, :)
AC_CHECK_TOOL(STRIP, strip, :)

# Check for any special flags to pass to ltconfig.
libtool_flags="--cache-file=$cache_file"
test "$enable_shared" = no && libtool_flags="$libtool_flags --disable-shared"
test "$enable_static" = no && libtool_flags="$libtool_flags --disable-static"
test "$enable_fast_install" = no && libtool_flags="$libtool_flags --disable-fast-install"
test "$ac_cv_prog_gcc" = yes && libtool_flags="$libtool_flags --with-gcc"
test "$ac_cv_prog_gnu_ld" = yes && libtool_flags="$libtool_flags --with-gnu-ld"
ifdef([AC_PROVIDE_AC_LIBTOOL_DLOPEN],
[libtool_flags="$libtool_flags --enable-dlopen"])
ifdef([AC_PROVIDE_AC_LIBTOOL_WIN32_DLL],
[libtool_flags="$libtool_flags --enable-win32-dll"])
AC_ARG_ENABLE(libtool-lock,
  [  --disable-libtool-lock  avoid locking (might break parallel builds)])
test "x$enable_libtool_lock" = xno && libtool_flags="$libtool_flags --disable-lock"
test x"$silent" = xyes && libtool_flags="$libtool_flags --silent"

AC_ARG_WITH(pic,
  [  --with-pic              try to use only PIC/non-PIC objects [default=use both]],
     pic_mode="$withval", pic_mode=default)
test x"$pic_mode" = xyes && libtool_flags="$libtool_flags --prefer-pic"
test x"$pic_mode" = xno && libtool_flags="$libtool_flags --prefer-non-pic"

# Some flags need to be propagated to the compiler or linker for good
# libtool support.
case "$lt_target" in
*-*-irix6*)
  # Find out which ABI we are using.
  echo '[#]line __oline__ "configure"' > conftest.$ac_ext
  if AC_TRY_EVAL(ac_compile); then
    case "`/usr/bin/file conftest.o`" in
    *32-bit*)
      LD="${LD-ld} -32"
      ;;
    *N32*)
      LD="${LD-ld} -n32"
      ;;
    *64-bit*)
      LD="${LD-ld} -64"
      ;;
    esac
  fi
  rm -rf conftest*
  ;;

*-*-sco3.2v5*)
  # On SCO OpenServer 5, we need -belf to get full-featured binaries.
  SAVE_CFLAGS="$CFLAGS"
  CFLAGS="$CFLAGS -belf"
  AC_CACHE_CHECK([whether the C compiler needs -belf], lt_cv_cc_needs_belf,
    [AC_TRY_LINK([],[],[lt_cv_cc_needs_belf=yes],[lt_cv_cc_needs_belf=no])])
  if test x"$lt_cv_cc_needs_belf" != x"yes"; then
    # this is probably gcc 2.8.0, egcs 1.0 or newer; no need for -belf
    CFLAGS="$SAVE_CFLAGS"
  fi
  ;;

ifdef([AC_PROVIDE_AC_LIBTOOL_WIN32_DLL],
[*-*-cygwin* | *-*-mingw*)
  AC_CHECK_TOOL(DLLTOOL, dlltool, false)
  AC_CHECK_TOOL(AS, as, false)
  AC_CHECK_TOOL(OBJDUMP, objdump, false)

  # recent cygwin and mingw systems supply a stub DllMain which the user
  # can override, but on older systems we have to supply one
  AC_CACHE_CHECK([if libtool should supply DllMain function], lt_cv_need_dllmain,
    [AC_TRY_LINK([],
      [extern int __attribute__((__stdcall__)) DllMain(void*, int, void*);
      DllMain (0, 0, 0);],
      [lt_cv_need_dllmain=yes],[lt_cv_need_dllmain=no])])

  case "$lt_target/$CC" in
  *-*-cygwin*/gcc*-mno-cygwin*|*-*-mingw*)
    # old mingw systems require "-dll" to link a DLL, while more recent ones
    # require "-mdll"
    SAVE_CFLAGS="$CFLAGS"
    CFLAGS="$CFLAGS -mdll"
    AC_CACHE_CHECK([how to link DLLs], lt_cv_cc_dll_switch,
      [AC_TRY_LINK([], [], [lt_cv_cc_dll_switch=-mdll],[lt_cv_cc_dll_switch=-dll])])
    CFLAGS="$SAVE_CFLAGS" ;;
  *-*-cygwin*)
    # cygwin systems need to pass --dll to the linker, and not link
    # crt.o which will require a WinMain@16 definition.
    lt_cv_cc_dll_switch="-Wl,--dll -nostartfiles" ;;
  esac
  ;;
  ])
esac
])

# AC_LIBTOOL_DLOPEN - enable checks for dlopen support
AC_DEFUN(AC_LIBTOOL_DLOPEN, [AC_BEFORE([$0],[AC_LIBTOOL_SETUP])])

# AC_LIBTOOL_WIN32_DLL - declare package support for building win32 dll's
AC_DEFUN(AC_LIBTOOL_WIN32_DLL, [AC_BEFORE([$0], [AC_LIBTOOL_SETUP])])

# AC_ENABLE_SHARED - implement the --enable-shared flag
# Usage: AC_ENABLE_SHARED[(DEFAULT)]
#   Where DEFAULT is either `yes' or `no'.  If omitted, it defaults to
#   `yes'.
AC_DEFUN(AC_ENABLE_SHARED, [dnl
define([AC_ENABLE_SHARED_DEFAULT], ifelse($1, no, no, yes))dnl
AC_ARG_ENABLE(shared,
changequote(<<, >>)dnl
<<  --enable-shared[=PKGS]  build shared libraries [default=>>AC_ENABLE_SHARED_DEFAULT],
changequote([, ])dnl
[p=${PACKAGE-default}
case "$enableval" in
yes) enable_shared=yes ;;
no) enable_shared=no ;;
*)
  enable_shared=no
  # Look at the argument we got.  We use all the common list separators.
  IFS="${IFS= 	}"; ac_save_ifs="$IFS"; IFS="${IFS}:,"
  for pkg in $enableval; do
    if test "X$pkg" = "X$p"; then
      enable_shared=yes
    fi
  done
  IFS="$ac_save_ifs"
  ;;
esac],
enable_shared=AC_ENABLE_SHARED_DEFAULT)dnl
])

# AC_DISABLE_SHARED - set the default shared flag to --disable-shared
AC_DEFUN(AC_DISABLE_SHARED, [AC_BEFORE([$0],[AC_LIBTOOL_SETUP])dnl
AC_ENABLE_SHARED(no)])

# AC_ENABLE_STATIC - implement the --enable-static flag
# Usage: AC_ENABLE_STATIC[(DEFAULT)]
#   Where DEFAULT is either `yes' or `no'.  If omitted, it defaults to
#   `yes'.
AC_DEFUN(AC_ENABLE_STATIC, [dnl
define([AC_ENABLE_STATIC_DEFAULT], ifelse($1, no, no, yes))dnl
AC_ARG_ENABLE(static,
changequote(<<, >>)dnl
<<  --enable-static[=PKGS]  build static libraries [default=>>AC_ENABLE_STATIC_DEFAULT],
changequote([, ])dnl
[p=${PACKAGE-default}
case "$enableval" in
yes) enable_static=yes ;;
no) enable_static=no ;;
*)
  enable_static=no
  # Look at the argument we got.  We use all the common list separators.
  IFS="${IFS= 	}"; ac_save_ifs="$IFS"; IFS="${IFS}:,"
  for pkg in $enableval; do
    if test "X$pkg" = "X$p"; then
      enable_static=yes
    fi
  done
  IFS="$ac_save_ifs"
  ;;
esac],
enable_static=AC_ENABLE_STATIC_DEFAULT)dnl
])

# AC_DISABLE_STATIC - set the default static flag to --disable-static
AC_DEFUN(AC_DISABLE_STATIC, [AC_BEFORE([$0],[AC_LIBTOOL_SETUP])dnl
AC_ENABLE_STATIC(no)])


# AC_ENABLE_FAST_INSTALL - implement the --enable-fast-install flag
# Usage: AC_ENABLE_FAST_INSTALL[(DEFAULT)]
#   Where DEFAULT is either `yes' or `no'.  If omitted, it defaults to
#   `yes'.
AC_DEFUN(AC_ENABLE_FAST_INSTALL, [dnl
define([AC_ENABLE_FAST_INSTALL_DEFAULT], ifelse($1, no, no, yes))dnl
AC_ARG_ENABLE(fast-install,
changequote(<<, >>)dnl
<<  --enable-fast-install[=PKGS]  optimize for fast installation [default=>>AC_ENABLE_FAST_INSTALL_DEFAULT],
changequote([, ])dnl
[p=${PACKAGE-default}
case "$enableval" in
yes) enable_fast_install=yes ;;
no) enable_fast_install=no ;;
*)
  enable_fast_install=no
  # Look at the argument we got.  We use all the common list separators.
  IFS="${IFS= 	}"; ac_save_ifs="$IFS"; IFS="${IFS}:,"
  for pkg in $enableval; do
    if test "X$pkg" = "X$p"; then
      enable_fast_install=yes
    fi
  done
  IFS="$ac_save_ifs"
  ;;
esac],
enable_fast_install=AC_ENABLE_FAST_INSTALL_DEFAULT)dnl
])

# AC_ENABLE_FAST_INSTALL - set the default to --disable-fast-install
AC_DEFUN(AC_DISABLE_FAST_INSTALL, [AC_BEFORE([$0],[AC_LIBTOOL_SETUP])dnl
AC_ENABLE_FAST_INSTALL(no)])


# AC_PATH_TOOL_PREFIX - find a file program which can recognise shared library
AC_DEFUN(AC_PATH_TOOL_PREFIX,
[AC_MSG_CHECKING([for $1])
AC_CACHE_VAL(lt_cv_path_FILE,
[case "$FILE" in
  /*)
  lt_cv_path_FILE="$FILE" # Let the user override the test with a path.
  ;;
  ?:/*)
  ac_cv_path_FILE="$FILE" # Let the user override the test with a dos path.
  ;;
  *)
  ac_save_file="$FILE"
  IFS="${IFS=   }"; ac_save_ifs="$IFS"; IFS=":"
dnl $ac_dummy forces splitting on constant user-supplied paths.
dnl POSIX.2 word splitting is done only on the output of word expansions,
dnl not every word.  This closes a longstanding sh security hole.
  ac_dummy="ifelse([$2], , $PATH, [$2])"
  for ac_dir in $ac_dummy; do
    test -z "$ac_dir" && ac_dir=.
    if test -f $ac_dir/$1; then
      lt_cv_path_FILE="$ac_dir/$1"
      if test -n "$file_magic_test_file"; then
        case "$deplibs_check_method" in
        "file_magic "*)
          file_magic_regex="`expr \"$deplibs_check_method\" : \"file_magic \(.*\)\"`"
	  FILE="$lt_cv_path_FILE"
          if eval $file_magic_cmd \$file_magic_test_file 2> /dev/null |
            egrep "$file_magic_regex" > /dev/null; then
            :
          else
            cat <<EOF 1>&2

*** Warning: the command libtool uses to detect shared libraries,
*** $file_magic_cmd, produces output that libtool cannot recognize.
*** The result is that libtool may fail to recognize shared libraries
*** as such.  This will affect the creation of libtool libraries that
*** depend on shared libraries, but programs linked with such libtool
*** libraries will work regardless of this problem.  Nevertheless, you
*** may want to report the problem to your system manager and/or to
*** bug-libtool@gnu.org

EOF
          fi ;;
        esac
      fi
      break
    fi
  done
  IFS="$ac_save_ifs"
  FILE="$ac_save_file"
  ;;
esac])
FILE="$lt_cv_path_FILE"
if test -n "$FILE"; then
  AC_MSG_RESULT($FILE)
else
  AC_MSG_RESULT(no)
fi
])


# AC_PATH_FILE - find a file program which can recognise a shared library
AC_DEFUN(AC_PATH_FILE,
[AC_REQUIRE([AC_CHECK_TOOL_PREFIX])dnl
AC_PATH_TOOL_PREFIX(${ac_tool_prefix}file, /usr/bin:$PATH)
if test -z "$lt_cv_path_FILE"; then
  if test -n "$ac_tool_prefix"; then
    AC_PATH_TOOL_PREFIX(file, /usr/bin:$PATH)
  else
    FILE=:
  fi
fi
])


# AC_PROG_LD - find the path to the GNU or non-GNU linker
AC_DEFUN(AC_PROG_LD,
[AC_ARG_WITH(gnu-ld,
[  --with-gnu-ld           assume the C compiler uses GNU ld [default=no]],
test "$withval" = no || with_gnu_ld=yes, with_gnu_ld=no)
AC_REQUIRE([AC_PROG_CC])dnl
AC_REQUIRE([AC_CANONICAL_HOST])dnl
AC_REQUIRE([AC_CANONICAL_BUILD])dnl
ac_prog=ld
if test "$ac_cv_prog_gcc" = yes; then
  # Check if gcc -print-prog-name=ld gives a path.
  AC_MSG_CHECKING([for ld used by GCC])
  case $lt_target in
  *-*-mingw*)
    # gcc leaves a trailing carriage return which upsets mingw
    ac_prog=`($CC -print-prog-name=ld) 2>&5 | tr -d '\015'` ;;
  *)
    ac_prog=`($CC -print-prog-name=ld) 2>&5` ;;
  esac
  case "$ac_prog" in
    # Accept absolute paths.
changequote(,)dnl
    [\\/]* | [A-Za-z]:[\\/]*)
      re_direlt='/[^/][^/]*/\.\./'
changequote([,])dnl
      # Canonicalize the path of ld
      ac_prog=`echo $ac_prog| sed 's%\\\\%/%g'`
      while echo $ac_prog | grep "$re_direlt" > /dev/null 2>&1; do
	ac_prog=`echo $ac_prog| sed "s%$re_direlt%/%"`
      done
      test -z "$LD" && LD="$ac_prog"
      ;;
  "")
    # If it fails, then pretend we aren't using GCC.
    ac_prog=ld
    ;;
  *)
    # If it is relative, then search for the first ld in PATH.
    with_gnu_ld=unknown
    ;;
  esac
elif test "$with_gnu_ld" = yes; then
  AC_MSG_CHECKING([for GNU ld])
else
  AC_MSG_CHECKING([for non-GNU ld])
fi
AC_CACHE_VAL(ac_cv_path_LD,
[if test -z "$LD"; then
  IFS="${IFS= 	}"; ac_save_ifs="$IFS"; IFS="${IFS}${PATH_SEPARATOR-:}"
  for ac_dir in $PATH; do
    test -z "$ac_dir" && ac_dir=.
    if test -f "$ac_dir/$ac_prog" || test -f "$ac_dir/$ac_prog$ac_exeext"; then
      ac_cv_path_LD="$ac_dir/$ac_prog"
      # Check to see if the program is GNU ld.  I'd rather use --version,
      # but apparently some GNU ld's only accept -v.
      # Break only if it was the GNU/non-GNU ld that we prefer.
      if "$ac_cv_path_LD" -v 2>&1 < /dev/null | egrep '(GNU|with BFD)' > /dev/null; then
	test "$with_gnu_ld" != no && break
      else
	test "$with_gnu_ld" != yes && break
      fi
    fi
  done
  IFS="$ac_save_ifs"
else
  ac_cv_path_LD="$LD" # Let the user override the test with a path.
fi])
LD="$ac_cv_path_LD"
if test -n "$LD"; then
  AC_MSG_RESULT($LD)
else
  AC_MSG_RESULT(no)
fi
test -z "$LD" && AC_MSG_ERROR([no acceptable ld found in \$PATH])
AC_PROG_LD_GNU
])

AC_DEFUN(AC_PROG_LD_GNU,
[AC_CACHE_CHECK([if the linker ($LD) is GNU ld], ac_cv_prog_gnu_ld,
[# I'd rather use --version here, but apparently some GNU ld's only accept -v.
if $LD -v 2>&1 </dev/null | egrep '(GNU|with BFD)' 1>&5; then
  ac_cv_prog_gnu_ld=yes
else
  ac_cv_prog_gnu_ld=no
fi])
with_gnu_ld=$ac_cv_prog_gnu_ld
])

# AC_PROG_LD_RELOAD_FLAG - find reload flag for linker
#   -- PORTME Some linkers may need a different reload flag.
AC_DEFUN(AC_PROG_LD_RELOAD_FLAG,
[AC_CACHE_CHECK([for $LD option to reload object files], lt_cv_ld_reload_flag,
[lt_cv_ld_reload_flag='-r'])
reload_flag=$lt_cv_ld_reload_flag
test -n "$reload_flag" && reload_flag=" $reload_flag"
])

# AC_DEPLIBS_CHECK_METHOD - how to check for library dependencies
#  -- PORTME fill in with the dynamic library characteristics
AC_DEFUN(AC_DEPLIBS_CHECK_METHOD,
[AC_CACHE_CHECK([how to recognise dependant libraries], 
lt_cv_deplibs_check_method,
[lt_cv_file_magic_cmd='$FILE'
lt_cv_file_magic_test_file=
lt_cv_deplibs_check_method='unknown'
# Need to set the preceding variable on all platforms that support
# interlibrary dependencies.
# 'none' -- dependencies not supported.
# `unknown' -- same as none, but documents that we really don't know.
# 'pass_all' -- all dependencies passed with no checks.
# 'test_compile' -- check by making test program.
# 'file_magic [regex]' -- check by looking for files in library path
# which responds to the $file_magic_cmd with a given egrep regex.
# If you have `file' or equivalent on your system and you're not sure
# whether `pass_all' will *always* work, you probably want this one.

case "$host_os" in
aix4* | beos*)
  lt_cv_deplibs_check_method=pass_all
  ;;
  
bsdi4*)
  lt_cv_deplibs_check_method='file_magic ELF [0-9][0-9]*-bit [ML]SB (shared object|dynamic lib)'
  lt_cv_file_magic_test_file=/shlib/libc.so
  ;;  

cygwin* | mingw*)
  lt_cv_deplibs_check_method='file_magic file format pei*-i386(.*architecture: i386)?'
  lt_cv_file_magic_cmd='${OBJDUMP} -f'
  ;;

freebsd*)
  case "$version_type" in
  freebsd-elf*)
    lt_cv_deplibs_check_method=pass_all
    ;;
  esac
  ;;
  
gnu*)
  lt_cv_deplibs_check_method=pass_all
  ;;
  
irix5* | irix6*)
  case "$host_os" in
  irix5*)
    # this will be overridden with pass_all, but let us keep it just in case
    lt_cv_deplibs_check_method="file_magic ELF 32-bit MSB dynamic lib MIPS - version 1"
    ;;
  *)
    case "$LD" in
    *-32|*"-32 ") libmagic=32-bit;;
    *-n32|*"-n32 ") libmagic=N32;;
    *-64|*"-64 ") libmagic=64-bit;;
    *) libmagic=never-match;;
    esac
    # this will be overridden with pass_all, but let us keep it just in case
    lt_cv_deplibs_check_method="file_magic ELF ${libmagic} MSB mips-[1234] dynamic lib MIPS - version 1"
    ;;
  esac
  lt_cv_file_magic_test_file=`echo /lib${libsuff}/libc.so*`
  lt_cv_deplibs_check_method=pass_all
  ;;

# This must be Linux ELF.
linux-gnu*)
  case "$host_cpu" in
  alpha* | i*86 | sparc* )
    lt_cv_deplibs_check_method=pass_all ;;
  *)
    # glibc up to 2.1.1 does not perform some relocations on ARM
    lt_cv_deplibs_check_method='file_magic ELF [0-9][0-9]*-bit [LM]SB (shared object|dynamic lib )' ;;
  esac
  lt_cv_file_magic_test_file=`echo /lib/libc.so* /lib/libc-*.so`
  ;;

osf3* | osf4* | osf5*)
  # this will be overridden with pass_all, but let us keep it just in case
  lt_cv_deplibs_check_method='file_magic COFF format alpha shared library'
  lt_cv_file_magic_test_file=/shlib/libc.so
  lt_cv_deplibs_check_method=pass_all
  ;;

sco3.2v5*)
  lt_cv_deplibs_check_method=pass_all
  ;;
  
solaris*)
  lt_cv_deplibs_check_method=pass_all
  lt_cv_file_magic_test_file=/lib/libc.so
  ;;

sysv4 | sysv4.2uw2* | sysv4.3* | sysv5*)
  case "$host_vendor" in
  ncr)
    lt_cv_deplibs_check_method=pass_all
    ;;
  motorola)
    lt_cv_deplibs_check_method='file_magic ELF [0-9][0-9]*-bit [ML]SB (shared object|dynamic lib) M[0-9][0-9]* Version [0-9]'
    lt_cv_file_magic_test_file=`echo /usr/lib/libc.so*`
    ;;
  esac
  ;;
esac
])
file_magic_cmd=$lt_cv_file_magic_cmd
deplibs_check_method=$lt_cv_deplibs_check_method
])


# AC_PROG_NM - find the path to a BSD-compatible name lister
AC_DEFUN(AC_PROG_NM,
[AC_MSG_CHECKING([for BSD-compatible nm])
AC_CACHE_VAL(ac_cv_path_NM,
[if test -n "$NM"; then
  # Let the user override the test.
  ac_cv_path_NM="$NM"
else
  IFS="${IFS= 	}"; ac_save_ifs="$IFS"; IFS="${IFS}${PATH_SEPARATOR-:}"
  for ac_dir in $PATH /usr/ccs/bin /usr/ucb /bin; do
    test -z "$ac_dir" && ac_dir=.
    if test -f $ac_dir/nm || test -f $ac_dir/nm$ac_exeext ; then
      # Check to see if the nm accepts a BSD-compat flag.
      # Adding the `sed 1q' prevents false positives on HP-UX, which says:
      #   nm: unknown option "B" ignored
      if ($ac_dir/nm -B /dev/null 2>&1 | sed '1q'; exit 0) | egrep /dev/null >/dev/null; then
	ac_cv_path_NM="$ac_dir/nm -B"
	break
      elif ($ac_dir/nm -p /dev/null 2>&1 | sed '1q'; exit 0) | egrep /dev/null >/dev/null; then
	ac_cv_path_NM="$ac_dir/nm -p"
	break
      else
	ac_cv_path_NM=${ac_cv_path_NM="$ac_dir/nm"} # keep the first match, but
	continue # so that we can try to find one that supports BSD flags
      fi
    fi
  done
  IFS="$ac_save_ifs"
  test -z "$ac_cv_path_NM" && ac_cv_path_NM=nm
fi])
NM="$ac_cv_path_NM"
AC_MSG_RESULT([$NM])
])

# AC_CHECK_LIBM - check for math library
AC_DEFUN(AC_CHECK_LIBM,
[AC_REQUIRE([AC_CANONICAL_HOST])dnl
LIBM=
case "$lt_target" in
*-*-beos* | *-*-cygwin*)
  # These system don't have libm
  ;;
*-ncr-sysv4.3*)
  AC_CHECK_LIB(mw, _mwvalidcheckl, LIBM="-lmw")
  AC_CHECK_LIB(m, main, LIBM="$LIBM -lm")
  ;;
*)
  AC_CHECK_LIB(m, main, LIBM="-lm")
  ;;
esac
])

# AC_LIBLTDL_CONVENIENCE[(dir)] - sets LIBLTDL to the link flags for
# the libltdl convenience library, adds --enable-ltdl-convenience to
# the configure arguments.  Note that LIBLTDL is not AC_SUBSTed, nor
# is AC_CONFIG_SUBDIRS called.  If DIR is not provided, it is assumed
# to be `${top_builddir}/libltdl'.  Make sure you start DIR with
# '${top_builddir}/' (note the single quotes!) if your package is not
# flat, and, if you're not using automake, define top_builddir as
# appropriate in the Makefiles.
AC_DEFUN(AC_LIBLTDL_CONVENIENCE, [AC_BEFORE([$0],[AC_LIBTOOL_SETUP])dnl
  case "$enable_ltdl_convenience" in
  no) AC_MSG_ERROR([this package needs a convenience libltdl]) ;;
  "") enable_ltdl_convenience=yes
      ac_configure_args="$ac_configure_args --enable-ltdl-convenience" ;;
  esac
  LIBLTDL=ifelse($#,1,$1,['${top_builddir}/libltdl'])/libltdlc.la
  INCLTDL=ifelse($#,1,-I$1,['-I${top_builddir}/libltdl'])
])

# AC_LIBLTDL_INSTALLABLE[(dir)] - sets LIBLTDL to the link flags for
# the libltdl installable library, and adds --enable-ltdl-install to
# the configure arguments.  Note that LIBLTDL is not AC_SUBSTed, nor
# is AC_CONFIG_SUBDIRS called.  If DIR is not provided, it is assumed
# to be `${top_builddir}/libltdl'.  Make sure you start DIR with
# '${top_builddir}/' (note the single quotes!) if your package is not
# flat, and, if you're not using automake, define top_builddir as
# appropriate in the Makefiles.
# In the future, this macro may have to be called after AC_PROG_LIBTOOL.
AC_DEFUN(AC_LIBLTDL_INSTALLABLE, [AC_BEFORE([$0],[AC_LIBTOOL_SETUP])dnl
  AC_CHECK_LIB(ltdl, main,
  [test x"$enable_ltdl_install" != xyes && enable_ltdl_install=no],
  [if test x"$enable_ltdl_install" = xno; then
     AC_MSG_WARN([libltdl not installed, but installation disabled])
   else
     enable_ltdl_install=yes
   fi
  ])
  if test x"$enable_ltdl_install" = x"yes"; then
    ac_configure_args="$ac_configure_args --enable-ltdl-install"
    LIBLTDL=ifelse($#,1,$1,['${top_builddir}/libltdl'])/libltdl.la
    INCLTDL=ifelse($#,1,-I$1,['-I${top_builddir}/libltdl'])
  else
    ac_configure_args="$ac_configure_args --enable-ltdl-install=no"
    LIBLTDL="-lltdl"
    INCLTDL=
  fi
])

# AC_LIBTOOL_CXX - enable support for C++ libraries
AC_DEFUN(AC_LIBTOOL_CXX,
[AC_REQUIRE([AC_PROG_CXX])
AC_REQUIRE([AC_PROG_CXXCPP])
AC_REQUIRE([AC_PROG_LIBTOOL])
lt_save_CC="$CC"
lt_save_CFLAGS="$CFLAGS"
dnl Make sure LTCC is set to the C compiler, i.e. set LTCC before CC
dnl is set to the C++ compiler.
AR="$AR" LTCC="$CC" CC="$CXX" CFLAGS="$CXXFLAGS" CPPFLAGS="$CPPFLAGS" \
FILE="$FILE" LIBS="$LIBS" \
LN_S="$LN_S" NM="$NM" RANLIB="$RANLIB" STRIP="$STRIP" \
AS="$AS" DLLTOOL="$DLLTOOL" OBJDUMP="$OBJDUMP" \
objext="$OBJEXT" exeext="$EXEEXT" \
deplibs_check_method="$deplibs_check_method" \
file_magic_cmd="$file_magic_cmd" \
${CONFIG_SHELL-/bin/sh} $ac_aux_dir/ltconfig -o libtool $libtool_flags \
--build="$build" --add-tag=CXX $ac_aux_dir/ltcf-cxx.sh \
|| AC_MSG_ERROR([libtool tag configuration failed])
CC="$lt_save_CC"
CFLAGS="$lt_save_CFLAGS"
])

dnl old names
AC_DEFUN(AM_PROG_LIBTOOL, [indir([AC_PROG_LIBTOOL])])dnl
AC_DEFUN(AM_ENABLE_SHARED, [indir([AC_ENABLE_SHARED], $@)])dnl
AC_DEFUN(AM_ENABLE_STATIC, [indir([AC_ENABLE_STATIC], $@)])dnl
AC_DEFUN(AM_DISABLE_SHARED, [indir([AC_DISABLE_SHARED], $@)])dnl
AC_DEFUN(AM_DISABLE_STATIC, [indir([AC_DISABLE_STATIC], $@)])dnl
AC_DEFUN(AM_PROG_LD, [indir([AC_PROG_LD])])dnl
AC_DEFUN(AM_PROG_NM, [indir([AC_PROG_NM])])dnl

dnl This is just to silence aclocal about the macro not being used
ifelse([AC_DISABLE_FAST_INSTALL])dnl

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
    ACE_SEARCH_THREAD_FLAGS(
      [mt pthread pthreads mthreads threads Kthread -thread_safe],,)
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

dnl -------------------------------------------------------------------------
dnl       $Id$
dnl 
dnl       features.m4
dnl
dnl       ACE M4 include file which contains ACE specific M4 macros
dnl       that determine availablility of certain OS features for ACE.
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

dnl Asynchronous IO check
dnl Use this macro to determine if asynchronous IO is working on a
dnl given platform.
dnl Usage: ACE_CHECK_ASYNCH_IO
AC_DEFUN(ACE_CHECK_ASYNCH_IO, dnl
[
 AC_REQUIRE([AC_PROG_CXX])
 AC_REQUIRE([AC_PROG_CXXCPP])
 AC_REQUIRE([AC_LANG_CPLUSPLUS])
 AC_REQUIRE([ACE_CHECK_THREADS])

 dnl In case a library with the asynchronous libraries is found but
 dnl the asynchronous IO support is not functional then save a copy
 dnl of the list of libraries before the asynch IO function library
 dnl is added to the list so that we can revert the list to its
 dnl pre-asynch-IO check state.
 ace_save_LIBS="$LIBS"

 dnl Asynchronous IO library check
 dnl Some platforms, such as Solaris puts aio_read in -lposix4, for example.
 dnl In some cases, the thread library must be linked to in addition to the
 dnl real-time support library.  As such, make sure these checks are done
 dnl after the thread library checks.
 ACE_SEARCH_LIBS([aio_read], [aio rt posix4],
    [ace_has_aio_funcs=yes], [ace_has_aio_funcs=no])

if test "$ace_has_aio_funcs" = yes; then
  ACE_CACHE_CHECK([for working asynchronous IO],
    [ace_cv_feature_aio_calls],
    [
     AC_TRY_RUN(
       [
#ifndef ACE_LACKS_UNISTD_H
#include <unistd.h>
#endif
#include <fcntl.h>
#ifndef ACE_LACKS_SYS_TYPES_H
# include <sys/types.h>
#endif
#include <sys/stat.h>
#include <signal.h>
#include <string.h>
#include <errno.h>
#include <stdio.h>
#include <iostream.h>

#include <aio.h>

class Test_Aio
{
public:
  Test_Aio (void);
  // Default constructor.

  int init (void);
  // Initting the output file and the buffer.

  int do_aio (void);
  // Doing the testing stuff.

  ~Test_Aio (void);
  // Destructor.
private:
  int out_fd_;
  // Output file descriptor.

  struct aiocb *aiocb_write_;
  // For writing to the file.

  struct aiocb *aiocb_read_;
  // Reading stuff from the file.

  char *buffer_write_;
  // The buffer to be written to the out_fd.

  char *buffer_read_;
  // The buffer to be read back from the file.
};

Test_Aio::Test_Aio (void)
  : out_fd_ (0),
    aiocb_write_ (new struct aiocb),
    aiocb_read_ (new struct aiocb),
    buffer_write_ (0),
    buffer_read_ (0)
{
}

Test_Aio::~Test_Aio (void)
{
  if (close (this->out_fd_) != 0)
    perror ("close");

  delete aiocb_write_;
  delete aiocb_read_;
  delete [] buffer_write_;
  delete [] buffer_read_;
}

// Init the output file and init the buffer.
int
Test_Aio::init (void)
{
  // Open the output file.
  this->out_fd_ = open ("test_aio.log", O_RDWR | O_CREAT | O_TRUNC, 0600);
  if (this->out_fd_ == -1)
    {
      perror ("open");
      return -1;
    }

  unlink ("test_aio.log"); // Unlink now so we don't have to do so later.

  const char message[] = "Welcome to the world of AIO... AIO Rules !!!";

  // Init the buffers.
  this->buffer_write_ = new char [sizeof (message) + 1];
  strcpy (this->buffer_write_, message);
  // cout << "The buffer : " << this->buffer_write_ << endl;
  this->buffer_read_ = new char [sizeof (message) + 1];

  return 0;
}

// Set the necessary things for the AIO stuff.
// Write the buffer asynchly.hmm Disable signals.
// Go on aio_suspend. Wait for completion.
// Print out the result.
int
Test_Aio::do_aio (void)
{
  // = Write to the file.

  // Setup AIOCB.
  this->aiocb_write_->aio_fildes = this->out_fd_;
  this->aiocb_write_->aio_offset = 0;
  this->aiocb_write_->aio_buf = this->buffer_write_;
  this->aiocb_write_->aio_nbytes = strlen (this->buffer_write_);
  this->aiocb_write_->aio_reqprio = 0;
  this->aiocb_write_->aio_sigevent.sigev_notify = SIGEV_NONE;
  //this->this->aiocb_.aio_sigevent.sigev_signo = SIGRTMAX;
  this->aiocb_write_->aio_sigevent.sigev_value.sival_ptr =
    (void *) this->aiocb_write_;

  // Fire off the aio write.
  if (aio_write (this->aiocb_write_) != 0)
    {
      perror ("aio_write");
      return -1;
    }

  // = Read from that file.

  // Setup AIOCB.
  this->aiocb_read_->aio_fildes = this->out_fd_;
  this->aiocb_read_->aio_offset = 0;
  this->aiocb_read_->aio_buf = this->buffer_read_;
  this->aiocb_read_->aio_nbytes = strlen (this->buffer_write_);
  this->aiocb_read_->aio_reqprio = 0;
  this->aiocb_read_->aio_sigevent.sigev_notify = SIGEV_NONE;
  //this->this->aiocb_.aio_sigevent.sigev_signo = SIGRTMAX;
  this->aiocb_read_->aio_sigevent.sigev_value.sival_ptr =
    (void *) this->aiocb_read_;

  // Fire off the aio write. If it doesnt get queued, carry on to get
  // the completion for the first one.
  if (aio_read (this->aiocb_read_) < 0)
    perror ("aio_read");
  
  // Wait for the completion on aio_suspend.
  struct aiocb *list_aiocb[2];
  list_aiocb [0] = this->aiocb_write_;
  list_aiocb [1] = this->aiocb_read_;

  // Do suspend till all the aiocbs in the list are done.
  int done = 0;
  while (!done)
    {
      if (aio_suspend (list_aiocb, 2, 0) != 0)
        {
          perror ("aio_suspend");
          return -1;
        }

      // Analyze return and error values.
      if (list_aiocb [0] != 0 && aio_error (list_aiocb [0]) != EINPROGRESS)
        {
          if (aio_return (list_aiocb [0]) == -1)
            {
              perror ("aio_return");
              return -1;
            }
          else
            {
              // Successful. Store the pointer somewhere and make the
              // entry NULL in the list.
              // @@ no need ----> this->aiocb_write_ = list_aiocb [0];
              list_aiocb [0] = 0;
            }
        }
//      else
//        cout << "AIO in progress" << endl;

      if (list_aiocb [1] != 0 && aio_error (list_aiocb [1]) != EINPROGRESS)
        {
          if (aio_return (list_aiocb [1]) == -1)
            {
              perror ("aio_return");
              return -1;
            }
          else
            {
              // Successful. Store the pointer somewhere and make the
              // entry NULL in the list.
              // @@ no need ----> this->aiocb_read_ = list_aiocb [1];
              list_aiocb [1] = 0;
            }
        }
//      else
//        cout << "AIO in progress" << endl;

      // Is it done?
      if ((list_aiocb [0] == 0) && (list_aiocb [1] == 0))
        done = 1;
    }

  //cout << "Both the AIO operations done." << endl;
  //cout << "The buffer is :" << this->buffer_read_ << endl;
  
  return 0;
}

int
main (int argc, char **argv)
{
  Test_Aio test_aio;

  if (test_aio.init () != 0)
    {
      //printf ("AIOCB test failed:\n"
      //        "ACE_POSIX_AIOCB_PROACTOR may not work in this platform\n");
      return -1;
    }
  
  if (test_aio.do_aio () != 0)
    {
      //printf ("AIOCB test failed:\n"
      //        "ACE_POSIX_AIOCB_PROACTOR may not work in this platform\n"); 
      return -1;
    }
  //printf ("AIOCB test successful:\n"
  //        "ACE_POSIX_AIOCB_PROACTOR should work in this platform\n");
  return 0;
}
       ],
       [
        dnl Now try another test

        dnl Create a file for the test program to read.
        cat > test_aiosig.txt <<EOF

*******************************************************
FOO BAR FOO BAR FOO BAR FOO BAR FOO BAR FOO BAR FOO BAR
*******************************************************
EOF


        AC_TRY_RUN(
          [
#ifndef ACE_LACKS_UNISTD_H
#include <unistd.h>
#endif
#include <fcntl.h>
#ifndef ACE_LACKS_SYS_TYPES_H
# include <sys/types.h>
#endif
#include <sys/stat.h>
#include <pthread.h>
#include <signal.h>
#include <string.h>
#include <errno.h>
#include <stdio.h>

#include <limits.h>

#include <aio.h>

#ifdef __cplusplus
extern "C"
#endif
void null_handler (int /* signal_number */,
                   siginfo_t * /* info */,
                   void *      /* context */);

int file_handle = -1;
char mb1 [BUFSIZ + 1];
char mb2 [BUFSIZ + 1];
aiocb aiocb1, aiocb2;
sigset_t completion_signal;

// Function prototypes.
int setup_signal_delivery (void);
int issue_aio_calls (void);
int query_aio_completions (void);
int test_aio_calls (void);
int setup_signal_handler (void);
int setup_signal_handler (int signal_number);

int 
setup_signal_delivery (void)
{
  // Make the sigset_t consisting of the completion signal.
  if (sigemptyset (&completion_signal) == -1)
    {
      perror ("Error:Couldn't init the RT completion signal set\n");
      return -1;
    }
  
  if (sigaddset (&completion_signal, SIGRTMIN) == -1)
    {
      perror ("Error:Couldn't init the RT completion signal set\n");
      return -1;
    }
  
  // Mask them.
  if (pthread_sigmask (SIG_BLOCK, &completion_signal, 0) == -1)
    {
      perror ("Error:Couldn't make the RT completion signals\n");
      return -1;
    }
  
  return setup_signal_handler (SIGRTMIN);
}

int
issue_aio_calls (void)
{
  // Setup AIOCB.
  aiocb1.aio_fildes = file_handle;
  aiocb1.aio_offset = 0;
  aiocb1.aio_buf = mb1;
  aiocb1.aio_nbytes = BUFSIZ;
  aiocb1.aio_reqprio = 0;
  aiocb1.aio_sigevent.sigev_notify = SIGEV_SIGNAL;
  aiocb1.aio_sigevent.sigev_signo = SIGRTMIN;
  aiocb1.aio_sigevent.sigev_value.sival_ptr = (void *) &aiocb1; 
  
  // Fire off the aio write.
  if (aio_read (&aiocb1) == -1)
    {
      // Queueing failed.
      perror ("Error:Asynch_Read_Stream: aio_read queueing failed\n");
      return -1;
    }
  
  // Setup AIOCB.
  aiocb2.aio_fildes = file_handle;
  aiocb2.aio_offset = BUFSIZ + 1;
  aiocb2.aio_buf = mb2;
  aiocb2.aio_nbytes = BUFSIZ;
  aiocb2.aio_reqprio = 0;
  aiocb2.aio_sigevent.sigev_notify = SIGEV_SIGNAL;
  aiocb2.aio_sigevent.sigev_signo = SIGRTMIN;
  aiocb2.aio_sigevent.sigev_value.sival_ptr = (void *) &aiocb2; 
  
  // Fire off the aio write.
  if (aio_read (&aiocb2) == -1)
    {
      // Queueing failed.
      perror ("Error:Asynch_Read_Stream: aio_read queueing failed\n");
      return -1;
    }
  return 0;
}

int
query_aio_completions (void)
{
  int result = 0;
  size_t number_of_completions = 0;
  for (number_of_completions = 0;
       number_of_completions < 2;
       number_of_completions++)
    {
      // Wait for <milli_seconds> amount of time.
      // @@ Assigning <milli_seconds> to tv_sec.
      timespec timeout;
      timeout.tv_sec = INT_MAX;
      timeout.tv_nsec = 0;
  
      // To get back the signal info.
      siginfo_t sig_info;
      
      // Await the RT completion signal.
      int sig_return = sigtimedwait (&completion_signal,
                                     &sig_info,
                                     &timeout);
      
      // Error case.
      // If failure is coz of timeout, then return *0* but set
      // errno appropriately. This is what the WinNT proactor
      // does.
      if (sig_return == -1)
        {
          perror ("Error:Error waiting for RT completion signals\n");
          return -1;
        }
      
      // RT completion signals returned.
      if (sig_return != SIGRTMIN)
        {
          //printf ("Unexpected signal (%d) has been received while waiting for RT Completion Signals\n",
          //        sig_return);
          return -1;
        }
      
      // @@ Debugging.
      //printf ("Sig number found in the sig_info block : %d\n",
      //        sig_info.si_signo);
  
      // Is the signo returned consistent?
      if (sig_info.si_signo != sig_return)
        {
          //printf ("Inconsistent signal number (%d) in the signal info block\n",
          //        sig_info.si_signo);
          return -1;
        }
  
      // @@ Debugging.
      //printf ("Signal code for this signal delivery : %d\n",
      //        sig_info.si_code);
  
      // Is the signal code an aio completion one?
      if ((sig_info.si_code != SI_ASYNCIO) &&
          (sig_info.si_code != SI_QUEUE))
        {
          //printf ("Unexpected signal code (%d) returned on completion querying\n",
          //        sig_info.si_code);
          return -1;
        }
  
      // Retrive the aiocb.
      aiocb* aiocb_ptr = (aiocb *) sig_info.si_value.sival_ptr;
      
      // Analyze error and return values. Return values are
      // actually <errno>'s associated with the <aio_> call
      // corresponding to aiocb_ptr.
      int error_code = aio_error (aiocb_ptr);
      if (error_code == -1)
        {
          perror ("Error:Invalid control block was sent to <aio_error> for compleion querying\n");
          return -1;
        }
  
      if (error_code != 0)
        {
          // Error occurred in the <aio_>call. Return the errno
          // corresponding to that <aio_> call.
          //printf ("Error:An AIO call has failed:Error code = %d\n",
          //        error_code);
          return -1;
        }

      // No error occured in the AIO operation.
      int nbytes = aio_return (aiocb_ptr);
      if (nbytes == -1)
        {
          perror ("Error:Invalid control block was sent to <aio_return>\n");
          return -1;
        }
      
      //if (number_of_completions == 0)
        // Print the buffer.
        //printf ("Number of bytes transferred : %d\n The buffer : %s \n",
        //        nbytes,
        //        mb1);
      //else
        // Print the buffer.
        //printf ("Number of bytes transferred : %d\n The buffer : %s \n",
        //        nbytes,
        //        mb2);
    }
  return 0;
}

int
test_aio_calls (void) 
{
  // Set up the input file.
  // Open file (in SEQUENTIAL_SCAN mode)
  file_handle = open ("test_aiosig.txt", O_RDONLY);
  
  if (file_handle == -1)
    {
      perror ("open");
      return -1;
    }
 
  unlink ("test_aiosig.txt"); // Unlink now so we don't have to do so later.

  if (setup_signal_delivery () < 0)
    return -1;
  
  if (issue_aio_calls () < 0)
    return -1;
  
  if (query_aio_completions () < 0)
    return -1;

  if (close (file_handle) != 0)
    {
      perror ("close");
      return -1;
    }

  return 0;
}

int
setup_signal_handler (int signal_number)
{
   // Setting up the handler(!) for these signals.
  struct sigaction reaction;
  sigemptyset (&reaction.sa_mask);   // Nothing else to mask.
  reaction.sa_flags = SA_SIGINFO;    // Realtime flag.
#if defined (SA_SIGACTION)
  // Lynx says, it is better to set this bit to be portable.
  reaction.sa_flags &= SA_SIGACTION;
#endif /* SA_SIGACTION */      
  reaction.sa_sigaction = null_handler;     // Null handler.
  int sigaction_return = sigaction (SIGRTMIN,
                                    &reaction,
                                    0);
  if (sigaction_return == -1)
    {
      perror ("Error:Proactor couldn't do sigaction for the RT SIGNAL");
      return -1;
    }
  
  return 0;
}

void
null_handler (int         /* signal_number */,
              siginfo_t * /* info */,
              void *      /* context */)
{
}

int
main (int, char *[])
{
  if (test_aio_calls () == 0)
    {
    //printf ("RT SIG test successful:\n"
    //        "ACE_POSIX_SIG_PROACTOR should work in this platform\n");
      return 0;
    }
  else
    {
    //printf ("RT SIG test failed:\n"
    //        "ACE_POSIX_SIG_PROACTOR may not work in this platform\n");
  return -1;
    }
}
         ],
         [
          ace_cv_feature_aio_calls=yes
         ],
         [
          ace_cv_feature_aio_calls=no
         ],
         [
          dnl Don't bother doing anything for cross-compiling here
          dnl since the outer run-time test will prevent this
          dnl inner run-time test from ever running when cross-compiling.
          dnl We just put something in here to prevent autoconf
          dnl from complaining.
          ace_just_a_place_holder=ignoreme
         ])
       ],
       [
        ace_cv_feature_aio_calls=no
       ],
       [
        dnl Asynchronous IO test for cross-compiled platforms
        dnl This test is weaker than the above run-time tests but it will
        dnl have to do.
        AC_TRY_COMPILE(
          [
#include <aio.h>
          ],
          [
           aiocb* aiocb_ptr (void);
          ],
          [
           ace_cv_feature_aio_calls=yes
          ],
          [
           ace_cv_feature_aio_calls=no
          ])
       ])
    ],[AC_DEFINE(ACE_HAS_AIO_CALLS)],[LIBS="$ace_save_LIBS"])
fi dnl test "$ace_has_aio_funcs" = yes
])

dnl -------------------------------------------------------------------------
dnl       $Id$
dnl 
dnl       platform.m4
dnl
dnl       ACE M4 include file which contains ACE specific M4 macros
dnl       that set/determine which known platform specific C++ macros
dnl       to define.
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

dnl Check for thread related libraries and compiler flags
dnl Usage: ACE_SET_PLATFORM_MACROS
AC_DEFUN(ACE_SET_PLATFORM_MACROS, dnl
[
dnl Begin ACE_SET_PLATFORM_MACROS

dnl At some point the below platform specific settings should be automated
dnl as much as possible!  We set things manually just to get things going
dnl with the auto{conf,make}/libtool integration into the ACE source tree.

dnl These settings need to be done _after_ the header, function and library
dnl checks!

dnl Platform specific flags
case "$target" in
  *aix3*)
    AC_DEFINE(ACE_DEFAULT_BASE_ADDR, ((char *) 0x80000000))
    ;;
  *aix4.1*)
    AC_DEFINE(AIX)
    AC_DEFINE(ACE_DEFAULT_BASE_ADDR, ((char *) 0x80000000))
    AC_DEFINE(ACE_HAS_AIX_BROKEN_SOCKET_HEADER)
    ;;
  *aix4.2*)
    AC_DEFINE(AIX)
    AC_DEFINE(ACE_DEFAULT_BASE_ADDR, ((char *) 0x80000000))
dnl    AC_DEFINE(ACE_HAS_AIX_BROKEN_SOCKET_HEADER)
    AC_DEFINE(ACE_TLI_TCP_DEVICE, "/dev/xti/tcp")
    ;;
  *cray*)
    ;;
  *dgux4.11*)
    AC_DEFINE(ACE_DGUX)
    AC_DEFINE(IP_ADD_MEMBERSHIP, 0x13)
    AC_DEFINE(IP_DROP_MEMBERSHIP, 0x14)
    ;;
  *dgux4*)
    AC_DEFINE(ACE_DGUX)
    AC_DEFINE(IP_ADD_MEMBERSHIP, 0x13)
    AC_DEFINE(IP_DROP_MEMBERSHIP, 0x14)
    ;;
  *freebsd*)
    ;;
  *fsu*)
dnl FIXME: "FSU" isn't a platform!  We need to move this somewhere.
    AC_DEFINE(PTHREAD_STACK_MIN, (1024*10))
    ;;
  *hpux9*)
    AC_DEFINE(HPUX)
    ;;
  *hpux10*)
    AC_DEFINE(HPUX)
    AC_DEFINE(HPUX_10)
    AC_DEFINE(ACE_DEFAULT_BASE_ADDR, ((char *) 0x80000000))
    AC_DEFINE(ACE_TLI_TCP_DEVICE, "/dev/inet_cots")
    ;;
  *hpux11*)
    AC_DEFINE(HPUX)
    AC_DEFINE(HPUX_11)
    AC_EGREP_CPP(ACE_ON_64BIT_HP,
      [
#ifdef __LP64__
          ACE_ON_64BIT_HP
#endif
      ],
      [
       AC_DEFINE(ACE_DEFAULT_BASE_ADDR, ((char *) 0x0000001100000000))
       AC_DEFINE(ACE_DEFAULT_BASE_ADDRL, ((char *) 0x0000001100000000))
      ],
      [
       AC_DEFINE(ACE_DEFAULT_BASE_ADDR, ((char *) 0x80000000))
      ])
    AC_DEFINE(ACE_TIMER_SKEW, (1000 * 10))
    ;;
  *irix5.2*)
    AC_DEFINE(IRIX5)
    ;;
  *irix5.3*)
    AC_DEFINE(IRIX5)
    ;;
  *irix6*)
    AC_DEFINE(IRIX6)
    AC_DEFINE(ACE_HAS_IRIX62_THREADS)
    AC_DEFINE(ACE_DEFAULT_BASE_ADDR, ((char *) (1024U * 1024 * 1024)))
    if test "$ace_user_enable_threads" = yes; then
      AC_DEFINE(ACE_HAS_IRIX62_THREADS)
    fi
    AC_DEFINE(ACE_TIMER_SKEW, (1000 * 10))
    ;;
  *linux*)
    AC_DEFINE(ACE_DEFAULT_MAX_SOCKET_BUFSIZ, 65535)
    AC_DEFINE(ACE_DEFAULT_SELECT_REACTOR_SIZE, 256)
    AC_DEFINE(ACE_DEFAULT_BASE_ADDR, ((char *) 0x80000000))
    AC_DEFINE(ACE_HAS_BIG_FD_SET) dnl FIXME: We need a test for this!
    AC_DEFINE(ACE_UINT64_FORMAT_SPECIFIER, "%Lu")
    AC_DEFINE(ACE_TIMER_SKEW, (1000 * 10))
    ;;
  *lynxos*)
    AC_DEFINE(ACE_MALLOC_ALIGN, 8)
    AC_DEFINE(ACE_MAP_PRIVATE, ACE_MAP_SHARED)
    AC_DEFINE(ACE_USE_RCSID, 0)
    AC_DEFINE(ACE_HAS_LYNXOS_SIGNALS)
    AC_DEFINE(ACE_TIMER_SKEW, (1000 * 10))
    ;;
  *m88k*)
    AC_DEFINE(m88k)
    AC_DEFINE(__m88k__)
    AC_DEFINE(IP_ADD_MEMBERSHIP, 0x13)
    AC_DEFINE(IP_DROP_MEMBERSHIP, 0x14)
    ;;
  *mvs*)
    ;;
  *netbsd*)
    AC_DEFINE(ACE_NETBSD)
    ;;
  *osf3.2*)
    AC_EGREP_CPP(ACE_DEC_CXX,
      [
#if defined(__DECCXX)
          ACE_DEC_CXX
#endif
      ],
      [
       AC_DEFINE(DEC_CXX)
      ],)
    ;;
  *osf4.0*)
dnl We need to add checks for g++, DEC C++ and Rational C++
    AC_EGREP_CPP(ACE_DEC_CXX,
      [
#if defined(__DECCXX)
          ACE_DEC_CXX
#endif
      ],
      [
       AC_DEFINE(DEC_CXX)
      ],)
dnl Check for _POSIX_C_SOURCE macro
    AC_EGREP_CPP(ACE_ON_DEC_WITH_POS_SRC,
      [
       /* Include unistd.h to define _POSIX_C_SOURCE. */
#ifndef ACE_LACKS_UNISTD_H
# include <unistd.h>
#endif

#if defined(_POSIX_C_SOURCE) && (_POSIX_C_SOURCE >= 199506L)
          ACE_ON_DEC_WITH_POS_SRC
#endif
      ],
      [
       AC_DEFINE(DIGITAL_UNIX)
      ],)
    AC_DEFINE(ACE_DEFAULT_BASE_ADDR, ((char *) 0x80000000))
    AC_DEFINE(ACE_NEEDS_HUGE_THREAD_STACKSIZE, (1024 * 1024))
    AC_DEFINE(ACE_TIMER_SKEW, (1000 * 10))
    ;;
  *psos*)
    AC_DEFINE(ACE_PSOS)
    AC_DEFINE(ACE_PSOSIM)
    AC_DEFINE(ACE_PSOSTBD)
    dnl need ACE_HAS_TSS_EMULATION for ACE_DEFAULT_THREAD_KEYS!
    AC_EGREP_CPP(ACE_TSS_EMULATION,
      [
#if defined (ACE_HAS_TSS_EMULATION)
         ACE_TSS_EMULATION
#endif
      ], AC_DEFINE(ACE_DEFAULT_THREAD_KEYS, 256),)
    AC_DEFINE(ACE_MAIN, extern "C" void root)
    AC_DEFINE(ACE_MALLOC_ALIGN, 8)
    AC_DEFINE(ACE_USE_RCSID, 0)
    ;;
  *sco4.2*)
    AC_DEFINE(SCO)
    AC_DEFINE(ACE_DEFAULT_CLOSE_ALL_HANDLES, 0)
    ;;
  *sco5*)
    AC_DEFINE(SCO)
    AC_DEFINE(ACE_DEFAULT_CLOSE_ALL_HANDLES, 0)
    AC_DEFINE(ACE_HAS_BIG_FD_SET) dnl FIXME: We need a test for this!
    ;;
  *sunos4*)
    AC_DEFINE(ACE_TIMER_SKEW, (1000 * 10))
    ;;
  *solaris2.4*)
    AC_DEFINE(ACE_TIMER_SKEW, (1000 * 10))
    AC_DEFINE(ACE_NEEDS_LWP_PRIO_SET)
    ;;
  *solaris2.5*)
    AC_DEFINE(ACE_MALLOC_ALIGN, 8)
    AC_DEFINE(ACE_TIMER_SKEW, (1000 * 10))
    AC_DEFINE(ACE_NEEDS_LWP_PRIO_SET)
    ;;
  *solaris2.6*)
    AC_DEFINE(ACE_MALLOC_ALIGN, 8)
    AC_DEFINE(ACE_TIMER_SKEW, (1000 * 10))
    AC_DEFINE(ACE_NEEDS_LWP_PRIO_SET)
    ;;
  *solaris2.7*)
    AC_DEFINE(ACE_MALLOC_ALIGN, 8)
    AC_DEFINE(ACE_TIMER_SKEW, (1000 * 10))
    AC_DEFINE(ACE_NEEDS_LWP_PRIO_SET)
    ;;
  *86*solaris*)
    AC_DEFINE(ACE_HAS_X86_STAT_MACROS)
    AC_DEFINE(ACE_TIMER_SKEW, (1000 * 10))
    AC_DEFINE(ACE_NEEDS_LWP_PRIO_SET)
    ;;
  *tandem*)
    AC_DEFINE(ACE_TIMER_SKEW, (1000 * 10))
    ;;
  *unixware2.0*)
    AC_DEFINE(UNIXWARE)
    AC_DEFINE(UNIXWARE_2_0)
    ;;
  *unixware2.1*)
    AC_DEFINE(UNIXWARE)
    AC_DEFINE(UNIXWARE_2_1)
    ;;
  *vxworks*)
    AC_DEFINE(VXWORKS)
    AC_DEFINE(ACE_MAIN, ace_main)
    AC_DEFINE(ACE_DEFAULT_MAX_SOCKET_BUFSIZ, 32768)
    dnl need ACE_HAS_TSS_EMULATION for ACE_DEFAULT_THREAD_KEYS!
    AC_EGREP_CPP(ACE_TSS_EMULATION,
      [
#if defined (ACE_HAS_TSS_EMULATION)
         ACE_TSS_EMULATION
#endif
      ], AC_DEFINE(ACE_DEFAULT_THREAD_KEYS, 16),)
    AC_DEFINE(ACE_THR_PRI_FIFO_DEF, 101)
    AC_DEFINE(ACE_USE_RCSID, 0)
    ;;
  *cygwin32*)
    ;;
  *win32*)
    AC_DEFINE(ACE_WIN32)
    AC_DEFINE(ACE_UINT64_FORMAT_SPECIFIER, "%I64u")
dnl    AC_DEFINE(ACE_USE_SELECT_REACTOR_FOR_REACTOR_IMPL)
    if test "$ace_u_long_long_typedef_set" != yes; then
      ACE_UINT64="unsigned __int64"
      ace_u_long_long_typedef_set=yes
    fi  dnl "$ace_u_long_long_typedef_set" != yes
    ;;
  *)
    ;;
esac

dnl End ACE_SET_PLATFORM_MACROS
])

