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

// Default constructor.

CC_LockSet::CC_LockSet (void)
  : related_lockset_ (0)
{
  ACE_NEW(mlock_, ACE_Thread_Mutex);
  TAO_TRY
    {
      this->Init(TAO_TRY_ENV);
      TAO_CHECK_ENV;
    }
  TAO_CATCHANY
    {
      TAO_TRY_ENV.print_exception("CC_LockSet::CC_LockSet(...)");
    }
  TAO_ENDTRY;
}

// Constructor used to create related lock sets.

CC_LockSet::CC_LockSet (CosConcurrencyControl::LockSet_ptr related)
  : related_lockset_ (related)
{
  ACE_NEW(mlock_, ACE_Thread_Mutex);
  TAO_TRY
    {
      this->Init(TAO_TRY_ENV);
      TAO_CHECK_ENV;
    }
  TAO_CATCHANY
    {
      TAO_TRY_ENV.print_exception("CC_LockSet::CC_LockSet(...)");
    }
  TAO_ENDTRY;
}

// Initialization.

void
CC_LockSet::Init(CORBA::Environment &_env)
{
  // Set the mode of the statically allocated locks
  lock_[CC_IR] = 0;
  lock_[CC_R] = 0;
  lock_[CC_U] = 0;
  lock_[CC_IW] = 0;
  lock_[CC_W] = 0;

  // Acquire the semaphore in order to be able to put requests on hold
  if(semaphore_.acquire()==-1)
    TAO_THROW (CORBA::INTERNAL (CORBA::COMPLETED_NO));
}

// Destructor

CC_LockSet::~CC_LockSet (void)
{
  if(this->mlock_!=0)
    delete mlock_;
}

// Returns true if the requested lock mode is compatible with the
// modes held. False otherwise.
CORBA::Boolean CC_LockSet::compatible(CC_LockModeEnum mr)
{
  int i=CC_IR;

  while(i<=CC_W)
    {
      if(this->lock_[i]>0)
        if(this->compatible_[i][mr]==CORBA::B_FALSE)
          {
            return CORBA::B_FALSE;
          }
      i++;
    }

  return CORBA::B_TRUE;
}

// Locks the lock in the desired mode. Blocks until success.

void
CC_LockSet::lock (CosConcurrencyControl::lock_mode mode,
                  CORBA::Environment &_env)
{
  ACE_DEBUG ((LM_DEBUG, "CC_LockSet::lock\n"));

  CC_LockModeEnum lm = lmconvert(mode);

  // Check to see if the requested mode is compatible with the
  // modes held so far. If not put the request on hold.

  if(this->lock_d(lm)==1)
    if(semaphore_.acquire()==-1)
      TAO_THROW (CORBA::INTERNAL (CORBA::COMPLETED_NO));

  //  ACE_DEBUG ((LM_DEBUG,
  //              "waiting_calls_: %i, IR: %i, R: %i, U: %i, IW: %i, W: %i\n",
  //              lock_queue_.size(),
  //              lock_[CC_IR], lock_[CC_R], lock_[CC_U], lock_[CC_IW], lock_[CC_W]));
}

// Tries to lock. If it is not possible false is returned.

CORBA::Boolean
CC_LockSet::try_lock (CosConcurrencyControl::lock_mode mode,
                      CORBA::Environment &_env)
{
  //  CORBA::Boolean success = CORBA::B_TRUE;

  CC_LockModeEnum lm = lmconvert(mode);

  ACE_DEBUG ((LM_DEBUG,
              "CC_LockSet::try_lock\n"));
  if(this->try_lock_d(lm)==0)
    return CORBA::B_FALSE; // success = CORBA::B_FALSE;
  else
    return CORBA::B_TRUE;  // success = CORBA::B_TRUE;

  //ACE_DEBUG ((LM_DEBUG,
  //            "waiting_calls_: %i, IR: %i, R: %i, U: %i, IW: %i, W: %i\n",
  //            lock_queue_.size(),
  //            lock_[CC_IR], lock_[CC_R], lock_[CC_U], lock_[CC_IW], lock_[CC_W]));
  //return success;
}

// Converts the enum from the spec to the internally (ordered)
// enum.

CC_LockModeEnum
CC_LockSet::lmconvert(CosConcurrencyControl::lock_mode mode)
{
  switch(mode)
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
                    CORBA::Environment &_env)
{
  ACE_DEBUG ((LM_DEBUG,
              "CC_LockSet::unlock\n"));

  CC_LockModeEnum lm = lmconvert(mode);

  ACE_GUARD (ACE_Thread_Mutex, ace_mon, *this->mlock_);

  TAO_TRY
    {
      if(lock_[lm]==0) // This lock is not held.
        TAO_THROW (CosConcurrencyControl::LockNotHeld);
      else
        lock_[lm]--;
      TAO_CHECK_ENV;
      // If we do not have a lock held in a weaker mode than the
      // strongest held and we have requests on the semaphore signal
      // the semaphore.
      while(lock_queue_.size()>0)
        {
          CC_LockModeEnum lock_on_queue = CC_EM;
          lock_queue_.dequeue_head(lock_on_queue);
          if(compatible(lock_on_queue)==CORBA::B_TRUE)
            {
              if(semaphore_.release()==-1)
                TAO_THROW (CORBA::INTERNAL (CORBA::COMPLETED_NO));
              lock_[lock_on_queue]++;
            }
          else
            {
              lock_queue_.enqueue_head(lock_on_queue);
              break;
            }
        }
    }
  TAO_CATCHANY
    {
      TAO_RETHROW;
    }
  TAO_ENDTRY;
  ACE_DEBUG ((LM_DEBUG,
              "waiting_calls_: %i, IR: %i, R: %i, U: %i, IW: %i, W: %i\n",
              lock_queue_.size(),
              lock_[CC_IR], lock_[CC_R], lock_[CC_U], lock_[CC_IW], lock_[CC_W]));
}

