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
