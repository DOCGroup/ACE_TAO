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
  : strongest_held_ (CC_EM),
    waiting_calls_ (0),
    related_lockset_ (0)
{
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
  : strongest_held_ (CC_EM),
    waiting_calls_ (0),
    related_lockset_ (related)
{
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
  // Nothing is dynamically allocated
}

CORBA::Boolean CC_LockSet::compatible(CC_LockModeEnum mh, CC_LockModeEnum mr)
{
  if(mh==CC_EM) // no locks held
    return CORBA::B_TRUE;

  return compatible_[mh][mr];
}

// Locks the lock in the desired mode. Blocks until success.

void
CC_LockSet::lock (CosConcurrencyControl::lock_mode mode,
                  CORBA::Environment &_env)
{
  ACE_DEBUG ((LM_DEBUG, "CC_LockSet::lock\n"));

  CC_LockModeEnum lm = lmconvert(mode);

  TAO_TRY
    {
      // Check to see if the requested mode is compatible with the
      // modes held so far. If not put the request on hold.  @@TAO I'm
      // not shure whether we can run into race conditions on the
      // variables here (waiting_calls_, lock_, and strongest_held_).
      if(compatible(strongest_held_,lm)==CORBA::B_FALSE)
        {
          waiting_calls_++;
          if(semaphore_.acquire()==-1)
            TAO_THROW (CORBA::INTERNAL (CORBA::COMPLETED_NO));
          lock_[lm]++;
          strongest_held_ = lm;
        }
      else
        {
          lock_[lm]++;
          // If the mode granted is stronger than the strongest mode
          // held so far, we must update the strongest_held_ variable.
          if(lm>strongest_held_)
            strongest_held_ = lm;
        }
    }
  TAO_CATCHANY
    {
      TAO_RETHROW;
    }
  TAO_ENDTRY;
  ACE_DEBUG ((LM_DEBUG,
              "waiting_calls_: %i, strongest_held_: %i, IR: %i, R: %i, U: %i, IW: %i, W: %i\n",
              waiting_calls_, strongest_held_,
              lock_[CC_IR], lock_[CC_R], lock_[CC_U], lock_[CC_IW], lock_[CC_W]));
}

// Tries to lock. If it is not possible false is returned.

CORBA::Boolean
CC_LockSet::try_lock (CosConcurrencyControl::lock_mode mode,
                      CORBA::Environment &_env)
{
  CORBA::Boolean success = CORBA::B_TRUE;

  CC_LockModeEnum lm = lmconvert(mode);

  ACE_DEBUG ((LM_DEBUG,
              "CC_LockSet::try_lock\n"));
  TAO_TRY
    {
      // if the mode is compatible we lock the lock otherwise we
      // return false because we cannot grant the lock.
      if(compatible(strongest_held_,lm)==CORBA::B_FALSE)
        {
          success = CORBA::B_FALSE;
        }
      else
        {
          this->lock (mode, TAO_TRY_ENV);
        }
      TAO_CHECK_ENV;
    }
  TAO_CATCHANY
    {
      TAO_RETHROW_RETURN (CORBA::B_FALSE);
    }
  TAO_ENDTRY;
  ACE_DEBUG ((LM_DEBUG,
              "waiting_calls_: %i, strongest_held_: %i, IR: %i, R: %i, U: %i, IW: %i, W: %i\n",
              waiting_calls_, strongest_held_,
              lock_[CC_IR], lock_[CC_R], lock_[CC_U], lock_[CC_IW], lock_[CC_W]));
  return success;
}

// Converts the enum from the spec to the internally (ordered)
// enum. It could be faster to use a static array to do the conversion
// but the switch is more portable.

CC_LockModeEnum
CC_LockSet::lmconvert(CosConcurrencyControl::lock_mode mode)
{
  switch(mode)
    {
    case CosConcurrencyControl::intention_read:
      return CC_IR;
      break;
    case CosConcurrencyControl::read:
      return CC_R;
      break;
    case CosConcurrencyControl::upgrade:
      return CC_U;
      break;
    case CosConcurrencyControl::intention_write:
      return CC_IW;
      break;
    case CosConcurrencyControl::write:
      return CC_W;
      break;
    default:
      return CC_EM;
    }
}

// Loop through the locks held up to the requested mode and return
// true if any of the locks are held

CORBA::Boolean
CC_LockSet::weaker_held(CC_LockModeEnum mode)
{
  int li = CC_IR;

  while(li<=mode)
    {
      if(lock_[li]>0)
        return CORBA::B_TRUE;
      li++;
    }
  return CORBA::B_FALSE;
}

// Unlock the lock

void
CC_LockSet::unlock (CosConcurrencyControl::lock_mode mode,
                    CORBA::Environment &_env)
{
  ACE_DEBUG ((LM_DEBUG,
              "CC_LockSet::unlock\n"));

  CC_LockModeEnum lm = lmconvert(mode);

  TAO_TRY
    {
      if(lock_[lm]==0) // This lock is not held
        TAO_THROW (CosConcurrencyControl::LockNotHeld);
      else
        lock_[lm]--;
      TAO_CHECK_ENV;
      // If we do not have a lock held in a weaker mode than the
      // strongest held and we have requests on the semaphore signal
      // the semaphore.
      if(weaker_held(strongest_held_)==CORBA::B_FALSE &&
         waiting_calls_>0)
        {
          waiting_calls_--;
          if(semaphore_.release()==-1)
            TAO_THROW (CORBA::INTERNAL (CORBA::COMPLETED_NO));
          // Here's a problem. We do not know the mode of the lock
          // request on the semaphore. That turns out to be handled in
          // the lock method and thus isn't a problem.
        }
      else
        {
          // check if any locks are held. if not set strongest held to CC_EM.
          int locks_held = 0;
          int strongest_so_far = 0;
          for(int i=CC_IR; i<=CC_W; i++)
            {
              locks_held+=lock_[i];
              if(i<=strongest_held_ &&
                 lock_[i]>0)
                strongest_so_far = i;
            }
          // We need to make sure, that the strongest_held_ is
          // decremented to the actual strongest held mode.
          if(locks_held==0)
            strongest_held_ = CC_EM;
          else
            strongest_held_ = (CC_LockModeEnum) strongest_so_far;
        }
    }
  TAO_CATCHANY
    {
      TAO_RETHROW;
    }
  TAO_ENDTRY;
  ACE_DEBUG ((LM_DEBUG,
              "waiting_calls_: %i, strongest_held_: %i, IR: %i, R: %i, U: %i, IW: %i, W: %i\n",
              waiting_calls_, strongest_held_,
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
      if(lock_[lm_held] == 0) // This lock is not held
        TAO_THROW (CosConcurrencyControl::LockNotHeld);
      else
        {
          // If the new mode is compatible with the strongest mode
          // held, we just grant the new mode. Otherwise we must
          // unlock the old mode and lock the new.
          if(compatible(strongest_held_, lm_new)==CORBA::B_TRUE)
            {
              lock_[lm_held]--;
              lock_[lm_new]++;
            }
          else
            {
              this->unlock(held_mode, TAO_TRY_ENV);
              TAO_CHECK_ENV;
              this->lock(new_mode, TAO_TRY_ENV);
              TAO_CHECK_ENV;
            }
        }
    }
  TAO_CATCHANY
    {
      TAO_RETHROW;
    }
  TAO_ENDTRY;

  ACE_DEBUG ((LM_DEBUG,
              "waiting_calls_: %i, strongest_held_: %i, IR: %i, R: %i, U: %i, IW: %i, W: %i\n",
              waiting_calls_, strongest_held_,
              lock_[CC_IR], lock_[CC_R], lock_[CC_U], lock_[CC_IW], lock_[CC_W]));
}

void
CC_LockSet::dump(void)
{
  printf("waiting_calls_: %i, strongest_held_: %i, ",
         waiting_calls_, strongest_held_);
  for(int i=CC_IR; i<=CC_W; i+=1)
    printf("%i ", lock_[i]);
  printf("\n");
}

CORBA::Boolean CC_LockSet::compatible_[NUMBER_OF_LOCK_MODES][NUMBER_OF_LOCK_MODES] ={
  {CORBA::B_TRUE, CORBA::B_TRUE, CORBA::B_TRUE, CORBA::B_TRUE, CORBA::B_FALSE},
  {CORBA::B_TRUE, CORBA::B_TRUE, CORBA::B_TRUE, CORBA::B_FALSE, CORBA::B_FALSE},
  {CORBA::B_TRUE, CORBA::B_TRUE, CORBA::B_FALSE, CORBA::B_FALSE, CORBA::B_FALSE},
  {CORBA::B_TRUE, CORBA::B_FALSE, CORBA::B_FALSE, CORBA::B_FALSE, CORBA::B_FALSE},
  {CORBA::B_FALSE, CORBA::B_FALSE, CORBA::B_FALSE, CORBA::B_FALSE, CORBA::B_FALSE}};
