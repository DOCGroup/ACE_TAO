/* -*- C++ -*- */
//=============================================================================
/**
 *  @file Event_Channel.h
 *
 *  $Id$
 *
 *  @author Tim Harrison (harrison@cs.wustl.edu)
 *
 *  TAO implementation of the Real Time Event Services. For more
 *  detailed information, see
 *  http://www.cs.wustl.edu/~schmidt/oopsla.ps.gz
 *
 * =  NAMING CONVENTIONS
 *  Some of the naming might be confusing.  For instance
 *  ACE_Push_Consumer_Proxy "is-a" ProxyPushSupplier.  To the
 *  channel, ACE_Push_Consumer_Proxy is a proxy to push consumers.
 *  To a push consumer, ACE_Push_Consumer_Proxy is a proxy to push
 *  suppliers.  I chose to name classes relative to the Event
 *  Channel.
 *
 *
 */
//=============================================================================


#ifndef ACE_EVENT_CHANNEL_H
#define ACE_EVENT_CHANNEL_H
#include "ace/pre.h"

#include "ace/Containers.h"
#include "ace/Unbounded_Set_Ex.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "ace/Map_Manager.h"
#include "ace/Functor.h"

#include "tao/Timeprobe.h"
#include "orbsvcs/Scheduler_Factory.h"
#include "orbsvcs/Event/Local_ESTypes.h"
#include "orbsvcs/Event/Timer_Module.h"
#include "orbsvcs/Event/ReactorTask.h"
#include "orbsvcs/Event/Event_Manip.h"

// ************************************************************

/**
 * An array of Events.  The Event Channel keeps several collections of
 * TAO_EC_Event objects, this is implemented using a simple Event Array.
 */
typedef ACE_Array<TAO_EC_Event> TAO_EC_Event_Array;

// ************************************************************

/**
 * Append an event to an event Array.  Simplify a common idiom when
 * manipulating arrays of events.
 */
ACE_INLINE
void operator += (TAO_EC_Event_Array &dest,
                  const TAO_EC_Event &item);

// ************************************************************

/**
 * Compare two events.  The Event Channel must compare events
 * (actually event headers) for equality.
 */
ACE_INLINE int operator == (const RtecEventComm::Event &event1,
                            const RtecEventComm::Event &event2);

// ************************************************************

/**
 * Compare two TAO_EC_Events.  This is only used in the instantiation of
 * ACE_Array<TAO_EC_Event>.
 */
ACE_INLINE
int operator != (const TAO_EC_Event &rhs, const TAO_EC_Event &lhs);

// ************************************************************

/**
 * Utility for debugging events.
 */
void TAO_RTOLDEvent_Export dump_event (const RtecEventComm::Event &event);

// ************************************************************


#if defined(_MSC_VER)
#if (_MSC_VER >= 1200)
#pragma warning(push)
#endif /* _MSC_VER >= 1200 */
#pragma warning(disable:4250)
#endif /* _MSC_VER */

/**
 * @class ACE_RTU_Manager
 *
 * @brief ACE RTU Manager
 *
 */
class TAO_RTOLDEvent_Export ACE_RTU_Manager
{
public:
  /// If <active> == 0, everything returns 0.  If <active> != 0, RTUs
  /// galore.
  ACE_RTU_Manager (int active);

  /// Returns 1 if the current task should preempt itself.  Otherwise,
  /// returns 0.  Resets should_preempt to zero.
  int should_preempt (void);

  /// Called by the dispatching module when the current task should
  /// preempt itself.
  void should_preempt (int s);

  /// If <nd> != 0, the current running task will be enqueued at the
  /// head of its dispatch tail.
  void not_done (int nd);

  /// Returns 1 if the current task needs to be dispatched again.
  /// Resets not_done_ to 0;
  int not_done (void);

  /// Get the priority of the current running task.
  RtecScheduler::OS_Priority priority (void);

  /// Set the priority of the current running task.
  void priority (RtecScheduler::OS_Priority priority);

private:
  int active_;
  int should_preempt_;
  int not_done_;
  RtecScheduler::OS_Priority priority_;
};

// ************************************************************

// Chesire cat.
// Forward declarations.
class ACE_ES_Priority_Timer;
class ACE_ES_Consumer_Module;
// This forward decl and typedef allow us to remove inheritence later
// on without changing any code.
//
class ACE_ES_Correlation_Module;
class ACE_ES_Subscription_Module;
class ACE_ES_Supplier_Module;
class ACE_ES_Dispatching_Base;
typedef ACE_ES_Dispatching_Base ACE_ES_Dispatching_Module;

// Forward declare the class used to connect several EC together.
class TAO_EC_Gateway;

// Factory class for the modules in the EC.
class TAO_Module_Factory;

/**
 * @class ACE_EventChannel
 *
 * @brief TAO's Real-time Event Channel.
 *
 * This class implements the interface defined in
 * RtecEventChannelAdmin.idl. For more details check:
 * http://www.cs.wustl.edu/~coryan/EC/JSAC98.pdf
 */
class TAO_RTOLDEvent_Export ACE_EventChannel : public POA_RtecEventChannelAdmin::EventChannel
{
public:
  enum { INITIAL_STATE = 0,
         CONSUMER = 1, SUPPLIER = 2,
         SHUTDOWN = CONSUMER | SUPPLIER };

  /**
   * Construction of the given <type>.  Check the **_CHANNEL
   * enumerations defined below.
   * By default we activate the threads on construction, but it is
   * possible to create the EC first and activate the threads later.
   * A factory for the modules can be provided, by default it uses
   * TAO_EC_Default_Module_Factory
   * If an scheduler is not provided it uses the singleton in
   * ACE_Scheduler_Factory.
   */
  ACE_EventChannel (CORBA::Boolean activate_threads = 1,
                    u_long type = ACE_DEFAULT_EVENT_CHANNEL_TYPE,
                    TAO_Module_Factory* factory = 0);
  ACE_EventChannel (RtecScheduler::Scheduler_ptr scheduler,
                    CORBA::Boolean activate_threads = 1,
                    u_long type = ACE_DEFAULT_EVENT_CHANNEL_TYPE,
                    TAO_Module_Factory* factory = 0);

  /// Calls destroy.
  virtual ~ACE_EventChannel (void);

  /// Allow transformations to RtecEventChannelAdmin::EventChannel.
  RtecEventChannelAdmin::EventChannel_ptr get_ref (ACE_ENV_SINGLE_ARG_DECL_NOT_USED);

