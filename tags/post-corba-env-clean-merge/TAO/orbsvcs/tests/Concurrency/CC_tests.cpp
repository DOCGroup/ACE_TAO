// $Id$

// ============================================================================
//
// = LIBRARY
//    TAO/tests/Concurrency
//
// = FILENAME
//    CC_tests.h
//
// = DESCRIPTION
//      This class implements a number of test objects to test the
//      concurrency service.
//
// = AUTHORS
//    Torben Worm <tworm@cs.wustl.edu>
//
// ============================================================================

#include "CC_tests.h"
#include "ace/OS.h"
ACE_RCSID(Concurrency, CC_tests, "$Id$")

CC_Test::CC_Test (CC_naming_service *ns)
  : result (CC_FAIL),
    naming_service_ (ns)
{
}

CC_Test::~CC_Test (void)
{
}

CosConcurrencyControl::LockSet_ptr
CC_Test::create_lock_set (void)
{
  // Create the lock set and return an obj ref corresponding to the
  // key.
  CosConcurrencyControl::LockSet_ptr lock_set(0);
  TAO_ENV_DECLARE_NEW_ENV;
  ACE_TRY
    {
      lock_set =
        this->naming_service_->get_lock_set_factory ()->create (TAO_ENV_SINGLE_ARG_PARAMETER);
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

char *
CC_Test::get_lock_mode_name (CosConcurrencyControl::lock_mode mode)
{
  if (mode==CosConcurrencyControl::read)
    return ACE_const_cast (char *, "read");

  if (mode==CosConcurrencyControl::write)
    return ACE_const_cast (char *, "write");

  if (mode==CosConcurrencyControl::upgrade)
    return ACE_const_cast (char *, "upgrade");

  if (mode==CosConcurrencyControl::intention_read)
    return ACE_const_cast (char *, "intension_read");

  if (mode==CosConcurrencyControl::intention_write)
    return ACE_const_cast (char *, "intension_write");

  return ACE_const_cast (char *, "unknown lock mode");
}

// ================================
// Here the tests start.

Test_Single_Lock_With_Mode::Test_Single_Lock_With_Mode (CC_naming_service *naming_service,
                                                        CosConcurrencyControl::lock_mode mode)
  : CC_Test (naming_service),
    mode_ (mode)
{
}

Test_Single_Lock_With_Mode::~Test_Single_Lock_With_Mode (void)
{
}

int
Test_Single_Lock_With_Mode::run (int /* times_to_run */)
{
  CORBA::Boolean lock_not_held;

  CosConcurrencyControl::LockSet_ptr cc_lock_set_ = create_lock_set ();

  TAO_ENV_DECLARE_NEW_ENV;
  ACE_TRY
    {
      cc_lock_set_->lock (mode_ TAO_ENV_ARG_PARAMETER);

      ACE_TRY_CHECK;
      ACE_DEBUG ((LM_DEBUG,
                  "%s lock set\n",
                  get_lock_mode_name (mode_)));

      lock_not_held = cc_lock_set_->try_lock (mode_
                                              TAO_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      if (lock_not_held)
        {
          ACE_DEBUG ((LM_DEBUG,
                      "%s lock not held\n",
                      get_lock_mode_name (mode_)));
          return CC_FAIL;
        }
      else
        ACE_DEBUG ((LM_DEBUG,
                    "%s lock held\n",
                    get_lock_mode_name (mode_)));

      cc_lock_set_->unlock (mode_
                            TAO_ENV_ARG_PARAMETER);
      ACE_DEBUG ((LM_DEBUG,
                  "%s lock released\n",
                  get_lock_mode_name (mode_)));

      lock_not_held = cc_lock_set_->try_lock (mode_
                                              TAO_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      if (lock_not_held)
        ACE_DEBUG ((LM_DEBUG,
                    "%s lock not held\n",
                    get_lock_mode_name (mode_)));
      else
        {
          ACE_DEBUG ((LM_DEBUG,
                      "%s lock held\n",
                      get_lock_mode_name (mode_)));
          return CC_FAIL;
        }

    }
  ACE_CATCHANY
    {
      ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION,
                           "Test_Single_Lock_With_Mode::run");
      return CC_FAIL;
    }
  ACE_ENDTRY;

  CORBA::release (cc_lock_set_);

  return CC_SUCCESS;
}

// ================================

Test_Setup_LockSet::Test_Setup_LockSet (CC_naming_service *naming_service,
                                        char *name)
  : CC_Test (naming_service),
    my_name_ (name)
{
}

Test_Setup_LockSet::~Test_Setup_LockSet (void)
{
}

int
Test_Setup_LockSet::run (int /* times_to_run */)
{
  TAO_ENV_DECLARE_NEW_ENV;
  ACE_TRY
    {
      CosConcurrencyControl::LockSet_ptr cc_lock_set_ =
        create_lock_set ();
      this->naming_service_->bind_name (my_name_, cc_lock_set_
                                        TAO_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      ACE_DEBUG ((LM_DEBUG,
                  "Name bound\n"));

      cc_lock_set_->lock (CosConcurrencyControl::read
                          TAO_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      ACE_DEBUG ((LM_DEBUG,
                  "Read lock set\n"));
    }
  ACE_CATCHANY
    {
      ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION,
                           "Test_Against_Other_LockSet::run");
      return CC_FAIL;
    }
  ACE_ENDTRY;

  return CC_SUCCESS;
}

// ================================

Test_Use_Already_Created_LockSet::
Test_Use_Already_Created_LockSet (CC_naming_service *naming_service,
                                  char *name)
  : CC_Test (naming_service),
    my_name_ (name)
{
}

Test_Use_Already_Created_LockSet::~Test_Use_Already_Created_LockSet (void)
{
}

int
Test_Use_Already_Created_LockSet::run (int /* times_to_run */)
{
  TAO_ENV_DECLARE_NEW_ENV;
  ACE_TRY
    {
      CORBA::Object_var ccls_obj =
        this->naming_service_->get_obj_from_name (ACE_const_cast (char *, ""),
                                                  my_name_
                                                  TAO_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      CosConcurrencyControl::LockSet_var ccls =
        CosConcurrencyControl::LockSet::_narrow (ccls_obj.in ()
                                                 TAO_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      ccls->lock (CosConcurrencyControl::read
                  TAO_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

    }
  ACE_CATCHANY
    {
      ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION,
                           "Test_Against_Other_LockSet::run");
      return CC_FAIL;
    }
  ACE_ENDTRY;

  return CC_SUCCESS;
}

// ================================

Test_Unlock_Already_Created_LockSet::
Test_Unlock_Already_Created_LockSet (CC_naming_service *naming_service,
                                     char *name)
  : CC_Test (naming_service),
    my_name_ (name)
{
}

Test_Unlock_Already_Created_LockSet::~Test_Unlock_Already_Created_LockSet (void)
{
}

int
Test_Unlock_Already_Created_LockSet::run (int /* times_to_run */)
{
  TAO_ENV_DECLARE_NEW_ENV;
  ACE_TRY
    {
      CORBA::Object_var ccls_obj =
        this->naming_service_->get_obj_from_name (ACE_const_cast (char *, ""), my_name_
                                                 TAO_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      CosConcurrencyControl::LockSet_var ccls =
        CosConcurrencyControl::LockSet::_narrow (ccls_obj.in ()
                                                 TAO_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      ccls->unlock (CosConcurrencyControl::read
                    TAO_ENV_ARG_PARAMETER);
    }
  ACE_CATCHANY
    {
      ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION,
                           "Test_Unlock_Already_Created_LockSet::run");
      return CC_FAIL;
    }
  ACE_ENDTRY;

  return CC_SUCCESS;
}

// ================================

Test_Release_Not_Held_Lock::Test_Release_Not_Held_Lock (CC_naming_service *naming_service,
                                                        CosConcurrencyControl::lock_mode mode)
  : CC_Test (naming_service),
    mode_ (mode)
{
}

Test_Release_Not_Held_Lock::~Test_Release_Not_Held_Lock (void)
{
}

int
Test_Release_Not_Held_Lock::run (int /* times_to_run */)
{
  CORBA::Boolean lock_not_held;

  // Create the lock set
  CosConcurrencyControl::LockSet_ptr cc_lock_set_ = create_lock_set ();
  TAO_ENV_DECLARE_NEW_ENV;
  ACE_TRY
    {
      // lock the lock
      cc_lock_set_->lock (mode_ TAO_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      ACE_DEBUG ((LM_DEBUG,
                  "%s lock set\n",
                  get_lock_mode_name (mode_)));

      // check to see if the lock is held
      lock_not_held = cc_lock_set_->try_lock (mode_
                                              TAO_ENV_ARG_PARAMETER);

      if (lock_not_held)
        {
          ACE_DEBUG ((LM_DEBUG,
                      "%s lock not held\n",
                      get_lock_mode_name (mode_)));
          return CC_FAIL;
        }
      else
        ACE_DEBUG ((LM_DEBUG,
                    "%s lock held\n",
                    get_lock_mode_name (mode_)));

      // release the lock
      cc_lock_set_->unlock (mode_
                            TAO_ENV_ARG_PARAMETER);
      ACE_DEBUG ((LM_DEBUG,
                  "%s lock released\n",
                  get_lock_mode_name (mode_)));

      // release the lock again. this should raise an exception
      cc_lock_set_->unlock (mode_
                            TAO_ENV_ARG_PARAMETER);

      ACE_DEBUG ((LM_DEBUG,
                  "attemptet to release %s lock\n",
                  get_lock_mode_name (mode_)));

      ACE_TRY_CHECK;
    }
  ACE_CATCH(CosConcurrencyControl::LockNotHeld, userex)
    {
      // We should end here
      ACE_UNUSED_ARG (userex);
      ACE_DEBUG((LM_DEBUG, "CosConcurrencyControl::LockNotHeld\n"));
      return CC_SUCCESS;
    }

  ACE_CATCHANY
    {
      ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION,
                           "Test_Release_Not_Held_Lock::run");
      return CC_FAIL;
    }
  ACE_ENDTRY;

  CORBA::release (cc_lock_set_);

  return CC_FAIL;
}
