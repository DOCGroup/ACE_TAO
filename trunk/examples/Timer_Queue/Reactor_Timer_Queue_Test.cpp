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

static const int NO_OF_IO_HANDLERS = 5;
#define  REACTOR  ACE_Reactor::instance ()

static void usage_prompt (void)
{
  ACE_DEBUG ((LM_DEBUG, "\n*****\n"
              "1) Schedule timer <usec>\n"
              "2) Cancel timer <id>\n"
              "3) List all timers\n"
              "4) Shutdown program\n"
              "Enter selection:"));
}

class Timer_Handler : public ACE_Event_Handler
{
public:
  virtual int handle_timeout (const ACE_Time_Value &tv,
                              const void *)
  {
    ACE_Time_Value txv = ACE_OS::gettimeofday ();
    ACE_DEBUG ((LM_DEBUG, "\nTimer #%d fired at %d.%06d (%T)!\n",
                this->tid_, txv.sec (), txv.usec ()));
    delete this;
    ::usage_prompt ();
    return 0;
  }

  void set_timer_id (long tid)
  {
    this->tid_ = tid;
  }
  
private:
  long tid_;
};

class Input_Handler : public ACE_Event_Handler
{
public:
  Input_Handler (ACE_Timer_Queue *tq)
    : done_ (0)
  {
    this->tq_ = tq;
  }

  int done (void)
  {
    return this->done_;
  }

  virtual int handle_input (ACE_HANDLE)
  {
    char buffer [MAXPATHLEN];
    int c = 0;
    long tv = 0;
    Timer_Handler *th;

    ssize_t n = ACE_OS::read (ACE_STDIN, buffer, sizeof buffer);
    if (n == -1)
      ACE_ERROR_RETURN ((LM_ERROR, "Huh?\n"), -1);
    
    sscanf (buffer, "%d %ld", &c, &tv);
    switch (c)
      {
      case 1:                   // Schedule timer.
        th = new Timer_Handler;
        if (th != 0)
          {
            long tid = this->reactor ()->schedule_timer
              (th, 0, ACE_Time_Value (0, tv));
            if (tid == -1)
              ACE_DEBUG ((LM_DEBUG, "Unable to schedule timer\n"));
            else
              {
                ACE_DEBUG ((LM_DEBUG,
                            "Timer #%d schedule to fire after %d usec from now.\n",
                            tid, tv));
                th->set_timer_id (tid);
              }
          }
        else
          ACE_ERROR_RETURN ((LM_ERROR, "not enought memeory?\n"), -1);
        break;
      case 2:                   // Cancel timer <id>
        if (this->reactor ()->cancel_timer (tv) == 0)
          ACE_DEBUG ((LM_DEBUG, "Timer #%d is not valid\n", tv));
        break;
      case 3:                   // Dump all timer in queue.
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
        }
        break;
      case 4:                   // Shutdown
        this->done_ = 1;
        ACE_DEBUG ((LM_DEBUG, "Shutting down event loop\n"));
        return -1;
      default:                  // Huh?
        ACE_DEBUG ((LM_DEBUG, "Got string (%d): %s\n", n, buffer));
        break;
      }
    ::usage_prompt ();
    return 0;
  }
private:
  ACE_Timer_Queue *tq_;
  // Keep a pointer to the timer queue we are using so we can
  // traverse the queue.

  int done_;
  // Flag used to close down program.
};

int
main (int, char *[])
{
  ACE_DEBUG ((LM_DEBUG, "TIMER TEST STARTED\n"));

  // pick a timer queue implementation
  // which happens to be the one that ACE is using.
  ACE_Timer_Heap private_queue;

  REACTOR->set_timer_queue (&private_queue);

  // This is the stdin handler.
  Input_Handler *thandler = new Input_Handler (&private_queue);

  ACE::register_stdin_handler (thandler, REACTOR,
                               ACE_Thread_Manager::instance ());

  ::usage_prompt ();

  // Run until we say stop.
  while (thandler->done () == 0)
    REACTOR->handle_events ();

  ACE_DEBUG ((LM_DEBUG, "TIMER TEST ENDED\n"));
  return 0;
}
