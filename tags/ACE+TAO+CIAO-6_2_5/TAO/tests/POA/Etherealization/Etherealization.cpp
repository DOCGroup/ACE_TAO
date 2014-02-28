
//=============================================================================
/**
 *  @file     Etherealization.cpp
 *
 *  $Id$
 *
 *   This program tests for deactivation and etherealization of
 *   reference counted and non reference counted servants.
 *
 *
 *  @author  Irfan Pyarali
 */
//=============================================================================


#include "testS.h"
#include "ace/OS_NS_string.h"
#include "tao/PortableServer/ServantActivatorC.h"

class test_i : public POA_test
{
public:
  void method ( /**/)
  {
  }

  ~test_i (void)
  {
    ACE_DEBUG ((LM_DEBUG, "~test_i called\n"));
  }
};

class test_i_with_reference_counting :
  public virtual POA_test
{
public:
  void method (void)
  {
  }

  ~test_i_with_reference_counting (void)
  {
    ACE_DEBUG ((LM_DEBUG, "~test_i_with_reference_counting called\n"));
  }
};

class Servant_Activator : public PortableServer::ServantActivator
{
public:
  PortableServer::Servant incarnate (const PortableServer::ObjectId &oid,
                                     PortableServer::POA_ptr poa);

  void etherealize (const PortableServer::ObjectId &oid,
                    PortableServer::POA_ptr adapter,
                    PortableServer::Servant servant,
                    CORBA::Boolean cleanup_in_progress,
                    CORBA::Boolean remaining_activations);
};

PortableServer::Servant
Servant_Activator::incarnate (const PortableServer::ObjectId &id,
                              PortableServer::POA_ptr)
{
  CORBA::String_var object_name =
    PortableServer::ObjectId_to_string (id);

  ACE_DEBUG ((LM_DEBUG,
              "\nIncarnate called with id = \"%C\"\n",
              object_name.in ()));

  if (ACE_OS::strcmp (object_name.in (),
                      "without reference counting") == 0)
    return new test_i;
  else
    return new test_i_with_reference_counting;
}


void
Servant_Activator::etherealize (const PortableServer::ObjectId &id,
                                PortableServer::POA_ptr ,
                                PortableServer::Servant servant,
                                CORBA::Boolean,
                                CORBA::Boolean)
{
  CORBA::String_var object_name =
    PortableServer::ObjectId_to_string (id);

  ACE_DEBUG ((LM_DEBUG,
              "Etherealize called with id = \"%C\"\n",
              object_name.in ()));

  if (ACE_OS::strcmp (object_name.in (),
                      "without reference counting") == 0)
    delete servant;
  else
    {
      servant->_remove_ref ();
    }
}

int
ACE_TMAIN(int argc, ACE_TCHAR *argv[])
{

  try
    {
      // Initialize the ORB first.
      CORBA::ORB_var orb = CORBA::ORB_init (argc, argv);

      // Obtain the RootPOA.
      CORBA::Object_var object =
        orb->resolve_initial_references ("RootPOA");

      // Get the POA_var object from Object_var.
      PortableServer::POA_var root_poa =
        PortableServer::POA::_narrow (object.in ());

      // Get the POAManager of the RootPOA.
      PortableServer::POAManager_var poa_manager =
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

      PortableServer::POA_var child_poa =
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
        PortableServer::ObjectId_var id =
          PortableServer::string_to_ObjectId ("without reference counting");

        object =
          child_poa->create_reference_with_id (id.in (),
                                               "IDL:test:1.0");

        test_var test =
          test::_narrow (object.in ());

        test->method ();

        child_poa->deactivate_object (id.in ());
      }

      {
        // Create a reference with user created ID in child POA which
        // uses the Servant_Activator.
        PortableServer::ObjectId_var id =
          PortableServer::string_to_ObjectId ("with reference counting");

        object =
          child_poa->create_reference_with_id (id.in (),
                                               "IDL:test:1.0");

        test_var test =
          test::_narrow (object.in ());

        test->method ();

        child_poa->deactivate_object (id.in ());
      }

      {
        // Create a reference with user created ID in child POA which
        // uses the Servant_Activator.
        PortableServer::ObjectId_var id =
          PortableServer::string_to_ObjectId ("no call made");

        object =
          child_poa->create_reference_with_id (id.in (),
                                               "IDL:test:1.0");

        child_poa->deactivate_object (id.in ());
      }

      {
        // Create a reference with user created ID in child POA which
        // uses the Servant_Activator but just don't call the reference at all
        PortableServer::ObjectId_var id =
          PortableServer::string_to_ObjectId ("no call");

        object =
          child_poa->create_reference_with_id (id.in (),
                                               "IDL:test:1.0");

        PortableServer::ObjectId_var oid =
          child_poa->reference_to_id (object.in ());

        child_poa->deactivate_object (oid.in ());
      }

      ACE_DEBUG ((LM_DEBUG,
                  "\nEnd of main()\n\n"));

      orb->destroy ();
    }
  catch (const CORBA::Exception& ex)
    {
      ex._tao_print_exception ("Exception caught");
      return -1;
    }

  return 0;
}
