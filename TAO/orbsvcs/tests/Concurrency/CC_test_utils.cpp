// $Id$

// ============================================================================
//
// = LIBRARY
//    TAO/orbsvcs/tests
//
// = FILENAME
//    CC_test_utils.cpp
//
// = DESCRIPTION
//      This class provides a namespace for utility functions for the
//      concurrency service test.
//
// = AUTHORS
//      Torben Worm <tworm@cs.wustl.edu>
//
// ============================================================================

#include "CC_test_utils.h"
#include "CC_naming_service.h"
#include "ace/OS.h"

ACE_RCSID(Concurrency, CC_test_utils, "$Id$")

char *CC_TestUtils::get_lock_mode_name (CosConcurrencyControl::lock_mode mode)
{
  if (mode==CosConcurrencyControl::read)
    return ACE_const_cast (char*, "read");

  if (mode==CosConcurrencyControl::write)
    return  ACE_const_cast (char*, "write");

  if (mode==CosConcurrencyControl::upgrade)
    return ACE_const_cast (char*, "upgrade");

  if (mode==CosConcurrencyControl::intention_read)
    return ACE_const_cast (char*, "intention_read");

  if (mode==CosConcurrencyControl::intention_write)
    return ACE_const_cast (char*, "intention_write");

  return ACE_const_cast (char*, "unknown lock mode");
}

CosConcurrencyControl::LockSet_ptr
CC_TestUtils::create_lock_set (void)
{
  // Create the lock set and return an obj ref corresponding to the
  // key.
  CosConcurrencyControl::LockSet_ptr lock_set(0);
  ACE_DECLARE_NEW_CORBA_ENV;
  ACE_TRY
    {
      lock_set =
        CC_naming_service::Instance()->get_lock_set_factory ()->create (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;

      if (CORBA::is_nil (lock_set))
        ACE_ERROR_RETURN ((LM_ERROR,
                           "null lock set objref returned by factory\n"),
                          0);
    }
  ACE_CATCHANY
    {
      ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION,
                           "CC_Client::create_lock_set");
      return 0;
    }
  ACE_ENDTRY;
  return lock_set;
}
