//=============================================================================
/**
 *  @file   config-win32-msvc.h
 *
 *  $Id$
 *
 *  @brief  Microsoft Visual C++ configuration file.
 *
 *  This file is the ACE configuration file for Microsoft Visual C++ versions
 *  5.0, 6.0, and 7.0 (.NET)
 *
 *  @author Darrell Brunsch <brunsch@cs.wustl.edu>
 */
//=============================================================================

#ifndef ACE_CONFIG_WIN32_MSVC_H
#define ACE_CONFIG_WIN32_MSVC_H
#include "ace/pre.h"

#ifndef ACE_CONFIG_WIN32_H
#error Use config-win32.h in config.h instead of this header
#endif /* ACE_CONFIG_WIN32_H */

#define ACE_CC_NAME ACE_LIB_TEXT ("Visual C++")
#define ACE_CC_PREPROCESSOR "CL.EXE"
#define ACE_CC_PREPROCESSOR_ARGS "-nologo -E"

#define ACE_CC_MAJOR_VERSION (_MSC_VER / 100 - 6)
#define ACE_CC_MINOR_VERSION (_MSC_VER % 100)
#define ACE_CC_BETA_VERSION (0)

// Support "decorated" library names for MSVC 6 or better.
#if (_MSC_VER >= 1200)
# if !defined (ACE_LD_DECORATOR_STR)
#  if defined (_DEBUG)
#   if ACE_HAS_MFC == 1
#    define ACE_LD_DECORATOR_STR ACE_LIB_TEXT ("mfcd")
#   else
#    define ACE_LD_DECORATOR_STR ACE_LIB_TEXT ("d")
#   endif  /* ACE_HAS_MFC */
#  else  /* _NDEBUG */
#   if ACE_HAS_MFC == 1
#    define ACE_LD_DECORATOR_STR ACE_LIB_TEXT ("mfc")
#   endif  /* ACE_HAS_MFC */
#  endif  /* _DEBUG */
# endif  /* ACE_LD_DECORATOR_STR */
#endif  /* _MSC_VER >= 1200 */

#if (_MSC_VER >= 1300)
# include "ace/config-win32-msvc-7.h"
#elif (_MSC_VER >= 1200)
# include "ace/config-win32-msvc-6.h"
#elif (_MSC_VER >= 1100)
# include "ace/config-win32-msvc-5.h"
#else
# error This version of Microsoft Visual C++ not supported.
#endif

#include <malloc.h>
// Although ACE does have alloca() on this compiler/platform combination, it is
// disabled by default since it can be dangerous.  Uncomment the following line
// if you ACE to use it.
//#define ACE_HAS_ALLOCA 1

#define ACE_LACKS_DIRENT_H
#define ACE_LACKS_DLFCN_H
#define ACE_LACKS_INTTYPES_H
#define ACE_LACKS_NETDB_H
#define ACE_LACKS_NET_IF_H
#define ACE_LACKS_NETINET_IN_H
#define ACE_LACKS_STDINT_H
#define ACE_LACKS_STROPTS_H
#define ACE_LACKS_SYS_IOCTL_H
#define ACE_LACKS_SYS_IPC_H
#define ACE_LACKS_SYS_MMAN_H
#define ACE_LACKS_SYS_RESOURCE_H
#define ACE_LACKS_SYS_SELECT_H
#define ACE_LACKS_SYS_SEM_H
#define ACE_LACKS_SYS_SOCKET_H
#define ACE_LACKS_SYS_TIME_H
#define ACE_LACKS_SYS_UIO_H
#define ACE_LACKS_SYS_WAIT_H
#define ACE_LACKS_UCONTEXT_H

// Turn off warnings for /W4
// To resume any of these warning: #pragma warning(default: 4xxx)
// which should be placed after these defines

#   if !defined (ALL_WARNINGS) && defined(_MSC_VER) && !defined(ghs) && !defined(__MINGW32__)
// #pragma warning(disable: 4101)  // unreferenced local variable
#     pragma warning(disable: 4127)  /* constant expression for TRACE/ASSERT */
#     pragma warning(disable: 4134)  /* message map member fxn casts */
#     pragma warning(disable: 4511)  /* private copy constructors are good to have */
#     pragma warning(disable: 4512)  /* private operator= are good to have */
#     pragma warning(disable: 4514)  /* unreferenced inlines are common */
#     pragma warning(disable: 4710)  /* private constructors are disallowed */
#     pragma warning(disable: 4705)  /* statement has no effect in optimized code */
// #pragma warning(disable: 4701)  // local variable *may* be used without init
// #pragma warning(disable: 4702)  // unreachable code caused by optimizations
#     pragma warning(disable: 4791)  /* loss of debugging info in retail version */
// #pragma warning(disable: 4204)  // non-constant aggregate initializer
#     pragma warning(disable: 4275)  /* deriving exported class from non-exported */
#     pragma warning(disable: 4251)  /* using non-exported as public in exported */
#     pragma warning(disable: 4786)  /* identifier was truncated to '255' characters in the browser information */
#     pragma warning(disable: 4097)  /* typedef-name used as synonym for class-name */
#   endif /* !ALL_WARNINGS && _MSV_VER && !ghs && !__MINGW32__ */

// STRICT type checking in WINDOWS.H enhances type safety for Windows
// programs by using distinct types to represent all the different
// HANDLES in Windows. So for example, STRICT prevents you from
// mistakenly passing an HPEN to a routine expecting an HBITMAP.
// Note that we only use this if we
#   if defined (ACE_HAS_STRICT) && (ACE_HAS_STRICT != 0)
#     if !defined (STRICT)   /* may already be defined */
#       define STRICT
#     endif /* !STRICT */
#   endif /* ACE_HAS_STRICT */

#include "ace/post.h"
#endif /* ACE_CONFIG_WIN32_MSVC_H */
