/* -*- C++ -*- */
//=============================================================================
/**
 *  @file Dispatching_Modules.h
 *
 *  $Id$
 *
 *  @author Tim Harrison (harrison@cs.wustl.edu)
 *
 *  This file holds the different Event Service dispatching
 *  mechanisms.  These include null-dispatching (EFD),
 *  single-threaded with (RTU) and without preemption (LAME), and a
 *  multithreaded implementation.
 *
 *
 */
//=============================================================================


#ifndef ACE_DISPATCHING_MODULES_H
#define ACE_DISPATCHING_MODULES_H
#include "ace/pre.h"

#include "ace/Reactor_Notification_Strategy.h"
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

/**
 * @class ACE_ES_Dispatching_Base
 *
 * @brief Event Service Dispatch Module base class
 *
 * We inherit from ACE_Event_Handler so that we can be called back
 * by the ReactorEx when requests are queued.  The virtual
 * dispatch_event method allows ACE_ES_Dispatch_Requests to call
 * back the dispatching module when acting as command objects. When
 * this implementation is used by the Event Channel it forwards all
 * dispatch calls without any queuing.  Therefore, it can be
 * used to build an EFD.  It is also inherited by the Priority
 * Dispatching module.
 */
class TAO_RTOLDEvent_Export ACE_ES_Dispatching_Base : public ACE_Event_Handler
{
public:
  /// Default construction.
  ACE_ES_Dispatching_Base (ACE_EventChannel *channel);

  /// Link to adjacent modules.
  virtual void open (ACE_ES_Consumer_Module *up,
                     ACE_ES_Correlation_Module *down);

  /// Forward down_.
  virtual void connected (ACE_Push_Consumer_Proxy *consumer
                          ACE_ENV_ARG_DECL_NOT_USED);

  /// Forward down_.
  virtual void disconnecting (ACE_Push_Consumer_Proxy *consumer
                              ACE_ENV_ARG_DECL_NOT_USED);

  /// Release any unneeded dispatching resources.
  virtual void disconnected (ACE_Push_Consumer_Proxy *consumer);

  // = Not needed.
  // void connected (ACE_Push_Supplier_Proxy *supplier);
  // void disconnecting (ACE_Push_Supplier_Proxy *supplier);

  /// Forward up_.
  virtual void push (ACE_ES_Dispatch_Request *request
                     ACE_ENV_ARG_DECL_NOT_USED) = 0;

  /// Called by ACE_ES_Dispatch_Requests when dequeued by RT_Tasks.
  virtual int dispatch_event (ACE_ES_Dispatch_Request *request,
                              u_long &command_action);

  /// Called when all the threads of a <q> have exited.
  virtual void dispatch_queue_closed (ACE_ES_Dispatch_Queue *q);

  /// This is called by the Event Channel. It will create all the
  /// threads and only return once they are all up and running.
  virtual void activate (int threads_per_queue);

  /// This is called by the Event Channel.  This will attempt to shut
  /// down all of its threads gracefully.  Wish it luck.
  virtual void shutdown (void);

protected:
  /// Dat der channel.
  ACE_EventChannel *channel_;

  /// To synchronize thr_count_.
  ACE_ES_MUTEX lock_;

  /// The total number of threads in the Dispatching Module.  This will
  /// be the sum of all the Dispatch Queue threads.
  int thr_count_;

  /// Next module up.
  ACE_ES_Consumer_Module *up_;

  /// Next module down.
  ACE_ES_Correlation_Module *down_;
};

// ************************************************************

/**
 * @class ACE_ES_Dispatch_Request
 *
 * @brief ACE Event Service Dispatch Request
 *
 * Encapsulates a consumer and the events that will be sent to the
 * consumer.  Right now, this class keeps a single_event_ that can
 * be used when only one event is sent to the consumer.  Since this
 * is frequently the case (except for correlations), this
 * optimization reduces the amount of dynamic memory allocation is
 * necessary.  This class is also a GOF Command object since it can
 * be dequeued by an RT_Task to call back the dispatching module
 * for request dispatching.
 */
class TAO_RTOLDEvent_Export ACE_ES_Dispatch_Request : public ACE_RT_Task_Command
{
public:
  /// Default construction.
  ACE_ES_Dispatch_Request (void);

  /// Default destruction.
  virtual ~ACE_ES_Dispatch_Request (void);

  /// All the events must be added after construction to the
  /// event_set.
  ACE_ES_Dispatch_Request (ACE_Push_Consumer_Proxy *consumer,
                           RtecScheduler::handle_t rt_info);

