
//=============================================================================
/**
 *  @file     MT_Servant_Locator.cpp
 *
 *  $Id$
 *
 *   This program tests that multiple calls to the Servant Locator
 *   can take place simultaneously.
 *
 *
 *  @author  Irfan Pyarali
 */
//=============================================================================


#include "testS.h"
#include "ace/Task.h"
#include "ace/Auto_Event.h"
#include "tao/PortableServer/ServantLocatorC.h"
#include "tao/CDR.h"

class test_i :
  public virtual POA_test
{
public:

  test_i (PortableServer::POA_ptr poa);

  void method (void);

  PortableServer::POA_var poa_;
};

test_i::test_i (PortableServer::POA_ptr poa)
  : poa_ (PortableServer::POA::_duplicate (poa))
{
}

void
test_i::method (void)
{
}

class Task : public ACE_Task_Base
{
public:

  void object (test_ptr test);

  int svc (void);

  test_var test_;

  ACE_Auto_Event pre_invoke_event_;
  ACE_Auto_Event post_invoke_event_;
};

void
Task::object (test_ptr test)
{
  this->test_ =
    test::_duplicate (test);
}

int
Task::svc (void)
{
  this->test_->method ();
  this->test_ =
    test::_nil ();
  return 0;
}

Task first_task;
Task second_task;

class Servant_Locator :
  public PortableServer::ServantLocator
{
public:

  Servant_Locator (PortableServer::POA_ptr poa);

  ::PortableServer::Servant preinvoke (const PortableServer::ObjectId &,
                                       PortableServer::POA_ptr,
                                       const char *,
                                       PortableServer::ServantLocator::Cookie &);

  void postinvoke (const PortableServer::ObjectId &,
                   PortableServer::POA_ptr,
                   const char *,
                   PortableServer::ServantLocator::Cookie,
                   PortableServer::Servant);

  test_i servant_;
};

Servant_Locator::Servant_Locator (PortableServer::POA_ptr poa)
  : servant_ (poa)
{
}

::PortableServer::Servant
Servant_Locator::preinvoke (const PortableServer::ObjectId &oid,
                            PortableServer::POA_ptr,
                            const char *,
                            PortableServer::ServantLocator::Cookie &)
{
  CORBA::String_var name =
    PortableServer::ObjectId_to_string (oid);

  ACE_DEBUG ((LM_DEBUG,
              "(%t) Starting Servant_Locator::preinvoke for %C servant\n",
              name.in ()));

  if (ACE_OS::strcmp (name.in (), "first") == 0)
    {
      second_task.pre_invoke_event_.signal ();
      first_task.pre_invoke_event_.wait ();
    }
  else
    {
      first_task.pre_invoke_event_.signal ();
      second_task.pre_invoke_event_.wait ();
    }

  ACE_DEBUG ((LM_DEBUG,
              "(%t) Ending Servant_Locator::preinvoke for %C servant\n",
              name.in ()));

  return &this->servant_;
}

void
Servant_Locator::postinvoke (const PortableServer::ObjectId &oid,
                             PortableServer::POA_ptr,
                             const char *,
                             PortableServer::ServantLocator::Cookie,
                             PortableServer::Servant)
{
  CORBA::String_var name =
    PortableServer::ObjectId_to_string (oid);

  ACE_DEBUG ((LM_DEBUG,
              "(%t) Starting Servant_Locator::postinvoke for %C servant\n",
              name.in ()));

  if (ACE_OS::strcmp (name.in (), "first") == 0)
    {
      second_task.post_invoke_event_.signal ();
      first_task.post_invoke_event_.wait ();
    }
  else
    {
      first_task.post_invoke_event_.signal ();
      second_task.post_invoke_event_.wait ();
    }

  ACE_DEBUG ((LM_DEBUG,
              "(%t) Ending Servant_Locator::postinvoke for %C servant\n",
              name.in ()));
}

