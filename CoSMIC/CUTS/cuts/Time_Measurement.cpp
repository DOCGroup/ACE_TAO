// $Id$

#include "cuts/Time_Measurement.h"

#if !defined (__CUTS_INLINE__)
#include "cuts/Time_Measurement.inl"
#endif

//=============================================================================
/*
 * CUTS_Timing_Measurement
 */
//=============================================================================

//
// CUTS_Timing_Measurement
//
CUTS_Time_Measurement::CUTS_Time_Measurement (void)
: count_ (0),
  sum_ (ACE_Time_Value::zero),
  max_ (ACE_Time_Value::zero),
  min_ (ACE_Time_Value::zero)
{

}

CUTS_Time_Measurement::CUTS_Time_Measurement (const CUTS_Time_Measurement &tm)
: count_ (tm.count_),
  sum_ (tm.sum_),
  max_ (tm.max_),
  min_ (tm.min_)
{

}

//
// ~CUTS_Timing_Measurement
//
CUTS_Time_Measurement::~CUTS_Time_Measurement (void)
{

}

//
// reset
//
void CUTS_Time_Measurement::reset (void)
{
  // Set all the time values to <zero>.
  this->sum_ = ACE_Time_Value::zero;
  this->max_ = ACE_Time_Value::zero;
  this->min_ = ACE_Time_Value::zero;

  // Reset the <reset_> flag.
  this->count_ = 0;
}

//
// operator +=
//
void CUTS_Time_Measurement::operator += (const ACE_Time_Value & time_value)
{
  // If the <reset_> flag is set, then this is the time value in
  // the timing measurement.
  if (this->count_ != 0)
  {
    // Determine if this new <Time_Value> is greater.
    if (time_value > this->max_)
      this->max_ = time_value;
    // Determine if this new <Time_Value> is lesser.
    else if (time_value < this->min_)
      this->min_ = time_value;
  }
  else
  {
    // This is the first time measurement. Therefore, set both the
    // <min_> and <max_> time values to this <tv>.
    this->max_ = time_value;
    this->min_ = time_value;
  }

  // Accumulate the new time value.
  this->sum_ += time_value;
  this->count_ ++;
}
