/* -*- C++ -*- */
// $Id$
//
// ============================================================================
//
// = LIBRARY
//    orbsvcs
//
// = FILENAME
//    Dispatching_Module
//
// = AUTHOR
//    Tim Harrison (harrison@cs.wustl.edu)
//
// = DESCRIPTION
//    This file holds the different Event Service dispatching
//    mechanisms.  These include null-dispatching (EFD),
//    single-threaded with (RTU) and without preemption (LAME), and a
//    multithreaded implementation.
//
// ============================================================================

#ifndef ACE_DISPATCHING_MODULES_H
#define ACE_DISPATCHING_MODULES_H

#include "tao/Timeprobe.h"
#include "orbsvcs/Event/ReactorTask.h"
#include "orbsvcs/Event/Event_Manip.h"
#include "orbsvcs/Event/Event_Channel.h"

// ************************************************************

// Forward declarations.
class ACE_ES_Dispatch_Queue;
class ACE_ES_Dispatch_Request;

// ************************************************************

// Forward declarations.
class ACE_ES_Consumer_Module;

class TAO_ORBSVCS_Export ACE_ES_Dispatching_Base : public ACE_Event_Handler
// = TITLE
//    Event Service Dispatch Module base class
//
// = DESCRIPTION
//    We inherit from ACE_Event_Handler so that we can be called back
//    by the ReactorEx when requests are queued.  The virtual
//    dispatch_event method allows ACE_ES_Dispatch_Requests to call
//    back the dispatching module when acting as command objects. When
//    this implementation is used by the Event Channel it forwards all
//    dispatch calls without any queuing.  Therefore, it can be
//    used to build an EFD.  It is also inherited by the Priority
//    Dispatching module.
{
public:
  ACE_ES_Dispatching_Base (ACE_EventChannel *channel);
  // Default construction.

  virtual void open (ACE_ES_Consumer_Module *up,
                     ACE_ES_Correlation_Module *down);
  // Link to adjacent modules.

  virtual void connected (ACE_Push_Consumer_Proxy *consumer,
                          CORBA::Environment &);
  // Forward down_.

  virtual void disconnecting (ACE_Push_Consumer_Proxy *consumer,
                              CORBA::Environment &);
  // Forward down_.

  virtual void disconnected (ACE_Push_Consumer_Proxy *consumer);
  // Release any unneeded dispatching resources.

  // = Not needed.
  // void connected (ACE_Push_Supplier_Proxy *supplier);
  // void disconnecting (ACE_Push_Supplier_Proxy *supplier);

  virtual void push (ACE_ES_Dispatch_Request *request,
                     CORBA::Environment &) = 0;
  // Forward up_.

  virtual int dispatch_event (ACE_ES_Dispatch_Request *request,
                              u_long &command_action);
  // Called by ACE_ES_Dispatch_Requests when dequeued by RT_Tasks.

  virtual void dispatch_queue_closed (ACE_ES_Dispatch_Queue *q);
  // Called when all the threads of a <q> have exited.

  virtual void activate (int threads_per_queue);
  // This is called by the Event Channel. It will create all the
  // threads and only return once they are all up and running.

  virtual void shutdown (void);
  // This is called by the Event Channel.  This will attempt to shut
  // down all of its threads gracefully.  Wish it luck.

protected:
  ACE_EventChannel *channel_;
  // Dat der channel.

  ACE_ES_MUTEX lock_;
  // To synchronize thr_count_.

  int thr_count_;
  // The total number of threads in the Dispatching Module.  This will
  // be the sum of all the Dispatch Queue threads.

  ACE_ES_Consumer_Module *up_;
  // Next module up.

  ACE_ES_Correlation_Module *down_;
  // Next module down.
};

// ************************************************************

