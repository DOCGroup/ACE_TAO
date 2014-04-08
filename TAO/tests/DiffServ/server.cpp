// $Id$

#include "testS.h"
#include "ace/Get_Opt.h"
#include "ace/OS_NS_stdio.h"
#include "tao/debug.h"
#include "tao/DiffServPolicy/DiffServPolicy.h"
#include "tao/DiffServPolicy/Server_Network_Priority_Policy.h"
#include "tao/DiffServPolicy/Client_Network_Priority_Policy.h"

#define IPDSFIELD_DSCP_DEFAULT  0x00
#define IPDSFIELD_DSCP_CS1      0x08
#define IPDSFIELD_DSCP_CS2      0x10
#define IPDSFIELD_DSCP_CS3      0x18
#define IPDSFIELD_DSCP_CS4      0x20
#define IPDSFIELD_DSCP_CS5      0x28
#define IPDSFIELD_DSCP_CS6      0x30
#define IPDSFIELD_DSCP_CS7      0x38
#define IPDSFIELD_DSCP_AF11     0x0A
#define IPDSFIELD_DSCP_AF12     0x0C
#define IPDSFIELD_DSCP_AF13     0x0E
#define IPDSFIELD_DSCP_AF21     0x12
#define IPDSFIELD_DSCP_AF22     0x14
#define IPDSFIELD_DSCP_AF23     0x16
#define IPDSFIELD_DSCP_AF31     0x1A
#define IPDSFIELD_DSCP_AF32     0x1C
#define IPDSFIELD_DSCP_AF33     0x1E
#define IPDSFIELD_DSCP_AF41     0x22
#define IPDSFIELD_DSCP_AF42     0x24
#define IPDSFIELD_DSCP_AF43     0x26
#define IPDSFIELD_ECT_MASK      0x02
#define IPDSFIELD_CE_MASK       0x01
#define IPDSFIELD_DSCP_EF       0x2E

static int const dscp[] =
{
  IPDSFIELD_DSCP_DEFAULT ,
  IPDSFIELD_DSCP_CS1     ,
  IPDSFIELD_DSCP_CS2     ,
  IPDSFIELD_DSCP_CS3     ,
  IPDSFIELD_DSCP_CS4     ,
  IPDSFIELD_DSCP_CS5     ,
  IPDSFIELD_DSCP_CS6     ,
  IPDSFIELD_DSCP_CS7     ,
  IPDSFIELD_DSCP_AF11    ,
  IPDSFIELD_DSCP_AF12    ,
  IPDSFIELD_DSCP_AF13    ,
  IPDSFIELD_DSCP_AF21    ,
  IPDSFIELD_DSCP_AF22    ,
  IPDSFIELD_DSCP_AF23    ,
  IPDSFIELD_DSCP_AF31    ,
  IPDSFIELD_DSCP_AF32    ,
  IPDSFIELD_DSCP_AF33    ,
  IPDSFIELD_DSCP_AF41    ,
  IPDSFIELD_DSCP_AF42    ,
  IPDSFIELD_DSCP_AF43    ,
  IPDSFIELD_DSCP_EF
};


class Test_i : public POA_Test
{
public:
  Test_i (CORBA::ORB_ptr orb);

  void test_method (void);

  //FUZZ: disable check_for_lack_ACE_OS
  void shutdown (void);
  //FUZZ: enable check_for_lack_ACE_OS

private:
  CORBA::ORB_var orb_;
};

Test_i::Test_i (CORBA::ORB_ptr orb)
  : orb_ (CORBA::ORB::_duplicate (orb))
{
}

void
Test_i::test_method (void)
{
  ACE_DEBUG ((LM_DEBUG,
              "Test_i::test_method\n"));
}

void
Test_i::shutdown (void)
{
  this->orb_->shutdown (0);
}

static const ACE_TCHAR *simple_servant_ior_file = ACE_TEXT("simple_servant.ior");

int
parse_args (int argc, ACE_TCHAR *argv[])
{
  ACE_Get_Opt get_opts (argc, argv, ACE_TEXT("o:"));
  int c;

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

void
create_object (PortableServer::POA_ptr poa,
               CORBA::ORB_ptr orb,
               Test_i *servant,
               const ACE_TCHAR *filename)
{
  // Register with poa.
  PortableServer::ObjectId_var id =
    poa->activate_object (servant);

  CORBA::Object_var object =
    poa->id_to_reference (id.in ());

  CORBA::String_var ior =
    orb->object_to_string (object.in ());

  ACE_DEBUG ((LM_DEBUG, "<%C>\n\n", ior.in ()));

  FILE *output_file= ACE_OS::fopen (filename, "w");
  ACE_OS::fprintf (output_file, "%s", ior.in ());
  ACE_OS::fclose (output_file);
}

int
ACE_TMAIN(int argc, ACE_TCHAR *argv[])
{
  try
    {
      CORBA::ORB_var orb =
        CORBA::ORB_init (argc, argv);

      if (parse_args (argc, argv) != 0)
        return 1;

      // RootPOA.
      CORBA::Object_var object =
        orb->resolve_initial_references ("RootPOA");

      PortableServer::POA_var root_poa =
        PortableServer::POA::_narrow (object.in ());

      PortableServer::POAManager_var poa_manager =
        root_poa->the_POAManager ();

      TAO::DiffservCodepoint request_dscp;
      TAO::DiffservCodepoint reply_dscp;
      int request_array_slot = 3;
      int reply_array_slot = 2;
      long request_dscp_codepoint = dscp[request_array_slot];
      long reply_dscp_codepoint = dscp[reply_array_slot];
      request_dscp = request_dscp_codepoint;
      reply_dscp = reply_dscp_codepoint;

      TAO::NetworkPriorityModel npm = TAO::CLIENT_PROPAGATED_NETWORK_PRIORITY;

      CORBA::PolicyList policy_list;
      policy_list.length (1);

      CORBA::Policy_var npp =
        orb->_create_policy (TAO::NETWORK_PRIORITY_TYPE);

      TAO::NetworkPriorityPolicy_var nw_priority =
        TAO::NetworkPriorityPolicy::_narrow (npp.in ());

      nw_priority->network_priority_model (npm);

      nw_priority->request_diffserv_codepoint (request_dscp);

      nw_priority->reply_diffserv_codepoint (reply_dscp);

      policy_list[0] = TAO::NetworkPriorityPolicy::_duplicate (nw_priority.in ());

      PortableServer::POA_var child_poa =
        root_poa->create_POA ("Child_POA",
                              poa_manager.in (),
                              policy_list);

      policy_list[0]->destroy ();

      // Servant.
      Test_i servant (orb.in ());

      // Create the first object in Root POA
      create_object (child_poa.in (),
                     orb.in (),
                     &servant,
                     simple_servant_ior_file);

      // Activate POA manager.
      poa_manager->activate ();

      // Start ORB event loop.
      orb->run ();

      ACE_DEBUG ((LM_DEBUG, "Server ORB event loop finished\n\n"));

      root_poa->destroy (1, 1);

      orb->destroy ();
    }
  catch (const CORBA::Exception& ex)
    {
      ex._tao_print_exception ("Unexpected exception caught:");
      return -1;
    }

  return 0;
}
