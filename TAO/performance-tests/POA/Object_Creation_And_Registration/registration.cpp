
//=============================================================================
/**
 *  @file    registration.cpp
 *
 *  This test is used to measure the time it takes to register and
 *  activate an object in the POA.
 *
 *  @author Irfan Pyarali
 */
//=============================================================================


#include "testS.h"
#include "tao/Server_Strategy_Factory.h"
#include "tao/ORB_Core.h"
#include "ace/Profile_Timer.h"
#include "ace/Get_Opt.h"
#include "ace/Truncate.h"
#include <cstdio>
#include <memory>
#include <vector>

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

struct Registration_Options
{
  bool measure_reverse_map_effectiveness {};
  u_long iterations {1000};
};

static int
parse_args (int argc, ACE_TCHAR **argv, Registration_Options &options)
{
  ACE_Get_Opt get_opts (argc, argv, ACE_TEXT("i:r"));
  int c;

  while ((c = get_opts ()) != -1)
    switch (c)
      {
      case 'i':
        options.iterations = static_cast<u_long> (ACE_OS::atoi (get_opts.opt_arg ()));
        break;

      case 'r':
        options.measure_reverse_map_effectiveness = true;
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
print_stats (ACE_Profile_Timer::ACE_Elapsed_Time &elapsed_time,
             u_long iterations)
{
  if (iterations > 0)
    {
      elapsed_time.real_time *= ACE_ONE_SECOND_IN_MSECS;
      elapsed_time.user_time *= ACE_ONE_SECOND_IN_MSECS;
      elapsed_time.system_time *= ACE_ONE_SECOND_IN_MSECS;

      elapsed_time.real_time /= iterations;
      elapsed_time.user_time /= iterations;
      elapsed_time.system_time /= iterations;

      double const tmp = 1000 / elapsed_time.real_time;

      ACE_DEBUG ((LM_DEBUG,
                  "\titerations\t = %B,\n"
                  "\treal_time\t = %0.06f ms,\n"
                  "\tuser_time\t = %0.06f ms,\n"
                  "\tsystem_time\t = %0.06f ms,\n"
                  "\t%0.00f calls/second\n",
                  static_cast<size_t> (iterations),
                  elapsed_time.real_time   < 0.0 ? 0.0 : elapsed_time.real_time,
                  elapsed_time.user_time   < 0.0 ? 0.0 : elapsed_time.user_time,
                  elapsed_time.system_time < 0.0 ? 0.0 : elapsed_time.system_time,
                  tmp < 0.0 ? 0.0 : tmp));
    }
  else
    ACE_ERROR ((LM_ERROR,
                "\tNo time stats printed.  Zero iterations or error occurred.\n"));
}


//
// Measures how well the reverse hash map works.
//
void
reverse_map_effectiveness (test_i const *servants,
                           u_long iterations)
{
  // Size of the active object map
  TAO_Server_Strategy_Factory::Active_Object_Map_Creation_Parameters const& parameters =
    TAO_ORB_Core_instance ()->server_factory ()->active_object_map_creation_parameters ();

  u_long const active_object_map_size = parameters.active_object_map_size_;

  // Hash counters
  std::vector<u_long> hash_counter (active_object_map_size);

  // Calculate the effectiveness of the hash.
  for (u_long i = 0; i < iterations; ++i)
    {
      u_long const hash_index =
        ACE_Utils::truncate_cast<u_long> (
          reinterpret_cast<intptr_t> (&servants[i])) % active_object_map_size;
      hash_counter[hash_index]++;
    }

  for (u_long i = 0; i < active_object_map_size; ++i)
    {
      if (((i) % 10) == 0)
        {
          ACE_DEBUG ((LM_DEBUG, "\n"));
        }
      ACE_DEBUG ((LM_DEBUG, "%B = %B, ", static_cast<size_t> (i),
                  static_cast<size_t> (hash_counter[i])));
    }
  ACE_DEBUG ((LM_DEBUG, "\n\n"));
}

class stats
{
public:
  stats (bool quantify,
         char const *test_name,
         u_long iterations)
    : quantify_ (quantify),
      test_name_ (test_name),
      iterations_ (iterations)
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

  stats (stats const&) = delete;
  stats& operator= (stats const&) = delete;
  stats (stats&&) = delete;
  stats& operator= (stats&&) = delete;

  ~stats ()
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
    print_stats (elapsed_time,
                 this->iterations_);
  }

private:
  // Profile timer
  ACE_Profile_Timer timer_;
  bool const quantify_;
  char const * const test_name_;
  u_long const iterations_;
};

void
child_poa_testing (PortableServer::POA_ptr root_poa,
                   Registration_Options const& options)
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

  // Create arrays of servants, references, and object ids.
  std::unique_ptr<test_i[]> servants = std::make_unique<test_i[]> (options.iterations);
  std::vector<CORBA::Object_var> objects (options.iterations);
  std::vector<PortableServer::ObjectId_var> object_ids (options.iterations);

  // Buffer for the id string.
  char id_buffer[128] {};

  {
    // Record and quantify stats.
    stats s (QUANTIFY_CREATE_REFERENCE_WITH_ID,
             "create_reference_with_id",
             options.iterations);
    ACE_UNUSED_ARG (s);

    for (u_long i = 0; i < options.iterations; ++i)
      {
        std::snprintf (id_buffer, sizeof (id_buffer), "%lu", i);

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
             "activate_object_with_id",
             options.iterations);
    ACE_UNUSED_ARG (s);

    for (u_long i = 0; i < options.iterations; ++i)
      {
        child_poa->activate_object_with_id (object_ids[i].in (),
                                            &servants[i]);
      }
  }

  {
    // Record and quantify stats.
    stats s (QUANTIFY_DEACTIVATE_OBJECT,
             "deactivate_object",
             options.iterations);
    ACE_UNUSED_ARG (s);

    for (u_long i = 0; i < options.iterations; ++i)
      {
        child_poa->deactivate_object (object_ids[i].in ());
      }
  }
}

