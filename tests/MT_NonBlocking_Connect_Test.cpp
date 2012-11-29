
//=============================================================================
/**
 *  @file    MT_NonBlocking_Connect_Test.cpp
 *
 *  $Id$
 *
 *
 *  This test makes non-blocking connects from multiple threads. Things are
 *  complicated by the fact that after the connection is attempted reactor's
 *  handle_events() is called and this leads to possible connection
 *  completion in the other thread. This is similar to what TAO does for
 *  oneway with SYNC_NONE sync scope policy.
 *  The following reactors are tested: Select, TP, WFMO, and Dev Poll
 *  (if enabled).
 *
 *
 *  @author Vladimir Zykov <vladimir.zykov@prismtech.com>
 */
//=============================================================================


#include "test_config.h"
#include "ace/Reactor.h"
#include "ace/Select_Reactor.h"
#include "ace/TP_Reactor.h"
#include "ace/WFMO_Reactor.h"
#include "ace/Dev_Poll_Reactor.h"
#include "ace/Svc_Handler.h"
#include "ace/SOCK_Stream.h"
#include "ace/Get_Opt.h"
#include "ace/Task.h"
#include "ace/Connector.h"
#include "ace/SOCK_Connector.h"
#include "ace/Thread_Mutex.h"

#if defined (ACE_HAS_THREADS)

static int test_select_reactor = 1;
static int test_tp_reactor = 1;
static int test_wfmo_reactor = 1;
static int test_dev_poll_reactor = 1;
static int number_of_threads = 10;
static int debug = 0;
static int result = 0;

static const ACE_TCHAR* hosts[] = {
  ACE_TEXT ("www.russiantvguide.com:80"),
  ACE_TEXT ("news.bbc.co.uk:80"),
  ACE_TEXT ("www.cnn.com:80"),
  ACE_TEXT ("www.waca.com.au:80"),
  ACE_TEXT ("www.uganda.co.ug:80"),
  ACE_TEXT ("www.cs.wustl.edu:80"),
  ACE_TEXT ("www.dre.vanderbilt.edu:80"),
  ACE_TEXT ("www.dhm.gov.np:80"),
  ACE_TEXT ("www.msn.com:80"),
  ACE_TEXT ("www.presidencymaldives.gov.mv:80")
};

class Svc_Handler : public ACE_Svc_Handler<ACE_SOCK_STREAM, ACE_NULL_SYNCH>
{
public:
  Svc_Handler (void)
  {
    ACE_TEST_ASSERT (0);
  }

  Svc_Handler (ACE_Thread_Manager *);

  int open (void *);

  int close (u_long flags);

  bool connected_;
};

Svc_Handler::Svc_Handler (ACE_Thread_Manager *)
  : connected_ (false)
{
  this->reference_counting_policy ().value (
    ACE_Event_Handler::Reference_Counting_Policy::ENABLED);
}

int
Svc_Handler::open (void *)
{
  this->connected_ = true;
  return 0;
}

int
Svc_Handler::close (u_long)
{
  ACE_DEBUG ((LM_DEBUG,
              "%t close connection on handle %d.\n",
              this->get_handle ()));
  return 0;
}

template<class SVC_HANDLER>
class Concurrency_Strategy :
  public ACE_Concurrency_Strategy<SVC_HANDLER>
{
public:
  virtual int activate_svc_handler (SVC_HANDLER *svc_handler, void *arg);
};

template<class SVC_HANDLER> int
Concurrency_Strategy<SVC_HANDLER>::
  activate_svc_handler (SVC_HANDLER *svc_handler, void *arg)
{
  // Every fourth connection fails.
  static long count = 0;
  if (++count % 4 == 0)
    {
      ACE_DEBUG ((LM_DEBUG,
                  "%t connection on handle %d has artificially failed.\n",
                  svc_handler->get_handle ()));
      return -1;
    }
  else
    {
      return ACE_Concurrency_Strategy<SVC_HANDLER>::activate_svc_handler (
        svc_handler, arg);
    }
}

typedef ACE_Creation_Strategy<Svc_Handler> CREATION_STRATEGY;
typedef ACE_Connect_Strategy<Svc_Handler,
                             ACE_SOCK_CONNECTOR> CONNECT_STRATEGY;
