// $Id$

#include "LatencyTest.h"

#include "ace/Sched_Params.h"
#include "ace/OS_NS_errno.h"

//#include "tao/Strategies/advanced_resource.h"

const char *ior_output_file = "test.ior";

int
ACE_TMAIN(int argc, ACE_TCHAR *argv[])
{
  int priority =
    (ACE_Sched_Params::priority_min (ACE_SCHED_FIFO)
     + ACE_Sched_Params::priority_max (ACE_SCHED_FIFO)) / 2;

  priority = ACE_Sched_Params::next_priority (ACE_SCHED_FIFO,
                                              priority);
  // Enable FIFO scheduling, e.g., RT scheduling class on Solaris.

  if (ACE_OS::sched_params (ACE_Sched_Params (ACE_SCHED_FIFO,
                                              priority,
                                              ACE_SCOPE_PROCESS)) != 0)
    {
      if (ACE_OS::last_error () == EPERM)
        {
          ACE_DEBUG ((LM_DEBUG,
                      "server (%P|%t): user is not superuser, "
                      "test runs in time-shared class\n"));
        }
      else
        {
          ACE_ERROR ((LM_ERROR,
                      "server (%P|%t): sched_params failed\n"));
        }
    }

  try
    {
      CORBA::ORB_var orb =
        CORBA::ORB_init (argc, argv);

      CORBA::Object_var poa_object =
        orb->resolve_initial_references("RootPOA");

      if (CORBA::is_nil (poa_object.in ()))
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             " (%P|%t) Unable to initialize the POA.\n"),
                            1);
        }

      PortableServer::POA_var root_poa =
        PortableServer::POA::_narrow (poa_object.in ());

      PortableServer::POAManager_var poa_manager =
        root_poa->the_POAManager ();

      LatencyTest *roundtrip_impl;
      ACE_NEW_RETURN (roundtrip_impl,
                      LatencyTest (orb.in ()),
                      1);
      PortableServer::ServantBase_var owner_transfer(roundtrip_impl);

      //Get the RoundTrip reference
      Benchmark::LatencyTest_var roundtrip =
        roundtrip_impl->_this ();

      CORBA::String_var ior =
        orb->object_to_string (roundtrip.in ());

      // If the ior_output_file exists, output the ior to it
      ACE_DEBUG ((LM_DEBUG, "%s", ior.in ()));

      poa_manager->activate ();

      orb->run ();

      ACE_DEBUG ((LM_DEBUG, "(%P|%t) server - event loop finished\n"));

      root_poa->destroy (1, 1);

      orb->destroy ();
    }
  catch (const CORBA::Exception& ex)
    {
      ex._tao_print_exception ("Exception caught:");
      return 1;
    }

  return 0;
}
