/* -*- C++ -*- */
/**
 *  @file PushConsumer.h
 *
 *  $Id$
 *
 *  @author Pradeep Gore <pradeep@oomworks.com>
 *
 *
 */

#ifndef TAO_Notify_PUSHCONSUMER_H
#define TAO_Notify_PUSHCONSUMER_H
#include /**/ "ace/pre.h"

#include "orbsvcs/Notify/notify_serv_export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "orbsvcs/CosNotifyCommC.h"
#include "orbsvcs/Notify/Consumer.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

/**
 * @class TAO_Notify_PushConsumer
 *
 * @brief Wrapper for the PushConsumer that connect to the EventChannel.
 *
 */
class TAO_Notify_Serv_Export TAO_Notify_PushConsumer : public TAO_Notify_Consumer
{
public:
  /// Constructor
  TAO_Notify_PushConsumer (TAO_Notify_ProxySupplier* proxy);

  /// Destructor
  virtual ~TAO_Notify_PushConsumer ();

  /// Init
  void init (CosEventComm::PushConsumer_ptr push_consumer);

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
  CosEventComm::PushConsumer_var push_consumer_;

private:
  /// TAO_Notify_Destroy_Callback methods.
  virtual void release (void);
};

TAO_END_VERSIONED_NAMESPACE_DECL

#include /**/ "ace/post.h"
#endif /* TAO_Notify_PUSHCONSUMER_H */
