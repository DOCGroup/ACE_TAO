// $Id$

#ifndef ACE_LINUX_ICC_COMMON_H
#define ACE_LINUX_ICC_COMMON_H
#include "ace/pre.h"

// Initial config file for the Intel compiler.  There are probably many things
// that could or should be defined, but this seems to work.


#if defined (i386) || defined (__i386__)
  // If running an Intel, assume that it's a Pentium so that
  // ACE_OS::gethrtime () can use the RDTSC instruction.  If
  // running a 486 or lower, be sure to comment this out.
  // (If not running an Intel CPU, this #define will not be seen
  //  because of the i386 protection, so it can be ignored.)
# define ACE_HAS_PENTIUM
#endif /* i386 */

#define ACE_TEMPLATES_REQUIRE_SOURCE

#define ACE_HAS_USING_KEYWORD

#define ACE_LACKS_ACE_IOSTREAM 1

#define ACE_HAS_CPLUSPLUS_HEADERS 1

// Define this if you want to use the standard C++ library
#define ACE_HAS_STANDARD_CPP_LIBRARY 1

#define ACE_HAS_STRING_CLASS
#define ACE_HAS_EXCEPTIONS 1
#define ACE_NEW_THROWS_EXCEPTIONS

#include "ace/post.h"
#endif /* ACE_LINUX_ICC_COMMON_H */

