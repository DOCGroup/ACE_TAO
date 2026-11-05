
//=============================================================================
/**
 *  @file    Thread_Timer_Queue_Test.cpp
 *
 *    This test exercises the <ACE_Thread_Timer_Queue_Adapter>
 *    using an <ACE_Timer_Heap>.
 *
 *  @author Carlos O'Ryan <coryan@cs.wustl.edu> and Douglas C. Schmidt <d.schmidt@vanderbilt.edu>
 */
//=============================================================================


#include "ace/OS_NS_sys_time.h"
#include "ace/Task.h"
#include "ace/Timer_Heap_T.h"
#include "ace/Timer_Queue_Adapters.h"
#include <atomic>
#include <cstdio>
#include <memory>
#include <new>

#include "Thread_Timer_Queue_Test.h"

#include "ace/Condition_T.h"
#include "ace/Thread_Mutex.h"
// Administrivia methods...
int
Upcall::deletion (ACE_Timer_Queue &,
                  ACE_Event_Handler *handler,
                  void const *)
{
  // The timer queue owns handlers that remain scheduled when it is closed.
  delete handler;
  return 0;
}

Handler::Handler (ACE_Time_Value const &expiration_time,
                  std::shared_ptr<std::atomic<long>> const &id)
  : expires_ (expiration_time),
    id_ (id)
{
}

// This is the method invoked when the Timer expires.

int
Handler::handle_timeout (ACE_Time_Value const &current_time,
                         void const *)
{
  ACE_Time_Value const delay = current_time - this->expires_;

  // Print the timer details from its callback.
  std::printf ("\nexpiring timer %ld at %lu.%7.7lu secs\n"
                  "\tthere was a %lu.%7.7lu secs delay\n",
                  this->id_->load (std::memory_order_relaxed),
                  static_cast<unsigned long> (current_time.sec ()),
                  static_cast<unsigned long> (current_time.usec ()),
                  static_cast<unsigned long> (delay.sec ()),
                  static_cast<unsigned long> (delay.usec ()));
  // Notice this delete is protected.
  // The timer queue owns and deletes this one-shot handler after its callback.
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
Input_Task::svc ()
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
  u_int const useconds = *static_cast<u_int const *> (argument);
  ACE_Time_Value const interval (useconds / usecs_,
                                useconds % usecs_);
  ACE_Time_Value const expire_at = ACE_OS::gettimeofday () + interval;

  // The callback may run before schedule returns and delete its handler.
  // Keep the ID state separately owned so it remains valid in either order.
  std::shared_ptr<std::atomic<long>> timer_id (
    std::make_shared<std::atomic<long>> (-1));
  std::unique_ptr<Handler> handler (new (std::nothrow) Handler (expire_at, timer_id));
  if (!handler)
    ACE_ERROR_RETURN ((LM_ERROR,
                       "handler allocation failed"),
                      -1);

  Handler *const raw_handler = handler.release ();
  long const id = queue_->schedule (raw_handler, raw_handler, expire_at);

  if (id == -1)
    {
      delete raw_handler;
      ACE_ERROR_RETURN ((LM_ERROR,
                         "schedule failed"),
                        -1);
    }

  // The ID is only used to produce nicer messages.
  timer_id->store (id, std::memory_order_relaxed);

  std::printf ("scheduling timer %ld\n", id);
  return 0;
}

// Cancel a timer.  This method will be called from inside the
//  <Timer_Queue_Test_Driver> class.  (see Command pattern)

int
Input_Task::cancel_timer (void *argument)
{
  long const id = *static_cast<long const *> (argument);
  void const *act {};
  int const canceled = this->queue_->cancel (id, &act);
  if (canceled > 0)
    {
      std::unique_ptr<Handler> handler (
        static_cast<Handler *> (const_cast<void *> (act)));
    }
  return canceled;
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
Input_Task::dump ()
{
  ACE_GUARD (ACE_SYNCH_RECURSIVE_MUTEX, ace_mon, this->queue_->mutex ());

  ACE_DEBUG ((LM_DEBUG,
              "begin dumping timer queue\n"));

  for (Timer_Heap_Iterator i (*this->queue_->timer_queue ());
       i.item () != nullptr;
       i.next ())
    i.item ()->dump ();

  ACE_DEBUG ((LM_DEBUG,
              "end dumping timer queue\n"));
}

// constructor

Thread_Timer_Queue_Test_Driver::Thread_Timer_Queue_Test_Driver ()
  : input_task_ (&timer_queue_, *this)
{
}

int
Thread_Timer_Queue_Test_Driver::run_test ()
{
  return this->init ();
}

int
Thread_Timer_Queue_Test_Driver::display_menu ()
{
  static char const menu[] =
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
Thread_Timer_Queue_Test_Driver::init ()
{
  using CMD = Command<Input_Task, Input_Task::ACTION>;

  // initialize the <Command> objects with their corresponding
  // methods from <Input_Task>
  CMD *raw_schedule_cmd {};
  ACE_NEW_RETURN (raw_schedule_cmd,
                  CMD (input_task_, &Input_Task::add_timer),
                  -1);
  schedule_cmd_.reset (raw_schedule_cmd);

  CMD *raw_cancel_cmd {};
  ACE_NEW_RETURN (raw_cancel_cmd,
                  CMD (input_task_, &Input_Task::cancel_timer),
                  -1);
  cancel_cmd_.reset (raw_cancel_cmd);

  CMD *raw_list_cmd {};
  ACE_NEW_RETURN (raw_list_cmd,
                  CMD (input_task_, &Input_Task::list_timer),
                  -1);
  list_cmd_.reset (raw_list_cmd);

  CMD *raw_shutdown_cmd {};
  ACE_NEW_RETURN (raw_shutdown_cmd,
                  CMD (input_task_, &Input_Task::shutdown_timer),
                  -1);
  shutdown_cmd_.reset (raw_shutdown_cmd);

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
