/* -*- C++ -*- */
// $Id$

// ============================================================================
//
// = LIBRARY
//    cos
// 
// = FILENAME
//   CC_LockSet.cpp
//
// = AUTHOR
//    Torben Worm <tworm@cs.wustl.edu>
// 
// ============================================================================

#include "CC_LockSet.h"

// Default constructor. We cannot create the lock at this point
// because we are not locking anything, and we do not know the mode of
// the lock. In a full fledged version the lock set (implementation)
// will be instantiated here

CC_LockSet::CC_LockSet (void)
  : related_lockset_ (0),
    lock_ (0)
{
  // Do nothing
}

// Constructor used to create related lock sets.

CC_LockSet::CC_LockSet (CosConcurrencyControl::LockSet_ptr related)
  : related_lockset_ (related),
    lock_ (0)
{
  // Do nothing
}

// Destructor. Remove the lock if it exists. In a later version the
// lock set must be removed here. Actually if all locks have been
// unlocked no lock instances should exist at this point.

CC_LockSet::~CC_LockSet (void)
{
  if (lock_!=0)
    delete lock_;

  lock_ = 0;
}

// Locks the lock in the desired mode. Blocks until success. In a
// later version the lock set should be searched for incompatible
// locks.

void 
CC_LockSet::lock (CosConcurrencyControl::lock_mode mode,
                  CORBA::Environment &_env)
{
  ACE_DEBUG ((LM_DEBUG, "CC_LockSet::lock\n"));
  if (lock_ == 0) 
    {
      ACE_NEW (lock_, CC_Lock (mode));
      if (errno == ENOMEM) 
        {
          _env.clear ();
          _env.exception (new CORBA::NO_MEMORY (CORBA::COMPLETED_NO));
          _env.print_exception ("CC_Lock::lock ()");
        }
    }
  TAO_TRY 
    {
      lock_->lock (_env);
    }
  TAO_CATCHANY 
    {
      TAO_RETHROW;
    }
  TAO_ENDTRY;
}

// Tries to lock. If it is not possible false is returned. Comments
// for lock holds for later version.

CORBA::Boolean 
CC_LockSet::try_lock (CosConcurrencyControl::lock_mode mode,
                      CORBA::Environment &_env)
{
  ACE_DEBUG ((LM_DEBUG,
              "CC_LockSet::try_lock\n"));
  TAO_TRY 
    {
      return lock_->try_lock (_env);
    }
  TAO_CATCHANY 
    {
      TAO_RETHROW_RETURN (CORBA::B_FALSE);
    }
  TAO_ENDTRY;
  return CORBA::B_TRUE;
}

// Drops the specified lock. In this simple case we have only one lock
// at any time so we just drop that lock.

void 
CC_LockSet::unlock (CosConcurrencyControl::lock_mode mode,
                    CORBA::Environment &_env)
{
  ACE_DEBUG ((LM_DEBUG,
              "CC_LockSet::unlock\n"));
  TAO_TRY 
    {
      lock_->unlock (_env);
    }
  TAO_CATCHANY 
    {
      TAO_RETHROW;
    }
  TAO_ENDTRY;
}

// Changes the mode of a held lock. In this version we deal with only
// one type of lock (in reality) and therefore the type is of no
// meaning.

void 
CC_LockSet::change_mode (CosConcurrencyControl::lock_mode held_mode,
                         CosConcurrencyControl::lock_mode new_mode,
                         CORBA::Environment &_env)
{
  ACE_DEBUG ((LM_DEBUG,
              "CC_LockSet::change_mode\n"));
  TAO_TRY
    {
      lock_->change_mode (new_mode, _env);
    }
  TAO_CATCHANY
    {
      TAO_RETHROW;
    }
  TAO_ENDTRY;
}

