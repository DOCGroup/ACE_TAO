// $Id$

#ifndef ACE_LINUX_ICC_COMMON_H
#define ACE_LINUX_ICC_COMMON_H
#include /**/ "ace/pre.h"

# define ACE_HAS_ANSI_CASTS
# define ACE_HAS_CPLUSPLUS_HEADERS
# define ACE_HAS_EXPLICIT_KEYWORD
# define ACE_HAS_MUTABLE_KEYWORD
# define ACE_HAS_STDCPP_STL_INCLUDES
# define ACE_HAS_TEMPLATE_TYPEDEFS
# define ACE_HAS_TYPENAME_KEYWORD
# define ACE_HAS_STD_TEMPLATE_SPECIALIZATION
# define ACE_HAS_STANDARD_CPP_LIBRARY 1
# define ACE_HAS_TEMPLATE_SPECIALIZATION
# define ACE_HAS_USING_KEYWORD
# define ACE_HAS_WORKING_EXPLICIT_TEMPLATE_DESTRUCTOR
# define ACE_USES_STD_NAMESPACE_FOR_STDCPP_LIB 1
# define ACE_HAS_STRING_CLASS

// __EXCEPTIONS is defined with -fexceptions, the egcs default.  It
// is not defined with -fno-exceptions, the ACE default for g++.
// ACE_HAS_EXCEPTIONS is defined in
// include/makeinclude/wrapper_macros.GNU, so this really isn't
// necessary.  Just in case . . .
# if defined (__EXCEPTIONS) && !defined (ACE_HAS_EXCEPTIONS)
#   define ACE_HAS_EXCEPTIONS
# endif /* __EXCEPTIONS && ! ACE_HAS_EXCEPTIONS */

# if defined (ACE_HAS_EXCEPTIONS)
#   define ACE_NEW_THROWS_EXCEPTIONS
# endif /* ACE_HAS_EXCEPTIONS */

#if (defined (i386) || defined (__i386__)) && !defined (ACE_SIZEOF_LONG_DOUBLE)
# define ACE_SIZEOF_LONG_DOUBLE 12
#endif /* i386 */

#if !defined (__MINGW32__) && (defined (i386) || defined (__i386__))
  // If running an Intel, assume that it's a Pentium so that
  // ACE_OS::gethrtime () can use the RDTSC instruction.  If running a
  // 486 or lower, be sure to comment this out.  (If not running an
  // Intel CPU, this #define will not be seen because of the i386
  // protection, so it can be ignored.)
# define ACE_HAS_PENTIUM
#endif /* i386 */

#if !defined (ACE_LACKS_PRAGMA_ONCE)
  // We define it with a -D with make depend.
# define ACE_LACKS_PRAGMA_ONCE
#endif /* ! ACE_LACKS_PRAGMA_ONCE */

#define ACE_TEMPLATES_REQUIRE_SOURCE

# if defined (ACE_HAS_EXCEPTIONS)
#   define ACE_NEW_THROWS_EXCEPTIONS
# endif /* ACE_HAS_EXCEPTIONS */

#include /**/ "ace/post.h"
#endif /* ACE_LINUX_ICC_COMMON_H */
