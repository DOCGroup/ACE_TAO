// $Id$

#include "ace/Sched_Params.h"
#include "orbsvcs/Scheduler_Factory.h"
#include "Memory_Pools.h"

#include "Dispatching_Modules.h"

#if !defined (__ACE_INLINE__)
#include "Dispatching_Modules.i"
#endif /* __ACE_INLINE__ */

// ************************************************************

void
ACE_ES_Dispatch_Request::make_copy (RtecEventComm::EventSet &dest) const
{
  if (use_single_event_)
    {
      dest.length (1);
      dest[0] = single_event_;
    }
  else
    {
      dest.length (event_set_.length ());

      for (CORBA::ULong index=0; index < event_set_.length (); index++)
	{
	  RtecEventComm::Event &dest_event = dest[index];
	  ACE_ES_Event_Container_var &source_event_var = ((ACE_ES_Event_Container_var &) event_set_[index]);
	  dest_event = *(source_event_var.operator->());
	}
    }
}

int
ACE_ES_Dispatch_Request::execute (u_long &command_action)
{
  ACE_TIMEPROBE ("  dispatch (dequeue) the event");

  return dispatching_module_->dispatch_event (this, command_action);
}

#if 0
void *
ACE_ES_Dispatch_Request::operator new (size_t nbytes)
{
  if (nbytes > sizeof (ACE_ES_Dispatch_Request))
    {
      ACE_ERROR ((LM_ERROR, "nbytes = %d, sizeof (ACE_ES_Dispatch_Request_Chunk) = %d.\n",
		  sizeof (ACE_ES_Dispatch_Request)));
      ACE_ASSERT (nbytes <= sizeof (ACE_ES_Dispatch_Request));
    }

  return ACE_ES_Memory_Pools::new_Dispatch_Request ();
}

void 
ACE_ES_Dispatch_Request::operator delete (void *mem)
{
  ACE_ES_Memory_Pools::delete_Dispatch_Request (mem);
}
#endif /* 0 */

// ************************************************************

ACE_ES_Priority_Dispatching::ACE_ES_Priority_Dispatching (ACE_EventChannel *channel,
							  int threads_per_queue)
  : ACE_ES_Dispatching_Base (channel),
    notification_strategy_ (this),
    highest_priority_ (0),
    shutdown_ (0),
    threads_per_queue_ (threads_per_queue)
{
  // If we're single threaded, then we need to use the notification strategy.
  if ((threads_per_queue_ == 0) &&
      (notification_strategy_.open () == -1))
    ACE_ERROR ((LM_ERROR, "%p.\n", "ACE_ES_Priority_Dispatching"));

  // Initialize the queues.
  for (int x=0; x < ACE_Scheduler_MAX_PRIORITIES; x++)
    {
      queues_[x] = 0;
      delete_me_queues_[x] = 0;
    }

  this->initialize_queues ();
}

ACE_ES_Priority_Dispatching::~ACE_ES_Priority_Dispatching (void)
{
  // Delete the queues.
  for (int x=0; x < ACE_Scheduler_MAX_PRIORITIES; x++)
    delete delete_me_queues_[x];
}


void
ACE_ES_Priority_Dispatching::initialize_queues (void)
{
  for (int x=0; x < ACE_Scheduler_MAX_PRIORITIES; x++)
    {
      RtecScheduler::Period tv = ACE_Scheduler_Rates[x];
      queues_[x] = new ACE_ES_Dispatch_Queue (this, &notification_strategy_); 
      if (queues_[x] == 0 ||
	  queues_[x]->open_queue (tv, threads_per_queue_) == -1)
	{
	  ACE_ERROR ((LM_ERROR, "%p.\n", "ACE_ES_Priority_Dispatching::initialize_queues"));
	  return;
	}

      queue_count_[x] = 1;
    }

  highest_priority_ = ACE_Scheduler_MAX_PRIORITIES - 1;
}

