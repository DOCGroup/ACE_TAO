#include "TestC.h"
#include "ace/Get_Opt.h"
#include "ace/Task.h"
#include "ace/Profile_Timer.h"
#include "ace/OS_NS_unistd.h"

namespace Test
{
  struct Client_Options
  {
    ACE_TString ior {ACE_TEXT ("file://server.ior")};
    bool blocked {};
  };

  bool
  parse_args (int argc, ACE_TCHAR *argv[], Client_Options &options)
  {
    ACE_Get_Opt get_opts (argc, argv, ACE_TEXT ("b:k:"));
    int c {};

    while ((c = get_opts ()) != -1)
      switch (c)
        {
        case 'b':
          {
            int const tmp =
              ACE_OS::atoi (get_opts.opt_arg ());
            options.blocked = tmp != 0;
          }
          break;
        case 'k':
          {
            options.ior = get_opts.opt_arg ();
          }
          break;
        case '?':
        default:
          ACE_ERROR_RETURN ((LM_ERROR,
                             "usage:  %s "
                             "-k <ior> "
                             "\n",
                             argv [0]),
                            false);
        }

    // Indicates successful parsing of the command line
    return true;
  }

  class Client_Task : public ACE_Task_Base
  {
  public:
    explicit Client_Task (Hang_ptr h)
      : h_ (Hang::_duplicate (h))
    {}

    Client_Task (Client_Task const &) = delete;
    Client_Task (Client_Task &&) = delete;
    Client_Task &operator= (Client_Task const &) = delete;
    Client_Task &operator= (Client_Task &&) = delete;

    int svc () override
    {
      try
        {
          this->h_->send_stuff ("Testing",
                                false);

          this->h_->send_stuff ("Testing",
                                false);

          this->h_->send_stuff ("Testing",
                                true);
        }
      catch (CORBA::COMM_FAILURE const &f)
        {
          ACE_UNUSED_ARG (f);
          ACE_DEBUG ((LM_DEBUG,
                      "(%P|%t) Caught COMM_FAILURE Exception\n"));

          ACE_DEBUG ((LM_DEBUG,
                      "(%P|%t) This is expected\n"));

          return 0;
        }
      catch (CORBA::Exception const &ex)
        {
          ex._tao_print_exception ("Caught CORBA Exception\n");

          ACE_ERROR ((LM_ERROR,
                      "(%P|%t) Error in test\n"));

          return -1;
        }
      catch (...)
        {
          ACE_DEBUG ((LM_DEBUG,
                      "(%P|%t) Caught a C++ exception\n"));
          ACE_ERROR ((LM_ERROR,
                      "(%P|%t) Error in test\n"));

          return -1;
        }

      return 0;
    }

  private:
    Hang_var h_ {};
  };

  class Shutdown_Task : public ACE_Task_Base
  {
  public:
    Shutdown_Task (CORBA::ORB_ptr o, bool blocked)
      : o_ (CORBA::ORB::_duplicate (o)),
        blocked_ (blocked)
    {}

    Shutdown_Task (Shutdown_Task const &) = delete;
    Shutdown_Task (Shutdown_Task &&) = delete;
    Shutdown_Task &operator= (Shutdown_Task const &) = delete;
    Shutdown_Task &operator= (Shutdown_Task &&) = delete;

    int svc () override
    {
      try
        {
          ACE_DEBUG ((LM_DEBUG,
                      "(%P|%t) Calling shutdown\n"));

          // Just wait for the main thread to start sending out
          // messages
          ACE_OS::sleep (4);

          // Start the timer
          this->profile_timer_.start ();

          this->o_->shutdown (this->blocked_);

          // Stop the timer
          this->profile_timer_.stop ();

          // Get the elampsed time
          ACE_Profile_Timer::ACE_Elapsed_Time el {};
          this->profile_timer_.elapsed_time (el);

          // The elapsed time is in secs
          if (el.real_time > 1)
            {
              ACE_ERROR ((LM_ERROR,
                          "(%P|%t) ERROR: Too long to shutdown\n"));

              return 0;
            }
        }
      catch (...)
        {
          ACE_DEBUG ((LM_DEBUG,
                      "(%P|%t) Caught exception during shutdown\n"));

          ACE_ERROR ((LM_ERROR,
                      "(%P|%t) Error in test\n"));
          return -1;
        }

      ACE_DEBUG ((LM_DEBUG,
                  "(%P|%t) Returning from shutdown\n"));
      return 0;
    }
  private:
    CORBA::ORB_var o_ {};
    bool blocked_ {};
    ACE_Profile_Timer profile_timer_;
  };

  static int
  try_main (int argc, ACE_TCHAR *argv[])
  {
    try
      {
        Client_Options options;
        CORBA::ORB_var const orb =
          CORBA::ORB_init (argc, argv);

        if (!parse_args (argc, argv, options))
          return -1;

        CORBA::Object_var const tmp = orb->string_to_object (
          ACE_TEXT_ALWAYS_CHAR (options.ior.c_str ()));

        Hang_var const test =
          Hang::_narrow (tmp.in ());

        if (CORBA::is_nil (test.in ()))
          {
            ACE_ERROR_RETURN ((LM_DEBUG,
                               ACE_TEXT ("Nil test reference <%s>\n"),
                               options.ior.c_str ()),
                              1);
          }

        Client_Task ct (test.in ());

        if (ct.activate (THR_NEW_LWP | THR_JOINABLE, 1) != 0)
          ACE_ERROR_RETURN ((LM_ERROR,
                             "Cannot activate client threads\n"),
                            1);

        ACE_DEBUG ((LM_DEBUG,
                    "(%P|%t) Activating shutdown thread\n"));

        Shutdown_Task st (orb.in (), options.blocked);

        if (st.activate (THR_NEW_LWP | THR_JOINABLE, 1) != 0)
          ACE_ERROR_RETURN ((LM_ERROR,
                             "Cannot activate shutdown threads\n"),
                            1);

        ACE_Thread_Manager::instance ()->wait ();

        orb->destroy ();
      }
    catch (CORBA::Exception const &ex)
      {
        ex._tao_print_exception ("CORBA Exception caught\n");
        ACE_ERROR ((LM_ERROR,
                    "(%P|%t) Eror in test\n"));
        return -1;
      }
    catch (...)
      {
        ACE_DEBUG ((LM_DEBUG,
                    "(%P|%t) Error in test\n"));
        return -1;
      }

    return 0;
  }
}

int
ACE_TMAIN(int argc, ACE_TCHAR *argv[])
{
  return Test::try_main (argc, argv);
}
