#include "testS.h"
#include "ace/Get_Opt.h"
#include "tao/RTCORBA/RTCORBA.h"
#include "tao/RTCORBA/RT_Policy_i.h"
#include "tao/RTPortableServer/RTPortableServer.h"
#include "tao/Strategies/advanced_resource.h"

#include <cstdio>
#include <memory>

class Test_i : public POA_Test
{
  // = TITLE
  //   An implementation for the Test interface in test.idl
  //
public:
  explicit Test_i (CORBA::ORB_ptr orb);
  // ctor

  void test_method () override;

  //FUZZ: disable check_for_lack_ACE_OS
  void shutdown () override;
  //FUZZ: enable check_for_lack_ACE_OS

private:
  Test_i (Test_i const &) = delete;
  Test_i &operator= (Test_i const &) = delete;
  Test_i (Test_i &&) = delete;
  Test_i &operator= (Test_i &&) = delete;

  CORBA::ORB_var orb_;
  // The ORB
};

Test_i::Test_i (CORBA::ORB_ptr orb)
  : orb_ (CORBA::ORB::_duplicate (orb))
{
}

void
Test_i::test_method ()
{
  ACE_DEBUG ((LM_DEBUG,
              "Test method invoked on the sever\n"));
}

void
Test_i::shutdown ()
{
  ACE_DEBUG ((LM_DEBUG,
              "(%P|%t) Shutting down\n"));
  this->orb_->shutdown (false);
}

//*************************************************************************

struct Server_Options
{
  ACE_TCHAR const *server_policy_ior {ACE_TEXT("test1.ior")};
  ACE_TCHAR const *default_policy_ior {ACE_TEXT("test2.ior")};
  CORBA::ULong protocol_type {};
};

// Parse command-line arguments.
int
parse_args (int argc, ACE_TCHAR *argv[], Server_Options &options)
{
  ACE_Get_Opt get_opts (argc, argv, ACE_TEXT("s:c:p:"));
  int c {};
  int result {};

  while ((c = get_opts ()) != -1)
    switch (c)
      {
      case 's':
        options.server_policy_ior = get_opts.opt_arg ();
        break;

      case 'c':
        options.default_policy_ior = get_opts.opt_arg ();
        break;

      case 'p':
        result = std::sscanf (ACE_TEXT_ALWAYS_CHAR (get_opts.opt_arg ()),
                              "%u",
                              &options.protocol_type);
        if (result == 0 || result == EOF)
          ACE_ERROR_RETURN ((LM_ERROR,
                             "Unable to process <-p> option"),
                            -1);
        break;

      case '?':
      default:
        ACE_ERROR_RETURN ((LM_ERROR,
                           "usage:  %s "
                           "-s <iorfile> "
                           "-c <iorfile> "
                           "-p <protocol_type> "
                           "\n",
                           argv [0]),
                          -1);
      }

  return 0;
}

int
check_for_nil (CORBA::Object_ptr obj, char const *msg)
{
  if (CORBA::is_nil (obj))
    ACE_ERROR_RETURN ((LM_ERROR,
                       "ERROR: Object reference <%C> is nil\n",
                       msg),
                      -1);
  else
    return 0;
}

int
create_object (PortableServer::POA_ptr poa,
               CORBA::ORB_ptr orb,
               Test_i * const server_impl,
               ACE_TCHAR const *filename)
{
  // Register with poa.
  PortableServer::ObjectId_var id =
    poa->activate_object (server_impl);

  CORBA::Object_var server =
    poa->id_to_reference (id.in ());

  // Print out the IOR.
  CORBA::String_var ior =
    orb->object_to_string (server.in ());

  ACE_DEBUG ((LM_DEBUG, "<%C>\n\n", ior.in ()));

  // Print ior to the file.
  if (filename != nullptr)
    {
      std::unique_ptr<FILE, int (*) (FILE *)> output_file (
        ACE_OS::fopen (filename, "w"), ACE_OS::fclose);
      if (!output_file)
        ACE_ERROR_RETURN ((LM_ERROR,
                           "Cannot open output file for writing IOR: %s",
                           filename),
                          -1);
      ACE_OS::fprintf (output_file.get (), "%s", ior.in ());
    }

  return 0;
}

int
ACE_TMAIN(int argc, ACE_TCHAR *argv[])
{
  try
    {
      // ORB.
      CORBA::ORB_var orb =
        CORBA::ORB_init (argc, argv);

      // Parse arguments.
      Server_Options options;
      if (parse_args (argc, argv, options) != 0)
        return -1;

      // RTORB.
      CORBA::Object_var object =
        orb->resolve_initial_references ("RTORB");
      RTCORBA::RTORB_var const rt_orb = RTCORBA::RTORB::_narrow (object.in ());
      if (check_for_nil (rt_orb.in (), "RTORB") == -1)
        return -1;

      // RootPOA.
      object =
        orb->resolve_initial_references("RootPOA");
      PortableServer::POA_var const root_poa =
        PortableServer::POA::_narrow (object.in ());
      if (check_for_nil (root_poa.in (), "RootPOA") == -1)
        return -1;

      // POAManager.
      PortableServer::POAManager_var const poa_manager =
        root_poa->the_POAManager ();

      // Create child POA with RTCORBA::ClientProtocolPolicy set.
      RTCORBA::ProtocolList protocols;
      protocols.length (1);
      protocols[0].protocol_type = options.protocol_type;
      protocols[0].orb_protocol_properties =
        TAO_Protocol_Properties_Factory::create_orb_protocol_property
        (options.protocol_type);
      protocols[0].transport_protocol_properties =
        TAO_Protocol_Properties_Factory::create_transport_protocol_property
        (options.protocol_type, orb->orb_core ());

      CORBA::PolicyList poa_policy_list;
      poa_policy_list.length (1);
      poa_policy_list[0] =
        rt_orb->create_client_protocol_policy (protocols);

      PortableServer::POA_var const poa =
        root_poa->create_POA ("Child_POA",
                              poa_manager.in (),
                              poa_policy_list);

      // Servant.
      Test_i server_impl (orb.in ());

      // Create object 1.
      int result {};
      ACE_DEBUG ((LM_DEBUG, "\nActivated object one as "));
      result = create_object (poa.in (), orb.in (), &server_impl,
                              options.server_policy_ior);
      if (result == -1)
        return -1;

      // Create object 2.
      ACE_DEBUG ((LM_DEBUG, "\nActivated object two as "));
      result = create_object (root_poa.in (), orb.in (), &server_impl,
                              options.default_policy_ior);
      if (result == -1)
        return -1;

      // Run ORB Event loop.
      poa_manager->activate ();

      orb->run ();

      ACE_DEBUG ((LM_DEBUG, "Server ORB event loop finished\n\n"));
    }
  catch (CORBA::Exception const &ex)
    {
      ex._tao_print_exception (
        "Unexpected exception caught in ClientProtocolPolicy: test server");
      return -1;
    }

  return 0;
}
