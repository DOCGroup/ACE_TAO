// -*- c++ -*-
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

CC_Test::CC_Test(CC_naming_service *ns)
  : result (CC_FAIL), naming_service_ (ns)
{
}

CC_Test::~CC_Test(void)
{
}

CosConcurrencyControl::LockSet_ptr
CC_Test:: create_lock_set(void)
{
  // Create the lock set and return an obj ref corresponding
  // to the key.
  CosConcurrencyControl::LockSet_ptr lock_set;
  
  TAO_TRY
    {
      lock_set =
        this->naming_service_->get_lock_set_factory()->create (TAO_TRY_ENV);
      TAO_CHECK_ENV;
      
      if (CORBA::is_nil (lock_set))
        ACE_ERROR_RETURN ((LM_ERROR,
                           "null lock set objref returned by factory\n"),
                          0);
    }
  TAO_CATCHANY
    {
      TAO_TRY_ENV.print_exception ("CC_Client::create_lock_set");
      return 0;
    }
  TAO_ENDTRY;
  return lock_set;
}

char *CC_Test::get_lock_mode_name(CosConcurrencyControl::lock_mode mode)
{
  if(mode==CosConcurrencyControl::read)
    return "read";

  if(mode==CosConcurrencyControl::write)
    return "write";

  if(mode==CosConcurrencyControl::upgrade)
    return "upgrade";

  if(mode==CosConcurrencyControl::intention_read)
    return "intension_read";

  if(mode==CosConcurrencyControl::intention_write)
    return "intension_write";

  return "unknown lock mode";
}

// Here the test start

Test_Single_Lock_With_Mode::
Test_Single_Lock_With_Mode(CC_naming_service *naming_service,
                           CosConcurrencyControl::lock_mode mode)
  : CC_Test (naming_service), mode_ (mode)
{
}

Test_Single_Lock_With_Mode::~Test_Single_Lock_With_Mode()
{
}

Test_Single_Lock_With_Mode::run(int times_to_run)
{
  CORBA::Boolean lock_not_held;
  
  CosConcurrencyControl::LockSet_ptr cc_lock_set_ = create_lock_set();

  TAO_TRY
    {
      cc_lock_set_->lock(mode_, TAO_TRY_ENV);
      ACE_DEBUG((LM_DEBUG, "%s lock set\n", get_lock_mode_name(mode_)));

      lock_not_held = cc_lock_set_->try_lock(mode_, TAO_TRY_ENV);
      if(lock_not_held)
        {
          ACE_DEBUG((LM_DEBUG, "%s lock not held\n",
                    get_lock_mode_name(mode_)));
          return CC_FAIL;
        }
      else
        ACE_DEBUG((LM_DEBUG, "%s lock held\n", get_lock_mode_name(mode_)));

      cc_lock_set_->unlock(mode_, TAO_TRY_ENV);
      ACE_DEBUG((LM_DEBUG, "%s lock released\n", get_lock_mode_name(mode_)));

      lock_not_held = cc_lock_set_->try_lock(mode_, TAO_TRY_ENV);
      if(lock_not_held)
        {
          ACE_DEBUG((LM_DEBUG, "%s lock not held\n",
                    get_lock_mode_name(mode_)));
        }
      else
        {
          ACE_DEBUG((LM_DEBUG, "%s lock held\n", get_lock_mode_name(mode_)));
          return CC_FAIL;
        }

    }
  TAO_CATCHANY
    {
      TAO_TRY_ENV.print_exception ("Test_Single_Lock_With_Mode::run");
      return CC_FAIL;
    }
  TAO_ENDTRY;

  CORBA::release (cc_lock_set_);
  
  return CC_SUCCESS;
}

Test_Setup_LockSet::Test_Setup_LockSet(CC_naming_service *naming_service,
                                       char *name)
  : CC_Test (naming_service), my_name_ (name)
{
}
  
Test_Setup_LockSet::~Test_Setup_LockSet()
{
}

int Test_Setup_LockSet::run(int times_to_run)
{
  CORBA::Boolean lock_held = CORBA::B_FALSE;

  TAO_TRY
    {
      CosConcurrencyControl::LockSet_ptr cc_lock_set_ = create_lock_set();
      this->naming_service_->bind_name(my_name_, cc_lock_set_,
                                       TAO_TRY_ENV);
      ACE_DEBUG((LM_DEBUG, "Name bound\n"));
      
      cc_lock_set_->lock(CosConcurrencyControl::read, TAO_TRY_ENV);
      ACE_DEBUG((LM_DEBUG, "Read lock set\n"));
    }
  TAO_CATCHANY
    {
      TAO_TRY_ENV.print_exception ("Test_Against_Other_LockSet::run");
      return CC_FAIL;
    }
  TAO_ENDTRY;
  
  return CC_SUCCESS;
}

Test_Use_Already_Created_LockSet::
Test_Use_Already_Created_LockSet(CC_naming_service *naming_service,
                                 char *name)
  : CC_Test (naming_service), my_name_ (name)
{
}
  
Test_Use_Already_Created_LockSet::~Test_Use_Already_Created_LockSet()
{
}
  
int Test_Use_Already_Created_LockSet::run(int times_to_run)
{
  CORBA::Boolean lock_held = CORBA::B_FALSE;
  
  TAO_TRY
    {
      CORBA::Object_var ccls_obj = 
        this->naming_service_->get_obj_from_name("", my_name_, 
                                                 TAO_TRY_ENV);
      CosConcurrencyControl::LockSet_var ccls =
        CosConcurrencyControl::LockSet::_narrow(ccls_obj.in(), TAO_TRY_ENV);
      
      ccls->lock(CosConcurrencyControl::read, TAO_TRY_ENV);
    }
  TAO_CATCHANY
    {
      TAO_TRY_ENV.print_exception ("Test_Against_Other_LockSet::run");
      return CC_FAIL;
    }
  TAO_ENDTRY;
  
  return CC_SUCCESS;
}

Test_Unlock_Already_Created_LockSet::
Test_Unlock_Already_Created_LockSet(CC_naming_service *naming_service,
                                 char *name)
  : CC_Test (naming_service), my_name_ (name)
{
}
  
Test_Unlock_Already_Created_LockSet::~Test_Unlock_Already_Created_LockSet()
{
}
  
int Test_Unlock_Already_Created_LockSet::run(int times_to_run)
{
  CORBA::Boolean lock_held = CORBA::B_FALSE;
  
  TAO_TRY
    {
      CORBA::Object_var ccls_obj = 
        this->naming_service_->get_obj_from_name("", my_name_, 
                                                 TAO_TRY_ENV);
      CosConcurrencyControl::LockSet_var ccls =
        CosConcurrencyControl::LockSet::_narrow(ccls_obj.in(), TAO_TRY_ENV);
      
      ccls->unlock(CosConcurrencyControl::read, TAO_TRY_ENV);
    }
  TAO_CATCHANY
    {
      TAO_TRY_ENV.print_exception ("Test_Against_Other_LockSet::run");
      return CC_FAIL;
    }
  TAO_ENDTRY;
  
  return CC_SUCCESS;
}

