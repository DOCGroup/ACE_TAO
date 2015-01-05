// $Id$

// ============================================================================
//
// = LIBRARY
//    tests
//
// = FILENAME
//    Notify_Performance_Test.cpp
//
// = DESCRIPTION
//    This test is used to time the notification mechanisms of the
//    ACE_Reactors. Both the WFMO_Reactor and Select_Reactor can be
//    tested. The notify() mechanism can also be tested with or
//    without data.
//
// = AUTHOR
//    Irfan Pyarali <irfan@cs.wustl.edu>
//
// ============================================================================

#include "test_config.h"
#include "ace/Profile_Timer.h"
#include "ace/Get_Opt.h"
#include "ace/Thread_Manager.h"
#include "ace/Reactor.h"
#include "ace/WFMO_Reactor.h"
#include "ace/Select_Reactor.h"
#include "ace/Dev_Poll_Reactor.h"
#include "ace/Auto_Ptr.h"
#include "ace/Atomic_Op.h"



#if defined (ACE_HAS_THREADS)

// Number of client (user) threads
static long opt_nthreads = 1;

// Number of notify calls
static int opt_nloops = 20000;

// Use the WFMO_Reactor
static int opt_wfmo_reactor = 0;

// Use the Select_Reactor
static int opt_select_reactor = 0;

// Use the Dev_Poll_Reactor
static int opt_dev_poll_reactor = 0;

// Pass data through the notify call
static int opt_pass_notify_data = 0;

// Simple no-op handler
class Handler : public ACE_Event_Handler
{
public:
  virtual int handle_exception (ACE_HANDLE fd = ACE_INVALID_HANDLE);
  // The Handler callbacks.
};

int
Handler::handle_exception (ACE_HANDLE handle)
{
  ACE_UNUSED_ARG (handle);

  return 0;
}

// Execute the client tests.

static void *
client (void *arg)
{
  // Number of client (user) threads
  static ACE_Atomic_Op<ACE_Thread_Mutex, long> thread_counter;
  thread_counter = opt_nthreads;

  // To pass or not to pass is the question
  Handler *handler = 0;
  if (!opt_pass_notify_data)
    handler = 0;
  else
    handler = (Handler *) arg;

  for (int i = 0; i < opt_nloops; i++)
    ACE_Reactor::instance ()->notify (handler);

  if (--thread_counter == 0)
    ACE_Reactor::instance()->end_reactor_event_loop ();

  return 0;
}

// Sets up the correct reactor (based on platform and options)

static void
create_reactor (void)
{
  ACE_Reactor_Impl *impl = 0;

  if (opt_wfmo_reactor)
    {
#if defined (ACE_HAS_WINSOCK2) && (ACE_HAS_WINSOCK2 == 1)
      ACE_NEW (impl, ACE_WFMO_Reactor);
#endif /* ACE_HAS_WINSOCK2 == 1 */
    }
  else if (opt_select_reactor)
    {
      ACE_NEW (impl, ACE_Select_Reactor);
    }
  else if (opt_dev_poll_reactor)
    {
#if defined (ACE_HAS_EVENT_POLL) || defined (ACE_HAS_DEV_POLL)
      ACE_NEW (impl, ACE_Dev_Poll_Reactor);
#endif /* ACE_HAS_EVENT_POLL || ACE_HAS_DEV_POLL */
    }
  ACE_Reactor *reactor = 0;
  ACE_NEW (reactor, ACE_Reactor (impl));
  ACE_Reactor::instance (reactor);
}

