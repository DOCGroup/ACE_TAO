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

#ifndef TAO_NS_BUFFERING_STRATEGY_H
#define TAO_NS_BUFFERING_STRATEGY_H
#include "ace/pre.h"

#include "notify_export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */


#include "ace/Message_Queue.h"
#include "orbsvcs/TimeBaseC.h"
#include "Property.h"
#include "Property_T.h"
#include "AdminProperties.h"

class TAO_NS_Method_Request;
class TAO_NS_QoSProperties;

typedef ACE_Message_Queue<ACE_NULL_SYNCH> TAO_NS_Message_Queue;

/**
 * @class TAO_NS_Buffering_Strategy
 *
 * @brief Base Strategy to enqueue and dequeue items from a Message Queue.
 *
 */
class TAO_Notify_Export TAO_NS_Buffering_Strategy
{
public:
  /// Constuctor
  TAO_NS_Buffering_Strategy (TAO_NS_Message_Queue& msg_queue, TAO_NS_AdminProperties_var& admin_properties, CORBA::Long batch_size);

  /// Destructor
  ~TAO_NS_Buffering_Strategy ();

  /// Update state with the following QoS Properties:
  /// Order Policy
  /// Discard Policy
  /// MaxEventsPerConsumer
  /// TAO_Notify_Extensions::BlockingPolicy
  void update_qos_properties (const TAO_NS_QoSProperties& qos_properties);

  /// Enqueue according the enqueing strategy.
  /// Return -1 on error else the number of items in the queue.
  int enqueue (TAO_NS_Method_Request& method_request);

  /// Dequeue batch. This method will block for <abstime> if non-zero or else blocks till an item is available.
  /// Return -1 on error or if nothing is available, else the number of items actually dequeued (1).
  int dequeue (TAO_NS_Method_Request* &method_request, const ACE_Time_Value *abstime);

  /// Shutdown
  void shutdown (void);

  /// Set the new batch size.
  void batch_size (CORBA::Long batch_size);

  /// Obtain our batch size
  CORBA::Long batch_size (void);

  /// Set the max local queue length.
  void max_local_queue_length (CORBA::Long length);

protected:
  /// Apply the Order Policy and queue. return -1 on error.
  int queue (TAO_NS_Method_Request& method_request);

  /// Discard as per the Discard Policy.
  int discard (void);

  ///= Data Members

  /// The local Message Queue
  TAO_NS_Message_Queue& msg_queue_;

  /// Reference to the properties per event channel.
  TAO_NS_AdminProperties_var admin_properties_;

  /// The shared global lock used by all the queues.
  ACE_SYNCH_MUTEX& global_queue_lock_;

  /// The shared Condition for global queue not full.
  ACE_SYNCH_CONDITION& global_queue_not_full_condition_;

  /// The global queue length - queue length accross all the queues.
  CORBA::Long& global_queue_length_;

  /// The maximum events that can be queued overall.
  const TAO_NS_Property_Long& max_global_queue_length_;

  /// The maximum queue length for the local queue.
  CORBA::Long max_local_queue_length_;

  /// Order of events in internal buffers.
  TAO_NS_Property_Short order_policy_;

  /// Policy to discard when buffers are full.
  TAO_NS_Property_Short discard_policy_;

  /// Flag that we should use discarding(1) or blocking (0).
  int use_discarding_;

  /// The blocking timeout will be used in place of discarding
  /// This is a TAO specific extension.
  ACE_Time_Value blocking_time_; // 0 means wait forever.

  /// Condition that the local queue is not full.
  ACE_SYNCH_CONDITION local_queue_not_full_condition_;

  /// The batch size that we want to monitor for dequeuing.
  CORBA::Long batch_size_;

  /// Condition that batch size reached.
  ACE_SYNCH_CONDITION batch_size_reached_condition_;

  /// Flag to shutdown.
  int shutdown_;
};

#if defined (__ACE_INLINE__)
#include "Buffering_Strategy.inl"
#endif /* __ACE_INLINE__ */

#include "ace/post.h"
#endif /* TAO_NS_BUFFERING_STRATEGY_H */
