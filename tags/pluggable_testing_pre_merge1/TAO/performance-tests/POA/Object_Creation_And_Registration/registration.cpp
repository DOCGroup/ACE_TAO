// $Id$

// ================================================================
//
// = LIBRARY
//    TAO/performance-tests/POA/Object_Creation_And_Registration
//
// = FILENAME
//    registration.cpp
//
// = DESCRIPTION
//
//    This test is used to measure the time it takes to register and
//    activate an object in the POA.
//
// = AUTHOR
//    Irfan Pyarali
//
// ================================================================

#include "testS.h"
#include "ace/Profile_Timer.h"
#include "ace/Get_Opt.h"

//#define USING_PURIFY
//#define USING_PURIFY_FOR_SERVANT_LOOKUP
//#define USING_PURIFY_FOR_UNDERBAR_THIS

#if defined (USING_PURIFY)
#include "pure.h"
#endif /* USING_PURIFY */

class test_i : public POA_test
  // = TITLE
  //     Oversimplified servant class
{
};

// Prototype for stat printing function
void
print_stats (ACE_Profile_Timer::ACE_Elapsed_Time &elapsed_time);

void
reverse_map_effectiveness (test_i *servants);

static int measure_reverse_map_effectiveness = 0;

// Default iterations
static int iterations = 1000;

static int
parse_args (int argc, char **argv)
{
  ACE_Get_Opt get_opts (argc, argv, "i:r");
  int c;

  while ((c = get_opts ()) != -1)
    switch (c)
      {
      case 'i':
        iterations = ACE_OS::atoi (get_opts.optarg);
        break;

      case 'r':
        measure_reverse_map_effectiveness = 1;
        break;

      case '?':
      default:
        ACE_ERROR_RETURN ((LM_ERROR,
                           "usage:  %s "
                           "[-r [measure reverse map effectiveness]] "
                           "[-i iterations] "
                           "\n",
                           argv [0]),
                          -1);
      }

  // Indicates successful parsing of command line.
  return 0;
}

int
main (int argc, char **argv)
{
  ACE_DECLARE_NEW_CORBA_ENV;

  ACE_TRY
    {
      // Initialize the ORB first.
      CORBA::ORB_var orb = CORBA::ORB_init (argc,
                                            argv,
                                            0,
                                            ACE_TRY_ENV);
      ACE_TRY_CHECK;

      int result = parse_args (argc, argv);
      if (result != 0)
        return result;

      // Obtain the RootPOA.
      CORBA::Object_var obj = orb->resolve_initial_references ("RootPOA",
                                                               ACE_TRY_ENV);
      ACE_TRY_CHECK;

      // Get the POA_var object from Object_var.
      PortableServer::POA_var root_poa =
        PortableServer::POA::_narrow (obj.in (),
                                      ACE_TRY_ENV);
      ACE_TRY_CHECK;

      // Create an array of servants
      test_i *servants = new test_i[iterations];

      // Create an array of objects
      test_var *objects = new test_var[iterations];

      // Create an array of objects
      PortableServer::ObjectId_var *object_ids = new PortableServer::ObjectId_var[iterations];

      if (measure_reverse_map_effectiveness)
        {
          reverse_map_effectiveness (servants);
        }

      // Index counter
      int i = 0;

      {
        // Profile timer
        ACE_Profile_Timer timer;
        ACE_Profile_Timer::ACE_Elapsed_Time elapsed_time;

        // We start the profile timer here...
        timer.start ();

#if defined (USING_PURIFY_FOR_UNDERBAR_THIS)
        // Reset Quantify data recording; whatever happened in the
        // past is not relevant to this test.
        QuantifyClearData ();
        QuantifyStartRecordingData ();
#endif /* USING_PURIFY_FOR_UNDERBAR_THIS */

        for (i = 0; i < iterations; i++)
          {
            objects[i] = servants[i]._this (ACE_TRY_ENV);
            ACE_TRY_CHECK;
          }

#if defined (USING_PURIFY_FOR_UNDERBAR_THIS)
        // Stop recording data here; whatever happens after this in
        // the test is not relevant to this test.
        QuantifyStopRecordingData ();
#endif /* USING_PURIFY_FOR_UNDERBAR_THIS */

        // stop the timer.
        timer.stop ();
        timer.elapsed_time (elapsed_time);

        // compute average time.
        ACE_DEBUG ((LM_DEBUG, "\n_this stats...\n"));
        print_stats (elapsed_time);
      }

      {
        // Profile timer
        ACE_Profile_Timer timer;
        ACE_Profile_Timer::ACE_Elapsed_Time elapsed_time;

        // We start the profile timer here...
        timer.start ();

#if defined (USING_PURIFY_FOR_SERVANT_LOOKUP)
        // Reset Quantify data recording; whatever happened in the
        // past is not relevant to this test.
        QuantifyClearData ();
        QuantifyStartRecordingData ();
#endif /* USING_PURIFY_FOR_SERVANT_LOOKUP */

        for (i = 0; i < iterations; i++)
          {
            object_ids[i] = root_poa->servant_to_id (&servants[i],
                                                     ACE_TRY_ENV);
            ACE_TRY_CHECK;
          }

#if defined (USING_PURIFY_FOR_SERVANT_LOOKUP)
        // Stop recording data here; whatever happens after this in
        // the test is not relevant to this test.
        QuantifyStopRecordingData ();
#endif /* USING_PURIFY_FOR_SERVANT_LOOKUP */

        // stop the timer.
        timer.stop ();
        timer.elapsed_time (elapsed_time);

        // compute average time.
        ACE_DEBUG ((LM_DEBUG, "\nservant_to_id stats...\n"));
        print_stats (elapsed_time);
      }

      // Destroy RootPOA.
      root_poa->destroy (1,
                         1,
                         ACE_TRY_ENV);
      ACE_TRY_CHECK;

      // Cleanup
      delete[] objects;
      delete[] servants;
    }
  ACE_CATCHANY
    {
      ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION, "Exception caught");
      return -1;
    }
  ACE_ENDTRY;
  ACE_CHECK_RETURN (-1);

  return 0;
}