bool
set_nil_servant_manager (PortableServer::POA_ptr poa)
{
  bool succeed = false;
  try
    {
      // Setting a nil servant manager should give an OBJ_Adapter exception with
      // minor code 4
      poa->set_servant_manager (PortableServer::ServantManager::_nil());
    }
  catch (const CORBA::OBJ_ADAPTER& ex)
    {
      if ((ex.minor() & 0xFFFU) == 4)
        {
          succeed = true;
        }
    }
  catch (const CORBA::Exception&)
    {
    }

  if (!succeed)
  {
    ACE_ERROR ((LM_ERROR,
                "(%t) ERROR, set nil servant manager failed\n"));
  }

  return succeed;
}

bool
overwrite_servant_manager (PortableServer::POA_ptr poa)
{
  bool succeed = false;
  try
    {
      Servant_Locator servant_locator (poa);

      // Setting a servant manager after it is already set should give
      // obj_adapter with minor code 6
      poa->set_servant_manager (&servant_locator);
    }
  catch (const CORBA::BAD_INV_ORDER& ex)
    {
      if ((ex.minor() & 0xFFFU) == 6)
        {
          succeed = true;
        }
    }
  catch (const CORBA::Exception&)
    {
    }

  if (!succeed)
  {
    ACE_ERROR ((LM_ERROR,
                "(%t) ERROR, overwrite servant manager failed\n"));
  }

  return succeed;
}

int
ACE_TMAIN (int argc, ACE_TCHAR *argv[])
{
  int retval = 0;

  try
    {
      CORBA::ORB_var orb =
        CORBA::ORB_init (argc, argv);

      CORBA::Object_var obj =
        orb->resolve_initial_references ("RootPOA");

      PortableServer::POA_var root_poa =
        PortableServer::POA::_narrow (obj.in ());

      PortableServer::POAManager_var poa_manager =
        root_poa->the_POAManager ();

      poa_manager->activate ();

      CORBA::PolicyList policies;
      CORBA::ULong current_length = 0;

      policies.length (current_length + 1);
      policies[current_length++] =
        root_poa->create_request_processing_policy (PortableServer::USE_SERVANT_MANAGER);

      policies.length (current_length + 1);
      policies[current_length++] =
        root_poa->create_servant_retention_policy (PortableServer::NON_RETAIN);

      policies.length (current_length + 1);
      policies[current_length++] =
        root_poa->create_id_assignment_policy (PortableServer::USER_ID);

      PortableServer::POA_var child_poa =
        root_poa->create_POA ("child",
                              poa_manager.in (),
                              policies);

      if (!set_nil_servant_manager (child_poa.in()))
        retval = -1;

      Servant_Locator servant_locator (child_poa.in ());
      child_poa->set_servant_manager (&servant_locator);

      if (!overwrite_servant_manager (child_poa.in()))
        retval = -1;

      PortableServer::ObjectId_var first_oid =
        PortableServer::string_to_ObjectId ("first");

      CORBA::Object_var first_object =
        child_poa->create_reference_with_id (first_oid.in (),
                                             "IDL:test:1.0");

      test_var first_test =
        test::_narrow (first_object.in ());

      PortableServer::ObjectId_var second_oid =
        PortableServer::string_to_ObjectId ("second");

      CORBA::Object_var second_object =
        child_poa->create_reference_with_id (second_oid.in (),
                                             "IDL:test:1.0");

      test_var second_test =
        test::_narrow (second_object.in ());

      first_task.object (first_test.in ());
      second_task.object (second_test.in ());

      first_task.activate ();
      second_task.activate ();

      first_task.wait ();
      second_task.wait ();

      root_poa->destroy (1,
                         1);

      orb->destroy ();

      ACE_DEBUG ((LM_DEBUG,
                  "%s successful\n",
                  argv[0]));
    }
  catch (const CORBA::Exception& ex)
    {
      ex._tao_print_exception ("Exception caught");
      retval = -1;
    }

  return retval;
}
