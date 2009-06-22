// $Id$

#include "tao/ORB_Core.h"
#include "tao/RTCORBA/RTCORBA.h"
#include "tao/RTCORBA/Priority_Mapping_Manager.h"
#include "ace/Get_Opt.h"

int
ACE_TMAIN (int argc, ACE_TCHAR *argv[])
{
  int result = 0;

   ACE_DEBUG ((LM_DEBUG, "Testing : "));

  for (int arg_i = 0; arg_i < argc; ++arg_i)
    {
      ACE_DEBUG ((LM_DEBUG, "%s ", argv[arg_i]));
    }

  ACE_DEBUG ((LM_DEBUG, "\n\n"));

  try
    {
      CORBA::ORB_var orb =
        CORBA::ORB_init (argc, argv);

      CORBA::Object_var object =
        orb->resolve_initial_references ("RTCurrent");

      RTCORBA::Current_var current =
        RTCORBA::Current::_narrow (object.in ());

      object = orb->resolve_initial_references ("PriorityMappingManager");

      RTCORBA::PriorityMappingManager_var mapping_manager =
        RTCORBA::PriorityMappingManager::_narrow (object.in ());

      RTCORBA::PriorityMapping *pm =
        mapping_manager->mapping ();

      ACE_DEBUG ((LM_DEBUG, "Testing part #1\n\n"));

      // Check for this priority mapping that we are kosher
      RTCORBA::Priority i = RTCORBA::minPriority;
      do
        {
          CORBA::Short native1 = -1, corba1 = -1, native2 = -1, corba2 = -1;

          if (pm->to_native (i, native1))
            {
              if (! (pm->to_CORBA (native1, corba1) &&
                    pm->to_native (corba1, native2) &&
                    pm->to_CORBA (native2, corba2)))
                {
                  ACE_DEBUG ((LM_DEBUG, "ERROR : Mapping reported false during CORBA %d -> Native %d -> CORBA %d -> Native %d\n",
                                                    i,
                                                    native1,
                                                    corba1,
                                                    native2
                                                    ));
                }
              else if (! (corba2 == corba1 && native1 == native2))
                {
                  // titsup
                  ACE_DEBUG ((LM_DEBUG, "ERROR - Not idem-thingy. Mapping went: CORBA %d -> Native %d -> CORBA %d -> Native %d - CORBA %d\n",
                                                    i,
                                                    native1,
                                                    corba1,
                                                    native2,
                                                    corba2
                                                    ));
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
          ACE_DEBUG ((LM_DEBUG, "ERROR: Unexpectedly was able to access thread CORBA priority of %d when it hadn't been set\n", i));
          result = -1;
        }
      catch (const CORBA::INITIALIZE &)
        {
          // This is what should happen
        }
      catch (const CORBA::Exception & ex)
        {
          ex._tao_print_exception ("ERROR: Unexpected exception type accessing thread CORBA priority when it hadn't been set: ");
          result = -1;
        }
      catch (...)
        {
          ACE_DEBUG ((LM_DEBUG, "ERROR: Wildly unexpected exception type accessing thread CORBA priority when it hadn't been set\n", i));
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
      catch (const CORBA::BAD_PARAM &)
        {
          // This is what should happen
        }
      catch (const CORBA::Exception & ex)
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
  catch (const CORBA::Exception & ex)
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
