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

ACE_RCSID(Concurrency, CC_LockSetFactory, "$Id$")

// Default constructor
CC_LockSetFactory::CC_LockSetFactory (void)
{
}

// Destructor.
CC_LockSetFactory::~CC_LockSetFactory (void)
{
}

CosConcurrencyControl::LockSet_ptr
CC_LockSetFactory::create (CORBA::Environment &TAO_IN_ENV)
{
  CC_LockSet *ls = 0;

  ACE_GUARD_RETURN (ACE_SYNCH_MUTEX, ace_mon, this->lock_, CosConcurrencyControl::LockSet::_nil ());

  TAO_IN_ENV.exception (new CORBA::NO_MEMORY (CORBA::COMPLETED_NO));
  ACE_NEW_THROW_RETURN (ls,
                        CC_LockSet,
                        CORBA::NO_MEMORY(CORBA::COMPLETED_NO),
                        CosConcurrencyControl::LockSet::_nil ());
  TAO_IN_ENV.clear ();

  return ls->_this (TAO_IN_ENV);
}

CosConcurrencyControl::LockSet_ptr
CC_LockSetFactory::create_related (CosConcurrencyControl::LockSet_ptr which,
                                   CORBA::Environment &TAO_IN_ENV)
{
  CC_LockSet *ls = 0;

  ACE_GUARD_RETURN (ACE_SYNCH_MUTEX, ace_mon, this->lock_, CosConcurrencyControl::LockSet::_nil ());

  TAO_IN_ENV.exception (new CORBA::NO_MEMORY (CORBA::COMPLETED_NO));
  ACE_NEW_THROW_RETURN (ls,
                        CC_LockSet (which),
                        CORBA::NO_MEMORY(CORBA::COMPLETED_NO),
                        CosConcurrencyControl::LockSet::_nil ());
  TAO_IN_ENV.clear ();

  return ls->_this (TAO_IN_ENV);
}
