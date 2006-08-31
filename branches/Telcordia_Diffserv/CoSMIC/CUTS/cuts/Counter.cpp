// $Id$

#include "cuts/Counter.h"
#if !defined (__CUTS_INLINE__)
# include "cuts/Counter.inl"
#endif

#include "ace/Guard_T.h"

//=============================================================================
/*
 * CUTS_Counter
 */
//=============================================================================

//
// CUTS_Counter
//
CUTS_Counter::CUTS_Counter (void)
: count_ (0)
{

}

//
// CUTS_Counter
//
CUTS_Counter::~CUTS_Counter (void)
{

}

//
// increment
//
void CUTS_Counter::increment (void)
{
  ACE_GUARD (ACE_Thread_Mutex, guard, this->lock_);
  this->count_ ++;
}

//
// reset
//
void CUTS_Counter::reset (void)
{
  ACE_GUARD (ACE_Thread_Mutex, guard, this->lock_);
  this->count_ = 0;
}

//
// count
//
size_t CUTS_Counter::count (void)
{
  ACE_GUARD_RETURN (ACE_Thread_Mutex, guard, this->lock_, this->count_);
  size_t retval = this->count_;
  return retval;
}

//=============================================================================
/*
 * CUTS_Bounded_Counter
 */
//=============================================================================

//
// CUTS_Bounded_Counter
//
CUTS_Bounded_Counter::CUTS_Bounded_Counter (size_t max_count)
: max_count_ (max_count),
  full_ (false)
{
  if (this->max_count_ == 0)
    this->full_ = true;
}

//
// ~CUTS_Bounded_Counter
//
CUTS_Bounded_Counter::~CUTS_Bounded_Counter (void)
{

}

//
// increment
//
void CUTS_Bounded_Counter::increment (void)
{
  ACE_GUARD (ACE_Thread_Mutex, guard, this->lock_);

  if (!this->full_)
  {
    // Increment the counter and determine if the counter is
    // full.
    this->count_ ++;
    this->full_ = (this->count_ >= this->max_count_);
  }
}

//
// reset
//
void CUTS_Bounded_Counter::reset (void)
{
  ACE_GUARD (ACE_Thread_Mutex, guard, this->lock_);

  this->full_ = false;
  this->count_ = 0;
}