class TAO_ORBSVCS_Export ACE_ES_Dispatch_Request : public ACE_RT_Task_Command
// = TITLE
//    ACE Event Service Dispatch Request
//
// = DESCRIPTION
//    Encapsulates a consumer and the events that will be sent to the
//    consumer.  Right now, this class keeps a single_event_ that can
//    be used when only one event is sent to the consumer.  Since this
//    is frequently the case (except for correlations), this
//    optimization reduces the amount of dynamic memory allocation is
//    necessary.  This class is also a GOF Command object since it can
//    be dequeued by an RT_Task to call back the dispatching module
//    for request dispatching.
{
public:
  ACE_ES_Dispatch_Request (void);
  // Default construction.

  virtual ~ACE_ES_Dispatch_Request (void);
  // Default destruction.

  ACE_ES_Dispatch_Request (ACE_Push_Consumer_Proxy *consumer,
                           RtecScheduler::handle_t rt_info);
  // All the events must be added after construction to the
  // event_set.

  ACE_ES_Dispatch_Request (ACE_Push_Consumer_Proxy *consumer,
                           const TAO_EC_Event_Array &event_set,
                           RtecScheduler::handle_t rt_info);
  // Set consumer_ to <consumer> and copy <event_set> to event_set_.
  // <rt_info> describes the method receiving this dispatch.

  ACE_ES_Dispatch_Request (ACE_Push_Consumer_Proxy *consumer,
                           const RtecEventComm::Time &time,
                           RtecScheduler::handle_t rt_info);
  // Set consumer_ to <consumer> and sets
  // single_event_.header.creation_time to <time>.  Sets
  // use_single_event_ to 1.  <rt_info> describes the method
  // receiving this dispatch.

  ACE_ES_Dispatch_Request (ACE_Push_Consumer_Proxy *consumer,
                           const TAO_EC_Event &event,
                           RtecScheduler::handle_t rt_info);
  // Sets consumer_ and the first slot of event_set_.  We use the
  // event_set_ instead of the single_event_ so that we can just carry
  // around the pointer to <event>.  <rt_info> describes the method
  // receiving this dispatch.

  RtecScheduler::handle_t rt_info (void);
  // Description of the method receiving this request.

  void set (ACE_ES_Dispatching_Base *dispatching_module,
            RtecScheduler::OS_Priority priority,
            RtecScheduler::Preemption_Subpriority_t sub_priority);
  // For multi-threaded implementations, <dispatching_module> is
  // called back when a request is dequeued.  <priority> is the
  // dispatch priority of the event.  <sub_priority> is the enqueue
  // priority of the event and will be forwarded to
  // ACE_Message_Block.

  ACE_Push_Consumer_Proxy *consumer (void) const;
  // Consumer accessor.

  const TAO_EC_Event_Array &event_set (void) const;
  // If accessed, make_copy will use event_set_.

  TAO_EC_Event_Array &event_set (void);
  // If accessed, make_copy will use event_set_.

  void append_event (const TAO_EC_Event& event);
  // Append an event to the list of events in the Request.

  CORBA::ULong number_of_events (void) const;
  // Returns 1 if we're using single_event, or event_set_.size ().

  void make_copy (RtecEventComm::EventSet &dest) const;
  // Copy single_event or event_set into <dest>.
  // @@ Change the name to something more meaningful...

  virtual int execute (u_long &command_action);
  // Calls dispatching_module_->dispatch_event.

  RtecScheduler::OS_Priority priority (void);
  // Priority accessor.

#if 0
  // @@ Memory pools
  // @@ This cannot be done: the object would be allocated using this
  // class operator new, but it will be removed using the
  // ACE_Message_Block operator delete!
  void *operator new (size_t nbytes);
  // Allocates memory from a thread-specific memory pool.

  void operator delete (void *);
  // Returns memory to a thread-specific memory pool.
#endif

protected:
  RtecScheduler::OS_Priority priority_;

  RtecScheduler::handle_t rt_info_;
  // Describes the method receiving this dispatch.

  ACE_ES_Dispatching_Base *dispatching_module_;
  // The dispatching module called back when we're dequeued by a
  // thread.

  int use_single_event_;
  // Is true if we're using a single event.  Is 0 is we're using
  // event_set_.

  ACE_Push_Consumer_Proxy *consumer_;
  // The final destination for single_event_ or event_set_.

  TAO_EC_Event single_event_;
  // This is used for single event dispatches.

  TAO_EC_Event_Array event_set_;
  // This is used for event sets that need to be dispatched.
};

// ************************************************************

