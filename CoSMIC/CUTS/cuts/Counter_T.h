// -*- ID -*-

//=============================================================================
/**
 * @file    Counter_T.h
 *
 * $Id$
 *
 * @author James H. Hill <hillj@isis.vanderbilt.edu>
 */
//=============================================================================

#ifndef _CUTS_COUNTER_T_H_
#define _CUTS_COUNTER_T_H_

//=============================================================================
/**
 * @class CUTS_Bounded_Counter_T
 *
 * This class is a parameterized version of the bounded counter. The maximum
 * value is specified as a parameter. This allows the compiler to perform
 * an optimization since the value is known at compile-time.
 */
//=============================================================================

template <size_t MAX>
class CUTS_Bounded_Counter_T
  : public CUTS_Bounded_Counter
{
public:
  /// Constructor.
  CUTS_Bounded_Counter_T (void);

  /// Destructor.
  virtual ~CUTS_Bounded_Counter_T (void);
};

#if defined (__CUTS_INLINE__)
#include "cuts/Counter_T.inl"
#endif

#include "cuts/Counter_T.cpp"

#endif  // !defined _CUTS_COUNTER_T_H_
