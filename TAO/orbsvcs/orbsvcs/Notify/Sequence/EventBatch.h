/* -*- C++ -*- */
/**
 *  @file EventBatch.h
 *
 *  $Id$
 *
 *  @author Pradeep Gore <pradeep@oomworks.com>
 *
 *
 */

#ifndef TAO_NS_EVENTBATCH_H
#define TAO_NS_EVENTBATCH_H
#include "ace/pre.h"

#include "../notify_export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "ace/Unbounded_Queue.h"
#include "orbsvcs/CosNotificationC.h"
#include "../Event.h"
#include "../Types.h"

/**
 * @class TAO_NS_EventBatch
 *
 * @brief
 *
 */
class TAO_Notify_Export TAO_NS_EventBatch
{
public:
  /// Constructor
  TAO_NS_EventBatch (void);

  /// Destructor.
  ~TAO_NS_EventBatch (void);

  /// Set the Batch size.
  void batch_size (CORBA::Long batch_size);

  /// Insert
  void insert (const TAO_NS_Event_var& event);

  /// Insert
  void insert (const TAO_NS_Event_Collection& event_collection);

  /// Populate the event_batch with max size <batch_size_>
  /// The first <batch_size_> number of events are removed.
  void extract (TAO_NS_Event_Collection& event_collection);

  /// Populate <event_batch> with <event_collection>
  static void populate (const TAO_NS_Event_Collection& event_collection, CosNotification::EventBatch& event_batch);

protected:
  // Event Batch
  TAO_NS_Event_Collection event_collection_;

  /// Max. batch size.
  CORBA::Long batch_size_;
};

#if defined (__ACE_INLINE__)
#include "EventBatch.inl"
#endif /* __ACE_INLINE__ */

#include "ace/post.h"
#endif /* TAO_NS_EVENTBATCH_H */
