/* -*- C++ -*- */
// $Id$

//Created for IBMCPP
// The following configuration file contains the defines
// common to all VisualAge compilers.

#ifndef ACE_WIN32_VISUALAGECPP_H
#define ACE_WIN32_VISUALAGECPP_H

#if defined (__IBMCPP__) && (__IBMCPP__ >= 400)

//These need to be defined for VisualAgeC++
#define ERRMAX 256 /* Needed for following define */
#define ACE_LACKS_SYS_NERR /* Needed for sys_nerr in Log_Msg.cpp */
#define ACE_HAS_CONSISTENT_SIGNAL_PROTOTYPES /* For signal handling */
#define ACE_HAS_TYPENAME_KEYWORD
#define ACE_LACKS_MKTEMP
#define ACE_HAS_CONSISTENT_SIGNAL_PROTOTYPES
#define NSIG 23 /* Maximum no. of signals + 1 */

#undef WIFEXITED
#undef WEXITSTATUS

#define ACE_HAS_CPLUSPLUS_HEADERS
#define ACE_HAS_BROKEN_NESTED_TEMPLATES

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
#define ENODEV		19
#define EPIPE		32
#define ENAMETOOLONG	38


#if defined (ACE_HAS_UNICODE)
 #undef ACE_HAS_UNICODE
#endif /* ACE_HAS_UNICODE */

#if defined (ACE_HAS_WCHAR_TYPEDEFS_USHORT)
  #undef ACE_HAS_WCHAR_TYPEDEFS_USHORT
#endif /* ACE_HAS_WCHAR_TYPEDEFS_USHORT */

#if defined (ACE_TEMPLATES_REQUIRE_SOURCE)
//  #undef ACE_TEMPLATES_REQUIRE_SOURCE
#endif

#endif /* defined(__IBMCPP__) */
#endif /* ACE_WIN32_VISUALAGECPP_H */
