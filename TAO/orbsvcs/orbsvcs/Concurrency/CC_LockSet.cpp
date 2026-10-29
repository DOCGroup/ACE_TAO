
//=============================================================================
/**
 *  @file   CC_LockSet.cpp
 *
 *  @author Torben Worm <tworm@cs.wustl.edu>
 */
//=============================================================================


#include "orbsvcs/Log_Macros.h"
#include "orbsvcs/Concurrency/CC_LockSet.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

// Default constructor.

CC_LockSet::CC_LockSet ()
{
  try
    {
      this->Init ();
    }
  catch (CORBA::Exception const &ex)
    {
      ex._tao_print_exception ("CC_LockSet::CC_LockSet ()");
    }
}

// Constructor used to create related lock sets.

CC_LockSet::CC_LockSet (CosConcurrencyControl::LockSet_ptr )
{
  try
    {
      this->Init ();
    }
  catch (CORBA::Exception const &ex)
    {
      ex._tao_print_exception ("CC_LockSet::CC_LockSet (...)");
    }
}

// Initialization.

void
CC_LockSet::Init ()
{
  // Acquire the semaphore in order to be able to put requests on hold
  if (semaphore_.acquire () == -1)
    throw CORBA::INTERNAL ();
}

// Returns true if the requested lock mode is compatible with the
// modes held. False otherwise.

bool CC_LockSet::compatible (CC_LockModeEnum mr) const
{
  for (size_t i = CC_IR; i <= CC_W; i++)
    if (this->lock_[i] > 0)
      if (!this->compatible_[i][mr])
        return false;

  return true;
}

// Locks the lock in the desired mode. Blocks until success.

void
CC_LockSet::lock (CosConcurrencyControl::lock_mode mode)
{
  ORBSVCS_DEBUG ((LM_DEBUG, "CC_LockSet::lock\n"));

  CC_LockModeEnum const lm = lmconvert (mode);

  // Check to see if the requested mode is compatible with the modes
  // held so far. If not put the request on hold.

  // @@ It's important to document somewhere that this code relies on
  // the FIFO properties of ACE_Token!
  if (this->lock_i (lm))
    if (semaphore_.acquire () == -1)
      throw CORBA::INTERNAL ();
}

// Tries to lock. If it is not possible false is returned.

CORBA::Boolean
CC_LockSet::try_lock (CosConcurrencyControl::lock_mode mode)
{
  CC_LockModeEnum const lm = lmconvert (mode);

  ORBSVCS_DEBUG ((LM_DEBUG,
              "CC_LockSet::try_lock\n"));

  return this->try_lock_i (lm);
}

// Converts the enum from the spec to the internally (ordered)
// enum.

CC_LockModeEnum
CC_LockSet::lmconvert (CosConcurrencyControl::lock_mode mode)
{
  switch (mode)
    {
    case CosConcurrencyControl::intention_read:
      return CC_IR;
    case CosConcurrencyControl::read:
      return CC_R;
    case CosConcurrencyControl::upgrade:
      return CC_U;
    case CosConcurrencyControl::intention_write:
      return CC_IW;
    case CosConcurrencyControl::write:
      return CC_W;
    default:
      return CC_EM;
    }
}

// Unlock the lock

void
CC_LockSet::unlock (CosConcurrencyControl::lock_mode mode)
{
  ORBSVCS_DEBUG ((LM_DEBUG,
              "CC_LockSet::unlock\n"));

  CC_LockModeEnum const lm = lmconvert (mode);

  ACE_GUARD (TAO_SYNCH_MUTEX, ace_mon, this->mlock_);

  if (lock_[lm] == 0) // This lock is not held.
    throw CosConcurrencyControl::LockNotHeld();
  else
    lock_[lm]--;

  // If we do not have a lock held in a weaker mode than the
  // strongest held and we have requests on the semaphore signal
  // the semaphore.
  while (lock_queue_.size () > 0)
    {
      CC_LockModeEnum lock_on_queue = CC_EM;

      lock_queue_.dequeue_head (lock_on_queue);

      if (compatible (lock_on_queue))
        {
          if (semaphore_.release () == -1)
            throw CORBA::INTERNAL ();
          lock_[lock_on_queue]++;
        }
      else
        {
          lock_queue_.enqueue_head (lock_on_queue);
          break;
        }
    }
  this->dump ();
}

