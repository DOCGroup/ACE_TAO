dnl aclocal.m4 generated automatically by aclocal 1.4

dnl Copyright (C) 1994, 1995-8, 1999 Free Software Foundation, Inc.
dnl This file is free software; the Free Software Foundation
dnl gives unlimited permission to copy and/or distribute it,
dnl with or without modifications, as long as this notice is preserved.

dnl This program is distributed in the hope that it will be useful,
dnl but WITHOUT ANY WARRANTY, to the extent permitted by law; without
dnl even the implied warranty of MERCHANTABILITY or FITNESS FOR A
dnl PARTICULAR PURPOSE.

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


# serial 35 AC_PROG_LIBTOOL
AC_DEFUN(AC_PROG_LIBTOOL,
[AC_REQUIRE([AC_LIBTOOL_SETUP])dnl

# Save cache, so that ltconfig can load it
AC_CACHE_SAVE

# Actually configure libtool.  ac_aux_dir is where install-sh is found.
CC="$CC" CFLAGS="$CFLAGS" CPPFLAGS="$CPPFLAGS" \
LD="$LD" NM="$NM" RANLIB="$RANLIB" LN_S="$LN_S" \
DLLTOOL="$DLLTOOL" AS="$AS" \
${CONFIG_SHELL-/bin/sh} $ac_aux_dir/ltconfig --no-reexec \
$libtool_flags --no-verify $ac_aux_dir/ltmain.sh $host \
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
AC_REQUIRE([AC_PROG_RANLIB])dnl
AC_REQUIRE([AC_PROG_CC])dnl
AC_REQUIRE([AC_PROG_LD])dnl
AC_REQUIRE([AC_PROG_NM])dnl
AC_REQUIRE([AC_SYS_NM_PARSE])dnl
AC_REQUIRE([AC_SYS_SYMBOL_UNDERSCORE])dnl
AC_REQUIRE([AC_PROG_LN_S])dnl
dnl

# Check for any special flags to pass to ltconfig.
libtool_flags="--cache-file=$cache_file"
test "$enable_shared" = no && libtool_flags="$libtool_flags --disable-shared"
test "$enable_static" = no && libtool_flags="$libtool_flags --disable-static"
test "$enable_fast_install" = no && libtool_flags="$libtool_flags --disable-fast-install"
test "$lt_dlopen" = yes && libtool_flags="$libtool_flags --enable-dlopen"
test "$silent" = yes && libtool_flags="$libtool_flags --silent"
test "$ac_cv_prog_gcc" = yes && libtool_flags="$libtool_flags --with-gcc"
test "$ac_cv_prog_gnu_ld" = yes && libtool_flags="$libtool_flags --with-gnu-ld"

# Some flags need to be propagated to the compiler or linker for good
# libtool support.
case "$host" in
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

*-*-cygwin*)
  AC_SYS_LIBTOOL_CYGWIN
  ;;

esac

# enable the --disable-libtool-lock switch

AC_ARG_ENABLE(libtool-lock,
[  --disable-libtool-lock  force libtool not to do file locking],
need_locks=$enableval,
need_locks=yes)

if test x"$need_locks" = xno; then
  libtool_flags="$libtool_flags --disable-lock"
fi
])

# AC_LIBTOOL_DLOPEN - check for dlopen support
AC_DEFUN(AC_LIBTOOL_DLOPEN, [lt_dlopen=yes])

# AC_ENABLE_SHARED - implement the --enable-shared flag
# Usage: AC_ENABLE_SHARED[(DEFAULT)]
#   Where DEFAULT is either `yes' or `no'.  If omitted, it defaults to
#   `yes'.
AC_DEFUN(AC_ENABLE_SHARED,
[define([AC_ENABLE_SHARED_DEFAULT], ifelse($1, no, no, yes))dnl
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
AC_DEFUN(AC_DISABLE_SHARED,
[AC_ENABLE_SHARED(no)])

# AC_ENABLE_STATIC - implement the --enable-static flag
# Usage: AC_ENABLE_STATIC[(DEFAULT)]
#   Where DEFAULT is either `yes' or `no'.  If omitted, it defaults to
#   `yes'.
AC_DEFUN(AC_ENABLE_STATIC,
[define([AC_ENABLE_STATIC_DEFAULT], ifelse($1, no, no, yes))dnl
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
AC_DEFUN(AC_DISABLE_STATIC,
[AC_ENABLE_STATIC(no)])


# AC_ENABLE_FAST_INSTALL - implement the --enable-fast-install flag
# Usage: AC_ENABLE_FAST_INSTALL[(DEFAULT)]
#   Where DEFAULT is either `yes' or `no'.  If omitted, it defaults to
#   `yes'.
AC_DEFUN(AC_ENABLE_FAST_INSTALL,
[define([AC_ENABLE_FAST_INSTALL_DEFAULT], ifelse($1, no, no, yes))dnl
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
AC_DEFUN(AC_DISABLE_FAST_INSTALL,
[AC_ENABLE_FAST_INSTALL(no)])


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
  ac_prog=`($CC -print-prog-name=ld) 2>&5`
  case "$ac_prog" in
    # Accept absolute paths.
changequote(,)dnl
    /* | [A-Za-z]:[\\/]*)
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
  IFS="${IFS= 	}"; ac_save_ifs="$IFS"; IFS="${IFS}:"
  for ac_dir in $PATH; do
    test -z "$ac_dir" && ac_dir=.
    if test -f "$ac_dir/$ac_prog"; then
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
AC_SUBST(LD)
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
])

# AC_PROG_NM - find the path to a BSD-compatible name lister
AC_DEFUN(AC_PROG_NM,
[AC_MSG_CHECKING([for BSD-compatible nm])
AC_CACHE_VAL(ac_cv_path_NM,
[if test -n "$NM"; then
  # Let the user override the test.
  ac_cv_path_NM="$NM"
else
  IFS="${IFS= 	}"; ac_save_ifs="$IFS"; IFS="${IFS}:"
  for ac_dir in $PATH /usr/ccs/bin /usr/ucb /bin; do
    test -z "$ac_dir" && ac_dir=.
    if test -f $ac_dir/nm; then
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
AC_SUBST(NM)
])

# AC_SYS_NM_PARSE - Check for command to grab the raw symbol name followed
# by C symbol name from nm.
AC_DEFUN(AC_SYS_NM_PARSE,
[AC_REQUIRE([AC_CANONICAL_HOST])dnl
AC_REQUIRE([AC_PROG_NM])dnl
# Check for command to grab the raw symbol name followed by C symbol from nm.
AC_MSG_CHECKING([command to parse $NM output])
AC_CACHE_VAL(ac_cv_sys_global_symbol_pipe,
[# These are sane defaults that work on at least a few old systems.
# {They come from Ultrix.  What could be older than Ultrix?!! ;)}

changequote(,)dnl
# Character class describing NM global symbol codes.
ac_symcode='[BCDEGRST]'

# Regexp to match symbols that can be accessed directly from C.
ac_sympat='\([_A-Za-z][_A-Za-z0-9]*\)'

# Transform the above into a raw symbol and a C symbol.
ac_symxfrm='\1 \2\3 \3'

# Transform an extracted symbol line into a proper C declaration
ac_global_symbol_to_cdecl="sed -n -e 's/^. .* \(.*\)$/extern char \1;/p'"

# Define system-specific variables.
case "$host_os" in
aix*)
  ac_symcode='[BCDT]'
  ;;
cygwin* | mingw*)
  ac_symcode='[ABCDGISTW]'
  ;;
hpux*)
  ac_global_symbol_to_cdecl="sed -n -e 's/^T .* \(.*\)$/extern char \1();/p' -e 's/^. .* \(.*\)$/extern char \1;/p'"
  ;;
irix*)
  ac_symcode='[BCDEGRST]'
  ;;
solaris*)
  ac_symcode='[BDT]'
  ;;
esac

# If we're using GNU nm, then use its standard symbol codes.
if $NM -V 2>&1 | egrep '(GNU|with BFD)' > /dev/null; then
  ac_symcode='[ABCDGISTW]'
fi
changequote([,])dnl

# Try without a prefix undercore, then with it.
for ac_symprfx in "" "_"; do

  ac_cv_sys_global_symbol_pipe="sed -n -e 's/^.*[ 	]\($ac_symcode\)[ 	][ 	]*\($ac_symprfx\)$ac_sympat$/$ac_symxfrm/p'"

  # Check to see that the pipe works correctly.
  ac_pipe_works=no
  rm -f conftest.$ac_ext
  cat > conftest.$ac_ext <<EOF
#ifdef __cplusplus
extern "C" {
#endif
char nm_test_var;
void nm_test_func(){}
#ifdef __cplusplus
}
#endif
int main(){nm_test_var='a';nm_test_func;return 0;}
EOF

  if AC_TRY_EVAL(ac_compile); then
    # Now try to grab the symbols.
    ac_nlist=conftest.nm
  
    if AC_TRY_EVAL(NM conftest.$ac_objext \| $ac_cv_sys_global_symbol_pipe \> $ac_nlist) && test -s "$ac_nlist"; then

      # Try sorting and uniquifying the output.
      if sort "$ac_nlist" | uniq > "$ac_nlist"T; then
	mv -f "$ac_nlist"T "$ac_nlist"
      else
	rm -f "$ac_nlist"T
      fi

      # Make sure that we snagged all the symbols we need.
      if egrep ' nm_test_var$' "$ac_nlist" >/dev/null; then
	if egrep ' nm_test_func$' "$ac_nlist" >/dev/null; then
	  cat <<EOF > conftest.c
#ifdef __cplusplus
extern "C" {
#endif

EOF
	  # Now generate the symbol file.
	  eval "$ac_global_symbol_to_cdecl"' < "$ac_nlist" >> conftest.c'

	  cat <<EOF >> conftest.c
#if defined (__STDC__) && __STDC__
# define lt_ptr_t void *
#else
# define lt_ptr_t char *
# define const
#endif

/* The mapping between symbol names and symbols. */
const struct {
  const char *name;
  lt_ptr_t address;
}
changequote(,)dnl
lt_preloaded_symbols[] =
changequote([,])dnl
{
EOF
	sed 's/^. \(.*\) \(.*\)$/  {"\2", (lt_ptr_t) \&\2},/' < "$ac_nlist" >> conftest.c
	cat <<\EOF >> conftest.c
  {0, (lt_ptr_t) 0}
};

#ifdef __cplusplus
}
#endif
EOF
	  # Now try linking the two files.
	  mv conftest.$ac_objext conftestm.$ac_objext
	  ac_save_LIBS="$LIBS"
	  ac_save_CFLAGS="$CFLAGS"
	  LIBS="conftestm.$ac_objext"
	  CFLAGS="$CFLAGS$no_builtin_flag"
	  if AC_TRY_EVAL(ac_link) && test -s conftest; then
	    ac_pipe_works=yes
	  else
	    echo "configure: failed program was:" >&AC_FD_CC
	    cat conftest.c >&AC_FD_CC
	  fi
	  LIBS="$ac_save_LIBS"
	  CFLAGS="$ac_save_CFLAGS"
	else
	  echo "cannot find nm_test_func in $ac_nlist" >&AC_FD_CC
	fi
      else
	echo "cannot find nm_test_var in $ac_nlist" >&AC_FD_CC
      fi
    else
      echo "cannot run $ac_cv_sys_global_symbol_pipe" >&AC_FD_CC
    fi
  else
    echo "$progname: failed program was:" >&AC_FD_CC
    cat conftest.c >&AC_FD_CC
  fi
  rm -rf conftest*

  # Do not use the global_symbol_pipe unless it works.
  if test "$ac_pipe_works" = yes; then
    if test x"$ac_symprfx" = x"_"; then
      ac_cv_sys_symbol_underscore=yes
    else
      ac_cv_sys_symbol_underscore=no
    fi
    break
  else
    ac_cv_sys_global_symbol_pipe=
  fi
done
])

ac_result=yes
if test -z "$ac_cv_sys_global_symbol_pipe"; then
   ac_result=no
fi
AC_MSG_RESULT($ac_result)
])

# AC_SYS_LIBTOOL_CYGWIN - find tools needed on cygwin
AC_DEFUN(AC_SYS_LIBTOOL_CYGWIN,
[AC_CHECK_TOOL(DLLTOOL, dlltool, false)
AC_CHECK_TOOL(AS, as, false)
])

# AC_SYS_SYMBOL_UNDERSCORE - does the compiler prefix global symbols
#                            with an underscore?
AC_DEFUN(AC_SYS_SYMBOL_UNDERSCORE,
[AC_REQUIRE([AC_PROG_NM])dnl
AC_REQUIRE([AC_SYS_NM_PARSE])dnl
AC_MSG_CHECKING([for _ prefix in compiled symbols])
AC_CACHE_VAL(ac_cv_sys_symbol_underscore,
[ac_cv_sys_symbol_underscore=no
cat > conftest.$ac_ext <<EOF
void nm_test_func(){}
int main(){nm_test_func;return 0;}
EOF
if AC_TRY_EVAL(ac_compile); then
  # Now try to grab the symbols.
  ac_nlist=conftest.nm
  if AC_TRY_EVAL(NM conftest.$ac_objext \| $ac_cv_sys_global_symbol_pipe \> $ac_nlist) && test -s "$ac_nlist"; then
    # See whether the symbols have a leading underscore.
    if egrep '^. _nm_test_func' "$ac_nlist" >/dev/null; then
      ac_cv_sys_symbol_underscore=yes
    else
      if egrep '^. nm_test_func ' "$ac_nlist" >/dev/null; then
	:
      else
	echo "configure: cannot find nm_test_func in $ac_nlist" >&AC_FD_CC
      fi
    fi
  else
    echo "configure: cannot run $ac_cv_sys_global_symbol_pipe" >&AC_FD_CC
  fi
else
  echo "configure: failed program was:" >&AC_FD_CC
  cat conftest.c >&AC_FD_CC
fi
rm -rf conftest*
])
AC_MSG_RESULT($ac_cv_sys_symbol_underscore)
USE_SYMBOL_UNDERSCORE=${ac_cv_sys_symbol_underscore=no}
AC_SUBST(USE_SYMBOL_UNDERSCORE)dnl
])

# AC_CHECK_LIBM - check for math library
AC_DEFUN(AC_CHECK_LIBM,
[AC_REQUIRE([AC_CANONICAL_HOST])dnl
case "$host" in
*-*-beos* | *-*-cygwin*)
  # These system don't have libm
  ;;
*-ncr-sysv4.3*)
  AC_CHECK_LIB(mw, _mwvalidcheckl)
  AC_CHECK_LIB(m, cos)
  ;;
*)
  AC_CHECK_LIB(m, cos)
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
AC_DEFUN(AC_LIBLTDL_CONVENIENCE, [
  case "$enable_ltdl_convenience" in
  no) AC_MSG_ERROR([this package needs a convenience libltdl]) ;;
  "") enable_ltdl_convenience=yes
      ac_configure_args="$ac_configure_args --enable-ltdl-convenience" ;;
  esac
  LIBLTDL=ifelse($#,1,$1,['${top_builddir}/libltdl'])/libltdlc.la
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
AC_DEFUN(AC_LIBLTDL_INSTALLABLE, [
  AC_CHECK_LIB(ltdl, main, LIBLTDL="-lltdl", [
    case "$enable_ltdl_install" in
    no) AC_MSG_WARN([libltdl not installed, but installation disabled]) ;;
    "") enable_ltdl_install=yes
        ac_configure_args="$ac_configure_args --enable-ltdl-install" ;;
    esac
  ])
  if test x"$enable_ltdl_install" != x"no"; then
    LIBLTDL=ifelse($#,1,$1,['${top_builddir}/libltdl'])/libltdl.la
  fi
])

dnl old names
AC_DEFUN(AM_PROG_LIBTOOL, [indir([AC_PROG_LIBTOOL])])dnl
AC_DEFUN(AM_ENABLE_SHARED, [indir([AC_ENABLE_SHARED], $@)])dnl
AC_DEFUN(AM_ENABLE_STATIC, [indir([AC_ENABLE_STATIC], $@)])dnl
AC_DEFUN(AM_DISABLE_SHARED, [indir([AC_DISABLE_SHARED], $@)])dnl
AC_DEFUN(AM_DISABLE_STATIC, [indir([AC_DISABLE_STATIC], $@)])dnl
AC_DEFUN(AM_PROG_LD, [indir([AC_PROG_LD])])dnl
AC_DEFUN(AM_PROG_NM, [indir([AC_PROG_NM])])dnl
AC_DEFUN(AM_SYS_NM_PARSE, [indir([AC_SYS_NM_PARSE])])dnl
AC_DEFUN(AM_SYS_SYMBOL_UNDERSCORE, [indir([AC_SYS_SYMBOL_UNDERSCORE])])dnl
AC_DEFUN(AM_SYS_LIBTOOL_CYGWIN, [indir([AC_SYS_LIBTOOL_CYGWIN])])dnl

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
              [  --enable-lib-all       build all ACE components         [default=yes]],
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
               ace_user_enable_lib_all=yes
              ])

AC_ARG_ENABLE(lib-full,
              [  --enable-lib-full      build the full ACE library       [default=yes]],
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

dnl -------------------------------------------------------------------------
dnl       $Id$
dnl 
dnl       ACE M4 include file which contains ACE specific M4 macros
dnl       to be used by the ACE configure script.
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

