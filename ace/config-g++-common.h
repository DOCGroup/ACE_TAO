/* -*- C++ -*- */
// $Id$

// This configuration file is designed to be included by another,
// specific configuration file.  It provides config information common
// to all g++ platforms, including egcs.

#if !defined (ACE_GNUG_COMMON_H)
#define ACE_GNUG_COMMON_H

#if __GNUC__ > 2 || ( __GNUC__ == 2 && __GNUC_MINOR__ >= 90)
  // egcs or really modern g++
# define ACE_HAS_ANSI_CASTS
# define ACE_HAS_CPLUSPLUS_HEADERS
# define ACE_HAS_STDCPP_STL_INCLUDES
# define ACE_HAS_TEMPLATE_TYPEDEFS
# define ACE_HAS_TYPENAME_KEYWORD

#elif __GNUC__ > 2 || ( __GNUC__ == 2 && __GNUC_MINOR__ >= 8)
  // egcs or g++ >= 2.8.0
// __EXCEPTIONS is defined with -fexceptions, the default.  It is not
// defined with -fno-exceptions.
# if defined (__EXCEPTIONS) && !defined (ACE_HAS_EXCEPTIONS)
#   define ACE_HAS_EXCEPTIONS
# endif /* __EXCEPTIONS && ! ACE_HAS_EXCEPTIONS */

#else  /* ! EGCS */
  // Plain old g++.
# define ACE_LACKS_STATIC_DATA_MEMBER_TEMPLATES
#endif /* ! EGCS */

#define ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION
#define ACE_HAS_TEMPLATE_SPECIALIZATION
#define ACE_TEMPLATES_REQUIRE_SOURCE

#endif /* ACE_GNUG_COMMON_H */
