// $Id$

#include "testS.h"
#include "ace/Get_Opt.h"
#include "ace/OS_NS_stdio.h"
#include "tao/RTCORBA/RTCORBA.h"
#include "tao/RTPortableServer/RTPortableServer.h"
#include "tao/RTCORBA/Network_Priority_Mapping_Manager.h"
#include "tao/RTCORBA/Network_Priority_Mapping.h"
#include "Custom_Network_Priority_Mapping.h"
#include "tao/debug.h"

class Test_i : public POA_Test
{
public:
  Test_i (CORBA::ORB_ptr orb);

  void test_method (ACE_ENV_SINGLE_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException));

  void shutdown (ACE_ENV_SINGLE_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException));

private:
  CORBA::ORB_var orb_;
};

Test_i::Test_i (CORBA::ORB_ptr orb)
  : orb_ (CORBA::ORB::_duplicate (orb))
{
}

void
Test_i::test_method (ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  ACE_DEBUG ((LM_DEBUG,
              "Test_i::test_method\n"));
}

void
Test_i::shutdown (ACE_ENV_SINGLE_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  this->orb_->shutdown (0 ACE_ENV_ARG_PARAMETER);
}

static const char *simple_servant_ior_file = "simple_servant.ior";
static const char *diffserv_servant_ior_file = "diffserv_servant.ior";
static RTCORBA::Priority corba_priority = RTCORBA::minPriority;

int
parse_args (int argc, char *argv[])
{
  ACE_Get_Opt get_opts (argc, argv, "p:");
  int c;

  while ((c = get_opts ()) != -1)
    switch (c)
      {
      case 'p':
        corba_priority = ACE_OS::atoi (get_opts.opt_arg ());
        break;

      default:
        ACE_ERROR_RETURN ((LM_ERROR,
                           "usage:  %s \n"
                           "\t-p <corba priority> [defaults to %d]\n"
                           "\n",
                           argv [0],
                           corba_priority),
                          -1);
      }

  return 0;
}

void
create_object (PortableServer::POA_ptr poa,
               CORBA::ORB_ptr orb,
               PortableServer::Servant servant,
               const char *filename
               ACE_ENV_ARG_DECL)
{
  // Register with poa.
  PortableServer::ObjectId_var id =
    poa->activate_object (servant
                          ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;

  CORBA::Object_var object =
    poa->id_to_reference (id.in ()
                          ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;

  CORBA::String_var ior =
    orb->object_to_string (object.in () ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;

  FILE *output_file= ACE_OS::fopen (filename, "w");
  ACE_OS::fprintf (output_file, "%s", ior.in ());
  ACE_OS::fclose (output_file);
}

int
main (int argc, char *argv[])
{
  ACE_TRY_NEW_ENV
    {
      CORBA::Object_var object;

      // ORB.
      CORBA::ORB_var orb =
        CORBA::ORB_init (argc, argv, ""
                         ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      // Parse arguments.
      if (parse_args (argc, argv) != 0)
        return -1;

      // RootPOA.
      object =
        orb->resolve_initial_references ("RootPOA"
                                         ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      PortableServer::POA_var root_poa =
        PortableServer::POA::_narrow (object.in () ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      // POAManager.
      PortableServer::POAManager_var poa_manager =
        root_poa->the_POAManager (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;

      // Servant.
      Test_i servant (orb.in ());

      // Create the first object in Root POA
      create_object (root_poa.in (),
                     orb.in (),
                     &servant,
                     simple_servant_ior_file
                     ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      object =
        orb->resolve_initial_references ("NetworkPriorityMappingManager"
                                         ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      RTCORBA::NetworkPriorityMappingManager_var mapping_manager =
        RTCORBA::NetworkPriorityMappingManager::_narrow (object.in ()
                                                         ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      Custom_Network_Priority_Mapping *cnpm = 0;
      ACE_NEW_RETURN  (cnpm,
                       Custom_Network_Priority_Mapping,
                       -1);

      cnpm->corba_priority (corba_priority);

      mapping_manager->mapping (cnpm);

      // RTORB.
      object =
        orb->resolve_initial_references ("RTORB"
                                         ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      RTCORBA::RTORB_var rt_orb =
        RTCORBA::RTORB::_narrow (object.in ()
                                 ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      // Set transport protocol properties
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

      CORBA::PolicyList policy_list;
      policy_list.length (1);

      policy_list[0] =
        rt_orb->create_server_protocol_policy (protocols
                                               ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      // Create POA with Diffserv enabled
      PortableServer::POA_var poa_with_diffserv =
        root_poa->create_POA ("POA_WITH_DS",
                              poa_manager.in (),
                              policy_list
                              ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      // Create object 2.
      create_object (poa_with_diffserv.in (),
                     orb.in (),
                     &servant,
                     diffserv_servant_ior_file
                     ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

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
                           "Unexpected exception caught:");
      return -1;
    }
  ACE_ENDTRY;

  return 0;
}
