#include "TestC.h"
#include <atomic>
#include <string>

#include "ace/Get_Opt.h"
#include "ace/Task.h"
#include "ace/OS_NS_unistd.h"

class Pinger : public ACE_Task_Base
{
private:
  std::string ior_;
  CORBA::ORB_var orb_;
  std::atomic<bool> do_shutdown_ {};
  std::atomic<bool> stop_ {};

public:
  Pinger (CORBA::ORB_ptr orb, std::string const &ior)
    : ior_ (ior)
    , orb_ (CORBA::ORB::_duplicate (orb))
  {
  }

  Pinger (Pinger const &) = delete;
  Pinger &operator= (Pinger const &) = delete;
  Pinger (Pinger &&) = delete;
  Pinger &operator= (Pinger &&) = delete;

  int svc () override
  {
    bool keep_going {true};
    while (keep_going && !this->stop_.load ())
      {
        try
          {
            CORBA::Object_var const tmp = this->orb_->string_to_object (this->ior_.c_str ());

            Test::IORTable_Shutdown_Race_var const target =
              Test::IORTable_Shutdown_Race::_narrow (tmp.in ());
            if (CORBA::is_nil (target.in ()))
                ACE_ERROR_RETURN ((LM_DEBUG,
                                   "(%P|%t) Nil target reference <%s>\n",
                                   this->ior_.c_str ()),
                                  1);
            target->ping ();
            if (this->do_shutdown_.exchange (false))
              {
                ACE_DEBUG ((LM_DEBUG, "(%P|%t) Calling shutdown\n"));
                target->shutdown ();
              }
          }
        catch (CORBA::Exception const &ex)
          {
            ACE_DEBUG ((LM_DEBUG,
                        "(%P|%t) caught an exception - %C\n", ex._name ()));
            keep_going = false;
          }
      }
    return 0;
  }

  void killit ()
  {
    this->do_shutdown_.store (true);
  }

  void stop ()
  {
    this->stop_.store (true);
  }
};

int
parse_args (int argc, ACE_TCHAR *argv[], int &port, ACE_TCHAR const *&target_host)
{
  ACE_Get_Opt get_opts (argc, argv, ACE_TEXT("p:h:"));
  int c {};

  while ((c = get_opts ()) != -1)
    switch (c)
      {
      case 'p':
        port = ACE_OS::atoi (get_opts.opt_arg ());
        break;
      case 'h':
        target_host = get_opts.opt_arg ();
        break;
      case '?':
      default:
        ACE_ERROR_RETURN ((LM_ERROR,
                           "usage:  %s "
                           "-p <server_port> "
                           "\n",
                           argv [0]),
                          -1);
      }
  // Indicates successful parsing of the command line
  return 0;
}

int
ACE_TMAIN (int argc, ACE_TCHAR *argv[])
{
  try
    {
      CORBA::ORB_var const orb =
        CORBA::ORB_init (argc, argv);
      int port {};
      // Any option value points into argv, which stays live for this process.
      ACE_TCHAR const *target_host = ACE_TEXT ("localhost");

      if (parse_args (argc, argv, port, target_host) != 0)
        return 1;

      std::string ior ("corbaloc::");
      ior += ACE_TEXT_ALWAYS_CHAR (target_host);
      ior += ':';
      ior += std::to_string (port);
      ior += "/Racer";

      Pinger pinger (orb.in (), ior);

      ACE_OS::sleep (1);

      ACE_DEBUG ((LM_DEBUG, "(%P|%t) client - starting client threads\n"));

      pinger.activate (THR_NEW_LWP | THR_JOINABLE, 5);

      ACE_OS::sleep (1);

      ACE_DEBUG ((LM_DEBUG, "(%P|%t) client - All running, time to shutdown server\n"));
      pinger.killit ();

      ACE_OS::sleep (2);

      ACE_DEBUG ((LM_DEBUG, "(%P|%t) client - Stopping client threads\n"));
      pinger.stop ();

      pinger.wait ();
      ACE_DEBUG ((LM_DEBUG, "(%P|%t) client done\n"));
    }
  catch (CORBA::Exception const &ex)
    {
      ACE_DEBUG ((LM_DEBUG, "Main caught %s\n", ex._name ()));
      return 1;
    }

  return 0;
}
