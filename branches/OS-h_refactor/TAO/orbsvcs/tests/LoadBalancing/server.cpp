// -*- C++ -*-

// $Id$

#include "orbsvcs/orbsvcs/LoadBalancingC.h"
#include "Hash_ReplicaControl.h"
#include "ace/Get_Opt.h"

int
main (int argc, char *argv[])
{
  ACE_DECLARE_NEW_CORBA_ENV;
  ACE_TRY
    {
      // Initialize ORB.
      CORBA::ORB_var orb =
        CORBA::ORB_init (argc, argv, "" ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      const char *balancer_ior = "file://test.ior";
      const char *ior_output = "server.ior";

      // Parse the application options after the ORB has been
      // initialized.
      ACE_Get_Opt options (argc, argv, "k:");
      int c = 0;

      while ((c = options ()) != -1)
        switch (c)
          {
          case 'k':
            balancer_ior = options.opt_arg ();
            break;

          case 'o':
            ior_output = options.opt_arg ();
            break;
          default:
            ACE_ERROR_RETURN ((LM_ERROR,
                               "Usage: %s -k <Load Balancer IOR>\n",
                               argv[0]),
                              -1);
          }

      CORBA::Object_var poa_object =
        orb->resolve_initial_references ("RootPOA" ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      if (CORBA::is_nil (poa_object.in ()))
        ACE_ERROR_RETURN ((LM_ERROR,
                           " (%P|%t) Unable to initialize the POA.\n"),
                          1);

      PortableServer::POA_var root_poa =
        PortableServer::POA::_narrow (poa_object.in () ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      PortableServer::POAManager_var poa_manager =
        root_poa->the_POAManager (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;

      poa_manager->activate (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;

      CORBA::Object_var obj =
        orb->string_to_object (balancer_ior ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      LoadBalancing::LoadBalancer_var load_balancer =
        LoadBalancing::LoadBalancer::_narrow (obj.in ()
                                              ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      if (CORBA::is_nil (load_balancer.in ()))
        ACE_ERROR_RETURN ((LM_ERROR,
                           ACE_TEXT ("Invalid Load Balancer IOR.\n")),
                          -1);

      CORBA::Object_var group =
        load_balancer->group_identity (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;

      CORBA::String_var str =
        orb->object_to_string (group.in () ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      FILE *ior = ACE_OS::fopen (ior_output, "w");
      ACE_OS::fprintf (ior, "%s", str.in ());
      ACE_OS::fclose (ior);

      Hash_ReplicaControl control;
      control.init (orb.in (),
                    load_balancer.in ()
                    ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

#ifndef ACE_WIN32
      orb->run (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;
#else
      while (1)
        {
          ACE_Time_Value tv (1, 0);
          orb->run (tv ACE_ENV_ARG_PARAMETER);
          ACE_TRY_CHECK;
          control.handle_timeout (tv, 0);
        }
#endif

      root_poa->destroy (1, 1 ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      orb->destroy (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;
    }
  ACE_CATCHANY
    {
      ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION, "SYS_EX");
    }
  ACE_ENDTRY;

  return 0;
}
