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

dnl  Copyright (C) 1998, 1999, 2000  Ossama Othman
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
dnl $WERROR is set in the ACE_SET_COMPILER_FLAGS macro.
 AC_REQUIRE([ACE_SET_COMPILER_FLAGS])dnl

dnl Some tests may pass because the compiler issues warnings
dnl instead of errors when errors should occur.  This macro converts
dnl warnings to errors when executing the action/test passed to this
dnl macro so that action/test fails when it is supposed to fail; at
dnl least that is the idea.

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
dnl Some compilers don't like the "struct" but we need the struct for
dnl some platforms to resolve ambiguities between functions and
dnl structures with with the same name.  So, we try the same test but
dnl without "struct" if the above test with "struct" fails.  If both
dnl tests fail, then we can be reasonably sure that we don't have the
dnl structure we are testing for.
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


dnl Check for 64 bit llseek() or lseek64()
dnl Usage: ACE_CHECK_LSEEK64
AC_DEFUN([ACE_CHECK_LSEEK64],
[
 AC_CHECK_FUNC([lseek64],
   [
    AC_DEFINE([ACE_HAS_LSEEK64])

    dnl Check for 64 bit offset type in the lseek64() prototype, if it
    dnl exists.
    dnl ACE_CHECK_OFF64_T([lseek64])

    dnl Check if _LARGEFILE64_SOURCE macro is needed to make the
    dnl lseek64() prototype visible, or if the prototype itself is missing.
    ACE_CACHE_CHECK([for lseek64 prototype],
      [ace_cv_lib_has_lseek64_prototype],
      [
       ace_save_CPPFLAGS="$CPPFLAGS"
       ace_no_largefile64="-U_LARGEFILE64_SOURCE"
       CPPFLAGS="$CPPFLAGS $ace_no_largefile64"
       AC_EGREP_HEADER([[^_]+lseek64], unistd.h,
         [
          ace_cv_lib_has_lseek64_prototype=yes
         ],
         [
          ace_cv_lib_has_lseek64_prototype=no
         ])
       dnl Reset the compiler flags
       CPPFLAGS="$ace_save_CPPFLAGS"
      ],, AC_DEFINE(ACE_LACKS_LSEEK64_PROTOTYPE))
   ],
   [
    AC_CHECK_FUNC([llseek],
      [
       AC_DEFINE([ACE_HAS_LLSEEK])
       dnl Check if _LARGEFILE64_SOURCE macro is needed to make the
       dnl llseek() prototype visible, or if the prototype itself is
       dnl missing.

       dnl Check for 64 bit offset type in the llseek() prototype, if
       dnl it exists.
       dnl ACE_CHECK_OFF64_T([llseek])

       ACE_CACHE_CHECK([for llseek prototype],
         [ace_cv_lib_has_llseek_prototype],
         [
          ace_save_CPPFLAGS="$CPPFLAGS"
          ace_no_largefile64="-U_LARGEFILE64_SOURCE"
          CPPFLAGS="$CPPFLAGS $ace_no_largefile64"
          AC_EGREP_HEADER([[^_]+llseek], unistd.h,
            [
             ace_cv_lib_has_llseek_prototype=no
            ],
            [
             ace_cv_lib_has_llseek_prototype=yes
            ],)
          dnl Reset the compiler flags
          CPPFLAGS="$ace_save_CPPFLAGS"
         ],, AC_DEFINE(ACE_LACKS_LLSEEK_PROTOTYPE))


      ],)
   ])
])

dnl Check what the 64 bit offset type is by checking what the offset
dnl argument for llseek()/lseek64() is.
dnl Usage: ACE_CHECK_LOFF_64(LSEEK64-FUNC)
AC_DEFUN([ACE_CHECK_OFF64_T],
[
  AC_MSG_CHECKING([for 64 bit offset type])
  AC_EGREP_HEADER([[ ]+$1.*\(.*], unistd.h,
    [
     cat > conftest.$ac_ext <<EOF
#include "confdefs.h"

/* Make sure 64 bit file feature test macro is defined. */
#ifndef _LARGEFILE64_SOURCE
# define _LARGEFILE64_SOURCE
#endif

#ifndef ACE_LACKS_UNISTD_H
# include <unistd.h>  /* needed for lseek64()/llseek() prototype */
#endif
EOF

changequote(, )dnl
dnl Here we attempt to determine the type of the second argument of
dnl lseek64()/llseek() from its prototype.
     ace_off64_t=`eval "$ac_cpp conftest.$ac_ext" | \
       egrep '[ ]+lseek64.*\(.*' | \
       sed -e 's/^.*(.*,[ ]*\(.*\) .*,.*$/\1/'`
changequote([, ])dnl


if test -n "$ace_off64_t"; then
     AC_MSG_RESULT([$ace_off64_t])
     AC_DEFINE_UNQUOTED([ACE_LOFF_T_TYPEDEF], [$ace_off64_t])
fi

     rm -rf conftest*

dnl Do not remove this parenthesis --> )
dnl It's only purpose is to keep Emacs from getting confused about
dnl mismatched parentheses.
    ],
    [
     AC_MSG_RESULT([no])
    ])
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

