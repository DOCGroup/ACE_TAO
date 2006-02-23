// -*- C++ -*-

//=============================================================================
/**
 *  @file   config-win32-msvc-6.h
 *
 *  $Id$
 *
 *  @brief  Microsoft Visual C++ 6.0 configuration file.
 *
 *  This file is the ACE configuration file for Microsoft Visual C++ version 6.
 *
 *  @note   Do not include this file directly, include config-win32.h instead.
 *
 *  @author Darrell Brunsch <brunsch@cs.wustl.edu>
 */
//=============================================================================

#ifndef ACE_CONFIG_WIN32_MSVC_6_H
#define ACE_CONFIG_WIN32_MSVC_6_H
#include /**/ "ace/pre.h"

#ifndef ACE_CONFIG_WIN32_H
#error Use config-win32.h in config.h instead of this header
#endif /* ACE_CONFIG_WIN32_H */

// The std library that comes with VC6 is pre-standard, but
// it's close enough that any discrepancies should be worked around
// with individual hacks.
# if !defined (ACE_HAS_STANDARD_CPP_LIBRARY)
#   define ACE_HAS_STANDARD_CPP_LIBRARY 1
# endif

// The STL that comes with ACE uses the std namespace. Note however, it is not
// part of the standard C++ library
# if !defined (ACE_USES_STD_NAMESPACE_FOR_STDCPP_LIB)
#  define ACE_USES_STD_NAMESPACE_FOR_STDCPP_LIB 1
# endif /* ACE_USES_STD_NAMESPACE_FOR_STDCPP_LIB */

# if !defined (ACE_HAS_BROKEN_NESTED_TEMPLATES)
#  define ACE_HAS_BROKEN_NESTED_TEMPLATES
# endif /* ACE_HAS_BROKEN_NESTED_TEMPLATES */

// Keywords

# define ACE_HAS_TYPENAME_KEYWORD

// No support for member templates.
#define ACE_LACKS_MEMBER_TEMPLATES

#define ACE_HAS_ITOA

#define ACE_ITOA_EQUIVALENT ::_itoa
#define ACE_STRCASECMP_EQUIVALENT ::_stricmp
#define ACE_STRNCASECMP_EQUIVALENT ::_strnicmp
#define ACE_WCSDUP_EQUIVALENT ::_wcsdup

# if !defined (ACE_HAS_WINCE)
#  define ACE_HAS_EXCEPTIONS
#  define ACE_HAS_BROKEN_UNEXPECTED_EXCEPTIONS
#  define ACE_HAS_NO_THROW_SPEC
#  define ACE_HAS_SIG_ATOMIC_T
# endif /* ACE_HAS_WINCE */

// CE SDKs didn't provide some errno values we need in eVC3 and 4 (which
// are the same base compiler version as MSVC 6).
#if defined (ACE_HAS_WINCE)
#  define EMFILE WSAEMFILE
#  define EINTR  WSAEINTR
#  define EACCES ERROR_ACCESS_DENIED
#  define ENOSPC ERROR_HANDLE_DISK_FULL
#  define EEXIST ERROR_FILE_EXISTS
#  define EPIPE  ERROR_BROKEN_PIPE
#  define EFAULT WSAEFAULT
#  define ENOENT WSAEINVAL
#  define EINVAL WSAEINVAL
#  define ERANGE WSAEINVAL
#  define EAGAIN WSAEWOULDBLOCK
#  define ENOMEM ERROR_OUTOFMEMORY
#  define ENODEV ERROR_BAD_DEVICE
#  define _MAX_FNAME 255
//#endif  // UNDER_CE
#endif /* ACE_HAS_WINCE */

# define ACE_HAS_WORKING_EXPLICIT_TEMPLATE_DESTRUCTOR


# define ACE_LACKS_INTPTR_T

// Compiler doesn't support static data member templates.
# define ACE_LACKS_STATIC_DATA_MEMBER_TEMPLATES

