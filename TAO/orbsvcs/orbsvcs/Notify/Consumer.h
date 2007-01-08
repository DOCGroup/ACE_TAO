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
    DISPATCH_FAIL};   // discard all messages and disconnect consumer

public:
  /// Constuctor
  TAO_Notify_Consumer (TAO_Notify_ProxySupplier* proxy);

  /// Destructor
  virtual ~TAO_Notify_Consumer ();

  /// Access Specific Proxy.
  TAO_Notify_ProxySupplier* proxy_supplier (void);

  /// Access Base Proxy.
  virtual TAO_Notify_Proxy* proxy (void);

  /// Dispatch Event to consumer
  void deliver (TAO_Notify_Method_Request_Event * request ACE_ENV_ARG_DECL);

  /// Push <event> to this consumer.
  virtual void push (const CORBA::Any& event ACE_ENV_ARG_DECL) = 0;

  /// Push <event> to this consumer.
  virtual void push (const CosNotification::StructuredEvent& event ACE_ENV_ARG_DECL) = 0;

  /// Push a batch of events to this consumer.
  virtual void push (const CosNotification::EventBatch& event ACE_ENV_ARG_DECL) = 0;

  /// Dispatch the batch of events to the attached consumer
  DispatchStatus dispatch_batch (const CosNotification::EventBatch& batch);

  /// Dispatch the pending events
  void dispatch_pending (ACE_ENV_SINGLE_ARG_DECL);

  /// Is the connection suspended?
  CORBA::Boolean is_suspended (void);

  /// Suspend Connection
  void suspend (ACE_ENV_SINGLE_ARG_DECL);

  /// Resume Connection
  void resume (ACE_ENV_SINGLE_ARG_DECL);

  /// Shutdown the consumer
  virtual void shutdown (ACE_ENV_SINGLE_ARG_DECL);

  /// on reconnect we need to move events from the old consumer
  /// to the new one
  virtual void reconnect_from_consumer (
    TAO_Notify_Consumer* old_consumer
    ACE_ENV_ARG_DECL) = 0;

  /// Override, Peer::qos_changed
  virtual void qos_changed (const TAO_Notify_QoSProperties& qos_properties);

protected:
  typedef ACE_Unbounded_Queue<TAO_Notify_Method_Request_Event_Queueable *> Request_Queue;

  DispatchStatus dispatch_request (TAO_Notify_Method_Request_Event * request);

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

  void enqueue_request(TAO_Notify_Method_Request_Event * request ACE_ENV_ARG_DECL);

  /// Add request to a queue if necessary.
  /// Overridden by sequence consumer to "always" put incoming events into the queue.
  /// @returns true the request has been enqueued; false the request should be handled now.
  virtual bool enqueue_if_necessary(
    TAO_Notify_Method_Request_Event * request
    ACE_ENV_ARG_DECL);

  // Dispatch updates
  virtual void dispatch_updates_i (const CosNotification::EventTypeSeq& added,
                                   const CosNotification::EventTypeSeq& removed
                                   ACE_ENV_ARG_DECL);

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

  /// The Pacing Interval
  const TAO_Notify_Property_Time & pacing_;

  /// Max. batch size.
  TAO_Notify_Property_Long max_batch_size_;

  /// Timer Id.
  long timer_id_;

  /// The Timer Manager that we use.
  TAO_Notify_Timer::Ptr timer_;

private:

  /// Events pending to be delivered.
  ACE_Auto_Ptr< Request_Queue > pending_events_;
};

TAO_END_VERSIONED_NAMESPACE_DECL

#if defined (__ACE_INLINE__)
#include "orbsvcs/Notify/Consumer.inl"
#endif /* __ACE_INLINE__ */

#include /**/ "ace/post.h"

#endif /* TAO_NOTIFY_CONSUMER_H */
