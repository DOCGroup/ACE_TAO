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

ACE_RCSID(Concurrency, CC_LockSet, "$Id$")

// Default constructor.

CC_LockSet::CC_LockSet (void)
  : related_lockset_ (0)
{
  TAO_TRY
    {
      this->Init (TAO_TRY_ENV);
      TAO_CHECK_ENV;
    }
  TAO_CATCHANY
    {
      TAO_TRY_ENV.print_exception ("CC_LockSet::CC_LockSet (void)");
    }
  TAO_ENDTRY;
}

// Constructor used to create related lock sets.

CC_LockSet::CC_LockSet (CosConcurrencyControl::LockSet_ptr related)
  : related_lockset_ (related)
{
  TAO_TRY
    {
      this->Init (TAO_TRY_ENV);
      TAO_CHECK_ENV;
    }
  TAO_CATCHANY
    {
      TAO_TRY_ENV.print_exception ("CC_LockSet::CC_LockSet (...)");
    }
  TAO_ENDTRY;
}

// Initialization.

void
CC_LockSet::Init (CORBA::Environment &TAO_IN_ENV)
{
  // Set the mode of the statically allocated locks
  lock_[CC_IR] = 0;
  lock_[CC_R] = 0;
  lock_[CC_U] = 0;
  lock_[CC_IW] = 0;
  lock_[CC_W] = 0;

  // Acquire the semaphore in order to be able to put requests on hold
  if (semaphore_.acquire () == -1)
    TAO_THROW (CORBA::INTERNAL (CORBA::COMPLETED_NO));
}

// Destructor

CC_LockSet::~CC_LockSet (void)
{
}

// Returns true if the requested lock mode is compatible with the
// modes held. False otherwise.

CORBA::Boolean CC_LockSet::compatible (CC_LockModeEnum mr)
{
  for (size_t i = CC_IR; i <= CC_W; i++)
    if (this->lock_[i] > 0)
      if (this->compatible_[i][mr] == 0)
        return 0;

  return 1;
}

// Locks the lock in the desired mode. Blocks until success.

void
CC_LockSet::lock (CosConcurrencyControl::lock_mode mode,
                  CORBA::Environment &TAO_IN_ENV)
{
  ACE_DEBUG ((LM_DEBUG, "CC_LockSet::lock\n"));

  CC_LockModeEnum lm = lmconvert (mode);

  // Check to see if the requested mode is compatible with the modes
  // held so far. If not put the request on hold.

  // @@ It's important to document somewhere that this code relies on
  // the FIFO properties of ACE_Token!
  if (this->lock_i (lm) == 1)
    if (semaphore_.acquire () == -1)
      TAO_THROW (CORBA::INTERNAL (CORBA::COMPLETED_NO));
}

// Tries to lock. If it is not possible false is returned.

