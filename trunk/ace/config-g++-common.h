/* -*- C++ -*- */
// $Id$

// This configuration file is designed to be included by another,
// specific configuration file.  It provides config information common
// to all g++ platforms, including egcs.

#if !defined (ACE_GNUG_COMMON_H)
#define ACE_GNUG_COMMON_H

#if __GNUC__ > 2 || ( __GNUC__ == 2 && __GNUC_MINOR__ >= 8)
  // egcs or g++ >= 2.8.0

# if __GNUC__ == 2 && __GNUC_MINOR__ >= 90
    // egcs or really modern g++.  Do these work with g++ 2.8.0?
#   define ACE_HAS_ANSI_CASTS
#   define ACE_HAS_CPLUSPLUS_HEADERS
#   define ACE_HAS_STDCPP_STL_INCLUDES
#   define ACE_HAS_TEMPLATE_TYPEDEFS
#   define ACE_HAS_TYPENAME_KEYWORD
    // It has gets (), but warns about using it (on Linux) at link time.
#   define ACE_LACKS_GETS
# endif /* __GNUC__ >= 2.90 */

# if __GNUC__ == 2 && __GNUC_MINOR__ != 91
#   // egcs 2.91 apparently has a bug with this . . .
#   define ACE_HAS_TEMPLATE_SPECIALIZATION
# endif /* __GNUC__ != 2.91 */

  // Some versions of egcs, e.g., egcs-2.90.27 980315 (egcs-1.0.2 release),
  // have bugs with static data members in template classes.
# define ACE_LACKS_STATIC_DATA_MEMBER_TEMPLATES

  // __EXCEPTIONS is defined with -fexceptions, the egcs default.  It
  // is not defined with -fno-exceptions, the ACE default for g++.
  // ACE_HAS_EXCEPTIONS is defined in
  // include/makeinclude/wrapper_macros.GNU, so this really isn't
  // necessary.  Just in case . . .
# if defined (__EXCEPTIONS) && !defined (ACE_HAS_EXCEPTIONS)
#   define ACE_HAS_EXCEPTIONS
# endif /* __EXCEPTIONS && ! ACE_HAS_EXCEPTIONS */

#else  /* ! EGCS */
  // Plain old g++.
# define ACE_LACKS_STATIC_DATA_MEMBER_TEMPLATES
# define ACE_HAS_TEMPLATE_SPECIALIZATION
#endif /* ! EGCS */

#if (defined (i386) || defined (__i386__)) && !defined (ACE_SIZEOF_LONG_DOUBLE)
# define ACE_SIZEOF_LONG_DOUBLE 12
#endif /* i386 */

#if defined (i386) || defined (__i386__)
  // If running an Intel, assume that it's a Pentium so that
  // ACE_OS::gethrtime () can use the RDTSC instruction.  If
  // running a 486 or lower, be sure to comment this out.
  // (If not running an Intel CPU, this #define will not be seen
  //  because of the i386 protection, so it can be ignored.)
# define ACE_HAS_PENTIUM
#endif /* i386 */

#define ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION
#define ACE_TEMPLATES_REQUIRE_SOURCE

#endif /* ACE_GNUG_COMMON_H */
