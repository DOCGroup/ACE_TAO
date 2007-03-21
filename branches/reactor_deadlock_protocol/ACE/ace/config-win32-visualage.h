// -*- C++ -*-

//=============================================================================
/**
 *  @file   config-win32-visualage.h
 *
 *  $Id$
 *
 *  @brief  Configuration file for VisualAge compilers
 *
 *  @note   Do not include this file directly, include config-win32.h instead.
 *
 *  @author Darrell Brunsch <brunsch@cs.wustl.edu>
 */
//=============================================================================

#ifndef ACE_CONFIG_WIN32_VISUALAGE_H
#define ACE_CONFIG_WIN32_VISUALAGE_H
#include /**/ "ace/pre.h"

#ifndef ACE_CONFIG_WIN32_H
#error Use config-win32.h in config.h instead of this header
#endif /* ACE_CONFIG_WIN32_H */

#if defined (__IBMCPP__) && (__IBMCPP__ >= 400)

#define ACE_CC_NAME ACE_LIB_TEXT ("IBM VisualAge C++")
#define ACE_CC_MAJOR_VERSION (__IBMCPP__ / 0x100)
#define ACE_CC_MINOR_VERSION (__IBMCPP__ % 0x100)
#define ACE_CC_BETA_VERSION (0)
#define ACE_CC_PREPROCESSOR ""
#define ACE_CC_PREPROCESSOR_ARGS ""

#define ACE_HAS_CUSTOM_EXPORT_MACROS
#define ACE_Proper_Export_Flag __declspec (dllexport)
#define ACE_Proper_Import_Flag __declspec (dllimport)
#define ACE_EXPORT_SINGLETON_DECLARATION(T) template class  T
#define ACE_EXPORT_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK) template class __declspec (dllexport) SINGLETON_TYPE<CLASS, LOCK>;
#define ACE_IMPORT_SINGLETON_DECLARATION(T) template class T
#define ACE_IMPORT_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK) template class SINGLETON_TYPE <CLASS, LOCK>;

// By default WIN32 has FD_SETSIZE of 64, which places the limit
// between 61 and 64 on the number of clients a server using the
// Select Reactor can support at the same time (i.e., 64 - standard in,
// out, error).  He we raise the limit to 1024.  Adjust the definition
// below if you need to raise or lower it.
//
#define FD_SETSIZE 1024

//These need to be defined for VisualAgeC++
#define ACE_HAS_CONSISTENT_SIGNAL_PROTOTYPES /* For signal handling */
#define ACE_LACKS_MKTEMP
#define ACE_HAS_CONSISTENT_SIGNAL_PROTOTYPES
#define NSIG 23 /* Maximum no. of signals + 1 */
#define ACE_HAS_CPLUSPLUS_HEADERS 1
#define ACE_HAS_EXCEPTIONS 1
#define ACE_HAS_GNU_CSTRING_H 1
#define ACE_HAS_NONCONST_SELECT_TIMEVAL 1
#define ACE_HAS_SIG_ATOMIC_T 1
#define ACE_HAS_STANDARD_CPP_LIBRARY 1
#define ACE_HAS_STDCPP_STL_INCLUDES 1
#define ACE_HAS_STRERROR 1
#define ACE_HAS_STRING_CLASS 1
#define ACE_HAS_TEMPLATE_SPECIALIZATION 1
#define ACE_HAS_TEMPLATE_TYPEDEFS 1
#define ACE_LACKS_ACE_IOSTREAM 1
#define ACE_LACKS_LINEBUFFERED_STREAMBUF 1
#define ACE_LACKS_STRPTIME 1
#define ACE_LACKS_PRAGMA_ONCE 1
#define ACE_LACKS_STRRECVFD 1
#define ACE_NEW_THROWS_EXCEPTIONS 1
#define ACE_SIZEOF_LONG_DOUBLE 10
#define ACE_TEMPLATES_REQUIRE_SOURCE 1
#define ACE_EXPLICIT_TEMPLATE_DESTRUCTOR_TAKES_ARGS 1
#define ACE_UINT64_FORMAT_SPECIFIER ACE_LIB_TEXT ("%I64u")
#define ACE_USES_STD_NAMESPACE_FOR_STDCPP_LIB 1

#define ACE_LACKS_NET_IF_H
#define ACE_LACKS_NETINET_IN_H
#define ACE_LACKS_DIRENT_H
#define ACE_LACKS_DLFCN_H
#define ACE_LACKS_NETDB_H
#define ACE_LACKS_PWD_H
#define ACE_LACKS_SEMAPHORE_H
#define ACE_LACKS_STDINT_H
#define ACE_LACKS_STRINGS_H
#define ACE_LACKS_SYS_IOCTL_H
#define ACE_LACKS_STROPTS_H
#define ACE_LACKS_UCONTEXT_H
#define ACE_LACKS_SYS_IPC_H
#define ACE_LACKS_SYS_MMAN_H
#define ACE_LACKS_SYS_RESOURCE_H
#define ACE_LACKS_SYS_SELECT_H
#define ACE_LACKS_SYS_SEM_H
#define ACE_LACKS_SYS_SOCKET_H
#define ACE_LACKS_SYS_TIME_H
#define ACE_LACKS_SYS_UIO_H
#define ACE_LACKS_SYS_WAIT_H
#define ACE_LACKS_SYS_SHM_H
#define ACE_LACKS_SYS_MSG_H
#define ACE_LACKS_TERMIOS_H
#define ACE_LACKS_POLL_H
#define ACE_LACKS_NETINET_TCP_H
#define ACE_HAS_3_PARAM_WCSTOK
#define ACE_LACKS_STRCASECMP
#define ENOTEMPTY     WSAENOTEMPTY

#undef WIFEXITED
#undef WEXITSTATUS

#define _O_CREAT  O_CREAT
#define _O_TRUNC   O_TRUNC
#define _O_TEMPORARY   0
#define _O_EXCL   O_EXCL
#define _O_RDWR   O_RDWR
#define _O_WRONLY O_WRONLY
#define _O_RDONLY O_RDONLY
#define _O_APPEND O_APPEND
#define _O_BINARY O_BINARY
#define _O_TEXT O_TEXT

#define _endthreadex _endthread
#define _beginthreadex _beginthread

//Error codes that are in MS Visual C++
#define EFAULT 99 /* Error code (should be in errno.h) */
#define ENODEV          19
#define EPIPE           32
#define ENAMETOOLONG    38

#define ACE_ENDTHREADEX(STATUS) ::_endthread ()

#endif /* defined(__IBMCPP__) */

#include /**/ "ace/post.h"
#endif /* ACE_CONFIG_WIN32_VISUALAGE_H */
