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

  test_i (test_i const &) = delete;
  test_i (test_i &&) = delete;
  test_i &operator= (test_i const &) = delete;
  test_i &operator= (test_i &&) = delete;

  void set_other (test_ptr test);

  void method () override;

  PortableServer::POA_ptr _default_POA () override;

  CORBA::ORB_var const orb_;

  PortableServer::POA_var const poa_;

  test_var other_;

  PortableServer::Current_var poa_current_;
};

test_i::test_i (CORBA::ORB_ptr orb,
                PortableServer::POA_ptr poa)
  : orb_ (CORBA::ORB::_duplicate (orb))
  , poa_ (PortableServer::POA::_duplicate (poa))
{
  CORBA::Object_var const object {this->orb_->resolve_initial_references ("POACurrent")};
  this->poa_current_ = PortableServer::Current::_narrow (object.in ());
}

void
test_i::set_other (test_ptr test)
{
  this->other_ = test::_duplicate (test);
}

void
test_i::method ()
{
  if (!CORBA::is_nil (this->other_.in ()))
    {
      this->other_->method ();
    }
  else
    {
      TAO::Portable_Server::POA_Current &tao_poa_current =
        dynamic_cast<TAO::Portable_Server::POA_Current &> (*(this->poa_current_.in ()));

      TAO::Portable_Server::POA_Current_Impl &tao_poa_current_implementation =
        *tao_poa_current.implementation ();

      char const * const upcall = tao_poa_current_implementation.previous () ? "collocated" : "remote";

      PortableServer::ObjectId_var const id {this->poa_current_->get_object_id ()};

      CORBA::String_var const id_string {PortableServer::ObjectId_to_string (id.in ())};

      ACE_DEBUG ((LM_DEBUG,
                  "%C method() invoked for <%C> servant in thread %t\n",
                  upcall,
                  id_string.in ()));
    }
}

PortableServer::POA_ptr
test_i::_default_POA ()
{
  return PortableServer::POA::_duplicate (this->poa_.in ());
}

class Task : public ACE_Task_Base
{
public:
  explicit Task (CORBA::ORB_ptr orb);

  Task (Task const&) = delete;
  Task (Task&&) = delete;
  Task& operator= (Task const&) = delete;
  Task& operator= (Task&&) = delete;

  int svc () override;

  CORBA::ORB_var const orb_;
};

Task::Task (CORBA::ORB_ptr orb)
  : orb_ (CORBA::ORB::_duplicate (orb))
{
}

int
Task::svc ()
{
  try
    {
      this->orb_->run ();
    }
  catch (CORBA::Exception const&)
    {
    }

  return 0;
}

int
ACE_TMAIN (int argc, ACE_TCHAR *argv[])
{
  try
    {
      CORBA::ORB_var const orb {CORBA::ORB_init (argc, argv)};

      CORBA::Object_var const root_poa_object {orb->resolve_initial_references ("RootPOA")};

      PortableServer::POA_var const root_poa {PortableServer::POA::_narrow (root_poa_object.in ())};

      PortableServer::POAManager_var const poa_manager {root_poa->the_POAManager ()};

      poa_manager->activate ();

      Task task (orb.in ());

      int const result = task.activate ();
      ACE_ASSERT (result != -1);
      ACE_UNUSED_ARG (result);

      CORBA::PolicyList policies {};
      policies.length (1);

      policies[0] =
        root_poa->create_id_assignment_policy (PortableServer::USER_ID);

      PortableServer::POA_var const child_poa =
        root_poa->create_POA ("child_poa",
                              poa_manager.in (),
                              policies);

      // The servant var owns each servant; the raw aliases below are nonowning.
      test_i * const base_servant = new test_i (orb.in (), child_poa.in ());

      PortableServer::ServantBase_var const safe_base_servant {base_servant};

      PortableServer::ObjectId_var const base_oid {PortableServer::string_to_ObjectId ("base")};

      child_poa->activate_object_with_id (base_oid.in (),
                                          base_servant);

      PortableServer::ObjectId_var const id_act {root_poa->activate_object (base_servant)};

      CORBA::Object_var const object_act {root_poa->id_to_reference (id_act.in ())};

      test_var const base_test {test::_narrow (object_act.in ())};

      test_i * const first_servant = new test_i (orb.in (), child_poa.in ());

      PortableServer::ServantBase_var const safe_first_servant {first_servant};

      PortableServer::ObjectId_var const first_oid {PortableServer::string_to_ObjectId ("first")};

      child_poa->activate_object_with_id (first_oid.in (),
                                          first_servant);

      PortableServer::ObjectId_var const id_actu {root_poa->activate_object (first_servant)};

      CORBA::Object_var const first_object_act {root_poa->id_to_reference (id_actu.in ())};

      test_var const first_test {test::_narrow (first_object_act.in ())};

      base_servant->set_other (first_test.in ());

      base_test->method ();

      PortableServer::ObjectId_var const second_oid {PortableServer::string_to_ObjectId ("second")};

      CORBA::Object_var const second_object {
        child_poa->create_reference_with_id (second_oid.in (), "IDL:test:1.0")};

      test_var const second_test {test::_narrow (second_object.in ())};

      test_i * const second_servant = new test_i (orb.in (), child_poa.in ());

      PortableServer::ServantBase_var const safe_second_servant {second_servant};

      child_poa->activate_object_with_id (second_oid.in (),
                                          second_servant);

      base_servant->set_other (second_test.in ());

      base_test->method ();

      PortableServer::ObjectId_var const third_oid {PortableServer::string_to_ObjectId ("third")};

      CORBA::Object_var third_object {
        child_poa->create_reference_with_id (third_oid.in (), "IDL:test:1.0")};

      CORBA::String_var const third_ior {orb->object_to_string (third_object.in ())};

      third_object = orb->string_to_object (third_ior.in ());

      test_var const third_test {test::_narrow (third_object.in ())};

      test_i * const third_servant = new test_i (orb.in (), child_poa.in ());

      PortableServer::ServantBase_var const safe_third_servant {third_servant};

      child_poa->activate_object_with_id (third_oid.in (),
                                          third_servant);

      base_servant->set_other (third_test.in ());

      base_test->method ();

      orb->shutdown (true);

      int const wait_result = task.wait ();
      ACE_ASSERT (wait_result != -1);
      ACE_UNUSED_ARG (wait_result);
    }
  catch (CORBA::Exception const &ex)
    {
      ex._tao_print_exception ("Caught exception:");
      return -1;
    }

  return 0;
}