int
ACE_TMAIN (int argc, ACE_TCHAR *argv[])
{
  try
    {
      // Initialize the ORB first.
      CORBA::ORB_var orb = CORBA::ORB_init (argc, argv);

      Registration_Options options;
      int const result = parse_args (argc, argv, options);
      if (result != 0)
        return result;

      // Obtain the RootPOA.
      CORBA::Object_var obj = orb->resolve_initial_references ("RootPOA");

      // Get the POA_var object from Object_var.
      PortableServer::POA_var root_poa =
        PortableServer::POA::_narrow (obj.in ());

      // Create arrays of servants, references, and object ids.
      std::unique_ptr<test_i[]> servants = std::make_unique<test_i[]> (options.iterations);
      std::vector<CORBA::Object_var> objects (options.iterations);
      std::vector<PortableServer::ObjectId_var> object_ids (options.iterations);

      if (options.measure_reverse_map_effectiveness)
        {
          reverse_map_effectiveness (servants.get (), options.iterations);
        }

      {
        stats s (QUANTIFY_UNDERBAR_THIS,
                 "_this",
                 options.iterations);
        ACE_UNUSED_ARG (s);

        for (u_long i = 0; i < options.iterations; ++i)
          {
            PortableServer::ObjectId_var id =
              root_poa->activate_object (&servants[i]);
            CORBA::Object_var object = root_poa->id_to_reference (id.in ());
            objects[i] = test::_narrow (object.in ());
          }
      }

      {
        stats s (QUANTIFY_SERVANT_TO_ID,
                 "servant_to_id",
                 options.iterations);
        ACE_UNUSED_ARG (s);

        for (u_long i = 0; i < options.iterations; ++i)
          {
            object_ids[i] = root_poa->servant_to_id (&servants[i]);
          }
      }

      // Create the child POA.
      child_poa_testing (root_poa.in (), options);

      // Destroy RootPOA.
      root_poa->destroy (true, true);
    }
  catch (const CORBA::Exception& ex)
    {
      ex._tao_print_exception ("Exception caught");
      return -1;
    }

  return 0;
}