#if defined (ACE_WIN32)
class TAO_ORBSVCS_Export ACE_ES_ReactorEx_NS : public ACE_Notification_Strategy
// = TITLE
//     Event Service ReactorEx Notification Strategy
//
// = DESCRIPTION
//     Integrates the ACE_Message_Queue notification to signal a
//     handle that will wake up the ACE_ES_Priority_Dispatching
//     module.  This is used in place of the
//     ACE_ReactorEx_Notification_Strategy to avoid any queueing by
//     the ReactorEx::notify mechanism.
{
public:
  ACE_ES_ReactorEx_NS (ACE_Event_Handler *eh,
                       TAO_EC_Timer_Module *tm);
  // Stores away <eh> for when this->open is called.

  int open (void);
  // Registers eh_ with the ReactorEx to be notified when this->notify
  // is called.

  void shutdown (void);
  // Removes self from the reactor.

  // = These result in eh_->handle_signal getting called.  eh_ should
  // point to a dispatching module.
  virtual int notify (void);
  virtual int notify (ACE_Event_Handler *,
                      ACE_Reactor_Mask mask);

  //  ACE_HANDLE get_handle (void);
  // Returns event_.handle ().

private:
  ACE_Auto_Event event_;
  // Registered with the ReactorEx.

  TAO_EC_Timer_Module *timer_module_;
  // To schedule timers.
};

typedef ACE_ES_ReactorEx_NS ACE_ES_Notification_Strategy;

#else // *******************************************************

class TAO_ORBSVCS_Export ACE_ES_Reactor_NS : public ACE_Reactor_Notification_Strategy
// = TITLE
//     Event Service Reactor Notification Strategy
//
// = DESCRIPTION
//     Maps to the ACE_Reactor_Notification_Strategy interface.  This
//     version is for non WIN32 platforms.
{
public:
  ACE_ES_Reactor_NS (ACE_Event_Handler *eh,
                     TAO_EC_Timer_Module *tm);
  // Calls ACE_Reactor_Notification_Strategy with the ORB's reactor
  // and signal mask.

  int open (void);
  // Does nothing.

  void shutdown (void);
  // Does nothing.
};

typedef ACE_ES_Reactor_NS ACE_ES_Notification_Strategy;

#endif /* ACE_WIN32 */

// ************************************************************

class TAO_ORBSVCS_Export ACE_ES_MQ : public ACE_ES_QUEUE
// = TITLE
//    Event Service Message Queue
{
  virtual int notify (void) { return 0;}
  // Does nothing.
};

// ************************************************************

class TAO_ORBSVCS_Export ACE_ES_Dispatch_Queue : public ACE_RT_Task
// = TITLE
//    Event Service Dispatch Queue
//
// = DESCRIPTION
//    An encapsulation of a dispatch queue.  By inheriting from
//    ACE_RT_Task, we can make this zero-threaded or multi-threaded.
{
public:
  ACE_ES_Dispatch_Queue (ACE_ES_Dispatching_Base *dispatching_module,
                         ACE_ES_Notification_Strategy *notification_strategy,
                         RtecScheduler::Scheduler_ptr scheduler);
  // Stores <dispatching_module> for this->threads_closed.  Stores
  // away <notification_strategy> for this->synch_threads.

  int open_queue (RtecScheduler::Period_t &period,
                  int threads);
  // This is a hack to get the channel to work with the new
  // scheduler.

#if 0
  int open_queue (RtecScheduler::OS_Priority priority,
                  int threads);
  // Creates a name from the <priority> and tries to find a scheduling
  // qos structure.  If one is not found, but created, qos_ is set
  // with default values.  Either way, if qos_->thread_ > 0, it calls
  // this->synch_threads.  Otherwise, our message queue will use
  // notification_strategy_.  This will cause the ReactorEx to call
  // back the dispatching_module_ when requests are queued on our
  // message queue.  Returns 0 on success, -1 on failure.
#endif

  virtual void threads_closed (void);
  // Called when every thread has exited.  This will call
  // dispatching_module_->dispatch_queue_closed.

private:
  ACE_ES_Dispatching_Base *dispatching_module_;
  // Used in threads_closed.

  ACE_ES_Notification_Strategy *notification_strategy_;
  // Notifies the Dispatching Module when a request has been queued on
  // our message queue.
};

// ************************************************************

