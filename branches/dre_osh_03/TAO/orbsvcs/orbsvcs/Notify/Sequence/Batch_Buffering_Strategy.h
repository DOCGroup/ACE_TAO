/* -*- C++ -*- */
/**
 *  @file Batch_Buffering_Strategy.h
 *
 *  $Id$
 *
 *  @author Pradeep Gore <pradeep@oomworks.com>
 *
 *
 */

#ifndef TAO_NS_BATCH_BUFFERING_STRATEGY_H
#define TAO_NS_BATCH_BUFFERING_STRATEGY_H
#include "ace/pre.h"

#include "../notify_export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "../Method_Request.h"
#include "../Buffering_Strategy.h"

/**
 * @class TAO_NS_Batch_Buffering_Strategy
 *
 * @brief
 *
 */
class TAO_Notify_Export TAO_NS_Batch_Buffering_Strategy : public TAO_NS_Buffering_Strategy
{
public:
  /// Constuctor
  TAO_NS_Batch_Buffering_Strategy (TAO_NS_Message_Queue& msg_queue, TAO_NS_AdminProperties_var& admin_properties, CORBA::Long batch_size);

  /// Destructor
  ~TAO_NS_Batch_Buffering_Strategy ();

  /// Dequeue batch. This method will block till <batch_size> is available..
  /// Return -1 on error else the number of items actually dequeued.
  int dequeue_batch (CosNotification::EventBatch& event_batch);

  /// Dequeue upto batch. This method will not block.
  /// Return -1 on error else the number of items dequeued (<batch_size>).
  /// <pending> is set to the number of events remaining in the queue.
  int dequeue_available (CosNotification::EventBatch& event_batch, int &pending);

protected:

  /// Extract upto <max_deq_count> number of items.
  int dequeue_i (int max_deq_count, CosNotification::EventBatch& event_batch);
};

#if defined (__ACE_INLINE__)
#include "Batch_Buffering_Strategy.inl"
#endif /* __ACE_INLINE__ */

#include "ace/post.h"
#endif /* TAO_NS_BATCH_BUFFERING_STRATEGY_H */
