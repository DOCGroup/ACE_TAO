
//=============================================================================
/**
 *  @file    CC_test_utils.cpp
 *
 *    This class provides a namespace for utility functions for the
 *    concurrency service test.
 *
 *  @author   Torben Worm <tworm@cs.wustl.edu>
 */
//=============================================================================

#include "CC_test_utils.h"
#include "CC_naming_service.h"
#include "ace/Log_Msg.h"

char const *CC_TestUtils::get_lock_mode_name (CosConcurrencyControl::lock_mode mode)
{
  if (mode == CosConcurrencyControl::read)
    return "read";

  if (mode == CosConcurrencyControl::write)
    return "write";

  if (mode == CosConcurrencyControl::upgrade)
    return "upgrade";

  if (mode == CosConcurrencyControl::intention_read)
    return "intention_read";

  if (mode == CosConcurrencyControl::intention_write)
    return "intention_write";

  return "unknown lock mode";
}

CosConcurrencyControl::LockSet_ptr
CC_TestUtils::create_lock_set ()
{
  // Create the lock set and return an obj ref corresponding to the
  // key.
  CosConcurrencyControl::LockSet_ptr lock_set {};
  try
    {
      lock_set =
        CC_naming_service::Instance()->get_lock_set_factory ()->create ();

      if (CORBA::is_nil (lock_set))
        {
          ACE_ERROR ((LM_ERROR,
                      ACE_TEXT ("null lock set objref returned by factory\n")));
          return nullptr;
        }
    }
  catch (CORBA::Exception const &ex)
    {
      ex._tao_print_exception ("CC_Client::create_lock_set");
      return nullptr;
    }
  return lock_set;
}
