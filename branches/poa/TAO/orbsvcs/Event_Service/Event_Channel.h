/* -*- C++ -*- */
// $Id$
//
// ============================================================================
//
// = LIBRARY
//    ace ORB
// 
// = FILENAME
//    Event_Channel
//
// = AUTHOR
//    Tim Harrison (harrison@cs.wustl.edu)
//
// = DESCRIPTION
//    TAO implementation of the Real Time Event Services. For more
//    detailed information, see
//    http://www.cs.wustl.edu/~schmidt/oopsla.ps.gz
//
// =  NAMING CONVENTIONS
//    Some of the naming might be confusing.  For instance
//    ACE_Push_Consumer_Proxy "is-a" ProxyPushSupplier.  To the
//    channel, ACE_Push_Consumer_Proxy is a proxy to push consumers.
//    To a push consumer, ACE_Push_Consumer_Proxy is a proxy to push
//    suppliers.  I chose to name classes relative to the Event
//    Channel.
//
// ============================================================================

#ifndef ACE_EVENT_CHANNEL_H
#define ACE_EVENT_CHANNEL_H

#include "ace/Containers.h"
#include "ace/Map_Manager.h"

#include "tao/Timeprobe.h"
#include "Local_ESTypes.h"
#include "CORBA_Utils_T.h"
#include "Task_Manager.h"
#include "ReactorTask.h"

//ACE_INLINE void operator += (ACE_CORBA_Sequence<RtecEventComm::Event_var> &dest,
//				 RtecEventComm::Event *item);

ACE_INLINE int operator == (const RtecEventComm::Event &event1, 
			    const RtecEventComm::Event &event2);
// This operation could be part of the classes, but in order to stay
// CORBA compliant, we're adding them as global operators.

// ************************************************************

class ACE_ES_Event_Container : public RtecEventComm_Event
// = TITLE
//    Event Container
//
// = DESCRIPTION
//    Basically an ACE_ES_Event with reference counting and
//    thread-specific memory allocation.
{
public:
  ACE_ES_Event_Container (void);
  // Default construction.

  ~ACE_ES_Event_Container (void);
  // Destruction.

  ACE_ES_Event_Container (const ACE_ES_Event_Container &);
  // Copy construction.

  ACE_ES_Event_Container (const RtecEventComm::Event &);
  // Construction with an event.

  ACE_ES_Event_Container *_duplicate (void);
  // Increments ref_count_ and returns this.

  void _release (void);
  // Decrements ref_count_ and deletes if 0.

  int operator== (const ACE_ES_Event_Container &event);
  // Returns 1 if the two are "equal," 0 otherwise.  Determines
  // equality using source_ and type_ only.  A 0 source_ is a wildcard
  // (always equal).  A type_ of ACE_ES_EVENT_ANY is also a wildcard.

  void *operator new (size_t nbytes);
  // Allocates memory from a thread-specific memory pool.

  void operator delete (void *);
  // Returns memory to a thread-specific memory pool.

  void dump (void);

private:
  int ref_count_;
};

typedef ACE_CORBA_var<ACE_ES_Event_Container> ACE_ES_Event_Container_var;

ACE_INLINE void operator += (ACE_CORBA_Sequence<ACE_ES_Event_Container_var> &dest,
			     ACE_ES_Event_Container *item);

#if defined(ACE_ES_LACKS_ORB)
// Utility for debugging sequences.
ACE_Svc_Export void dump_sequence (const ACE_CORBA_Sequence<RtecEventComm::Event> &seq);
#endif /* ACE_ES_LACKS_ORB */

// Utility for debugging events.
void dump_event (const RtecEventComm::Event &event);

// ************************************************************

class ACE_RTU_Manager
// = TITLE
//   ACE RTU Manager
//   
// = DESCRIPTION
{
public:
  ACE_RTU_Manager (int active);
  // If <active> == 0, everything returns 0.  If <active> != 0, RTUs
  // galore.

  int should_preempt (void);
  // Returns 1 if the current task should preempt itself.  Otherwise,
  // returns 0.  Resets should_preempt to zero.

  void should_preempt (int s);
  // Called by the dispatching module when the current task should
  // preempt itself.

  void not_done (int nd);
  // If <nd> != 0, the current running task will be enqueued at the
  // head of its dispatch tail.

  int not_done (void);
  // Returns 1 if the current task needs to be dispatched again.
  // Resets not_done_ to 0;

  // = Get/set the priority of the current running task.
  RtecScheduler::OS_Priority priority (void);
  void priority (RtecScheduler::OS_Priority priority);

private:
  int active_;
  int should_preempt_;
  int not_done_;
  RtecScheduler::OS_Priority priority_;
};

// ************************************************************

// Chesire cat.
class ACE_ES_Priority_Timer;
// Forward declarations.
class ACE_ES_Consumer_Module;
class ACE_ES_Correlation_Module;
class ACE_ES_Subscription_Module;
class ACE_ES_Supplier_Module;
// This forward decl and typedef allow us to remove inheritence later
// on without changing any code.
class ACE_ES_Dispatching_Base;
typedef ACE_ES_Dispatching_Base ACE_ES_Dispatching_Module;

