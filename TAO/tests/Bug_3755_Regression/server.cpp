#include "ace/Get_Opt.h"
#include "ace/ARGV.h"
#include "ace/OS_NS_stdio.h"
#include "tao/RTCORBA/RTCORBA.h"
#include "tao/RTCORBA/Priority_Mapping_Manager.h"

#include <memory>
#include <string>

int
parse_args (int argc,
            ACE_TCHAR *argv[],
            std::basic_string<ACE_TCHAR> &orb1_args,
            std::basic_string<ACE_TCHAR> &orb2_args,
            std::basic_string<ACE_TCHAR> &mapping_file1,
            std::basic_string<ACE_TCHAR> &mapping_file2)
{
  ACE_Get_Opt get_opts (argc, argv, ACE_TEXT ("a:b:y:z:"));
  int option {};

  while ((option = get_opts ()) != -1)
    switch (option)
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
                           ACE_TEXT ("-z <mapping_file_for_orb2>\n"),
                           argv [0]),
                          -1);
      }

  return 0;
}

int
write_priority_mapping (RTCORBA::PriorityMapping *mapping,
                        ACE_TCHAR const *mapping_file,
                        char const *mapping_name)
{
  std::unique_ptr<FILE, int (*) (FILE *)> const output_file (
    ACE_OS::fopen (mapping_file, "w"), ACE_OS::fclose);
  if (!output_file)
    ACE_ERROR_RETURN ((LM_ERROR,
                       ACE_TEXT ("Cannot open output file for mapping %C\n"),
                       mapping_name),
                      -1);

  ACE_DEBUG ((LM_DEBUG,
              ACE_TEXT ("Sending mapped priorities to %s...\n"),
              mapping_file));

  CORBA::Short rt_corba_priority {RTCORBA::minPriority - 1};
  while (rt_corba_priority < RTCORBA::maxPriority)
    {
      ++rt_corba_priority;

      CORBA::Short native_priority {};
      bool const status = mapping->to_native (rt_corba_priority, native_priority);

      if (!status)
        ACE_ERROR ((LM_ERROR,
                    ACE_TEXT ("Could not map rtCorba priority %d "
                              "to a native priority.\n"),
                    rt_corba_priority));
      else
        ACE_OS::fprintf (output_file.get (), "%d %d\n",
                         rt_corba_priority,
                         native_priority);
    }

  return 0;
}

int
ACE_TMAIN (int argc, ACE_TCHAR *argv[])
{
  try
    {
      std::basic_string<ACE_TCHAR> orb1_args {
        ACE_TEXT ("AAA -ORBGestalt LOCAL -ORBSvcConf MY_TEST_ORB_1.conf")};
      std::basic_string<ACE_TCHAR> orb2_args {
        ACE_TEXT ("BBB -ORBGestalt LOCAL -ORBSvcConf MY_TEST_ORB_2.conf")};
      std::basic_string<ACE_TCHAR> mapping_file1 {ACE_TEXT ("TAO_PRIORITY_MAPPING1.dat")};
      std::basic_string<ACE_TCHAR> mapping_file2 {ACE_TEXT ("TAO_PRIORITY_MAPPING2.dat")};

      if (parse_args (argc, argv, orb1_args, orb2_args, mapping_file1, mapping_file2) != 0)
        ACE_ERROR_RETURN ((LM_ERROR,
                           ACE_TEXT ("Cannot process args\n")),
                          1);

      ACE_ARGV orb1_argv {orb1_args.c_str ()};
      int orb1_argc = orb1_argv.argc ();
      ACE_ARGV orb2_argv {orb2_args.c_str ()};
      int orb2_argc = orb2_argv.argc ();

      ACE_DEBUG ((LM_DEBUG,
                  "Initialize ORB instances...\n"));

      CORBA::ORB_var const orb1 {
        CORBA::ORB_init (orb1_argc, orb1_argv.argv (), mapping_file1.c_str ())};
      CORBA::ORB_var const orb2 {
        CORBA::ORB_init (orb2_argc, orb2_argv.argv (), mapping_file2.c_str ())};

      ACE_DEBUG ((LM_DEBUG,
                  "After ORB_init...\n"));

      CORBA::Object_var const manager_object1 =
        orb1->resolve_initial_references ("PriorityMappingManager");
      RTCORBA::PriorityMappingManager_var const mapping_manager1 =
        RTCORBA::PriorityMappingManager::_narrow (manager_object1.in ());
      // This pointer is borrowed; the mapping manager owns the mapping.
      RTCORBA::PriorityMapping *const priority_mapping1 = mapping_manager1->mapping ();

      ACE_DEBUG ((LM_DEBUG,
                  "Priority Manager & mapping interfaces resolved.\n"));
      if (write_priority_mapping (priority_mapping1, mapping_file1.c_str (), "1") != 0)
        return -1;

      CORBA::Object_var const manager_object2 =
        orb2->resolve_initial_references ("PriorityMappingManager");
      RTCORBA::PriorityMappingManager_var const mapping_manager2 =
        RTCORBA::PriorityMappingManager::_narrow (manager_object2.in ());
      // This pointer is borrowed; the mapping manager owns the mapping.
      RTCORBA::PriorityMapping *const priority_mapping2 = mapping_manager2->mapping ();

      ACE_DEBUG ((LM_DEBUG,
                  "Priority Manager & mapping interfaces resolved.\n"));
      if (write_priority_mapping (priority_mapping2, mapping_file2.c_str (), "2") != 0)
        return -1;
    }
  catch (CORBA::SystemException const &ex)
    {
      ex._tao_print_exception ("Caught SystemException in main:\n");
      return 1;
    }

  return 0;
}
