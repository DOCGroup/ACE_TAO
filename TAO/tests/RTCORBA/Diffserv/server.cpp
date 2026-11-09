#include "testS.h"
#include "ace/Get_Opt.h"
#include "ace/OS_NS_stdio.h"
#include "tao/RTCORBA/RTCORBA.h"
#include "tao/RTPortableServer/RTPortableServer.h"
#include "tao/RTCORBA/Network_Priority_Mapping_Manager.h"
#include "tao/RTCORBA/Network_Priority_Mapping.h"
#include "Custom_Network_Priority_Mapping.h"
#include "tao/debug.h"

#include <memory>

struct Server_Options
{
  RTCORBA::Priority corba_priority {RTCORBA::minPriority};
};

class Test_i : public POA_Test
{
public:
  explicit Test_i (CORBA::ORB_ptr orb);
  Test_i (Test_i const &) = delete;
  Test_i (Test_i &&) = delete;
  Test_i &operator= (Test_i const &) = delete;
  Test_i &operator= (Test_i &&) = delete;

  void test_method () override;

  //FUZZ: disable check_for_lack_ACE_OS
  void shutdown () override;
  //FUZZ: enable check_for_lack_ACE_OS

private:
  CORBA::ORB_var const orb_;
};

Test_i::Test_i (CORBA::ORB_ptr orb)
  : orb_ (CORBA::ORB::_duplicate (orb))
{
}

void
Test_i::test_method ()
{
  ACE_DEBUG ((LM_DEBUG,
              "Test_i::test_method\n"));
}

void
Test_i::shutdown ()
{
  this->orb_->shutdown (false);
}

constexpr ACE_TCHAR simple_servant_ior_file[] = ACE_TEXT("simple_servant.ior");
constexpr ACE_TCHAR diffserv_servant_ior_file[] = ACE_TEXT("diffserv_servant.ior");

int
parse_args (int argc, ACE_TCHAR *argv[], Server_Options &options)
{
  ACE_Get_Opt get_opts (argc, argv, ACE_TEXT("p:"));
  int option;

  while ((option = get_opts ()) != -1)
    switch (option)
      {
      case 'p':
        options.corba_priority = ACE_OS::atoi (get_opts.opt_arg ());
        break;

      default:
        ACE_ERROR_RETURN ((LM_ERROR,
                           "usage:  %s\n"
                           "\t-p <corba priority> [defaults to %d]\n"
                           "\n",
                           argv [0],
                           options.corba_priority),
                          -1);
      }

  return 0;
}

int
create_object (PortableServer::POA_ptr poa,
               CORBA::ORB_ptr orb,
               PortableServer::Servant servant,
               ACE_TCHAR const *filename)
{
  // Register with poa.
  PortableServer::ObjectId_var const id =
    poa->activate_object (servant);

  CORBA::Object_var const object =
    poa->id_to_reference (id.in ());

  CORBA::String_var const ior =
    orb->object_to_string (object.in ());

  std::unique_ptr<FILE, int (*) (FILE *)> const output_file (
    ACE_OS::fopen (filename, "w"), ACE_OS::fclose);
  if (!output_file)
    ACE_ERROR_RETURN ((LM_ERROR,
                       ACE_TEXT ("Cannot open output file for IOR: %s\n"),
                       filename),
                      -1);

  ACE_OS::fprintf (output_file.get (), "%s", ior.in ());
  return 0;
}

int
ACE_TMAIN(int argc, ACE_TCHAR *argv[])
{
  try
    {
      CORBA::Object_var object;

      // ORB.
      CORBA::ORB_var const orb =
        CORBA::ORB_init (argc, argv);

      // Parse arguments.
      Server_Options options;
      if (parse_args (argc, argv, options) != 0)
        return -1;

      // RootPOA.
      object =
        orb->resolve_initial_references ("RootPOA");

      PortableServer::POA_var const root_poa =
        PortableServer::POA::_narrow (object.in ());

      // POAManager.
      PortableServer::POAManager_var const poa_manager =
        root_poa->the_POAManager ();

      // Servant.
      Test_i servant (orb.in ());

      // Create the first object in Root POA
      if (create_object (root_poa.in (),
                         orb.in (),
                         &servant,
                         simple_servant_ior_file) != 0)
        return -1;

      object =
        orb->resolve_initial_references ("NetworkPriorityMappingManager");

      RTCORBA::NetworkPriorityMappingManager_var const mapping_manager =
        RTCORBA::NetworkPriorityMappingManager::_narrow (object.in ());

      Custom_Network_Priority_Mapping *raw_mapping {};
      ACE_NEW_RETURN (raw_mapping,
                      Custom_Network_Priority_Mapping,
                      -1);
      std::unique_ptr<Custom_Network_Priority_Mapping> cnpm {raw_mapping};

      cnpm->corba_priority (options.corba_priority);

      // The manager takes ownership of the custom mapping.
      mapping_manager->mapping (cnpm.release ());

      // RTORB.
      object =
        orb->resolve_initial_references ("RTORB");

      RTCORBA::RTORB_var const rt_orb =
        RTCORBA::RTORB::_narrow (object.in ());

      // Set transport protocol properties
      RTCORBA::TCPProtocolProperties_var const tcp_properties =
        rt_orb->create_tcp_protocol_properties (ACE_DEFAULT_MAX_SOCKET_BUFSIZ,
                                                ACE_DEFAULT_MAX_SOCKET_BUFSIZ,
                                                1,
                                                0,
                                                1,
                                                1);

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
        rt_orb->create_server_protocol_policy (protocols);

      // Create POA with Diffserv enabled
      PortableServer::POA_var poa_with_diffserv =
        root_poa->create_POA ("POA_WITH_DS",
                              poa_manager.in (),
                              policy_list);

      // Create object 2.
      if (create_object (poa_with_diffserv.in (),
                         orb.in (),
                         &servant,
                         diffserv_servant_ior_file) != 0)
        return -1;

      // Activate POA manager.
      poa_manager->activate ();

      // Start ORB event loop.
      orb->run ();

      ACE_DEBUG ((LM_DEBUG, "Server ORB event loop finished\n\n"));
    }
  catch (CORBA::Exception const &ex)
    {
      ex._tao_print_exception ("Unexpected exception caught:");
      return -1;
    }

  return 0;
}