// ec..
class ACE_EventChannel : public RtecEventChannelAdmin_EventChannelBOAImpl
// = TITLE
//   ACE Event Channel.
//   
// = DESCRIPTION
//   Implementation of COSS Event Channel.  For more detailed
//   information, see http://www.cs.wustl.edu/~mda/event.html.
{
public:
  enum { INITIAL_STATE = 0,
	 CONSUMER = 1, SUPPLIER = 2, 
	 SHUTDOWN = CONSUMER | SUPPLIER };

  ACE_EventChannel (u_long type = ACE_DEFAULT_EVENT_CHANNEL_TYPE);
  // Construction of the given <type>.  Check the **_CHANNEL
  // enumerations defined below.

  virtual ~ACE_EventChannel (void);
  // Calls destroy.

  // = Accessor methods to Event Channel objects.  The Event Channel
  // acts as a sort of service repository of object references.  All
  // objects in the Event Service come to this interface to obtain
  // object references during initialization.

  virtual RtecEventChannelAdmin::ConsumerAdmin_ptr for_consumers (CORBA::Environment &);
  // Consumer administration factory method.

  virtual RtecEventChannelAdmin::SupplierAdmin_ptr for_suppliers (CORBA::Environment &);
  // Supplier administration factory method.

  virtual void destroy (CORBA::Environment &);
  // Explicitly shut down the channel.

  RtecEventChannelAdmin::EventChannel_ptr get_ref (void);
  // Allow transformations to RtecEventChannelAdmin::EventChannel.

  ACE_RTU_Manager *rtu_manager (void);
  // Returns a reference to the RTU manager.

  ACE_ES_Priority_Timer *timer (void);
  // Timer accessor.

  // = These should be private.
  ACE_ES_Consumer_Module *consumer_module_;
  ACE_ES_Dispatching_Module *dispatching_module_;
  ACE_ES_Correlation_Module *correlation_module_;
  ACE_ES_Subscription_Module *subscription_module_;
  ACE_ES_Supplier_Module *supplier_module_;

  ACE_ES_Priority_Timer *timer_;

  void report_connect (u_long);
  // Consumer or supplier connected.

  void report_disconnect (u_long);
  // Consumer or supplier disconnected.

  void shutdown (void);
  // Do not call this.  The last module has shut down.

private:
  ACE_RTU_Manager *rtu_manager_;
  // The RTU manager dude!

  u_long type_;
  // Can be any **_CHANNEL. (well, except NO_CHANNEL).

  u_long state_;
  // Can be INITIAL_STATE, NO_CONSUMERS, NO_SUPPLIERS, or SHUTDOWN.

  ACE_ES_MUTEX lock_;
  // Used to lock shared state.

  int destroyed_;
  // Ensures this->destory is executed only once.
};

// ************************************************************

class ACE_ES_Dependency_Iterator
// = TITLE 
//    ConsumerQOS Iterator
//
// = DESCRIPTION
//    This is used by the Event Channel to parse ConsumerDependency objects.
{
public:
  ACE_ES_Dependency_Iterator (RtecEventChannelAdmin::DependencySet &rep);
  // Construct and iterator for <rep>.

  int advance_dependency (void);
  // Returns 0 if the advance succeeded.  Returns -1 if there are no
  // more dependencies in the group.
  
  int parse (void);
  // Cache values for n_** methods.

  int n_conjunctions (void);
  // Returns the number of conjunction groups in the dependency set.

  int n_disjunctions (void);
  // Returns the number of disjunction groups in the dependency set.

  int n_timeouts (void);
  // Returns the number of timeouts registered.

  int n_events (void);
  // Returns the number of events registered.

  RtecEventChannelAdmin::Dependency &operator *(void);
  // Accessor to the current ConsumerDependency pointed to by the
  // iterator.

  RtecScheduler::handle_t first_rt_info (void);
  // Returns the first RT_Info in the dependencies.

protected:
  RtecScheduler::handle_t rt_info_;
  // The first rt_info in the dependencies.

  RtecEventChannelAdmin::DependencySet &rep_;
  // Reference to the dependency array.

  int index_;
  // Index into rep_.

  RtecEventComm::EventType group_type_;
  // The type of the current correlation group.

  int n_conjunctions_;
  // Number of conjunction groups.

  int n_disjunctions_;
  // Number of disjunction groups.

  int n_timeouts_;
  // Number of timeouts registered.

  int n_events_;
  // Number of events registered.
};

// ************************************************************

class ACE_ES_Timer_ACT
// = TITLE
//    Timer Asynchronous Completion Token
//
// = DESCRIPTION
//    Implements Command pattern with timers.
{
public:
  virtual void execute (void) = 0;
};

// ************************************************************
// Forward decl.
class ACE_ES_Consumer_Rep_Timeout;

