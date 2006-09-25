// -*- C++ -*-
//=============================================================================
/**
 * @file Truncate.h
 *
 * $Id$
 *
 * @author Steve Huston <shuston@riverace.com>
 */
//=============================================================================
#ifndef ACE_TRUNCATE_H
#define ACE_TRUNCATE_H
#include /**/ "ace/pre.h"

#include /**/ "ace/config-all.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "ace/Global_Macros.h"

#if !defined(ACE_LACKS_NUMERIC_LIMITS)
// some platforms pollute the namespace by defining max() and min() macros
#  ifdef max
#    undef max
#  endif
#  ifdef min
#    undef min
#  endif
#  include <limits>
#else
#  include "ace/os_include/os_limits.h"
#endif /* ACE_LACKS_NUMERIC_LIMITS */

ACE_BEGIN_VERSIONED_NAMESPACE_DECL

namespace ACE_Utils
{
/**
 * @class Truncate
 *
 * @brief Helper function to truncate an integral value to an int.
 *
 *        Very useful since ACE methods return int very often and the value's
 *        source is often a different-size integral type, such as size_t.
 *        This function hides the truncation logic and resolves compiler
 *        diagnostics.
 *
 * @internal Internal use only.
 */
template<typename X>
inline int Truncate (const X& val)
{
#if !defined (ACE_LACKS_NUMERIC_LIMITS)
  if (val > static_cast<X> (std::numeric_limits<int>::max ()))
    return std::numeric_limits<int>::max ();
#else
  if (val > static_cast<X> (INT_MAX))
    return INT_MAX;
#endif /* ACE_LACKS_NUMERIC_LIMITS */
  return static_cast<int> (val);
}

// Specialize one for size_t to alleviate the explicit instantiation pain.
template<>
inline int Truncate<size_t> (const size_t& val)
{
#if !defined (ACE_LACKS_NUMERIC_LIMITS)
  if (val > static_cast<size_t> (std::numeric_limits<int>::max ()))
    return std::numeric_limits<int>::max ();
#else
  if (val > static_cast<size_t> (INT_MAX))
    return INT_MAX;
#endif /* ACE_LACKS_NUMERIC_LIMITS */
  return static_cast<int> (val);
}

} // namespace ACE_Utils

ACE_END_VERSIONED_NAMESPACE_DECL

#include /**/ "ace/post.h"
#endif /* ACE_TRUNCATE_H*/
