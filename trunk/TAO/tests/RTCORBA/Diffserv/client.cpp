// $Id$

#include "testC.h"
#include "tao/RTCORBA/RTCORBA.h"
#include "tao/RTCORBA/Network_Priority_Mapping_Manager.h"
#include "tao/RTCORBA/Network_Priority_Mapping.h"
#include "Custom_Network_Priority_Mapping.h"
#include "ace/Get_Opt.h"
#include "tao/Policy_Manager.h"

static const char *ior = "file://simple_servant.ior";
static int iterations = 1;
static int corba_priority = RTCORBA::minPriority;
static int shutdown_server = 0;

enum Priority_Level
{
  OBJECT,
  THREAD,
  ORB
};

void
change_network_priority (int enable_network_priority,
                         Priority_Level level,
                         Test_var &server,
                         CORBA::ORB_ptr orb)
{
  ACE_TRY_NEW_ENV
    {
      CORBA::Object_var object =
        orb->resolve_initial_references ("RTORB" ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      RTCORBA::RTORB_var rt_orb =
        RTCORBA::RTORB::_narrow (object.in ()
                                 ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      // Set the tcp protocol protperties
      RTCORBA::TCPProtocolProperties_var tcp_properties =
        rt_orb->create_tcp_protocol_properties (ACE_DEFAULT_MAX_SOCKET_BUFSIZ,
                                                ACE_DEFAULT_MAX_SOCKET_BUFSIZ,
                                                1,
                                                0,
                                                1,
                                                enable_network_priority
                                                ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

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
        rt_orb->create_client_protocol_policy (protocols
                                               ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      switch (level)
        {
          // Change policy at ORB level
        case ORB:
          {
            object = orb->resolve_initial_references ("ORBPolicyManager"
                                                      ACE_ENV_ARG_PARAMETER);
            ACE_TRY_CHECK;

            CORBA::PolicyManager_var policy_manager =
              CORBA::PolicyManager::_narrow (object.in () ACE_ENV_ARG_PARAMETER);
            ACE_TRY_CHECK;

            policy_manager->set_policy_overrides (policy_list,
                                                  CORBA::SET_OVERRIDE
                                                  ACE_ENV_ARG_PARAMETER);
            ACE_TRY_CHECK;

            break;
          }

          // Change policy at THREAD level
        case THREAD:
          {
            object =
              orb->resolve_initial_references ("PolicyCurrent"
                                               ACE_ENV_ARG_PARAMETER);
            ACE_TRY_CHECK;

            CORBA::PolicyCurrent_var policy_current =
              CORBA::PolicyCurrent::_narrow (object.in () ACE_ENV_ARG_PARAMETER);
            ACE_TRY_CHECK;

            policy_current->set_policy_overrides (policy_list,
                                                  CORBA::SET_OVERRIDE
                                                  ACE_ENV_ARG_PARAMETER);
            ACE_TRY_CHECK;

            break;
          }

          // Change policy at OBJECT level
        case OBJECT:
          {
            CORBA::Object_var object = server->_set_policy_overrides (policy_list,
                                                                      CORBA::SET_OVERRIDE
                                                                      ACE_ENV_ARG_PARAMETER);
            ACE_TRY_CHECK;

            server = Test::_narrow (object.in () ACE_ENV_ARG_PARAMETER);
            ACE_TRY_CHECK;

            break;
          }
        }
    }
  ACE_CATCHANY
    {
      ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION,
                           "Caught exception:");
    }
  ACE_ENDTRY;
}

int
parse_args (int argc, char *argv[])
{
  ACE_Get_Opt get_opts (argc, argv, "k:n:p:x:");
  int c;

  while ((c = get_opts ()) != -1)
    switch (c)
      {
      case 'k':
        ior = get_opts.opt_arg ();
        break;

        // number of itarations
      case 'n':
        iterations = ACE_OS::atoi (get_opts.opt_arg ());
        break;

        // corba priority
      case 'p':
        corba_priority = ACE_OS::atoi (get_opts.opt_arg ());
        break;

        // shutdown server
      case 'x':
        shutdown_server = ACE_OS::atoi (get_opts.opt_arg ());
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
                           ior,
                           iterations,
                           corba_priority,
                           shutdown_server),
                          -1);
      }

  return 0;
}

void
toggle (int &i)
{
  if (i == 0)
    i = 1;
  else i = 0;
}

int
main (int argc, char *argv[])
{
  ACE_TRY_NEW_ENV
    {
      CORBA::ORB_var orb =
        CORBA::ORB_init (argc, argv, "" ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      if (parse_args (argc, argv) != 0)
        return -1;

      // Initialize and obtain reference to the Test object.
      CORBA::Object_var client_object =
        orb->string_to_object (ior ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      Test_var server =
        Test::_narrow (client_object.in () ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      // Resolve the Network priority Mapping Manager
      CORBA::Object_var object =
        orb->resolve_initial_references ("NetworkPriorityMappingManager"
                                         ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      RTCORBA::NetworkPriorityMappingManager_var mapping_manager =
        RTCORBA::NetworkPriorityMappingManager::_narrow (object.in ()
                                                         ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      // Initialize the custom priority mapping
      Custom_Network_Priority_Mapping *cnpm = 0;
      ACE_NEW_RETURN  (cnpm,
                       Custom_Network_Priority_Mapping,
                       -1);

      // Set the desired corba priority on the network mapping manager
      cnpm->corba_priority (corba_priority);

      // Load the custom network priority mapping object in the
      // network priority mapping manager. The user can thus add his
      // own priority mapping.
      mapping_manager->mapping (cnpm);

      int enable_network_priority = 1;

      // Make several invocation,
      for (int i = 0; i < iterations; ++i)
        {
          toggle (enable_network_priority);

          switch (i)
            {
            case 0:
            case 1:
              change_network_priority (enable_network_priority, ORB, server, orb.in ());
              break;
            case 2:
            case 3:
              change_network_priority (enable_network_priority, THREAD, server, orb.in ());
              break;
            default:
              change_network_priority (enable_network_priority, OBJECT, server, orb.in ());
              break;
            }

          server->test_method (ACE_ENV_SINGLE_ARG_PARAMETER);
          ACE_TRY_CHECK;
        }

      // Shut down Server ORB.
      if (shutdown_server)
        {
          server->shutdown (ACE_ENV_SINGLE_ARG_PARAMETER);
          ACE_TRY_CHECK;
        }
    }
  ACE_CATCHANY
    {
      ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION,
                           "Caught exception:");
      return -1;
    }
  ACE_ENDTRY;

  return 0;
}
