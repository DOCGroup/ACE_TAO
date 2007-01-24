// -*- C++ -*-

#include "Server_ORBInitializer.h"

#include "tao/ORBInitializer_Registry.h"
#include "testC.h"

#include "tao/PortableServer/PortableServer.h"
#include "ace/Log_Msg.h"

ACE_RCSID (PolicyFactory,
           server,
           "$Id$")

int
main (int argc, char *argv[])
{
  ACE_DECLARE_NEW_CORBA_ENV;
  ACE_TRY
    {
      PortableInterceptor::ORBInitializer_ptr temp_initializer =
        PortableInterceptor::ORBInitializer::_nil ();

      ACE_NEW_RETURN (temp_initializer,
                      Server_ORBInitializer,
                      -1);  // No CORBA exceptions yet!
      PortableInterceptor::ORBInitializer_var orb_initializer =
        temp_initializer;

      PortableInterceptor::register_orb_initializer (orb_initializer.in ()
                                                     ACE_ENV_ARG_PARAMETER);

      CORBA::ORB_var orb = CORBA::ORB_init (argc,
                                            argv,
                                            "test_orb"
                                            ACE_ENV_ARG_PARAMETER);

      // Create the test policy.

      const CORBA::ULong val = 40442;
      CORBA::Any any;
      any <<= val;

      CORBA::Policy_var p (orb->create_policy (Test::POLICY_TYPE,
                                               any
                                               ACE_ENV_ARG_PARAMETER));

      const CORBA::PolicyType ptype =
        p->policy_type ();

      // Sanity check.
      if (ptype != Test::POLICY_TYPE)
        ACE_TRY_THROW (CORBA::INTERNAL ());

      Test::Policy_var policy (Test::Policy::_narrow (p.in ()
                                                      ACE_ENV_ARG_PARAMETER));

      const CORBA::ULong pval = policy->value ();

      // Sanity check.
      if (val != pval)
        ACE_TRY_THROW (CORBA::INTERNAL ());

      CORBA::Object_var obj =
        orb->resolve_initial_references ("RootPOA"
                                         ACE_ENV_ARG_PARAMETER);

      PortableServer::POA_var root_poa =
        PortableServer::POA::_narrow (obj.in ()
                                      ACE_ENV_ARG_PARAMETER);

      if (CORBA::is_nil (root_poa.in ()))
        ACE_ERROR_RETURN ((LM_ERROR,
                           "Unable to obtain RootPOA reference.\n"),
                          -1);

      PortableServer::POAManager_var poa_manager =
        root_poa->the_POAManager ();

      CORBA::PolicyList policies (1);
      policies.length (1);

      policies[0] = policy->copy ();

      PortableServer::POA_var poa =
        root_poa->create_POA ("Test POA",
                              poa_manager.in (),
                              policies
                              ACE_ENV_ARG_PARAMETER);

      policy->destroy ();

      poa_manager->activate ();

      root_poa->destroy (1, 1 ACE_ENV_ARG_PARAMETER);

      orb->destroy ();

      ACE_DEBUG ((LM_INFO,
                  "\n"
                  "PolicyFactory test succeeded.\n"));
    }
  ACE_CATCHANY
    {
      ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION,
                           "PolicyFactory test:");

      return -1;
    }
  ACE_ENDTRY;

  return 0;
}
