#include "Server_ORBInitializer.h"
#include "tao/ORBInitializer_Registry.h"
#include "testC.h"
#include "tao/PortableServer/PortableServer.h"
#include "ace/Log_Msg.h"

int
ACE_TMAIN (int argc, ACE_TCHAR *argv[])
{
  try
    {
      PortableInterceptor::ORBInitializer_ptr temp_initializer {};

      ACE_NEW_RETURN (temp_initializer,
                      Server_ORBInitializer,
                      -1);  // No CORBA exceptions yet!
      PortableInterceptor::ORBInitializer_var const orb_initializer =
        temp_initializer;

      PortableInterceptor::register_orb_initializer (orb_initializer.in ());

      CORBA::ORB_var const orb = CORBA::ORB_init (argc,
                                                  argv,
                                                  "test_orb");

      // Create the test policy.

      CORBA::ULong const val (40442);
      CORBA::Any any {};
      any <<= val;

      CORBA::Policy_var const p (orb->create_policy (Test::POLICY_TYPE,
                                                     any));

      CORBA::PolicyType const ptype (p->policy_type ());

      // Sanity check.
      if (ptype != Test::POLICY_TYPE)
      {
        throw CORBA::INTERNAL ();
      }

      Test::Policy_var const policy (Test::Policy::_narrow (p.in ()));

      CORBA::ULong const pval (policy->value ());

      // Sanity check.
      if (val != pval)
      {
        throw CORBA::INTERNAL ();
      }

      CORBA::Object_var const obj =
        orb->resolve_initial_references ("RootPOA");

      PortableServer::POA_var const root_poa =
        PortableServer::POA::_narrow (obj.in ());

      if (CORBA::is_nil (root_poa.in ()))
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "Unable to obtain RootPOA reference.\n"),
                            -1);
        }

      PortableServer::POAManager_var const poa_manager =
        root_poa->the_POAManager ();

      CORBA::PolicyList policies (1);
      policies.length (1);

      policies[0] = policy->copy ();

      PortableServer::POA_var const poa =
        root_poa->create_POA ("Test POA",
                              poa_manager.in (),
                              policies);

      policy->destroy ();

      poa_manager->activate ();

      root_poa->destroy (true, true);

      orb->destroy ();

      ACE_DEBUG ((LM_INFO,
                  "\n"
                  "PolicyFactory test succeeded.\n"));
    }
  catch (CORBA::Exception const &ex)
    {
      ex._tao_print_exception ("PolicyFactory test:");

      return -1;
    }

  return 0;
}