  /// Set consumer_ to <consumer> and copy <event_set> to event_set_.
  /// <rt_info> describes the method receiving this dispatch.
  ACE_ES_Dispatch_Request (ACE_Push_Consumer_Proxy *consumer,
                           const TAO_EC_Event_Array &event_set,
                           RtecScheduler::handle_t rt_info);

  /**
   * Set consumer_ to <consumer> and sets
   * single_event_.header.creation_time to <time>.  Sets
   * use_single_event_ to 1.  <rt_info> describes the method
   * receiving this dispatch.
   */
  ACE_ES_Dispatch_Request (ACE_Push_Consumer_Proxy *consumer,
                           const RtecEventComm::Time &time,
                           RtecScheduler::handle_t rt_info);

  /**
   * Sets consumer_ and the first slot of event_set_.  We use the
   * event_set_ instead of the single_event_ so that we can just carry
   * around the pointer to <event>.  <rt_info> describes the method
   * receiving this dispatch.
   */
  ACE_ES_Dispatch_Request (ACE_Push_Consumer_Proxy *consumer,
                           const TAO_EC_Event &event,
                           RtecScheduler::handle_t rt_info);

  /// Description of the method receiving this request.
  RtecScheduler::handle_t rt_info (void);

  /**
   * For multi-threaded implementations, <dispatching_module> is
   * called back when a request is dequeued.  <priority> is the
   * dispatch priority of the event.  <sub_priority> is the enqueue
   * priority of the event and will be forwarded to
   * ACE_Message_Block.
   */
  void set (ACE_ES_Dispatching_Base *dispatching_module,
            RtecScheduler::OS_Priority priority,
            RtecScheduler::Preemption_Subpriority_t sub_priority);

  /// Consumer accessor.
  ACE_Push_Consumer_Proxy *consumer (void) const;

  /// If accessed, make_copy will use event_set_.
  const TAO_EC_Event_Array &event_set (void) const;

  /// If accessed, make_copy will use event_set_.
  TAO_EC_Event_Array &event_set (void);

  /// Append an event to the list of events in the Request.
  void append_event (const TAO_EC_Event& event);

  /// Returns 1 if we're using single_event, or event_set_.size ().
  CORBA::ULong number_of_events (void) const;

  /// Copy single_event or event_set into <dest>.
  /// @@ Change the name to something more meaningful...
  void make_copy (RtecEventComm::EventSet &dest) const;

  /// Calls dispatching_module_->dispatch_event.
  virtual int execute (u_long &command_action);

  /// Priority accessor.
  RtecScheduler::OS_Priority priority (void);

#if 0
  // @@ Memory pools
  // @@ This cannot be done: the object would be allocated using this
  // class operator new, but it will be removed using the
  // ACE_Message_Block operator delete!
  /// Allocates memory from a thread-specific memory pool.
  void *operator new (size_t nbytes);

  /// Returns memory to a thread-specific memory pool.
  void operator delete (void *);
#endif

protected:
  RtecScheduler::OS_Priority priority_;

  /// Describes the method receiving this dispatch.
  RtecScheduler::handle_t rt_info_;

  /// The dispatching module called back when we're dequeued by a
  /// thread.
  ACE_ES_Dispatching_Base *dispatching_module_;

  /// Is true if we're using a single event.  Is 0 is we're using
  /// event_set_.
  int use_single_event_;

  /// The final destination for single_event_ or event_set_.
  ACE_Push_Consumer_Proxy *consumer_;

  /// This is used for single event dispatches.
  TAO_EC_Event single_event_;

  /// This is used for event sets that need to be dispatched.
  TAO_EC_Event_Array event_set_;
};

// ************************************************************

#if defined (ACE_WIN32)
/**
 * @class ACE_ES_ReactorEx_NS
 *
 * @brief Event Service ReactorEx Notification Strategy
 *
 * Integrates the ACE_Message_Queue notification to signal a
 * handle that will wake up the ACE_ES_Priority_Dispatching
 * module.  This is used in place of the
 * ACE_ReactorEx_Notification_Strategy to avoid any queueing by
 * the ReactorEx::notify mechanism.
 */
class TAO_RTOLDEvent_Export ACE_ES_ReactorEx_NS : public ACE_Notification_Strategy
{
public:
  /// Stores away <eh> for when this->open is called.
  ACE_ES_ReactorEx_NS (ACE_Event_Handler *eh,
                       TAO_EC_Timer_Module *tm);

