// $Id$

// ============================================================================
//
// = LIBRARY
//    examples
// 
// = FILENAME
//    Thread_Timer_Queue_Test.cpp
//
// = DESCRIPTION
//      This test exercises the <ACE_Thread_Timer_Queue_Adapter> 
//      using an <ACE_Timer_Heap>.
//
// = AUTHORS
//    Carlos O'Ryan and Douglas C. Schmidt
// 
// ============================================================================

#include "ace/Task.h"
#include "ace/Timer_Heap_T.h"
#include "ace/Timer_Queue_Adapters.h"

#include "Thread_Timer_Queue_Test.h"

// Administrivia methods...
Handler::Handler(const ACE_Time_Value &expiration_time)
  :  expires_ (expiration_time),
     id_ (0)
{
}

Handler::~Handler (void)
{
}

void 
Handler::set_id (int id)
{
  this->id_ = id;
}

// This is the method invoked when the Timer expires.

int 
Handler::handle_timeout (const ACE_Time_Value &current_time,
 			const void *)
{
  ACE_Time_Value delay = current_time - this->expires_;

  // No need to protect this printf is always called from a Async safe
  // point.
  ACE_OS::printf ("\nexpiring timer %d at %u.%07.7u secs\n"
		  "\tthere was a %u.%07.7u secs delay\n",
		  this->id_, 
		  current_time.sec (),
		  current_time.usec (),
		  delay.sec (), delay.usec ());

  // Notice this delete is protected.
  delete this;
  return 0;
}

// The handler was cancelled, so we must delete this.

int 
Handler::cancelled (void)
{
  delete this;
  return 0;
}

Input_Task::Input_Task (Thread_Timer_Queue *queue, Thread_Timer_Queue_Test_Driver &timer_queue_driver)
  : queue_ (queue),
    usecs_ (ACE_ONE_SECOND_IN_USECS), 
    driver_ (timer_queue_driver)
{
}

// Svc method is called from the thread library to read input from  the user.

int 
Input_Task::svc (void)
{
  for (;;)
    // call bacck to the driver's implementation on how to read and parse input.
    if (this->driver_.get_next_request () == -1)
      break;

  // we are done.
  this->queue_->deactivate ();
  ACE_DEBUG ((LM_DEBUG, "terminating input thread\n"));
  return 0;
}

// schedule a new timer.  This method will be called from inside the 
//  <Timer_Queue_Test_Driver> class.  (see Command pattern)

int
Input_Task::add_timer (void *argument)
{
  u_long useconds = *(int *)argument;
  ACE_Time_Value interval (useconds / usecs_, useconds % usecs_);
  ACE_Time_Value expire_at = ACE_OS::gettimeofday () + interval;

  Handler *h;

  ACE_NEW_RETURN (h, Handler (expire_at), -1);

  int id = queue_->schedule (h, 0, expire_at);
  
  if (id == -1)
    ACE_ERROR_RETURN ((LM_ERROR, "schedule failed"), -1);

  // We store the id into the handler, this is only used to produce
  // nicer messages.
  // @@ Should this be something that a user can extract?
  h->set_id (id);

  ACE_OS::printf ("scheduling timer %d\n", id);
  return 0;
}

// Cancel a timer.  This method will be called from inside the 
//  <Timer_Queue_Test_Driver> class.  (see Command pattern)

int
Input_Task::cancel_timer (void *argument)
{
  return  this->queue_->cancel (*(int *)argument);
}

// lists the timers in the queue.  Ignores the argument. This 
// method will be called from inside the <Timer_Queue_Test_Driver> 
// class.  (see Command pattern)

int
Input_Task::list_timer (void *argument)
{
  // Macro to avoid "warning: unused parameter" type warning.
  ACE_UNUSED_ARG (argument);

  // Thread cancellation point, if ACE supports it.
#if !defined (ACE_LACKS_PTHREAD_CANCEL)
  ACE_PTHREAD_CLEANUP_PUSH(&this->queue_->lock ());
#endif

  // dump the timer queue contents.
  this->dump ();

  // Thread cancellation point (POP)
#if !defined (ACE_LACKS_PTHREAD_CANCEL)
  ACE_PTHREAD_CLEANUP_POP(1);
#endif

  return 0;
}

// Shutdown the timer queue.  Return -1 indicates to the 
// <Timer_Queue_Test_Driver> class that we are done.