typedef Concurrency_Strategy<Svc_Handler> CONCURRENCY_STRATEGY;
typedef ACE_Strategy_Connector<Svc_Handler,
                               ACE_SOCK_CONNECTOR> BASE_CONNECTOR;

class Connect_Thread : public ACE_Task_Base
{
public:
  Connect_Thread (ACE_Thread_Manager &thread_manager,
                  ACE_Reactor &reactor,
                  ACE_Thread_Mutex &reactor_lock)
    : ACE_Task_Base (&thread_manager)
    , reactor_ (reactor)
    , reactor_lock_ (reactor_lock)
    , threads_ (number_of_threads)
  {
    // Open the connector.
    this->base_connector_.open (&this->reactor_,
                                &this->cns_, &this->cts_, &this->cys_);
  }

  int svc (void);

private:
  ACE_Reactor &reactor_;
  ACE_Thread_Mutex &reactor_lock_;

  CREATION_STRATEGY cns_;
  CONNECT_STRATEGY cts_;
  CONCURRENCY_STRATEGY cys_;
  BASE_CONNECTOR base_connector_;

  ACE_Atomic_Op<ACE_Thread_Mutex, long> threads_;
};

int
Connect_Thread::svc (void)
{
  size_t const nr_names = sizeof hosts / sizeof (char *);
  ACE_INET_Addr *addresses = new ACE_INET_Addr[nr_names];

  for (size_t i = 0; i < nr_names; ++i)
    {
      if (addresses[i].set (hosts[i]) != 0)
        {
          ACE_DEBUG ((LM_INFO,
                      ACE_TEXT ("%p\n"),
                      hosts[i]));
        }
    }

  ACE_INET_Addr local_addr;
  ACE_Synch_Options synch_options (ACE_Synch_Options::USE_REACTOR);

  for (size_t i = 0; i < nr_names; ++i)
    {
      ACE_INET_Addr &remote_addr = addresses[i];

      ACE_DEBUG ((LM_DEBUG,
                  "%t connecting to %s...\n",
                  hosts[i]));

      // Create a new handler. It's what begin_connection() in TAO does.
      Svc_Handler *svc_handler = 0;
      result = this->base_connector_.connect (svc_handler,
                                              remote_addr,
                                              synch_options,
                                              local_addr);
      ACE_Event_Handler_var release_guard (svc_handler);

      // Complete connection. It's what complete_connection() in TAO does.
      // Not exactly but for the test it's enough.
      int run_for_wfmo = 5;
      while (this->reactor_.work_pending () && run_for_wfmo)
        {
          ACE_GUARD_RETURN (ACE_Thread_Mutex, ace_mon,
                            this->reactor_lock_, -1);

          if (this->reactor_.work_pending () && run_for_wfmo)
            {
              --run_for_wfmo;

              ACE_DEBUG ((LM_DEBUG,
                          "%t waiting for connection to %s...\n",
                          hosts[i]));

              this->reactor_.owner (ACE_Thread::self ());

              this->reactor_.handle_events ();
            }
        }

      if (svc_handler->connected_)
        {
          this->reactor_.register_handler (svc_handler,
                                           ACE_Event_Handler::READ_MASK);

          ACE_DEBUG ((LM_DEBUG,
                      "%t connection for %s completed on handle %d.\n",
                      hosts[i],
                      svc_handler->get_handle ()));
          // We are connected successfully. Close the connection.
          this->reactor_.remove_handler (svc_handler->get_handle (),
                                         ACE_Event_Handler::ALL_EVENTS_MASK |
                                         ACE_Event_Handler::DONT_CALL);
        }
      else
        {
          // Connection didn't succeed. svc_handler will be deleted when
          // we go out of the scope of this loop.
          ACE_DEBUG ((LM_DEBUG,
                      "%t connection for %s did not complete.\n",
                      hosts[i]));
        }
    }

  if (--this->threads_ == 0)
    {
      // Close the connector.
      this->base_connector_.close ();
    }

  delete[] addresses;

  return 0;
}

