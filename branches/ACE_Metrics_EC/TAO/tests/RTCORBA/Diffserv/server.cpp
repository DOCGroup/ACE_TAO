// $Id$

#include "testS.h"
#include "ace/Get_Opt.h"
#include "tao/RTCORBA/RTCORBA.h"
#include "tao/RTPortableServer/RTPortableServer.h"
#include "tao/RTCORBA/Network_Priority_Mapping_Manager.h"
#include "tao/RTCORBA/Network_Priority_Mapping.h"
#include "Custom_Network_Priority_Mapping.h"
#include "tao/debug.h"

class Test_i : public POA_Test
{
  // = TITLE
  //   An implementation for the Test interface in test.idl
  //
public:
  Test_i (CORBA::ORB_ptr orb);
  // ctor

  void test_method (CORBA::Short priority
                    ACE_ENV_ARG_DECL_NOT_USED)
    ACE_THROW_SPEC ((CORBA::SystemException));

  void shutdown (ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
    ACE_THROW_SPEC ((CORBA::SystemException));

private:
  CORBA::ORB_var orb_;
  // The ORB
};

Test_i::Test_i (CORBA::ORB_ptr orb)
  :  orb_ (CORBA::ORB::_duplicate (orb))
{
}

void
Test_i::test_method (CORBA::Short priority
                     ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  // Use RTCurrent to find out the CORBA priority of the current
  // thread.

  CORBA::Object_var obj =
    this->orb_->resolve_initial_references ("RTCurrent" ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;

  RTCORBA::Current_var current =
    RTCORBA::Current::_narrow (obj.in () ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;

  if (CORBA::is_nil (obj.in ()))
    ACE_THROW (CORBA::INTERNAL ());

  CORBA::Short servant_thread_priority =
    current->the_priority (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK;

  // Print out the info.
  if (servant_thread_priority != priority)
    ACE_DEBUG ((LM_DEBUG,
                "ERROR: servant thread priority is not equal"
                "to method argument.\n"));

  if (TAO_debug_level > 0)
    ACE_DEBUG ((LM_DEBUG,
                "Servant thread priority: %d\n",
                servant_thread_priority));
}

void
Test_i::shutdown (ACE_ENV_SINGLE_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  this->orb_->shutdown (0 ACE_ENV_ARG_PARAMETER);
}

//*************************************************************************

const char *ior_output_file1 = "test1.ior";
const char *ior_output_file2 = "test2.ior";
CORBA::Short poa_priority = -1;
CORBA::Short object_priority = -1;
int test_server = 0;
int test_client = 1;

// Parse command-line arguments.
int
parse_args (int argc, char *argv[])
{
  ACE_Get_Opt get_opts (argc, argv, "p:o:a:b:sc");
  int c, result;

  while ((c = get_opts ()) != -1)
    switch (c)
      {
      case 'k':
        ior_output_file1 = get_opts.opt_arg ();
        break;

      case 'o':
        ior_output_file2 = get_opts.opt_arg ();
        break;

      case 'p':
        result = ::sscanf (get_opts.opt_arg (),
                           "%hd",
                           &poa_priority);
        if (result == 0 || result == EOF)
          ACE_ERROR_RETURN ((LM_ERROR,
                             "Unable to process <-a> option"),
                            -1);
        break;
      case 's':
        test_server = 1;
        test_client = 0;
        break;

      case 'c':
        test_client = 1;
        test_server = 0;
        break;
      case '?':
      default:
        ACE_ERROR_RETURN ((LM_ERROR,
                           "usage:  %s "
                           "-k <iorfile1> "
                           "-o <iorfile2> "
                           "-p <poa_priority> "
                           "-s [test_server setting of dscp]"
                           "-c [test client setting of dscp]"
                           "\n",
                           argv [0]),
                          -1);
      }

  return 0;
}

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

int
create_object (RTPortableServer::POA_ptr poa,
               CORBA::ORB_ptr orb,
               Test_i *server_impl,
               //CORBA::Short priority,
               const char *filename
               ACE_ENV_ARG_DECL)
{
  // Register with poa.
  PortableServer::ObjectId_var id;

  id = poa->activate_object (server_impl ACE_ENV_ARG_PARAMETER);

  ACE_CHECK_RETURN (-1);

  CORBA::Object_var server =
    poa->id_to_reference (id.in ()
                          ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (-1);

  // Print out the IOR.
  CORBA::String_var ior =
    orb->object_to_string (server.in () ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (-1);

  ACE_DEBUG ((LM_DEBUG, "<%s>\n\n", ior.in ()));

  // Print ior to the file.
  if (filename != 0)
    {
      FILE *output_file= ACE_OS::fopen (filename, "w");
      if (output_file == 0)
        ACE_ERROR_RETURN ((LM_ERROR,
                           "Cannot open output file for writing IOR: %s",
                           filename),
                          -1);
      ACE_OS::fprintf (output_file, "%s", ior.in ());
      ACE_OS::fclose (output_file);
    }

  return 0;
}

int
main (int argc, char *argv[])
{
  // Make sure we can support multiple priorities that are required
  // for this test.


  ACE_TRY_NEW_ENV
    {
      // ORB.
      CORBA::ORB_var orb =
        CORBA::ORB_init (argc, argv, "" ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      // Parse arguments.
      if (parse_args (argc, argv) != 0)
        return -1;

      // RTORB.
      CORBA::Object_var object =
        orb->resolve_initial_references ("RTORB" ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;
      RTCORBA::RTORB_var rt_orb = RTCORBA::RTORB::_narrow (object.in ()
                                                           ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;
      if (check_for_nil (rt_orb.in (), "RTORB") == -1)
        return -1;

      // RootPOA.
      object =
        orb->resolve_initial_references("RootPOA" ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;
      PortableServer::POA_var root_poa =
        PortableServer::POA::_narrow (object.in () ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;
      if (check_for_nil (root_poa.in (), "RootPOA") == -1)
        return -1;

      // POAManager.
      PortableServer::POAManager_var poa_manager =
        root_poa->the_POAManager (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;

      // Servant.
      Test_i server_impl (orb.in ());

      CORBA::PolicyList poa_policy_list;

      int result;

      if (test_client)
        {
          poa_policy_list.length (1);

          poa_policy_list[0] =
            root_poa->create_id_uniqueness_policy (PortableServer::MULTIPLE_ID
                                                   ACE_ENV_ARG_PARAMETER);
          ACE_TRY_CHECK;

          //Create POA without Diffserv enabled
          PortableServer::POA_var poa_without_ds =
            root_poa->create_POA ("POA_WITHOUT_DS",
                                  poa_manager.in (),
                                  poa_policy_list
                                  ACE_ENV_ARG_PARAMETER);
          ACE_TRY_CHECK;

          RTPortableServer::POA_var rt_poa_without_ds =
            RTPortableServer::POA::_narrow (poa_without_ds.in () ACE_ENV_ARG_PARAMETER);
          ACE_TRY_CHECK;
          if (check_for_nil (poa_without_ds.in (), "RTPOA") == -1)
            return -1;

          // Create object 1
          ACE_DEBUG ((LM_DEBUG, "\nActivated object two as "));
          result = create_object (rt_poa_without_ds.in (), orb.in (), &server_impl,
                                  ior_output_file1 ACE_ENV_ARG_PARAMETER);
          ACE_TRY_CHECK;
          if (result == -1)
            return -1;
        }
      else if (test_server)
        {
          poa_policy_list.length (1);
          poa_policy_list[0] =
            rt_orb->create_priority_model_policy (RTCORBA::CLIENT_PROPAGATED,
                                                  0
                                                  ACE_ENV_ARG_PARAMETER);
          ACE_TRY_CHECK;

          object = orb->resolve_initial_references ("NetworkPriorityMappingManager"
                                                    ACE_ENV_ARG_PARAMETER);
          ACE_TRY_CHECK;

          RTCORBA::NetworkPriorityMappingManager_var mapping_manager =
            RTCORBA::NetworkPriorityMappingManager::_narrow (object.in ()
                                                             ACE_ENV_ARG_PARAMETER);
          ACE_TRY_CHECK;

          TAO_Custom_Network_Priority_Mapping *cnpm = 0;
          ACE_NEW_RETURN  (cnpm,
                           TAO_Custom_Network_Priority_Mapping,
                           -1);

          cnpm->desired_priority (poa_priority);

          mapping_manager->mapping (cnpm);

          poa_policy_list.length (2);

          //Set transport protocol properties
          RTCORBA::TCPProtocolProperties_var tcp_properties =
            rt_orb->create_tcp_protocol_properties (ACE_DEFAULT_MAX_SOCKET_BUFSIZ,
                                                    ACE_DEFAULT_MAX_SOCKET_BUFSIZ,
                                                    1,
                                                    0,
                                                    1,
                                                    1
                                                    ACE_ENV_ARG_PARAMETER);
          ACE_TRY_CHECK;

          RTCORBA::ProtocolList protocols;
          protocols.length (1);
          protocols[0].protocol_type = 0;
          protocols[0].transport_protocol_properties =
            RTCORBA::ProtocolProperties::_duplicate (tcp_properties.in ());
          protocols[0].orb_protocol_properties =
            RTCORBA::ProtocolProperties::_nil ();

          poa_policy_list[1] =
            rt_orb->create_server_protocol_policy (protocols
                                                   ACE_ENV_ARG_PARAMETER);
          ACE_TRY_CHECK;

          //Create POA with Diffserv enabled
          PortableServer::POA_var poa_with_ds =
            root_poa->create_POA ("POA_WITH_DS",
                                  poa_manager.in (),
                                  poa_policy_list
                                  ACE_ENV_ARG_PARAMETER);
          ACE_TRY_CHECK;

          RTPortableServer::POA_var rt_poa_with_ds =
            RTPortableServer::POA::_narrow (poa_with_ds.in () ACE_ENV_ARG_PARAMETER);
          ACE_TRY_CHECK;
          if (check_for_nil (poa_with_ds.in (), "RTPOA") == -1)
            return -1;

          // Create object 2 (it will inherit POA's priority).
          ACE_DEBUG ((LM_DEBUG, "\nActivated object one as "));
          result = create_object (rt_poa_with_ds.in (), orb.in (), &server_impl,
                                  ior_output_file2 ACE_ENV_ARG_PARAMETER);
          ACE_TRY_CHECK;
          if (result == -1)
            return -1;



        }

      // Activate POA manager.
      poa_manager->activate (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;

      // Start ORB event loop.
      orb->run (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;

      ACE_DEBUG ((LM_DEBUG, "Server ORB event loop finished\n\n"));
    }
  ACE_CATCHANY
    {
      ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION,
                           "Unexpected exception caught in Server_Declared test server:");
      return -1;
    }
  ACE_ENDTRY;

  return 0;
}
