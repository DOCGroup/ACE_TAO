// -*- C++ -*-

// $Id$

#include "ace/Get_Opt.h"

int
main (int argc, char *argv[])
{
  ACE_DECLARE_NEW_CORBA_ENV;
  ACE_TRY
    {
      // Initialize ORB.
      CORBA::ORB_var orb =
        CORBA::ORB_init (argc, argv, "", ACE_TRY_ENV);
      ACE_TRY_CHECK;

      const char *balancer_ior = "file://test.ior";

      // Parse the application options after the ORB has been
      // initialized.
      ACE_Get_Opt options (argc, argv, "k:");
      int c = 0;

      while ((c = get_opts ()) != -1)
        switch (c)
          {
          case 'k':
            balancer_ior = options.optarg;
            break;
          default:
            ACE_ERROR_RETURN ((LM_ERROR,
                               "Usage: %s -k <Load Balancer IOR>\n",
                               argv[0]),
                              -1);
          }

      CORBA::Object_var poa_object =
        orb->resolve_initial_references ("RootPOA");
      if (CORBA::is_nil (poa_object.in ()))
        ACE_ERROR_RETURN ((LM_ERROR,
                           " (%P|%t) Unable to initialize the POA.\n"),
                          1);

      PortableServer::POA_var root_poa =
        PortableServer::POA::_narrow (poa_object.in (), ACE_TRY_ENV);
      ACE_TRY_CHECK;

      PortableServer::POAManager_var poa_manager =
        root_poa->the_POAManager (ACE_TRY_ENV);
      ACE_TRY_CHECK;

      poa_manager->activate (ACE_TRY_ENV);
      ACE_TRY_CHECK;

      CORBA::Object_var obj =
        orb->string_to_object (balancer_ior);

      LoadBalancing::LoadBalancer_ptr load_balancer =
        LoadBalancing::LoadBalancer::_unchecked_narrow (obj.in (),
                                                        ACE_TRY_ENV);
      ACE_TRY_CHECK;

      Hash_ReplicaControl control (load_balancer);

      orb->run (ACE_TRY_ENV);
      ACE_TRY_CHECK;

      poa->destroy (1, 1, ACE_TRY_ENV);
      ACE_TRY_CHECK;

      orb->destroy (ACE_TRY_ENV);
      ACE_TRY_CHECK;
    }
  ACE_CATCHANY
    {
      ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION, "SYS_EX");
    }
  ACE_ENDTRY;

  return 0;
}