class ACE_ES_Disjunction_Group
// = TITLE
//    Disjunction Group
//
// = DESCRIPTION
//    Represents a disjunction group, such as (A|B|C).
{
public:
  ACE_ES_Disjunction_Group (void);
  // Default construction.

  void set_correlation_module (ACE_ES_Correlation_Module *cm);
  // <cm> is needed for rescheduling deadlines.

  void reschedule_deadline (void);
  // If deadline_timer_rep_ is set, it is cancelled and rescheduled.

  int set_deadline_timeout (ACE_ES_Consumer_Rep_Timeout *cr);
  // Set the group's reference to the deadline timer.  Returns 0 on
  // success, -1 on failure.

  typedef ACE_CORBA_Sequence<ACE_ES_Event_Container_var> Event_Set;
  
  virtual void add_events (Event_Set *outbox,
			   Event_Set *pending_events,
			   u_long &pending_flags);
  // Does nothing.  This is the only virtual method in this little
  // heirarchy with the conjunction group.

  void set_act (RtecEventComm::Event &act);
  // Set the ACT for this group.

protected:
  ACE_ES_Event_Container_var act_;
  // To be sent with this group.

private:
  ACE_ES_Consumer_Rep_Timeout *deadline_timer_rep_;
  // The disjunction group keeps a reference to the deadline timer.

  ACE_ES_Correlation_Module *correlation_module_;
  // Used for cancelling and scheduling deadline_timer_rep_.
};

// ************************************************************

class ACE_ES_Conjunction_Group : public ACE_ES_Disjunction_Group
// = TITLE
//    Conjunction Group
//
// = DESCRIPTION
//    Represents a conjunction group, such as (A+B+C).
{
public:
  ACE_ES_Conjunction_Group (void);
  // Default construction.

  int add_type (int type_id);
  // Set the <type_id>th bit in the forward_value_.

  int should_forward (u_long pending_flags);
  // Returns 1 if this conjunction group's dependencies have been
  // satisfied.  Returns 0 otherwise.

  typedef ACE_CORBA_Sequence<ACE_ES_Event_Container_var> Event_Set;
  
  virtual void add_events (Event_Set *outbox,
			   Event_Set *pending_events,
			   u_long &pending_flags);
  // For each bit set in forward_value_, the corresponding events in
  // <pending_events> is added to <outbox>.  Each bit set in
  // <forward_value_> is cleared in <pending_flags>.  If <oubox> == 0,
  // then add_events just clears the pending events and flags.

private:
  u_long forward_value_;
};

// ************************************************************

// Forward decl.
class ACE_ES_Consumer_Correlation;

class ACE_ES_Consumer_Rep : public ACE_ES_Timer_ACT
// = TITLE
//    Consumer Representation.
//
// = DESCRIPTION
//    These are stored in the subscription module.  They store
//    information that allows optimized correlations.  It represents
//    the consumer that will handle *one* type of event.  This
//    probably shouldn't inherit from ACE_ES_Timer_ACT since it's used
//    only by ACE_ES_Consumer_Rep_Timeout.  However, this allows me to
//    minimize dynamic allocation.
{
public:
  ACE_ES_Consumer_Rep (void);
  // Default construction.

  void init (ACE_ES_Consumer_Correlation *correlation,
	     RtecEventChannelAdmin::Dependency &dep);
  // <dep> describes the event subscribed to and the method handling
  // the event.   <correlation> is the parent correlation object.

  virtual ~ACE_ES_Consumer_Rep (void);
  // Virtual destruction.

  RtecEventChannelAdmin::Dependency *dependency (void);
  // The event subscribed to and the method that will handle this
  // event.

  int type_id (void);
  // Get the correlation group index of this consumer rep's event
  // type.

  void type_id (int);
  // Set the correlation group index of this consumer rep's event
  // type.

  enum Correlation_Type 
  { 
    NO_CORRELATION,
    CORRELATE,
    DEADLINE_TIMEOUT,
    GLOBAL_DEADLINE
  };

  u_long correlation_type (void);
  // If this returns 0, then the event associated with this consumer
  // should be forwarded without running any correlations.

  void correlation_type (u_long ct);
  // Set whether the event should be correlated.  <ct> is a
  // Correlation_Type.

  int add_disjunction_group (ACE_ES_Disjunction_Group &);
  // Add a disjunction group.

  ACE_ES_Disjunction_Group *top_group (void);
  // Returns the first disjunction group added via
  // this->add_disjunction_group.

  void reschedule_deadlines (void);
  // Calls reschedule_deadline on all disjunction groups added through
  // this->add_disjunction_group. 

  int receiving_events (void);
  // Returns 1 if events should be sent to this consumer.  Returns 0
  // if they should not (suspended or disconnected).

  void suspend (void);
  // Stop forwarding events to the calling consumer.

  void resume (void);
  // Resume forwarding events to the calling consumer.

  ACE_ES_Consumer_Correlation *correlation (void);
  // Returns the Consumer_Correlation object for the target consumer.

  void disconnect (void);
  // Schedules the consumer rep to be removed from all subscription
  // lists.

  int disconnected (void);
  // Returns 1 if the consumer rep should be removed from all
  // subscription lists.

  void _duplicate (void);
  // Increments ref_count_.

  void _release (void);
  // Decrements ref_count_ and deletes this if 0.

protected:
  int disconnected_;
  // Whether the rep should be removed from all subscription lists.

  virtual void execute (void);
  // This is called when timeouts occur.  This implementation prints
  // out an error message (since it really shouldn't be implemented in
  // this class).

  int suspended_;
  // Whether events should be dropped or forwarded.

  u_long correlation_type_;
  // Whether any correlating should be done for this event.

  RtecEventChannelAdmin::Dependency *dependency_;
  // Event subscribed to.

  ACE_ES_Consumer_Correlation *correlation_;
  // The target consumer of events.

  int type_id_;
  // Correlation group index of event_->type_.

  ACE_ES_Disjunction_Group *disjunction_group_;
  // This should be a set.  We'll just have room for one now.

  ACE_Atomic_Op<ACE_ES_MUTEX, int> ref_count_;
  // Lock for reference count.
};

