// $Id$

// ============================================================================
/**
 *
 * @file    Select_Reactor_Notify_Stress_Test.cpp
 *
 * @brief A stress test for the ACE_Select_Reactor's notify ()
 * mechanism.
 *
 * This test stresses the user level queues maintained by the
 * ACE_Select_Reactor's notify () mechanism. This test has been
 * designed to make sure that the ACE_Select_Reactor's notify ()
 * mechanism works with large number of notify messages that cannot be
 * easily handled with a pipe.
 *
 * @@NOTE: In ACE 5.2, ACE wrote a message in the pipe for every
 * message in the user level queue which caused weird deadlocks. This
 * test is to make sure that scalability problems of that sort dont
 * find its way back into the ACE_Select_Reactor
 *
 * @author Balachandran Natarajan <bala@cs.wustl.edu>
 */
//============================================================================
#include "test_config.h"
#include "ace/Select_Reactor.h"

ACE_RCSID(tests,
          Select_Reactor_Notify_Stress_Test,
          "$Id$")

#if defined (ACE_HAS_THREADS) && (ACE_HAS_REACTOR_NOTIFICATION_QUEUE)
#include "ace/Log_Msg.h"
#include "ace/Synch.h"
#include "ace/Task.h"
#include "ace/Pipe.h"
#include "ace/Auto_Ptr.h"

static const int max_notify_iterations = 100000;
static const int nthreads = 6;

class Notify_Handler: public ACE_Event_Handler
{
public:
  /// Ctor
  Notify_Handler (void);

  /// Template method
  virtual int handle_input (ACE_HANDLE h = ACE_INVALID_HANDLE);
  virtual int handle_close (ACE_HANDLE h,
                            ACE_Reactor_Mask mask);

  /// Method that returns the number of upcalls received.
  int calls_made (void);

private:
  /// Mutex to restore internal sanity
  ACE_SYNCH_MUTEX mutex_;

  /// Number of upcalls received
  int no_upcalls_;
};


Notify_Handler::Notify_Handler (void)
  : mutex_ (),
    no_upcalls_ (0)
{
}

int
Notify_Handler::handle_input (ACE_HANDLE)
{
  ACE_GUARD_RETURN (ACE_SYNCH_MUTEX,
                    ace_mon,
                    this->mutex_,
                    -1);

  ++this->no_upcalls_;

  for (size_t j = 0; j < 100; ++j)
    {
      // Eat a little CPU
      /* takes about 40.2 usecs on a 167 MHz Ultra2 */
      u_long n = 11UL;
      ACE::is_prime (n, 2, n / 2);
    }

  return 0;
}

int
Notify_Handler::handle_close (ACE_HANDLE,
                              ACE_Reactor_Mask m)
{
  // no-op
  return 0;
}

int
Notify_Handler::calls_made (void)
{
  ACE_GUARD_RETURN (ACE_SYNCH_MUTEX,
                    ace_mon,
                    this->mutex_,
                    -1);
  // no-op

  return this->no_upcalls_;
}


/******************************************************/
class Notify_Task : public ACE_Task_Base
{
public:
  /// Constructor.
  Notify_Task (ACE_Reactor *r,
               ACE_Event *event,
               Notify_Handler *nh);

  /// Destructor.
  ~Notify_Task (void);

  /// Generates events and sends them to the <Reactor>'s <notify>
  /// method.
  virtual int svc (void);

private:
  /// The reactor to which we need to send notifies
  ACE_Reactor *r_;

  /// The ACE_Event in which we wait
  ACE_Event *event_;

  /// The notify handler that should handle the upcalls from the
  /// reactor.
  Notify_Handler *nh_;
};

Notify_Task::Notify_Task (ACE_Reactor *r,
                          ACE_Event *event,
                          Notify_Handler *nh)
  : r_ (r),
    event_ (event),
    nh_ (nh)
{
}

Notify_Task::~Notify_Task (void)
{
}

int
Notify_Task::svc (void)
{
  ACE_DEBUG ((LM_DEBUG,
              ACE_TEXT ("(%P|%t) Waking up for first set..\n")));

  int i = 0;

  // Send notificationa to the reactor
  for (i = 0;
       i != max_notify_iterations;
       i++)
    {
      int retval = this->r_->notify (this->nh_,
                                     ACE_Event_Handler::READ_MASK);
      if (retval == -1)
        ACE_ERROR_RETURN ((LM_ERROR,
                           ACE_TEXT ("(%P|%t) Error while sending notify () \n")),
                           -1);
    }

  // Wait on the event before proceeding
  this->event_->wait ();

  ACE_DEBUG ((LM_DEBUG,
              ACE_TEXT ("(%P|%t) Waking for second set..\n")));

  for (i = 0;
       i != max_notify_iterations;
       i++)
    {
      this->r_->notify (this->nh_,
                        ACE_Event_Handler::READ_MASK);
    }

  ACE_DEBUG ((LM_DEBUG,
              ACE_TEXT ("(%P|%t) Return..\n")));

  return 0;
}

/**************************************************************/
class Run_Task: public ACE_Task_Base
{
public:
  Run_Task (ACE_Event &ev);

  /// template method..
  int svc (void);

private:
  ACE_Event &ev_;
};

Run_Task::Run_Task (ACE_Event &ev)
  :ev_ (ev)
{
}

int
Run_Task::svc (void)
{
  int iter, retval = 0;

  ACE_Reactor::instance ()->owner (ACE_Thread::self ());

  ACE_Time_Value tv (40, 0);

  for (iter = 0;
       iter != nthreads*max_notify_iterations;
       iter++)
    {
      retval = ACE_Reactor::instance ()->handle_events (tv);

        //      ACE_DEBUG ((LM_DEBUG,
       //           "(%P|%t) going on 1..\n"));
      if (retval == -1)
        {
          // Signal the threads
          this->ev_.signal ();

          ACE_ERROR_RETURN ((LM_ERROR,
                             ACE_TEXT ("(%P|%t) Error in handle events in [%d] due to %p\n"),
                             iter),
                             -1);

        }
    }

  // Signal the threads
  this->ev_.signal ();

  ACE_DEBUG ((LM_DEBUG,
              ACE_TEXT ("(%P|%t) Handle events again\n")));

  for (iter = 0;
       iter != (nthreads -1 ) * max_notify_iterations;
       iter++)
    {
      retval = ACE_Reactor::instance ()->handle_events ();

      //ACE_DEBUG ((LM_DEBUG,
        //          "(%P|%t) going on 2..\n"));
      if (retval == -1)
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             ACE_TEXT ("(%P|%t) Error in handle events \n")),
                             -1);
        }
    }

  return 0;
}

static int
do_work (void)
{
  ACE_Select_Reactor sr;
  ACE_Reactor new_reactor (&sr);
  ACE_Reactor::instance (&new_reactor);

  // Set the max_notify_iterations to 1
  ACE_Reactor::instance ()->max_notify_iterations (1);

  Notify_Handler *nh = 0;
  ACE_NEW_RETURN (nh,
                  Notify_Handler (),
                  -1);

  ACE_Manual_Event ev;

  Notify_Task notify_tasks (&new_reactor,
                            &ev,
                            nh);

  Run_Task run_task (ev);

  if (notify_tasks.activate (THR_NEW_LWP | THR_JOINABLE,
                             nthreads) != 0)
    ACE_ERROR_RETURN ((LM_ERROR,
                       "Cannot activate notify_task threads\n"),
                      -1);

  if (run_task.activate (THR_NEW_LWP | THR_JOINABLE,
                         1) != 0)
    ACE_ERROR_RETURN ((LM_ERROR,
                       "Cannot activate run_task thread\n"),
                      -1);

  notify_tasks.thr_mgr ()->wait ();

  run_task.thr_mgr ()->wait ();

  int purged =
    ACE_Reactor::instance ()->purge_pending_notifications (nh);

  if (purged != max_notify_iterations)
    ACE_ERROR_RETURN ((LM_ERROR,
                       ACE_TEXT ("(%P|%t) Problems with the test \n")),
                       -1);

  return 0;
}


int
ACE_TMAIN (int, ACE_TCHAR *[])
{
  ACE_START_TEST (ACE_TEXT ("Select_Reactor_Notify_Stress_Test"));


  if (do_work () == -1)
    {
      ACE_DEBUG ((LM_ERROR,
                  ACE_TEXT ("(%P|%t) Error while doing work \n")));

      return -1;
    }

  return 0;

}

#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)
#elif defined (ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)
#endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */

#else /*ACE_HAS_THREADS && ACE_HAS_REACTOR_NOTIFICATION_QUEUE*/

int
ACE_TMAIN (int, ACE_TCHAR *[])
{
  ACE_DEBUG ((LM_DEBUG,
              ACE_TEXT ("(%P|%t) This test cannot be run in this configuration \n")));

  return 0;
}

#endif /*ACE_HAS_THREADS && ACE_HAS_REACTOR_NOTIFICATION_QUEUE*/
