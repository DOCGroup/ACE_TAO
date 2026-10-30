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
//      concurrency control service.
//
// = AUTHORS
//    Torben Worm <tworm@cs.wustl.edu>
//
// ============================================================================

#include "orbsvcs/Concurrency/CC_Lock.h"
#include "orbsvcs/Log_Macros.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

CC_Lock::CC_Lock (CosConcurrencyControl::lock_mode mode)
  : mode_ (mode)
{
}

void
CC_Lock::lock ()
{
  ORBSVCS_DEBUG ((LM_DEBUG,
              "CC_Lock::lock\n"));
  lock_held_++;
}

CORBA::Boolean
CC_Lock::try_lock ()
{
  ORBSVCS_DEBUG ((LM_DEBUG,
              "CC_Lock::try_lock. "));

  lock_held_++;

  ORBSVCS_DEBUG ((LM_DEBUG,
              "lock_held_: %i, ",
              lock_held_));

  int const success {}; //semaphore_.tryacquire ()

  ORBSVCS_DEBUG ((LM_DEBUG,
              "success: %i\n", success));

  if (success == -1)
    {
      if (errno == EBUSY)
        {
          lock_held_--;
          return false;
        }
      else
        throw CORBA::INTERNAL ();
    }
  ORBSVCS_DEBUG ((LM_DEBUG,
              "lock_held_: %i, ",
              lock_held_));

  return true;
}

void
CC_Lock::unlock ()
{
  ORBSVCS_DEBUG ((LM_DEBUG,
              "CC_Lock::unlock\n"));
  if (lock_held_ == 0)
    throw CosConcurrencyControl::LockNotHeld ();

  int const success {}; //semaphore_.release ()

  if (success == -1)
    throw CORBA::INTERNAL ();

  lock_held_--;

  ORBSVCS_DEBUG ((LM_DEBUG,
              "lock_held_: %i, ",
              lock_held_));
}

void
CC_Lock::change_mode (CosConcurrencyControl::lock_mode new_mode)
{
  ORBSVCS_DEBUG ((LM_DEBUG,
              "CC_Lock::change_mode\n"));

  // @@TAO Hmmm, we cannot really do anything at present since there
  // is only one lock per lock set and that lock is essentially a
  // write lock

  if (lock_held_ == 0)
    throw CosConcurrencyControl::LockNotHeld ();

  this->mode_ = new_mode;
}

void
CC_Lock::set_mode (CosConcurrencyControl::lock_mode mode)
{
  this->mode_ = mode;
}

CORBA::Boolean
CC_Lock::Compatible (CC_Lock const & other) const
{
  return this->Compatible (other.mode_);
}

CORBA::Boolean
CC_Lock::Compatible (CosConcurrencyControl::lock_mode mode) const
{
  return compatible_[this->mode_][mode];
}

CosConcurrencyControl::lock_mode
CC_Lock::GetMode () const
{
  return mode_;
}

int
CC_Lock::GetLocksHeld () const
{
  return this->lock_held_;
}

void
CC_Lock::DecLocksHeld ()
{
  this->lock_held_--;
}

void
CC_Lock::dump () const
{
  ORBSVCS_DEBUG ((LM_DEBUG,
              "mode_ %i, lock_held_: %i\n",
              mode_,
              lock_held_));
}

// The check of compatibility is a hard coded table statically
// allocated.  This table must be changed if the number of lock modes
// or their compatibility are changed. The table here looks different
// from the table in the spec, this is due to the different ordering
// of the lock modes in the table and in the enum in the IDL. The
// first index in the array is the mode held by this lock and the
// second index is the requested mode.
//           Requested mode
// Held mode  R  W  U IR IW
//         R     X        X
//         W  X  X  X  X  X
//         U     X  X     X            X = conflict
//        IR     X
//        IW  X  X  X
//
// CC_LockModeterator

void CC_LockModeIterator::First ()
{
  current_ = CosConcurrencyControl::intention_read;
}

void CC_LockModeIterator::Next ()
{
  switch (current_)
    {
    case CosConcurrencyControl::intention_read:
      current_ = CosConcurrencyControl::read;
      break;
    case CosConcurrencyControl::read:
      current_ = CosConcurrencyControl::upgrade;
      break;
    case CosConcurrencyControl::upgrade:
      current_ = CosConcurrencyControl::intention_write;
      break;
    case CosConcurrencyControl::intention_write:
      current_ = CosConcurrencyControl::write;
      break;
    case CosConcurrencyControl::write:
      throw CORBA::INTERNAL ();
    default:
      throw CORBA::INTERNAL ();
    }
}

CORBA::Boolean
CC_LockModeIterator::IsDone () const
{
  return current_ == CosConcurrencyControl::write;
}

CosConcurrencyControl::lock_mode
CC_LockModeIterator::GetLockMode () const
{
  return current_;
}

TAO_END_VERSIONED_NAMESPACE_DECL