class ACE_ES_Consumer_Rep_Timeout : public ACE_ES_Consumer_Rep
// = TITLE
//    Consumer Representation.
//
// = DESCRIPTION
//    These are stored in the subscription module.  They store
//    information that allows optimized correlations.  It represents
//    the consumer that will handle *one* type of event.
{
public:
  ACE_ES_Consumer_Rep_Timeout (void);
  // Default construction.
  
  void init (ACE_ES_Consumer_Correlation *correlation,
	     RtecEventChannelAdmin::Dependency &dep);
  // <dep> describes the event subscribed to and the method handling
  // the event.   <correlation> is the parent correlation object.

  // = Get/set timer returned from the reactor.
  int timer_id (void);
  void timer_id (int);
  
  // = Get/set preemption priority.
  RtecScheduler::OS_Priority preemption_priority (void);
  void preemption_priority (RtecScheduler::OS_Priority pp);

protected:
  virtual void execute (void);
  // This is called when timeouts occur.  Calls correlation_->

  int timer_id_;
  // For cancelling timers.

  RtecScheduler::OS_Priority preemption_priority_;
  // Store the preemption priority so we can cancel the correct timer.
  // The priority values may change during the life.

  ACE_ES_Event_Container_var timeout_event_;
};

// ************************************************************

class ACE_ES_Subscription_Info
// = TITLE
//    Event Service Subscription Info
//
// = DESCRIPTION
//    Contains information on all consumers subscribed to a supplier.
//    Each Push_Supplier_Proxy has an instance of this class.  This
//    should really be defined in Channel_Modules.h, but I want to
//    have an instance of it in each ACE_Push_Supplier_Proxy.  This
//    allows us to reduce the amount of dynamic memory allocation.
{
public:
  ~ACE_ES_Subscription_Info (void);
  // Free up dynamic resources.

  typedef ACE_Unbounded_Set_Iterator<ACE_ES_Consumer_Rep *> Subscriber_Set_Iterator;
  typedef ACE_Unbounded_Set<ACE_ES_Consumer_Rep *> Subscriber_Set;

  class Type_Subscribers
    // = DESCRIPTION
    //    There is one of these for each event type generated by the
    //    supplier.  It contains the subscribers and the
    //    dependency_info_ describing the method that generates this
    //    event type.
  {
  public:
    Type_Subscribers (RtecScheduler::Dependency_Info *d)
      : dependency_info_ (d) {}
    // Construction requires a dependency info describing the method
    // that generates events for the consumers_.  We use a pointer so
    // that a null can be passed in this->insert_or_allocate.

    // void operator= (const Subscriber_Set &);
    // Copy.

    Subscriber_Set consumers_;
    // All the consumers that have registered for this event.

    RtecScheduler::Dependency_Info *dependency_info_;
    // Description of the method that generates this event.
  };

  typedef RtecEventComm::EventType EXT;
  typedef Type_Subscribers *INT;
  typedef ACE_Null_Mutex SYNCH;
  typedef ACE_Map_Manager<EXT, INT, SYNCH> Subscriber_Map;
  typedef ACE_Map_Iterator<EXT, INT, SYNCH> Subscriber_Map_Iterator;
  typedef ACE_Map_Entry<EXT, INT> Subscriber_Map_Entry;

  Subscriber_Set source_subscribers_;
  // Source-based subscribers.

  Subscriber_Map type_subscribers_;
  // Type-based subscribers.

  // = These are just typedefs for source-based subscriptions.
  typedef RtecEventComm::EventSourceID sEXT;
  typedef Subscriber_Set *sINT;
  typedef ACE_Map_Manager<sEXT, sINT, SYNCH> SourceID_Map;
  typedef ACE_Map_Iterator<sEXT, sINT, SYNCH> SourceID_Map_Iterator;
  typedef ACE_Map_Entry<sEXT, sINT> SourceID_Map_Entry;

  ACE_ES_RW_LOCK lock_;
  // Serializes writes to source_subscribers_ and type_subscribers_.

  static int insert_or_allocate (SourceID_Map &source_subscribers,
				 ACE_ES_Consumer_Rep *consumer,
				 RtecEventComm::EventSourceID sid);
  // <source_subscribers> contains a mapping of source id to consumer
  // list.  Insert <consumer> into the list of consumers subscribed to
  // <sid>.  Allocate a list for <sid> if necessary.

  static int insert_or_allocate (Subscriber_Map &type_subscribers,
				 ACE_ES_Consumer_Rep *consumer,
				 RtecEventComm::EventType type);
  // Add <consumer> to the set of consumers bound to <type> in
  // <type_subscribers>.  If there is consumer set for <type>, one is
  // allocated.  Returns -1 on failure, 0 otherwise.

  static int insert_or_fail (Subscriber_Map &type_subscribers,
			     ACE_ES_Consumer_Rep *consumer,
			     RtecEventComm::EventType type,
			     RtecScheduler::Dependency_Info *&dependency);
  // Add <consumer> to the set of consumers bound to <type> in
  // <type_subscribers>.  If there is consumer set for <type>, the
  // operation fails.  Returns -1 on failure, 0 otherwise.

  static int remove (Subscriber_Map &type_map,
		     ACE_ES_Consumer_Rep *consumer,
		     RtecEventComm::EventType type);
  // Remove <consumer> from the consumer set in <type_map> set
  // corresponding to <type>.

  static int remove (SourceID_Map &source_subscribers,
		     ACE_ES_Consumer_Rep *consumer,
		     RtecEventComm::EventSourceID sid);
  // Remove <consumer> from the consumer set in the
  // <source_subscribers> set corresponding to <sid>.

  static void append_subscribers (Subscriber_Set &dest,
				  Subscriber_Set &src);
  // Insert all elements of <src> into <dest>.
};

