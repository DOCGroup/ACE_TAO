// -*- C++ -*-
// $Id$

// The following configuration file contains the defines
// common to all Win32/MSVC/MFC combinations.

#ifndef ACE_WIN32_MSVC_H
#define ACE_WIN32_MSVC_H

#if defined (_MSC_VER)

# include "ace/config-win32-common.h"

# define ACE_CC_NAME "Visual C++"
# define ACE_CC_PREPROCESSOR "CL.EXE"
# define ACE_CC_PREPROCESSOR_ARGS "-nologo -E"

# if (_MSC_VER >= 1200)
#  define ACE_CC_MAJOR_VERSION 6
# elif (_MSC_VER >= 1100)
#  define ACE_CC_MAJOR_VERSION 5
# elif (_MSC_VER >= 1000)
#  define ACE_CC_MAJOR_VERSION 4
# endif /* _MSC_VER  >= 1200 */

# define ACE_CC_MINOR_VERSION (_MSC_VER % 100)
# define ACE_CC_BETA_VERSION (0)

// Define this if you want to use the standard C++ library
//#define ACE_HAS_STANDARD_CPP_LIBRARY 1

// MSVC enforces the One Definition Rule
# define ACE_HAS_ONE_DEFINITION_RULE

# if defined (_MSC_VER) && (_MSC_VER >= 1020)
#  if !defined (ACE_HAS_STANDARD_CPP_LIBRARY)
#   define ACE_HAS_STANDARD_CPP_LIBRARY    0
#  endif
# else
#  if defined (ACE_HAS_STANDARD_CPP_LIBRARY)
#   undef ACE_HAS_STANDARD_CPP_LIBRARY
#  endif
#  define ACE_HAS_STANDARD_CPP_LIBRARY 0
# endif

// The STL that comes with ACE uses the std namespace. Note however, it is not
// part of the standard C++ library
# if !defined (ACE_USES_STD_NAMESPACE_FOR_STDCPP_LIB)
#  define ACE_USES_STD_NAMESPACE_FOR_STDCPP_LIB 1
# endif /* ACE_USES_STD_NAMESPACE_FOR_STDCPP_LIB */

# if !defined (ACE_HAS_BROKEN_NESTED_TEMPLATES)
#  define ACE_HAS_BROKEN_NESTED_TEMPLATES
# endif /* ACE_HAS_BROKEN_NESTED_TEMPLATES */

// By default, we disable the C++ casting because
// it requires the RTTI support to be turned on which
// is not something we usually do.
# if !defined (ACE_HAS_ANSI_CASTS)
#  define ACE_HAS_ANSI_CASTS 0
# endif

// MSVC already defined __TEXT
# define ACE_HAS_TEXT_MACRO_CONFLICT

# define ACE_HAS_EXPLICIT_KEYWORD
# define ACE_HAS_MUTABLE_KEYWORD

// VC5 doesn't support operator placement delete
# if defined (_MSC_VER) && (_MSC_VER < 1200)  
#  define ACE_LACKS_PLACEMENT_OPERATOR_DELETE
# endif /* _MSC_VER && _MSC_VER < 1200 */

# define ACE_HAS_WCHAR_TYPEDEFS_USHORT
# if !defined (ACE_HAS_WINCE)
#  define ACE_HAS_EXCEPTIONS
# endif /* ACE_HAS_WINCE */
# define ACE_HAS_BROKEN_NAMESPACES
# define ACE_HAS_BROKEN_IMPLICIT_CONST_CAST
# define ACE_HAS_WORKING_EXPLICIT_TEMPLATE_DESTRUCTOR

# if defined (ACE_HAS_ANSI_CASTS) && (ACE_HAS_ANSI_CASTS == 0)
#  undef  ACE_HAS_ANSI_CASTS
# endif /* ACE_HAS_ANSI_CASTS && ACE_HAS_ANSI_CASTS == 0 */

# if _MSC_VER < 1000
#  define ACE_LACKS_PRAGMA_ONCE
# endif /* _MSC_VER < 1000 */

// Only >= MSVC 5.0 definitions
# if (_MSC_VER >= 1100)
#  if !defined (ACE_HAS_WINCE)
#   define ACE_HAS_SIG_ATOMIC_T
#  endif /* ACE_HAS_WINCE */
# endif /* _MSC_VER >= 1100 */

