#include "tao/ORB_Core.h"
#include "tao/RTCORBA/RTCORBA.h"
#include "tao/RTCORBA/Multi_Priority_Mapping.h"
#include "tao/RTCORBA/Priority_Mapping_Manager.h"
#include "ace/Sched_Params.h"

static bool
test_non_contiguous_priority_mapping ()
{
  int const policy = ACE_SCHED_FIFO;
  int const base_native_priority = ACE_Sched_Params::priority_max (policy);
  int const base_corba_priority = RTCORBA::maxPriority;
  TAO_Multi_Priority_Mapping mapping (base_native_priority,
                                      base_corba_priority,
                                      1,
                                      false,
                                      policy);

  RTCORBA::NativePriority native_priority {-1};
  if (!mapping.to_native (base_corba_priority - 1, native_priority) ||
      native_priority != ACE_Sched_Params::previous_priority (policy,
                                                               base_native_priority,
                                                               ACE_SCOPE_THREAD))
    {
      ACE_ERROR ((LM_ERROR,
                  "ERROR: Non-contiguous mapping failed for one priority step\n"));
      return false;
    }

  int expected_native_priority = base_native_priority;
  for (int i {}; i < 3; ++i)
    {
      expected_native_priority =
        ACE_Sched_Params::previous_priority (policy,
                                             expected_native_priority,
                                             ACE_SCOPE_THREAD);
    }

  if (!mapping.to_native (base_corba_priority - 3, native_priority) ||
      native_priority != expected_native_priority)
    {
      ACE_ERROR ((LM_ERROR,
                  "ERROR: Non-contiguous mapping failed for three priority steps\n"));
      return false;
    }

  return true;
}

int
ACE_TMAIN (int argc, ACE_TCHAR *argv[])
{
  int result {};

  if (!test_non_contiguous_priority_mapping ())
    {
      result = -1;
    }

  ACE_DEBUG ((LM_DEBUG, "Testing : "));

  for (int arg_i = 0; arg_i < argc; ++arg_i)
    {
      ACE_DEBUG ((LM_DEBUG, "%s ", argv[arg_i]));
    }

  ACE_DEBUG ((LM_DEBUG, "\n\n"));

  try
    {
      CORBA::ORB_var const orb =
        CORBA::ORB_init (argc, argv);

      CORBA::Object_var object =
        orb->resolve_initial_references ("RTCurrent");

      RTCORBA::Current_var const current =
        RTCORBA::Current::_narrow (object.in ());

      object = orb->resolve_initial_references ("PriorityMappingManager");

      RTCORBA::PriorityMappingManager_var const mapping_manager =
        RTCORBA::PriorityMappingManager::_narrow (object.in ());

      RTCORBA::PriorityMapping * const pm =
        mapping_manager->mapping ();

      ACE_DEBUG ((LM_DEBUG, "Testing part #1\n\n"));

      // Check for this priority mapping that we are kosher
      RTCORBA::Priority i {RTCORBA::minPriority};
      do
        {
          CORBA::Short native1 {-1};
          CORBA::Short corba1 {-1};
          CORBA::Short native2 {-1};
          CORBA::Short corba2 {-1};

          if (pm->to_native (i, native1))
            {
              if (! (pm->to_CORBA (native1, corba1) &&
                    pm->to_native (corba1, native2) &&
                    pm->to_CORBA (native2, corba2)))
                {
                  ACE_DEBUG ((LM_DEBUG,
                              "ERROR : Mapping reported false during CORBA %d -> Native %d -> CORBA %d -> Native %d\n",
                              i,
                              native1,
                              corba1,
                              native2));
                }
              else if (! (corba2 == corba1 && native1 == native2))
                {
                  // titsup
                  ACE_DEBUG ((LM_DEBUG,
                              "ERROR - Not idem-thingy. Mapping went: CORBA %d -> Native %d -> CORBA %d -> "
                              "Native %d -> CORBA %d\n",
                              i,
                              native1,
                              corba1,
                              native2,
                              corba2));
                  result = -1;
                }
              }
          i = i + 1; // There's a reason to not use unary increment
        }
      while (i != RTCORBA::minPriority);

      ACE_DEBUG ((LM_DEBUG, "\n\nTesting part #2\n\n"));

      // Accessing the priority when it hasn't been set should throw an exception
      try
        {
          i = current->the_priority ();
          ACE_DEBUG ((LM_DEBUG,
                      "ERROR: Unexpectedly was able to access thread CORBA priority of %d when it hadn't been set\n",
                      i));
          result = -1;
        }
      catch (CORBA::INITIALIZE const &)
        {
          // This is what should happen
        }
      catch (CORBA::Exception const &ex)
        {
          ex._tao_print_exception (
            "ERROR: Unexpected exception type accessing thread CORBA priority when it hadn't been set: ");
          result = -1;
        }
      catch (...)
        {
          ACE_DEBUG ((LM_DEBUG,
                      "ERROR: Wildly unexpected exception type accessing thread CORBA priority "
                      "when it hadn't been set\n"));
          result = -1;
        }

      ACE_DEBUG ((LM_DEBUG, "\nTesting part #3\n\n"));

      // Setting an invalid CORBA priority should generate a BAD_PARAM if the mapping has rejected it
      try
        {
          current->the_priority (-1);
          ACE_DEBUG ((LM_DEBUG, "ERROR: Unexpectedly was able to set a thread CORBA priority\n"));
          result = -1;
        }
      catch (CORBA::BAD_PARAM const &)
        {
          // This is what should happen
        }
      catch (CORBA::Exception const &ex)
        {
          ex._tao_print_exception ("ERROR: Unexpected exception type setting an invalid CORBA priority");
          result = -1;
        }
      catch (...)
        {
          ACE_DEBUG ((LM_DEBUG, "ERROR: Wildly unexpected exception setting an invalid CORBA priority\n"));
          result = -1;
        }
    }
  catch (CORBA::Exception const &ex)
    {
      ex._tao_print_exception ("ERROR: Unexpected exception ");
      result = -1;
    }
  catch (...)
    {
      ACE_DEBUG ((LM_DEBUG, "ERROR: Unknown exception\n"));
      result = -1;
    }

  return result;
}