void 
ACE_ES_Priority_Dispatching::connected (ACE_Push_Consumer_Proxy *consumer, 
					CORBA::Environment &_env)
{
  down_->connected (consumer, _env);

  // This code does dynamic allocation of channel dispatch threads.
  // It requires that consumer's priorities are known at connection
  // time and that threads can request priorities from the scheduler
  // at run-time.  These are both antithetical to static scheduling.
  // The constructor now allocates a thread per rate group.
#if 0
  // We have to tell the lower portions of the channel about the
  // consumer first.  This is so that any changes to the consumer's
  // qos will take effect when we get the dispatch priority.
  down_->connected (consumer, _env);

  RtecScheduler::OS_Priority priority = 
    ACE_Scheduler::instance ().preemption_priority (consumer->qos ().rt_info_);

  {
    ACE_ES_GUARD ace_mon (lock_);
  
    // If a queue has not been created for the consumer's priority,
    // create one.
    if (queues_[priority] == 0)
      {
	// Allocate a new dispatch queue.
	queues_[priority] = new ACE_ES_Dispatch_Queue (this, &notification_strategy_);
	if (queues_[priority] == 0)
	  TAO_THROW (CORBA::NO_MEMORY (0, CORBA::COMPLETED_NO, 
					  "ACE_ES_Priority_Dispatching::connected"));

	// Initialize the dispatch queue corresponding to the
	// consumer's priority.  With a full implementation of the
	// run-time scheduler, the dispatch queue can find it's
	// scheduling qos online.  However, we pass the rate in case
	// one is not found.  The rate can be used to obtain the
	// proper priority.  If threads_per_queue_ == 0, then these
	// queues will be passive.  Otherwise, they will be active.
	// This switches us between MT_ORB and ST_ORB.  If we're
	// single-threaded, this registers us with the ReactorEx using
	// our notification_strategy_.  If we're multi-threaded, this
	// spawns the threads.
	if (queues_[priority]->open_queue (priority,
					   threads_per_queue_) == -1)
	  TAO_THROW (DISPATCH_ERROR (0, CORBA::COMPLETED_NO,
				     "ACE_ES_Priority_Dispatching::connected:"
				     "queue open failed.\n"));

	// When this goes down to 0, we will shutdown the queue.
	queue_count_[priority] = 1;

	// Keep track of this to optimize handle_signal.
	if (priority > highest_priority_)
	  highest_priority_ = priority;

	ACE_DEBUG ((LM_DEBUG, "Created queue priority = %d.\n", priority));
      }
    else
      queue_count_[priority]++;
  }
#endif
}

void 
ACE_ES_Priority_Dispatching::disconnected (ACE_Push_Consumer_Proxy *consumer)
{
  // We'll not dynamically close down queues.
  ACE_UNUSED_ARG (consumer);

#if 0
  RtecScheduler::OS_Priority priority = 
    ACE_Scheduler::instance ().preemption_priority (consumer->qos ().rt_info_);

  {
    ACE_ES_GUARD ace_mon (lock_);

    // If there are no more users of this queue, then we *could* shut
    // it down.  However, we will not.
    if (--queue_count_[priority] <= 0)
      {
	ACE_DEBUG ((LM_DEBUG, "(%t) unused dispatch queue priority = %d, "
		    "is_empty = %d.\n",
		    priority, queues_[priority]->msg_queue ()->is_empty ()));
      
	queues_[priority]->shutdown_task ();
      }
  }
#endif
}

