/* -*- C++ -*- */
// $Id$

// ============================================================================
//
// = LIBRARY
//    TAO/orbsvcs/Concurrency_Service
//
// = FILENAME
//      CC_LockSet.h
//
// = DESCRIPTION
//      This class implements the lock set interface from the
//      concurrency service
//
// = AUTHORS
//      Torben Worm <tworm@cs.wustl.edu>
//
// ============================================================================

#if !defined (_CC_LOCKSET_H)
#define _CC_LOCKSET_H

// #include "tao/corba.h"
#include "CC_Lock.h"
#include "orbsvcs/CosConcurrencyControlS.h"

class TAO_ORBSVCS_Export CC_LockSet :  public POA_CosConcurrencyControl::LockSet
  // = TITLE
  //     CC_LockSet
  //
  // = DESCRIPTION
  //     This class implements the LockSet interface that is part
  //     of the CosConcurrency service. Please consult the idl file for
  //     detailed descriptions apart from the comments in this file
  //     At present the lock set is not really a set, but only one lock.
{
public:
  CC_LockSet(void);
  // Default constructor
  
  CC_LockSet(CosConcurrencyControl::LockSet_ptr related);
  // Constructor used if create_related is used to create the lock set

  ~CC_LockSet(void);
  // Destructor

  virtual void lock (CosConcurrencyControl::lock_mode mode,
		     CORBA::Environment &env);
  
  virtual CORBA::Boolean try_lock (CosConcurrencyControl::lock_mode mode,
				   CORBA::Environment &env);
  
  virtual void unlock (CosConcurrencyControl::lock_mode mode,
		       CORBA::Environment &env);
  
  virtual void change_mode (CosConcurrencyControl::lock_mode held_mode,
			    CosConcurrencyControl::lock_mode new_mode,
			    CORBA::Environment &env);
private:
  CC_Lock *lock_;
  // At present the lock set contains only one lock

  CosConcurrencyControl::LockSet_ptr related_lockset_;
  // If this lock set is related to another lock set, this is the pointer
  // to the related lock set. This is a really simple solution, but since
  // transactions are not supported in the first version there should be
  // no reason to drop lock sets together. The LockSetCoordinator is not
  // implemented (it has the responsibilities of dropping the locks).
}; // CC_LockSet

//#if defined (__ACE_INLINE__)
//#include "CC_LockSet.i"
//#endif // defined INLINE

#endif /* _CC_LOCKSET_H */
