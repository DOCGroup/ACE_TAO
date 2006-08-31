// $Id$

//
// repetitions
//
CUTS_INLINE
size_t CUTS_Operation_Measurement::repetitions (void) const
{
  return this->reps_;
}

//
// time
//
CUTS_INLINE
const CUTS_Time_Measurement & CUTS_Operation_Measurement::time (void) const
{
  return this->time_;
}
