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


Test_Single_Lock_With_Mode::Test_Single_Lock_With_Mode(CC_naming_service *naming_service,
                                                       CosConcurrencyControl::lock_mode mode) : CC_Test (naming_service)
{
}

Test_Single_Lock_With_Mode::~Test_Single_Lock_With_Mode()
{
}

Test_Single_Lock_With_Mode::run(int times_to_run)
{
  CORBA::Environment _env;
  CORBA::Boolean lock_held;
  char s[10];
  
  gets(s);
  
  CosConcurrencyControl::LockSet_ptr cc_lock_set_ = create_lock_set();

  naming_service_->bind_name("LockSet_1", cc_lock_set_, _env);
  ACE_DEBUG((LM_DEBUG, "Name bound\n"));
  cc_lock_set_->lock(CosConcurrencyControl::read, _env);
  ACE_DEBUG((LM_DEBUG, "Read lock set\n"));
 
  gets(s);

  CORBA::Object_var ccls_obj = 
    naming_service_->get_obj_from_name("",
                                       "LockSet_1", _env);
  CosConcurrencyControl::LockSet_var ccls =
    CosConcurrencyControl::LockSet::_narrow(ccls_obj.in(), _env);

  lock_held = ccls->try_lock(CosConcurrencyControl::read, _env);
  //  lock_held = this->cc_lock_set_->try_lock(CosConcurrencyControl::read, _env);
  if(lock_held)
    ACE_DEBUG((LM_DEBUG, "Read lock not held\n"));
  else
    ACE_DEBUG((LM_DEBUG, "Read lock held\n"));

  gets(s);

  cc_lock_set_->unlock(CosConcurrencyControl::read, _env);
  ACE_DEBUG((LM_DEBUG, "Read lock released\n"));

  gets(s);

  cc_lock_set_->lock(CosConcurrencyControl::write, _env);
  ACE_DEBUG((LM_DEBUG, "Write lock set\n"));
  
  gets(s);

  CORBA::release (cc_lock_set_);

  return CC_SUCCESS;
}

Test_Against_Other_LockSet::
Test_Against_Other_LockSet(CC_naming_service *naming_service,
                           char *lock_set) : CC_Test (naming_service)
{
}

Test_Against_Other_LockSet::~Test_Against_Other_LockSet()
{
}

int Test_Against_Other_LockSet::run(int times_to_run)
{
  CORBA::Boolean lock_held = CORBA::B_FALSE;
  CosConcurrencyControl::LockSet_ptr cc_lock_set_ = create_lock_set();

  TAO_TRY
    {
      this->naming_service_->bind_name("LockSet_1", cc_lock_set_,
                                       TAO_TRY_ENV);
      ACE_DEBUG((LM_DEBUG, "Name bound\n"));
      cc_lock_set_->lock(CosConcurrencyControl::read, TAO_TRY_ENV);
      ACE_DEBUG((LM_DEBUG, "Read lock set\n"));
      
      CORBA::Object_var ccls_obj = 
        this->naming_service_->get_obj_from_name("", "LockSet_1", 
                                                 TAO_TRY_ENV);
      CosConcurrencyControl::LockSet_var ccls =
        CosConcurrencyControl::LockSet::_narrow(ccls_obj.in(), TAO_TRY_ENV);
      
      lock_held = ccls->try_lock(CosConcurrencyControl::read, TAO_TRY_ENV);
    }
  TAO_CATCHANY
    {
      TAO_TRY_ENV.print_exception ("CC_Client::run_extended_tests");
      return CC_FAIL;
    }
  TAO_ENDTRY;

  return CC_SUCCESS;
}
