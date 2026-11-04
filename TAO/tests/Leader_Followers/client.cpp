
//=============================================================================
/**
 *  @file     client.cpp
 *
 *   See README.
 *
 *  @author  Irfan Pyarali
 */
//=============================================================================


#include "ace/Get_Opt.h"
#include "ace/Log_Msg.h"
#include "ace/Task.h"
#include "ace/OS_NS_stdlib.h"
#include "ace/OS_NS_unistd.h"
#include "testC.h"

#include "tao/Strategies/advanced_resource.h"

#include <string>

struct Client_Options
{
  std::basic_string<ACE_TCHAR> ior {ACE_TEXT ("file://ior")};
  int client_threads {3};
  int event_loop_threads {1};
  u_long remote_work {5000};
  u_long event_loop_timeout {7000};
  bool shutdown_server {};
};

static int
parse_args (int argc, ACE_TCHAR *argv[], Client_Options& options)
{
  ACE_Get_Opt get_opts (argc, argv, ACE_TEXT ("k:c:e:w:t:x"));
  int c {};

  while ((c = get_opts ()) != -1)
    switch (c)
      {
      case 'k':
        options.ior = get_opts.opt_arg ();
        break;

      case 'c':
        options.client_threads = ACE_OS::atoi (get_opts.opt_arg ());
        break;

      case 'e':
        options.event_loop_threads = ACE_OS::atoi (get_opts.opt_arg ());
        break;

      case 't':
        options.event_loop_timeout = ACE_OS::atoi (get_opts.opt_arg ());
        break;

      case 'w':
        options.remote_work = ACE_OS::atoi (get_opts.opt_arg ());
        break;

      case 'x':
        options.shutdown_server = true;
        break;

      case '?':
      default:
        ACE_ERROR_RETURN ((LM_ERROR,
                           ACE_TEXT ("usage:  %s "
                                     "-k IOR "
                                     "-c number of client threads "
                                     "-e number of event loop threads "
                                     "-t event loop timeout "
                                     "-w remote work "
                                     "-x shutdown server "
                                     "\n"),
                           argv [0]),
                          -1);
      }

  return 0;
}

class Client_Task : public ACE_Task_Base
{
public:
  Client_Task (test_ptr t, Client_Options const& options)
    : test_ (test::_duplicate (t)),
      options_ (options)
    {
    }

  Client_Task (Client_Task const&) = delete;
  Client_Task (Client_Task&&) = delete;
  Client_Task& operator= (Client_Task const&) = delete;
  Client_Task& operator= (Client_Task&&) = delete;

  int svc () override
    {
      try
        {
          u_long work_from_this_thread {};
          ACE_Time_Value sleep_for_this_thread;

          {
            ACE_GUARD_RETURN (TAO_SYNCH_MUTEX, ace_mon, this->lock_, -1);

            this->work_so_far_ += this->options_.remote_work / this->options_.client_threads;
            work_from_this_thread = this->work_so_far_;

            sleep_for_this_thread.msec (this->sleep_);
            this->sleep_ += 1000 / this->options_.client_threads;
          }

          // Small pause to avoid overrunning the server.
          ACE_OS::sleep (sleep_for_this_thread);

          // Invoke the method.
          ACE_DEBUG ((LM_DEBUG,
                      ACE_TEXT ("Client: Invoking server from thread %t for time %u @ %T\n"),
                      static_cast<unsigned int> (work_from_this_thread)));

          CORBA::ULong const result = this->test_->method (work_from_this_thread);

          if (work_from_this_thread != result)
            {
              ACE_DEBUG ((LM_DEBUG,
                          ACE_TEXT ("Client: result is %u\n"), result));
              ACE_ASSERT (work_from_this_thread == result);
            }

          ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("Client: client loop finished for thread %t @ %T\n")));
        }
      catch (CORBA::Exception const& ex)
        {
          ex._tao_print_exception ("Exception caught in thread:");
          return -1;
        }


      return 0;
    }

