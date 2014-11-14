/* -*- C++ -*- */
/**
 *  @file Consumer.h
 *
 *  $Id$
 *
 *  @author Pradeep Gore <pradeep@oomworks.com>
 *
 *
 */

#ifndef TAO_NOTIFY_CONSUMER_H
#define TAO_NOTIFY_CONSUMER_H

#include /**/ "ace/pre.h"

#include "orbsvcs/Notify/notify_serv_export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "orbsvcs/CosNotifyCommC.h"
#include "orbsvcs/CosNotificationC.h"

#include "orbsvcs/Notify/Peer.h"
#include "orbsvcs/Notify/Event.h"
#include "orbsvcs/Notify/Timer.h"
#include "ace/Event_Handler.h"
#include "ace/Atomic_Op.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

class TAO_Notify_ProxySupplier;
class TAO_Notify_Proxy;
class TAO_Notify_Method_Request_Event_Queueable;
class TAO_Notify_Method_Request_Event;
/**
 * @class TAO_Notify_Consumer
 *
 * @brief Astract Base class for wrapping consumer objects that connect to the EventChannel
 *
 */
class TAO_Notify_Serv_Export TAO_Notify_Consumer
  : public TAO_Notify_Peer
  , public ACE_Event_Handler    // to support timer
{

public:
  /// Status returned from dispatch attempts
  enum DispatchStatus {
    DISPATCH_SUCCESS,
    DISPATCH_RETRY,   // retry this message
    DISPATCH_DISCARD, // discard this message
    DISPATCH_FAIL,    // discard all messages and disconnect consumer
    DISPATCH_FAIL_TIMEOUT // Same as DISPATCH_FAIL, but due to a timeout
  };

public:

typedef TAO_Notify_Refcountable_Guard_T< TAO_Notify_Consumer > Ptr;

  /// Constructor
  TAO_Notify_Consumer (TAO_Notify_ProxySupplier* proxy);

  /// Destructor
  virtual ~TAO_Notify_Consumer ();

  /// This method sigantures deliberately match the RefCounting methods required for ESF Proxy
  CORBA::ULong _incr_refcnt (void);
  CORBA::ULong _decr_refcnt (void);

  /// Access Specific Proxy.
  TAO_Notify_ProxySupplier* proxy_supplier (void);

  /// Access Base Proxy.
  virtual TAO_Notify_Proxy* proxy (void);

  /// Dispatch Event to consumer
  void deliver (TAO_Notify_Method_Request_Event * request);

  /// Push @a event to this consumer.
  virtual void push (const CORBA::Any& event) = 0;

  /// Push @a event to this consumer.
  virtual void push (const CosNotification::StructuredEvent& event) = 0;

  /// Push a batch of events to this consumer.
  virtual void push (const CosNotification::EventBatch& event) = 0;

  /// Dispatch the batch of events to the attached consumer
  DispatchStatus dispatch_batch (const CosNotification::EventBatch& batch);

  /// Dispatch the pending events
  void dispatch_pending (void);

  /// Is the connection suspended?
  CORBA::Boolean is_suspended (void);

  /// Suspend Connection
  void suspend (void);

  /// Resume Connection
  void resume (void);

  /// Shutdown the consumer
  virtual void shutdown (void);

  /// On reconnect we need to move events from the old consumer
  /// to the new one
  virtual void reconnect_from_consumer (TAO_Notify_Consumer* old_consumer) = 0;

  /// Override, Peer::qos_changed
  virtual void qos_changed (const TAO_Notify_QoSProperties& qos_properties);

  /// Take the pending queue from the rhs, cancel it's timer and
  /// schedule our timer.  The caller should have locked the proxy lock
  /// before calling this method.
  void assume_pending_events (TAO_Notify_Consumer& rhs);

  /// Is the connected consumer still around?
  bool is_alive (bool allow_nil_consumer);

  /// Estimate how many events are pending delivery for this consumer
  ///
  /// The estimate does not include events queued at the admin level which
  /// have not been passed to this consumer for delivery yet.
  size_t pending_count (void);

protected:

  /// This method is called by the is_alive() method.  It should provide
  /// the connected consumer or nil if there is none.
  virtual CORBA::Object_ptr get_consumer (void) = 0;

  typedef ACE_Unbounded_Queue<TAO_Notify_Method_Request_Event_Queueable *> Request_Queue;

  DispatchStatus dispatch_request (TAO_Notify_Method_Request_Event * request);

// FUZZ: disable check_for_ACE_Guard
  /**
   * \brief Attempt to dispatch event from a queue.
   *
   * Called by dispatch_pending.  Deliver one or more events to the Consumer.
   * If delivery fails, events are left in the queue (or discarded depending
   * on QoS parameters.)
   * Undelivered, undiscarded requests are left at the front of the queue.
   * Overridden in sequence consumer to dispatch as an EventBatch.
   * \return false if delivery failed and the request(s) cannot be discarded.
   */
  virtual bool dispatch_from_queue (
    Request_Queue & requests,
    ACE_Guard <TAO_SYNCH_MUTEX> & ace_mon);
// FUZZ: enable check_for_ACE_Guard

  void enqueue_request(TAO_Notify_Method_Request_Event * request);

  /// Add request to a queue if necessary.
  /// Overridden by sequence consumer to "always" put incoming events into the queue.
  /// @returns true the request has been enqueued; false the request should be handled now.
  virtual bool enqueue_if_necessary(
    TAO_Notify_Method_Request_Event * request);

  // Dispatch updates
  virtual void dispatch_updates_i (const CosNotification::EventTypeSeq& added,
                                   const CosNotification::EventTypeSeq& removed);

  /// Get the shared Proxy Lock
  TAO_SYNCH_MUTEX* proxy_lock (void);

protected:
  virtual int handle_timeout (const ACE_Time_Value& current_time,
                              const void* act = 0);


  /// Schedule timer
  void schedule_timer (bool is_error = false);

  /// Cancel timer
  void cancel_timer (void);

  ///= Protected Data Members
protected:
  Request_Queue& pending_events();

  /// The Proxy that we associate with.
  TAO_Notify_ProxySupplier* proxy_;

  /// Suspended Flag.
  CORBA::Boolean is_suspended_;

  /// Interface that accepts offer_changes
  CosNotifyComm::NotifyPublish_var publish_;
  bool have_not_yet_verified_publish_;

  /// The Pacing Interval
  const TAO_Notify_Property_Time & pacing_;

  /// Max. batch size.
  TAO_Notify_Property_Long max_batch_size_;

  /// Timer Id.
  long timer_id_;

  /// The Timer Manager that we use.
  TAO_Notify_Timer::Ptr timer_;

  /// Last time either push an event or validate connection
  /// via _non_exist call.
  ACE_Atomic_Op<TAO_SYNCH_MUTEX, ACE_Time_Value> last_ping_;

private:

  /// Events pending to be delivered.
  ACE_Auto_Ptr< Request_Queue > pending_events_;

  CORBA::Object_var rtt_obj_;
};

TAO_END_VERSIONED_NAMESPACE_DECL

#if defined (__ACE_INLINE__)
#include "orbsvcs/Notify/Consumer.inl"
#endif /* __ACE_INLINE__ */

#include /**/ "ace/post.h"

#endif /* TAO_NOTIFY_CONSUMER_H */