  /// Returns a reference to the RTU manager.
  ACE_RTU_Manager *rtu_manager (void);

  // = These should be private.
  ACE_ES_Consumer_Module *consumer_module_;
  ACE_ES_Dispatching_Module *dispatching_module_;
  ACE_ES_Correlation_Module *correlation_module_;
  ACE_ES_Subscription_Module *subscription_module_;
  ACE_ES_Supplier_Module *supplier_module_;

  /// Consumer or supplier connected.
  void report_connect (u_long);

  /// Consumer or supplier disconnected.
  void report_disconnect (u_long);

  /// Activate the internal threads of the EC
  void activate (void);

  /// Do not call this.  The last module has shut down.
  void shutdown (void);

  /// Consumer or supplier connected.
  void report_connect_i (u_long);

  /// Consumer or supplier disconnected.
  void report_disconnect_i (u_long);

  /// Add gateways from the EC.
  void add_gateway (TAO_EC_Gateway* gw ACE_ENV_ARG_DECL);

  /// Remove gateways from the EC.
  void del_gateway (TAO_EC_Gateway* gw ACE_ENV_ARG_DECL);

  /// The consumer list has changed, thus the EC has to
  /// inform any gateways it has.
  void update_consumer_gwys (ACE_ENV_SINGLE_ARG_DECL);

  /// The supplier list has changed, thus the EC has to
  /// inform any gateways it has.
  void update_supplier_gwys (ACE_ENV_SINGLE_ARG_DECL);

  /// The timer module controls the strategy to dispatch timers.
  TAO_EC_Timer_Module* timer_module (void) const;

  // = The RtecEventChannelAdmin::EventChannel methods.

