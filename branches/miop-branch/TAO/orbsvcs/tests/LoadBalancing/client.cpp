// -*- C++ -*-

// $Id$

// TAO Load Balancer test client

#include "Hash_ReplicaC.h"
#include "ace/Get_Opt.h"
#include "ace/Stats.h"
#include "ace/High_Res_Timer.h"

static void run_test (int iterations,
                      int timeout,
                      Hash_Replica_ptr hasher
                      TAO_ENV_ARG_DECL);

int
main (int argc, char *argv[])
{
  TAO_ENV_DECLARE_NEW_ENV;
  ACE_TRY
    {
      // Initialize ORB.
      CORBA::ORB_var orb =
        CORBA::ORB_init (argc, argv, "" TAO_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      const char *ior = "file://test.ior";
      long timeout = 50; // timeout in msecs
      long iterations = 10000; // iterations

      // Parse the application options after the ORB has been
      // initialized.
      ACE_Get_Opt options (argc, argv, "k:t:i:");
      int c = 0;

      while ((c = options ()) != -1)
        switch (c)
          {
          case 'k':
            ior = options.opt_arg ();
            break;

          case 't':
            timeout = ACE_OS::atoi (options.opt_arg ());
            break;

          case 'i':
            iterations = ACE_OS::atoi (options.opt_arg ());
            break;

          default:
            ACE_ERROR_RETURN ((LM_ERROR,
                               ACE_TEXT ("Usage: %s ")
                               ACE_TEXT ("[-k ReplicaIOR] ")
                               ACE_TEXT ("[-t timeout (msecs)] ")
                               ACE_TEXT ("[-i iterations] ")
                               ACE_TEXT ("\n"),
                               argv[0]),
                              -1);
          }

      CORBA::Object_var obj =
        orb->string_to_object (ior TAO_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      Hash_Replica_var hasher =
        Hash_Replica::_unchecked_narrow (obj.in ()
                                         TAO_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      if (CORBA::is_nil (hasher.in ()))
        ACE_ERROR_RETURN ((LM_ERROR,
                           ACE_TEXT ("Invalid replica IOR.\n")),
                          -1);

      run_test (iterations, timeout, hasher.in () TAO_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      orb->destroy (TAO_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;
    }
  ACE_CATCHANY
    {
      ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION, "SYS_EX");
    }
  ACE_ENDTRY;

  return 0;
}

void
run_test (int iterations,
          int timeout,
          Hash_Replica_ptr hasher
          TAO_ENV_ARG_DECL)
{
  ACE_Time_Value tv (0, timeout * 1000);
  ACE_Throughput_Stats stats;
  ACE_UINT64 test_start = ACE_OS::gethrtime ();

  for (long i = 0; i != iterations; ++i)
    {
      ACE_UINT64 call_start = ACE_OS::gethrtime ();

      hasher->do_hash ("This is a silly test" TAO_ENV_ARG_PARAMETER);
      ACE_CHECK;
      ACE_UINT64 end = ACE_OS::gethrtime ();

      stats.sample (end - test_start, end - call_start);
      ACE_OS::sleep (tv);
    }

  ACE_UINT32 gsf = ACE_High_Res_Timer::global_scale_factor ();
  stats.dump_results ("Hash Stats", gsf);
}
