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
#include "ace/Log_Msg.h"



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
  try
    {
      lock_set =
        this->naming_service_->get_lock_set_factory ()->create ();

      if (CORBA::is_nil (lock_set))
        ACE_ERROR_RETURN ((LM_ERROR,
                           "null lock set objref returned by factory\n"),
                          0);
    }
  catch (const CORBA::Exception& ex)
    {
      ex._tao_print_exception ("CC_Client::create_lock_set");
      return 0;
    }
  return lock_set;
}

char *
CC_Test::get_lock_mode_name (CosConcurrencyControl::lock_mode mode)
{
  if (mode==CosConcurrencyControl::read)
    return const_cast<char *> ("read");

  if (mode==CosConcurrencyControl::write)
    return const_cast<char *> ("write");

  if (mode==CosConcurrencyControl::upgrade)
    return const_cast<char *> ("upgrade");

  if (mode==CosConcurrencyControl::intention_read)
    return const_cast<char *> ("intension_read");

  if (mode==CosConcurrencyControl::intention_write)
    return const_cast<char *> ("intension_write");

  return const_cast<char *> ("unknown lock mode");
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

  try
    {
      cc_lock_set_->lock (mode_);

      ACE_DEBUG ((LM_DEBUG,
                  "%s lock set\n",
                  get_lock_mode_name (mode_)));

      lock_not_held = cc_lock_set_->try_lock (mode_);

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

      cc_lock_set_->unlock (mode_);
      ACE_DEBUG ((LM_DEBUG,
                  "%s lock released\n",
                  get_lock_mode_name (mode_)));

      lock_not_held = cc_lock_set_->try_lock (mode_);

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
  catch (const CORBA::Exception& ex)
    {
      ex._tao_print_exception ("Test_Single_Lock_With_Mode::run");
      return CC_FAIL;
    }

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
  try
    {
      CosConcurrencyControl::LockSet_ptr cc_lock_set_ =
        create_lock_set ();
      this->naming_service_->bind_name (my_name_, cc_lock_set_);

      ACE_DEBUG ((LM_DEBUG,
                  "Name bound\n"));

      cc_lock_set_->lock (CosConcurrencyControl::read);

      ACE_DEBUG ((LM_DEBUG,
                  "Read lock set\n"));
    }
  catch (const CORBA::Exception& ex)
    {
      ex._tao_print_exception ("Test_Against_Other_LockSet::run");
      return CC_FAIL;
    }

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
  try
    {
      CORBA::Object_var ccls_obj =
        this->naming_service_->get_obj_from_name (const_cast<char *> (""),
                                                  my_name_);

      CosConcurrencyControl::LockSet_var ccls =
        CosConcurrencyControl::LockSet::_narrow (ccls_obj.in ());

      ccls->lock (CosConcurrencyControl::read);

    }
  catch (const CORBA::Exception& ex)
    {
      ex._tao_print_exception ("Test_Against_Other_LockSet::run");
      return CC_FAIL;
    }

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
  try
    {
      CORBA::Object_var ccls_obj =
        this->naming_service_->get_obj_from_name (const_cast<char *> (""), my_name_);

      CosConcurrencyControl::LockSet_var ccls =
        CosConcurrencyControl::LockSet::_narrow (ccls_obj.in ());

      ccls->unlock (CosConcurrencyControl::read);
    }
  catch (const CORBA::Exception& ex)
    {
      ex._tao_print_exception ("Test_Unlock_Already_Created_LockSet::run");
      return CC_FAIL;
    }

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
  try
    {
      // lock the lock
      cc_lock_set_->lock (mode_);

      ACE_DEBUG ((LM_DEBUG,
                  "%s lock set\n",
                  get_lock_mode_name (mode_)));

      // check to see if the lock is held
      lock_not_held = cc_lock_set_->try_lock (mode_);

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
      cc_lock_set_->unlock (mode_);
      ACE_DEBUG ((LM_DEBUG,
                  "%s lock released\n",
                  get_lock_mode_name (mode_)));

      // release the lock again. this should raise an exception
      cc_lock_set_->unlock (mode_);

      ACE_DEBUG ((LM_DEBUG,
                  "attemptet to release %s lock\n",
                  get_lock_mode_name (mode_)));

    }
  catch (const CosConcurrencyControl::LockNotHeld& userex)
    {
      // We should end here
      ACE_UNUSED_ARG (userex);
      ACE_DEBUG((LM_DEBUG, "CosConcurrencyControl::LockNotHeld\n"));
      return CC_SUCCESS;
    }

  catch (const CORBA::Exception& ex)
    {
      ex._tao_print_exception ("Test_Release_Not_Held_Lock::run");
      return CC_FAIL;
    }

  CORBA::release (cc_lock_set_);

  return CC_FAIL;
}
