// -*- C++ -*-
//$Id$

// The following configuration file contains defines for Borland compilers.

#ifndef ACE_CONFIG_WIN32_BORLAND_H
#define ACE_CONFIG_WIN32_BORLAND_H
#include /**/ "ace/pre.h"

#ifndef ACE_CONFIG_WIN32_H
#error Use config-win32.h in config.h instead of this header
#endif /* ACE_CONFIG_WIN32_H */

#include "config-borland-common.h"

// Automatically define WIN32 macro if the compiler tells us it is our
// target platform.
# if defined (__WIN32__) && !defined (WIN32)
#  define WIN32 1
# endif

// When building a VCL application, the main VCL header file should be
// included before anything else. You can define ACE_HAS_VCL=1 in your
// project settings to have this file included for you automatically.
# if defined (ACE_HAS_VCL) && (ACE_HAS_VCL != 0)
#  include /**/ <vcl.h>
# endif

# define ACE_CC_PREPROCESSOR "CPP32.EXE"

# include "ace/config-win32-common.h"

# define ACE_STRCASECMP_EQUIVALENT ::stricmp
# define ACE_STRNCASECMP_EQUIVALENT ::strnicmp
# define ACE_WCSDUP_EQUIVALENT ::_wcsdup

# define ACE_LACKS_MODE_MASKS 1
# define ACE_WSTRING_HAS_USHORT_SUPPORT 1
# define ACE_HAS_ITOA 1
# define ACE_HAS_DIRENT

#ifdef ACE_USES_STD_NAMESPACE_FOR_STDC_LIB
#undef ACE_USES_STD_NAMESPACE_FOR_STDC_LIB
#define ACE_USES_STD_NAMESPACE_FOR_STDC_LIB 1
#endif

#define ACE_NEEDS_DL_UNDERSCORE

#define ACE_LACKS_TERMIOS_H
#define ACE_LACKS_NETINET_TCP_H
#define ACE_LACKS_REGEX_H
#define ACE_LACKS_SYS_MSG_H
#define ACE_LACKS_PWD_H
#define ACE_LACKS_POLL_H
#define ACE_LACKS_SYS_SHM_H
#define ACE_LACKS_STRINGS_H
#define ACE_LACKS_SEMAPHORE_H
#define ACE_LACKS_INTTYPES_H
#define ACE_LACKS_UCONTEXT_H
#define ACE_LACKS_SYS_SELECT_H
#define ACE_LACKS_SYS_TIME_H
#define ACE_LACKS_SYS_RESOURCE_H
#define ACE_LACKS_SYS_WAIT_H
#define ACE_LACKS_DLFCN_H
#define ACE_LACKS_SYS_MMAN_H
#define ACE_LACKS_SYS_UIO_H
#define ACE_LACKS_SYS_SOCKET_H
#define ACE_LACKS_NETINET_IN_H
#define ACE_LACKS_NETDB_H
#define ACE_LACKS_NET_IF_H
#define ACE_LACKS_SYS_IPC_H
#define ACE_LACKS_SYS_SEM_H
#define ACE_LACKS_STDINT_H
#define ACE_LACKS_DIRENT_H
#define ACE_LACKS_SYS_IOCTL_H
#define ACE_LACKS_STROPTS_H

#define ACE_LACKS_STRRECVFD

#if defined(__BORLANDC__) && (__BORLANDC__ >= 0x600)
# define ACE_LACKS_PTRDIFF_T
# define ACE_PTRDIFF_T_TYPE std::ptrdiff_t
#endif

#if defined(__BORLANDC__) && (__BORLANDC__ < 0x600)
#define ACE_USES_EXPLICIT_STD_NAMESPACE
#endif

#include /**/ "ace/post.h"
#endif /* ACE_CONFIG_WIN32_BORLAND_H */