// ************************************************************

// Forward declarations.
class ACE_ES_Dispatch_Request;
class ACE_Push_Consumer_Proxy;

class ACE_ES_Consumer_Correlation : public RtecEventComm_PushSupplierBOAImpl
// = TITLE
//    Event Service Consumer_Correlation
//
// = DESCRIPTION
//    There is one Consumer Correlation object per call to
//    connect_push_consumer.  It handles all the consumer's
//    correlation dependencies including timeouts.  This is also a
//    PushSupplier to support event forwarding.
{
public:
  ACE_ES_Consumer_Correlation (void);
  // Default construction.

  virtual ~ACE_ES_Consumer_Correlation (void);
  // Deletes lock_.

  int connected (ACE_Push_Consumer_Proxy *consumer, 
		 ACE_ES_Correlation_Module *correlation_module);
  // Initialization.  <correlation_module> is stored for delegating
  // channel operations.  <consumer> is stored to access the consumers
  // qos and filterin data.  Returns 0 on success, -1 on failure.

  int disconnecting (void);
  // Shutdown.

  ACE_ES_Dispatch_Request *push (ACE_ES_Consumer_Rep *consumer,
				 ACE_ES_Event_Container *event);
  // Takes <event> and adds it to the correlation.  Returns the
  // dispatch request that should be forwarded.

  void suspend (void);
  // Stop forwarding events to the calling consumer.

  void resume (void);
  // Resume forwarding events to the calling consumer.

  ACE_ES_Correlation_Module *correlation_module_;
  // Pointer back to the main correlation module.  This is public so
  // that ACE_ES_Consumer_Rep_Timeout::execute can access it.

  typedef ACE_CORBA_Sequence<ACE_ES_Event_Container_var> Event_Set;
  
private:
  virtual void disconnect_push_supplier (CORBA::Environment &);
  // Called when the channel disconnects us.

  int allocate_correlation_resources (ACE_ES_Dependency_Iterator &iter);
  // Dynamically allocates structures needed for correlations.  0 on
  // success, -1 on failure.

  ACE_ES_Dispatch_Request * correlate (ACE_ES_Consumer_Rep *cr, 
				       ACE_ES_Event_Container *event);
  // Helper function for this->push.
  
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

  RtecEventChannelAdmin::ProxyPushConsumer_ptr channel_;
  // For event forwarding.

  RtecEventChannelAdmin::SupplierQOS qos_;
  // Supplier QOS specifications.

  // Events waiting to be forwarded.
  Event_Set *pending_events_;

  // Used to synchronize pending_events_ and by the correlation module.
  ACE_ES_MUTEX lock_;
  // Used to lock shared state.

  ACE_Push_Consumer_Proxy *consumer_;

  u_long pending_flags_;
  // A bit is set for each dependency satisfied.

  ACE_ES_Consumer_Rep **consumer_reps_;
  // Array of consumer rep pointers.
  int n_consumer_reps_;
  ACE_ES_Consumer_Rep_Timeout *timer_reps_;
  int n_timer_reps_;

  ACE_ES_Conjunction_Group *conjunction_groups_;
  int n_conjunction_groups_;
  ACE_ES_Disjunction_Group *disjunction_groups_;
  int n_disjunction_groups_;

  int connected_;
  // True when we're connected to the channel for forwarding.
};

// ************************************************************

class ACE_ES_ACT
// = TITLE
//    Event Service ACT
//
// = DESCRIPTION
//    
{
public:
  ACE_ES_ACT (void);
  int has_act_;
  RtecEventComm::Event act_;
};

// ************************************************************

// Forward declarations.
class ACE_ES_Dispatch_Request;

