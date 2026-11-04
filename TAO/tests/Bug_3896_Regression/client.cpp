#include "TestC.h"
#include "ace/Get_Opt.h"
#include "ace/Task_T.h"
#include "ace/OS_NS_unistd.h"
#include "tao/TimeBaseC.h"
#include "tao/Messaging/Messaging.h"

#include <string>

int
parse_args (int argc,
            ACE_TCHAR *argv[],
            std::basic_string<ACE_TCHAR> &ior,
            bool &shutdown_server)
{
  ACE_Get_Opt get_opts (argc, argv, ACE_TEXT ("k:x"));
  int option {};

  while ((option = get_opts ()) != -1)
    switch (option)
      {
      case 'k':
        ior = get_opts.opt_arg ();
        break;
      case 'x':
        shutdown_server = true;
        break;

      case '?':
      default:
        ACE_ERROR_RETURN ((LM_ERROR,
                           "usage:  %s "
                           "-k <ior> "
                           "\n",
                           argv [0]),
                          -1);
      }
  // Indicates successful parsing of the command line
  return 0;
}

class Client : public ACE_Task_Base
{
public:
  explicit Client (CORBA::ORB_ptr const server);
  Client (Client const &) = delete;
  Client (Client &&) = delete;
  Client &operator= (Client const &) = delete;
  Client &operator= (Client &&) = delete;

  int svc () override;

private:
  CORBA::ORB_var orb_ {};
};

int
ACE_TMAIN (int argc, ACE_TCHAR *argv[])
{
  try
    {
      CORBA::ORB_var const orb {CORBA::ORB_init (argc, argv)};

      std::basic_string<ACE_TCHAR> ior {ACE_TEXT ("file://test.ior")};
      bool shutdown_server {};
      if (parse_args (argc, argv, ior, shutdown_server) != 0)
        return 1;

      constexpr int timeout_secs = 5;
      CORBA::Object_var const obj = orb->resolve_initial_references ("ORBPolicyManager");
      CORBA::PolicyManager_var const policy_manager =
        CORBA::PolicyManager::_narrow (obj.in ());

      CORBA::PolicyList policy_list;
      policy_list.length (0);
      policy_manager->set_policy_overrides (policy_list, CORBA::SET_OVERRIDE);

      TimeBase::TimeT const timeout = timeout_secs * 10000000;
      CORBA::Any orb_timeout;
      orb_timeout <<= timeout;
      policy_list.length (1);
      policy_list[0] = orb->create_policy (
        Messaging::RELATIVE_RT_TIMEOUT_POLICY_TYPE, orb_timeout);

      policy_manager->set_policy_overrides (policy_list, CORBA::SET_OVERRIDE);

      CORBA::Object_var const tmp =
        orb->string_to_object (ACE_TEXT_ALWAYS_CHAR (ior.c_str ()));

      Test::Hello_var const hello = Test::Hello::_narrow (tmp.in ());

      if (CORBA::is_nil (hello.in ()))
        {
          ACE_ERROR_RETURN ((LM_DEBUG,
                             "Nil Test::Hello reference <%s>\n",
                             ior.c_str ()),
                            1);
        }

      if (shutdown_server)
        {
          ACE_DEBUG ((LM_DEBUG,
                     "(%P|%t) Calling shutdown on server\n"));
          hello->shutdown ();
        }
      else
        {
          Client client (orb.in ());
          if (client.activate (THR_NEW_LWP | THR_JOINABLE, 1) != 0)
            ACE_ERROR_RETURN ((LM_ERROR,
                              "Cannot activate client threads\n"),
                              1);

          CORBA::String_var the_string {};
          try
            {
              the_string = hello->get_string ();
            }
            catch (CORBA::TIMEOUT const &)
            {
              ACE_DEBUG ((LM_DEBUG,
                          "(%P|%t) Error: got timeout\n"));
            }
            catch (CORBA::COMM_FAILURE const &)
            {
              ACE_DEBUG ((LM_DEBUG,
                          "(%P|%t) Ok: got comm exception\n"));
            }

          ACE_DEBUG ((LM_DEBUG, "(%P|%t) - string returned <%C>\n",
                      the_string.in ()));

          client.thr_mgr ()->wait ();
        }

      orb->destroy ();
    }
  catch (CORBA::Exception const &ex)
    {
      ex._tao_print_exception ("Exception caught:");
      return 1;
    }

  return 0;
}

Client::Client (CORBA::ORB_ptr const orb)
  : orb_ (CORBA::ORB::_duplicate (orb))
{
}

int
Client::svc ()
{
  try
    {
      ACE_OS::sleep (2);
      ACE_DEBUG ((LM_DEBUG,
                  "(%P|%t) Shutting down ORB\n"));
      this->orb_->shutdown (true);
    }
  catch (CORBA::Exception const &)
    {
    }
  return 0;
}
