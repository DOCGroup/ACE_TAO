// -*- C++ -*-
//$Id$

// The following configuration file contains defines for Borland compilers.

#ifndef ACE_CONFIG_WIN32_BORLAND_H
#define ACE_CONFIG_WIN32_BORLAND_H
#include "ace/pre.h"

#ifndef ACE_CONFIG_WIN32_H
#error Use config-win32.h in config.h instead of this header
#endif /* ACE_CONFIG_WIN32_H */

# if defined (ACE_HAS_PACE)
#  ifndef PACE_HAS_ALL_POSIX_FUNCS
#   define PACE_HAS_ALL_POSIX_FUNCS 1
#  endif /* PACE_HAS_ALL_POSIX_FUNCS */
# endif /* ACE_HAS_PACE */

# if (__BORLANDC__ == 0x540)
// The linker in C++Builder 4 has trouble with the large number of DLL
// function exports created when you compile without inline functions.
// Therefore we will always use inline functions with this version of
// the compiler.
#  if defined (__ACE_INLINE__)
#   undef __ACE_INLINE__
#  endif /* __ACE_INLINE__ */
#  define __ACE_INLINE__ 1
# else /* __BORLANDC__ == 0x540 */
// In later versions of C++Builder we will prefer inline functions by
// default. The debug configuration of ACE is built with functions
// out-of-line, so when linking your application against a debug ACE
// build, you can choose to use the out-of-line functions by adding
// ACE_NO_INLINE=1 to your project settings.
#  if !defined (__ACE_INLINE__)
#   define __ACE_INLINE__ 1
#  endif /* __ACE_INLINE__ */
# endif /* __BORLANDC__ == 0x540 */

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

# include "ace/config-win32-common.h"

# define ACE_CC_NAME ACE_LIB_TEXT ("Borland C++ Builder")
# define ACE_CC_MAJOR_VERSION (__BORLANDC__ / 0x100)
# define ACE_CC_MINOR_VERSION (__BORLANDC__ % 0x100)
# define ACE_CC_BETA_VERSION (0)
# define ACE_CC_PREPROCESSOR "CPP32.EXE"
# define ACE_CC_PREPROCESSOR_ARGS "-q -P- -o%s"

# define ACE_HAS_ITOA 1
# define ACE_STRCASECMP_EQUIVALENT ::stricmp 
# define ACE_STRNCASECMP_EQUIVALENT ::strnicmp 
# define ACE_WCSDUP_EQUIVALENT ::_wcsdup

# define ACE_EXPORT_NESTED_CLASSES 1
# define ACE_HAS_ANSI_CASTS 1
# define ACE_HAS_CPLUSPLUS_HEADERS 1
# define ACE_HAS_EXCEPTIONS 1
# define ACE_HAS_EXPLICIT_KEYWORD 1
# define ACE_HAS_GNU_CSTRING_H 1
# define ACE_HAS_MINIMUM_IOSTREAMH_INCLUSION 1
# define ACE_HAS_MUTABLE_KEYWORD 1
# define ACE_HAS_NONCONST_SELECT_TIMEVAL 1
# define ACE_HAS_SIG_ATOMIC_T 1
# define ACE_HAS_STANDARD_CPP_LIBRARY 1
# if (__BORLANDC__ <= 0x540)
#  define ACE_HAS_STD_TEMPLATE_METHOD_SPECIALIZATION 1
# endif
# define ACE_HAS_STD_TEMPLATE_SPECIALIZATION 1
# define ACE_HAS_STDCPP_STL_INCLUDES 1
# define ACE_HAS_STRERROR 1
# define ACE_HAS_STRING_CLASS 1
# define ACE_HAS_STRPTIME 1
# define ACE_HAS_TEMPLATE_SPECIALIZATION 1
# define ACE_HAS_TEMPLATE_TYPEDEFS 1
# define ACE_HAS_TYPENAME_KEYWORD 1
# define ACE_HAS_USER_MODE_MASKS 1
# define ACE_HAS_USING_KEYWORD 1
# define ACE_LACKS_ACE_IOSTREAM 1
# define ACE_LACKS_INLINE_ASSEMBLY 1
# define ACE_LACKS_LINEBUFFERED_STREAMBUF 1
# define ACE_LACKS_MODE_MASKS 1
# define ACE_LACKS_NATIVE_STRPTIME 1
# define ACE_LACKS_PLACEMENT_OPERATOR_DELETE 1
# define ACE_LACKS_PRAGMA_ONCE 1
# define ACE_LACKS_STRRECVFD 1
# define ACE_NEW_THROWS_EXCEPTIONS 1
# define ACE_SIZEOF_LONG_DOUBLE 10
# define ACE_TEMPLATES_REQUIRE_SOURCE 1
# define ACE_UINT64_FORMAT_SPECIFIER ACE_LIB_TEXT ("%Lu")
# define ACE_INT64_FORMAT_SPECIFIER ACE_LIB_TEXT ("%Ld")
# define ACE_USES_STD_NAMESPACE_FOR_STDCPP_LIB 1
# define ACE_USES_STD_NAMESPACE_FOR_STDC_LIB 1
# define ACE_WSTRING_HAS_USHORT_SUPPORT 1

# define ACE_ENDTHREADEX(STATUS) ::_endthreadex ((DWORD) STATUS)

#include "ace/post.h"
#endif /* ACE_CONFIG_WIN32_BORLAND_H */
