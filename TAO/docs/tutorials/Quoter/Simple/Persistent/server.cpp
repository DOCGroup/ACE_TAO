
//=============================================================================
/**
 *  @file     server.cpp
 *
 *   In this example, a new POA (childPOA) is created and its
 *   policy is set so that it object references are persistent.
 *
 *  @author  Priyanka Gontla <pgontla@ece.uci.edu>
 */
//=============================================================================

#include "Stock_Factory_i.h"
#include "tao/Utils/PolicyList_Destroyer.h"
#include "ace/Log_Msg.h"

#include <iostream>

int ACE_TMAIN (int argc, ACE_TCHAR *argv[])
{
  try
    {
      CORBA::ORB_var orb (CORBA::ORB_init (argc, argv));
      CORBA::Object_var root_object (orb->resolve_initial_references ("RootPOA"));
      PortableServer::POA_var root_poa (
        PortableServer::POA::_narrow (root_object.in ()));
      PortableServer::POAManager_var poa_manager (root_poa->the_POAManager ());
      poa_manager->activate ();

      TAO::Utils::PolicyList_Destroyer policies (2);
      policies.length (2);
      policies[0] =
        root_poa->create_id_assignment_policy (PortableServer::USER_ID);
      policies[1] =
        root_poa->create_lifespan_policy (PortableServer::PERSISTENT);
      PortableServer::POA_var child_poa (
        root_poa->create_POA ("childPOA", poa_manager.in (), policies));

      Quoter_Stock_Factory_i stock_factory_i;
      PortableServer::ObjectId_var oid (
        PortableServer::string_to_ObjectId ("Stock_Factory"));
      child_poa->activate_object_with_id (oid.in (), &stock_factory_i);

      CORBA::Object_var stock_factory (child_poa->id_to_reference (oid.in ()));
      CORBA::String_var const ior (orb->object_to_string (stock_factory.in ()));
      std::cout << ior.in () << std::endl;

      orb->run ();
      root_poa->destroy (true, true);
      orb->destroy ();
    }
  catch (CORBA::Exception const &)
    {
      ACE_ERROR ((LM_ERROR, "CORBA exception raised!\n"));
      return 1;
    }
  return 0;
}
