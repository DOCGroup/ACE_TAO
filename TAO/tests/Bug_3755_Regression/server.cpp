// $Id$

#include "ace/Get_Opt.h"
#include "ace/ARGV.h"
#include "tao/RTCORBA/RTCORBA.h"
#include "tao/RTCORBA/Priority_Mapping_Manager.h"

ACE_TCHAR const *orb1_args =
  ACE_TEXT ("AAA -ORBGestalt LOCAL -ORBSvcConf MY_TEST_ORB_1.conf");
ACE_TCHAR const *orb2_args =
  ACE_TEXT ("BBB -ORBGestalt LOCAL -ORBSvcConf MY_TEST_ORB_2.conf");
ACE_TCHAR const *mapping_file1 = ACE_TEXT ("TAO_PRIORITY_MAPPING1.dat");
ACE_TCHAR const *mapping_file2 = ACE_TEXT ("TAO_PRIORITY_MAPPING2.dat");

int
parse_args (int argc, ACE_TCHAR *argv[])
{
  ACE_Get_Opt get_opts (argc, argv, ACE_TEXT ("a:b:y:z:"));
  int c;

  while ((c = get_opts ()) != -1)
    switch (c)
      {
      case 'a':
        orb1_args = get_opts.opt_arg ();
        break;
      case 'b':
        orb2_args = get_opts.opt_arg ();
        break;
      case 'y':
        mapping_file1 = get_opts.opt_arg ();
        break;
      case 'z':
        mapping_file2 = get_opts.opt_arg ();
        break;
      case '?':
      default:
        ACE_ERROR_RETURN ((LM_ERROR,
                           ACE_TEXT ("usage:  %s ")
                           ACE_TEXT ("-a <orb1_args> ")
                           ACE_TEXT ("-b <orb2_args> ")
                           ACE_TEXT ("-y <mapping_file_for_orb1> ")
                           ACE_TEXT ("-z <mapping_file_for_orb2>")
                           ACE_TEXT ("\n"),
                           argv [0]),
                          -1);
      }
  // Indicates successful parsing of the command line
  return 0;
}

int
ACE_TMAIN (int argc, ACE_TCHAR *argv[])
{
  try
    {
      if (parse_args (argc, argv) != 0)
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "Cannot process args\n"),
                            1);
        }

      ACE_ARGV orb1_argv (orb1_args);
      int orb1_argc = orb1_argv.argc ();

      ACE_ARGV orb2_argv (orb2_args);
      int orb2_argc = orb2_argv.argc ();

      ACE_DEBUG ((LM_DEBUG,
                  "Initialize ORB instances...\n"));

      CORBA::ORB_var orb1 =
        CORBA::ORB_init (orb1_argc, orb1_argv.argv (), mapping_file1);

      CORBA::ORB_var orb2 =
        CORBA::ORB_init (orb2_argc, orb2_argv.argv (), mapping_file2);

      ACE_DEBUG ((LM_DEBUG,
                  "After ORB_init...\n"));


      // Demonstrate the priority mapping  ====================================

      CORBA::Object_var pmMgrObj1 =
        orb1->resolve_initial_references ("PriorityMappingManager");

      RTCORBA::PriorityMappingManager_var mappingMgr1 =
        RTCORBA::PriorityMappingManager::_narrow (pmMgrObj1.in ());

      RTCORBA::PriorityMapping * priorityMapping1 = mappingMgr1->mapping ();

      ACE_DEBUG ((LM_DEBUG,
                  "Priority Manager & mapping interfaces resolved.\n"));

      // Write the mapping to a file
      FILE *output_file = 0;
      if (mapping_file1 != 0)
        {
          output_file = ACE_OS::fopen (mapping_file1, "w");

          if (output_file == 0)
            {
              ACE_ERROR ((LM_ERROR,
                          "Cannot open output file for mapping 1\n"));
              return -1;
            }
        }

      short rtCorbaPriority1 = RTCORBA::minPriority - 1;
      short nativePriority1 = 0;
      bool status1 = false;

      ACE_DEBUG ((LM_DEBUG,
                  "Sending mapped priorities to %s...\n",
                  mapping_file1));

      while (rtCorbaPriority1 < RTCORBA::maxPriority)
        {
          ++rtCorbaPriority1;

          status1 = priorityMapping1->to_native (rtCorbaPriority1,
                                                 nativePriority1);

          if (!status1)
            {
              ACE_ERROR ((LM_ERROR,
                          "Could not map rtCorba priority %d "
                          "to a native priority.\n",
                          rtCorbaPriority1));
            }
          else
            {
              ACE_OS::fprintf (output_file, "%d %d\n",
                               rtCorbaPriority1,
                               nativePriority1);
            }
        }

      ACE_OS::fclose (output_file);

      CORBA::Object_var pmMgrObj2 =
        orb2->resolve_initial_references ("PriorityMappingManager");

      RTCORBA::PriorityMappingManager_var mappingMgr2 =
        RTCORBA::PriorityMappingManager::_narrow (pmMgrObj2.in ());

      RTCORBA::PriorityMapping * priorityMapping2 = mappingMgr2->mapping ();

      ACE_DEBUG ((LM_DEBUG,
                  "Priority Manager & mapping interfaces resolved.\n"));

      // Write the mapping to a file
      output_file = 0;
      if (mapping_file2 != 0)
        {
          output_file = ACE_OS::fopen (mapping_file2, "w");

          if (output_file == 0)
            {
              ACE_ERROR ((LM_ERROR,
                          "Cannot open output file for mapping 1\n"));
              return -1;
            }
        }

      short rtCorbaPriority2 = RTCORBA::minPriority - 1;
      short nativePriority2 = 0;
      bool status2 = false;

      ACE_DEBUG ((LM_DEBUG,
                  "Sending mapped priorities to %s...\n",
                  mapping_file2));

      while (rtCorbaPriority2 < RTCORBA::maxPriority)
        {
          ++rtCorbaPriority2;

          status2 = priorityMapping2->to_native (rtCorbaPriority2,
                                                 nativePriority2);

          if (!status2)
            {
              ACE_ERROR ((LM_ERROR,
                          "Could not map rtCorba priority %d "
                          "to a native priority.\n",
                          rtCorbaPriority2));
            }
          else
            {
              ACE_OS::fprintf (output_file, "%d %d\n",
                               rtCorbaPriority2,
                               nativePriority2);
            }
        }

      ACE_OS::fclose (output_file);

      // END priority mapping demonstration
    }
  catch (CORBA::SystemException const &sysEx)
    {
      sysEx._tao_print_exception ("Caught SystemException in main:\n");
      return 1;
    }

  return 0;
}
