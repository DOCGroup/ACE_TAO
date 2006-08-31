// $Id$

#include "cuts/Time_Metric.h"

#if !defined (__CUTS_INLINE__)
#include "cuts/Time_Metric.inl"
#endif

#include "cuts/System_Metrics_Visitor.h"

//
// CUTS_Time_Metric
//
CUTS_Time_Metric::CUTS_Time_Metric (void)
: count_ (0),
  best_time_ (ACE_Time_Value::zero),
  worse_time_ (ACE_Time_Value::zero),
  average_time_ (ACE_Time_Value::zero)
{

}

CUTS_Time_Metric::CUTS_Time_Metric (const CUTS_Time_Metric & tm)
: count_ (tm.count_),
  best_time_ (tm.best_time_),
  worse_time_ (tm.worse_time_),
  average_time_ (tm.average_time_)
{

}

//
// ~CUTS_Time_Metric
//
CUTS_Time_Metric::~CUTS_Time_Metric (void)
{

}

//
// operator =
//
const CUTS_Time_Metric &
CUTS_Time_Metric::operator = (const CUTS_Time_Metric & tm)
{
  ACE_WRITE_GUARD_RETURN (ACE_RW_Thread_Mutex, guard, this->lock_, *this);

  this->count_ = tm.count_;
  this->average_time_ = tm.average_time_;
  this->best_time_ = tm.best_time_;
  this->worse_time_ = tm.worse_time_;

  return *this;
}

//
// update
//
void CUTS_Time_Metric::update (size_t count,
                               long avg,
                               long best,
                               long worse)
{
  ACE_WRITE_GUARD (ACE_RW_Thread_Mutex, guard, this->lock_);

  this->count_ = count;
  this->average_time_.msec (avg);
  this->best_time_.msec (best);
  this->worse_time_.msec (worse);
}

//
// reset
//
void CUTS_Time_Metric::reset (void)
{
  ACE_WRITE_GUARD (ACE_RW_Thread_Mutex, guard, this->lock_);

  this->count_ = 0;
  this->best_time_ = ACE_Time_Value::zero;
  this->worse_time_ = ACE_Time_Value::zero;
  this->average_time_ = ACE_Time_Value::zero;
}

//
// operator +
//
const CUTS_Time_Metric operator + (const CUTS_Time_Metric & lhs,
                                   const CUTS_Time_Metric & rhs)
{
  CUTS_Time_Metric time_metric (lhs);
  time_metric += rhs;
  return time_metric;
}

//
// operator +=
//
const CUTS_Time_Metric &
CUTS_Time_Metric::operator += (const CUTS_Time_Metric & rhs)
{
  ACE_WRITE_GUARD_RETURN (ACE_RW_Thread_Mutex, guard, this->lock_, *this);

  this->average_time_ += rhs.average_time_;
  this->best_time_ += rhs.best_time_;
  this->worse_time_ += rhs.worse_time_;
  return *this;
}

//
// accept
//
void CUTS_Time_Metric::accept (CUTS_System_Metrics_Visitor & visitor)
{
  visitor.visit_time_metrics (*this);
}
