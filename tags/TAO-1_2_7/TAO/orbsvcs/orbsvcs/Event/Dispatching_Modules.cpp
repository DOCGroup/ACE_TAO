// $Id$

#include "ace/Sched_Params.h"
#include "orbsvcs/Scheduler_Factory.h"
#include "orbsvcs/Time_Utilities.h"
#include "Memory_Pools.h"

#include "Dispatching_Modules.h"

#if !defined (__ACE_INLINE__)
#include "Dispatching_Modules.i"
#endif /* __ACE_INLINE__ */

#include "tao/Timeprobe.h"

ACE_RCSID (Event,
           Dispatching_Modules,
           "$Id$")

#if defined (ACE_ENABLE_TIMEPROBES)

static const char *TAO_Dispatching_Modules_Timeprobe_Description[] =
{
  "dispatch (dequeue) the event",
  "push_source_type: Correlation Module",
  "Priority_Dispatching::push - priority requested",
  "Priority_Dispatching::push - priority obtained"
};

enum
{
  // Timeprobe description table start key
  TAO_DISPATCHING_MODULES_DISPATCH_THE_EVENT = 5000,
  TAO_DISPATCHING_MODULES_PUSH_SOURCE_TYPE_CORRELATION_MODULE,
  TAO_DISPATCHING_MODULES_PRIORITY_DISPATCHING_PUSH_PRIORITY_REQUESTED,
  TAO_DISPATCHING_MODULES_PRIORITY_DISPATCHING_PUSH_PRIORITY_OBTAINED
};

// Setup Timeprobes
ACE_TIMEPROBE_EVENT_DESCRIPTIONS (TAO_Dispatching_Modules_Timeprobe_Description,
                                  TAO_DISPATCHING_MODULES_DISPATCH_THE_EVENT);

#endif /* ACE_ENABLE_TIMEPROBES */

// ************************************************************

ACE_ES_Dispatch_Request::
ACE_ES_Dispatch_Request (ACE_Push_Consumer_Proxy *consumer,
                         const TAO_EC_Event &event,
                         RtecScheduler::handle_t rt_info)
  : priority_ (0),
    rt_info_ (rt_info),
    dispatching_module_ (0),
    use_single_event_ (0),
    consumer_ (consumer),
    event_set_ (1)
{
  this->event_set_.set (event, 0);
}

void
ACE_ES_Dispatch_Request::make_copy (RtecEventComm::EventSet &dest) const
{
  if (use_single_event_)
    {
      // The RtecEventComm::EventSet will hold a pointer to the
      // buffer, without owning it, thus it is not removed!
      // @@ TODO Check what happens in the collocated case.
      dest.replace (1, 1,
                    ACE_const_cast(RtecEventComm::Event*,
                                   &this->single_event_.event ()),
                    0);
    }
  else if (this->event_set_.size () == 1)
    {
      dest.replace (1, 1,
                    ACE_const_cast(RtecEventComm::Event*,
                                   &this->event_set_[0].event ()),
                    0);
    }
  else
    {
      dest.length (this->event_set_.size ());

      int c = 0;
      for (CORBA::ULong i = 0; i < this->event_set_.size (); ++i)
        {
          if (this->event_set_[i].empty ())
            continue;
          dest[c] = this->event_set_[i].event ();
          c++;
        }
      dest.length (c);
    }
}

void
ACE_ES_Dispatch_Request::append_event (const TAO_EC_Event& event)
{
  size_t size = this->event_set_.size ();
  if (this->event_set_.size (size + 1) == 0)
    this->event_set_.set (event, size);
}

int
ACE_ES_Dispatch_Request::execute (u_long &command_action)
{
  ACE_TIMEPROBE (TAO_DISPATCHING_MODULES_DISPATCH_THE_EVENT);

  return dispatching_module_->dispatch_event (this, command_action);
}

#if 0
// @@ Memory pools
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

void
ACE_ES_Dispatching_Base::activate (int)
{
}

void
ACE_ES_Dispatching_Base::shutdown (void)
{
  ACE_DEBUG ((LM_DEBUG,
              "EC (%t) ACE_ES_Dispatching_Base module shutting down.\n"));
}

