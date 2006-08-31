// $Id$

//
// average_time
//
CUTS_INLINE
long CUTS_Time_Metric::average_time (void)
{
  ACE_READ_GUARD_RETURN (
    ACE_RW_Thread_Mutex,
    guard,
    this->lock_,
    this->average_time_.msec ());
  return this->average_time_.msec ();
}

//
// best_time
//
CUTS_INLINE
long CUTS_Time_Metric::best_time (void)
{
  ACE_READ_GUARD_RETURN (
    ACE_RW_Thread_Mutex,
    guard,
    this->lock_,
    this->best_time_.msec ());
  return this->best_time_.msec ();
}

//
// worse_time
//
CUTS_INLINE
long CUTS_Time_Metric::worse_time (void)
{
  ACE_READ_GUARD_RETURN (
    ACE_RW_Thread_Mutex,
    guard,
    this->lock_,
    this->worse_time_.msec ());
  return this->worse_time_.msec ();
}

//
// count
//
CUTS_INLINE
long CUTS_Time_Metric::count (void)
{
  ACE_READ_GUARD_RETURN (
    ACE_RW_Thread_Mutex,
    guard,
    this->lock_,
    this->count_);
  return this->count_;
}

//
// timestamp
//
CUTS_INLINE
const ACE_Time_Value & CUTS_Time_Metric::timestamp (void) const
{
  return this->timestamp_;
}

//
// timestamp
//
CUTS_INLINE
void CUTS_Time_Metric::timestamp (const ACE_Time_Value & timestamp)
{
  this->timestamp_ = timestamp;
}
