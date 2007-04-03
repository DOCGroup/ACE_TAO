// -*- C++ -*-
//
// $Id$

ACE_BEGIN_VERSIONED_NAMESPACE_DECL

namespace ACE_OS {

  ACE_INLINE double
  floor (double x)
  {
    // This method computes the largest integral value not greater than x.
    if(x > 0)
      return static_cast<long> (x);
    else if (static_cast<long> (x) == x)
      return x;
    else
      return static_cast<long>(x) - 1;
  }

  ACE_INLINE double
  ceil (double x)
  {
    // This method computes the smallest integral value not less than x.
    if (x < 0)
      return static_cast<long> (x);
    else if (static_cast<long> (x) == x)
      return x;
    else
      return static_cast<long> (x) + 1;
  }

} // ACE_OS namespace

ACE_END_VERSIONED_NAMESPACE_DECL
