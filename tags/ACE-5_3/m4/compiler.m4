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

 AC_REQUIRE([ACE_COMPILATION_OPTIONS])

 if test -n "$GXX"; then
dnl Temporarily change M4 quotes to prevent "regex []" from being eaten
changequote(, )dnl
   if $CXX --version | egrep -v '^2\.[0-7]' > /dev/null; then
changequote([, ])dnl
     :  # Do nothing
   else
     AC_DEFINE(ACE_HAS_GNUG_PRE_2_8)dnl
     AC_DEFINE(ACE_HAS_GNUC_BROKEN_TEMPLATE_INLINE_FUNCTIONS)dnl
   fi

   case `$CXX --version` in
     2.9* | 3*)
       if test "$ace_user_enable_exceptions" != yes; then
         ACE_CXXFLAGS="$ACE_CXXFLAGS -fcheck-new"
       fi
       ;;
   esac
 fi

 dnl Compiler Flag Key
 dnl    CXXFLAGS  - C++ flags to use during the configure script run and
 dnl                during ACE compilation.  The user may set this prior to
 dnl                running the configure script.  As such, it is important
 dnl                not to replace the existing value of CXXFLAGS; rather
 dnl                one should only add to it.
 dnl    ACE_CXXFLAGS
 dnl              - General C++ flags the configure script should set before
 dnl                CXXFLAGS to allow the user override them.
 dnl    DCXXFLAGS - C++ debugging flags
 dnl    OCXXFLAGS - C++ optimization flags
 dnl    CPPFLAGS  - C++ preprocessor flags
 dnl    ACE_CPPFLAGS
 dnl              - General C++ preprocessor flags the configure
 dnl                script should set before CPPFLAGS to allow the
 dnl                user override them.
 dnl    WERROR    - Compiler flag that converts warnings to errors

 if test -n "$GXX"; then
    WERROR="-Werror"
 fi

 case "$host" in
   *aix*)
     dnl In case anything here or in the config depends on OS
     dnl version number, grab it here and pass it all to the
     dnl compiler as well.
     AIX_MAJOR_VERS=`uname -v`
     AIX_MINOR_VERS=`uname -r`

     ACE_CPPFLAGS="$ACE_CPPFLAGS -DACE_AIX_MAJOR_VERS=$AIX_MAJOR_VERS"
     ACE_CPPFLAGS="$ACE_CPPFLAGS -DACE_AIX_MINOR_VERS=$AIX_MINOR_VERS"

     case "$CXX" in
       xlC*)
         dnl IBM C/C++ compiler 3.6.x produces a bazillion warnings about
         dnl 0-valued preprocessor defs. Since both 3.1 and 3.4 could
         dnl be installed, don't ask lslpp for one or the
         dnl other. Instead, compile a file and see which compiler the
         dnl user has set up for use. This trick was submitted by
         dnl Craig Rodrigues <rodrigc@mediaone.net>, originally from
         dnl the vacpp compiler newsgroup.  It relies on the
         dnl preprocessor defining __xlC__ to the proper version
         dnl number of the compiler.

         AC_EGREP_CPP(0x0306,
           [
            __xlC__
           ],
           [
            CXXFLAGS="$CXXFLAGS -qflag=e:e"
           ],
           [
            CXXFLAGS="$CXXFLAGS -qflag=w:w"
           ])
         ;;
     esac
     ;;
 esac

 case "$host" in
   *aix4.2* | *aix4.3*)
     case "$CXX" in
       xlC*)
         CXXFLAGS="$CXXFLAGS"
         ACE_CXXFLAGS="$ACE_CXXFLAGS "
         DCXXFLAGS="-g -qcheck=nobounds:div:null"
         OCXXFLAGS="-qarch=com"
         CPPFLAGS="$CPPFLAGS -qlanglvl=ansi"
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
         ACE_CXXFLAGS="$ACE_CXXFLAGS +W302,495,667,829"
         DCXXFLAGS="-g"
         OCXXFLAGS=""
         WERROR="+We67"
         # Warning 67: Invalid pragma name -- needed for ACE_LACKS_PRAGMA_ONCE
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

           dnl Inlining appears to cause link problems with early
           dnl releases of CC 5.0.
	   AC_DEFINE(ACE_LACKS_INLINE_FUNCTIONS)

           if test "$ace_user_enable_exceptions" != yes; then
             dnl See /opt/SUNWspro_5.0/SC5.0/include/CC/stdcomp.h.
             AC_DEFINE(_RWSTD_NO_EXCEPTIONS)
           fi
         fi

         CXXFLAGS="$CXXFLAGS"
         ACE_CXXFLAGS="$ACE_CXXFLAGS"
         DCXXFLAGS="$DCXXFLAGS -g"
         OCXXFLAGS="$OCXXFLAGS -O"
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

 dnl Additional flags
 if test -n "$GXX"; then
   ACE_CXXFLAGS="$ACE_CXXFLAGS -W -Wall -Wpointer-arith"
   if test "$ace_user_enable_repo" = no; then
     ACE_CXXFLAGS="$ACE_CXXFLAGS -fno-implicit-templates"
   fi
 fi

 if test -n "$GCC"; then
   ACE_CFLAGS="$ACE_CFLAGS -W -Wall -Wpointer-arith"
 fi
])
