// -*- C++ -*-
// $Id$

namespace ACE_OS {

  ACE_INLINE double
  floor (double x)
  {
    // This method computes the largest integral value not greater than x.
    return double (static_cast<long> (x));
  }

  ACE_INLINE double
  ceil (double x)
  {
    // This method computes the smallest integral value not less than x.
    const double floor = ACE_OS::floor (x);
    if (floor == x)
      return floor;
    else
      return floor + 1;
  }

} // ACE_OS namespace