// Changes the mode of a held lock.

void
CC_LockSet::change_mode (CosConcurrencyControl::lock_mode held_mode,
                         CosConcurrencyControl::lock_mode new_mode,
                         CORBA::Environment &_env)
{
  ACE_DEBUG ((LM_DEBUG,
              "CC_LockSet::change_mode\n"));
  CC_LockModeEnum lm_held = lmconvert(held_mode);
  CC_LockModeEnum lm_new = lmconvert(new_mode);

  TAO_TRY
    {
      if(this->lock_held(lm_held) == 0) // This lock is not held
        TAO_THROW (CosConcurrencyControl::LockNotHeld);
      else
        {
          if(this->change_mode_d(lm_held, lm_new)==1)
            {
              this->unlock(held_mode, _env);
              if(semaphore_.acquire()==-1)
                TAO_THROW (CORBA::INTERNAL (CORBA::COMPLETED_NO));
            }
        }
    }
  TAO_CATCHANY
    {
      TAO_RETHROW;
    }
  TAO_ENDTRY;

  ACE_DEBUG ((LM_DEBUG,
              "waiting_calls_: %i, IR: %i, R: %i, U: %i, IW: %i, W: %i\n",
              lock_queue_.size(),
              lock_[CC_IR], lock_[CC_R], lock_[CC_U], lock_[CC_IW], lock_[CC_W]));
}

int
CC_LockSet::lock_d(CC_LockModeEnum lm)
{
  ACE_GUARD_RETURN (ACE_Thread_Mutex, ace_mon, *this->mlock_, 1);
  // If the lock is not compatible with the locks we hold allready or
  // there is lock requests in the queue we cannot grant the lock and
  // thus we queue the request. Otherwise update the lock count.
  if(compatible(lm)==CORBA::B_FALSE || lock_queue_.size()>0)
    {
      // Put the lock mode in the queue
      lock_queue_.enqueue_tail(lm);
      this->dump();
      return 1; // Lock the semaphore
    }
  else
    {
      lock_[lm]++;
    }
  this->dump();
  return 0;
}

int
CC_LockSet::try_lock_d(CC_LockModeEnum lm)
{
  ACE_GUARD_RETURN (ACE_Thread_Mutex, ace_mon, *this->mlock_, 1);
  // If the lock we try is compatible with the locks we hold we just
  // opdates the count. Otherwise we return false.
  if(compatible(lm)==CORBA::B_FALSE)
    {
      this->dump();
      return 0;
    }
  else
    {
      lock_[lm]++;
    }
  this->dump();
  return 1;
}

int
CC_LockSet::change_mode_d(CC_LockModeEnum lm_held,
                          CC_LockModeEnum lm_new)
{
  ACE_GUARD_RETURN (ACE_Thread_Mutex, ace_mon, *this->mlock_, 1);
  // If the new mode is compatible with the modes we hold we change
  // the counts for the two locks. If not we must queue the new
  // request. We can decrement the count for the old mode without
  // signalling the semaphore because we know we only check modes
  // granted this far.
  lock_[lm_held]--;
  if(compatible(lm_new)==CORBA::B_TRUE)
    {
      lock_[lm_new]++;
      this->dump();
      return 0;
    }
  else
    {
      lock_[lm_held]++;
      lock_queue_.enqueue_tail(lm_new);
      this->dump();
      return 1;
    }
}

int
CC_LockSet::lock_held(CC_LockModeEnum lm)
{
  ACE_GUARD_RETURN (ACE_Thread_Mutex, ace_mon, *this->mlock_, 1);
  if(lock_[lm]>0)
    return 1;
  else
    return 0;
}

void
CC_LockSet::dump(void)
{
  printf("waiting_calls_: %i, ", lock_queue_.size());
  for(int i=CC_IR; i<=CC_W; i+=1)
    printf("%i ", lock_[i]);
  printf("\n");
}

CORBA::Boolean CC_LockSet::compatible_[NUMBER_OF_LOCK_MODES][NUMBER_OF_LOCK_MODES] ={
  {CORBA::B_TRUE, CORBA::B_TRUE, CORBA::B_TRUE, CORBA::B_TRUE, CORBA::B_FALSE},
  {CORBA::B_TRUE, CORBA::B_TRUE, CORBA::B_TRUE, CORBA::B_FALSE, CORBA::B_FALSE},
  {CORBA::B_TRUE, CORBA::B_TRUE, CORBA::B_FALSE, CORBA::B_FALSE, CORBA::B_FALSE},
  {CORBA::B_TRUE, CORBA::B_FALSE, CORBA::B_FALSE, CORBA::B_TRUE, CORBA::B_FALSE},
  {CORBA::B_FALSE, CORBA::B_FALSE, CORBA::B_FALSE, CORBA::B_FALSE, CORBA::B_FALSE}};
