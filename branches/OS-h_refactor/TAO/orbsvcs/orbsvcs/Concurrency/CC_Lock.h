/* -*- C++ -*- */

//=============================================================================
/**
 *  @file    CC_Lock.h
 *
 *  $Id$
 *
 *    This class implements a lock used by the lock set from the
 *    concurrency control service
 *
 *
 *  @author Torben Worm <tworm@cs.wustl.edu>
 */
//=============================================================================


#ifndef _CC_LOCK_H
#define _CC_LOCK_H
#include "ace/pre.h"

#include "ace/Synch.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "orbsvcs/CosConcurrencyControlC.h"
#include "concurrency_export.h"

/// This constant defines the number of lock modes. There is really no
/// way to set this constant dynamically because the nuber of lock
/// modes are not stated as part of the IDL.
#define NUMBER_OF_LOCK_MODES 5

/**
 * @class CC_Lock
 *
 * @brief CC_Lock
 *
 * This class implements the lock concept from the concurrency
 * control service. The lock holds its mode - this might later
 * be changed to subclasses depending on the differences of the
 * locks.  At present the is only a lock-pr-thread/client-type
 * which is essentially a write lock since it is not allowed to
 * have more than one lock pr. servant in this implementation.
 */
class TAO_Concurrency_Export CC_Lock
{
public:
  /// Creates the lock with mode = intention_read (weakest)
  CC_Lock (void);

  /// Creates the lock with the desired mode
  CC_Lock (CosConcurrencyControl::lock_mode mode);

  /// Deletes the lock
  ~CC_Lock (void);

  /// Acquires this lock. Blocks until lock is obtained
  void lock (ACE_ENV_SINGLE_ARG_DECL);

  /// Tries to acquire this lock. If it is not possible to acquire the
  /// lock, false is returned
  CORBA::Boolean try_lock (ACE_ENV_SINGLE_ARG_DECL);

  /// Releases this lock.
  void unlock (ACE_ENV_SINGLE_ARG_DECL);

  /// Changes the mode of this lock.
  void change_mode (CosConcurrencyControl::lock_mode new_mode
                   ACE_ENV_ARG_DECL);

  /// Sets the mode_ of the lock. Used in initialization
  void set_mode (CosConcurrencyControl::lock_mode mode);

  /// Returns true if this lock is compatible with the other lock.
  CORBA::Boolean Compatible (const CC_Lock &other);

  /// Returns true is this lock is compatible with the referenced mode.
  CORBA::Boolean Compatible (CosConcurrencyControl::lock_mode mode);

  /// Returns the mode of the lock.
  CosConcurrencyControl::lock_mode GetMode (void);

  /// Returns the number of times this lock have been locked
  int GetLocksHeld(void);

  /// Decrements the number of locks held in this mode. Used by change_mode.
  void DecLocksHeld(void);

  /// Dumps the state of the object to stdout
  void dump(void);

protected:
  /// Holds the lock's mode.
  CosConcurrencyControl::lock_mode mode_;

private:
  /// If greater than zero the lock is held (that number of times).
  int lock_held_;

  /// Defines the compatibility of the locks.
  static CORBA::Boolean compatible_[NUMBER_OF_LOCK_MODES][NUMBER_OF_LOCK_MODES];
};

/**
 * @class CC_LockModeIterator
 *
 * @brief CC_LockModeIterator
 *
 * This class implements an iterator over the lock modes in
 * order to make an ordered traversal over the locks from the
 * weakest (intention read) to the strongest (write).
 * Ordering: IR -> R -> U -> IW -> W
 */
class TAO_Concurrency_Export CC_LockModeIterator
{
public:
  /// Default constructor
  CC_LockModeIterator(void);

  /// Destructor
  ~CC_LockModeIterator(void);

  /// Reset the iterator to the first element
  void First(void);

  /// Advance the iterator to the next element
  /// Throws exception if out of range
  void Next(ACE_ENV_SINGLE_ARG_DECL);

  /// Returns true if the iterator has reached the last element
  CORBA::Boolean IsDone(void);

  /// Get the current element
  CosConcurrencyControl::lock_mode GetLockMode(void);

private:
  CosConcurrencyControl::lock_mode current_;
};

#include "ace/post.h"
#endif /* !defined (_CC_LOCK_H) */