  /// In this implementation of the EC this returns the interface for
  /// the Consumer_Module.
  virtual RtecEventChannelAdmin::ConsumerAdmin_ptr
    for_consumers (ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
      ACE_THROW_SPEC ((CORBA::SystemException));

  /// Return an interface to the Supplier_Module.
  virtual RtecEventChannelAdmin::SupplierAdmin_ptr
    for_suppliers (ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
      ACE_THROW_SPEC ((CORBA::SystemException));

  /// Shutdown the EC, free all resources, stop all threads and then
  /// shutdown the server where the Servant is running.
  virtual void destroy (ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
    ACE_THROW_SPEC ((CORBA::SystemException));

  /// The observer manipulators
  virtual RtecEventChannelAdmin::Observer_Handle
    append_observer (RtecEventChannelAdmin::Observer_ptr observer
                     ACE_ENV_ARG_DECL)
    ACE_THROW_SPEC ((
        CORBA::SystemException,
        RtecEventChannelAdmin::EventChannel::SYNCHRONIZATION_ERROR,
        RtecEventChannelAdmin::EventChannel::CANT_APPEND_OBSERVER));
  virtual void remove_observer (RtecEventChannelAdmin::Observer_Handle
                                ACE_ENV_ARG_DECL)
    ACE_THROW_SPEC ((
        CORBA::SystemException,
        RtecEventChannelAdmin::EventChannel::SYNCHRONIZATION_ERROR,
        RtecEventChannelAdmin::EventChannel::CANT_REMOVE_OBSERVER));

  // = Timer managment
  /// Schedule a timer at the appropriate priority for <preemption_priority>.
  /// Returns the preemption priority used on success, -1 on failure.
  int schedule_timer (RtecScheduler::handle_t rt_info,
                      const ACE_Command_Base *act,
                      RtecScheduler::OS_Priority preemption_priority,
                      const RtecScheduler::Time& delta,
                      const RtecScheduler::Time& interval = ORBSVCS_Time::zero ());

  /**
   * Cancel the timer associated with the priority of
   * <preemption_priority> and <id>.  <act> is filled in with the
   * Timer_ACT used when scheduling the timer.  Returns 0 on success,
   * -1 on failure.
   */
  int cancel_timer (RtecScheduler::OS_Priority preemption_priority,
                    int id,
                    ACE_Command_Base *&act);

  /**
   * Return a reference to its SchedulerService, notice that it uses
   * the CORBA semantics for memory managment, i.e. the user gains
   * ownership of the reference returned.
   */
  RtecScheduler::Scheduler_ptr scheduler (void);

  /**
   * @struct Observer_Entry
   *
   * @brief The data kept for each observer.
   *
   * The observer and its handle are kept in a simple structure.
   * In the future this structure could contain QoS information,
   * such as:
   * + how often do we update the observer?
   * + When was the last update.
   * + Does it want to receive all changes?
   */
  struct Observer_Entry
  {

    Observer_Entry (void);
    Observer_Entry (RtecEventChannelAdmin::Observer_Handle h,
                    RtecEventChannelAdmin::Observer_ptr o);

    /// The handle
    RtecEventChannelAdmin::Observer_Handle handle;

    /// The observer
    RtecEventChannelAdmin::Observer_var observer;

  };

private:
  /// Factor out commonality in the constructor.
  void init (int activate_threads);

  /// Remove all the observers, this simplifies the shutdown process.
  void cleanup_observers (void);

  /// The RTU manager dude!
  ACE_RTU_Manager *rtu_manager_;

  /// Can be any **_CHANNEL. (well, except NO_CHANNEL).
  u_long type_;

  /// Can be INITIAL_STATE, NO_CONSUMERS, NO_SUPPLIERS, or SHUTDOWN.
  u_long state_;

  /// Used to lock shared state.
  ACE_ES_MUTEX lock_;

  /// Ensures this->destory is executed only once.
  int destroyed_;

  typedef ACE_Map_Manager<RtecEventChannelAdmin::Observer_Handle,Observer_Entry,ACE_Null_Mutex> Observer_Map;
  typedef ACE_Map_Iterator<RtecEventChannelAdmin::Observer_Handle,Observer_Entry,ACE_Null_Mutex> Observer_Map_Iterator;

  /// The handles are generated in sequential order, but are opaque to
  /// the client.
  RtecEventChannelAdmin::Observer_Handle handle_generator_;

  /// Keep the set of Gateways, i.e. connections to peer EC.
  Observer_Map observers_;

  /// The strategy to dispatch timers.
  TAO_EC_Timer_Module* timer_module_;

  /// If 1 then we created the factory, thus we have to destroy it.
  int own_factory_;

  /// This is the factory we use to create and destroy the Event
  /// Channel modules.
  TAO_Module_Factory* module_factory_;

  /// The scheduler
  RtecScheduler::Scheduler_var scheduler_;
};

// ************************************************************

/**
 * @class ACE_ES_Dependency_Iterator
 *
 * @brief ConsumerQOS Iterator
 *
 * This is used by the Event Channel to parse ConsumerDependency objects.
 */
class TAO_RTOLDEvent_Export ACE_ES_Dependency_Iterator
{
public:
  /// Construct and iterator for <rep>.
  ACE_ES_Dependency_Iterator (RtecEventChannelAdmin::DependencySet &rep);

  /// Returns 0 if the advance succeeded.  Returns -1 if there are no
  /// more dependencies in the group.
  int advance_dependency (void);

  /// Cache values for n_** methods.
  int parse (void);

  /// Returns the number of conjunction groups in the dependency set.
  int n_conjunctions (void);

  /// Returns the number of disjunction groups in the dependency set.
  int n_disjunctions (void);

  /// Returns the number of timeouts registered.
  int n_timeouts (void);

  /// Returns the number of events registered.
  int n_events (void);

  /// Accessor to the current ConsumerDependency pointed to by the
  /// iterator.
  RtecEventChannelAdmin::Dependency &operator *(void);

  /// Returns the first RT_Info in the dependencies.
  RtecScheduler::handle_t first_rt_info (void);

protected:
  /// The first rt_info in the dependencies.
  RtecScheduler::handle_t rt_info_;

  /// Reference to the dependency array.
  RtecEventChannelAdmin::DependencySet &rep_;

  /// Index into rep_.
  int index_;

  /// The type of the current correlation group.
  RtecEventComm::EventType group_type_;

  /// Number of conjunction groups.
  int n_conjunctions_;

  /// Number of disjunction groups.
  int n_disjunctions_;

  /// Number of timeouts registered.
  int n_timeouts_;

  /// Number of events registered.
  int n_events_;
};

// ************************************************************
// Forward decl.
class ACE_ES_Consumer_Rep_Timeout;

/**
 * @class ACE_ES_Disjunction_Group
 *
 * @brief Disjunction Group
 *
 * Represents a disjunction group, such as (A|B|C).
 */
class TAO_RTOLDEvent_Export ACE_ES_Disjunction_Group
{
public:
  /// Default construction.
  ACE_ES_Disjunction_Group (void);

  /// Destructor.
  virtual ~ACE_ES_Disjunction_Group (void);

  /// <cm> is needed for rescheduling deadlines.
  void set_correlation_module (ACE_ES_Correlation_Module *cm);

  /// If deadline_timer_rep_ is set, it is cancelled and rescheduled.
  void reschedule_deadline (void);

  /// Set the group's reference to the deadline timer.  Returns 0 on
  /// success, -1 on failure.
  int set_deadline_timeout (ACE_ES_Consumer_Rep_Timeout *cr);

  /// Does nothing.  This is the only virtual method in this little
  /// heirarchy with the conjunction group.
  virtual void add_events (TAO_EC_Event_Array *outbox,
                           TAO_EC_Event_Array *pending_events,
                           u_long &pending_flags);

  /// Set the ACT for this group.
  void set_act (RtecEventComm::Event &act);

protected:
  /// To be sent with this group.
  TAO_EC_Event act_;

private:
  /// The disjunction group keeps a reference to the deadline timer.
  ACE_ES_Consumer_Rep_Timeout *deadline_timer_rep_;

  /// Used for cancelling and scheduling deadline_timer_rep_.
  ACE_ES_Correlation_Module *correlation_module_;
};

// ************************************************************

/**
 * @class ACE_ES_Conjunction_Group
 *
 * @brief Conjunction Group
 *
 * Represents a conjunction group, such as (A+B+C).
 */
class TAO_RTOLDEvent_Export ACE_ES_Conjunction_Group : public ACE_ES_Disjunction_Group
{
public:
  /// Default construction.
  ACE_ES_Conjunction_Group (void);

  /// Destructor.
  virtual ~ACE_ES_Conjunction_Group (void);

  /// Set the <type_id>th bit in the forward_value_.
  int add_type (int type_id);

  /// Returns 1 if this conjunction group's dependencies have been
  /// satisfied.  Returns 0 otherwise.
  int should_forward (u_long pending_flags);

  /**
   * For each bit set in forward_value_, the corresponding events in
   * <pending_events> is added to <outbox>.  Each bit set in
   * <forward_value_> is cleared in <pending_flags>.  If <oubox> == 0,
   * then add_events just clears the pending events and flags.
   */
  virtual void add_events (TAO_EC_Event_Array *outbox,
                           TAO_EC_Event_Array *pending_events,
                           u_long &pending_flags);

private:
  u_long forward_value_;
};

// ************************************************************

// Forward decl.
class ACE_ES_Consumer_Correlation;

/**
 * @class ACE_ES_Consumer_Rep
 *
 * @brief Consumer Representation.
 *
 * These are stored in the subscription module.  They store
 * information that allows optimized correlations.  It represents
 * the consumer that will handle *one* type of event.  This
 * probably shouldn't inherit from ACE_Command_Base since it's used
 * only by ACE_ES_Consumer_Rep_Timeout.  However, this allows me to
 * minimize dynamic allocation.
 */
class TAO_RTOLDEvent_Export ACE_ES_Consumer_Rep : public ACE_Command_Base
{
public:
  /// Default construction.
  ACE_ES_Consumer_Rep (void);

  /// <dep> describes the event subscribed to and the method handling
  /// the event.   <correlation> is the parent correlation object.
  void init (ACE_ES_Consumer_Correlation *correlation,
             RtecEventChannelAdmin::Dependency &dep);

  /// Virtual destruction.
  virtual ~ACE_ES_Consumer_Rep (void);

  /// The event subscribed to and the method that will handle this
  /// event.
  RtecEventChannelAdmin::Dependency *dependency (void);

  /// Get the correlation group index of this consumer rep's event
  /// type.
  int type_id (void);

  /// Set the correlation group index of this consumer rep's event
  /// type.
  void type_id (int);

  enum Correlation_Type
  {
    NO_CORRELATION,
    CORRELATE,
    DEADLINE_TIMEOUT,
    GLOBAL_DEADLINE
  };

  /// If this returns 0, then the event associated with this consumer
  /// should be forwarded without running any correlations.
  u_long correlation_type (void);

  /// Set whether the event should be correlated.  <ct> is a
  /// Correlation_Type.
  void correlation_type (u_long ct);

  /// Add a disjunction group.
  int add_disjunction_group (ACE_ES_Disjunction_Group &);

  /// Returns the first disjunction group added via
  /// this->add_disjunction_group.
  ACE_ES_Disjunction_Group *top_group (void);

  /// Calls reschedule_deadline on all disjunction groups added through
  /// this->add_disjunction_group.
  void reschedule_deadlines (void);

  /// Returns 1 if events should be sent to this consumer.  Returns 0
  /// if they should not (suspended or disconnected).
  int receiving_events (void);

  /// Stop forwarding events to the calling consumer.
  void suspend (void);

  /// Resume forwarding events to the calling consumer.
  void resume (void);

  /// Returns the Consumer_Correlation object for the target consumer.
  ACE_ES_Consumer_Correlation *correlation (void);

  /// Schedules the consumer rep to be removed from all subscription
  /// lists.
  void disconnect (void);

  /// Returns 1 if the consumer rep should be removed from all
  /// subscription lists.
  int disconnected (void);

  /// Increments ref_count_.
  void _duplicate (void);

  /// Decrements ref_count_ and deletes this if 0.
  void _release (void);

protected:
  /// Whether the rep should be removed from all subscription lists.
  int disconnected_;

  /**
   * This is called when timeouts occur.  This implementation prints
   * out an error message (since it really shouldn't be implemented in
   * this class).
   */
  virtual int execute (void* arg = 0);

  /// Whether events should be dropped or forwarded.
  int suspended_;

  /// Whether any correlating should be done for this event.
  u_long correlation_type_;

  /// Event subscribed to.
  RtecEventChannelAdmin::Dependency *dependency_;

  /// The target consumer of events.
  ACE_ES_Consumer_Correlation *correlation_;

  /// Correlation group index of event_->type_.
  int type_id_;

  /// This should be a set.  We'll just have room for one now.
  ACE_ES_Disjunction_Group *disjunction_group_;

  /// Lock for reference count.
  ACE_Atomic_Op<ACE_ES_MUTEX, int> ref_count_;
};

/**
 * @class ACE_ES_Consumer_Rep_Timeout
 *
 * @brief Consumer Representation.
 *
 * These are stored in the subscription module.  They store
 * information that allows optimized correlations.  It represents
 * the consumer that will handle *one* type of event.
 */
class TAO_RTOLDEvent_Export ACE_ES_Consumer_Rep_Timeout : public ACE_ES_Consumer_Rep
{
public:
  /// Default construction.
  ACE_ES_Consumer_Rep_Timeout (void);

  /// <dep> describes the event subscribed to and the method handling
  /// the event.   <correlation> is the parent correlation object.
  void init (ACE_ES_Consumer_Correlation *correlation,
             RtecEventChannelAdmin::Dependency &dep);

  // = Get/set timer returned from the reactor.
  int timer_id (void);
  void timer_id (int);

  // = Get/set preemption priority.
  RtecScheduler::OS_Priority preemption_priority (void);
  void preemption_priority (RtecScheduler::OS_Priority pp);

protected:
  /// This is called when timeouts occur.  Calls correlation_->
  virtual int execute (void* arg = 0);

  /// For cancelling timers.
  int timer_id_;

  /// Store the preemption priority so we can cancel the correct timer.
  /// The priority values may change during the life.
  RtecScheduler::OS_Priority preemption_priority_;

  TAO_EC_Event timeout_event_;
};

// ************************************************************

/**
 * @class ACE_ES_Subscription_Info
 *
 * @brief Event Service Subscription Info
 *
 * Contains information on all consumers subscribed to a supplier.
 * Each Push_Supplier_Proxy has an instance of this class.  This
 * should really be defined in Channel_Modules.h, but I want to
 * have an instance of it in each ACE_Push_Supplier_Proxy.  This
 * allows us to reduce the amount of dynamic memory allocation.
 */
class TAO_RTOLDEvent_Export ACE_ES_Subscription_Info
{
public:
  /// Free up dynamic resources.
  ~ACE_ES_Subscription_Info (void);

  typedef ACE_Unbounded_Set_Ex_Iterator<ACE_ES_Consumer_Rep *> Subscriber_Set_Iterator;
  typedef ACE_Unbounded_Set_Ex<ACE_ES_Consumer_Rep *> Subscriber_Set;

/**
 * @class Type_Subscribers
 *
 * There is one of these for each event type generated by the
 * supplier.  It contains the subscribers and the
 * dependency_info_ describing the method that generates this
 * event type.
 */
  class Type_Subscribers
  {
  public:
    /**
     * Construction requires a dependency info describing the method
     * that generates events for the consumers_.  We use a pointer so
     * that a null can be passed in this->insert_or_allocate.
     */
    Type_Subscribers (RtecScheduler::Dependency_Info *d)
      : dependency_info_ (d) {}

    // void operator= (const Subscriber_Set &);
    // Copy.

    /// All the consumers that have registered for this event.
    Subscriber_Set consumers_;

    /// Description of the method that generates this event.
    RtecScheduler::Dependency_Info *dependency_info_;
  };

  typedef RtecEventComm::EventType EXT;
  typedef Type_Subscribers *INT;
  typedef ACE_Null_Mutex SYNCH;
  typedef ACE_Map_Manager<EXT, INT, SYNCH> Subscriber_Map;
  typedef ACE_Map_Iterator<EXT, INT, SYNCH> Subscriber_Map_Iterator;
  typedef ACE_Map_Entry<EXT, INT> Subscriber_Map_Entry;

  /// Source-based subscribers.
  Subscriber_Set source_subscribers_;

  /// Type-based subscribers.
  Subscriber_Map type_subscribers_;

  // = These are just typedefs for source-based subscriptions.
  typedef RtecEventComm::EventSourceID sEXT;
  typedef Subscriber_Set *sINT;
  typedef ACE_Map_Manager<sEXT, sINT, SYNCH> SourceID_Map;
  typedef ACE_Map_Iterator<sEXT, sINT, SYNCH> SourceID_Map_Iterator;
  typedef ACE_Map_Entry<sEXT, sINT> SourceID_Map_Entry;

  /// Serializes writes to source_subscribers_ and type_subscribers_.
  ACE_ES_RW_LOCK lock_;

  /**
   * <source_subscribers> contains a mapping of source id to consumer
   * list.  Insert <consumer> into the list of consumers subscribed to
   * <sid>.  Allocate a list for <sid> if necessary.
   */
  static int insert_or_allocate (SourceID_Map &source_subscribers,
                                 ACE_ES_Consumer_Rep *consumer,
                                 RtecEventComm::EventSourceID sid);

  /**
   * Add <consumer> to the set of consumers bound to <type> in
   * <type_subscribers>.  If there is consumer set for <type>, one is
   * allocated.  Returns -1 on failure, 0 otherwise.
   */
  static int insert_or_allocate (Subscriber_Map &type_subscribers,
                                 ACE_ES_Consumer_Rep *consumer,
                                 RtecEventComm::EventType type);

  /**
   * Add <consumer> to the set of consumers bound to <type> in
   * <type_subscribers>.  If there is consumer set for <type>, the
   * operation fails.  Returns -1 on failure, 0 otherwise.
   */
  static int insert_or_fail (Subscriber_Map &type_subscribers,
                             ACE_ES_Consumer_Rep *consumer,
                             RtecEventComm::EventType type,
                             RtecScheduler::Dependency_Info *&dependency);

  /// Remove <consumer> from the consumer set in <type_map> set
  /// corresponding to <type>.
  static int remove (Subscriber_Map &type_map,
                     ACE_ES_Consumer_Rep *consumer,
                     RtecEventComm::EventType type);

  /// Remove <consumer> from the consumer set in the
  /// <source_subscribers> set corresponding to <sid>.
  static int remove (SourceID_Map &source_subscribers,
                     ACE_ES_Consumer_Rep *consumer,
                     RtecEventComm::EventSourceID sid);

  /// Insert all elements of <src> into <dest>.
  static void append_subscribers (Subscriber_Set &dest,
                                  Subscriber_Set &src);
};

// ************************************************************

// Forward declarations.
class ACE_ES_Dispatch_Request;
class ACE_Push_Consumer_Proxy;

/**
 * @class ACE_ES_Consumer_Correlation
 *
 * @brief Event Service Consumer_Correlation
 *
 * There is one Consumer Correlation object per call to
 * connect_push_consumer.  It handles all the consumer's
 * correlation dependencies including timeouts.  This is also a
 * PushSupplier to support event forwarding.
 */
class TAO_RTOLDEvent_Export ACE_ES_Consumer_Correlation : public POA_RtecEventComm::PushSupplier
{
public:
  /// Default construction.
  ACE_ES_Consumer_Correlation (void);

  /// Deletes lock_.
  virtual ~ACE_ES_Consumer_Correlation (void);

  /**
   * Initialization.  <correlation_module> is stored for delegating
   * channel operations.  <consumer> is stored to access the consumers
   * qos and filterin data.  Returns 0 on success, -1 on failure.
   */
  int connected (ACE_Push_Consumer_Proxy *consumer,
                 ACE_ES_Correlation_Module *correlation_module);

  /// Shutdown.
  int disconnecting (void);

  /// Takes <event> and adds it to the correlation.  Returns the
  /// dispatch request that should be forwarded.
  ACE_ES_Dispatch_Request *push (ACE_ES_Consumer_Rep *consumer,
                                 const TAO_EC_Event& event);

  /// Stop forwarding events to the calling consumer.
  void suspend (void);

  /// Resume forwarding events to the calling consumer.
  void resume (void);

  /// Pointer back to the main correlation module.  This is public so
  /// that ACE_ES_Consumer_Rep_Timeout::execute can access it.
  ACE_ES_Correlation_Module *correlation_module_;

private:
  /// Called when the channel disconnects us.
  virtual void disconnect_push_supplier (ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
      ACE_THROW_SPEC ((CORBA::SystemException));

  /// Dynamically allocates structures needed for correlations.  0 on
  /// success, -1 on failure.
  int allocate_correlation_resources (ACE_ES_Dependency_Iterator &iter);

  /// Helper function for this->push.
  ACE_ES_Dispatch_Request * correlate (ACE_ES_Consumer_Rep *cr,
                                       const TAO_EC_Event& event);

  // = Registration helper functions.
  int register_deadline_timeout (RtecEventChannelAdmin::Dependency &dependency,
                                 RtecEventComm::EventType group_type,
                                 int cgindex,
                                 int dgindex,
                                 int &trep_index);
  int register_interval_timeout (RtecEventChannelAdmin::Dependency &dependency,
                                 RtecEventComm::EventType group_type,
                                 int cgindex,
                                 int dgindex,
                                 int &trep_index);
  int register_event (RtecEventChannelAdmin::Dependency &dependency,
                      RtecEventComm::EventType group_type,
                      int cgindex,
                      int dgindex,
                      int &crep_index);

  ACE_ES_Consumer_Rep *get_consumer_rep (RtecEventChannelAdmin::Dependency &dependency,
                                          int &crep_index);
  int new_type_id (void);

  int type_id_index_;

  /// For event forwarding.
  RtecEventChannelAdmin::ProxyPushConsumer_ptr channel_;

  /// Supplier QOS specifications.
  RtecEventChannelAdmin::SupplierQOS qos_;

  // Events waiting to be forwarded.
  TAO_EC_Event_Array *pending_events_;

  // Used to synchronize pending_events_ and by the correlation module.
  /// Used to lock shared state.
  ACE_ES_MUTEX lock_;

  ACE_Push_Consumer_Proxy *consumer_;

  /// A bit is set for each dependency satisfied.
  u_long pending_flags_;

  /// Array of consumer rep pointers.
  ACE_ES_Consumer_Rep **consumer_reps_;
  int n_consumer_reps_;
  ACE_ES_Consumer_Rep_Timeout *timer_reps_;
  int n_timer_reps_;

  ACE_ES_Conjunction_Group *conjunction_groups_;
  int n_conjunction_groups_;
  ACE_ES_Disjunction_Group *disjunction_groups_;
  int n_disjunction_groups_;

  /// True when we're connected to the channel for forwarding.
  int connected_;
};

// ************************************************************

/**
 * @class ACE_ES_ACT
 *
 * @brief Event Service ACT
 *
 */
class TAO_RTOLDEvent_Export ACE_ES_ACT
{
public:
  ACE_ES_ACT (void);
  int has_act_;
  RtecEventComm::Event act_;
};

// ************************************************************

// Forward declarations.
class ACE_ES_Dispatch_Request;

/**
 * @class ACE_ES_Consumer_Module
 *
 * @brief Event Service Consumer Module
 *
 * ProxyPushSupplier factory.
 */
class TAO_RTOLDEvent_Export ACE_ES_Consumer_Module : public POA_RtecEventChannelAdmin::ConsumerAdmin
{
public:
  /// Default construction.
  ACE_ES_Consumer_Module (ACE_EventChannel *channel);

  /// Link to the next module.
  void open (ACE_ES_Dispatching_Module *down);

  /// Factory method for push consumer proxies.
  virtual RtecEventChannelAdmin::ProxyPushSupplier_ptr
      obtain_push_supplier (ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
        ACE_THROW_SPEC ((CORBA::SystemException));

  /// Register the consumer with the Event Service.  This handles all
  /// the details regarding Correlation_Module and Subscription_Module.
  void connected (ACE_Push_Consumer_Proxy *consumer
                  ACE_ENV_ARG_DECL_NOT_USED);

  /// Unregister the consumer from the Event Service.
  void disconnecting (ACE_Push_Consumer_Proxy *consumer
                      ACE_ENV_ARG_DECL_NOT_USED);

  virtual void push (const ACE_ES_Dispatch_Request *request
                     ACE_ENV_ARG_DECL_NOT_USED);

  /// Allow transformations to RtecEventChannelAdmin::ConsumerAdmin.
  RtecEventChannelAdmin::ConsumerAdmin_ptr get_ref (ACE_ENV_SINGLE_ARG_DECL_NOT_USED);

  /// This is called by Shutdown_Consumer command objects when a
  /// consumer proxy is ready to be deleted.
  void shutdown_request (ACE_ES_Dispatch_Request *request);

  /// Actively disconnect from all consumers.
  void shutdown (void);

  /**
   * Fill the QoS with the disjuction off all the subscriptions in
   * this EC.
   * It leaves the gateways out of the list.
   */
  void fill_qos (RtecEventChannelAdmin::ConsumerQOS& c_qos);

private:
  typedef ACE_Unbounded_Set_Ex_Iterator<ACE_Push_Consumer_Proxy *> Consumer_Iterator;
  typedef ACE_Unbounded_Set_Ex<ACE_Push_Consumer_Proxy *> Consumers;

  /// Protects access to all_consumers_.
  ACE_ES_MUTEX lock_;

  Consumers all_consumers_;

  /// Used to test for shutdown.
  ACE_EventChannel *channel_;

  /// Next module down.
  ACE_ES_Dispatching_Module *down_;
};

// ************************************************************

// Forward declaration.
class ACE_ES_Subscription_Module;

/**
 * @class ACE_ES_Correlation_Module
 *
 * @brief Event Service Correlation Module
 *
 */
class TAO_RTOLDEvent_Export ACE_ES_Correlation_Module
{
public:
  /// Default construction.
  ACE_ES_Correlation_Module (ACE_EventChannel *channel);

  /// Link to adjacent modules.
  void open (ACE_ES_Dispatching_Module *up,
             ACE_ES_Subscription_Module *down);

  /// Create the consumers filter object.
  void connected (ACE_Push_Consumer_Proxy *consumer
                  ACE_ENV_ARG_DECL_NOT_USED);

  /// Release the consumers filter object.
  void disconnecting (ACE_Push_Consumer_Proxy *consumer
                      ACE_ENV_ARG_DECL_NOT_USED);

  /**
   * Take in an event and its subscriber.  Apply consumer-specific
   * filters to each event and forward any dispatch requests to the
   * Dispatching Module.
   */
  void push (ACE_ES_Consumer_Rep *consumer,
             const TAO_EC_Event &event
             ACE_ENV_ARG_DECL_NOT_USED);

  // = These are called by ACE_ES_Consumer_Reps.

  /// Forwards to the subscription module.
  int subscribe (ACE_ES_Consumer_Rep *consumer);

  /// Forwards to the subscription module.
  int unsubscribe (ACE_ES_Consumer_Rep *consumer);

  /// Schedule consumer timeout.  Return 0 on success, -1 on failure.
  int schedule_timeout (ACE_ES_Consumer_Rep_Timeout *consumer);

  /// Cancel consumer timeout.  Return 0 on success, -1 on failure.
  int cancel_timeout (ACE_ES_Consumer_Rep_Timeout *consumer);

  /// Reschedule consumer timeout.  Return 0 on success, -1 on failure.
  int reschedule_timeout (ACE_ES_Consumer_Rep_Timeout *consumer);

  /// The master channel.  This is public so that Consumer_Correlation
  /// objects can access it.
  ACE_EventChannel *channel_;

  /// Does nothing.
  void shutdown (void);

private:
  /// Next module up.
  ACE_ES_Dispatching_Module *up_;

  /// Next module down.
  ACE_ES_Subscription_Module *subscription_module_;
};

// ************************************************************

// Forward declaration.
class ACE_ES_Supplier_Module;
class ACE_Push_Supplier_Proxy;

/**
 * @class ACE_ES_Subscription_Module
 *
 * @brief Event Service Subscription Module
 *
 * = SYNCHRONIZATION
 * This is currently implemented with very coarse-grain
 * synchronization.  Basically, there is a single readers/writer
 * lock.  All operations acquire the writer lock to change any
 * subscription record.  All operations acquire a reader lock to
 * read any subscription record.  This is fine for normal
 * operations (which are *all* read operations).  However, the
 * initialization and shutdown periods might benefit from the
 * potential increase in concurrency if we used finer grain locks
 * (e.g., lock-per-source).
 */
class TAO_RTOLDEvent_Export ACE_ES_Subscription_Module
{
public:
  /// Default construction.
  ACE_ES_Subscription_Module (ACE_EventChannel *channel);

  /// Link to the adjacent modules.
  void open (ACE_ES_Correlation_Module *up,
             ACE_ES_Supplier_Module *down);

  /// Deletes the lock_.
  ~ACE_ES_Subscription_Module (void);

  /// Register a new consumer.  Calls into <consumer> to figure out the
  /// subscription options.  Returns 0 on success, -1 on failure.
  int subscribe (ACE_ES_Consumer_Rep *consumer);

  /// Removes the -consumer- from any subscription lists.
  int unsubscribe (ACE_ES_Consumer_Rep *consumer);

  void connected (ACE_Push_Supplier_Proxy *supplier
                  ACE_ENV_ARG_DECL_NOT_USED);
  void disconnecting (ACE_Push_Supplier_Proxy *supplier
                      ACE_ENV_ARG_DECL_NOT_USED);

  /// Takes in an event and pushes subscriber sets to the
  /// Correlation Module.
  void push (ACE_Push_Supplier_Proxy *source,
             const TAO_EC_Event &event
             ACE_ENV_ARG_DECL_NOT_USED);

  /// Unsubscribes all consumers from the suppliers.
  void shutdown (void);

private:
  /// Reregister any consumers that registered for <source_id> before
  /// it actually connected to the channel.
  void reregister_consumers (RtecEventComm::EventSourceID source_id);

  /// The channel of all channels.
  ACE_EventChannel *channel_;

  /// Source-only subscribers.
  /*
  typedef ACE_ES_Subscription_Info::Subscriber_Set INT;
  typedef ACE_Null_Mutex SYNCH;
  typedef ACE_Map_Manager<EXT, INT, SYNCH> Source_Collection;
  typedef ACE_Map_Iterator<EXT, INT, SYNCH> Source_Collection_Iterator;
  typedef ACE_Map_Entry<EXT, INT> Source_Collection_Entry;
  Source_Collection source_subscription_info_;
  */

  // = Subscribe helper methods.  Returns 0 on success, -1 on failure.

  int subscribe_all (ACE_ES_Consumer_Rep *consumer);

  int subscribe_type (ACE_ES_Consumer_Rep *consumer,
                      RtecEventComm::EventType type);

  int subscribe_source (ACE_ES_Consumer_Rep *consumer,
                        RtecEventComm::EventSourceID source);

  int subscribe_source_type (ACE_ES_Consumer_Rep *consumer,
                             RtecEventComm::EventSourceID source,
                             RtecEventComm::EventType type);

  int unsubscribe_all (ACE_ES_Consumer_Rep *consumer);

  int unsubscribe_type (ACE_ES_Consumer_Rep *consumer,
                        RtecEventComm::EventType type);

  int unsubscribe_source (ACE_ES_Consumer_Rep *consumer,
                          RtecEventComm::EventSourceID source);

  int unsubscribe_source_type (ACE_ES_Consumer_Rep *consumer,
                               RtecEventComm::EventSourceID source,
                               RtecEventComm::EventType type);

  // = Push helper methods.

  /// Push <event> to all consumers subscribed to all events from
  /// <source>.  Returns 0 on success, -1 on failure.
  int push_source (ACE_Push_Supplier_Proxy *source,
                   const TAO_EC_Event &event
                   ACE_ENV_ARG_DECL);

  /// Push <event> to all consumers subscribed to <event>.type_ from
  /// <source>.  Returns 0 on success, -1 on failure.
  int push_source_type (ACE_Push_Supplier_Proxy *source,
                        const TAO_EC_Event &event
                        ACE_ENV_ARG_DECL);

  /// Push <event> to all_suppliers_.
  void push_all (const TAO_EC_Event &event
                 ACE_ENV_ARG_DECL_NOT_USED);

  /// Next module up stream.
  ACE_ES_Correlation_Module *up_;

  /// Next module down stream.
  ACE_ES_Supplier_Module *down_;

  typedef ACE_Unbounded_Set_Ex_Iterator<ACE_Push_Supplier_Proxy *> Supplier_Iterator;
  typedef ACE_Unbounded_Set_Ex<ACE_Push_Supplier_Proxy *> Suppliers;

  /// All suppliers.
  Suppliers all_suppliers_;

  /// Type-based subscribers.
  ACE_ES_Subscription_Info::Subscriber_Map type_subscribers_;

  /// Source-based subscribers.
  ACE_ES_Subscription_Info::SourceID_Map source_subscribers_;

  /// Protects access to all_suppliers_ and type_suppliers_;
  ACE_ES_RW_LOCK lock_;

  /// The scheduler;
  RtecScheduler::Scheduler_ptr scheduler_;
};

// ************************************************************

/**
 * @class ACE_ES_Supplier_Module
 *
 * @brief Event Service Supplier Proxy Module
 *
 * ProxyPushConsumer factory.
 */
class TAO_RTOLDEvent_Export ACE_ES_Supplier_Module : public POA_RtecEventChannelAdmin::SupplierAdmin
{
public:
  /// Default construction.
  ACE_ES_Supplier_Module (ACE_EventChannel *channel);

  /// Associate the module to a channel.
  void open (ACE_ES_Subscription_Module *up);

  /// Factory method for push supplier proxies.
  virtual RtecEventChannelAdmin::ProxyPushConsumer_ptr
      obtain_push_consumer (ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
        ACE_THROW_SPEC ((CORBA::SystemException));

  /// The supplier module acts on behalf of the supplier proxy to
  /// forward events through the channel.
  virtual void push (ACE_Push_Supplier_Proxy *proxy,
                     RtecEventComm::EventSet &event
                     ACE_ENV_ARG_DECL_NOT_USED);

  /// Register the consumer with the Event Service.  This handles all
  /// the details regarding Correlation_Module and Subscription_Module.
  void connected (ACE_Push_Supplier_Proxy *supplier
                  ACE_ENV_ARG_DECL_NOT_USED);

  /// Unregister the consumer from the Event Service.
  void disconnecting (ACE_Push_Supplier_Proxy *supplier
                      ACE_ENV_ARG_DECL_NOT_USED);

  /// Allow transformations to RtecEventComm::PushConsumer.
  RtecEventChannelAdmin::SupplierAdmin_ptr get_ref (ACE_ENV_SINGLE_ARG_DECL_NOT_USED);

  /// Actively disconnect from all suppliers.
  void shutdown (void);

  /**
   * Fill the QoS with the disjuction off all the publications in
   * this EC.
   * It leaves the gateways out of the list.
   */
  void fill_qos (RtecEventChannelAdmin::SupplierQOS& s_qos);

private:
  typedef ACE_Unbounded_Set_Ex_Iterator<ACE_Push_Supplier_Proxy *> Supplier_Iterator;
  typedef ACE_Unbounded_Set_Ex<ACE_Push_Supplier_Proxy *> Suppliers;

  /// All suppliers.
  Suppliers all_suppliers_;

  /// Protects access to all_suppliers_ and type_suppliers_;
  ACE_ES_MUTEX lock_;

  ACE_ES_Subscription_Module *up_;

  /// Used to test for shutdown.
  ACE_EventChannel *channel_;
};

// ************************************************************

// Forward declarations.
class ACE_EventChannel;

// = Event Channel interfaces.

/**
 * @class ACE_Push_Supplier_Proxy
 *
 * @brief Push Supplier Proxy.
 *
 * To the channel, this is a proxy to suppliers.  To suppliers, it
 * exports a PushConsumer interface.  It is a
 * RtecEventChannelAdmin::ProxyPushConsumer.  Suppliers use this
 * interface to connect to the channel, push events to consumers,
 * and to disconnect from the channel.
 */
class TAO_RTOLDEvent_Export ACE_Push_Supplier_Proxy : public POA_RtecEventChannelAdmin::ProxyPushConsumer, public PortableServer::RefCountServantBase
{
public:
  /// Must be created with an owning supplier admin.
  ACE_Push_Supplier_Proxy (ACE_ES_Supplier_Module *supplier_module);

  // = Operations public to suppliers.

  /**
   * Suppliers connect via this interface.  <push_supplier> is a
   * reference to the supplier.  <qos> represents the publish types of
   * the supplier.
   */
  virtual void connect_push_supplier (
      RtecEventComm::PushSupplier_ptr push_supplier,
      const RtecEventChannelAdmin::SupplierQOS& qos
      ACE_ENV_ARG_DECL_NOT_USED)
        ACE_THROW_SPEC ((CORBA::SystemException,
                         RtecEventChannelAdmin::AlreadyConnected));

  /// Data arriving from a PushSupplier that must be sent to
  /// consumers.  This is the entry point of all events.
  virtual void push (const RtecEventComm::EventSet &event
                     ACE_ENV_ARG_DECL_NOT_USED)
      ACE_THROW_SPEC ((CORBA::SystemException));

  /// Disconnect the supplier from the channel.
  virtual void disconnect_push_consumer (ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
      ACE_THROW_SPEC ((CORBA::SystemException));

  // = Operations for the Event Channel.

  /// Returns 1 if the proxy has been connected to a "remote" client.
  int connected (void);

  /// Actively disconnect from the supplier.
  void shutdown (void);

  // This is a hook so that the Subscription Module can associate
  // state with supplier proxies.
  ACE_ES_Subscription_Info &subscription_info (void);

  /// Filtering criteria.
  RtecEventChannelAdmin::SupplierQOS &qos (void);

  /// Is this object a proxy for -rhs-.  Simple pointer comparison for now.
  int operator== (const RtecEventComm::EventSourceID rhs);

  /// Returns underlying supplier object ref.
  RtecEventComm::EventSourceID source_id (void);

  /// The QoS for this supplier
  const RtecEventChannelAdmin::SupplierQOS& qos (void) const;

private:
  void time_stamp (RtecEventComm::EventSet &event);

private:
  /// Reference to the supplier's qos params.
  RtecEventChannelAdmin::SupplierQOS qos_;

  ACE_ES_Subscription_Info subscription_info_;

  ACE_ES_Supplier_Module *supplier_module_;

  /// We keep a proxy of the Supplier source_id_;
  RtecEventComm::EventSourceID source_id_;

  /// CORBA reference to remote push supplier.
  RtecEventComm::PushSupplier_ptr push_supplier_;
};

// ************************************************************

/**
 * @class ACE_Push_Consumer_Proxy
 *
 * @brief Push Consumer Proxy.
 *
 * This is the channels proxy to a push consumer.  It implements
 * the RtecEventChannelAdmin::ProxyPushSupplier IDL interface.
 * Consumers use this interface to connect and disconnect from the
 * channel.
 */
class TAO_RTOLDEvent_Export ACE_Push_Consumer_Proxy : public POA_RtecEventChannelAdmin::ProxyPushSupplier, public PortableServer::RefCountServantBase
{
public:
  /// Must be created with an consumer admin.
  ACE_Push_Consumer_Proxy (ACE_ES_Consumer_Module *cm);

  /// Default destruction
  virtual ~ACE_Push_Consumer_Proxy (void);

  // = Interfaces exported to consumers.

  /// A push consumer is connecting.  <push_consumer> is a reference to
  /// the consumer.  <qos> is the subscription types for the consumer.
  virtual void connect_push_consumer (
       RtecEventComm::PushConsumer_ptr push_consumer,
       const RtecEventChannelAdmin::ConsumerQOS& qos
       ACE_ENV_ARG_DECL_NOT_USED)
    ACE_THROW_SPEC ((CORBA::SystemException,
                     RtecEventChannelAdmin::AlreadyConnected,
                     RtecEventChannelAdmin::TypeError));

  /// The consumer is disconnecting.
  virtual void disconnect_push_supplier (ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
      ACE_THROW_SPEC ((CORBA::SystemException));

  /// Stop forwarding events to the calling consumer.
  virtual void suspend_connection (ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
      ACE_THROW_SPEC ((CORBA::SystemException));

  /// Resume forwarding events to the calling consumer.
  virtual void resume_connection (ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
      ACE_THROW_SPEC ((CORBA::SystemException));

  // = Event Channel operations.

  /// Push <events> to push_consumer_.
  void push (const RtecEventComm::EventSet &events
             ACE_ENV_ARG_DECL_NOT_USED);

  /// Returns 1 if the proxy has been connected to a "remote" client.
  int connected (void);

  /// Actively disconnect from the consumer.
  void shutdown (void);

  /// Access the consumer-specific Consumer_Correlation.
  ACE_ES_Consumer_Correlation &correlation (void);

  /// Filtering criteria.
  RtecEventChannelAdmin::ConsumerQOS &qos (void);

private:
  /// A reference to the consumers Quality of Service parameters.
  RtecEventChannelAdmin::ConsumerQOS qos_;

  /// A hook so that the Correlation Module can associate correlation
  /// information with the consumer.
  ACE_ES_Consumer_Correlation correlation_;

  /// Reference to our push consumer.
  RtecEventComm::PushConsumer_var push_consumer_;

  /// TODO: Maybe this should be a _var or _duplicate/_release should
  /// be used
  ACE_ES_Consumer_Module *consumer_module_;
};

#if defined (__ACE_INLINE__)
#include "Event_Channel.i"
#endif /* __ACE_INLINE__ */

#if defined(_MSC_VER) && (_MSC_VER >= 1200)
#pragma warning(pop)
#endif /* _MSC_VER */

#include "ace/post.h"
#endif /* ACE_EVENT_CHANNEL_H */
