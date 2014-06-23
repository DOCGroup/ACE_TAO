// $Id$

#include "testS.h"
#include "ace/Task.h"
#include "tao/PortableServer/Object_Adapter.h"
#include "tao/PortableServer/POA_Current.h"
#include "tao/PortableServer/POA_Current_Impl.h"

class test_i :
  public POA_test
{
public:

  test_i (CORBA::ORB_ptr orb,
          PortableServer::POA_ptr poa);

  void set_other (test_ptr test);

  void method (void);

  PortableServer::POA_ptr _default_POA (void);

  CORBA::ORB_var orb_;

  PortableServer::POA_var poa_;

  test_var other_;

  PortableServer::Current_var poa_current_;
};

test_i::test_i (CORBA::ORB_ptr orb,
                PortableServer::POA_ptr poa)
  : orb_ (CORBA::ORB::_duplicate (orb)),
    poa_ (PortableServer::POA::_duplicate (poa))
{
  CORBA::Object_var object;

  object =
    this->orb_->resolve_initial_references ("POACurrent");

  this->poa_current_ =
    PortableServer::Current::_narrow (object.in ());
}

void
test_i::set_other (test_ptr test)
{
  this->other_ = test::_duplicate (test);
}

void
test_i::method (void)
{
  if (this->other_.in () != test::_nil ())
    {
      this->other_->method ();
    }
  else
    {
      TAO::Portable_Server::POA_Current &tao_poa_current =
        dynamic_cast <TAO::Portable_Server::POA_Current &>
                          (*(this->poa_current_.in ()));

      TAO::Portable_Server::POA_Current_Impl &tao_poa_current_implementation =
        *tao_poa_current.implementation ();

      const char *upcall = 0;
      if (tao_poa_current_implementation.previous ())
        upcall = "collocated";
      else
        upcall = "remote";

      PortableServer::ObjectId_var id =
        this->poa_current_->get_object_id ();

      CORBA::String_var id_string =
        PortableServer::ObjectId_to_string (id.in ());

      ACE_DEBUG ((LM_DEBUG,
                  "%C method() invoked for <%C> servant in thread %t\n",
                  upcall,
                  id_string.in ()));
    }
}

PortableServer::POA_ptr
test_i::_default_POA (void)
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
  try
    {
      this->orb_->run ();
    }
  catch (const CORBA::Exception&)
    {
    }

  return 0;
}

int
ACE_TMAIN(int argc, ACE_TCHAR *argv[])
{
  try
    {
      CORBA::ORB_var orb =
        CORBA::ORB_init (argc, argv);

      CORBA::Object_var object =
        orb->resolve_initial_references ("RootPOA");

      PortableServer::POA_var root_poa =
        PortableServer::POA::_narrow (object.in ());

      PortableServer::POAManager_var poa_manager =
        root_poa->the_POAManager ();

      poa_manager->activate ();

      Task task (orb.in ());

      int result =
        task.activate ();
      ACE_ASSERT (result != -1);
      ACE_UNUSED_ARG (result);

      CORBA::PolicyList policies (1);
      policies.length (1);

      policies[0] =
        root_poa->create_id_assignment_policy (PortableServer::USER_ID);

      PortableServer::POA_var child_poa =
        root_poa->create_POA ("child_poa",
                              poa_manager.in (),
                              policies);

      test_i *base_servant =
        new test_i (orb.in (),
                    child_poa.in ());

      PortableServer::ServantBase_var safe_base_servant (base_servant);

      PortableServer::ObjectId_var base_oid =
        PortableServer::string_to_ObjectId ("base");

      child_poa->activate_object_with_id (base_oid.in (),
                                          base_servant);

      PortableServer::ObjectId_var id_act =
        root_poa->activate_object (base_servant);

      CORBA::Object_var object_act = root_poa->id_to_reference (id_act.in ());

      test_var base_test =
        test::_narrow (object_act.in ());

      test_i *first_servant =
        new test_i (orb.in (),
                    child_poa.in ());

      PortableServer::ServantBase_var safe_first_servant (first_servant);

      PortableServer::ObjectId_var first_oid =
        PortableServer::string_to_ObjectId ("first");

      child_poa->activate_object_with_id (first_oid.in (),
                                          first_servant);

      PortableServer::ObjectId_var id_actu =
        root_poa->activate_object (first_servant);

      object_act = root_poa->id_to_reference (id_actu.in ());

      test_var first_test =
        test::_narrow (object_act.in ());

      base_servant->set_other (first_test.in ());

      base_test->method ();

      PortableServer::ObjectId_var second_oid =
        PortableServer::string_to_ObjectId ("second");

      object =
        child_poa->create_reference_with_id (second_oid.in (),
                                             "IDL:test:1.0");

      test_var second_test =
        test::_narrow (object.in ());

      test_i *second_servant =
        new test_i (orb.in (),
                    child_poa.in ());

      PortableServer::ServantBase_var safe_second_servant (second_servant);

      child_poa->activate_object_with_id (second_oid.in (),
                                          second_servant);

      base_servant->set_other (second_test.in ());

      base_test->method ();

      PortableServer::ObjectId_var third_oid =
        PortableServer::string_to_ObjectId ("third");

      object =
        child_poa->create_reference_with_id (third_oid.in (),
                                             "IDL:test:1.0");

      CORBA::String_var third_ior =
        orb->object_to_string (object.in ());

      object =
        orb->string_to_object (third_ior.in ());

      test_var third_test =
        test::_narrow (object.in ());

      test_i *third_servant =
        new test_i (orb.in (),
                    child_poa.in ());

      PortableServer::ServantBase_var safe_third_servant (third_servant);

      child_poa->activate_object_with_id (third_oid.in (),
                                          third_servant);

      base_servant->set_other (third_test.in ());

      base_test->method ();

      orb->shutdown (1);

      result = task.wait ();
      ACE_ASSERT (result != -1);
      ACE_UNUSED_ARG (result);
    }
  catch (const CORBA::Exception& ex)
    {
      ex._tao_print_exception ("Caught exception:");
      return -1;
    }

  return 0;
}