// @@ This method could have a bypass optimization.
// <request> has been dynamically allocated by the filtering module.
void
ACE_ES_Priority_Dispatching::push (ACE_ES_Dispatch_Request *request,
				   CORBA::Environment &_env)
{
  ACE_TIMEPROBE ("  push_source_type: Correlation Module");

  RtecScheduler::OS_Priority thread_priority;
  RtecScheduler::Sub_Priority subpriority;
  RtecScheduler::Preemption_Priority preemption_priority;

  if (request->rt_info () != 0)
    {
      // @@ TODO use TAO_TRY&friends
      ACE_TIMEPROBE ("  Priority_Dispatching::push - priority requested");
      ACE_Scheduler_Factory::server ()->priority
	(request->rt_info (),
	 thread_priority,
	 subpriority,
	 preemption_priority,
	 _env);
      ACE_TIMEPROBE ("  Priority_Dispatching::push - priority obtained");
      if (_env.exception ())
	{
	  return;
	}
    }
  else
    {
      thread_priority =
	ACE_Sched_Params::priority_min (ACE_SCHED_FIFO,
					ACE_SCOPE_PROCESS);
      subpriority = ACE_Scheduler_MIN_SUB_PRIORITY;
      preemption_priority = ACE_Scheduler_MIN_PREEMPTION_PRIORITY;
    }
  
  // If it's a request to forward an event, it needs a reference to us
  // to call dispatch_event.
  request->set (this, preemption_priority, subpriority);

  // Make sure that a queue exists for this priority.
  if (queues_[preemption_priority] == 0)
    {
      ACE_ERROR ((LM_ERROR, "Push to closed queue %d, dropping event.\n", preemption_priority));
      return;
#if 0
      TAO_THROW (SYNC_ERROR (0, CORBA::COMPLETED_NO, "ACE_ES_Priority_Dispatching::push"));
#endif /* 0 */
    }

  // Enqueue the request.  If we're multi-threaded, this request is a
  // command object that will be called by the threads in the queue,
  // or will be dequeued by this->handle_signal if we're
  // single-threaded.
  if (queues_[preemption_priority]->try_put (request) == -1)
    {
      if (ACE_ES_Dispatch_Request::release (request) != 0)
	ACE_ERROR ((LM_ERROR, "ACE_ES_Priority_Dispatching::push"
		    " release failed.\n"));
      if (errno != EPIPE)
	{
	  TAO_THROW (CORBA::NO_MEMORY (CORBA::COMPLETED_NO));
	  // @@ Orbix parameters
	  // 0, CORBA::COMPLETED_NO, 
	  // "ACE_ES_Priority_Dispatching::push enqueue failed"));
	}
      else
	{
	  ACE_DEBUG ((LM_DEBUG,
		      "Request rejected from closed queue %d.\n",
		      preemption_priority));
	}
    }
}

// Start at highest priority queue checking for queued events
// continuing to lowest priority queue.  If an event is ever found,
// dispatch it and then start back at the highest priority queue
// again.
int 
ACE_ES_Priority_Dispatching::handle_signal (int, siginfo_t *, ucontext_t *)
{
  int done;

  do
    {
      done = 1;
      for (int x = 0; x <= highest_priority_; x++)
	{
	  // If the queue is not empty, dispatch the request and then
	  // start the for loop from the beginning.
	  if ((queues_[x] != 0) && (!queues_[x]->msg_queue ()->is_empty ()))
	    {
	      // Dequeue and service the request.
	      queues_[x]->svc_one ();

	      // Exit the for loop and start over.
	      done = 0;
	      break;
	    }

	  // If we get through the whole for loop without dispatching
	  // anything, then we're done.
	}
    }
  while (!done);


  return 0;
}

// This is only for the non-win32 single-threaded implementation.
int 
ACE_ES_Priority_Dispatching::handle_input (ACE_HANDLE)
{
  return this->handle_signal (0, 0, 0);
}

// Shutdown each queue.  When each queue exits, they will call back
// this->dispatch_queue_closed which allows us to free up resources.
// When the last queue has closed, we'll delete ourselves.
void
ACE_ES_Priority_Dispatching::shutdown (void)
{
  if (shutdown_)
    return;
 
  ACE_DEBUG ((LM_DEBUG, "(%t) ACE_ES_Priority_Dispatching "
	      "module shutting down.\n"));
 
  shutdown_ = 1;

  // If we're single threaded, then we need to shut down the
  // notification strategy so it can remove itself from the reactor.
  if (threads_per_queue_ == 0)
    notification_strategy_.shutdown ();

  // Whether these are active or not, they must be shut down.
  for (int x = 0; x <= highest_priority_; x++)
    if (queues_[x] != 0)
      {
	ACE_DEBUG ((LM_DEBUG, "shutting down dispatch queue %d.\n", x));
	queues_[x]->shutdown_task ();
      }
}

