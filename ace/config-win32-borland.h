// -*- C++ -*-
// $Id$

// The following configuration file contains defines for Borland compilers.

#ifndef ACE_WIN32_BORLAND_H
#include "ace/pre.h"
#define ACE_WIN32_BORLAND_H

#if defined (__BORLANDC__)

# if (__BORLANDC__ == 0x540)
// The linker in C++Builder 4 has trouble with the large number of DLL
// function exports created when you compile without inline
// functions. Therefore we will use inline functions by default with
// this version of the compiler.
#  if ! defined (__ACE_INLINE__)
#   define __ACE_INLINE__ 1
#  endif /* __ACE_INLINE__ */
# endif /* __BORLANDC__ == 0x540 */

# include "ace/config-win32-common.h"

# define ACE_CC_NAME "Borland C++ Builder"
# define ACE_CC_MAJOR_VERSION (__BORLANDC__ / 0x100)
# define ACE_CC_MINOR_VERSION (__BORLANDC__ % 0x100)
# define ACE_CC_BETA_VERSION (0)
# define ACE_CC_PREPROCESSOR "CPP32.EXE"
# define ACE_CC_PREPROCESSOR_ARGS "-P- -ocon"

# define ACE_EXPORT_NESTED_CLASSES 1
# define ACE_HAS_ANSI_CASTS 1
# define ACE_HAS_CPLUSPLUS_HEADERS 1
# define ACE_HAS_EXCEPTIONS 1
# define ACE_HAS_EXPLICIT_KEYWORD 1
# define ACE_HAS_GNU_CSTRING_H 1
# define ACE_HAS_MINIMUM_IOSTREAMH_INCLUSION 1
# define ACE_HAS_MUTABLE_KEYWORD 1
# define ACE_HAS_NONCONST_SELECT_TIMEVAL 1
# define ACE_HAS_ONE_DEFINITION_RULE 1
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
# define ACE_HAS_TEXT_MACRO_CONFLICT 1
# define ACE_HAS_TYPENAME_KEYWORD 1
# define ACE_HAS_USER_MODE_MASKS 1
# define ACE_HAS_USING_KEYWORD 1
# define ACE_LACKS_ACE_IOSTREAM 1
# define ACE_LACKS_LINEBUFFERED_STREAMBUF 1
# define ACE_LACKS_MODE_MASKS 1
# define ACE_LACKS_NATIVE_STRPTIME 1
# define ACE_LACKS_PLACEMENT_OPERATOR_DELETE 1
# define ACE_LACKS_PRAGMA_ONCE 1
# define ACE_LACKS_STRRECVFD 1
# define ACE_NEW_THROWS_EXCEPTIONS 1
# define ACE_SIZEOF_LONG_DOUBLE 10
# define ACE_TEMPLATES_REQUIRE_SOURCE 1
# define ACE_UINT64_FORMAT_SPECIFIER "%Lu"
# define ACE_INT64_FORMAT_SPECIFIER "%Ld"
# define ACE_USES_STD_NAMESPACE_FOR_STDCPP_LIB 1

/* need to ensure these are included before <iomanip> */
# include <time.h>
# include <stdlib.h>

#endif /* defined(__BORLANDC__) */

#include "ace/post.h"
#endif /* ACE_WIN32_BORLAND_H */
