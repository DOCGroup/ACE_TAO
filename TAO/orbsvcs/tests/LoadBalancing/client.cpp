// -*- C++ -*-

// $Id$

// TAO Load Balancer test client

#include "Hash_ReplicaC.h"
#include "ace/Get_Opt.h"
#include "ace/Stats.h"
#include "ace/High_Res_Timer.h"

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
            ior = options.optarg;
            break;

          case 't':
            timeout = ACE_OS::atoi (options.optarg);
            break;

          case 'i':
            iterations = ACE_OS::atoi (options.optarg);
            break;

          default:
            ACE_ERROR_RETURN ((LM_ERROR,
                               "Usage: %s "
                               "[-k ReplicaIOR] "
                               "[-t timeout (msecs)] "
                               "[-i iterations] "
                               "\n",
                               argv[0]),
                              -1);
          }

      CORBA::Object_var obj =
        orb->string_to_object (ior);

      Hash_Replica_var hasher =
        Hash_Replica::_unchecked_narrow (obj.in (),
                                         ACE_TRY_ENV);
      ACE_TRY_CHECK;

      ACE_Time_Value tv (0, timeout * 1000);
      ACE_Throughput_Stats stats;
      ACE_UINT64 test_start = ACE_OS::gethrtime ();

      for (long i = 0; i != iterations; ++i)
        {
          ACE_UINT64 call_start = ACE_OS::gethrtime ();
          hasher->do_hash (argv[0], ACE_TRY_ENV);
          ACE_TRY_CHECK;
          ACE_UINT64 end = ACE_OS::gethrtime ();

          stats.sample (end - test_start, end - call_start);
          ACE_OS::sleep (tv);
        }

      ACE_UINT32 gsf = ACE_High_Res_Timer::global_scale_factor ();
      stats.dump_results ("Hash Stats", gsf);

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