// Just forward the request.  This is basically a hook for the RTU
// stuff.
int
ACE_ES_Dispatching_Base::dispatch_event (ACE_ES_Dispatch_Request *request,
                                         u_long &command_action)
{
  ACE_DECLARE_NEW_CORBA_ENV;
  ACE_TRY
    {
      // Forward the request.
      up_->push (request ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;
    }
  ACE_CATCHANY
    {
      // No exceptions should be raised (push is a oneway) but we try
      // to print something useful anyway.
      ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION,
                           "ACE_ES_Dispatching_Base::dispatch_event");
    }
  ACE_ENDTRY;

  // Tell our caller to release the request.
  command_action = ACE_RT_Task_Command::RELEASE;

  // Return zero so our calling thread does not exit.
  return 0;
}

// ************************************************************

ACE_ES_Priority_Dispatching::ACE_ES_Priority_Dispatching (ACE_EventChannel *channel)
  : ACE_ES_Dispatching_Base (channel),
    notification_strategy_ (this, channel->timer_module ()),
    highest_priority_ (0),
    shutdown_ (0),
    threads_per_queue_ (0)
{
  // If we're single threaded, then we need to use the notification strategy.
  if ((threads_per_queue_ == 0) &&
      (notification_strategy_.open () == -1))
    ACE_ERROR ((LM_ERROR, "%p.\n", "ACE_ES_Priority_Dispatching"));

  // Initialize the queues.
  for (int x = 0; x < ACE_Scheduler_MAX_PRIORITIES; x++)
    {
      this->queues_[x] = 0;
    }

  this->scheduler_ =
    this->channel_->scheduler ();
}

ACE_ES_Priority_Dispatching::~ACE_ES_Priority_Dispatching (void)
{
}


void
ACE_ES_Priority_Dispatching::initialize_queues (void)
{
  for (int x = 0; x < ACE_Scheduler_MAX_PRIORITIES; x++)
    {
      if (this->queues_[x] != 0)
        continue;

      // Convert ACE_Scheduler_Rate (it's really a period, not a rate!)
      // to a form we can easily work with.
      ACE_Time_Value period_tv;
      ORBSVCS_Time::TimeT_to_Time_Value (period_tv, ACE_Scheduler_Rates[x]);

      RtecScheduler::Period_t period = period_tv.sec () * 10000000 +
                                       period_tv.usec () * 10;

      ACE_NEW (this->queues_[x],
               ACE_ES_Dispatch_Queue (this,
                                      &this->notification_strategy_,
                                      this->scheduler_.in ()));
      this->queues_[x]->thr_mgr (&this->thr_mgr_);

      if ( this->queues_[x]->open_queue (period,
                                         threads_per_queue_) == -1)
        {
          ACE_ERROR ((LM_ERROR, "%p.\n",
                      "ACE_ES_Priority_Dispatching::initialize_queues"));
          return;
        }

      this->queue_count_[x] = 1;
    }
  highest_priority_ = ACE_Scheduler_MAX_PRIORITIES - 1;
}