static void
print_results (ACE_Profile_Timer::ACE_Elapsed_Time &et)
{
  const ACE_TCHAR *reactor_type = 0;
  if (opt_wfmo_reactor)
    reactor_type = ACE_TEXT ("WFMO_Reactor");
  else if (opt_select_reactor)
    reactor_type = ACE_TEXT ("Select_Reactor");
  else if (opt_dev_poll_reactor)
    reactor_type = ACE_TEXT ("Dev_Poll_Reactor");
  else
    reactor_type = ACE_TEXT ("Platform's default Reactor");

  ACE_DEBUG ((LM_DEBUG,
              ACE_TEXT ("\nNotify_Performance Test statistics:\n\n")));
  ACE_DEBUG ((LM_DEBUG,
              ACE_TEXT ("\tReactor Type: %s\n"),
              reactor_type));
  ACE_DEBUG ((LM_DEBUG,
              ACE_TEXT ("\tWorker threads (calling notify()): %d\n"),
              opt_nthreads));
  ACE_DEBUG ((LM_DEBUG,
              ACE_TEXT ("\tIteration per thread: %d\n"),
              opt_nloops));
  if (opt_pass_notify_data)
    ACE_DEBUG ((LM_DEBUG,
                ACE_TEXT ("\tData was passed in the notify() call\n")));
  else
    ACE_DEBUG ((LM_DEBUG,
                ACE_TEXT ("\tNo data was passed in the notify() call\n")));

  ACE_DEBUG ((LM_DEBUG,
              ACE_TEXT ("\n\tTiming results notify() call:\n")));
  ACE_DEBUG ((LM_DEBUG,
              ACE_TEXT ("\t\treal time = %f secs \n\t\tuser time = %f secs \n\t\tsystem time = %f secs\n\n"),
              et.real_time,
              et.user_time,
              et.system_time));
}

int
run_main (int argc, ACE_TCHAR *argv[])
{
  ACE_START_TEST (ACE_TEXT ("Notify_Performance_Test"));

  //FUZZ: disable check_for_lack_ACE_OS
  ACE_Get_Opt getopt (argc, argv, ACE_TEXT ("pswdc:l:"));

  for (int c; (c = getopt ()) != -1; )
    switch (c)
      {
  //FUZZ: enable check_for_lack_ACE_OS
      case 'p':
        opt_dev_poll_reactor = 1;
        break;
      case 's':
        opt_select_reactor = 1;
        break;
      case 'w':
        opt_wfmo_reactor = 1;
        break;
      case 'c':
        opt_nthreads = ACE_OS::atoi (getopt.opt_arg ());
        break;
      case 'l':
        opt_nloops = ACE_OS::atoi (getopt.opt_arg ());
        break;
      case 'd':
        opt_pass_notify_data = 1;
        break;
      }

  // Sets up the correct reactor (based on platform and options)
  create_reactor ();

  // Manage memory automagically.
  auto_ptr<ACE_Reactor> reactor (ACE_Reactor::instance ());
  auto_ptr<ACE_Reactor_Impl> impl;

  // If we are using other that the default implementation, we must
  // clean up.
  if (opt_select_reactor || opt_wfmo_reactor || opt_dev_poll_reactor)
    {
      auto_ptr<ACE_Reactor_Impl> auto_impl (ACE_Reactor::instance ()->implementation ());
      impl = auto_impl;
    }

  // Callback object
  Handler handler;

  // Spawn worker threads
  if (ACE_Thread_Manager::instance ()->spawn_n
      (opt_nthreads,
       ACE_THR_FUNC (client),
       (void *) &handler,
       THR_NEW_LWP | THR_DETACHED) == -1)
    ACE_ERROR ((LM_ERROR, ACE_TEXT ("(%P|%t) %p\n%a"), ACE_TEXT ("thread create failed")));

  // Timer business
  ACE_Profile_Timer timer;
  timer.start ();

  // Run event loop
  ACE_Reactor::instance()->run_reactor_event_loop ();

  timer.stop ();

  ACE_Profile_Timer::ACE_Elapsed_Time et;
  timer.elapsed_time (et);

  // Print results
  print_results (et);

  ACE_DEBUG ((LM_DEBUG,
              ACE_TEXT ("(%P|%t) waiting for the worker threads...\n")));

  // Wait for all worker to get done.
  ACE_Thread_Manager::instance ()->wait ();

  ACE_END_TEST;
  return 0;
}

#else
int
run_main (int, ACE_TCHAR *[])
{
  ACE_START_TEST (ACE_TEXT ("Notify_Performance_Test"));

  ACE_ERROR ((LM_INFO,
              ACE_TEXT ("threads not supported on this platform\n")));

  ACE_END_TEST;
  return 0;
}
#endif /* ACE_HAS_THREADS */
