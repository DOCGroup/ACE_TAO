// $Id$

//========================================================================
//
// = LIBRARY
//     TAO/tests/POA/MT_Servant_Locator
//
// = FILENAME
//     MT_Servant_Locator.cpp
//
// = DESCRIPTION
//     This program tests that multiple calls to the Servant Locator
//     can take place simultaneously.
//
// = AUTHOR
//     Irfan Pyarali
//
//=========================================================================

#include "testS.h"
#include "ace/Task.h"
#include "ace/Synch.h"

class test_i : public virtual PortableServer::RefCountServantBase,
               public virtual POA_test
{
public:

  test_i (PortableServer::POA_ptr poa);

  void method (ACE_ENV_SINGLE_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException));

  PortableServer::POA_var poa_;
};

test_i::test_i (PortableServer::POA_ptr poa)
  : poa_ (PortableServer::POA::_duplicate (poa))
{
}

void
test_i::method (ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((CORBA::SystemException))
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
                                       PortableServer::ServantLocator::Cookie &
                                       ACE_ENV_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException,
                     PortableServer::ForwardRequest));

  void postinvoke (const PortableServer::ObjectId &,
                   PortableServer::POA_ptr,
                   const char *,
                   PortableServer::ServantLocator::Cookie,
                   PortableServer::Servant
                   ACE_ENV_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException));

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
                            PortableServer::ServantLocator::Cookie &
                            ACE_ENV_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   PortableServer::ForwardRequest))
{
  CORBA::String_var name =
    PortableServer::ObjectId_to_string (oid);

  ACE_DEBUG ((LM_DEBUG,
              "(%t) Starting Servant_Locator::preinvoke for %s servant\n",
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
              "(%t) Ending Servant_Locator::preinvoke for %s servant\n",
              name.in ()));

  return &this->servant_;
}

void
Servant_Locator::postinvoke (const PortableServer::ObjectId &oid,
                             PortableServer::POA_ptr,
                             const char *,
                             PortableServer::ServantLocator::Cookie,
                             PortableServer::Servant
                             ACE_ENV_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  CORBA::String_var name =
    PortableServer::ObjectId_to_string (oid);

  ACE_DEBUG ((LM_DEBUG,
              "(%t) Starting Servant_Locator::postinvoke for %s servant\n",
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
              "(%t) Ending Servant_Locator::postinvoke for %s servant\n",
              name.in ()));
}

int
main (int argc, char **argv)
{
  ACE_TRY_NEW_ENV
    {
      CORBA::ORB_var orb =
        CORBA::ORB_init (argc,
                         argv,
                         0
                         ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      CORBA::Object_var obj =
        orb->resolve_initial_references ("RootPOA"
                                         ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      PortableServer::POA_var root_poa =
        PortableServer::POA::_narrow (obj.in ()
                                      ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      PortableServer::POAManager_var poa_manager =
        root_poa->the_POAManager (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;

      poa_manager->activate (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;

      CORBA::PolicyList policies;
      CORBA::ULong current_length = 0;

      policies.length (current_length + 1);
      policies[current_length++] =
        root_poa->create_request_processing_policy (PortableServer::USE_SERVANT_MANAGER
                                                    ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      policies.length (current_length + 1);
      policies[current_length++] =
        root_poa->create_servant_retention_policy (PortableServer::NON_RETAIN
                                                   ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      policies.length (current_length + 1);
      policies[current_length++] =
        root_poa->create_id_assignment_policy (PortableServer::USER_ID
                                               ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      PortableServer::POA_var child_poa =
        root_poa->create_POA ("child",
                              poa_manager.in (),
                              policies
                              ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      Servant_Locator servant_locator (child_poa.in ());
      child_poa->set_servant_manager (&servant_locator
                                      ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      PortableServer::ObjectId_var first_oid =
        PortableServer::string_to_ObjectId ("first");

      CORBA::Object_var first_object =
        child_poa->create_reference_with_id (first_oid.in (),
                                             "IDL:test:1.0"
                                             ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      test_var first_test =
        test::_narrow (first_object.in ()
                       ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      PortableServer::ObjectId_var second_oid =
        PortableServer::string_to_ObjectId ("second");

      CORBA::Object_var second_object =
        child_poa->create_reference_with_id (second_oid.in (),
                                             "IDL:test:1.0"
                                             ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      test_var second_test =
        test::_narrow (second_object.in ()
                       ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      first_task.object (first_test.in ());
      second_task.object (second_test.in ());

      first_task.activate ();
      second_task.activate ();

      first_task.wait ();
      second_task.wait ();

      root_poa->destroy (1,
                         1
                         ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      ACE_DEBUG ((LM_DEBUG,
                  "%s successful\n",
                  argv[0]));
    }
  ACE_CATCHANY
    {
      ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION, "Exception caught");
      return -1;
    }
  ACE_ENDTRY;

  return 0;
}