//
// Code for printing stats
//
void
print_stats (ACE_Profile_Timer::ACE_Elapsed_Time &elapsed_time)
{
  if (iterations > 0)
    {
      elapsed_time.real_time *= ACE_ONE_SECOND_IN_MSECS;
      elapsed_time.user_time *= ACE_ONE_SECOND_IN_MSECS;
      elapsed_time.system_time *= ACE_ONE_SECOND_IN_MSECS;

      elapsed_time.real_time /= iterations;
      elapsed_time.user_time /= iterations;
      elapsed_time.system_time /= iterations;

      double tmp = 1000 / elapsed_time.real_time;

      ACE_DEBUG ((LM_DEBUG,
		  "\titerations\t = %d, \n"
		  "\treal_time\t = %0.06f ms, \n"
		  "\tuser_time\t = %0.06f ms, \n"
		  "\tsystem_time\t = %0.06f ms, \n"
		  "\t%0.00f calls/second\n",
                  iterations,
		  elapsed_time.real_time   < 0.0 ? 0.0 : elapsed_time.real_time,
		  elapsed_time.user_time   < 0.0 ? 0.0 : elapsed_time.user_time,
		  elapsed_time.system_time < 0.0 ? 0.0 : elapsed_time.system_time,
		  tmp < 0.0 ? 0.0 : tmp));
    }
  else
    ACE_ERROR ((LM_ERROR,
		"\tNo time stats printed.  Zero iterations or error ocurred.\n"));
}


//
// Measures how well the reverse hash map works.
//
void
reverse_map_effectiveness (test_i *servants)
{
  // Size of the active object map
  const TAO_Server_Strategy_Factory::Active_Object_Map_Creation_Parameters &parameters =
    TAO_ORB_Core_instance ()->server_factory ()->active_object_map_creation_parameters ();

  u_long active_object_map_size = parameters.active_object_map_size_;

  // Hash counters
  u_long *hash_counter = new u_long[active_object_map_size];

  // Index counter
  int i = 0;

  // Initialize the hash counters
  for (i = 0; i < active_object_map_size; i++)
    {
      hash_counter[i] = 0;
    }

  // Calculate the effectiveness of the hash.
  for (i = 0; i < iterations; i++)
    {
      u_long hash_index = u_long (&servants[i]) % active_object_map_size;
      hash_counter[hash_index]++;
    }

  for (i = 0; i < active_object_map_size; i++)
    {
      if (((i) % 10) == 0)
        {
          ACE_DEBUG ((LM_DEBUG, "\n"));
        }
      ACE_DEBUG ((LM_DEBUG, "%d = %d, ", i, hash_counter[i]));
    }
  ACE_DEBUG ((LM_DEBUG, "\n\n"));

  delete[] hash_counter;
}