int
Input_Task::shutdown_timer (void *argument)
{
  // Macro to avoid "warning: unused parameter" type warning.
  ACE_UNUSED_ARG (argument);

  // -1 indicates we are shutting down the application.
  return -1;
}

void 
Input_Task::dump (void)
{
  ACE_GUARD (ACE_SYNCH_MUTEX, ace_mon, this->queue_->lock ());

  ACE_DEBUG ((LM_DEBUG, "begin dumping timer queue\n"));

  for (Timer_Heap_Iterator i (this->queue_->timer_queue ());
       i.item () != 0;
       i.next ())
    i.item ()->dump ();

  ACE_DEBUG ((LM_DEBUG, "end dumping timer queue\n"));
}

// constructor

Thread_Timer_Queue_Test_Driver::Thread_Timer_Queue_Test_Driver (void)
    : input_task_ (&timer_queue_, *this)
    {}

int 
Thread_Timer_Queue_Test_Driver::run_test (void)
{
  this->init ();
  return 0;
}

int 
Thread_Timer_Queue_Test_Driver::display_menu (void)
{ 
  static char menu[] = 
    "Usage:\n"
    "1 <microseconds>: setups a new timer\n"
    "2 <timerid>: removes a timer\n"
    "3 : prints timer queue\n"
    "4 : exit\n";
  
  ACE_DEBUG ((LM_DEBUG, "%s", menu)); 

  return 0; 
}

int 
Thread_Timer_Queue_Test_Driver::init (void)
{
  typedef Command<Input_Task, Input_Task::ACTION> COMMAND;

  // initialize the <Command> objects with their corresponding 
  // methods from <Input_Task>
  ACE_NEW_RETURN (schedule_cmd_, 
		  COMMAND (input_task_, &Input_Task::add_timer),
		  -1);
  
  ACE_NEW_RETURN (cancel_cmd_,
		  COMMAND (input_task_, &Input_Task::cancel_timer),
		  -1);

  ACE_NEW_RETURN (list_cmd_,
		  COMMAND (input_task_, &Input_Task::list_timer),
		  -1);

  ACE_NEW_RETURN (shutdown_cmd_,
		  COMMAND (input_task_, &Input_Task::shutdown_timer),
		  -1);

  if (input_task_.activate () == -1)
    ACE_ERROR_RETURN ((LM_ERROR, "cannot activate input task"), -1);
  
  if (timer_queue_.activate () == -1)
    ACE_ERROR_RETURN ((LM_ERROR, "cannot activate timer queue"), -1);
  
  if (ACE_Thread_Manager::instance ()->wait () == -1)
    ACE_ERROR_RETURN ((LM_ERROR, "wait on Thread_Manager failed"),-1);

  return 0;     
} 
    


#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)
template class ACE_Thread_Timer_Queue_Adapter<Timer_Heap>;
#elif defined (ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)
#pragma instantiate ACE_Thread_Timer_Queue_Adapter<Timer_Heap>
#endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */

#if !defined(ACE_MT_SAFE)

// These templates will specialized in liACE.* if the platforms does
// not define ACE_MT_SAFE.

#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)

template class ACE_Event_Handler_Handle_Timeout_Upcall<ACE_Null_Mutex>;
template class ACE_Timer_Heap_T<ACE_Event_Handler *, ACE_Event_Handler_Handle_Timeout_Upcall<ACE_Null_Mutex>, ACE_Null_Mutex>;
template class ACE_Timer_Heap_Iterator_T<ACE_Event_Handler *, ACE_Event_Handler_Handle_Timeout_Upcall<ACE_Null_Mutex>, ACE_Null_Mutex>;

#elif defined (ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)

#pragma instantiate ACE_Event_Handler_Handle_Timeout_Upcall<ACE_Null_Mutex>
#pragma instantiate ACE_Timer_Heap_T<ACE_Event_Handler *, ACE_Event_Handler_Handle_Timeout_Upcall<ACE_Null_Mutex>, ACE_Null_Mutex>
#pragma instantiate ACE_Timer_Heap_Iterator_T<ACE_Event_Handler *, ACE_Event_Handler_Handle_Timeout_Upcall<ACE_Null_Mutex>, ACE_Null_Mutex>

#endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */
#endif /* ACE_MT_SAFE */