void
test (ACE_Reactor_Impl *reactor_impl)
{
  ACE_Reactor reactor (reactor_impl, true);

  ACE_Thread_Mutex reactor_lock;
  ACE_Thread_Manager thread_manager;

  ACE_DEBUG ((LM_DEBUG,
              "Starting %d connection threads...\n",
              number_of_threads));

  Connect_Thread connect_thread (thread_manager, reactor, reactor_lock);
  result = connect_thread.activate (THR_NEW_LWP|THR_JOINABLE,
                                    number_of_threads);
  ACE_TEST_ASSERT (result == 0);

  // Wait for threads to exit.
  result = thread_manager.wait ();
  ACE_TEST_ASSERT (result == 0);
}

static int
parse_args (int argc, ACE_TCHAR *argv[])
{
  ACE_Get_Opt get_opt (argc, argv, ACE_TEXT ("a:b:c:d:f:g:k:lm:n:o:uz:"));

  int cc;
  while ((cc = get_opt ()) != -1)
    {
      switch (cc)
        {
        case 'a':
          test_select_reactor = ACE_OS::atoi (get_opt.opt_arg ());
          break;
        case 'b':
          test_tp_reactor = ACE_OS::atoi (get_opt.opt_arg ());
          break;
        case 'c':
          test_wfmo_reactor = ACE_OS::atoi (get_opt.opt_arg ());
          break;
        case 'd':
          test_dev_poll_reactor = ACE_OS::atoi (get_opt.opt_arg ());
          break;
        case 'f':
          number_of_threads = ACE_OS::atoi (get_opt.opt_arg ());
          break;
        case 'z':
          debug = ACE_OS::atoi (get_opt.opt_arg ());
          break;
        case 'u':
        default:
          ACE_ERROR ((LM_ERROR,
                      ACE_TEXT ("\nusage: %s \n\n")
                      ACE_TEXT ("\t[-a test Select Reactor] (defaults to %d)\n")
                      ACE_TEXT ("\t[-b test TP Reactor] (defaults to %d)\n")
                      ACE_TEXT ("\t[-c test WFMO Reactor] (defaults to %d)\n")
                      ACE_TEXT ("\t[-d test Dev Poll Reactor] (defaults to %d)\n")
                      ACE_TEXT ("\t[-f number of threads] (defaults to %d)\n")
                      ACE_TEXT ("\t[-z debug] (defaults to %d)\n")
                      ACE_TEXT ("\n"),
                      argv[0],
                      test_select_reactor,
                      test_tp_reactor,
                      test_wfmo_reactor,
                      test_dev_poll_reactor,
                      number_of_threads,
                      debug));
          return -1;
        }
    }

  return 0;
}

int
run_main (int argc, ACE_TCHAR *argv[])
{
  ACE_START_TEST (ACE_TEXT ("MT_NonBlocking_Connect_Test"));

  // Validate options.
  result = parse_args (argc, argv);
  if (result != 0)
    return result;

  if (test_select_reactor)
    {
      ACE_DEBUG ((LM_DEBUG,
                  ACE_TEXT ("\n\n(%t) Testing Select Reactor....\n\n")));

      test (new ACE_Select_Reactor);
    }

  if (test_tp_reactor)
    {
      ACE_DEBUG ((LM_DEBUG,
                  ACE_TEXT ("\n\n(%t) Testing TP Reactor....\n\n")));

      test (new ACE_TP_Reactor);
    }

#if defined (ACE_HAS_EVENT_POLL)

  if (test_dev_poll_reactor)
    {
      ACE_DEBUG ((LM_DEBUG,
                  ACE_TEXT ("\n\n(%t) Testing Dev Poll Reactor....\n\n")));

      test (new ACE_Dev_Poll_Reactor);
    }

#endif

#if defined (ACE_WIN32)

  if (test_wfmo_reactor)
    {
      ACE_DEBUG ((LM_DEBUG,
                  ACE_TEXT ("\n\n(%t) Testing WFMO Reactor....\n\n")));

      test (new ACE_WFMO_Reactor);
    }

#endif /* ACE_WIN32 */

  ACE_END_TEST;

  return result;
}

#else /* ACE_HAS_THREADS */

int
run_main (int, ACE_TCHAR *[])
{
  ACE_START_TEST (ACE_TEXT ("MT_NonBlocking_Connect_Test"));

  ACE_ERROR ((LM_INFO,
              ACE_TEXT ("threads not supported on this platform\n")));

  ACE_END_TEST;

  return 0;
}

#endif /* ACE_HAS_THREADS */
