// $Id$

//=============================================================================
/*
 * CUTS_Timing_Measurement
 */
//=============================================================================

//
// maximum
//
CUTS_INLINE
const ACE_Time_Value & CUTS_Time_Measurement::maximum (void) const
{
  return this->max_;
}

//
// minimum
//
CUTS_INLINE
const ACE_Time_Value & CUTS_Time_Measurement::minimum (void) const
{
  return this->min_;
}

//
// total
//
CUTS_INLINE
const ACE_Time_Value & CUTS_Time_Measurement::accumulation (void) const
{
  return this->sum_;
}

//
// average
//
CUTS_INLINE
size_t CUTS_Time_Measurement::count (void) const
{
  return this->count_;
}