# define ACE_LACKS_STRRECVFD

// Compiler/platform has correctly prototyped header files.
# define ACE_HAS_CPLUSPLUS_HEADERS

// Platform supports POSIX timers via timestruc_t.
# define ACE_HAS_STRPTIME
# define ACE_LACKS_NATIVE_STRPTIME

// Compiler/platform supports strerror ().
# define ACE_HAS_STRERROR

# define ACE_TEMPLATES_REQUIRE_SOURCE

// Platform provides ACE_TLI function prototypes.
// For Win32, this is not really true, but saves a lot of hassle!
# define ACE_HAS_TLI_PROTOTYPES

// Platform support linebuffered streaming is broken
# define ACE_LACKS_LINEBUFFERED_STREAMBUF

// Template specialization is supported.
# define ACE_HAS_TEMPLATE_SPECIALIZATION


// At least for ACE_UNIMPLEMENTED_FUNC in class templates, this is needed to
// explicitly instantiate a template that has ACE_UNIMPLEMENTED_FUNC.
# define ACE_NEEDS_FUNC_DEFINITIONS

// ----------------- "derived" defines and includes -----------

# if defined (ACE_HAS_STANDARD_CPP_LIBRARY) && (ACE_HAS_STANDARD_CPP_LIBRARY != 0)

// Platform has its Standard C++ library in the namespace std
#   if !defined (ACE_USES_STD_NAMESPACE_FOR_STDCPP_LIB)
#    define ACE_USES_STD_NAMESPACE_FOR_STDCPP_LIB       1
#   endif /* ACE_USES_STD_NAMESPACE_FOR_STDCPP_LIB */

// Microsoft's auto_ptr in standard cpp library doesn't have reset ().
#  define ACE_AUTO_PTR_LACKS_RESET

// ace/iostream.h does not work with the standard cpp library (yet).
#  if !defined (ACE_USES_OLD_IOSTREAMS)
#   define ACE_LACKS_ACE_IOSTREAM
#  endif /* ! ACE_USES_OLD_IOSTREAMS */
# else
// iostream header lacks ipfx (), isfx (), etc., declarations
#  define ACE_LACKS_IOSTREAM_FX
# endif

// While digging the MSVC 4.0 include files, I found how to disable
// MSVC warnings: --Amos Shapira


// "C4355: 'this' : used in base member initializer list"
# pragma warning(disable:4355) /* disable C4514 warning */
//      #pragma warning(default:4355)   // use this to reenable, if desired

# pragma warning(disable:4201)  /* winnt.h uses nameless structs */

# pragma warning(disable:4231)
// Disable warning of using Microsoft Extension.

// MSVC allows throw keyword but complains about it.
# pragma warning( disable : 4290 )

// Inheritance by dominance is A-OK by us
# pragma warning (disable:4250)

// Disable warning about long names (due to templates).
# pragma warning( disable : 4786 )

# if !defined(ACE_HAS_WINCE)
#  if defined(ACE_HAS_DLL) && (ACE_HAS_DLL != 0)
#   if !defined(_DLL)
// *** DO NOT *** DO NOT *** defeat this error message
// by defining _DLL yourself.  RTFM and see who set _DLL.
#    error You must link against (Debug) Multithreaded DLL run-time libraries.
#   endif /* !_DLL */
#  endif  /* ACE_HAS_DLL && ACE_HAS_DLL != 0 */
# endif /* !ACE_HAS_WINCE */

# pragma warning(default: 4201)  /* winnt.h uses nameless structs */

# define ACE_INT64_FORMAT_SPECIFIER ACE_LIB_TEXT ("%I64d")
# define ACE_UINT64_FORMAT_SPECIFIER ACE_LIB_TEXT ("%I64u")

#include /**/ "ace/post.h"
#endif /* ACE_CONFIG_WIN32_MSVC_6_H */
