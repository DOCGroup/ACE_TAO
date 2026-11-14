
//=============================================================================
/**
 *  @file    CC_tests.cpp
 *
 *    This class implements a number of test objects to test the
 *    concurrency service.
 *
 *  @author Torben Worm <tworm@cs.wustl.edu>
 */
//=============================================================================

#include "CC_tests.h"
#include "CC_test_utils.h"
#include "ace/Log_Msg.h"

CC_Test::CC_Test (CC_naming_service *ns)
  : naming_service_ (ns)
{
}

CosConcurrencyControl::LockSet_var
CC_Test::create_lock_set ()
{
  CosConcurrencyControl::LockSet_var lock_set;
  try
    {
      lock_set =
        this->naming_service_->get_lock_set_factory ()->create ();

      if (CORBA::is_nil (lock_set))
        ACE_ERROR ((LM_ERROR,
                    "null lock set objref returned by factory\n"));
    }
  catch (CORBA::Exception const &ex)
    {
      ex._tao_print_exception ("CC_Client::create_lock_set");
    }
  return lock_set;
}

Test_Single_Lock_With_Mode::Test_Single_Lock_With_Mode (CC_naming_service *naming_service,
                                                        CosConcurrencyControl::lock_mode mode)
  : CC_Test (naming_service),
    mode_ (mode)
{
}

int
Test_Single_Lock_With_Mode::run (int /* times_to_run */)
{
  CORBA::Boolean lock_not_held {};

  CosConcurrencyControl::LockSet_var cc_lock_set = this->create_lock_set ();
  if (CORBA::is_nil (cc_lock_set.in ()))
    return CC_FAIL;

  try
    {
      cc_lock_set->lock (mode_);

      ACE_DEBUG ((LM_DEBUG,
                  "%s lock set\n",
                  CC_TestUtils::get_lock_mode_name (mode_)));

      lock_not_held = cc_lock_set->try_lock (mode_);

      if (lock_not_held)
        {
          ACE_DEBUG ((LM_DEBUG,
                      "%s lock not held\n",
                      CC_TestUtils::get_lock_mode_name (mode_)));
          return CC_FAIL;
        }
      else
        ACE_DEBUG ((LM_DEBUG,
                    "%s lock held\n",
                    CC_TestUtils::get_lock_mode_name (mode_)));

      cc_lock_set->unlock (mode_);
      ACE_DEBUG ((LM_DEBUG,
                  "%s lock released\n",
                  CC_TestUtils::get_lock_mode_name (mode_)));

      lock_not_held = cc_lock_set->try_lock (mode_);

      if (lock_not_held)
        ACE_DEBUG ((LM_DEBUG,
                    "%s lock not held\n",
                    CC_TestUtils::get_lock_mode_name (mode_)));
      else
        {
          ACE_DEBUG ((LM_DEBUG,
                      "%s lock held\n",
                      CC_TestUtils::get_lock_mode_name (mode_)));
          return CC_FAIL;
        }
    }
  catch (CORBA::Exception const &ex)
    {
      ex._tao_print_exception ("Test_Single_Lock_With_Mode::run");
      return CC_FAIL;
    }

  return CC_SUCCESS;
}

Test_Setup_LockSet::Test_Setup_LockSet (CC_naming_service *naming_service,
                                        std::string const &name)
  : CC_Test (naming_service),
    my_name_ (name)
{
}

int
Test_Setup_LockSet::run (int /* times_to_run */)
{
  try
    {
      CosConcurrencyControl::LockSet_var cc_lock_set = this->create_lock_set ();
      if (CORBA::is_nil (cc_lock_set.in ()))
        return CC_FAIL;
      this->naming_service_->bind_name (this->my_name_, cc_lock_set.in ());

      ACE_DEBUG ((LM_DEBUG,
                  "Name bound\n"));

      cc_lock_set->lock (CosConcurrencyControl::read);

      ACE_DEBUG ((LM_DEBUG,
                  "Read lock set\n"));
    }
  catch (CORBA::Exception const &ex)
    {
      ex._tao_print_exception ("Test_Against_Other_LockSet::run");
      return CC_FAIL;
    }

  return CC_SUCCESS;
}