class TAO_ORBSVCS_Export ACE_ES_Priority_Dispatching : public ACE_ES_Dispatching_Base
// = TITLE
//    Event Service Priority Dispatching Module
//
// = DESCRIPTION
//    Inherits from ACE_Event_Handler to utilitize the
//    ACE_Message_Queue notification strategy.  This implementation
//    does priority dispatching without preemption.
{
public:
  ACE_ES_Priority_Dispatching (ACE_EventChannel *channel);
  // Store <channel>.

  ~ACE_ES_Priority_Dispatching (void);
  // Delete queues.

  void connected (ACE_Push_Consumer_Proxy *consumer,
                  CORBA::Environment &);
  // Allocate any needed dispatching resources for this consumers
  // priority.

  void disconnected (ACE_Push_Consumer_Proxy *consumer);
  // Release unneeded dispatch queues.

  // = Not needed.
  // void connected (ACE_Push_Supplier_Proxy *supplier);
  // void disconnecting (ACE_Push_Supplier_Proxy *supplier);

  virtual void push (ACE_ES_Dispatch_Request *request,
                     CORBA::Environment &);
  // Enqueues the request on the appropriate Dispatch Queue.

  virtual void activate (int threads_per_queue);
  // Open all queues.
  // Spawns <threads_per_queue> thread for each dispatch queue.
  // If != 0, then the channel is an MT_CHANNEL.
  // If == 0, then the channel is an ST_CHANNEL.

  virtual void shutdown (void);
  // Closes all queues "asynchronously."  When all queues are closed,
  // deletes them all and then deletes itself.

  virtual void dispatch_queue_closed (ACE_ES_Dispatch_Queue *q);
  // Called when all the threads of a <q> have exited.  Deletes <q>.

  //  virtual ACE_HANDLE get_handle (void) const;
  // Get the I/O handle.

protected:
  virtual int handle_signal (int signum, siginfo_t * = 0, ucontext_t * = 0);
  // Called when input has arrived on a message queue.  This is used
  // for single-threaded implementations.

  virtual int handle_input (ACE_HANDLE);
  // For single-threaded implementations on non-win32 platforms that
  // use the ACE_Reactor_Notification_Strategy.  This just forwards
  // all calls to this->handle_signal ().

  ACE_ES_Notification_Strategy notification_strategy_;
  // Shared between all dispatch queues.

  void initialize_queues (void);
  // This is a hack to create a queue for each of the 4 rate groups.

  ACE_ES_Dispatch_Queue *queues_[ACE_Scheduler_MAX_PRIORITIES];
  // Pointers to dispatch queues.

  int queue_count_[ACE_Scheduler_MAX_PRIORITIES];
  // The number of consumers using each queue.

  int highest_priority_;
  // The highest priority queue in queues_.  This allows us to
  // optimize the handle_signal method.

  int shutdown_;
  // Make sure to only shutdown the dispatching module once.

  int threads_per_queue_;
  // The number of threads to spawn for each dispatch queue.

  ACE_RT_Thread_Manager thr_mgr_;
  // The thread manager for the threads of this object.

  RtecScheduler::Scheduler_var scheduler_;
  // The scheduler.
};

// ************************************************************

class TAO_ORBSVCS_Export ACE_ES_EFD_Dispatching : public ACE_ES_Dispatching_Base
// = TITLE
//    Event Service EFD Dispatching Module
//
// = DESCRIPTION
//    Implements a zero-threaded dispatcher with no preemption.
{
public:
  ACE_ES_EFD_Dispatching (ACE_EventChannel *channel);
  // Acquires the proper qos structure and passes <channel> onto to
  // the dispatching base constructor.

  virtual void push (ACE_ES_Dispatch_Request *request,
                     CORBA::Environment &);
  // Forward up_.
};

// ************************************************************

class TAO_ORBSVCS_Export ACE_ES_RTU_Dispatching : public ACE_ES_Priority_Dispatching
// = TITLE
//    Event Service RTU Dispatching Module
//
// = DESCRIPTION
//    Implements a single-threaded dispatcher with delayed preemption.
{
public:
  ACE_ES_RTU_Dispatching (ACE_EventChannel *channel);
  // Store <channel>.

  virtual int dispatch_event (ACE_ES_Dispatch_Request *request,
                              u_long &command_action);
  // Called by ACE_Dispatch_Queues and handle_signal when an event
  // needs to be dequeued.  Implements an RTU-like delayed preemption
  // policy.

  virtual void push (ACE_ES_Dispatch_Request *request,
                     CORBA::Environment &);
  // Calls ACE_ES_Priority_Dispatching::push and then checks if
  // preemption is necessary.
};

#if defined (__ACE_INLINE__)
#include "Dispatching_Modules.i"
#endif /* __ACE_INLINE__ */

#endif /* ACE_DISPATCHING_MODULES_H */


