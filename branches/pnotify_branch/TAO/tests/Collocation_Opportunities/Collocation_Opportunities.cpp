// $Id$

#include "testS.h"
#include "ace/Task.h"
#include "tao/PortableServer/Object_Adapter.h"

ACE_RCSID(Collocation_Opportunities, Collocation_Opportunities, "$Id$")

class test_i :
  public POA_test,
  public PortableServer::RefCountServantBase
{
public:

  test_i (CORBA::ORB_ptr orb,
          PortableServer::POA_ptr poa
          ACE_ENV_ARG_DECL);

  void set_other (test_ptr test);

  void method (ACE_ENV_SINGLE_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException));

  PortableServer::POA_ptr _default_POA (ACE_ENV_SINGLE_ARG_DECL);

  CORBA::ORB_var orb_;

  PortableServer::POA_var poa_;

  test_var other_;

  PortableServer::Current_var poa_current_;
};

test_i::test_i (CORBA::ORB_ptr orb,
                PortableServer::POA_ptr poa
                ACE_ENV_ARG_DECL)
  : orb_ (CORBA::ORB::_duplicate (orb)),
    poa_ (PortableServer::POA::_duplicate (poa))
{
  CORBA::Object_var object;

  object =
    this->orb_->resolve_initial_references ("POACurrent"
                                            ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;

  this->poa_current_ =
    PortableServer::Current::_narrow (object.in ()
                                      ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;
}

void
test_i::set_other (test_ptr test)
{
  this->other_ = test::_duplicate (test);
}

void
test_i::method (ACE_ENV_SINGLE_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  if (this->other_.in () != test::_nil ())
    {
      this->other_->method (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_CHECK;
    }
  else
    {
      TAO_POA_Current &tao_poa_current =
        ACE_dynamic_cast (TAO_POA_Current &,
                          *(this->poa_current_.in ()));

      TAO_POA_Current_Impl &tao_poa_current_implementation =
        *tao_poa_current.implementation ();

      const char *upcall = 0;
      if (tao_poa_current_implementation.previous ())
        upcall = "collocated";
      else
        upcall = "remote";

      PortableServer::ObjectId_var id =
        this->poa_current_->get_object_id (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_CHECK;

      CORBA::String_var id_string =
        PortableServer::ObjectId_to_string (id.in ());

      ACE_DEBUG ((LM_DEBUG,
                  "%s method() invoked for %s servant in thread %t\n",
                  upcall,
                  id_string.in ()));
    }
}

PortableServer::POA_ptr
test_i::_default_POA (ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
{
  return PortableServer::POA::_duplicate (this->poa_.in ());
}

class Task : public ACE_Task_Base
{
public:

  Task (CORBA::ORB_ptr orb);

  int svc (void);

  CORBA::ORB_var orb_;

};

Task::Task (CORBA::ORB_ptr orb)
  : orb_ (CORBA::ORB::_duplicate (orb))
{
}

int
Task::svc (void)
{
  ACE_TRY_NEW_ENV
    {
      this->orb_->run (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;
    }
  ACE_CATCHANY
    {
    }
  ACE_ENDTRY;

  return 0;
}

int
main (int argc, char *argv[])
{
  ACE_TRY_NEW_ENV
    {
      CORBA::ORB_var orb =
        CORBA::ORB_init (argc,
                         argv,
                         0
                         ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      CORBA::Object_var object =
        orb->resolve_initial_references ("RootPOA"
                                         ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      PortableServer::POA_var root_poa =
        PortableServer::POA::_narrow (object.in ()
                                      ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      PortableServer::POAManager_var poa_manager =
        root_poa->the_POAManager (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;

      poa_manager->activate (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;

      Task task (orb.in ());

      int result =
        task.activate ();
      ACE_ASSERT (result != -1);
      ACE_UNUSED_ARG (result);

      CORBA::PolicyList policies (1);
      policies.length (1);

      policies[0] =
        root_poa->create_id_assignment_policy (PortableServer::USER_ID
                                               ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      PortableServer::POA_var child_poa =
        root_poa->create_POA ("child_poa",
                              poa_manager.in (),
                              policies
                              ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      test_i *base_servant =
        new test_i (orb.in (),
                    child_poa.in ()
                    ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      PortableServer::ServantBase_var safe_base_servant (base_servant);

      PortableServer::ObjectId_var base_oid =
        PortableServer::string_to_ObjectId ("base");

      child_poa->activate_object_with_id (base_oid.in (),
                                          base_servant
                                          ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      test_var base_test =
        base_servant->_this (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;

      test_i *first_servant =
        new test_i (orb.in (),
                    child_poa.in ()
                    ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      PortableServer::ServantBase_var safe_first_servant (first_servant);

      PortableServer::ObjectId_var first_oid =
        PortableServer::string_to_ObjectId ("first");

      child_poa->activate_object_with_id (first_oid.in (),
                                          first_servant
                                          ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      test_var first_test =
        first_servant->_this (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;

      base_servant->set_other (first_test.in ());

      base_test->method (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;

      PortableServer::ObjectId_var second_oid =
        PortableServer::string_to_ObjectId ("second");

      object =
        child_poa->create_reference_with_id (second_oid.in (),
                                             "IDL:test:1.0"
                                             ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      test_var second_test =
        test::_narrow (object.in ()
                       ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      test_i *second_servant =
        new test_i (orb.in (),
                    child_poa.in ()
                    ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      PortableServer::ServantBase_var safe_second_servant (second_servant);

      child_poa->activate_object_with_id (second_oid.in (),
                                          second_servant
                                          ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      base_servant->set_other (second_test.in ());

      base_test->method (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;

      PortableServer::ObjectId_var third_oid =
        PortableServer::string_to_ObjectId ("third");

      object =
        child_poa->create_reference_with_id (third_oid.in (),
                                             "IDL:test:1.0"
                                             ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      CORBA::String_var third_ior =
        orb->object_to_string (object.in ()
                               ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      object =
        orb->string_to_object (third_ior.in ()
                               ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      test_var third_test =
        test::_narrow (object.in ()
                       ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      test_i *third_servant =
        new test_i (orb.in (),
                    child_poa.in ()
                    ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      PortableServer::ServantBase_var safe_third_servant (third_servant);

      child_poa->activate_object_with_id (third_oid.in (),
                                          third_servant
                                          ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      base_servant->set_other (third_test.in ());

      base_test->method (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;

      orb->shutdown (1
                     ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      result =
        task.wait ();
      ACE_ASSERT (result != -1);
      ACE_UNUSED_ARG (result);
    }
  ACE_CATCHANY
    {
      ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION,
                           "Caught exception:");
      return -1;
    }
  ACE_ENDTRY;

  return 0;
}
