// -*- C++ -*-
//
// $Id$

#if !defined(ACE_LACKS_NUMERIC_LIMITS)
// some platforms pollute the namespace by defining max() and min() macros
#ifdef max
#undef max
#endif
#ifdef min
#undef min
#endif
#include <limits>
#else
#include "ace/os_include/os_limits.h"
#endif /* ACE_LACKS_NUMERIC_LIMITS */

ACE_BEGIN_VERSIONED_NAMESPACE_DECL

template<typename X> ACE_INLINE_TEMPLATE_FUNCTION int
ACE_Utils::Truncate (const X &val)
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

template<> ACE_INLINE_TEMPLATE_FUNCTION int
ACE_Utils::Truncate<size_t> (const size_t &val)
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

ACE_END_VERSIONED_NAMESPACE_DECL