Test_Use_Already_Created_LockSet::
Test_Use_Already_Created_LockSet (CC_naming_service *naming_service,
                                  std::string const &name)
  : CC_Test (naming_service),
    my_name_ (name)
{
}

int
Test_Use_Already_Created_LockSet::run (int /* times_to_run */)
{
  try
    {
      CORBA::Object_var ccls_obj = this->naming_service_->get_obj_from_name ("", this->my_name_);

      CosConcurrencyControl::LockSet_var ccls =
        CosConcurrencyControl::LockSet::_narrow (ccls_obj.in ());

      ccls->lock (CosConcurrencyControl::read);
    }
  catch (CORBA::Exception const &ex)
    {
      ex._tao_print_exception ("Test_Against_Other_LockSet::run");
      return CC_FAIL;
    }

  return CC_SUCCESS;
}

Test_Unlock_Already_Created_LockSet::
Test_Unlock_Already_Created_LockSet (CC_naming_service *naming_service,
                                     std::string const &name)
  : CC_Test (naming_service),
    my_name_ (name)
{
}

int
Test_Unlock_Already_Created_LockSet::run (int /* times_to_run */)
{
  try
    {
      CORBA::Object_var ccls_obj = this->naming_service_->get_obj_from_name ("", this->my_name_);

      CosConcurrencyControl::LockSet_var ccls =
        CosConcurrencyControl::LockSet::_narrow (ccls_obj.in ());

      ccls->unlock (CosConcurrencyControl::read);
    }
  catch (CORBA::Exception const &ex)
    {
      ex._tao_print_exception ("Test_Unlock_Already_Created_LockSet::run");
      return CC_FAIL;
    }

  return CC_SUCCESS;
}

Test_Release_Not_Held_Lock::Test_Release_Not_Held_Lock (CC_naming_service *naming_service,
                                                        CosConcurrencyControl::lock_mode mode)
  : CC_Test (naming_service),
    mode_ (mode)
{
}

int
Test_Release_Not_Held_Lock::run (int /* times_to_run */)
{
  CORBA::Boolean lock_not_held {};

  CosConcurrencyControl::LockSet_var cc_lock_set = this->create_lock_set ();
  if (CORBA::is_nil (cc_lock_set.in ()))
    return CC_FAIL;
  try
    {
      cc_lock_set->lock (mode_);

      ACE_DEBUG ((LM_DEBUG,
                  "%s lock set\n",
                  CC_TestUtils::get_lock_mode_name (mode_)));

      // check to see if the lock is held
      lock_not_held = cc_lock_set->try_lock (mode_);

      if (lock_not_held)
        {
          ACE_DEBUG ((LM_DEBUG,
                      "%s lock not held\n",
                      CC_TestUtils::get_lock_mode_name (mode_)));
          return CC_FAIL;
        }
      else
        ACE_DEBUG ((LM_DEBUG,
                    "%s lock held\n",
                    CC_TestUtils::get_lock_mode_name (mode_)));

      cc_lock_set->unlock (mode_);
      ACE_DEBUG ((LM_DEBUG,
                  "%s lock released\n",
                  CC_TestUtils::get_lock_mode_name (mode_)));

      cc_lock_set->unlock (mode_);

      ACE_DEBUG ((LM_DEBUG,
                  "attemptet to release %s lock\n",
                  CC_TestUtils::get_lock_mode_name (mode_)));
    }
  catch (CosConcurrencyControl::LockNotHeld const &userex)
    {
      // We should end here
      ACE_UNUSED_ARG (userex);
      ACE_DEBUG ((LM_DEBUG, "CosConcurrencyControl::LockNotHeld\n"));
      return CC_SUCCESS;
    }

  catch (CORBA::Exception const &ex)
    {
      ex._tao_print_exception ("Test_Release_Not_Held_Lock::run");
      return CC_FAIL;
    }

  return CC_FAIL;
}
