#include "Tester_ComponentC.h"
#include "ace/Get_Opt.h"
#include <iostream>

// IOR file of the Tester_Component.
const char * ior = 0;
// SA netwokr file.
const char * san = 0;
// Task map file.
const char * tm = 0;

int
parse_args (int argc, char *argv[])
{
  ACE_Get_Opt get_opts (argc, argv, "k:s:t:");
  int c = 0;

  while ((c = get_opts ()) != -1)
    {
      switch (c)
        {
        case 'k':
          ior = get_opts.opt_arg ();
	        break;

        case 's':
          san = get_opts.opt_arg ();
	        break;

        case 't':
          tm = get_opts.opt_arg ();
	        break;

        case '?':  // display help for use of the server.
        default:
          ACE_ERROR_RETURN ((LM_ERROR,
                            "Usage:  %s"
                             "-k <Tester_Component IOR> (default is file://Tester_Component.ior)\n",
                             "-s <SA Network file name> (default is test.san)\n",
                             "-t <Task map file name> (default is test.tm)\n",
                            argv [0]),
                            -1);
          break;
        }
    }

  if (ior  == 0)
    {
      ior = "file://Tester_Component.ior";
   }
  if (san  == 0)
    {
      san = "C:/Middleware/CIAO/RACE/descriptors/test.san";
   }
  if (tm  == 0)
    {
      tm = "test.tm";
    }

  return 0;
}

int
main (int argc, char *argv[])
{
  try
    {
      // Initialize orb
      CORBA::ORB_var orb = CORBA::ORB_init (argc, argv,
                                            "");

      if (parse_args (argc, argv) != 0)
        {
          return -1;
        }

      CORBA::Object_var obj =
        orb->string_to_object (ior);

      ::CIAO::RACE::SA_POP::Testing_var test =
        ::CIAO::RACE::SA_POP::Testing::_narrow (obj.in ());

      if (CORBA::is_nil (test.in ()))
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "Unable to acquire Tester's objref\n"),
                            -1);
        }


      // Initialize the SA-POP planner.
      test->init (san, tm);


      unsigned int num_goals = 0;
      std::cout << "Number of goals: ";
      std::cin >> num_goals;
      std::cout << std::endl;

      // Populate the goal.
      ::CIAO::RACE::GoalStructure goal_map;
      for (unsigned int loop = 0; loop < num_goals; loop++) {
        // Get goal ID and utility.
        CIAO::RACE::GoalCondition condition;
        std::cout << "Goal condition ID: ";
        std::cin >> condition.condID;
        std::cout << std::endl;
        std::cout << "Goal utility: ";
        std::cin >> condition.utility;
        std::cout << std::endl;

        // Now we add it to the sequence.
        goal_map.goalConds.length(goal_map.goalConds.length() + 1);
        goal_map.goalConds[goal_map.goalConds.length() - 1] = condition;
      }

      ::CORBA::String_var ID;
      ::CORBA::Boolean dp_success = test->deploy_from_goal (goal_map, ID.out());

      if (dp_success)
        ACE_DEBUG ((LM_DEBUG, "Successfully deployed opstring from goal."));
      else
        ACE_DEBUG ((LM_DEBUG, "Deployment failed."));


      orb->destroy ();
    }
  catch (const CORBA::Exception& ex)
    {
      ex._tao_print_exception ("Unknown exception \n");
      return -1;
    }

  return 0;
}
