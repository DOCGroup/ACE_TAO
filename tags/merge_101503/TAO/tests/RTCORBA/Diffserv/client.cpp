// $Id$

#include "testC.h"
#include "tao/RTCORBA/RTCORBA.h"
#include "tao/RTCORBA/RT_Policy_i.h"
#include "tao/RTCORBA/Network_Priority_Mapping_Manager.h"
#include "tao/RTCORBA/Network_Priority_Mapping.h"
#include "ace/Get_Opt.h"
#include "Custom_Network_Priority_Mapping.h"
#include "tao/Stub.h"

const char *ior = "file://test1.ior";
int n = 1;
int desired_priority = 0;

int
check_for_nil (CORBA::Object_ptr obj, const char *msg)
{
  if (CORBA::is_nil (obj))
    ACE_ERROR_RETURN ((LM_ERROR,
                       "ERROR: Object reference <%s> is nil\n",
                       msg),
                      -1);
  else
    return 0;
}

enum
  {
    OBJECT,
    THREAD,
    ORB
  };

void
change_network_priority (int enable_network_priority,
                         int level,
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

      if (check_for_nil (rt_orb.in (), "RTORB") == -1)
        ACE_ERROR ((LM_ERROR,
                    "RTORB is nil\n"));

      //Set the tcp protocol protperties
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
          //Change policy at ORB level
        case ORB:
          {
            // PolicyManager.
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
          //Change policy at THREAD level
        case THREAD:
          orb->orb_core ()->policy_current ().set_policy_overrides (policy_list,
                                                                    CORBA::SET_OVERRIDE
                                                                    ACE_ENV_ARG_PARAMETER);

          ACE_TRY_CHECK;
          break;
          //Change policy at OBJECT level
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
        };

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
  ACE_Get_Opt get_opts (argc, argv, "k:n:p:sc");
  int c;

  while ((c = get_opts ()) != -1)
    switch (c)
      {
      case 'k':
        ior = get_opts.opt_arg ();
        break;
      case 'n':// number of itarations
        n = ACE_OS::atoi (get_opts.opt_arg ());
        break;
      case 'p':// desired priority
        desired_priority = ACE_OS::atoi (get_opts.opt_arg ());
        break;
      case 'c':
        ior = "file://test1.ior";
        break;
      case 's':
        ior = "file://test2.ior";
        break;
      case '?':
      default:
        ACE_ERROR_RETURN ((LM_ERROR,
                           "usage:  %s "
                           "-k <ior> "
                           "-n <number_of_iterations>"
                           "-p <desired priority>"
                           "-s [test server setting dscp]"
                           "-c [test client setting dscp]"
                           "\n",
                           argv [0]),
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

      if (CORBA::is_nil (server.in ()))
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "ERROR: Object reference <%s> is nil\n",
                             ior),
                            -1);
        }

      //Resolve the Network priority Mapping Manager
      CORBA::Object_var object = orb->resolve_initial_references ("NetworkPriorityMappingManager"
                                                                  ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      RTCORBA::NetworkPriorityMappingManager_var mapping_manager =
        RTCORBA::NetworkPriorityMappingManager::_narrow (object.in ()
                                                         ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      // Initialize the custom priority mapping
      TAO_Custom_Network_Priority_Mapping *cnpm = 0;
      ACE_NEW_RETURN  (cnpm,
                       TAO_Custom_Network_Priority_Mapping,
                       -1);

      //Set the desired corba priority on the network mapping manager
      cnpm->desired_priority (desired_priority);

      //Load the custom network priority mapping object in the network priority      //mapping manager. The user can thus add his own priority mapping.
      mapping_manager->mapping (cnpm);

      ACE_DEBUG ((LM_DEBUG,
                  "Desired Priority = %d\n",
                  desired_priority));

      int enp = 0;       //enable_network_priority

      // Make several invocation,
      for (int i = 0; i < n; ++i)
        {
          switch (i)
            {
            case 0://Set
            case 1://Unset
              toggle (enp);
              change_network_priority (enp, ORB, server ,orb.in ());
              break;
            case 2://Set
            case 3://Unset
              toggle (enp);
              change_network_priority (enp, THREAD, server, orb.in ());
              break;
            case 4://Set
            case 5://Unset
            default:
              toggle (enp);
              change_network_priority (enp, OBJECT, server, orb.in ());
              break;
            };

          server->test_method (0 ACE_ENV_ARG_PARAMETER);
          ACE_TRY_CHECK;

        }

      // Shut down Server ORB.
      server->shutdown ();//(0 ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;
    }
  ACE_CATCH (CORBA::DATA_CONVERSION, ex)
    {
      ACE_PRINT_EXCEPTION(ex,
                          "Most likely, this is due to the in-ability "
                          "to set the thread priority.");
      return -1;
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
