// $Id$
//
// ============================================================================
//
// = LIBRARY
//    examples
// 
// = FILENAME
//    test_abandoned.cpp
//
// = DESCRIPTION
//
//    Tests the WFMO_Reactor's ability to handle abandoned mutexes.
//    
// = AUTHOR
//    
//    Irfan Pyarali
// 
// ============================================================================

#include "ace/Reactor.h"
#include "ace/Synch.h"
#include "ace/Thread_Manager.h"

ACE_RCSID(ReactorEx, test_abandoned, "$Id$")

static int abandon = 1;

void *worker (void *);

class Event_Handler : public ACE_Event_Handler
{
public:
  int handle_signal (int signum, siginfo_t * = 0, ucontext_t * = 0);  

  int handle_timeout (const ACE_Time_Value &tv,
                      const void *arg = 0);

  ACE_Auto_Event handle_;
  ACE_Process_Mutex *mutex_;
  int iterations_;
};

int 
Event_Handler::handle_signal (int signum, siginfo_t *s, ucontext_t *)
{
  HANDLE handle = s->si_handle_;
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
Event_Handler::handle_timeout (const ACE_Time_Value &tv,
                               const void *arg)
{
  ACE_DEBUG ((LM_DEBUG, 
              "(%t) timeout occured @ %T, iterations left %d\n",
              --this->iterations_));  
  
  if (this->iterations_ == 0)
    ACE_Reactor::end_event_loop ();    
  
  else
    {
      this->mutex_ = new ACE_Process_Mutex;
      ACE_ASSERT (ACE_Thread_Manager::instance ()->spawn (&worker,
                                                          this) != -1);
    }

  return 0;
}

Event_Handler event_handler;
  
void *
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
main (int argc, char** argv)
{
  event_handler.iterations_ = 5;
  ACE_ASSERT (ACE_Reactor::instance ()->register_handler (&event_handler, 
                                                          event_handler.handle_.handle ()) == 0);

  ACE_Time_Value timeout (2);
  ACE_ASSERT (ACE_Reactor::instance ()->schedule_timer (&event_handler, 
                                                        0,
                                                        timeout,
                                                        timeout) != -1);
  
  ACE_Reactor::run_event_loop ();

  return 0;
}

