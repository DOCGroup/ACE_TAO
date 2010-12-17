// $Id$

// ============================================================================
//
// = LIBRARY
//    examples
//
// = FILENAME
//    Abandoned.cpp
//
// = DESCRIPTION
//
//    Tests the WFMO_Reactor's ability to handle abandoned mutexes.
//
// = AUTHOR
//
//    Irfan Pyarali <irfan@cs.wustl.edu>
//
// ============================================================================

#include "ace/OS_main.h"

#if defined (ACE_WIN32)

#include "ace/Reactor.h"
#include "ace/Thread_Manager.h"
#include "ace/Process_Mutex.h"
#include "ace/Auto_Event.h"



class Event_Handler : public ACE_Event_Handler
{
public:
  int handle_signal (int signum,
                     siginfo_t * = 0,
                     ucontext_t * = 0);

  int handle_timeout (const ACE_Time_Value &tv,
                      const void *arg = 0);

  ACE_Auto_Event handle_;
  ACE_Process_Mutex *mutex_;
  int iterations_;
};

static int abandon = 1;

static ACE_THR_FUNC_RETURN
worker (void *data)
{
  Event_Handler *handler = (Event_Handler *) data;

  handler->handle_.signal ();
  handler->mutex_->acquire ();

  if (!abandon)
    handler->mutex_->release ();

  return 0;
}

int
Event_Handler::handle_signal (int,
                              siginfo_t *s,
                              ucontext_t *)
{
  ACE_HANDLE handle = s->si_handle_;
  if (handle == this->handle_.handle ())
    ACE_Reactor::instance ()->register_handler (this,
                                                this->mutex_->lock ().proc_mutex_);
  else
    {
      ACE_Reactor::instance ()->remove_handler (this->mutex_->lock ().proc_mutex_,
                                                ACE_Event_Handler::DONT_CALL);
      delete this->mutex_;
    }

  return 0;
}

int
Event_Handler::handle_timeout (const ACE_Time_Value &,
                               const void *)
{
  --this->iterations_;
  ACE_DEBUG ((LM_DEBUG,
              "(%t) timeout occured @ %T, iterations left %d\n",
              this->iterations_));

  if (this->iterations_ == 0)
    {
      ACE_Reactor::instance ()->remove_handler (this->handle_.handle (),
                                                ACE_Event_Handler::DONT_CALL);

      ACE_Reactor::instance ()->cancel_timer (this);

      ACE_Reactor::end_event_loop ();
    }
  else
    {
      ACE_NEW_RETURN (this->mutex_,
                      ACE_Process_Mutex,
                      -1);
      int result = ACE_Thread_Manager::instance ()->spawn (&worker, this);
      ACE_ASSERT (result != -1);
      ACE_UNUSED_ARG (result);
    }

  return 0;
}

int
ACE_TMAIN (int , ACE_TCHAR *[])
{
  Event_Handler event_handler;

  event_handler.iterations_ = 5;
  int result = ACE_Reactor::instance ()->register_handler
    (&event_handler,
     event_handler.handle_.handle ());
  ACE_ASSERT (result == 0);

  ACE_Time_Value timeout (2);
  result = ACE_Reactor::instance ()->schedule_timer (&event_handler,
                                                     0,
                                                     timeout,
                                                     timeout);
  ACE_ASSERT (result != -1);

  ACE_Reactor::run_event_loop ();

  return 0;
}
#else /* !ACE_WIN32 */
int
ACE_TMAIN (int , ACE_TCHAR *[])
{
  return 0;
}
#endif /* ACE_WIN32 */