CORBA::Boolean
CC_LockSet::try_lock (CosConcurrencyControl::lock_mode mode,
                      CORBA::Environment &)
{
  CC_LockModeEnum lm = lmconvert (mode);

  ACE_DEBUG ((LM_DEBUG,
              "CC_LockSet::try_lock\n"));

  if (this->try_lock_i (lm) == 0)
    return 0;
  else
    return 1;
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
CC_LockSet::unlock (CosConcurrencyControl::lock_mode mode,
                    CORBA::Environment &TAO_IN_ENV)
{
  ACE_DEBUG ((LM_DEBUG,
              "CC_LockSet::unlock\n"));

  CC_LockModeEnum lm = lmconvert (mode);

  ACE_GUARD (ACE_SYNCH_MUTEX, ace_mon, this->mlock_);

  TAO_TRY
    {
      if (lock_[lm] == 0) // This lock is not held.
        TAO_THROW (CosConcurrencyControl::LockNotHeld());
      else
        lock_[lm]--;

      TAO_CHECK_ENV;

      // If we do not have a lock held in a weaker mode than the
      // strongest held and we have requests on the semaphore signal
      // the semaphore.
      while (lock_queue_.size () > 0)
        {
          CC_LockModeEnum lock_on_queue = CC_EM;

          lock_queue_.dequeue_head (lock_on_queue);

          if (compatible (lock_on_queue) == 1)
            {
              if (semaphore_.release () == -1)
                TAO_THROW (CORBA::INTERNAL (CORBA::COMPLETED_NO));
              lock_[lock_on_queue]++;
            }
          else
            {
              lock_queue_.enqueue_head (lock_on_queue);
              break;
            }
        }
    }
  TAO_CATCHANY
    {
      TAO_RETHROW;
    }
  TAO_ENDTRY;
  this->dump ();
}

// Changes the mode of a held lock.

void
CC_LockSet::change_mode (CosConcurrencyControl::lock_mode held_mode,
                         CosConcurrencyControl::lock_mode new_mode,
                         CORBA::Environment &TAO_IN_ENV)
{
  ACE_DEBUG ((LM_DEBUG,
              "CC_LockSet::change_mode\n"));
  CC_LockModeEnum lm_held = lmconvert (held_mode);
  CC_LockModeEnum lm_new = lmconvert (new_mode);

  TAO_TRY
    {
      if (this->lock_held (lm_held) == 0) // This lock is not held
        TAO_THROW (CosConcurrencyControl::LockNotHeld());
      else if (this->change_mode_i (lm_held, lm_new)==1)
        {
          this->unlock (held_mode, TAO_IN_ENV);
          TAO_CHECK_ENV;

          if (semaphore_.acquire () == -1)
            TAO_THROW (CORBA::INTERNAL (CORBA::COMPLETED_NO));
        }
    }
  TAO_CATCHANY
    {
      TAO_RETHROW;
    }
  TAO_ENDTRY;

  //  this->dump ();
}

int
CC_LockSet::lock_i (CC_LockModeEnum lm)
{
  ACE_GUARD_RETURN (ACE_SYNCH_MUTEX, ace_mon, this->mlock_, 1);
  // If the lock is not compatible with the locks we hold allready or
  // there is lock requests in the queue we cannot grant the lock and
  // thus we queue the request. Otherwise update the lock count.
  if (compatible (lm) == 0 || lock_queue_.size () > 0)
    {
      // Put the lock mode in the queue
      lock_queue_.enqueue_tail (lm);
      this->dump ();
      return 1; // Lock the semaphore.
    }
  else
    lock_[lm]++;

  this->dump ();
  return 0;
}

int
CC_LockSet::try_lock_i (CC_LockModeEnum lm)
{
  ACE_GUARD_RETURN (ACE_SYNCH_MUTEX, ace_mon, this->mlock_, 1);
  // If the lock we try is compatible with the locks we hold we just
  // opdates the count. Otherwise we return false.
  if (compatible (lm) == 0)
    {
      this->dump ();
      return 0;
    }
  else
    lock_[lm]++;

  this->dump ();
  return 1;
}

int
CC_LockSet::change_mode_i (CC_LockModeEnum lm_held,
                          CC_LockModeEnum lm_new)
{
  ACE_GUARD_RETURN (ACE_SYNCH_MUTEX, ace_mon, this->mlock_, 1);
  // If the new mode is compatible with the modes we hold we change
  // the counts for the two locks. If not we must queue the new
  // request. We can decrement the count for the old mode without
  // signalling the semaphore because we know we only check modes
  // granted this far.

  lock_[lm_held]--;

  if (compatible (lm_new) == 1)
    {
      lock_[lm_new]++;
      this->dump ();
      return 0;
    }
  else
    {
      lock_[lm_held]++;
      lock_queue_.enqueue_tail (lm_new);
      this->dump ();
      return 1;
    }
}

int
CC_LockSet::lock_held (CC_LockModeEnum lm)
{
  ACE_GUARD_RETURN (ACE_SYNCH_MUTEX, ace_mon, this->mlock_, 1);
  if (lock_[lm] > 0)
    return 1;
  else
    return 0;
}

void
CC_LockSet::dump (void)
{
  ACE_DEBUG ((LM_DEBUG,
              "waiting_calls_: %i, IR: %i, R: %i, U: %i, IW: %i, W: %i\n",
              lock_queue_.size (),
              lock_[CC_IR],
              lock_[CC_R],
              lock_[CC_U],
              lock_[CC_IW],
              lock_[CC_W]));
}

CORBA::Boolean CC_LockSet::compatible_[NUMBER_OF_LOCK_MODES][NUMBER_OF_LOCK_MODES] ={
  {1, 1, 1, 1, 0},
  {1, 1, 1, 0, 0},
  {1, 1, 0, 0, 0},
  {1, 0, 0, 1, 0},
  {0, 0, 0, 0, 0}};

#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)
template class ACE_Node<CC_LockModeEnum>;
template class ACE_Unbounded_Queue<CC_LockModeEnum>;
template class ACE_Unbounded_Queue_Iterator<CC_LockModeEnum>;
#elif defined (ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)
#pragma instantiate ACE_Node<CC_LockModeEnum>
#pragma instantiate ACE_Unbounded_Queue<CC_LockModeEnum>
#pragma instantiate ACE_Unbounded_Queue_Iterator<CC_LockModeEnum>
#endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */
