// $Id$

//========================================================================
//
// = LIBRARY
//     TAO/tests/BiDir_Multiple_ORB
//
// = FILENAME
//     destroy.cpp
//
// = DESCRIPTION
//     Modified ORB destruction test.
//
// = AUTHOR
//     Andrew Schnable <Andrew.Schnable@veritas.com>
//     Iliyan Jeliazkov <jeliazkov_i@ociweb.com>
//
//=========================================================================

#include "tao/corba.h"
#include "tao/PortableServer/PortableServer.h"
#include "tao/AnyTypeCode/Any.h"
#include "tao/BiDir_GIOP/BiDirGIOP.h"



int
test_with_bidir_poa (int argc,
                     ACE_TCHAR **argv,
                     const char *orb_name,
                     int destroy_orb)
{

  try
    {
      CORBA::ORB_var orb =
        CORBA::ORB_init (argc, argv, orb_name);

      CORBA::Object_var obj =
        orb->resolve_initial_references ("RootPOA");

      PortableServer::POA_var root_poa =
        PortableServer::POA::_narrow (obj.in ());

      PortableServer::POAManager_var poa_manager =
        root_poa->the_POAManager ();


      CORBA::PolicyList policies (1);
      policies.length (1);

      CORBA::Any pol;
      pol <<= BiDirPolicy::BOTH;
      policies[0] =
        orb->create_policy (BiDirPolicy::BIDIRECTIONAL_POLICY_TYPE,
                            pol);

      // Create POA as child of RootPOA with the above policies.  This POA
      // will receive request in the same connection in which it sent
      // the request
      PortableServer::POA_var child_poa =
        root_poa->create_POA ("childPOA",
                              poa_manager.in (),
                              policies);

      // Creation of childPOA is over. Destroy the Policy objects.
      for (CORBA::ULong i = 0;
           i < policies.length ();
           ++i)
        {
          policies[i]->destroy ();
        }

      poa_manager->activate ();


      root_poa->destroy (1, 1);

      if (destroy_orb)
        {
          orb->destroy ();
        }
    }
  catch (const CORBA::Exception& ex)
    {
      ex._tao_print_exception ("Exception raised");
    }

  return 0;
}

int
ACE_TMAIN(int argc, ACE_TCHAR *argv[])
{
  int result = 0;

  for (int i=0; i<10; i++)
  {
    result = test_with_bidir_poa (argc, argv, "poa_1", 1);
    ACE_ASSERT (result == 0);

    result = test_with_bidir_poa (argc, argv, "poa_2", 1);
    ACE_ASSERT (result == 0);
  }
  ACE_DEBUG ((LM_DEBUG, "Completed OK\n"));
  return result;
}
