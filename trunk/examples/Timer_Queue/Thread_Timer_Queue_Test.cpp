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

// These typedefs ensure that we use the minimal amount of locking
// necessary.
typedef ACE_Event_Handler_Handle_Timeout_Upcall<ACE_Null_Mutex> 
	Upcall;
typedef ACE_Timer_Heap_T<ACE_Event_Handler *,
	                 ACE_Event_Handler_Handle_Timeout_Upcall<ACE_Null_Mutex>,
			 ACE_Null_Mutex>
	Timer_Heap;
typedef ACE_Timer_Heap_Iterator_T<ACE_Event_Handler *,
                                  ACE_Event_Handler_Handle_Timeout_Upcall<ACE_Null_Mutex>,
				  ACE_Null_Mutex> 
        Timer_Heap_Iterator;
typedef Thread_Timer_Queue_Adapter<Timer_Heap> 
        Thread_Timer_Queue;

class Handler : public ACE_Event_Handler 
  // = TITLE
  //    This class implements a simple Event_Handler,
  //
  // = DESCRIPTION
  //     The <handle_timeout> hook method justs printouts the current
  //     time, delete this and prints the delay on the twhen it is
  //     expired. 
{
public:
  Handler (const ACE_Time_Value &expiration_time);
  ~Handler (void);

  void set_id (int id);
  // Store an "id" for the Handler, which is only use to print better
  // messages.

  virtual int handle_timeout (const ACE_Time_Value &current_time,
			      const void *arg);
  // Call back hook.
  virtual int cancelled (void);

private:
  int id_;
  // Store an "id" for the Handler, which is only use to print better
  // messages.
};

class Input_Task : public ACE_Task<ACE_SYNCH>
  // = TITLE
  //   Read user actions on the Timer_Queue from stdin.
  //
  // = DESCRIPTION
  //   This class reads user input from stdin; those commands permit the
  //   control of a Timer_Queue, which is dispatched by another thread.
{
public:
  Input_Task (Thread_Timer_Queue* queue);

  virtual int svc (void);
  // The method run on the new thread.

private:
  // Some helper methods.
  void usage (void) const;
  int add_timer (u_long seconds);
  void cancel_timer (int id);
  int parse_commands (const char *buffer);
  void dump (void);

private:
  Thread_Timer_Queue *queue_;

  const int usecs_;
};

// Administrivia methods...
Handler::Handler(const ACE_Time_Value& expiration_time)
  :  expires(expiration_time),
     id(0)
{
}

Handler::~Handler()
{
}

void 
Handler::set_id (int id)
{
  id_ = id;
}

// This is the method invoked when the Timer expires.

int 
Handler::handle_timeout(const ACE_Time_Value &current_time,
 			const void*)
{
  ACE_Time_Value delay = current_time - expires;

  // No need to protect this printf is always called from a Async safe
  // point.
  ACE_OS::printf ("\nexpiring timer %d at %u.%07.7u secs\n"
		  "\tthere was a %u.%07.7u secs delay\n",
		  id, current_time.sec (), current_time.usec (),
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

Input_Task::Input_Task (Thread_Timer_Queue *queue)
  : queue_ (queue),
    usecs_ (1000000) // @@ Make this an ACE #define constant?
{
}

int 
Input_Task::svc (void)
{
  for (;;)
    {
      char buf[BUFSIZ];

      ACE_OS::printf ("please enter your choice: ");
      ACE_OS::fflush (stdout);

      if (ACE_OS::read (ACE_STDIN, buf, sizeof buf) <= 0)
	break;

      if (parse_commands (buf) < 0)
	break;
    }

  this->queue_->deactivate ();
  ACE_DEBUG ((LM_DEBUG, "terminating input thread\n"));
  return 0;
}

int
Input_Task::add_timer (u_long useconds)
{
  ACE_Time_Value interval (useconds / usecs_, useconds % usecs_);
  ACE_Time_Value expire_at = ACE_OS::gettimeofday () + interval;

  Handler *h;

  ACE_NEW_RETURN (h, Handler (expire_at));

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

void 
Input_Task::cancel_timer (int id)
{
  this->queue_->cancel (id);
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

void 
Input_Task::usage (void) const
{
  ACE_OS::printf ("Usage:\n"
		  "1 <microseconds>: setups a new timer\n"
		  "2 <timerid>: removes a timer\n"
		  "3 : prints timer queue\n"
		  "4 : exit\n");
}

int 
Input_Task::parse_commands (const char *buf)
{
  int option;

  if (::sscanf (buf, "%d", &option) <= 0)
    {
      // If there was an error reading the option simply print thge
      // usage and try on the next line.
      this->usage ();
      return 0;
    }

  switch (option)
    {
    case 1: // Install a new timer.
      {
	u_long useconds;
	// We just reread the option, this simplies parsing (since
	// sscanf can do it for us.)
	if (::sscanf (buf, "%d %lu", &option, &useconds) < 2)
	  {
	    this->usage ();
	    return 0;
	  }
	if (this->add_timer (useconds) == -1)
	  ACE_ERROR_RETURN ((LM_ERROR, "%t %p\n", "new timer failed"), -1);
      }
    break; // Cancel an existing timer.
    case 2:
      {
	u_long id;
	// We just reread the option, this simplies parsing (since
	// sscanf can do it for us.)
	if (::sscanf (buf, "%d %lu", &option, &id) < 2)
	  {
	    this->usage ();
	    return 0;
	  }
	this->cancel_timer (id);
      }
      break;

    case 3: // Dump the existing timers.
      this->dump ();
      break;

    case 4: // Exit the program.
      return -1;
      break;

    default:
      this->usage ();
      break;
    }
  return 0;
}

int 
main (int argc, char* argv[])
{
  Thread_Timer_Queue tq;

  Input_Task input_task (&tq);
  
  if (input_task.activate () == -1)
    ACE_ERROR_RETURN ((LM_ERROR, "cannot activate input task"), -1);
  
  if (tq.activate () == -1)
    ACE_ERROR_RETURN ((LM_ERROR, "cannot activate timer queue"), -1);
  
  if (ACE_Thread_Manager::instance ()->wait () == -1)
    ACE_ERROR_RETURN ((LM_ERROR, "wait on Thread_Manager failed"),-1);

  return 0;
}

// @@ Add template specializations.

#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)
template class ACE_Thread_Timer_Queue_Adapter<ACE_Timer_Heap>;
#elif defined (ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)
#pragma instantiate ACE_Thread_Timer_Queue_Adapter<ACE_Timer_Heap>
#endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */
