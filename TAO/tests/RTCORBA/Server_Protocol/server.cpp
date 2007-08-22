// $Id$

#include "testS.h"
#include "tao/RTCORBA/RT_Policy_i.h"
#include "tao/RTPortableServer/RTPortableServer.h"
#include "tao/Strategies/advanced_resource.h"
#include "tao/ORB_Core.h"
#include "tao/Policy_Set.h"
#include "ace/Get_Opt.h"
#include "tao/PortableServer/Root_POA.h"

class Test_i : public POA_Test
{
  // = TITLE
  //   An implementation for the Test interface in test.idl
  //
public:
  Test_i (CORBA::ORB_ptr orb);
  // ctor

  //FUZZ: disable check_for_lack_ACE_OS
  void shutdown (void);
  //FUZZ: enable check_for_lack_ACE_OS

private:
  CORBA::ORB_var orb_;
  // The ORB
};

Test_i::Test_i (CORBA::ORB_ptr orb)
  :  orb_ (CORBA::ORB::_duplicate (orb))
{
}

void
Test_i::shutdown (void)
{
  ACE_DEBUG ((LM_DEBUG,
              "Received request to shut down the ORB\n"));
  this->orb_->shutdown (0);
}

//*************************************************************************

const char *ior_output_file = "test.ior";
int protocol_chosen = 0;
CORBA::ULong protocol_type = 0;

// Parse command-line arguments.
int
parse_args (int argc, char *argv[])
{
  ACE_Get_Opt get_opts (argc, argv, "o:p:");
  int c, result;

  while ((c = get_opts ()) != -1)
    switch (c)
      {
      case 'o':
        ior_output_file = get_opts.opt_arg ();
        break;

      case 'p':
        result = ::sscanf (get_opts.opt_arg (),
                           "%u",
                           &protocol_type);
        if (result == 0 || result == EOF)
          ACE_ERROR_RETURN ((LM_ERROR,
                             "Unable to process <-p> option"),
                            -1);

        protocol_chosen = 1;
        break;

      case '?':
      default:
        ACE_ERROR_RETURN ((LM_ERROR,
                           "usage:  %s "
                           "-o <iorfile>"
                           "\n",
                           argv [0]),
                          -1);
      }

  return 0;
}

int
check_server_protocol_at_root_poa (CORBA::ORB_ptr /*orb*/,
                                   PortableServer::POA_ptr poa)
{
  // Users should never write code like below.
  // It is for testing purposes only! (Unfortunately, there
  // is no standard way to access POA policies).
  TAO_Root_POA &tao_poa =
    dynamic_cast<TAO_Root_POA &> (*poa);

  TAO_POA_Policy_Set &policies =
    tao_poa.policies ();

  CORBA::Policy_var server_protocol =
    policies.get_cached_policy (TAO_CACHED_POLICY_RT_SERVER_PROTOCOL);

  RTCORBA::ServerProtocolPolicy_var policy =
    RTCORBA::ServerProtocolPolicy::_narrow (server_protocol.in ());

  if (CORBA::is_nil (policy.in ()))
    ACE_ERROR_RETURN ((LM_ERROR,
                       "\nERROR: ORB default for ServerProtocolPolicy "
                       "is missing\n"),
                      -1);

  RTCORBA::ProtocolList_var protocols = policy->protocols ();

  ACE_DEBUG ((LM_DEBUG,
              "\nRoot POA ServerProtocolPolicy "
              "contains %u protocols:\n", protocols->length ()));

  for (CORBA::ULong i = 0; i < protocols->length (); ++i)
    {
      CORBA::ULong protocol_type = protocols[i].protocol_type;
      ACE_DEBUG ((LM_DEBUG,
                  "  Protocol %u: IOP::ProfileId = %u\n",
                  i, protocol_type));
      RTCORBA::ProtocolProperties_var properties =
        protocols[i].transport_protocol_properties;

      // Print properties.
      if (protocol_type == IOP::TAG_INTERNET_IOP)
        {
          RTCORBA::TCPProtocolProperties_var tcp_properties =
            RTCORBA::TCPProtocolProperties::_narrow (properties.in ());

          if (!CORBA::is_nil (tcp_properties.in ()))
            ACE_DEBUG ((LM_DEBUG,
                        "     Properties: \n"
                        "      send_buffer_size = %d\n"
                        "      receive_buffer_size = %d\n"
                        "      keep_alive = %d\n"
                        "      dont_route = %d\n"
                        "      no_delay = %d\n"
                        "      enable_network_priority = %d\n",
                        tcp_properties->send_buffer_size (),
                        tcp_properties->recv_buffer_size (),
                        tcp_properties->keep_alive (),
                        tcp_properties->dont_route (),
                        tcp_properties->no_delay (),
                        tcp_properties->enable_network_priority ()));
        }
      else if (protocol_type == TAO_TAG_UIOP_PROFILE)
        {
          RTCORBA::UnixDomainProtocolProperties_var uiop_properties =
            RTCORBA::UnixDomainProtocolProperties::_narrow (properties.in ());

          if (!CORBA::is_nil (uiop_properties.in ()))
            ACE_DEBUG ((LM_DEBUG,
                        "     Properties: \n"
                        "      send_buffer_size = %d\n"
                        "      receive_buffer_size = %d\n",
                        uiop_properties->send_buffer_size (),
                        uiop_properties->recv_buffer_size ()));
        }
      else if (protocol_type == TAO_TAG_SHMEM_PROFILE)
        {
          RTCORBA::SharedMemoryProtocolProperties_var shmem_properties =
            RTCORBA::SharedMemoryProtocolProperties::_narrow (properties.in ());

          if (!CORBA::is_nil (shmem_properties.in ()))
            ACE_DEBUG ((LM_DEBUG,
                        "     Properties: \n"
                        "      send_buffer_size = %d\n"
                        "      receive_buffer_size = %d\n"
                        "      keep_alive = %d\n"
                        "      dont_route = %d\n"
                        "      no_delay = %d\n"
                        "      preallocate_buffer_size = %d\n"
                        "      mmap_filename = %s\n"
                        "      mmap_lockname = %s\n",
                        shmem_properties->send_buffer_size (),
                        shmem_properties->recv_buffer_size (),
                        shmem_properties->keep_alive (),
                        shmem_properties->dont_route (),
                        shmem_properties->no_delay (),
                        shmem_properties->preallocate_buffer_size (),
                        shmem_properties->mmap_filename (),
                        shmem_properties->mmap_lockname ()));
        }
      else if (protocol_type == TAO_TAG_DIOP_PROFILE)
        {
          RTCORBA::UserDatagramProtocolProperties_var diop_properties =
            RTCORBA::UserDatagramProtocolProperties::_narrow (properties.in ());

          if (!CORBA::is_nil (diop_properties.in ()))
            ACE_DEBUG ((LM_DEBUG,
                        "     Properties: \n"
                        "      enable_network_priority = %d\n",
                        diop_properties->enable_network_priority ()));
        }
      else if (protocol_type == TAO_TAG_SCIOP_PROFILE)
        {
          RTCORBA::StreamControlProtocolProperties_var sciop_properties =
            RTCORBA::StreamControlProtocolProperties::_narrow (properties.in ());

          if (!CORBA::is_nil (sciop_properties.in ()))
            ACE_DEBUG ((LM_DEBUG,
                        "     Properties: \n"
                        "      send_buffer_size = %d\n"
                        "      receive_buffer_size = %d\n"
                        "      keep_alive = %d\n"
                        "      dont_route = %d\n"
                        "      no_delay = %d\n"
                        "      enable_network_priority = %d\n",
                        sciop_properties->send_buffer_size (),
                        sciop_properties->recv_buffer_size (),
                        sciop_properties->keep_alive (),
                        sciop_properties->dont_route (),
                        sciop_properties->no_delay (),
                        sciop_properties->enable_network_priority ()));
        }
    }

  return 0;
}


