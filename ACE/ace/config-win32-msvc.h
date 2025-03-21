//=============================================================================
/**
 *  @file   config-win32-msvc.h
 *
 *  @brief  Microsoft Visual C++ configuration file.
 *
 *  This file is the ACE configuration file for Microsoft Visual C++
 *
 *  @author Darrell Brunsch <brunsch@cs.wustl.edu>
 */
//=============================================================================

#ifndef ACE_CONFIG_WIN32_MSVC_H
#define ACE_CONFIG_WIN32_MSVC_H
#include /**/ "ace/pre.h"

#ifndef ACE_CONFIG_WIN32_H
#error Use config-win32.h in config.h instead of this header
#endif /* ACE_CONFIG_WIN32_H */

#define ACE_CC_NAME ACE_TEXT ("Visual C++")
#define ACE_CC_PREPROCESSOR "CL.EXE"
#define ACE_CC_PREPROCESSOR_ARGS "-nologo -E"

#define ACE_CC_MAJOR_VERSION (_MSC_VER / 100 - 6)
#define ACE_CC_MINOR_VERSION (_MSC_VER % 100)
#define ACE_CC_BETA_VERSION (0)

#if !defined(_NATIVE_WCHAR_T_DEFINED)
  #define ACE_LACKS_NATIVE_WCHAR_T
#endif

#  define ACE_ENDTHREADEX(STATUS) ::_endthreadex ((DWORD) STATUS)

//FUZZ: disable check_for_msc_ver
#if (_MSC_VER >= 1930)
# include "ace/config-win32-msvc-143.h"
#elif (_MSC_VER >= 1920)
# include "ace/config-win32-msvc-142.h"
#elif (_MSC_VER >= 1910)
# include "ace/config-win32-msvc-141.h"
#else
# error This version of Microsoft Visual C++ is not supported.
#endif
//FUZZ: enable check_for_msc_ver

#if defined(ACE_MT_SAFE) && (ACE_MT_SAFE != 0)
// must have _MT defined to include multithreading
// features from win32 headers
# if !defined(_MT)
// *** DO NOT *** defeat this error message by defining _MT yourself.
// On MSVC, this is changed by selecting the Multithreaded
// DLL or Debug Multithreaded DLL in the Project Settings
// under C++ Code Generation.
#  error You must link against multi-threaded libraries when using ACE (check your project settings)
# endif /* !_MT */
#endif /* ACE_MT_SAFE && ACE_MT_SAFE != 0 */

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

#define ACE_LACKS_SEMAPHORE_H
#define ACE_LACKS_STRINGS_H
#define ACE_LACKS_PWD_H
#define ACE_LACKS_POLL_H
#define ACE_LACKS_SYS_SHM_H
#define ACE_LACKS_SYS_MSG_H
#define ACE_LACKS_NETINET_TCP_H
#define ACE_LACKS_TERMIOS_H
#define ACE_LACKS_REGEX_H

#define ACE_LACKS_LOCALTIME_R
#define ACE_LACKS_GMTIME_R
#define ACE_LACKS_ASCTIME_R

#define ACE_INT64_FORMAT_SPECIFIER_ASCII "%I64d"
#define ACE_UINT64_FORMAT_SPECIFIER_ASCII "%I64u"

#define ACE_STRTOLL_EQUIVALENT ::_strtoi64
#define ACE_WCSTOLL_EQUIVALENT ::_wcstoi64
#define ACE_STRTOULL_EQUIVALENT ::_strtoui64
#define ACE_WCSTOULL_EQUIVALENT ::_wcstoui64
#define ACE_WTOF_EQUIVALENT ::_wtof

#define ACE_LACKS_ISBLANK
#define ACE_LACKS_ISWBLANK
#define ACE_LACKS_CORRECT_ISWPRINT_TAB
#define ACE_ISCTYPE_EQUIVALENT ::_isctype

#define ACE_HAS_WIN32_STRUCTURED_EXCEPTIONS
#define ACE_HAS_CRTSETREPORTMODE
#define ACE_HAS_SETUNHANDLEDEXCEPTIONFILTER

// Turn off warnings for /W4
// To resume any of these warning: #pragma warning(default: 4xxx)
// which should be placed after these defines

#if !defined (ALL_WARNINGS) && defined(_MSC_VER) && !defined(__MINGW32__)
#     pragma warning(disable: 4127)  /* constant expression for TRACE/ASSERT */
#     pragma warning(disable: 4134)  /* message map member fxn casts */
#     pragma warning(disable: 4511)  /* private copy constructors are good to have */
#     pragma warning(disable: 4512)  /* private operator= are good to have */
#     pragma warning(disable: 4514)  /* unreferenced inlines are common */
#     pragma warning(disable: 4710)  /* private constructors are disallowed */
#     pragma warning(disable: 4705)  /* statement has no effect in optimized code */
#     pragma warning(disable: 4791)  /* loss of debugging info in retail version */
#     pragma warning(disable: 4275)  /* deriving exported class from non-exported */
#     pragma warning(disable: 4251)  /* using non-exported as public in exported */
#     pragma warning(disable: 4786)  /* identifier was truncated to '255' characters in the browser information */
#     pragma warning(disable: 4097)  /* typedef-name used as synonym for class-name */
#     pragma warning(disable: 4800)  /* converting int to boolean */
# if defined (__INTEL_COMPILER)
#     pragma warning(disable: 1744)  /* field of class type without a DLL interface used in a class with a DLL interface */
#     pragma warning(disable: 1738)
# endif
#endif /* !ALL_WARNINGS && _MSV_VER && !__MINGW32__ */

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

#include /**/ "ace/post.h"
#endif /* ACE_CONFIG_WIN32_MSVC_H */
