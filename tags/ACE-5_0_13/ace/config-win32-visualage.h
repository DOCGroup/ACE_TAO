/* -*- C++ -*- */
// $Id$

//Created for IBMCPP
// The following configuration file contains the defines
// common to all VisualAge compilers.

#ifndef ACE_WIN32_VISUALAGECPP_H
#define ACE_WIN32_VISUALAGECPP_H

#if defined (__IBMCPP__) && (__IBMCPP__ >= 400)

#define ACE_CC_NAME "IBM VisualAge C++"
#define ACE_CC_MAJOR_VERSION (__IBMCPP__ / 0x100)
#define ACE_CC_MINOR_VERSION (__IBMCPP__ % 0x100)
#define ACE_CC_BETA_VERSION (0)
#define ACE_CC_PREPROCESSOR ""
#define ACE_CC_PREPROCESSOR_ARGS ""

//These need to be defined for VisualAgeC++
#define ERRMAX 256 /* Needed for following define */
#define ACE_LACKS_SYS_NERR /* Needed for sys_nerr in Log_Msg.cpp */
#define ACE_HAS_CONSISTENT_SIGNAL_PROTOTYPES /* For signal handling */
#define ACE_HAS_TYPENAME_KEYWORD
#define ACE_LACKS_MKTEMP
#define ACE_HAS_CONSISTENT_SIGNAL_PROTOTYPES
#define NSIG 23 /* Maximum no. of signals + 1 */
#define ACE_HAS_ANSI_CASTS 1
#define ACE_HAS_BROKEN_NESTED_TEMPLATES 1
#define ACE_HAS_CPLUSPLUS_HEADERS 1
#define ACE_HAS_EXCEPTIONS 1
#define ACE_HAS_EXPLICIT_KEYWORD 1
#define ACE_HAS_GNU_CSTRING_H 1
#define ACE_HAS_MUTABLE_KEYWORD 1
#define ACE_HAS_NONCONST_SELECT_TIMEVAL 1
#define ACE_HAS_ONE_DEFINITION_RULE 1
#define ACE_HAS_SIG_ATOMIC_T 1
#define ACE_HAS_STANDARD_CPP_LIBRARY 1
#define ACE_HAS_STDCPP_STL_INCLUDES 1
#define ACE_HAS_STRERROR 1
#define ACE_HAS_STRING_CLASS 1
#define ACE_HAS_STRPTIME 1
#define ACE_HAS_TEMPLATE_SPECIALIZATION 1
#define ACE_HAS_TEMPLATE_TYPEDEFS 1
#define ACE_HAS_TEXT_MACRO_CONFLICT 1
#define ACE_HAS_TYPENAME_KEYWORD 1
#define ACE_HAS_USING_KEYWORD 1
#define ACE_LACKS_ACE_IOSTREAM 1
#define ACE_LACKS_LINEBUFFERED_STREAMBUF 1
#define ACE_LACKS_MODE_MASKS 1
#define ACE_LACKS_NATIVE_STRPTIME 1
#define ACE_LACKS_PRAGMA_ONCE 1
#define ACE_LACKS_STRRECVFD 1
#define ACE_NEW_THROWS_EXCEPTIONS 1
#define ACE_SIZEOF_LONG_DOUBLE 10
#define ACE_TEMPLATES_REQUIRE_SOURCE 1
#define ACE_UINT64_FORMAT_SPECIFIER "%I64u"
#define ACE_USES_STD_NAMESPACE_FOR_STDCPP_LIB 1

#undef WIFEXITED
#undef WEXITSTATUS

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
# undef ACE_HAS_UNICODE
#endif /* ACE_HAS_UNICODE */

#endif /* defined(__IBMCPP__) */

#endif /* ACE_WIN32_VISUALAGECPP_H */
