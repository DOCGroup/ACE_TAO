/* -*- C++ -*- */
/**
 *  @file Method_Request_Dispatch_EventBatch.h
 *
 *  $Id$
 *
 *  @author Pradeep Gore <pradeep@oomworks.com>
 *
 *
 */

#ifndef TAO_NS_METHOD_REQUEST_DISPATCH_EVENTBATCH_H
#define TAO_NS_METHOD_REQUEST_DISPATCH_EVENTBATCH_H
#include "ace/pre.h"

#include "../notify_export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "ace/Unbounded_Queue.h"
#include "orbsvcs/CosNotificationC.h"
#include "../Method_Request.h"
#include "../Types.h"
#include "../Event.h"

class TAO_NS_SequencePushConsumer;

/**
 * @class TAO_NS_Method_Request_Dispatch_EventBatch
 *
 * @brief
 *
 */
class TAO_Notify_Export TAO_NS_Method_Request_Dispatch_EventBatch : public TAO_NS_Method_Request
{
public:
  /// Constuctor
  TAO_NS_Method_Request_Dispatch_EventBatch (TAO_NS_Event_Collection& event_collection, TAO_NS_SequencePushConsumer* consumer);

  /// Destructor
  ~TAO_NS_Method_Request_Dispatch_EventBatch ();

  /// Execute the Request
  int execute (ACE_ENV_SINGLE_ARG_DECL);

  /// Create a copy of this object.
  TAO_NS_Method_Request* copy (void);

protected:
  /// The consumer to push to.
  TAO_NS_SequencePushConsumer* consumer_;

  /// Event Collection
  TAO_NS_Event_Collection event_collection_;
};

#if defined (__ACE_INLINE__)
#include "Method_Request_Dispatch_EventBatch.inl"
#endif /* __ACE_INLINE__ */

#include "ace/post.h"
#endif /* TAO_NS_METHOD_REQUEST_DISPATCH_EVENTBATCH_H */