  /// Registers eh_ with the ReactorEx to be notified when this->notify
  /// is called.
  int open (void);

  /// Removes self from the reactor.
  void shutdown (void);

  // = These result in eh_->handle_signal getting called.  eh_ should
  // point to a dispatching module.
  virtual int notify (void);
  virtual int notify (ACE_Event_Handler *,
                      ACE_Reactor_Mask mask);

  //  ACE_HANDLE get_handle (void);
  // Returns event_.handle ().

private:
  /// Registered with the ReactorEx.
  ACE_Auto_Event event_;

  /// To schedule timers.
  TAO_EC_Timer_Module *timer_module_;
};

typedef ACE_ES_ReactorEx_NS ACE_ES_Notification_Strategy;

#else // *******************************************************

/**
 * @class ACE_ES_Reactor_NS
 *
 * @brief Event Service Reactor Notification Strategy
 *
 * Maps to the ACE_Reactor_Notification_Strategy interface.  This
 * version is for non WIN32 platforms.
 */
class TAO_RTOLDEvent_Export ACE_ES_Reactor_NS : public ACE_Reactor_Notification_Strategy
{
public:
  /// Calls ACE_Reactor_Notification_Strategy with the ORB's reactor
  /// and signal mask.
  ACE_ES_Reactor_NS (ACE_Event_Handler *eh,
                     TAO_EC_Timer_Module *tm);

  /// Does nothing.
  int open (void);

  /// Does nothing.
  void shutdown (void);
};

typedef ACE_ES_Reactor_NS ACE_ES_Notification_Strategy;

#endif /* ACE_WIN32 */

// ************************************************************

/**
 * @class ACE_ES_MQ
 *
 * @brief Event Service Message Queue
 */
class TAO_RTOLDEvent_Export ACE_ES_MQ : public ACE_ES_QUEUE
{
  /// Does nothing.
  virtual int notify (void) { return 0;}
};

// ************************************************************

/**
 * @class ACE_ES_Dispatch_Queue
 *
 * @brief Event Service Dispatch Queue
 *
 * An encapsulation of a dispatch queue.  By inheriting from
 * ACE_RT_Task, we can make this zero-threaded or multi-threaded.
 */
class TAO_RTOLDEvent_Export ACE_ES_Dispatch_Queue : public ACE_RT_Task
{
public:
  /// Stores <dispatching_module> for this->threads_closed.  Stores
  /// away <notification_strategy> for this->synch_threads.
  ACE_ES_Dispatch_Queue (ACE_ES_Dispatching_Base *dispatching_module,
                         ACE_ES_Notification_Strategy *notification_strategy,
                         RtecScheduler::Scheduler_ptr scheduler);

  /// This is a hack to get the channel to work with the new
  /// scheduler.
  int open_queue (RtecScheduler::Period_t &period,
                  int threads);

#if 0
  /**
   * Creates a name from the <priority> and tries to find a scheduling
   * qos structure.  If one is not found, but created, qos_ is set
   * with default values.  Either way, if qos_->thread_ > 0, it calls
   * this->synch_threads.  Otherwise, our message queue will use
   * notification_strategy_.  This will cause the ReactorEx to call
   * back the dispatching_module_ when requests are queued on our
   * message queue.  Returns 0 on success, -1 on failure.
   */
  int open_queue (RtecScheduler::OS_Priority priority,
                  int threads);
#endif

  /// Called when every thread has exited.  This will call
  /// dispatching_module_->dispatch_queue_closed.
  virtual void threads_closed (void);

private:
  /// Used in threads_closed.
  ACE_ES_Dispatching_Base *dispatching_module_;

  /// Notifies the Dispatching Module when a request has been queued on
  /// our message queue.
  ACE_ES_Notification_Strategy *notification_strategy_;
};

// ************************************************************

/**
 * @class ACE_ES_Priority_Dispatching
 *
 * @brief Event Service Priority Dispatching Module
 *
 * Inherits from ACE_Event_Handler to utilitize the
 * ACE_Message_Queue notification strategy.  This implementation
 * does priority dispatching without preemption.
 */
class TAO_RTOLDEvent_Export ACE_ES_Priority_Dispatching : public ACE_ES_Dispatching_Base
{
public:
  /// Store <channel>.
  ACE_ES_Priority_Dispatching (ACE_EventChannel *channel);

  /// Delete queues.
  ~ACE_ES_Priority_Dispatching (void);

  /// Allocate any needed dispatching resources for this consumers
  /// priority.
  void connected (ACE_Push_Consumer_Proxy *consumer
                  ACE_ENV_ARG_DECL_NOT_USED);