// This gets called every time a Dispatch Queue closes down.  We
// search for <queue> and delete it.  If we have been told to delete
// ourself, after the last queue has been deleted, we delete
// ourselves.
void
ACE_ES_Priority_Dispatching::dispatch_queue_closed (ACE_ES_Dispatch_Queue *queue)
{
  ACE_ES_GUARD ace_mon (lock_);

  // Find the queue.
  for (int x = 0; x <= highest_priority_; x++)
    {
      if (queues_[x] == queue)
	{
	  ACE_DEBUG ((LM_DEBUG, "(%t) Dispatch queue %d is closed.\n", x));

	  // Store the queue for deleting in this object's destructor.
	  delete_me_queues_[x] = queues_[x];
	  queues_[x] = 0;

	  // Reset highest_priority_.
	  if (x == highest_priority_)
	    {
	      while ((--highest_priority_ >= 0) &&
		     (queues_[highest_priority_] == 0));

	      if (highest_priority_ < 0)
		{
		  ACE_DEBUG ((LM_DEBUG, "Dispatching module shut down.\n"));
		  up_->shutdown ();
		  return;
		}
	    }

	  // If we found the queue, we can exit the for loop.
	  break;
	}
    }
}

/*
ACE_HANDLE  
ACE_ES_Priority_Dispatching::get_handle (void) const
{
  ACE_ES_Priority_Dispatching *fake_this = (ACE_ES_Priority_Dispatching *) this;
  return fake_this->notification_strategy_.get_handle ();
}
*/

// ************************************************************

ACE_ES_Dispatch_Queue::ACE_ES_Dispatch_Queue (ACE_ES_Dispatching_Base *dispatching_module,
					      ACE_ES_Notification_Strategy *notification_strategy)
  : dispatching_module_ (dispatching_module),
    notification_strategy_ (notification_strategy)
{
}

int
ACE_ES_Dispatch_Queue::open_queue (RtecScheduler::Period &period,
				   int threads)
{
  // First set up the correct message queue according to whether the
  // dispatch queue will be active or not.

  // If there are no threads specified, we'll register with the
  // reactor to be called back when someone queues a message.
  if (threads == 0)
    {
      // Allocate a message queue that notifies a reactor when events
      // arrive via the msg_queue call.  If that succeeds, set the
      // notification strategy in our message queue via the open call.
      if (this->msg_queue () == 0 ||
	  this->msg_queue ()->open (ACE_ES_QUEUE::DEFAULT_HWM,
				    ACE_ES_QUEUE::DEFAULT_LWM,
				    notification_strategy_) == -1)
	ACE_ERROR_RETURN ((LM_ERROR, "%p msg_queue.open failed.\n",
			   "ACE_ES_Dispatch_Queue::open_queue"), -1);
    }
  else
    {
      // Allocate a message queue that does not notify.
      ACE_ES_MQ *mq = new ACE_ES_MQ;
      if (mq == 0)
	ACE_ERROR_RETURN ((LM_ERROR, "%p.\n",
			   "ACE_ES_Dispatch_Queue::open_queue"), -1);
      else
	{
	  // This deletes previous message queue.
	  this->msg_queue (mq);
	  // Set this so that the destructor of ACE_Task deletes our
	  // message queue.  Note, this must be after the call to
	  // msg_queue.
	  delete_msg_queue_ = 1;
	}
    }

  // Create a name for ourselves using the priority.
  char temp[64];
  ACE_OS::sprintf (temp, "ACE_ES_Dispatch_Queue-%u", period);

  // Open the task.  This will query the scheduler for our qos
  // structure.  It will also synch_threads if it succeeds.
  int result = this->open_task (temp);

  switch (result)
    {
    case -1:
      // Error.
      ACE_ERROR_RETURN ((LM_ERROR, "%p.\n",
 			 "ACE_ES_Dispatch_Queue::open_queue"), -1);
    case 0:
      TAO_TRY
	{// @@ TODO: Handle exceptions...
	  ACE_Scheduler_Factory::server()->set(rt_info_,
					       0, 0, 0, 0,
					       RtecScheduler::VERY_LOW,
					       RtecScheduler::NO_QUANTUM,
					       1, TAO_TRY_ENV);
	  TAO_CHECK_ENV;
	}
      TAO_CATCHANY
	{
	  ACE_ERROR_RETURN ((LM_ERROR,
			     "ACE_ES_Display_Queue::exception"), -1);
	}
      TAO_ENDTRY;
    case 1:
      // Found.
      break;
    }

  // Spawn threads.
  return this->synch_threads (threads);
}

