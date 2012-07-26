
//=============================================================================
/**
 *  @file    registration.cpp
 *
 *  $Id$
 *
 *
 *  This test is used to measure the time it takes to register and
 *  activate an object in the POA.
 *
 *
 *  @author Irfan Pyarali
 */
//=============================================================================


#include "testS.h"
#include "tao/Server_Strategy_Factory.h"
#include "tao/ORB_Core.h"
#include "ace/Profile_Timer.h"
#include "ace/Get_Opt.h"

//
// The following macros help take a very precise look into the
// different object creations and registration calls through
// Quantify. You'll have to define USING_QUANTIFY and define any of
// the other specific defines that you are interested in to be 1.
//
// Compilation: Add the directory where pure.h is (usually C:\Program
// Files\Rational\Quantify) to the list of include directories.  In
// addition, add pure_api.c to the source file compilation list.
//

//#define USING_QUANTIFY
#define QUANTIFY_DEACTIVATE_OBJECT 0
#define QUANTIFY_ACTIVATE_OBJECT_WITH_ID 0
#define QUANTIFY_CREATE_REFERENCE_WITH_ID 0
#define QUANTIFY_SERVANT_TO_ID 0
#define QUANTIFY_UNDERBAR_THIS 0

#if defined (USING_QUANTIFY)
#if defined (ACE_WIN32)
#include "pure.h"

#else

#include "quantify.h"
inline int QuantifyClearData ()
{
  return quantify_clear_data ();
}

inline int QuantifyStartRecordingData ()
{
  return quantify_start_recording_data ();
}

inline int QuantifyStopRecordingData ()
{
  return quantify_stop_recording_data ();
}

#endif /* ACE_WIN32 */
#endif /* USING_QUANTIFY */

/**
 * @class test_i
 *
 * @brief Oversimplified servant class
 */
class test_i : public POA_test
{
};

// Program statics
static int measure_reverse_map_effectiveness = 0;
static u_long iterations = 1000;