void
ACE_ES_Priority_Dispatching::connected (ACE_Push_Consumer_Proxy *consumer
                                        ACE_ENV_ARG_DECL)
{
  down_->connected (consumer ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;

  // This code does dynamic allocation of channel dispatch threads.
  // It requires that consumer's priorities are known at connection
  // time and that threads can request priorities from the scheduler
  // at run-time.  These are both antithetical to static scheduling.
  // The constructor now allocates a thread per rate group.
#if 0
  // We have to tell the lower portions of the channel about the
  // consumer first.  This is so that any changes to the consumer's
  // qos will take effect when we get the dispatch priority.
  down_->connected (consumer ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;

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
          TAO_THROW (CORBA::NO_MEMORY (0, ,
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
          TAO_THROW (DISPATCH_ERROR (0, ,
                                     "ACE_ES_Priority_Dispatching::connected:"
                                     "queue open failed.\n"));

        // When this goes down to 0, we will shutdown the queue.
        queue_count_[priority] = 1;

        // Keep track of this to optimize handle_signal.
        if (priority > highest_priority_)
          highest_priority_ = priority;

        ACE_DEBUG ((LM_DEBUG,
                    "EC (%t) Created queue priority = %d.\n", priority));
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
        ACE_DEBUG ((LM_DEBUG, "EC (%t) unused dispatch queue priority = %d, "
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
ACE_ES_Priority_Dispatching::push (ACE_ES_Dispatch_Request *request
                                   ACE_ENV_ARG_DECL)
{
  ACE_TIMEPROBE (TAO_DISPATCHING_MODULES_PUSH_SOURCE_TYPE_CORRELATION_MODULE);

  RtecScheduler::OS_Priority thread_priority;
  RtecScheduler::Preemption_Subpriority_t subpriority;
  RtecScheduler::Preemption_Priority_t preemption_priority;

  if (request->rt_info () != 0)
    {
      ACE_TIMEPROBE (TAO_DISPATCHING_MODULES_PRIORITY_DISPATCHING_PUSH_PRIORITY_REQUESTED);
#if 1
      this->scheduler_->priority
        (request->rt_info (),
         thread_priority,
         subpriority,
         preemption_priority
          ACE_ENV_ARG_PARAMETER);
#else
      ACE_Scheduler_Factory::server ()->priority
        (request->rt_info (),
         thread_priority,
         subpriority,
         preemption_priority
          ACE_ENV_ARG_PARAMETER);
#endif
      ACE_TIMEPROBE (TAO_DISPATCHING_MODULES_PRIORITY_DISPATCHING_PUSH_PRIORITY_OBTAINED);
      ACE_CHECK;
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
      ACE_ERROR ((LM_ERROR, "EC (%t): Push to closed queue %d,"
                  " dropping event.\n", preemption_priority));
      return;
#if 0
      ACE_THROW (SYNC_ERROR (0, , "ACE_ES_Priority_Dispatching::push"));
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
          ACE_THROW (CORBA::NO_MEMORY ());
        }
      else
        {
          ACE_DEBUG ((LM_DEBUG,
                      "EC (%t) Request rejected from closed queue %d.\n",
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

void
ACE_ES_Priority_Dispatching::activate (int threads_per_queue)
{
  this->threads_per_queue_ = threads_per_queue;
  this->initialize_queues ();
}

// Shutdown each queue.  When each queue exits, they will call back
// this->dispatch_queue_closed which allows us to free up resources.
// When the last queue has closed, we'll delete ourselves.
void
ACE_ES_Priority_Dispatching::shutdown (void)
{
  if (shutdown_)
    return;

  ACE_DEBUG ((LM_DEBUG, "EC (%t) ACE_ES_Priority_Dispatching "
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
        ACE_DEBUG ((LM_DEBUG,
                    "EC (%t) shutting down dispatch queue %d.\n", x));
        queues_[x]->shutdown_task ();
      }

  if (this->thr_mgr_.wait () == -1)
    ACE_ERROR ((LM_ERROR, "%p\n",
                "Priority_Dispatching::shutdown - waiting"));

  for (int i = 0; i <= this->highest_priority_; ++i)
    {
      if (this->queues_[i] != 0)
        {
          delete this->queues_[i];
          this->queues_[i] = 0;
        }
    }
}

// This gets called every time a Dispatch Queue closes down.  We
// search for <queue> and delete it.  If we have been told to delete
// ourself, after the last queue has been deleted, we delete
// ourselves.
void
ACE_ES_Priority_Dispatching::dispatch_queue_closed (ACE_ES_Dispatch_Queue *queue)
{
  ACE_UNUSED_ARG (queue);
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

ACE_ES_Dispatch_Queue::
   ACE_ES_Dispatch_Queue (ACE_ES_Dispatching_Base *dispatching_module,
                          ACE_ES_Notification_Strategy *notification_strategy,
                          RtecScheduler::Scheduler_ptr scheduler)
  : ACE_RT_Task (scheduler),
    dispatching_module_ (dispatching_module),
    notification_strategy_ (notification_strategy)
{
}

int
ACE_ES_Dispatch_Queue::open_queue (RtecScheduler::Period_t &period,
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
      // quick hack to test dynamic queue performance (to be replaced soon)
          ACE_ES_QUEUE *mq = 0;
#if defined (TAO_USES_STRATEGY_SCHEDULER)
#if defined (TAO_USES_EDF_SCHEDULING)

          ACE_Deadline_Message_Strategy *adms = new ACE_Deadline_Message_Strategy;

          if (adms)
          {
            mq = new ACE_Dynamic_Message_Queue<ACE_SYNCH> (*adms);
          }

#elif defined (TAO_USES_MLF_SCHEDULING) || defined (TAO_USES_MUF_SCHEDULING)

          ACE_Laxity_Message_Strategy *alms = new ACE_Laxity_Message_Strategy;

          if (alms)
          {
            mq = new ACE_Dynamic_Message_Queue<ACE_SYNCH> (*alms);
          }

#else
          mq = new ACE_ES_QUEUE;
#endif
#else
        // Allocate a message queue that does not notify.
        mq = new ACE_ES_MQ;
#endif

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

  // Create a name for ourselves using the period.  The period is
  // in 100 ns units; first convert to usec by dividing by 10.
  char temp[64];
  ACE_OS::sprintf (temp, "ACE_ES_Dispatch_Queue-%u.us", period / 10);

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
      {
        ACE_DECLARE_NEW_CORBA_ENV;
        ACE_TRY
          {// @@ TODO: Handle exceptions...
#if 1
            this->scheduler_->set
              (rt_info_,
               RtecScheduler::VERY_HIGH_CRITICALITY,
               ORBSVCS_Time::zero (),
               ORBSVCS_Time::zero (),
               ORBSVCS_Time::zero (),
               period,
               RtecScheduler::VERY_LOW_IMPORTANCE,
               ORBSVCS_Time::zero (),
               1,
               RtecScheduler::OPERATION
                ACE_ENV_ARG_PARAMETER);
#else
            ACE_Scheduler_Factory::server()->set (rt_info_,
                                                  RtecScheduler::VERY_HIGH_CRITICALITY,
                                                  ORBSVCS_Time::zero (),
                                                  ORBSVCS_Time::zero (),
                                                  ORBSVCS_Time::zero (),
                                                  period,
                                                  RtecScheduler::VERY_LOW_IMPORTANCE,
                                                  ORBSVCS_Time::zero (),
                                                  1,
                                                  RtecScheduler::OPERATION
                                                   ACE_ENV_ARG_PARAMETER);
#endif
            ACE_TRY_CHECK;
          }
        ACE_CATCHANY
          {
            ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION,
                                 "ACE_ES_Dispatch_Queue::exception");
            return -1;
          }
        ACE_ENDTRY;
      }
      // FALLTHROUGH
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
ACE_ES_EFD_Dispatching::push (ACE_ES_Dispatch_Request *request
                              ACE_ENV_ARG_DECL_NOT_USED)
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
  : ACE_ES_Priority_Dispatching (channel)
{
}

// We're called from a dispatch queue, so we can not release the request.
int
ACE_ES_RTU_Dispatching::dispatch_event (ACE_ES_Dispatch_Request *request,
                                        u_long &command_action)
{
  // Store the priority of the task currently running.
  channel_->rtu_manager ()->priority (request->priority ());

  ACE_DECLARE_NEW_CORBA_ENV;
  ACE_TRY
    {
      // Forward the request.
      up_->push (request ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;
    }
  ACE_CATCHANY
    {
      // No exceptions should be raised (push is a oneway) but we try
      // to print something useful anyway.
      ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION,
                           "ACE_ES_Dispatching_Base::dispatch_event");
    }
  ACE_ENDTRY;

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
ACE_ES_RTU_Dispatching::push (ACE_ES_Dispatch_Request *request
                              ACE_ENV_ARG_DECL)
{
  // First enqueue the message in the proper queue.
  ACE_ES_Priority_Dispatching::push (request ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;

  // If the current event is higher priority (lower value) than the
  // current running task, then tell the task to preempt itself.
  int request_priority = request->priority ();
  int running_priority = channel_->rtu_manager ()->priority ();
  if (request_priority < running_priority)
    channel_->rtu_manager ()->should_preempt (1);
  return;
}

#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)

#elif defined(ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)

#endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */
