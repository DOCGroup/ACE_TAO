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
