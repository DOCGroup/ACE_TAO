/* -*- C++ -*- */
/**
 *  @file StructuredPushConsumer.h
 *
 *  $Id$
 *
 *  @author Pradeep Gore <pradeep@oomworks.com>
 *
 *
 */

#ifndef TAO_Notify_STRUCTUREDPUSHCONSUMER_H
#define TAO_Notify_STRUCTUREDPUSHCONSUMER_H
#include /**/ "ace/pre.h"

#include "orbsvcs/Notify/notify_serv_export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "orbsvcs/CosNotifyCommC.h"
#include "orbsvcs/Notify/Consumer.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

class TAO_Notify_ProxySupplier;

/**
 * @class TAO_Notify_StructuredPushConsumer
 *
 * @brief Wrapper for the StructuredPushConsumer that connect to the EventChannel.
 *
 */
class TAO_Notify_Serv_Export TAO_Notify_StructuredPushConsumer : public TAO_Notify_Consumer
{
public:
  /// Constructor
  TAO_Notify_StructuredPushConsumer (TAO_Notify_ProxySupplier* proxy);

  /// Init the Consumer
  void init (CosNotifyComm::StructuredPushConsumer_ptr push_consumer);

  /// Destructor
  virtual ~TAO_Notify_StructuredPushConsumer ();

  /// Push <event> to this consumer.
//  virtual void push_i (const TAO_Notify_Event* event);

  /// Push <event> to this consumer.
  virtual void push (const CORBA::Any& event);

  /// Push <event> to this consumer.
  virtual void push (const CosNotification::StructuredEvent& event);

  /// Push a batch of events to this consumer.
  virtual void push (const CosNotification::EventBatch& event);

  /// Retrieve the ior of this peer
  virtual ACE_CString get_ior (void) const;

  /// on reconnect we need to move events from the old consumer
  /// to the new one
  virtual void reconnect_from_consumer (
    TAO_Notify_Consumer* old_consumer);


protected:
  /// The Consumer
  CosNotifyComm::StructuredPushConsumer_var push_consumer_;

private:
  /// Release
  virtual void release (void);
};

TAO_END_VERSIONED_NAMESPACE_DECL

#include /**/ "ace/post.h"
#endif /* TAO_Notify_STRUCTUREDPUSHCONSUMER_H */
