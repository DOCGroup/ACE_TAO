// -*- C++ -*-

//=============================================================================
/**
 * @file    Versioned_Namespace.h
 *
 * Versioned namespace support.
 *
 * Useful for preventing conflicts when using a third party library.
 *
 * @author Ossama Othman <ossama@dre.vanderbilt.edu>
 */
//=============================================================================

#ifndef TAO_VERSIONED_NAMESPACE_H
#define TAO_VERSIONED_NAMESPACE_H

#if !defined (TAO_ORBCONF_H) && !defined (ACE_CONFIG_MACROS_H)
# error This header is only meant to be included after "tao/orbconf.h".
#endif  /* !TAO_ORBCONF_H */


#if !defined (TAO_HAS_VERSIONED_NAMESPACE) \
  && defined (ACE_HAS_VERSIONED_NAMESPACE) \
  && ACE_HAS_VERSIONED_NAMESPACE == 1
# define TAO_HAS_VERSIONED_NAMESPACE 1
#endif  /* !TAO_HAS_VERSIONED_NAMESPACE
           && ACE_HAS_VERSIONED_NAMESPACE == 1*/

#if defined (TAO_HAS_VERSIONED_NAMESPACE) && TAO_HAS_VERSIONED_NAMESPACE == 1

# ifndef TAO_VERSIONED_NAMESPACE_NAME
#  include "tao/Version.h"

// Preprocessor symbols will not be expanded if they are
// concatenated.  Force the preprocessor to expand them during the
// argument prescan by calling a macro that itself calls another that
// performs the actual concatenation.
#  define TAO_MAKE_VERSIONED_NAMESPACE_NAME_IMPL(MAJOR,MINOR,MICRO) TAO_ ## MAJOR ## _ ## MINOR ## _ ## MICRO
#  define TAO_MAKE_VERSIONED_NAMESPACE_NAME(MAJOR,MINOR,MICRO) TAO_MAKE_VERSIONED_NAMESPACE_NAME_IMPL(MAJOR,MINOR,MICRO)
#  define TAO_VERSIONED_NAMESPACE_NAME TAO_MAKE_VERSIONED_NAMESPACE_NAME(TAO_MAJOR_VERSION,TAO_MINOR_VERSION,TAO_MICRO_VERSION)
# endif  /* !TAO_VERSIONED_NAMESPACE_NAME */

# define TAO_BEGIN_VERSIONED_NAMESPACE_DECL namespace TAO_VERSIONED_NAMESPACE_NAME {
# define TAO_END_VERSIONED_NAMESPACE_DECL } \
  using namespace TAO_VERSIONED_NAMESPACE_NAME;

#else

# define TAO_VERSIONED_NAMESPACE_NAME
# define TAO_BEGIN_VERSIONED_NAMESPACE_DECL
# define TAO_END_VERSIONED_NAMESPACE_DECL

#endif  /* TAO_HAS_VERSIONED_NAMESPACE */

#endif  /* !TAO_VERSIONED_NAMESPACE_H */
