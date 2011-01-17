//=============================================================================
/**
 *  @file   CC_LockSetFactory.cpp
 *
 *  $Id$
 *
 *  @author Torben Worm <tworm@cs.wustl.edu>
 */
//=============================================================================


#include "orbsvcs/Concurrency/CC_LockSetFactory.h"
#include "orbsvcs/Concurrency/CC_LockSet.h"



TAO_BEGIN_VERSIONED_NAMESPACE_DECL

// Default constructor
CC_LockSetFactory::CC_LockSetFactory (void)
{
}

// Destructor.
CC_LockSetFactory::~CC_LockSetFactory (void)
{
}

CosConcurrencyControl::LockSet_ptr
CC_LockSetFactory::create (void)
{
  CC_LockSet *ls = 0;

  ACE_GUARD_RETURN (TAO_SYNCH_MUTEX, ace_mon, this->lock_, CosConcurrencyControl::LockSet::_nil ());

  ACE_NEW_THROW_EX (ls,
                    CC_LockSet,
                    CORBA::NO_MEMORY());

  return ls->_this ();
}

CosConcurrencyControl::LockSet_ptr
CC_LockSetFactory::create_related (CosConcurrencyControl::LockSet_ptr which)
{
  CC_LockSet *ls = 0;

  ACE_GUARD_RETURN (TAO_SYNCH_MUTEX, ace_mon, this->lock_, CosConcurrencyControl::LockSet::_nil ());

  ACE_NEW_THROW_EX (ls,
                    CC_LockSet (which),
                    CORBA::NO_MEMORY());

  return ls->_this ();
}

TAO_END_VERSIONED_NAMESPACE_DECL
