
//=============================================================================
/**
 *  @file    Bug_2659_Regression_Test.cpp
 *
 *  $Id$
 *
 *     Reproduces the problems reported in bug 2659:
 *      http://deuce.doc.wustl.edu/bugzilla/show_bug.cgi?id=2659
 *
 *
 *  @author Ciju John <johnc at ociweb>
 */
//=============================================================================


#include "test_config.h"

#include "ace/Log_Msg.h"
#include "ace/Task.h"
#include "ace/OS_NS_unistd.h"
#include "ace/Thread_Manager.h"
#include "ace/TP_Reactor.h"

#if !defined (ACE_LACKS_UNIX_SIGNALS)

bool reactor_task_ready = false;


//#define RUN_DEBUG 1

#if defined RUN_DEBUG

#define EE_DEBUG(CNAME,MNAME,LOC) \
EntryExit ee (CNAME,MNAME,LOC)

class EntryExit
{
public:
  EntryExit (const ACE_TCHAR* class_name,
             const ACE_TCHAR *method_name,
             void *location = 0)
  {
    class_name_ [20] = method_name_[20] = 0;

    ACE_OS::strncpy (class_name_, class_name, 20);
    ACE_OS::strncpy (method_name_, method_name, 20);
    location_ = location;

    ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("(%P|%t) Entry %@ %s::%s\n")
         , location, class_name, method_name));
  };

  ~EntryExit ()
  {
    ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("(%P|%t) Exit %@ %s::%s\n")
         , location_, class_name_, method_name_));
  };

private:
  ACE_TCHAR class_name_[21];
  ACE_TCHAR method_name_[21];
  void *location_;
};

#else

#define EE_DEBUG(CNAME,MNAME,LOC)

#endif // if defined RUN_DEBUG



static void
handle_signal (int )
{
  EE_DEBUG ("", "handle_signal", 0);

  // Dummy signal handler
}

class ReactorTask : public ACE_Task_Base
{
public:

  virtual ~ReactorTask ()
  {
    EE_DEBUG ("ReactorTask", "~ReactorTask", this);
  };

  virtual int svc (void )
  {
    EE_DEBUG ("ReactorTask", "svc", this);

    // Register a valid signal handler
    // so process doesn't die upon receiving signal
    ACE_Sig_Action sa ((ACE_SignalHandler) &::handle_signal, SIGUSR1);
    ACE_UNUSED_ARG (sa);

    if (simulate_perform_work () == -1) {
      ACE_ERROR_RETURN ((LM_ERROR,
       ACE_TEXT ("(%P|%t) ERROR: simulated_perform_work failed.\n"))
      , -1);
    }

    return 0;
  };

private:
  int simulate_perform_work ()
  {
    EE_DEBUG ("ReactorTask", "simulate_perform_work", this);

    // Create a reactor which doesn't automatically restart
    // upon interruption
    ACE_TP_Reactor tp_reactor (ACE_TP_Reactor::DEFAULT_SIZE, 0);

    reactor_task_ready = true;

    // This will return upon signal interruption
    return tp_reactor.handle_events ();
  }
};

int
run_main (int, ACE_TCHAR *[])
{
  ACE_START_TEST (ACE_TEXT ("Bug_2659_Regression_Test"));
  EE_DEBUG ("", "run_main", 0);

  ReactorTask reactor_task;

  if (reactor_task.activate () == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
             ACE_TEXT ("(%P|%t) Task activation failed.\n"))
            , -1);
    }

  ACE_Thread_Manager *thread_manager = reactor_task.thr_mgr ();
  if (thread_manager == 0)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
             ACE_TEXT ("(%P|%t) No Thread Manager found.\n"))
            , -1);
    }

  while (!reactor_task_ready)
    {
      ACE_OS::sleep (1);
    }

  ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("(%P|%t) Signalling task.\n")));

  if (thread_manager->kill_all (SIGUSR1) == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                        ACE_TEXT ("(%P|%t) Task signalling failed.\n")),
                        -1);
    }

  // Wait 5 seconds for the other thread to exit, if it didn't exit the
  // wait return -1 and we return with an error
  ACE_Time_Value timeout (5);
  if (thread_manager->wait (&timeout, false, false) == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                        ACE_TEXT ("(%P|%t) Error, task wait failed.\n")),
                        -1);
      }

  ACE_END_TEST;

  return 0;
}

#else
int
run_main (int, ACE_TCHAR *[])
{
  ACE_START_TEST (ACE_TEXT ("Bug_2659_Regression_Test"));
  ACE_ERROR ((LM_INFO,
              ACE_TEXT ("The Unix Signals capability is not supported on this platform\n")));
  ACE_END_TEST;
  return 0;
}
#endif /* !defined (ACE_LACKS_UNIX_SIGNALS) */