class ACE_ES_Consumer_Module : public RtecEventChannelAdmin_ConsumerAdminBOAImpl
// = TITLE
//    Event Service Consumer Module
//
// = DESCRIPTION
//    ProxyPushSupplier factory.
{
public:
  ACE_ES_Consumer_Module (ACE_EventChannel *channel);
  // Default construction.

  void open (ACE_ES_Dispatching_Module *down);
  // Link to the next module.

  virtual RtecEventChannelAdmin::ProxyPushSupplier_ptr obtain_push_supplier (CORBA::Environment &);
  // Factory method for push consumer proxies.

  void connected (ACE_Push_Consumer_Proxy *consumer,
		  CORBA::Environment &);
  // Register the consumer with the Event Service.  This handles all
  // the details regarding Correlation_Module and Subscription_Module.

  void disconnecting (ACE_Push_Consumer_Proxy *consumer,
		      CORBA::Environment &);
  // Unregister the consumer from the Event Service.

  void push (const ACE_ES_Dispatch_Request *request,
	     CORBA::Environment &);

  RtecEventChannelAdmin::ConsumerAdmin_ptr get_ref (void);
  // Allow transformations to RtecEventChannelAdmin::ConsumerAdmin.

  void shutdown_request (ACE_ES_Dispatch_Request *request);
  // This is called by Shutdown_Consumer command objects when a
  // consumer proxy is ready to be deleted.

  void shutdown (void);
  // Actively disconnect from all consumers.

private:
  typedef ACE_Unbounded_Set_Iterator<ACE_Push_Consumer_Proxy *> Consumer_Iterator;
  typedef ACE_Unbounded_Set<ACE_Push_Consumer_Proxy *> Consumers;

  ACE_ES_MUTEX lock_;
  // Protects access to all_consumers_.

  Consumers all_consumers_;

  ACE_EventChannel *channel_;
  // Used to test for shutdown.

  ACE_ES_Dispatching_Module *down_;
  // Next module down.
};

// ************************************************************

// Forward declaration.
class ACE_ES_Subscription_Module;

class ACE_ES_Correlation_Module
// = TITLE
//    Event Service Correlation Module
//
// = DESCRIPTION
//    
{
public:
  ACE_ES_Correlation_Module (ACE_EventChannel *channel);
  // Default construction.

  void open (ACE_ES_Dispatching_Module *up,
	     ACE_ES_Subscription_Module *down);
  // Link to adjacent modules.

  void connected (ACE_Push_Consumer_Proxy *consumer,
		  CORBA::Environment &);
  // Create the consumers filter object.

  void disconnecting (ACE_Push_Consumer_Proxy *consumer,
		      CORBA::Environment &);
  // Release the consumers filter object.

  void push (ACE_ES_Consumer_Rep *consumer,
	     ACE_ES_Event_Container *event,
	     CORBA::Environment &);
  // Take in an event and its subscriber.  Apply consumer-specific
  // filters to each event and forward any dispatch requests to the
  // Dispatching Module.

  // = These are called by ACE_ES_Consumer_Reps.

  int subscribe (ACE_ES_Consumer_Rep *consumer);
  // Forwards to the subscription module.

  //int unsubscribe (ACE_ES_Consumer_Rep *consumer);
  // Forwards to the subscription module.

  int schedule_timeout (ACE_ES_Consumer_Rep_Timeout *consumer);
  // Schedule consumer timeout.  Return 0 on success, -1 on failure.

  int cancel_timeout (ACE_ES_Consumer_Rep_Timeout *consumer);
  // Cancel consumer timeout.  Return 0 on success, -1 on failure.

  int reschedule_timeout (ACE_ES_Consumer_Rep_Timeout *consumer);
  // Reschedule consumer timeout.  Return 0 on success, -1 on failure.

  ACE_EventChannel *channel_;
  // The master channel.  This is public so that Consumer_Correlation
  // objects can access it.

  void shutdown (void);
  // Does nothing.

private:
  ACE_ES_Dispatching_Module *up_;
  // Next module up.

  ACE_ES_Subscription_Module *subscription_module_;
  // Next module down.
};

// ************************************************************

// Forward declaration.
class ACE_ES_Supplier_Module;
class ACE_Push_Supplier_Proxy;

