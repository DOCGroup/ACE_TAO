// $Id$

#include "TestC.h"
#include "ace/Get_Opt.h"
#include "ace/Task.h"
#include "ace/OS_NS_unistd.h"

class Pinger : public ACE_Task_Base
{
private:
  const char * ior_;
  CORBA::ORB_var orb_;
  bool do_shutdown_;
  bool stop_;

public:
  Pinger (CORBA::ORB_var &orb, const char *ior)
    : ior_ (ior),
      orb_(orb),
      do_shutdown_ (false),
      stop_ (false)
  {
  }

  int svc (void)
  {
    bool keep_going = true;
    while (keep_going && !this->stop_)
      {
        try
          {
            CORBA::Object_var tmp = this->orb_->string_to_object(this->ior_);

            Test::IORTable_Shutdown_Race_var target =
              Test::IORTable_Shutdown_Race::_narrow(tmp.in ());
            if (CORBA::is_nil (target.in ()))
                ACE_ERROR_RETURN ((LM_DEBUG,
                                   "(%P|%t) Nil target reference <%s>\n",
                                   this->ior_),
                                  1);
            target->ping();
            if (this->do_shutdown_)
              {
                ACE_DEBUG ((LM_DEBUG,"(%P|%t) Calling shutdown\n"));
                this->do_shutdown_ = false;
                target->shutdown ();
              }
          }
        catch (CORBA::Exception &ex)
          {
            ACE_DEBUG ((LM_DEBUG,
                        "(%P|%t) caught an exception - %C\n",ex._name()));
            keep_going = false;
          }
      }
    return 0;
  }

  void killit()
  {
    do_shutdown_ = true;
  }

  void stop ()
  {
    stop_ = true;
  }

};

int port = 0;
ACE_TCHAR const * target_host = ACE_TEXT("localhost");

int
parse_args (int argc, ACE_TCHAR *argv[])
{
  ACE_Get_Opt get_opts (argc, argv, ACE_TEXT("p:h:"));
  int c;

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
ACE_TMAIN(int argc, ACE_TCHAR *argv[])
{
  try
    {
      CORBA::ORB_var orb =
        CORBA::ORB_init (argc, argv);

      if (parse_args (argc, argv) != 0)
        return 1;

      char ior[100];
      ACE_OS::sprintf (ior, "corbaloc::%s:%d/Racer",
                       ACE_TEXT_ALWAYS_CHAR (target_host), port);

      Pinger pinger(orb, ior);

      ACE_OS::sleep (1);

      ACE_DEBUG ((LM_DEBUG,"(%P|%t) client - starting client threads\n"));

      pinger.activate (THR_NEW_LWP | THR_JOINABLE, 5);

      ACE_OS::sleep (1);

      ACE_DEBUG ((LM_DEBUG,"(%P|%t) client - All running, time to shutdown server\n"));
      pinger.killit();

      ACE_OS::sleep (2);

      ACE_DEBUG ((LM_DEBUG,"(%P|%t) client - Stopping client threads\n"));
      pinger.stop ();

      pinger.wait();
      ACE_DEBUG ((LM_DEBUG,"(%P|%t) client done\n"));

    }
  catch (CORBA::Exception &ex)
    {
      ACE_DEBUG ((LM_DEBUG,"Main caught %s\n",ex._name()));
      return 1;
    }

  return 0;
}