static int
parse_args (int argc, ACE_TCHAR **argv)
{
  ACE_Get_Opt get_opts (argc, argv, ACE_TEXT("i:r"));
  int c;

  while ((c = get_opts ()) != -1)
    switch (c)
      {
      case 'i':
        iterations = static_cast<u_long> (ACE_OS::atoi (get_opts.opt_arg ()));
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
                  "\titerations\t = %d,\n"
                  "\treal_time\t = %0.06f ms,\n"
                  "\tuser_time\t = %0.06f ms,\n"
                  "\tsystem_time\t = %0.06f ms,\n"
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
  u_long i = 0;

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

class stats
{
public:
  stats (int quantify,
         const char *test_name)
    : quantify_ (quantify),
      test_name_ (test_name)
  {
    this->timer_.start ();

    if (this->quantify_)
      {
#if defined (USING_QUANTIFY)
        // Reset Quantify data recording; whatever happened before this
        // point is not relevant anymore.
        QuantifyClearData ();
        QuantifyStartRecordingData ();
#endif /* USING_QUANTIFY */
      }
  }

  ~stats (void)
  {
    if (this->quantify_)
      {
#if defined (USING_QUANTIFY)
        // Stop recording data here; whatever happens after this is
        // not relevant.
        QuantifyStopRecordingData ();
#endif /* USING_QUANTIFY */
      }

    // stop the timer.
    this->timer_.stop ();

    ACE_Profile_Timer::ACE_Elapsed_Time elapsed_time;
    this->timer_.elapsed_time (elapsed_time);

    // compute average time.
    ACE_DEBUG ((LM_DEBUG, "\n%C stats...\n",
                this->test_name_));
    print_stats (elapsed_time);
  }

private:
  // Profile timer
  ACE_Profile_Timer timer_;
  int quantify_;
  const char *test_name_;
};

void
child_poa_testing (PortableServer::POA_ptr root_poa)
{
  // Policies for the child POA.
  CORBA::PolicyList policies (1);
  policies.length (1);

  // Id Assignment Policy
  policies[0] =
    root_poa->create_id_assignment_policy (PortableServer::USER_ID);

  // Create the child POA under the RootPOA.
  PortableServer::POA_var child_poa =
    root_poa->create_POA ("child POA",
                          PortableServer::POAManager::_nil (),
                          policies);

  // Create an array of servants
  test_i *servants =
    new test_i[iterations];

      // Create an array of objects
  CORBA::Object_var *objects =
    new CORBA::Object_var[iterations];

      // Create an array of object ids
  PortableServer::ObjectId_var *object_ids =
    new PortableServer::ObjectId_var[iterations];

  // Buffer for the id string.
  char id_buffer[128];

  // Index counter
  u_long i = 0;

  {
    // Record and quantify stats.
    stats s (QUANTIFY_CREATE_REFERENCE_WITH_ID,
             "create_reference_with_id");
    ACE_UNUSED_ARG (s);

    for (i = 0; i < iterations; i++)
      {
        ACE_OS::sprintf (id_buffer,
                         "%ld",
                         i);

        object_ids[i] =
          PortableServer::string_to_ObjectId (id_buffer);

        objects[i] =
          child_poa->create_reference_with_id (object_ids[i].in (),
                                               "IDL:test:1.0");
      }
  }

  {
    // Record and quantify stats.
    stats s (QUANTIFY_ACTIVATE_OBJECT_WITH_ID,
             "activate_object_with_id");
    ACE_UNUSED_ARG (s);

    for (i = 0; i < iterations; i++)
      {
        child_poa->activate_object_with_id (object_ids[i].in (),
                                            &servants[i]);
      }
  }

  {
    // Record and quantify stats.
    stats s (QUANTIFY_DEACTIVATE_OBJECT,
             "deactivate_object");
    ACE_UNUSED_ARG (s);

    for (i = 0; i < iterations; i++)
      {
        child_poa->deactivate_object (object_ids[i].in ());
      }
  }

  // Cleanup
  delete[] object_ids;
  delete[] objects;
  delete[] servants;
}

int
ACE_TMAIN (int argc, ACE_TCHAR *argv[])
{

  try
    {
      // Initialize the ORB first.
      CORBA::ORB_var orb = CORBA::ORB_init (argc, argv);

      int result = parse_args (argc, argv);
      if (result != 0)
        return result;

      // Obtain the RootPOA.
      CORBA::Object_var obj = orb->resolve_initial_references ("RootPOA");

      // Get the POA_var object from Object_var.
      PortableServer::POA_var root_poa =
        PortableServer::POA::_narrow (obj.in ());

      // Create an array of servants
      test_i *servants =
        new test_i[iterations];

      // Create an array of objects
      CORBA::Object_var *objects =
        new CORBA::Object_var[iterations];

      // Create an array of object ids
      PortableServer::ObjectId_var *object_ids =
        new PortableServer::ObjectId_var[iterations];

      if (measure_reverse_map_effectiveness)
        {
          reverse_map_effectiveness (servants);
        }

      // Index counter
      u_long i = 0;

      {
        stats s (QUANTIFY_UNDERBAR_THIS,
                 "_this");
        ACE_UNUSED_ARG (s);

        for (i = 0; i < iterations; i++)
          {
            PortableServer::ObjectId_var id =
              root_poa->activate_object (&servants[i]);
            CORBA::Object_var object = root_poa->id_to_reference (id.in ());
            objects[i] = test::_narrow (object.in ());
          }
      }

      {
        stats s (QUANTIFY_SERVANT_TO_ID,
                 "servant_to_id");
        ACE_UNUSED_ARG (s);

        for (i = 0; i < iterations; i++)
          {
            object_ids[i] = root_poa->servant_to_id (&servants[i]);
          }
      }

      // Create the child POA.
      child_poa_testing (root_poa.in ());

      // Destroy RootPOA.
      root_poa->destroy (1,
                         1);

      // Cleanup
      delete[] object_ids;
      delete[] objects;
      delete[] servants;
    }
  catch (const CORBA::Exception& ex)
    {
      ex._tao_print_exception ("Exception caught");
      return -1;
    }

  return 0;
}