// Changes the mode of a held lock.

void
CC_LockSet::change_mode (CosConcurrencyControl::lock_mode held_mode,
                         CosConcurrencyControl::lock_mode new_mode)
{
  ORBSVCS_DEBUG ((LM_DEBUG,
              "CC_LockSet::change_mode\n"));
  CC_LockModeEnum const lm_held = lmconvert (held_mode);
  CC_LockModeEnum const lm_new = lmconvert (new_mode);

  if (!this->lock_held (lm_held)) // This lock is not held
    throw CosConcurrencyControl::LockNotHeld();
  else if (this->change_mode_i (lm_held, lm_new))
    {
      this->unlock (held_mode);

      if (semaphore_.acquire () == -1)
        throw CORBA::INTERNAL ();
    }
  //  this->dump ();
}

bool
CC_LockSet::lock_i (CC_LockModeEnum lm)
{
  ACE_GUARD_RETURN (TAO_SYNCH_MUTEX, ace_mon, this->mlock_, true);
  // If the lock is not compatible with the locks we hold allready or
  // there is lock requests in the queue we cannot grant the lock and
  // thus we queue the request. Otherwise update the lock count.
  if (!compatible (lm) || lock_queue_.size () > 0)
    {
      // Put the lock mode in the queue
      lock_queue_.enqueue_tail (lm);
      this->dump ();
      return true; // Lock the semaphore.
    }
  else
    lock_[lm]++;

  this->dump ();
  return false;
}

bool
CC_LockSet::try_lock_i (CC_LockModeEnum lm)
{
  ACE_GUARD_RETURN (TAO_SYNCH_MUTEX, ace_mon, this->mlock_, true);
  // If the lock we try is compatible with the locks we hold we just
  // opdates the count. Otherwise we return false.
  if (!compatible (lm))
    {
      this->dump ();
      return false;
    }
  else
    lock_[lm]++;

  this->dump ();
  return true;
}

bool
CC_LockSet::change_mode_i (CC_LockModeEnum lm_held,
                          CC_LockModeEnum lm_new)
{
  ACE_GUARD_RETURN (TAO_SYNCH_MUTEX, ace_mon, this->mlock_, true);
  // If the new mode is compatible with the modes we hold we change
  // the counts for the two locks. If not we must queue the new
  // request. We can decrement the count for the old mode without
  // signalling the semaphore because we know we only check modes
  // granted this far.

  lock_[lm_held]--;

  if (compatible (lm_new))
    {
      lock_[lm_new]++;
      this->dump ();
      return false;
    }
  else
    {
      lock_[lm_held]++;
      lock_queue_.enqueue_tail (lm_new);
      this->dump ();
      return true;
    }
}

bool
CC_LockSet::lock_held (CC_LockModeEnum lm)
{
  ACE_GUARD_RETURN (TAO_SYNCH_MUTEX, ace_mon, this->mlock_, true);
  return lock_[lm] > 0;
}

void
CC_LockSet::dump () const
{
  ORBSVCS_DEBUG ((LM_DEBUG,
              "waiting_calls_: %i, IR: %i, R: %i, U: %i, IW: %i, W: %i\n",
              lock_queue_.size (),
              lock_[CC_IR],
              lock_[CC_R],
              lock_[CC_U],
              lock_[CC_IW],
              lock_[CC_W]));
}

bool const
CC_LockSet::compatible_[number_of_lock_modes][number_of_lock_modes] =
  {
    {true, true, true, true, false},
    {true, true, true, false, false},
    {true, true, false, false, false},
    {true, false, false, true, false},
    {false, false, false, false, false}
  };

TAO_END_VERSIONED_NAMESPACE_DECL
