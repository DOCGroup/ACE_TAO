// -*- C++ -*-
// $Id$

//
// The following configuration file is designed to work for win32
// platforms using gcc/g++ with mingw32 (http://www.mingw.org).
//

#ifndef ACE_CONFIG_WIN32_MINGW_H
#define ACE_CONFIG_WIN32_MINGW_H
#include "ace/pre.h"

#ifndef ACE_CONFIG_WIN32_H
#  error Use config-win32.h in config.h instead of this header
#endif /* ACE_CONFIG_WIN32_H */

#define ACE_CC_NAME ACE_LIB_TEXT ("g++")
#define ACE_CC_PREPROCESSOR "cpp"
#define ACE_CC_PREPROCESOR_ARGS ""

// Why all this is not in config-g++-common.h?
#define ACE_CC_MAJOR_VERSION __GNUC__
#define ACE_CC_MINOR_VERSION __GNUC_MINOR__
#define ACE_CC_BETA_VERSION (0)

#if !defined(__MINGW32__)
#  error You do not seem to be using mingw32
#endif

#include "ace/config-g++-common.h"

#include /**/ <_mingw.h>
#include /**/ <w32api.h>

#define ACE_LACKS_MODE_MASKS
#define ACE_HAS_USER_MODE_MASKS

#if (__MINGW32_MAJOR_VERSION < 2)
#  error You need a newer version (>= 2.0) of mingw32/w32api
#endif

#define ACE_LACKS_STRRECVFD
#define ACE_LACKS_NATIVE_STRPTIME
#define ACE_HAS_STRERROR
#define ACE_HAS_STRPTIME
#define ACE_LACKS_DIRENT_H
#define ACE_LACKS_UCONTEXT_H
#define ACE_LACKS_SYS_SELECT_H
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
#define ACE_LACKS_STROPTS_H
#define ACE_LACKS_SYS_IOCTL_H

// We trust the user: He must have used -mpentiumpro or -mpentium
// if that is what he wants.
#if defined(pentiumpro) || defined(pentium)
# define ACE_HAS_PENTIUM
#endif

#if !defined (ACE_HAS_WINNT4)
# if (defined (WINNT) && WINNT == 1) \
     || (defined (__WINNT__) && __WINNT__ == 1)
#  define ACE_HAS_WINNT4 1
# else
#  define ACE_HAS_WINNT4 0
# endif
#endif

#define ACE_DLL_PREFIX ACE_LIB_TEXT ("lib")

#define ACE_ENDTHREADEX(STATUS)  ::_endthreadex ((DWORD) (STATUS))

#include "ace/post.h"
#endif /* ACE_CONFIG_WIN32_MINGW_H */