class ACE_ES_Subscription_Module
// = TITLE
//    Event Service Subscription Module
//
// = DESCRIPTION
// 
// = SYNCHRONIZATION
//    This is currently implemented with very coarse-grain
//    synchronization.  Basically, there is a single readers/writer
//    lock.  All operations acquire the writer lock to change any
//    subscription record.  All operations acquire a reader lock to
//    read any subscription record.  This is fine for normal
//    operations (which are *all* read operations).  However, the
//    initialization and shutdown periods might benefit from the
//    potential increase in concurrency if we used finer grain locks
//    (e.g., lock-per-source).
{
public:
  ACE_ES_Subscription_Module (ACE_EventChannel *channel);
  // Default construction.

  void open (ACE_ES_Correlation_Module *up,
	     ACE_ES_Supplier_Module *down);
  // Link to the adjacent modules.

  ~ACE_ES_Subscription_Module (void);
  // Deletes the lock_.

  int subscribe (ACE_ES_Consumer_Rep *consumer);
  // Register a new consumer.  Calls into <consumer> to figure out the
  // subscription options.  Returns 0 on success, -1 on failure.

  int unsubscribe (ACE_ES_Consumer_Rep *consumer);
  // Removes the -consumer- from any subscription lists.

  void connected (ACE_Push_Supplier_Proxy *supplier,
		  CORBA::Environment &);
  void disconnecting (ACE_Push_Supplier_Proxy *supplier,
		      CORBA::Environment &);

  void push (ACE_Push_Supplier_Proxy *source,
	     ACE_ES_Event_Container *event,
	     CORBA::Environment &);
  // Takes in a set of events and pushes subscriber sets to the
  // Correlation Module.

  // void push (ACE_Push_Supplier_Proxy *source,
  // const RtecEventComm::Event event);
  // This doesn't need one of these since it will never be called.

  void shutdown (void);
  // Unsubscribes all consumers from the suppliers.

private:
  void reregister_consumers (RtecEventComm::EventSourceID source_id);
  // Reregister any consumers that registered for <source_id> before
  // it actually connected to the channel.

  ACE_EventChannel *channel_;
  // The channel of all channels.

  /*
  typedef ACE_ES_Subscription_Info::Subscriber_Set INT;
  typedef ACE_Null_Mutex SYNCH;
  typedef ACE_Map_Manager<EXT, INT, SYNCH> Source_Collection;
  typedef ACE_Map_Iterator<EXT, INT, SYNCH> Source_Collection_Iterator;
  typedef ACE_Map_Entry<EXT, INT> Source_Collection_Entry;
  Source_Collection source_subscription_info_;
  // Source-only subscribers.
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

  int push_source (ACE_Push_Supplier_Proxy *source,
		    ACE_ES_Event_Container *event);
  // Push <event> to all consumers subscribed to all events from
  // <source>.  Returns 0 on success, -1 on failure.

  int push_source_type (ACE_Push_Supplier_Proxy *source,
			 ACE_ES_Event_Container *event);
  // Push <event> to all consumers subscribed to <event>.type_ from
  // <source>.  Returns 0 on success, -1 on failure.

  void push_all (ACE_ES_Event_Container *event,
		 CORBA::Environment &);
  // Push <event> to all_suppliers_.

  ACE_ES_Correlation_Module *up_;
  // Next module up stream.

  ACE_ES_Supplier_Module *down_;
  // Next module down stream.

  typedef ACE_Unbounded_Set_Iterator<ACE_Push_Supplier_Proxy *> Supplier_Iterator;
  typedef ACE_Unbounded_Set<ACE_Push_Supplier_Proxy *> Suppliers;

  Suppliers all_suppliers_;
  // All suppliers.

  ACE_ES_Subscription_Info::Subscriber_Map type_subscribers_;
  // Type-based subscribers.

  ACE_ES_Subscription_Info::SourceID_Map source_subscribers_;
  // Source-based subscribers.

  ACE_ES_RW_LOCK lock_;
  // Protects access to all_suppliers_ and type_suppliers_;
};

// ************************************************************

class ACE_ES_Supplier_Module : public RtecEventChannelAdmin_SupplierAdminBOAImpl
// = TITLE
//    Event Service Supplier Proxy Module
//
// = DESCRIPTION
//    ProxyPushConsumer factory.
{
public:
  ACE_ES_Supplier_Module (ACE_EventChannel *channel);
  // Default construction.

  void open (ACE_ES_Subscription_Module *up);
  // Associate the module to a channel.

  virtual RtecEventChannelAdmin::ProxyPushConsumer_ptr obtain_push_consumer (CORBA::Environment &);
  // Factory method for push supplier proxies.

  void push (ACE_Push_Supplier_Proxy *proxy,
	     const RtecEventComm::EventSet &event,
	     CORBA::Environment &);
  // The supplier module acts on behalf of the supplier proxy to
  // forward events through the channel.

  void connected (ACE_Push_Supplier_Proxy *supplier,
		  CORBA::Environment &);
  // Register the consumer with the Event Service.  This handles all
  // the details regarding Correlation_Module and Subscription_Module.

  void disconnecting (ACE_Push_Supplier_Proxy *supplier,
		      CORBA::Environment &);
  // Unregister the consumer from the Event Service.

  RtecEventChannelAdmin::SupplierAdmin_ptr get_ref (void);
  // Allow transformations to RtecEventComm::PushConsumer.

  void shutdown (void);
  // Actively disconnect from all suppliers.

private:
  typedef ACE_Unbounded_Set_Iterator<ACE_Push_Supplier_Proxy *> Supplier_Iterator;
  typedef ACE_Unbounded_Set<ACE_Push_Supplier_Proxy *> Suppliers;

  Suppliers all_suppliers_;
  // All suppliers.

  ACE_ES_MUTEX lock_;
  // Protects access to all_suppliers_ and type_suppliers_;

  ACE_ES_Subscription_Module *up_;

  ACE_EventChannel *channel_;
  // Used to test for shutdown.
};

// ************************************************************

// Forward declarations.
class ACE_EventChannel;

// = Event Channel interfaces.

class ACE_Push_Supplier_Proxy : public RtecEventChannelAdmin_ProxyPushConsumerBOAImpl
// = TITLE
//    Push Supplier Proxy.
//
// = DESCRIPTION
//    To the channel, this is a proxy to suppliers.  To suppliers, it
//    exports a PushConsumer interface.  It is a
//    RtecEventChannelAdmin::ProxyPushConsumer.  Suppliers use this
//    interface to connect to the channel, push events to consumers,
//    and to disconnect from the channel.
{
public:
  ACE_Push_Supplier_Proxy (ACE_ES_Supplier_Module *supplier_module);
  // Must be created with an owning supplier admin.

  // = Operations public to suppliers.

  virtual void connect_push_supplier (RtecEventComm::PushSupplier_ptr push_supplier,
				      const RtecEventChannelAdmin::SupplierQOS& qos,
				      CORBA::Environment &);
  // Suppliers connect via this interface.  <push_supplier> is a
  // reference to the supplier.  <qos> represents the publish types of
  // the supplier.

  virtual void push (const RtecEventComm::EventSet &event,
		     CORBA::Environment &);
  // Data arriving from a PushSupplier that must be sent to
  // consumers.  This is the entry point of all events.

  virtual void disconnect_push_consumer (CORBA::Environment &);
  // Disconnect the supplier from the channel.

  // = Operations for the Event Channel.

  RtecEventChannelAdmin::ProxyPushConsumer_ptr get_ref (void);
  // Allow transformations to RtecEventChannelAdmin::ProxyPushConsumer.

  int connected (void);
  // Returns 1 if the proxy has been connected to a "remote" client.

  void shutdown (void);
  // Actively disconnect from the supplier.

  // This is a hook so that the Subscription Module can associate
  // state with supplier proxies.
  ACE_ES_Subscription_Info &subscription_info (void);

  RtecEventChannelAdmin::SupplierQOS &qos (void);
  // Filtering criteria.

  int operator== (const RtecEventComm::EventSourceID rhs);
  // Is this object a proxy for -rhs-.  Simple pointer comparison for now.

  RtecEventComm::EventSourceID source_id (void);
  // Returns underlying supplier object ref.

private:
  RtecEventChannelAdmin::SupplierQOS qos_;
  // Reference to the supplier's qos params.

  ACE_ES_Subscription_Info subscription_info_;

  ACE_ES_Supplier_Module *supplier_module_;

  RtecEventComm::EventSourceID source_id_;
  // We keep a proxy of the Supplier source_id_;
  
  RtecEventComm::PushSupplier_ptr push_supplier_;
  // CORBA reference to remote push supplier.
};

// ************************************************************

class ACE_Push_Consumer_Proxy : public RtecEventChannelAdmin_ProxyPushSupplierBOAImpl
// = TITLE
//     Push Consumer Proxy.
//
// = DESCRIPTION
//     This is the channels proxy to a push consumer.  It implements
//     the RtecEventChannelAdmin::ProxyPushSupplier IDL interface.
//     Consumers use this interface to connect and disconnect from the
//     channel.
{
public:
  ACE_Push_Consumer_Proxy (ACE_ES_Consumer_Module *cm);
  // Must be created with an consumer admin.

  virtual ~ACE_Push_Consumer_Proxy (void);
  // Default destruction

  // = Interfaces exported to consumers.

  virtual void connect_push_consumer (RtecEventComm::PushConsumer_ptr push_consumer,
				      const RtecEventChannelAdmin::ConsumerQOS& qos,
				      CORBA::Environment &);
  // A push consumer is connecting.  <push_consumer> is a reference to
  // the consumer.  <qos> is the subscription types for the consumer.

  virtual void disconnect_push_supplier (CORBA::Environment &);
  // The consumer is disconnecting.

  virtual void suspend (CORBA::Environment &);
  // Stop forwarding events to the calling consumer.

  virtual void resume (CORBA::Environment &);
  // Resume forwarding events to the calling consumer.

  // = Event Channel operations.

  void push (const RtecEventComm::EventSet &events,
	     CORBA::Environment &);
  // Push <events> to push_consumer_.

  int connected (void);
  // Returns 1 if the proxy has been connected to a "remote" client.

  void shutdown (void);
  // Actively disconnect from the consumer.

  RtecEventChannelAdmin::ProxyPushSupplier_ptr get_ref (void);
  // Allow transformations to RtecEventChannelAdmin::ProxyPushSupplier.

  ACE_ES_Consumer_Correlation &correlation (void);
  // Access the consumer-specific Consumer_Correlation.

  RtecEventChannelAdmin::ConsumerQOS &qos (void);
  // Filtering criteria.

private:
  RtecEventChannelAdmin::ConsumerQOS qos_;
  // A reference to the consumers Quality of Service parameters.

  ACE_ES_Consumer_Correlation correlation_;
  // A hook so that the Correlation Module can associate correlation
  // information with the consumer.

  RtecEventComm::PushConsumer_ptr push_consumer_;
  // Reference to our push consumer.
  
  ACE_ES_Consumer_Module *consumer_module_;
  // TODO: Maybe this should be a _var or _duplicate/_release should
  // be used
};

// ************************************************************

// Helper function that returns
// qos.dependencies_[0].rt_info_->entry_point or "no-name".
const char *
ACE_ES_Consumer_Name (const RtecEventChannelAdmin::ConsumerQOS &qos);

// ************************************************************

typedef ACE_ES_Simple_Array <ACE_ES_Consumer_Rep *,
  ACE_ES_MAX_CONSUMERS_PER_SUPPLIER> ACE_ES_CRSet;

typedef ACE_ES_Array_Iterator <ACE_ES_Consumer_Rep *> ACE_ES_CRSet_Iterator;

#if defined (__ACE_INLINE__)
#include "Event_Channel.i"
#endif /* __ACE_INLINE__ */
#endif /* ACE_EVENT_CHANNEL_H */

