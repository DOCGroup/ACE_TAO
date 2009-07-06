// -*- C++ -*-
//
// $Id$

ACE_BEGIN_VERSIONED_NAMESPACE_DECL

namespace ACE_OS {

  ACE_INLINE double
  floor (double x)
  {
#if defined (ACE_LACKS_FLOOR)
    // This method computes the largest integral value not greater than x.
    if(x > 0)
      return static_cast<long> (x);
    else if ((static_cast<long> (x) < x) || (static_cast<long> (x) > x) )
      return static_cast<long>(x) - 1;
    else
      return x;
#else
    return ::floor (x);
#endif
  }

  ACE_INLINE double
  ceil (double x)
  {
#if defined (ACE_LACKS_CEIL)
    // This method computes the smallest integral value not less than x.
    if (x < 0)
      return static_cast<long> (x);
    else if ((static_cast<long> (x) < x) || (static_cast<long> (x) > x))
      return static_cast<long> (x) + 1;
    else
      return x;
#else
    return ::ceil (x);
#endif
  }

  ACE_INLINE double
  log2 (double x)
  {
    return ace_log2_helper (x);
  }

} // ACE_OS namespace

ACE_END_VERSIONED_NAMESPACE_DECL
