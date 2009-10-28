/* -*- C++ -*- */
/**
 *  @file Buffering_Strategy.h
 *
 *  $Id$
 *
 *  @author Pradeep Gore <pradeep@oomworks.com>
 *
 *
 */

#ifndef TAO_Notify_BUFFERING_STRATEGY_H
#define TAO_Notify_BUFFERING_STRATEGY_H

#include /**/ "ace/pre.h"

#include "orbsvcs/Notify/notify_serv_export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "ace/Null_Condition.h"
#include "ace/Message_Queue.h"

#include "orbsvcs/TimeBaseC.h"

#include "orbsvcs/Notify/Property.h"
#include "orbsvcs/Notify/Property_T.h"
#include "orbsvcs/Notify/AdminProperties.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

class TAO_Notify_Method_Request_Queueable;
class TAO_Notify_QoSProperties;

typedef ACE_Message_Queue<ACE_NULL_SYNCH> TAO_Notify_Message_Queue;

/**
 * @class TAO_Notify_Buffering_Strategy
 *
 * @brief Base Strategy to enqueue and dequeue items from a Message Queue.
 *
 */
class TAO_Notify_Serv_Export TAO_Notify_Buffering_Strategy
{
public:
  TAO_Notify_Buffering_Strategy (
    TAO_Notify_Message_Queue& msg_queue,
    const TAO_Notify_AdminProperties::Ptr& admin_properties);

  ~TAO_Notify_Buffering_Strategy ();

  /// Update state with the following QoS Properties:
  /// Order Policy
  /// Discard Policy
  /// MaxEventsPerConsumer
  void update_qos_properties (const TAO_Notify_QoSProperties& qos_properties);

  /// Enqueue according the enqueing strategy.
  /// Return -1 on error else the number of items in the queue.
  int enqueue (TAO_Notify_Method_Request_Queueable* method_request);

  /// Dequeue batch. This method will block for @a abstime if non-zero or else blocks till an item is available.
  /// Return -1 on error or if nothing is available, else the number of items actually dequeued (1).
  int dequeue (TAO_Notify_Method_Request_Queueable* &method_request,
               const ACE_Time_Value *abstime);

  /// Shutdown
  void shutdown (void);

  /// Provide the time value of the oldest event in the queue.
  ACE_Time_Value oldest_event (void);

  /// This interface allows tracking of the queue size
  class TAO_Notify_Serv_Export Tracker
  {
  public:
    Tracker (void);
    virtual ~Tracker (void);
    virtual void update_queue_count (size_t count) = 0;
    virtual void count_queue_overflow (bool local_overflow, bool global_overflow) = 0;
    void register_child (Tracker * child);
    void unregister_child (Tracker * child);
  protected:
    Tracker * child_;
  };

  /// Set the tracker object.  This strategy does not own the tracker.
  void set_tracker (Tracker* tracker);

private:

  /// Apply the Order Policy and queue. return -1 on error.
  int queue (TAO_Notify_Method_Request_Queueable* method_request);

  /// Discard as per the Discard Policy.
  bool discard (TAO_Notify_Method_Request_Queueable* method_request);

  ///= Data Members

  /// The local Message Queue
  TAO_Notify_Message_Queue& msg_queue_;

  /// Reference to the properties per event channel.
  TAO_Notify_AdminProperties::Ptr admin_properties_;

  /// The shared global lock used by all the queues.
  TAO_SYNCH_MUTEX& global_queue_lock_;

  /// The global queue length - queue length accross all the queues.
  CORBA::Long& global_queue_length_;

  /// The maximum events that can be queued overall.
  const TAO_Notify_Property_Long& max_queue_length_;

  /// Order of events in internal buffers.
  TAO_Notify_Property_Short order_policy_;

  /// Policy to discard when buffers are full.
  TAO_Notify_Property_Short discard_policy_;

  TAO_Notify_Property_Long max_events_per_consumer_;
  TAO_Notify_Property_Time blocking_policy_;

  TAO_SYNCH_CONDITION& global_not_full_;
  TAO_SYNCH_CONDITION local_not_full_;

  /// Condition that batch size reached.
  TAO_SYNCH_CONDITION local_not_empty_;

  /// Flag to shutdown.
  bool shutdown_;

  /// Optional queue tracker
  Tracker* tracker_;
};

TAO_END_VERSIONED_NAMESPACE_DECL

#include /**/ "ace/post.h"

#endif /* TAO_Notify_BUFFERING_STRATEGY_H */
