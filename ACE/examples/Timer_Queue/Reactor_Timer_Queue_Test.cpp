
//=============================================================================
/**
*  @file    Reactor_Timer_Queue_Test.cpp
 *
 *  This example tests the timer queue mechanism of ACE_Reactor.
 *
 *  @author Nanbor Wang <nw1@cs.wustl.edu> and Sergio Flores-Gaitan <sergio@cs.wustl.edu>
 */
//=============================================================================


#include "ace/OS_NS_sys_time.h"
#include "ace/Thread_Manager.h"
#include "ace/Select_Reactor.h"
#include "ace/Reactor.h"
#include "ace/Timer_Heap.h"

#include "Driver.h"
#include "Reactor_Timer_Queue_Test.h"
#include <memory>
#include <new>


void
Reactor_Timer_Handler::set_timer_id (long tid)
{
  this->tid_ = tid;
}

int
Reactor_Timer_Handler::handle_timeout (ACE_Time_Value const &,
                                       void const *)
{
  ACE_Time_Value const txv = ACE_OS::gettimeofday ();
  ACE_DEBUG ((LM_DEBUG,
              "\nTimer #%q fired at %:.%06d (%T)!\n",
              static_cast<ACE_INT64> (this->tid_),
              txv.sec (),
              txv.usec ()));
  // Reactor callbacks own and delete their one-shot handler.
  delete this;

  return 0;
}

Input_Handler::Input_Handler (ACE_Timer_Queue *tq,
                              Reactor_Timer_Queue_Test_Driver &timer_queue_driver)
  : tq_ (tq),
    driver_ (timer_queue_driver)
{
}

bool
Input_Handler::done () const
{
  return this->done_;
}

int
Input_Handler::schedule_timer (void *argument)
{
  u_int const delay = *static_cast<u_int const *> (argument);
  std::unique_ptr<Reactor_Timer_Handler> handler (
    new (std::nothrow) Reactor_Timer_Handler);
  if (!handler)
    ACE_ERROR_RETURN ((LM_ERROR,
                       "not enough memory?\n"),
                      -1);

  long const tid = this->reactor ()->schedule_timer (handler.get (),
                                                     handler.get (),
                                                     ACE_Time_Value (0, delay));
  if (tid == -1)
    ACE_ERROR_RETURN ((LM_DEBUG,
                       "Unable to schedule timer\n"),
                      -1);

  handler->set_timer_id (tid);
  handler.release ();
  ACE_DEBUG ((LM_DEBUG,
              "Timer #%q scheduled to fire after %u usec from now.\n",
              static_cast<ACE_INT64> (tid),
              delay));
  return 0;
}

int
Input_Handler::cancel_timer (void *argument)
{
  long const id = *static_cast<long const *> (argument);
  void const *act {};
  int const canceled = this->reactor ()->cancel_timer (id, &act);
  if (canceled > 0)
    {
      std::unique_ptr<Reactor_Timer_Handler> handler (
        static_cast<Reactor_Timer_Handler *> (const_cast<void *> (act)));
    }
  return canceled;
}

int
Input_Handler::shutdown_timer (void *)
{
  this->done_ = true;
  ACE_DEBUG ((LM_DEBUG,
              "Shutting down event loop\n"));
  return -1;
}

int
Input_Handler::list_timer (void *)
{
  ACE_Timer_Queue_Iterator &iter = this->tq_->iter ();
  ACE_DEBUG ((LM_DEBUG,
              "\n\nTimers in queue:\n"));

  for (; !iter.isdone (); iter.next ())
    {
      ACE_Timer_Node *tn = iter.item ();
      ACE_DEBUG ((LM_DEBUG, "Timer #%q: %:.%06d\n",
                  static_cast<ACE_INT64> (tn->get_timer_id ()),
                  tn->get_timer_value ().sec (),
                  tn->get_timer_value ().usec ()));
    }
  return 0;
}

int
Input_Handler::handle_input (ACE_HANDLE)
{
  return driver_.get_next_request ();
}

Reactor_Timer_Queue_Test_Driver::Reactor_Timer_Queue_Test_Driver ()
  : thandler_ (&timer_queue_, *this)
{
}

Reactor_Timer_Queue_Test_Driver::~Reactor_Timer_Queue_Test_Driver ()
{
  // unhook our timer queue
  ACE_Reactor::instance ()->timer_queue (nullptr);
}

int
Reactor_Timer_Queue_Test_Driver::display_menu ()
{
  static char const menu[] =
    "\n*****\n"
    "1) Schedule timer <usec>\n"
    "2) Cancel timer <id>\n"
    "3) List all timers\n"
    "4) Shutdown program\n"
    "Enter selection:";

  ACE_DEBUG ((LM_DEBUG,
              "%s",
              menu));
  return 0;
}

int
Reactor_Timer_Queue_Test_Driver::init ()
{
  using CMD = Command<Input_Handler, Input_Handler::ACTION>;

  // Initialize <Command>s with their corresponding <Input_Handler> methods.

  CMD *raw_schedule_cmd {};
  ACE_NEW_RETURN (raw_schedule_cmd,
                  CMD (thandler_, &Input_Handler::schedule_timer),
                  -1);
  schedule_cmd_.reset (raw_schedule_cmd);

  CMD *raw_cancel_cmd {};
  ACE_NEW_RETURN (raw_cancel_cmd,
                  CMD (thandler_, &Input_Handler::cancel_timer),
                  -1);
  cancel_cmd_.reset (raw_cancel_cmd);

  CMD *raw_list_cmd {};
  ACE_NEW_RETURN (raw_list_cmd,
                  CMD (thandler_, &Input_Handler::list_timer),
                  -1);
  list_cmd_.reset (raw_list_cmd);

  CMD *raw_shutdown_cmd {};
  ACE_NEW_RETURN (raw_shutdown_cmd,
                  CMD (thandler_, &Input_Handler::shutdown_timer),
                  -1);
  shutdown_cmd_.reset (raw_shutdown_cmd);

  ACE_Reactor::instance ()->timer_queue (&timer_queue_);

  ACE_Event_Handler::register_stdin_handler (&thandler_,
                                             ACE_Reactor::instance (),
                                             ACE_Thread_Manager::instance ());

  // print the menu of options.
  this->display_menu ();

  return 0;
}

// run test was overrun due to the reactive way of handling input.

int
Reactor_Timer_Queue_Test_Driver::run_test ()
{
  ACE_DEBUG ((LM_DEBUG,
              "TIMER TEST STARTED\n"));

  if (this->init () == -1)
    return -1;

  // Run until we say stop.
  while (!thandler_.done ())
    ACE_Reactor::instance ()->handle_events ();

  ACE_DEBUG ((LM_DEBUG,
              "TIMER TEST ENDED\n"));
  return 0;
}