int
ACE_TMAIN(int argc, ACE_TCHAR *argv[])
{
  try
    {
      CORBA::ORB_var orb =
        CORBA::ORB_init (argc, argv);

      if (parse_args (argc, argv) != 0)
        return -1;

      CORBA::Object_var object =
        orb->resolve_initial_references("RootPOA");

      PortableServer::POA_var root_poa =
        PortableServer::POA::_narrow (object.in ());

      if (CORBA::is_nil (root_poa.in ()))
        ACE_ERROR_RETURN ((LM_ERROR,
                           "ERROR: Panic <RootPOA> is nil\n"),
                          -1);

      PortableServer::POAManager_var poa_manager =
        root_poa->the_POAManager ();

      object = orb->resolve_initial_references ("RTORB");
      RTCORBA::RTORB_var rt_orb = RTCORBA::RTORB::_narrow (object.in ());

      // Check ServerProtocol configurations.
      int result = check_server_protocol_at_root_poa (orb.in (),
                                                      root_poa.in ());
      if (result != 0)
        return -1;

      // If a protocol type was indicated using command line option,
      // create a child POA with ServerProtocolPolicy containing
      // that protocol and register Test object with child POA.
      // Otherwise, register Test object with the root POA (ORB
      // default ServerProtocolPolicy will get used).
      RTCORBA::ProtocolList protocols;
      protocols.length (1);
      protocols[0].protocol_type = protocol_type;
      protocols[0].transport_protocol_properties =
        RTCORBA::ProtocolProperties::_nil ();
      protocols[0].orb_protocol_properties =
        RTCORBA::ProtocolProperties::_nil ();

      CORBA::PolicyList poa_policy_list;
      poa_policy_list.length (1);
      poa_policy_list[0] =
        rt_orb->create_server_protocol_policy (protocols);

      PortableServer::POA_var poa;

      if (protocol_chosen)
        {
          poa =
            root_poa->create_POA ("Child_POA",
                                  poa_manager.in (),
                                  poa_policy_list);
        }

      else
        {
          poa = PortableServer::POA::_duplicate (root_poa.in ());
        }

      Test_i server_impl (orb.in ());

      PortableServer::ObjectId_var id =
        poa->activate_object (&server_impl);

      CORBA::Object_var server =
        poa->id_to_reference (id.in ());

      // Print Object IOR.
      CORBA::String_var ior =
        orb->object_to_string (server.in ());

      ACE_DEBUG ((LM_DEBUG, "\nActivated as <%s>\n\n", ior.in ()));

      if (ior_output_file != 0)
        {
          FILE *output_file= ACE_OS::fopen (ior_output_file, "w");
          if (output_file == 0)
            ACE_ERROR_RETURN ((LM_ERROR,
                               "Cannot open output file for writing IOR: %s",
                               ior_output_file),
                              -1);
          ACE_OS::fprintf (output_file, "%s", ior.in ());
          ACE_OS::fclose (output_file);
        }

      // Run ORB Event loop.
      poa_manager->activate ();

      orb->run ();

      ACE_DEBUG ((LM_DEBUG, "Server ORB event loop finished\n\n"));
    }
  catch (const CORBA::Exception& ex)
    {
      ex._tao_print_exception ("Exception caught:");
      return -1;
    }

  return 0;
}
