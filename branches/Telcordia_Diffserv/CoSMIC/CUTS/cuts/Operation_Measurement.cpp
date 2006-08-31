#include "cuts/Operation_Measurement.h"

#if !defined (__CUTS_INLINE__)
#include "cuts/Operation_Measurement.inl"
#endif

//
// CUTS_Operation_Measurement
//
CUTS_Operation_Measurement::CUTS_Operation_Measurement (void)
: reps_ (0)
{

}

CUTS_Operation_Measurement::CUTS_Operation_Measurement (const CUTS_Operation_Measurement &om)
: reps_ (om.reps_),
  time_ (om.time_)
{

}

//
// ~CUTS_Operation_Measurement
//
CUTS_Operation_Measurement::~CUTS_Operation_Measurement (void)
{

}

//
// reset
//
void CUTS_Operation_Measurement::reset (void)
{
  this->reps_ = 0;
  this->time_.reset ();
}

//
// operator =
//
const CUTS_Operation_Measurement &
CUTS_Operation_Measurement::operator = (const CUTS_Operation_Measurement &om)
{
  this->reps_ = om.reps_;
  this->time_ = om.time_;
  return *this;
}

//
// operator ()
//
void CUTS_Operation_Measurement::operator () (size_t reps, const ACE_Time_Value &tv)
{
  this->reps_ += reps;
  this->time_ += tv;
}