  /// Release unneeded dispatch queues.
  void disconnected (ACE_Push_Consumer_Proxy *consumer);

  // = Not needed.
  // void connected (ACE_Push_Supplier_Proxy *supplier);
  // void disconnecting (ACE_Push_Supplier_Proxy *supplier);

  /// Enqueues the request on the appropriate Dispatch Queue.
  virtual void push (ACE_ES_Dispatch_Request *request
                     ACE_ENV_ARG_DECL_NOT_USED);

  /**
   * Open all queues.
   * Spawns <threads_per_queue> thread for each dispatch queue.
   * If != 0, then the channel is an MT_CHANNEL.
   * If == 0, then the channel is an ST_CHANNEL.
   */
  virtual void activate (int threads_per_queue);

  /// Closes all queues "asynchronously."  When all queues are closed,
  /// deletes them all and then deletes itself.
  virtual void shutdown (void);

  /// Called when all the threads of a <q> have exited.  Deletes <q>.
  virtual void dispatch_queue_closed (ACE_ES_Dispatch_Queue *q);

  //  virtual ACE_HANDLE get_handle (void) const;
  // Get the I/O handle.

protected:
  /// Called when input has arrived on a message queue.  This is used
  /// for single-threaded implementations.
  virtual int handle_signal (int signum, siginfo_t * = 0, ucontext_t * = 0);

  /**
   * For single-threaded implementations on non-win32 platforms that
   * use the ACE_Reactor_Notification_Strategy.  This just forwards
   * all calls to this->handle_signal ().
   */
  virtual int handle_input (ACE_HANDLE);

  /// Shared between all dispatch queues.
  ACE_ES_Notification_Strategy notification_strategy_;

  /// This is a hack to create a queue for each of the 4 rate groups.
  void initialize_queues (void);

  /// Pointers to dispatch queues.
  ACE_ES_Dispatch_Queue *queues_[ACE_Scheduler_MAX_PRIORITIES];

  /// The number of consumers using each queue.
  int queue_count_[ACE_Scheduler_MAX_PRIORITIES];

  /// The highest priority queue in queues_.  This allows us to
  /// optimize the handle_signal method.
  int highest_priority_;

  /// Make sure to only shutdown the dispatching module once.
  int shutdown_;

  /// The number of threads to spawn for each dispatch queue.
  int threads_per_queue_;

  /// The thread manager for the threads of this object.
  ACE_RT_Thread_Manager thr_mgr_;

  /// The scheduler.
  RtecScheduler::Scheduler_var scheduler_;
};

// ************************************************************

/**
 * @class ACE_ES_EFD_Dispatching
 *
 * @brief Event Service EFD Dispatching Module
 *
 * Implements a zero-threaded dispatcher with no preemption.
 */
class TAO_RTOLDEvent_Export ACE_ES_EFD_Dispatching : public ACE_ES_Dispatching_Base
{
public:
  /// Acquires the proper qos structure and passes <channel> onto to
  /// the dispatching base constructor.
  ACE_ES_EFD_Dispatching (ACE_EventChannel *channel);

  /// Forward up_.
  virtual void push (ACE_ES_Dispatch_Request *request
                     ACE_ENV_ARG_DECL_NOT_USED);
};

// ************************************************************

/**
 * @class ACE_ES_RTU_Dispatching
 *
 * @brief Event Service RTU Dispatching Module
 *
 * Implements a single-threaded dispatcher with delayed preemption.
 */
class TAO_RTOLDEvent_Export ACE_ES_RTU_Dispatching : public ACE_ES_Priority_Dispatching
{
public:
  /// Store <channel>.
  ACE_ES_RTU_Dispatching (ACE_EventChannel *channel);

  /**
   * Called by ACE_Dispatch_Queues and handle_signal when an event
   * needs to be dequeued.  Implements an RTU-like delayed preemption
   * policy.
   */
  virtual int dispatch_event (ACE_ES_Dispatch_Request *request,
                              u_long &command_action);

  /// Calls ACE_ES_Priority_Dispatching::push and then checks if
  /// preemption is necessary.
  virtual void push (ACE_ES_Dispatch_Request *request
                     ACE_ENV_ARG_DECL_NOT_USED);
};

#if defined (__ACE_INLINE__)
#include "Dispatching_Modules.i"
#endif /* __ACE_INLINE__ */

#include "ace/post.h"
#endif /* ACE_DISPATCHING_MODULES_H */
