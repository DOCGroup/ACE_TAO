// -*- C++ -*-

// $Id$

// TAO Load Balancer test client

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
      long timeout = 50000; // timeout in msecs
      long iterations = 10000; // iterations

      // Parse the application options after the ORB has been
      // initialized.
      ACE_Get_Opt options (argc, argv, "k:t:");
      int c = 0;

      while ((c = get_opts ()) != -1)
        switch (c)
          {
          case 'k':
            ior = options.optarg;
            break;

          case 't':
            timeout = ACE_OS::atoi (options.optarg);
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
        Has_Replica::_unchecked_narrow (obj.in (),
                                        ACE_TRY_ENV);
      ACE_TRY_CHECK;

      ACE_Throughput_Stats stats;
      ACE_UINT64 test_start = ACE_OS::gethrtime ();

      for (long i = 0; i != iterations; ++i)
        {
          CORBA::String_var hash_result;

          ACE_UINT64 call_start = ACE_OS::gethrtime ();
          hasher->do_hash (argv[0], hash_result.out (),
                           ACE_TRY_ENV);
          ACE_TRY_CHECK;
          ACE_UINT64 end = ACE_OS::gethrtime ();

          stats.sample (end - test_start, end - call_start);
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
