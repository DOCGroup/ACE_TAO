
#include "ace/Task.h"
#include "ace/Mutex.h"
#include "ace/Get_Opt.h"
#include "ace/SString.h"
#include "orbsvcs/PortableGroup/GOA.h"
#include "tao/Policy_Manager.h"
#include "tao/DiffServPolicy/DiffServPolicy.h"
#include "tao/DiffServPolicy/Client_Network_Priority_Policy.h"
#include "tao/Utils/PolicyList_Destroyer.h"
#include "HelloC.h"

constexpr int CLIENT_SLEEP_TIME { 1000 };
constexpr int CLIENT_THREAD_NUMBER { 32 };
constexpr CORBA::Short HELLO_CALL_NUMBER { 100 };

constexpr TAO::DiffservCodepoint IPDSFIELD_DSCP_AF22 { 0x14 };
constexpr TAO::DiffservCodepoint IPDSFIELD_DSCP_AF23 { 0x16 };

struct Client_Options
{
  ACE_TString ior_file;
};

void
test_sleep (int microsec)
{
  ACE_Time_Value const tv (microsec / 1000000, microsec % 1000000);

  ACE_OS::sleep (tv);
}

int
parse_args (int argc, ACE_TCHAR *argv[], Client_Options &options)
{
  ACE_Get_Opt get_opts (argc, argv, ACE_TEXT ("k:"));
  int c {};
  while ((c = get_opts ()) != -1)
    {
      switch (c)
        {
        case 'k':
          options.ior_file = get_opts.opt_arg ();
          break;
        case '?':
        default:
          ACE_ERROR_RETURN ((LM_ERROR,
                             ACE_TEXT ("usage: %s -k <iorfile>\n"),
                             argv [0]),
                            -1);
        }
    }

  if (options.ior_file.length () == 0)
    ACE_ERROR_RETURN ((LM_ERROR,
                       ACE_TEXT ("usage: %s -k <iorfile>\n"),
                       argv [0]),
                      -1);

  return 0;
}

class ClientThread
  : public ACE_Task_Base
{
public:
  ClientThread (Test::Hello_ptr hello, CORBA::Short calls)
    : hello_ (Test::Hello::_duplicate (hello))
    , call_count_ (calls)
    , count_ {}
  {
  }

  ClientThread (ClientThread const &) = delete;
  ClientThread (ClientThread &&) = delete;
  ClientThread &operator= (ClientThread const &) = delete;
  ClientThread &operator= (ClientThread &&) = delete;

  int svc () override
  {
    try
      {
        while (true)
          {
            CORBA::Short c {};
            {
              ACE_GUARD_RETURN (ACE_Mutex, ace_mon, this->mutex_, 0);
              c = this->count_++;
            }

            if (c >= this->call_count_)
              break;

            this->hello_->say_hello (c);

            ACE_DEBUG ((LM_DEBUG,
                        ACE_TEXT ("%02d>>; "),
                        c));

            test_sleep (CLIENT_SLEEP_TIME);
          }
      }
    catch (CORBA::Exception const &ex)
      {
        ex._tao_print_exception ("Exception caught in ClientThread:");
        return -1;
      }

    return 0;
  }

private:
  ACE_Mutex mutex_;
  Test::Hello_var hello_;
  CORBA::Short const call_count_;
  CORBA::Short count_;
};

int
ACE_TMAIN (int argc, ACE_TCHAR *argv[])
{
  try
    {
      CORBA::ORB_var orb =
        CORBA::ORB_init (argc, argv);

      Client_Options options;
      if (parse_args (argc, argv, options) != 0)
        return 2;

      CORBA::Object_var miop_ref =
        orb->string_to_object (options.ior_file.c_str ());

      // create Hello reference
      Test::Hello_var hello =
        Test::Hello::_unchecked_narrow (miop_ref.in ());

      if (CORBA::is_nil (hello.in ()))
        return 5;

      TAO::DiffservCodepoint const request_dscp = IPDSFIELD_DSCP_AF22;
      TAO::DiffservCodepoint const reply_dscp = IPDSFIELD_DSCP_AF23;

      CORBA::Object_var over_ridden_object;
      {
        TAO::Utils::PolicyList_Destroyer policy_list (1);
        policy_list.length (1);

        CORBA::Policy_var client_network_policy =
          orb->_create_policy (TAO::CLIENT_NETWORK_PRIORITY_TYPE);

        TAO::NetworkPriorityPolicy_var nw_priority =
          TAO::NetworkPriorityPolicy::_narrow (client_network_policy.in ());

        nw_priority->request_diffserv_codepoint (request_dscp);

        nw_priority->reply_diffserv_codepoint (reply_dscp);

        nw_priority->network_priority_model (
          TAO::CLIENT_PROPAGATED_NETWORK_PRIORITY);

        policy_list[0] = TAO::NetworkPriorityPolicy::_duplicate (nw_priority.in ());

        over_ridden_object =
          hello->_set_policy_overrides (policy_list, CORBA::SET_OVERRIDE);
      }

      hello = Test::Hello::_unchecked_narrow (over_ridden_object.in ());

      {
        // start clients
        ClientThread cln_thr (hello.in (), HELLO_CALL_NUMBER);
        cln_thr.activate (THR_NEW_LWP | THR_JOINABLE, CLIENT_THREAD_NUMBER);
        cln_thr.wait ();
      }

      // shutdown the server, after 10 invocations of shutdown() we can be
      // more or less sure that server actually received that call
      for (int i = 0; i < 10; ++i)
        hello->shutdown ();

      orb->destroy ();
    }
  catch (CORBA::Exception const &ex)
    {
      ex._tao_print_exception ("Exception caught in client main ():");
      return 4;
    }

  ACE_DEBUG ((LM_DEBUG,
              ACE_TEXT ("\n (%P|%t) client finished successfully..\n")));
  return 0;
}
