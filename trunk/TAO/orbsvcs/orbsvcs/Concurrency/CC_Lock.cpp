/* -*- C++ -*- */
// $Id$

// ============================================================================
//
// = LIBRARY
//    TAO/orbsvcs/Concurrency_Service
//
// = FILENAME
//    CC_Lock.cpp
//
// = DESCRIPTION
//      This class implements a lock used by the lock set from the
//      concurrency control service
//
// = AUTHORS
//    Torben Worm <tworm@cs.wustl.edu>
//
// ============================================================================

#include "CC_Lock.h"
#include "tao/corba.h"

CC_Lock::CC_Lock(CosConcurrencyControl::lock_mode mode)
  : mode_ (mode), lock_held_ (0)
{
}

CC_Lock::~CC_Lock()
{
}

void CC_Lock::lock(CORBA::Environment &_env)
{
  ACE_DEBUG((LM_DEBUG, "CC_Lock::lock\n"));
  lock_held_++;
  int success = semaphore_.acquire();
  if(success==-1) {
    TAO_THROW(CORBA::INTERNAL(CORBA::COMPLETED_NO));
  }
}

CORBA::Boolean CC_Lock::try_lock(CORBA::Environment &_env)
{
  ACE_DEBUG((LM_DEBUG, "CC_Lock::try_lock. "));
  lock_held_++;
  ACE_DEBUG((LM_DEBUG, "lock_held_: %i, ", lock_held_));
  int success = semaphore_.tryacquire();
  ACE_DEBUG((LM_DEBUG, "success: %i\n", success));
  if(success==-1) {
    if(errno==EBUSY) {
      lock_held_--;
      return CORBA::B_FALSE;
    }
    else
      TAO_THROW_RETURN(CORBA::INTERNAL(CORBA::COMPLETED_NO), CORBA::B_FALSE);
  }
  //  else {
  //    this->lock(_env);
  //  }
}

void CC_Lock::unlock(CORBA::Environment &_env)
{
  ACE_DEBUG((LM_DEBUG, "CC_Lock::unlock\n"));
  if(lock_held_==0)
    TAO_THROW(CosConcurrencyControl::LockNotHeld);
  int success = semaphore_.release();
  if(success==-1) {
    TAO_THROW(CORBA::INTERNAL(CORBA::COMPLETED_NO));
  }
  lock_held_--;
}

void CC_Lock::change_mode(CosConcurrencyControl::lock_mode new_mode,
                          CORBA::Environment &_env)
{
  ACE_DEBUG((LM_DEBUG, "CC_Lock::change_mode\n"));
  // @@TAO Hmmm, we cannot really do anything at present since there is
  // only one lock per lock set and that lock is essentially a write lock
  if(lock_held_==0)
    TAO_THROW(CosConcurrencyControl::LockNotHeld);
  this->mode_ = new_mode;
}
  
CORBA::Boolean CC_Lock::Compatible(const CC_Lock &other)
{
  return this->Compatible(other.mode_);
}

CORBA::Boolean CC_Lock::Compatible(CosConcurrencyControl::lock_mode mode)
{
  return compatible_[this->mode_][mode];
}

CosConcurrencyControl::lock_mode CC_Lock::GetMode(void)
{
  return mode_;
}

// The check of compatibility is a hard coded table statically allocated
// This table must be changed if the number of lock modes or their
// compatibility are changed. 5 = number of lock modes
CORBA::Boolean CC_Lock::compatible_[5][5] ={
  {CORBA::B_TRUE, CORBA::B_TRUE, CORBA::B_TRUE, CORBA::B_TRUE, CORBA::B_FALSE},
  {CORBA::B_TRUE, CORBA::B_TRUE, CORBA::B_TRUE, CORBA::B_FALSE, CORBA::B_FALSE},
  {CORBA::B_TRUE, CORBA::B_TRUE, CORBA::B_FALSE, CORBA::B_FALSE, CORBA::B_FALSE},
  {CORBA::B_TRUE, CORBA::B_FALSE, CORBA::B_FALSE, CORBA::B_FALSE, CORBA::B_TRUE},
  {CORBA::B_FALSE, CORBA::B_FALSE, CORBA::B_FALSE, CORBA::B_FALSE, CORBA::B_FALSE}};

//#if defined (__ACE_INLINE__)
//#include "CC_Lock.i"
//#endif // defined INLINE