private:
  test_var const test_;
  Client_Options const options_;

  // Work counter.
  u_long work_so_far_ {};

  // Lock for work counter.
  TAO_SYNCH_MUTEX lock_;

  // Small pause to avoid overrunning the server.
  long sleep_ {};
};

class Event_Loop_Task : public ACE_Task_Base
{
public:
  Event_Loop_Task (CORBA::ORB_ptr orb, Client_Options const& options)
    : orb_ (CORBA::ORB::_duplicate (orb)),
      options_ (options)
    {
    }

  Event_Loop_Task (Event_Loop_Task const&) = delete;
  Event_Loop_Task (Event_Loop_Task&&) = delete;
  Event_Loop_Task& operator= (Event_Loop_Task const&) = delete;
  Event_Loop_Task& operator= (Event_Loop_Task&&) = delete;

  int svc () override
    {
      try
        {
          u_long event_loop_timeout_for_this_thread {};

          {
            ACE_GUARD_RETURN (TAO_SYNCH_MUTEX, ace_mon, this->lock_, -1);

            this->event_loop_timeout_so_far_ +=
              this->options_.event_loop_timeout / this->options_.event_loop_threads;
            event_loop_timeout_for_this_thread = this->event_loop_timeout_so_far_;
          }

          ACE_DEBUG ((LM_DEBUG,
                      ACE_TEXT ("Client: Event loop thread %t starting event loop "
                                "for %u milli seconds @ %T\n"),
                      static_cast<unsigned int> (event_loop_timeout_for_this_thread)));

          ACE_Time_Value timeout (0,
                                  event_loop_timeout_for_this_thread * 1000);

          this->orb_->run (timeout);

          ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("Client: Event loop finished for thread %t @ %T\n")));
        }
      catch (CORBA::Exception const& ex)
        {
          ex._tao_print_exception ("Exception caught in thread:");
          return -1;
        }


      return 0;
    }

private:
  // ORB reference.
  CORBA::ORB_var const orb_;
  Client_Options const options_;

  // Event loop timeout counter.
  u_long event_loop_timeout_so_far_ {};

  // Lock for event loop timeout counter.
  TAO_SYNCH_MUTEX lock_;
};

int
ACE_TMAIN (int argc, ACE_TCHAR *argv[])
{
  try
    {
      // Initialize the ORB.
      CORBA::ORB_var const orb =
        CORBA::ORB_init (argc, argv);

      // Initialize options based on command-line arguments.
      Client_Options options;
      int const parse_args_result = parse_args (argc, argv, options);
      if (parse_args_result != 0)
        return parse_args_result;

      // Get an object reference from the argument string.
      CORBA::Object_var const object =
        orb->string_to_object (options.ior.c_str ());

      // Try to narrow the object reference to a <server> reference.
      test_var const server = test::_narrow (object.in ());

      Client_Task client_task (server.in (), options);

      if (client_task.activate (THR_NEW_LWP | THR_JOINABLE,
                                options.client_threads) != 0)
        ACE_ERROR_RETURN ((LM_ERROR,
                           ACE_TEXT ("Cannot activate client threads\n")),
                          -1);

      // Make sure to give the client threads enough time to become
      // leaders.
      ACE_OS::sleep (4);

      Event_Loop_Task event_loop_task (orb.in (), options);

      if (event_loop_task.activate (THR_NEW_LWP | THR_JOINABLE,
                                    options.event_loop_threads) != 0)
        ACE_ERROR_RETURN ((LM_ERROR,
                           ACE_TEXT ("Cannot activate event_loop threads\n")),
                          -1);

      event_loop_task.thr_mgr ()->wait ();
      client_task.thr_mgr ()->wait ();

      ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("Client: All threads finished @ %T\n")));

      // Shutdown server.
      if (options.shutdown_server)
        {
          server->shutdown ();
        }
    }
  catch (CORBA::Exception const& ex)
    {
      ex._tao_print_exception ("Exception caught:");
      return -1;
    }


  return 0;
}
