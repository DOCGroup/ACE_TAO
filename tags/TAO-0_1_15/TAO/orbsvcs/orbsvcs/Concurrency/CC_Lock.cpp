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

// #include "tao/corba.h"
#include "CC_Lock.h"

CC_Lock::CC_Lock(CosConcurrencyControl::lock_mode mode) : mode_ (mode)
{
  semaphore_.open("test"); // @@ The semaphore must have a unique name??
}

CC_Lock::~CC_Lock()
{
  semaphore_.close();
}

void CC_Lock::lock(void)
{
  semaphore_.acquire();
}

CORBA::Boolean CC_Lock::try_lock()
{
  semaphore_.tryacquire(); //@@ What does tryacquire return??
  return CORBA::B_TRUE;
}

void CC_Lock::unlock(void)
{
  semaphore_.release();
}

void CC_Lock::change_mode(CosConcurrencyControl::lock_mode new_mode)
{
  // @@TAO Hmmm, we cannot really do anything at present since there is
  // only one lock per lock set and that lock is essentially a write lock
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
