/* -*- C++ -*- */
// $Id$

// ============================================================================
//
// = LIBRARY
//    cos
// 
// = FILENAME
//   CC_LockSetFactory.cpp
//
// = AUTHOR
//    Torben Worm <tworm@cs.wustl.edu>
// 
// ============================================================================

#include "CC_LockSetFactory.h"
#include "CC_LockSet.h"

// Default constructor
CC_LockSetFactory::CC_LockSetFactory (void)
{
  // @@ Torben, can you please comment this code?
  ACE_NEW (this->lock_, ACE_Lock_Adapter<ACE_Thread_Mutex>);
}

// Destructor.
CC_LockSetFactory::~CC_LockSetFactory (void)
{
  delete this->lock_;
}

CosConcurrencyControl::LockSet_ptr
CC_LockSetFactory::create ( CORBA::Environment &_env)
{
  CC_LockSet *ls = 0;

  env.clear ();
  env.exception (new CORBA::UNKNOWN (CORBA::COMPLETED_NO));
  ACE_GUARD_RETURN (ACE_Lock, ace_mon, *this->lock_, CosConcurrencyControl::LockSet::_nil ());

  env.clear ();
  
  env.exception (new CORBA::NO_MEMORY (CORBA::COMPLETED_NO));
  ACE_NEW_RETURN (ls,
                  CC_LockSet,
                  CosConcurrencyControl::LockSet::_nil ());
  env.clear ();

  return ls->_this (_env);
}

CosConcurrencyControl::LockSet_ptr
CC_LockSetFactory::create_related (CosConcurrencyControl::LockSet_ptr which,
                                   CORBA::Environment &_env)
{
  CC_LockSet *ls = 0;

  env.clear ();
  env.exception (new CORBA::UNKNOWN (CORBA::COMPLETED_NO));
  ACE_GUARD_RETURN (ACE_Lock, ace_mon, *this->lock_, CosConcurrencyControl::LockSet::_nil ());

  env.clear ();
  
  env.exception (new CORBA::NO_MEMORY (CORBA::COMPLETED_NO));
  ACE_NEW_RETURN (ls,
                  CC_LockSet (which),
                  CosConcurrencyControl::LockSet::_nil ());
  env.clear ();

  return ls->_this (_env);
}
