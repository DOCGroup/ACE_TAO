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

#ifndef TAO_NS_STRUCTUREDPUSHCONSUMER_H
#define TAO_NS_STRUCTUREDPUSHCONSUMER_H
#include "ace/pre.h"

#include "../notify_export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "orbsvcs/CosNotifyCommC.h"
#include "../Consumer.h"
#include "../Destroy_Callback.h"

/**
 * @class TAO_NS_StructuredPushConsumer
 *
 * @brief Wrapper for the StructuredPushConsumer that connect to the EventChannel.
 *
 */
class TAO_Notify_Export TAO_NS_StructuredPushConsumer : public TAO_NS_Consumer, public TAO_NS_Destroy_Callback
{
public:
  /// Constuctor
  TAO_NS_StructuredPushConsumer (CosNotifyComm::StructuredPushConsumer_ptr push_consumer);

  /// Destructor
  ~TAO_NS_StructuredPushConsumer ();  

  /// TAO_NS_Destroy_Callback methods.
  virtual void release (void);

  virtual void push (const CosNotification::StructuredEvent & event ACE_ENV_ARG_DECL);
 
protected:
  /// The Consumer 
  CosNotifyComm::StructuredPushConsumer_var push_consumer_;
};

#if defined (__ACE_INLINE__)
#include "StructuredPushConsumer.inl"
#endif /* __ACE_INLINE__ */

#include "ace/post.h"
#endif /* TAO_NS_STRUCTUREDPUSHCONSUMER_H */
