/* -*- C++ -*- */
// $Id$

// ============================================================================
//
// = LIBRARY
//    TAO/orbsvcs/Concurrency_Service
//
// = FILENAME
//    CC_Lock.h
//
// = DESCRIPTION
//      This class implements a lock used by the lock set from the
//      concurrency control service
//
// = AUTHORS
//    Torben Worm <tworm@cs.wustl.edu>
//
// ============================================================================

#ifndef _CC_LOCK_H
#define _CC_LOCK_H
#include "ace/pre.h"

#include "ace/Synch.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "orbsvcs/CosConcurrencyControlC.h"
#include "concurrency_export.h"

#define NUMBER_OF_LOCK_MODES 5
// This constant defines the number of lock modes. There is really no
// way to set this constant dynamically because the nuber of lock
// modes are not stated as part of the IDL.

class TAO_Concurrency_Export CC_Lock
{
  // = TITLE
  //     CC_Lock
  //
  // = DESCRIPTION
  //     This class implements the lock concept from the concurrency
  //     control service. The lock holds its mode - this might later
  //     be changed to subclasses depending on the differences of the
  //     locks.  At present the is only a lock-pr-thread/client-type
  //     which is essentially a write lock since it is not allowed to
  //     have more than one lock pr. servant in this implementation.
public:
  CC_Lock (void);
  // Creates the lock with mode = intention_read (weakest)

  CC_Lock (CosConcurrencyControl::lock_mode mode);
  // Creates the lock with the desired mode

  ~CC_Lock (void);
  // Deletes the lock

  void lock (ACE_ENV_SINGLE_ARG_DECL);
  // Acquires this lock. Blocks until lock is obtained

  CORBA::Boolean try_lock (ACE_ENV_SINGLE_ARG_DECL);
  // Tries to acquire this lock. If it is not possible to acquire the
  // lock, false is returned

  void unlock (ACE_ENV_SINGLE_ARG_DECL);
  // Releases this lock.

  void change_mode (CosConcurrencyControl::lock_mode new_mode
                   ACE_ENV_ARG_DECL);
  // Changes the mode of this lock.

  void set_mode (CosConcurrencyControl::lock_mode mode);
  // Sets the mode_ of the lock. Used in initialization

  CORBA::Boolean Compatible (const CC_Lock &other);
  // returns true if this lock is compatible with the other lock.

  CORBA::Boolean Compatible (CosConcurrencyControl::lock_mode mode);
  // Returns true is this lock is compatible with the referenced mode.

  CosConcurrencyControl::lock_mode GetMode (void);
  // Returns the mode of the lock.

  int GetLocksHeld(void);
  // Returns the number of times this lock have been locked

  void DecLocksHeld(void);
  // Decrements the number of locks held in this mode. Used by change_mode.

  void dump(void);
  // Dumps the state of the object to stdout

protected:
  CosConcurrencyControl::lock_mode mode_;
  // Holds the lock's mode.

private:
  int lock_held_;
  // If greater than zero the lock is held (that number of times).

  static CORBA::Boolean compatible_[NUMBER_OF_LOCK_MODES][NUMBER_OF_LOCK_MODES];
  // Defines the compatibility of the locks.
};

class TAO_Concurrency_Export CC_LockModeIterator
{
  // = TITLE
  //     CC_LockModeIterator
  //
  // = DESCRIPTION
  //     This class implements an iterator over the lock modes in
  //     order to make an ordered traversal over the locks from the
  //     weakest (intention read) to the strongest (write).
  //     Ordering: IR -> R -> U -> IW -> W
public:
  CC_LockModeIterator(void);
  // Default constructor

  ~CC_LockModeIterator(void);
  // Destructor

  void First(void);
  // Reset the iterator to the first element

  void Next(ACE_ENV_SINGLE_ARG_DECL);
  // Advance the iterator to the next element
  // Throws exception if out of range

  CORBA::Boolean IsDone(void);
  // Returns true if the iterator has reached the last element

  CosConcurrencyControl::lock_mode GetLockMode(void);
  // Get the current element

private:
  CosConcurrencyControl::lock_mode current_;
};

#include "ace/post.h"
#endif /* !defined (_CC_LOCK_H) */