// Only >= MSVC 6.0 definitions
# if (_MSC_VER >= 1200)
#  define ACE_HAS_TYPENAME_KEYWORD
#  define ACE_HAS_STD_TEMPLATE_SPECIALIZATION
# endif /* _MSC_VER >= 1200 */

// Compiler doesn't support static data member templates.
# define ACE_LACKS_STATIC_DATA_MEMBER_TEMPLATES

# define ACE_LACKS_MODE_MASKS
# define ACE_LACKS_STRRECVFD

// Compiler/platform has correctly prototyped header files.
# define ACE_HAS_CPLUSPLUS_HEADERS

// Platform supports POSIX timers via timestruc_t.
//define ACE_HAS_POSIX_TIME
# define ACE_HAS_STRPTIME
# define ACE_LACKS_NATIVE_STRPTIME

// Compiler/platform supports strerror ().
# define ACE_HAS_STRERROR

# define ACE_TEMPLATES_REQUIRE_SOURCE

// Platform provides ACE_TLI function prototypes.
// For Win32, this is not really true, but saves a lot of hassle!
# define ACE_HAS_TLI_PROTOTYPES
# define ACE_HAS_GNU_CSTRING_H

// Platform support linebuffered streaming is broken
# define ACE_LACKS_LINEBUFFERED_STREAMBUF

// Template specialization is supported.
# define ACE_HAS_TEMPLATE_SPECIALIZATION

// ----------------- "derived" defines and includes -----------

# if defined (ACE_HAS_STANDARD_CPP_LIBRARY) && (ACE_HAS_STANDARD_CPP_LIBRARY != 0)

#  if (_MSC_VER > 1020)
// Platform has its Standard C++ library in the namespace std
#   if !defined (ACE_USES_STD_NAMESPACE_FOR_STDCPP_LIB)
#    define ACE_USES_STD_NAMESPACE_FOR_STDCPP_LIB       1
#   endif /* ACE_USES_STD_NAMESPACE_FOR_STDCPP_LIB */
#  else /* (_MSC_VER > 1020) */
#   if defined (ACE_USES_STD_NAMESPACE_FOR_STDCPP_LIB)
#    undef ACE_USES_STD_NAMESPACE_FOR_STDCPP_LIB
#   endif /* ACE_USES_STD_NAMESPACE_FOR_STDCPP_LIB */
#   define ACE_USES_STD_NAMESPACE_FOR_STDCPP_LIB 0
#  endif /* (_MSC_VER > 1020) */

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

// MSVC 4.0 or greater
# if (_MSC_VER >= 1000)
// Compiler/Platform supports the "using" keyword.
#  define ACE_HAS_USING_KEYWORD
# endif

// MSVC 2.2 or lower
# if (_MSC_VER < 1000)
// This needs to be here since MSVC++ 2.0 seems to have forgotten to
// include it. Does anybody know which MSC_VER MSVC 2.0 has ?
inline void *operator new (unsigned int, void *p) { return p; }
# endif

# if !defined(ACE_HAS_WINCE)
#  if defined(ACE_HAS_DLL) && (ACE_HAS_DLL != 0)
#   if !defined(_DLL)
// *** DO NOT *** DO NOT *** defeat this error message
// by defining _DLL yourself.  RTFM and see who set _DLL.
#    error You must link against (Debug) Multithreaded DLL run-time libraries.
#   endif /* !_DLL */
#  endif  /* ACE_HAS_DLL && ACE_HAS_DLL != 0 */
# endif /* !ACE_HAS_WINCE */

# ifdef _DEBUG
#  if !defined (ACE_HAS_WINCE)
#   include /**/ <crtdbg.h>
#  endif /* ACE_HAS_WINCE */
# endif

# pragma warning(default: 4201)  /* winnt.h uses nameless structs */

// At least for Win32 - MSVC compiler (ver. 5)
# define ACE_INT64_FORMAT_SPECIFIER "%I64d"
# define ACE_UINT64_FORMAT_SPECIFIER "%I64u"

#endif /* _MSC_VER */

#endif /* ACE_WIN32_COMMON_H */
