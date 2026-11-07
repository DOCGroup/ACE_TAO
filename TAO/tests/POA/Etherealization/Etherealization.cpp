//=============================================================================
/**
 *  @file     Etherealization.cpp
 *
 *   This program tests for deactivation and etherealization of
 *   reference counted and non reference counted servants.
 *
 *  @author  Irfan Pyarali
 */
#include "testS.h"
#include "tao/PortableServer/ServantActivatorC.h"
#include <cstring>

class test_i : public POA_test
{
public:
  test_i () = default;
  void method () override
  {
  }

  ~test_i () override
  {
    ACE_DEBUG ((LM_DEBUG, "~test_i called\n"));
  }

  test_i (test_i const &) = delete;
  test_i (test_i &&) = delete;
  test_i &operator= (test_i const &) = delete;
  test_i &operator= (test_i &&) = delete;
};

class test_i_with_reference_counting :
  public virtual POA_test
{
public:
  test_i_with_reference_counting () = default;
  void method () override
  {
  }

  ~test_i_with_reference_counting () override
  {
    ACE_DEBUG ((LM_DEBUG, "~test_i_with_reference_counting called\n"));
  }

  test_i_with_reference_counting (test_i_with_reference_counting const &) = delete;
  test_i_with_reference_counting (test_i_with_reference_counting &&) = delete;
  test_i_with_reference_counting &operator= (test_i_with_reference_counting const &) = delete;
  test_i_with_reference_counting &operator= (test_i_with_reference_counting &&) = delete;
};

class Servant_Activator : public PortableServer::ServantActivator
{
public:
  Servant_Activator () = default;
  Servant_Activator (Servant_Activator const &) = delete;
  Servant_Activator (Servant_Activator &&) = delete;
  Servant_Activator &operator= (Servant_Activator const &) = delete;
  Servant_Activator &operator= (Servant_Activator &&) = delete;

  PortableServer::Servant incarnate (PortableServer::ObjectId const &oid,
                                     PortableServer::POA_ptr poa) override;

  void etherealize (PortableServer::ObjectId const &oid,
                    PortableServer::POA_ptr adapter,
                    PortableServer::Servant servant,
                    CORBA::Boolean cleanup_in_progress,
                    CORBA::Boolean remaining_activations) override;
};

PortableServer::Servant
Servant_Activator::incarnate (PortableServer::ObjectId const &id,
                              PortableServer::POA_ptr)
{
  CORBA::String_var const object_name =
    PortableServer::ObjectId_to_string (id);

  ACE_DEBUG ((LM_DEBUG,
              "\nIncarnate called with id = \"%C\"\n",
              object_name.in ()));

  if (std::strcmp (object_name.in (), "without reference counting") == 0)
    // The POA assumes ownership of the non-reference-counted servant.
    return new test_i;
  else
    // The POA and etherealize callback manage this servant's reference count.
    return new test_i_with_reference_counting;
}

void
Servant_Activator::etherealize (PortableServer::ObjectId const &id,
                                PortableServer::POA_ptr,
                                PortableServer::Servant servant,
                                CORBA::Boolean,
                                CORBA::Boolean)
{
  CORBA::String_var const object_name =
    PortableServer::ObjectId_to_string (id);

  ACE_DEBUG ((LM_DEBUG,
              "Etherealize called with id = \"%C\"\n",
              object_name.in ()));

  if (std::strcmp (object_name.in (), "without reference counting") == 0)
    delete servant;
  else
    {
      servant->_remove_ref ();
    }
}

int
ACE_TMAIN (int argc, ACE_TCHAR *argv[])
{
  try
    {
      // Initialize the ORB first.
      CORBA::ORB_var const orb = CORBA::ORB_init (argc, argv);

      // Obtain the RootPOA.
      CORBA::Object_var object =
        orb->resolve_initial_references ("RootPOA");

      // Get the POA_var object from Object_var.
      PortableServer::POA_var const root_poa =
        PortableServer::POA::_narrow (object.in ());

      // Get the POAManager of the RootPOA.
      PortableServer::POAManager_var const poa_manager =
        root_poa->the_POAManager ();

      CORBA::PolicyList policies (3);
      policies.length (3);

      // ID Assignment Policy
      policies[0] =
        root_poa->create_id_assignment_policy (PortableServer::USER_ID);

      // Lifespan Policy
      policies[1] =
        root_poa->create_lifespan_policy (PortableServer::PERSISTENT);

      // Request Processing Policy
      policies[2] =
        root_poa->create_request_processing_policy (PortableServer::USE_SERVANT_MANAGER);

      PortableServer::POA_var const child_poa =
        root_poa->create_POA ("child",
                              poa_manager.in (),
                              policies);

      poa_manager->activate ();

      // Create servant activator.
      Servant_Activator servant_manager;

      // Set servant_activator as the servant_manager of child POA.
      child_poa->set_servant_manager (&servant_manager);

      {
        // Create a reference with user created ID in child POA which
        // uses the Servant_Activator.
        PortableServer::ObjectId_var const id =
          PortableServer::string_to_ObjectId ("without reference counting");

        object =
          child_poa->create_reference_with_id (id.in (),
                                               "IDL:test:1.0");

        test_var const test =
          test::_narrow (object.in ());

        test->method ();

        child_poa->deactivate_object (id.in ());
      }

      {
        // Create a reference with user created ID in child POA which
        // uses the Servant_Activator.
        PortableServer::ObjectId_var const id =
          PortableServer::string_to_ObjectId ("with reference counting");

        object =
          child_poa->create_reference_with_id (id.in (),
                                               "IDL:test:1.0");

        test_var const test =
          test::_narrow (object.in ());

        test->method ();

        child_poa->deactivate_object (id.in ());
      }

      {
        // Create a reference with user created ID in child POA which
        // uses the Servant_Activator.
        PortableServer::ObjectId_var const id =
          PortableServer::string_to_ObjectId ("no call made");

        object =
          child_poa->create_reference_with_id (id.in (),
                                               "IDL:test:1.0");

        child_poa->deactivate_object (id.in ());
      }

      {
        // Create a reference with user created ID in child POA which
        // uses the Servant_Activator but just don't call the reference at all
        PortableServer::ObjectId_var const id =
          PortableServer::string_to_ObjectId ("no call");

        object =
          child_poa->create_reference_with_id (id.in (),
                                               "IDL:test:1.0");

        PortableServer::ObjectId_var const oid =
          child_poa->reference_to_id (object.in ());

        child_poa->deactivate_object (oid.in ());
      }

      ACE_DEBUG ((LM_DEBUG,
                  "\nEnd of main()\n\n"));

      orb->destroy ();
    }
  catch (CORBA::Exception const &ex)
    {
      ex._tao_print_exception ("Exception caught");
      return -1;
    }

  return 0;
}
