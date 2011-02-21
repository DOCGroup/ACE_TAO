
//=============================================================================
/**
 *  @file    Thread_Timer_Queue_Test.cpp
 *
 *  $Id$
 *
 *    This test exercises the <ACE_Thread_Timer_Queue_Adapter>
 *    using an <ACE_Timer_Heap>.
 *
 *
 *  @author Carlos O'Ryan <coryan@cs.wustl.edu> and Douglas C. Schmidt <schmidt@cs.wustl.edu>
 */
//=============================================================================


#include "ace/OS_NS_stdio.h"
#include "ace/OS_NS_sys_time.h"
#include "ace/Task.h"
#include "ace/Timer_Heap_T.h"
#include "ace/Timer_Queue_Adapters.h"

#include "Thread_Timer_Queue_Test.h"

#include "ace/Condition_T.h"
#include "ace/Thread_Mutex.h"



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
  ACE_OS::printf ("\nexpiring timer %d at %lu.%7.7lu secs\n"
                  "\tthere was a %lu.%7.7lu secs delay\n",
                  this->id_,
                  current_time.sec (),
                  current_time.usec (),
                  delay.sec (),
                  delay.usec ());
  // Notice this delete is protected.
  delete this;
  return 0;
}

Input_Task::Input_Task (Thread_Timer_Queue *queue,
                        Thread_Timer_Queue_Test_Driver &timer_queue_driver)
  : ACE_Task_Base (ACE_Thread_Manager::instance ()),
    queue_ (queue),
    usecs_ (ACE_ONE_SECOND_IN_USECS),
    driver_ (timer_queue_driver)
{
}

// Svc method is called from the thread library to read input from the
// user.

int
Input_Task::svc (void)
{
  for (;;)
    // call back to the driver's implementation on how to read and
    // parse input.
    if (this->driver_.get_next_request () == -1)
      break;

  // we are done.
  this->queue_->deactivate ();
  ACE_DEBUG ((LM_DEBUG,
              "terminating input thread\n"));
  return 0;
}

// schedule a new timer.  This method will be called from inside the
//  <Timer_Queue_Test_Driver> class.  (see Command pattern)

int
Input_Task::add_timer (void *argument)
{
  u_long useconds = *reinterpret_cast<int *> (argument);
  ACE_Time_Value interval (useconds / usecs_,
                           useconds % usecs_);
  ACE_Time_Value expire_at = ACE_OS::gettimeofday () + interval;

  Handler *h;

  ACE_NEW_RETURN (h,
                  Handler (expire_at),
                  -1);

  int id = queue_->schedule (h, 0, expire_at);

  if (id == -1)
    ACE_ERROR_RETURN ((LM_ERROR,
                       "schedule failed"),
                      -1);

  // We store the id into the handler, this is only used to produce
  // nicer messages.
  h->set_id (id);

  ACE_OS::printf ("scheduling timer %d\n",
                  id);
  return 0;
}

// Cancel a timer.  This method will be called from inside the
//  <Timer_Queue_Test_Driver> class.  (see Command pattern)

int
Input_Task::cancel_timer (void *argument)
{
  return this->queue_->cancel (*reinterpret_cast<int *> (argument));
}

// Lists the timers in the queue.  Ignores the argument. This method
// will be called from inside the <Timer_Queue_Test_Driver> class.
// (see Command pattern)

int
Input_Task::list_timer (void *)
{
  // Dump the timer queue contents.
  this->dump ();

  return 0;
}

// Shutdown the timer queue.  Return -1 indicates to the
// <Timer_Queue_Test_Driver> class that we are done.

int
Input_Task::shutdown_timer (void *)
{
#if defined (ACE_LACKS_PTHREAD_CANCEL)
  // Cancel the thread timer queue task "voluntarily."
  this->queue_->deactivate ();
#else
  // Cancel the thread timer queue task "preemptively."
  if (ACE_Thread::cancel (this->queue_->thr_id ()) == -1)
    ACE_ERROR ((LM_ERROR,
                "%p\n",
                "cancel"));
#endif /* ACE_LACKS_PTHREAD_CANCEL */

  // -1 indicates we are shutting down the application.
  return -1;
}

void
Input_Task::dump (void)
{
  ACE_GUARD (ACE_SYNCH_RECURSIVE_MUTEX, ace_mon, this->queue_->mutex ());

  ACE_DEBUG ((LM_DEBUG,
              "begin dumping timer queue\n"));

  for (Timer_Heap_Iterator i (*this->queue_->timer_queue ());
       i.item () != 0;
       i.next ())
    i.item ()->dump ();

  ACE_DEBUG ((LM_DEBUG,
              "end dumping timer queue\n"));
}

// constructor

Thread_Timer_Queue_Test_Driver::Thread_Timer_Queue_Test_Driver (void)
  : input_task_ (&timer_queue_, *this)
{
}

Thread_Timer_Queue_Test_Driver::~Thread_Timer_Queue_Test_Driver (void)
{
}

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

  ACE_DEBUG ((LM_DEBUG,
              "%s",
              menu));
  return 0;
}

int
Thread_Timer_Queue_Test_Driver::init (void)
{
  typedef Command<Input_Task, Input_Task::ACTION> CMD;

  // initialize the <Command> objects with their corresponding
  // methods from <Input_Task>
  ACE_NEW_RETURN (schedule_cmd_,
                  CMD (input_task_, &Input_Task::add_timer),
                  -1);

  ACE_NEW_RETURN (cancel_cmd_,
                  CMD (input_task_, &Input_Task::cancel_timer),
                  -1);

  ACE_NEW_RETURN (list_cmd_,
                  CMD (input_task_, &Input_Task::list_timer),
                  -1);

  ACE_NEW_RETURN (shutdown_cmd_,
                  CMD (input_task_, &Input_Task::shutdown_timer),
                  -1);

  if (this->input_task_.activate () == -1)
    ACE_ERROR_RETURN ((LM_ERROR,
                       "cannot activate input task"),
                      -1);
  else if (this->timer_queue_.activate () == -1)
    ACE_ERROR_RETURN ((LM_ERROR,
                       "cannot activate timer queue"),
                      -1);
  else if (ACE_Thread_Manager::instance ()->wait () == -1)
    ACE_ERROR_RETURN ((LM_ERROR,
                       "wait on Thread_Manager failed"),
                      -1);
  return 0;
}
