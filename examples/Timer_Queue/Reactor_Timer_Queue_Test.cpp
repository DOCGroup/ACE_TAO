// $Id$

// ============================================================================
// = LIBRARY
//    examples
// 
// = FILENAME
//    Reactor_Timer_Queue_Test
//
// = DESCRIPTION
//    This example tests the timer queue mechanism of ACE_Reactor.
//
// = AUTHOR
//    Nanbor Wang <nw1@cs.wustl.edu>
// 
// ============================================================================

#include "ace/Thread_Manager.h"
#include "ace/Select_Reactor.h"
#include "ace/Reactor.h"
#include "ace/Timer_Heap.h"

#include "Driver.h"
#include "Reactor_Timer_Queue_Test.h"

static const int NO_OF_IO_HANDLERS = 5;
#define  REACTOR  ACE_Reactor::instance ()

Timer_Handler::Timer_Handler (void)
{}

void 
Timer_Handler::set_timer_id (long tid)
{
  this->tid_ = tid;
}

int 
Timer_Handler::handle_timeout (const ACE_Time_Value &tv,
			       const void *)
{
  ACE_Time_Value txv = ACE_OS::gettimeofday ();
  ACE_DEBUG ((LM_DEBUG, "\nTimer #%d fired at %d.%06d (%T)!\n",
	      this->tid_, txv.sec (), txv.usec ()));
  delete this;

  return 0;
}

Input_Handler::Input_Handler (ACE_Timer_Queue *tq, Reactor_Timer_Queue_Test_Driver &timer_queue_driver)
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
  Timer_Handler *th;
  long tid;

  th = new Timer_Handler;
  if (th != 0)
    {
      tid = this->reactor ()->schedule_timer (th, 
					      0, 
					      ACE_Time_Value (0, delay));
      if (tid == -1)
	ACE_DEBUG ((LM_DEBUG, "Unable to schedule timer\n"));
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
    ACE_ERROR_RETURN ((LM_ERROR, "not enough memory?\n"), -1);

  return tid;
}
  
int 
Input_Handler::cancel_timer (void *argument)
{
  int id = *(int *) argument;
  return this->reactor ()->cancel_timer (id);
}

int
Input_Handler::shutdown_timer (void *argument)
{
  this->done_ = 1;
  ACE_DEBUG ((LM_DEBUG, "Shutting down event loop\n"));
  return -1;
}
  
int
Input_Handler::list_timer (void *argument)
{
  ACE_Timer_Queue_Iterator &iter = this->tq_->iter ();
  ACE_DEBUG ((LM_DEBUG, "\n\nTimers in queue:\n"));
          
  for (; ! iter.isdone (); iter.next ())
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
    : thandler (&timer_queue_, *this)
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
  
  ACE_DEBUG ((LM_DEBUG, "%s", menu));  

  return  0;
}

int 
Reactor_Timer_Queue_Test_Driver::init (void)
{
  // initialize commands with their corresponding input_task methods.
  ACE_NEW_RETURN (schedule_cmd_, 
		  Command<Input_Handler> (thandler,
					  thandler.schedule_timer),
		  -1);
  
  ACE_NEW_RETURN (cancel_cmd_,
		  Command<Input_Handler> (thandler,
					  thandler.cancel_timer),
		  -1);

  ACE_NEW_RETURN (list_cmd_,
		  Command<Input_Handler> (thandler,
					  thandler.list_timer),
		  -1);

  ACE_NEW_RETURN (shutdown_cmd_,
		  Command<Input_Handler> (thandler,
					  thandler.shutdown_timer),
		  -1);

  REACTOR->set_timer_queue (&timer_queue_);

  ACE::register_stdin_handler (&thandler, REACTOR,
                               ACE_Thread_Manager::instance ());

  this->display_menu ();
}

int 
Reactor_Timer_Queue_Test_Driver::run_test (void) 
{     
  ACE_DEBUG ((LM_DEBUG, "TIMER TEST STARTED\n"));

  this->init ();

  // Run until we say stop.
  while (thandler.done () == 0)
    REACTOR->handle_events ();

  ACE_DEBUG ((LM_DEBUG, "TIMER TEST ENDED\n"));
  return 0;
}
