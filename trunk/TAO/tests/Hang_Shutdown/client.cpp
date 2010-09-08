// $Id$

#include "TestC.h"
#include "ace/Get_Opt.h"
#include "ace/OS.h"
#include "ace/Task.h"
#include "ace/Profile_Timer.h"

namespace Test
{
  const ACE_TCHAR *ior = ACE_TEXT("file://server.ior");

  ACE_Profile_Timer profile_timer;
  bool blocked = false;

  bool
  parse_args (int argc, ACE_TCHAR *argv[])
  {
    ACE_Get_Opt get_opts (argc, argv, ACE_TEXT("b:k:"));
    int c;

    while ((c = get_opts ()) != -1)
      switch (c)
        {
        case 'b':
          {
            int tmp =
              ACE_OS::atoi (get_opts.opt_arg ());

            if (tmp)
              blocked = true;
            else
              blocked = false;
          }
          break;
        case 'k':
          {
            ior = get_opts.opt_arg ();
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

    // Indicates sucessful parsing of the command line
    return true;
  }

  class Client_Task : public ACE_Task_Base
  {
  public:
    Client_Task (Hang_ptr h)
      : h_ (Hang::_duplicate (h))
    {}

    virtual int svc (void)
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
      catch (const CORBA::COMM_FAILURE& f)
        {
          ACE_UNUSED_ARG (f);
          ACE_DEBUG ((LM_DEBUG,
                      "(%P|%t) Caught COMM_FAILURE Exception\n"));

          ACE_DEBUG ((LM_DEBUG,
                      "(%P|%t) This is expected\n"));

          return 0;
        }
      catch (const CORBA::Exception& ex)
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
    Hang_var h_;
  };

  class Shutdown_Task : public ACE_Task_Base
  {
  public:
    Shutdown_Task (CORBA::ORB_ptr o)
      : o_ (CORBA::ORB::_duplicate (o))
    {}

    virtual int svc (void)
    {

      try
        {
          ACE_DEBUG ((LM_DEBUG,
                      "(%P|%t) Calling shutdown\n"));

          // Just wait for the main thread to start sending out
          // messages
          ACE_OS::sleep (4);

          // Start the timer
          profile_timer.start ();

          this->o_->shutdown (blocked);

          // Stop the timer
          profile_timer.stop ();

          // Get the elampsed time
          ACE_Profile_Timer::ACE_Elapsed_Time el;
          profile_timer.elapsed_time (el);

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
    CORBA::ORB_var o_;
  };

  static int
  try_main (int argc, ACE_TCHAR *argv[])
  {

    try
      {
        CORBA::ORB_var orb =
          CORBA::ORB_init (argc, argv);

        if (parse_args (argc, argv) == false)
          return -1;

        CORBA::Object_var tmp =
          orb->string_to_object (ior);

        Hang_var test =
          Hang::_narrow (tmp.in ());

        if (CORBA::is_nil (test.in ()))
          {
            ACE_ERROR_RETURN ((LM_DEBUG,
                               "Nil test reference <%s>\n",
                               ior),
                              1);
          }

        Client_Task ct (test.in ());

        if (ct.activate (THR_NEW_LWP | THR_JOINABLE, 1) != 0)
          ACE_ERROR_RETURN ((LM_ERROR,
                             "Cannot activate client threads\n"),
                            1);

        ACE_DEBUG ((LM_DEBUG,
                    "(%P|%t) Activating shutdown thread\n"));

        Shutdown_Task st (orb.in ());

        if (st.activate (THR_NEW_LWP | THR_JOINABLE, 1) != 0)
          ACE_ERROR_RETURN ((LM_ERROR,
                             "Cannot activate shutdown threads\n"),
                            1);

        ACE_Thread_Manager::instance ()->wait ();

        orb->destroy ();

      }
    catch (const CORBA::Exception& ex)
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
