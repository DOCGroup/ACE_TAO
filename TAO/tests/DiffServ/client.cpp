#include "testC.h"
#include "ace/Get_Opt.h"
#include "tao/Policy_Manager.h"
#include "tao/DiffServPolicy/DiffServPolicy.h"
#include "tao/DiffServPolicy/Client_Network_Priority_Policy.h"
#include "tao/DiffServPolicy/Server_Network_Priority_Policy.h"

constexpr TAO::DiffservCodepoint dscp_af22 {0x14};
constexpr TAO::DiffservCodepoint dscp_af23 {0x16};

int
parse_args (int argc, ACE_TCHAR *argv[], ACE_TString &ior, int &iterations)
{
  ACE_Get_Opt get_opts (argc, argv, ACE_TEXT("k:n:"));
  int c {};

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
                           ior.c_str (),
                           iterations),
                          -1);
      }

  return 0;
}

int
ACE_TMAIN (int argc, ACE_TCHAR *argv[])
{
  try
    {
      CORBA::ORB_var orb =
        CORBA::ORB_init (argc, argv);

      ACE_TString ior (ACE_TEXT("file://simple_servant.ior"));
      int iterations {1};
      if (parse_args (argc, argv, ior, iterations) != 0)
        return -1;

      // Initialize and obtain reference to the Test object.
      CORBA::Object_var const client_object =
        orb->string_to_object (ior.c_str ());

      Test_var server =
        Test::_narrow (client_object.in ());

      CORBA::PolicyList policy_list;
      policy_list.length (1);

      CORBA::Policy_var const client_network_policy =
        orb->_create_policy (TAO::CLIENT_NETWORK_PRIORITY_TYPE);

      TAO::NetworkPriorityPolicy_var const nw_priority =
        TAO::NetworkPriorityPolicy::_narrow (client_network_policy.in ());

      nw_priority->request_diffserv_codepoint (
           dscp_af22);

      nw_priority->reply_diffserv_codepoint (
           dscp_af23);

      nw_priority->network_priority_model (
        TAO::CLIENT_PROPAGATED_NETWORK_PRIORITY);

      policy_list[0] = TAO::NetworkPriorityPolicy::_duplicate (nw_priority.in ());

      CORBA::Object_var const over_ridden_object = server->_set_policy_overrides (
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
