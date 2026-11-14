#include "testC.h"
#include "tao/RTCORBA/RTCORBA.h"
#include "tao/RTCORBA/Network_Priority_Mapping_Manager.h"
#include "tao/RTCORBA/Network_Priority_Mapping.h"
#include "Custom_Network_Priority_Mapping.h"
#include "ace/Get_Opt.h"
#include "tao/Policy_ManagerC.h"
#include "tao/Policy_CurrentC.h"

#include <memory>

struct Client_Options
{
  ACE_TCHAR const *ior {ACE_TEXT("file://simple_servant.ior")};
  int iterations {1};
  RTCORBA::Priority corba_priority {RTCORBA::minPriority};
  bool shutdown_server {};
};

enum class Priority_Level
{
  OBJECT,
  THREAD,
  ORB
};

void
change_network_priority (bool const enable_network_priority,
                         Priority_Level const level,
                         Test_var &server,
                         CORBA::ORB_ptr orb)
{
  try
    {
      CORBA::Object_var object =
        orb->resolve_initial_references ("RTORB");

      RTCORBA::RTORB_var const rt_orb =
        RTCORBA::RTORB::_narrow (object.in ());

      // Set the tcp protocol properties
      RTCORBA::TCPProtocolProperties_var const tcp_properties =
        rt_orb->create_tcp_protocol_properties (ACE_DEFAULT_MAX_SOCKET_BUFSIZ,
                                                ACE_DEFAULT_MAX_SOCKET_BUFSIZ,
                                                1,
                                                0,
                                                1,
                                                enable_network_priority);

      RTCORBA::ProtocolList protocols;
      protocols.length (1);
      protocols[0].protocol_type = 0;
      protocols[0].transport_protocol_properties =
        RTCORBA::ProtocolProperties::_duplicate (tcp_properties.in ());
      protocols[0].orb_protocol_properties =
        RTCORBA::ProtocolProperties::_nil ();

      CORBA::PolicyList policy_list;
      policy_list.length (1);
      policy_list[0] =
        rt_orb->create_client_protocol_policy (protocols);

      switch (level)
        {
          // Change policy at ORB level
        case Priority_Level::ORB:
          {
            object = orb->resolve_initial_references ("ORBPolicyManager");

            CORBA::PolicyManager_var const policy_manager =
              CORBA::PolicyManager::_narrow (object.in ());

            policy_manager->set_policy_overrides (policy_list,
                                                  CORBA::SET_OVERRIDE);

            break;
          }

          // Change policy at THREAD level
        case Priority_Level::THREAD:
          {
            object =
              orb->resolve_initial_references ("PolicyCurrent");

            CORBA::PolicyCurrent_var const policy_current =
              CORBA::PolicyCurrent::_narrow (object.in ());

            policy_current->set_policy_overrides (policy_list,
                                                  CORBA::SET_OVERRIDE);

            break;
          }

          // Change policy at OBJECT level
        case Priority_Level::OBJECT:
          {
            CORBA::Object_var const object = server->_set_policy_overrides (
              policy_list,
              CORBA::SET_OVERRIDE);

            server = Test::_narrow (object.in ());

            break;
          }
        }
    }
  catch (CORBA::Exception const &ex)
    {
      ex._tao_print_exception ("Caught exception:");
    }
}

int
parse_args (int argc, ACE_TCHAR *argv[], Client_Options &options)
{
  ACE_Get_Opt get_opts (argc, argv, ACE_TEXT("k:n:p:x:"));
  int option;

  while ((option = get_opts ()) != -1)
    switch (option)
      {
      case 'k':
        options.ior = get_opts.opt_arg ();
        break;

        // number of iterations
      case 'n':
        options.iterations = ACE_OS::atoi (get_opts.opt_arg ());
        break;

        // corba priority
      case 'p':
        options.corba_priority = ACE_OS::atoi (get_opts.opt_arg ());
        break;

        // shutdown server
      case 'x':
        options.shutdown_server = ACE_OS::atoi (get_opts.opt_arg ()) != 0;
        break;

      default:
        ACE_ERROR_RETURN ((LM_ERROR,
                           "usage:  %s\n"
                           "\t-k <ior> [default is %s]\n"
                           "\t-n <number of invocations> [defaults to %d]\n"
                           "\t-p <corba priority> [defaults to %d]\n"
                           "\t-x <shutdown server> [defaults to %d]\n"
                           "\n",
                           argv [0],
                           options.ior,
                           options.iterations,
                           options.corba_priority,
                           static_cast<int> (options.shutdown_server)),
                          -1);
      }

  return 0;
}

int
ACE_TMAIN(int argc, ACE_TCHAR *argv[])
{
  try
    {
      CORBA::ORB_var const orb =
        CORBA::ORB_init (argc, argv);

      Client_Options options;
      if (parse_args (argc, argv, options) != 0)
        return -1;

      // Initialize and obtain reference to the Test object.
      CORBA::Object_var const client_object =
        orb->string_to_object (options.ior);

      Test_var server =
        Test::_narrow (client_object.in ());

      // Resolve the Network priority Mapping Manager
      CORBA::Object_var const object =
        orb->resolve_initial_references ("NetworkPriorityMappingManager");

      RTCORBA::NetworkPriorityMappingManager_var const mapping_manager =
        RTCORBA::NetworkPriorityMappingManager::_narrow (object.in ());

      // Initialize the custom priority mapping
      Custom_Network_Priority_Mapping *raw_mapping {};
      ACE_NEW_RETURN (raw_mapping,
                       Custom_Network_Priority_Mapping,
                       -1);

      std::unique_ptr<Custom_Network_Priority_Mapping> cnpm {raw_mapping};

      // Set the desired corba priority on the network mapping manager
      cnpm->corba_priority (options.corba_priority);

      // Load the custom network priority mapping object in the
      // network priority mapping manager. The user can thus add his
      // own priority mapping.
      // The manager takes ownership of the custom mapping.
      mapping_manager->mapping (cnpm.release ());

      bool enable_network_priority = true;

      // Make several invocation,
      for (int i = 0; i < options.iterations; ++i)
        {
          enable_network_priority = !enable_network_priority;

          switch (i)
            {
            case 0:
            case 1:
              change_network_priority (enable_network_priority,
                                       Priority_Level::ORB,
                                       server,
                                       orb.in ());
              break;
            case 2:
            case 3:
              change_network_priority (enable_network_priority,
                                       Priority_Level::THREAD,
                                       server,
                                       orb.in ());
              break;
            default:
              change_network_priority (enable_network_priority,
                                       Priority_Level::OBJECT,
                                       server,
                                       orb.in ());
              break;
            }

          server->test_method ();
        }

      // Shut down Server ORB.
      if (options.shutdown_server)
        {
          server->shutdown ();
        }
    }
  catch (CORBA::Exception const &ex)
    {
      ex._tao_print_exception ("Caught exception:");
      return -1;
    }

  return 0;
}
