
//=============================================================================
/**
*  @file    Reactor_Timer_Queue_Test.cpp
 *
 *  $Id$
 *
 *  This example tests the timer queue mechanism of ACE_Reactor.
 *
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



void
Reactor_Timer_Handler::set_timer_id (long tid)
{
  this->tid_ = tid;
}

int
Reactor_Timer_Handler::handle_timeout (const ACE_Time_Value &,
                                       const void *)
{
  ACE_Time_Value txv = ACE_OS::gettimeofday ();
  ACE_DEBUG ((LM_DEBUG,
              "\nTimer #%d fired at %d.%06d (%T)!\n",
              this->tid_,
              txv.sec (),
              txv.usec ()));
  delete this;

  return 0;
}

Input_Handler::Input_Handler (ACE_Timer_Queue *tq,
                              Reactor_Timer_Queue_Test_Driver &timer_queue_driver)
  : done_ (0),
    driver_ (timer_queue_driver)
{
  this->tq_ = tq;
}

int
Input_Handler::done (void)
{
  return this->done_;
}

int
Input_Handler::schedule_timer (void *argument)
{
  int delay = *(int *) argument;
  Reactor_Timer_Handler *th;
  long tid;

  th = new Reactor_Timer_Handler;
  if (th != 0)
    {
      tid = this->reactor ()->schedule_timer (th,
                                              0,
                                              ACE_Time_Value (0, delay));
      if (tid == -1)
        ACE_DEBUG ((LM_DEBUG,
                    "Unable to schedule timer\n"));
      else
        {
          ACE_DEBUG ((LM_DEBUG,
                      "Timer #%d schedule to fire after %d usec from now.\n",
                      tid,
                      delay));
          th->set_timer_id (tid);
        }
    }
  else
    ACE_ERROR_RETURN ((LM_ERROR,
                       "not enough memory?\n"),
                      -1);
  return tid;
}

int
Input_Handler::cancel_timer (void *argument)
{
  int id = *(int *) argument;
  return this->reactor ()->cancel_timer (id);
}

int
Input_Handler::shutdown_timer (void *)
{
  this->done_ = 1;
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
      ACE_DEBUG ((LM_DEBUG, "Timer #%d: %d.%06d\n",
                  tn->get_timer_id (),
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

Reactor_Timer_Queue_Test_Driver::Reactor_Timer_Queue_Test_Driver (void)
  : thandler_ (&timer_queue_, *this)
{
}

Reactor_Timer_Queue_Test_Driver::~Reactor_Timer_Queue_Test_Driver (void)
{
}

int
Reactor_Timer_Queue_Test_Driver::display_menu (void)
{
  static char menu[] =
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
Reactor_Timer_Queue_Test_Driver::init (void)
{
  typedef Command<Input_Handler, Input_Handler::ACTION> CMD;

  // initialize <Command>s with their corresponding <Input_Handler>  methods.
  ACE_NEW_RETURN (schedule_cmd_,
                  CMD (thandler_, &Input_Handler::schedule_timer),
                  -1);

  ACE_NEW_RETURN (cancel_cmd_,
                  CMD (thandler_, &Input_Handler::cancel_timer),
                  -1);

  ACE_NEW_RETURN (list_cmd_,
                  CMD (thandler_, &Input_Handler::list_timer),
                  -1);

  ACE_NEW_RETURN (shutdown_cmd_,
                  CMD (thandler_, &Input_Handler::shutdown_timer),
                  -1);

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
Reactor_Timer_Queue_Test_Driver::run_test (void)
{
  ACE_DEBUG ((LM_DEBUG,
              "TIMER TEST STARTED\n"));

  this->init ();

  // Run until we say stop.
  while (thandler_.done () == 0)
    ACE_Reactor::instance ()->handle_events ();

  ACE_DEBUG ((LM_DEBUG,
              "TIMER TEST ENDED\n"));
  return 0;
}
