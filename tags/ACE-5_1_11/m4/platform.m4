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

dnl Check for thread related libraries and compiler flags
dnl Usage: ACE_SET_PLATFORM_MACROS
AC_DEFUN(ACE_SET_PLATFORM_MACROS, dnl
[
dnl Begin ACE_SET_PLATFORM_MACROS

dnl At some point the below platform specific settings should be
dnl automated as much as possible!  We set things manually just to get
dnl things going with the auto{conf,make}/libtool integration into the
dnl ACE source tree.

dnl Platform specific flags
case "$host" in
  *aix3*)
    AC_DEFINE(AIX)
    AC_DEFINE(ACE_DEFAULT_BASE_ADDR, ((char *) 0x80000000))
    ;;
  *aix4.1*)
    AC_DEFINE(AIX)
    dnl Use BSD 4.4 socket definitions for pre-AIX 4.2.  The _BSD
    dnl setting also controls the data type used for waitpid(),
    dnl wait(), and wait3().
    ACE_CPPFLAGS="$ACE_CPPFLAGS -D_BSD=44"
    dnl pre-AIX 4.3 requires _BSD_INCLUDES
    ACE_CPPFLAGS="$ACE_CPPFLAGS -D_BSD_INCLUDES"
    AC_DEFINE(ACE_DEFAULT_BASE_ADDR, ((char *) 0x80000000))
    AC_DEFINE(ACE_HAS_AIX_BROKEN_SOCKET_HEADER)
    ;;
  *aix4.2*)
    AC_DEFINE(AIX)
    dnl pre-AIX 4.3 requires _BSD_INCLUDES
    ACE_CPPFLAGS="$ACE_CPPFLAGS -D_BSD_INCLUDES"
    AC_DEFINE(ACE_DEFAULT_BASE_ADDR, ((char *) 0x80000000))
dnl    AC_DEFINE(ACE_HAS_AIX_BROKEN_SOCKET_HEADER)
    AC_DEFINE(ACE_TLI_TCP_DEVICE, "/dev/xti/tcp")
    ;;
  *aix*)
    AC_DEFINE(AIX)
    ;;
  t3e-cray-unicosmk*)
    ACE_CPPFLAGS="$ACE_CPPFLAGS -D_CRAYMPP -D_CRAYT3E -D_UNICOS"
    ;;
  t3e-cray*)
    ACE_CPPFLAGS="$ACE_CPPFLAGS -D_CRAYT3E -D_UNICOS"
    ;;
  *cray-unicos*)
    ACE_CPPFLAGS="$ACE_CPPFLAGS -D_UNICOS"
    ;;
  *dgux4.11*)
    AC_DEFINE(ACE_DGUX)
    AC_DEFINE(IP_ADD_MEMBERSHIP, 0x13)
    AC_DEFINE(IP_DROP_MEMBERSHIP, 0x14)
    ACE_CPPFLAGS="$ACE_CPPFLAGS -D_POSIX_SOURCE -D_DGUX_SOURCE"
    ;;
  *dgux4*)
    AC_DEFINE(ACE_DGUX)
    AC_DEFINE(IP_ADD_MEMBERSHIP, 0x13)
    AC_DEFINE(IP_DROP_MEMBERSHIP, 0x14)
    ACE_CPPFLAGS="$ACE_CPPFLAGS -D_POSIX4A_DRAFT10_SOURCE -D_POSIX4_DRAFT_SOURCE"
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
    ACE_CPPFLAGS="$ACE_CPPFLAGS -D_HPUX_SOURCE"
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
    if test -z "$GXX"; then
      ACE_CPPFLAGS="$ACE_CPPFLAGS -D_BSD_TYPES"
    fi
    ;;
  *irix6*)
    AC_DEFINE(IRIX6)
    AC_DEFINE(ACE_DEFAULT_BASE_ADDR, ((char *) (1024U * 1024 * 1024)))
    AC_DEFINE(ACE_TIMER_SKEW, (1000 * 10))
    ACE_CPPFLAGS="$ACE_CPPFLAGS -D_SGI_MP_SOURCE -D_MODERN_C_"

    case "$host" in
      *irix6.2*)
        dnl Recent versions of IRIX do not appear to require this macro.
        if test "$ace_user_enable_threads" = yes; then
          AC_DEFINE(ACE_HAS_IRIX62_THREADS)
        fi
        ;;
    esac
    ;;
  *linux*)
    AC_DEFINE(ACE_DEFAULT_MAX_SOCKET_BUFSIZ, 65535)
    AC_DEFINE(ACE_DEFAULT_BASE_ADDR, ((char *) 0x80000000))
    AC_DEFINE(ACE_HAS_BIG_FD_SET) dnl FIXME: We need a test for this!
    AC_DEFINE(ACE_UINT64_FORMAT_SPECIFIER, "%Lu")
    AC_DEFINE(ACE_TIMER_SKEW, (1000 * 10))
    ;;
  *lynxos*)
    AC_DEFINE(_POSIX_THREADS_CALLS)
    AC_DEFINE(__NO_INCLUDE_WARN__)
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
    ACE_CPPFLAGS="$ACE_CPPFLAGS -D_ALL_SOURCE"
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
    ACE_CPPFLAGS="$ACE_CPPFLAGS -D_SVID3"
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
    AC_DEFINE(CYGWIN32)
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
  *qnx* | *nto* | *neutrino*)
    dnl These should be defined on the command line, not in config.h.
    ACE_CPPFLAGS="$ACE_CPPFLAGS -D_QNX_SOURCE -D_POSIX_C_SOURCE=199506"
    ACE_CPPFLAGS="$ACE_CPPFLAGS -D_POSIX_NAME_MAX=14"  # Max bytes in a
                                                       # filename
    ACE_CPPFLAGS="$ACE_CPPFLAGS -D_POSIX_PATH_MAX=256" # Num. bytes in
                                                       # pathname (excl. NULL)
    case "$host" in
changequote(, )dnl
      i[3456]86*)
changequote([, ])dnl
        if test -n "$GXX"; then
          # Neutrino defines memcpy as a macro on x86, which then
          # hoses the ACE_OS::memcpy() method.  Undefining
          # __OPTIMIZE__ prevents this from happening.
          ACE_CPPFLAGS="$ACE_CPPFLAGS -U__OPTIMIZE__"
        fi
        ;;
    esac
    ;;
  *)
    ;;
esac

dnl End ACE_SET_PLATFORM_MACROS
])
