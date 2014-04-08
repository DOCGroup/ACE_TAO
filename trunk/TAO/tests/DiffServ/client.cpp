// $Id$

#include "testC.h"
#include "ace/Get_Opt.h"
#include "tao/Policy_Manager.h"
#include "tao/DiffServPolicy/DiffServPolicy.h"
#include "tao/DiffServPolicy/Client_Network_Priority_Policy.h"
#include "tao/DiffServPolicy/Server_Network_Priority_Policy.h"

static const ACE_TCHAR *ior = ACE_TEXT("file://simple_servant.ior");
static int iterations = 1;

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

int
parse_args (int argc, ACE_TCHAR *argv[])
{
  ACE_Get_Opt get_opts (argc, argv, ACE_TEXT("k:n:x:"));
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

      default:
        ACE_ERROR_RETURN ((LM_ERROR,
                           "usage:  %s\n"
                           "\t-k <ior> [default is %s]\n"
                           "\t-n <number of invocations> [defaults to %d]\n"
                           "\n",
                           argv [0],
                           ior,
                           iterations),
                          -1);
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

      // Initialize and obtain reference to the Test object.
      CORBA::Object_var client_object =
        orb->string_to_object (ior);

      Test_var server =
        Test::_narrow (client_object.in ());

      TAO::DiffservCodepoint request_dscp;
      TAO::DiffservCodepoint reply_dscp;
      int request_array_slot = 12;
      int reply_array_slot = 13;
      long request_dscp_codepoint = dscp[request_array_slot];
      long reply_dscp_codepoint = dscp[reply_array_slot];
      request_dscp = request_dscp_codepoint;
      reply_dscp = reply_dscp_codepoint;

      CORBA::PolicyList policy_list;
      policy_list.length (1);

      CORBA::Policy_var client_network_policy =
        orb->_create_policy (TAO::CLIENT_NETWORK_PRIORITY_TYPE);

      TAO::NetworkPriorityPolicy_var nw_priority =
        TAO::NetworkPriorityPolicy::_narrow (client_network_policy.in ());

      nw_priority->request_diffserv_codepoint (
           request_dscp);

      nw_priority->reply_diffserv_codepoint (
           reply_dscp);

      nw_priority->network_priority_model (
        TAO::CLIENT_PROPAGATED_NETWORK_PRIORITY);

      policy_list[0] = TAO::NetworkPriorityPolicy::_duplicate (nw_priority.in ());

      CORBA::Object_var over_ridden_object = server->_set_policy_overrides (
         policy_list, CORBA::SET_OVERRIDE);

      policy_list[0]->destroy ();

      server = Test::_narrow (over_ridden_object.in ());

      // Make several invocation,
      for (int i = 0; i < iterations; ++i)
        {
          server->test_method ();
        }

      ACE_DEBUG ((LM_DEBUG, "(%P|%t) Calling shutdown on server\n"));

      server->shutdown ();

      ACE_DEBUG ((LM_DEBUG, "(%P|%t) Destroying ORB\n"));

      orb->destroy ();
    }
  catch (const CORBA::Exception& ex)
    {
      ex._tao_print_exception ("Caught exception:");
      return -1;
    }

  return 0;
}
