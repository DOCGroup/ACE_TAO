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
}

// Destructor.
CC_LockSetFactory::~CC_LockSetFactory (void)
{
}

CosConcurrencyControl::LockSet_ptr
CC_LockSetFactory::create (CORBA::Environment &_env)
{
  CC_LockSet *ls = 0;

  ACE_GUARD_RETURN (ACE_Thread_Mutex, ace_mon, this->lock_, CosConcurrencyControl::LockSet::_nil ());

  _env.exception (new CORBA::NO_MEMORY (CORBA::COMPLETED_NO));
  ACE_NEW_RETURN (ls,
                  CC_LockSet,
                  CosConcurrencyControl::LockSet::_nil ());
  _env.clear ();

  return ls->_this (_env);
}

CosConcurrencyControl::LockSet_ptr
CC_LockSetFactory::create_related (CosConcurrencyControl::LockSet_ptr which,
                                   CORBA::Environment &_env)
{
  CC_LockSet *ls = 0;

  ACE_GUARD_RETURN (ACE_Thread_Mutex, ace_mon, this->lock_, CosConcurrencyControl::LockSet::_nil ());

  _env.exception (new CORBA::NO_MEMORY (CORBA::COMPLETED_NO));
  ACE_NEW_RETURN (ls,
                  CC_LockSet (which),
                  CosConcurrencyControl::LockSet::_nil ());
  _env.clear ();

  return ls->_this (_env);
}
