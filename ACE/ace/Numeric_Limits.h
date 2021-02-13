// -*- C++ -*-

//=============================================================================
/**
 * @file    Numeric_Limits.h
 *
 * Traits containing basic integer limits.  Useful for template-based
 * code on platforms that lack @c std::numeric_limits<>.
 *
 * @note These traits are not meant to be a replacement for
 *       @c std::numeric_limits<>.  Rather they are a crutch until all
 *       ACE-supported platforms also support
 *       @c std::numeric_limits<>.
 *
 * @internal   Only meant for internal use by ACE.
 * @deprecated This header will be removed once all platforms
 *             supported by ACE support @c std::numeric_limits<>.
 *
 * @author  Ossama Othman <ossama_othman at symantec dot com>
 */
//=============================================================================

#ifndef ACE_NUMERIC_LIMITS_H
#define ACE_NUMERIC_LIMITS_H

#include /**/ "ace/pre.h"

#include /**/ "ace/ACE_export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
#  pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#if defined __MINGW32__
// Windows defines min/max macros that interfere with the
// numeric_limits::min/max() traits.  Undefine those macros before
// including <limits>.
//
// Ideally, we could prevent those macros from being defined by
// defining the Windows-specific NOMINMAX symbol before any Windows
// headers are included, preferably on the command line.  However,
// that would probably break some applications.
//
// @@ Why isn't this a problem with MSVC++ and Borland builds?
# undef min
# undef max
#endif  /* __MINGW32__ */

#include <limits>

// Address global namespace pollution potentially incurred by some
// platforms.
#undef min
#undef max

ACE_BEGIN_VERSIONED_NAMESPACE_DECL

// std::numeric_limits<> has all of the necessary specializations.
// Just wrap it.
template <typename T>
struct ACE_Numeric_Limits
{
  static T min () { return std::numeric_limits<T>::min (); }
  static T max () { return std::numeric_limits<T>::max (); }
};

ACE_END_VERSIONED_NAMESPACE_DECL

#include /**/ "ace/post.h"

#endif  /* ACE_NUMERIC_LIMITS_H */
