#include "testS.h"
#include "ace/Get_Opt.h"
#include "ace/OS_NS_stdio.h"
#include "tao/debug.h"
#include "tao/DiffServPolicy/DiffServPolicy.h"
#include "tao/DiffServPolicy/Server_Network_Priority_Policy.h"
#include "tao/DiffServPolicy/Client_Network_Priority_Policy.h"
#include <memory>

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

int
parse_args (int argc, ACE_TCHAR *argv[], ACE_TString &simple_servant_ior_file)
{
  ACE_Get_Opt get_opts (argc, argv, ACE_TEXT("o:"));
  int c {};

  while ((c = get_opts ()) != -1)
    switch (c)
      {
      case 'o':
        simple_servant_ior_file = get_opts.opt_arg ();
        break;
      }
  // Indicates successful parsing of the command line
  return 0;
}

int
create_object (PortableServer::POA_ptr poa,
               CORBA::ORB_ptr orb,
               Test_i &servant,
               ACE_TCHAR const *filename)
{
  // Register with poa.
  PortableServer::ObjectId_var id =
    poa->activate_object (std::addressof (servant));

  CORBA::Object_var const object =
    poa->id_to_reference (id.in ());

  CORBA::String_var const ior =
    orb->object_to_string (object.in ());

  ACE_DEBUG ((LM_DEBUG, "<%C>\n\n", ior.in ()));

  std::unique_ptr<FILE, int (*) (FILE *)> output_file (
    ACE_OS::fopen (filename, "w"), ACE_OS::fclose);
  if (!output_file)
    ACE_ERROR_RETURN ((LM_ERROR,
                       "Cannot open output file for writing IOR: %s",
                       filename),
                      -1);
  ACE_OS::fprintf (output_file.get (), "%s", ior.in ());
  return 0;
}

int
ACE_TMAIN (int argc, ACE_TCHAR *argv[])
{
  try
    {
      CORBA::ORB_var orb =
        CORBA::ORB_init (argc, argv);

      ACE_TString simple_servant_ior_file (ACE_TEXT("simple_servant.ior"));
      if (parse_args (argc, argv, simple_servant_ior_file) != 0)
        return 1;

      // RootPOA.
      CORBA::Object_var const object =
        orb->resolve_initial_references ("RootPOA");

      PortableServer::POA_var root_poa =
        PortableServer::POA::_narrow (object.in ());

      PortableServer::POAManager_var poa_manager =
        root_poa->the_POAManager ();

      constexpr TAO::DiffservCodepoint dscp_cs3 {0x18};
      constexpr TAO::DiffservCodepoint dscp_cs2 {0x10};

      TAO::NetworkPriorityModel const npm = TAO::CLIENT_PROPAGATED_NETWORK_PRIORITY;

      CORBA::PolicyList policy_list;
      policy_list.length (1);

      CORBA::Policy_var const npp =
        orb->_create_policy (TAO::NETWORK_PRIORITY_TYPE);

      TAO::NetworkPriorityPolicy_var const nw_priority =
        TAO::NetworkPriorityPolicy::_narrow (npp.in ());

      nw_priority->network_priority_model (npm);

      nw_priority->request_diffserv_codepoint (dscp_cs3);

      nw_priority->reply_diffserv_codepoint (dscp_cs2);

      policy_list[0] = TAO::NetworkPriorityPolicy::_duplicate (nw_priority.in ());

      PortableServer::POA_var child_poa =
        root_poa->create_POA ("Child_POA",
                              poa_manager.in (),
                              policy_list);

      policy_list[0]->destroy ();

      // Servant.
      Test_i servant (orb.in ());

      // Create the first object in Root POA
      if (create_object (child_poa.in (),
                         orb.in (),
                         servant,
                         simple_servant_ior_file.c_str ()) != 0)
        return 1;

      // Activate POA manager.
      poa_manager->activate ();

      // Start ORB event loop.
      orb->run ();

      ACE_DEBUG ((LM_DEBUG, "Server ORB event loop finished\n\n"));

      root_poa->destroy (true, true);

      orb->destroy ();
    }
  catch (const CORBA::Exception& ex)
    {
      ex._tao_print_exception ("Unexpected exception caught:");
      return -1;
    }

  return 0;
}