// This is called back by ACE_RT_Task when all the threads have
// exited.  We're going to forward this event to the dispatching
// module so it can clean up any resources.
void
ACE_ES_Dispatch_Queue::threads_closed (void)
{
  dispatching_module_->dispatch_queue_closed (this);
}

// ************************************************************

ACE_ES_EFD_Dispatching::ACE_ES_EFD_Dispatching (ACE_EventChannel *channel)
  : ACE_ES_Dispatching_Base (channel)
{
}

void
ACE_ES_EFD_Dispatching::push (ACE_ES_Dispatch_Request *request,
			      CORBA::Environment &)
{
  // If it's a request to forward an event, it needs a reference to us
  // to call dispatch_event.
  request->set (this, 0, ACE_Scheduler_MIN_SUB_PRIORITY);

  u_long command_action = ACE_RT_Task_Command::RELEASE;

  // This may be a request to delete a push consumer proxy, so we
  // should execute it instead of just forwarding it.
  request->execute (command_action);

  switch (command_action)
    {
    case ACE_RT_Task_Command::RELEASE:
      // Free the request.
      if (ACE_ES_Dispatch_Request::release (request) != 0)
	ACE_ERROR ((LM_ERROR, "ACE_ES_EFD_Dispatching::push"
		    " release failed.\n"));
      break;

    default:
      ACE_ERROR ((LM_ERROR, "ACE_ES_EFD_Dispatching::push: unknown command action.\n"));
    }
}

// ************************************************************

ACE_ES_RTU_Dispatching::ACE_ES_RTU_Dispatching (ACE_EventChannel *channel)
  : ACE_ES_Priority_Dispatching (channel, 0)
{
}

// We're called from a dispatch queue, so we can not release the request.
int 
ACE_ES_RTU_Dispatching::dispatch_event (ACE_ES_Dispatch_Request *request,
					u_long &command_action)
{
  // Store the priority of the task currently running.
  channel_->rtu_manager ()->priority (request->priority ());

  TAO_TRY
    {
      // Forward the request.
      up_->push (request, TAO_TRY_ENV);
      TAO_CHECK_ENV;
    }
  TAO_CATCHANY
    {
      ACE_ERROR ((LM_ERROR, "ACE_ES_RTU_Dispatching::dispatch_event unknown exception.\n"));
    }
  TAO_ENDTRY;

  // Reset the priority.
  channel_->rtu_manager ()->priority (ACE_Sched_Params::priority_min (ACE_SCHED_FIFO, ACE_SCOPE_PROCESS));

  // If the task was preempted, enqueue the request on the head of the
  // dispatch queue.
  if (channel_->rtu_manager ()->not_done ())
    command_action = ACE_RT_Task_Command::UNGETQ;
  else
    // We're done with it.
    command_action = ACE_RT_Task_Command::RELEASE;

  return 0;
}

void
ACE_ES_RTU_Dispatching::push (ACE_ES_Dispatch_Request *request,
			      CORBA::Environment &_env)
{
  // First enqueue the message in the proper queue.
  ACE_ES_Priority_Dispatching::push (request, _env);

  // If the current event is higher priority (lower value) than the
  // current running task, then tell the task to preempt itself.
  int request_priority = request->priority ();
  int running_priority = channel_->rtu_manager ()->priority ();
  if (request_priority < running_priority)
    channel_->rtu_manager ()->should_preempt (1);
  return;
}

#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)
template class ACE_CORBA_Sequence<ACE_CORBA_var<ACE_ES_Event_Container> >;
template void operator+=(ACE_CORBA_Sequence<ACE_CORBA_var<ACE_ES_Event_Container> > &, ACE_CORBA_var<ACE_ES_Event_Container> const &);

#elif defined(ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)

#pragma instantiate ACE_CORBA_Sequence<ACE_CORBA_var<ACE_ES_Event_Container> >
#pragma instantiate void operator+=(ACE_CORBA_Sequence<ACE_CORBA_var<ACE_ES_Event_Container> > &, ACE_CORBA_var<ACE_ES_Event_Container> const &)

#endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */
