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
CC_LockSetFactory::create (CORBA::Environment &ACE_TRY_ENV)
{
  CC_LockSet *ls = 0;

  ACE_GUARD_RETURN (ACE_SYNCH_MUTEX, ace_mon, this->lock_, CosConcurrencyControl::LockSet::_nil ());

  TAO_IN_ENV.exception (new CORBA::NO_MEMORY ());
  ACE_NEW_THROW_EX (ls,
                    CC_LockSet,
                    CORBA::NO_MEMORY());
  ACE_CHECK_RETURN (CosConcurrencyControl::LockSet::_nil ());

  return ls->_this (ACE_TRY_ENV);
}

CosConcurrencyControl::LockSet_ptr
CC_LockSetFactory::create_related (CosConcurrencyControl::LockSet_ptr which,
                                   CORBA::Environment &ACE_TRY_ENV)
{
  CC_LockSet *ls = 0;

  ACE_GUARD_RETURN (ACE_SYNCH_MUTEX, ace_mon, this->lock_, CosConcurrencyControl::LockSet::_nil ());

  // @@ I commented out the following statement becuase it doesn't make any
  //    sense at all.    -- Nanbor
  //  TAO_IN_ENV.exception (new CORBA::NO_MEMORY ());
  ACE_NEW_THROW_EX (ls,
                    CC_LockSet (which),
                    CORBA::NO_MEMORY());
  ACE_CHECK_RETURN (CosConcurrencyControl::LockSet::_nil ());

  return ls->_this (ACE_TRY_ENV);
}
