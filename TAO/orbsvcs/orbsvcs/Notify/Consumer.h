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

#ifndef TAO_NS_CONSUMER_H
#define TAO_NS_CONSUMER_H
#include "ace/pre.h"

#include "notify_export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "orbsvcs/CosNotificationC.h"
#include "Refcountable.h"
#include "Types.h"

/**
 * @class TAO_NS_Consumer
 *
 * @brief Astract Base class for wrapping consumer objects that connect to the EventChannel
 *
 */
class TAO_Notify_Export TAO_NS_Consumer : public TAO_NS_Refcountable
{
public:
  /// Constuctor
  TAO_NS_Consumer (void);

  /// Destructor
  virtual ~TAO_NS_Consumer (); 

  /// Push <event> to this consumer.
  virtual void push (const CosNotification::StructuredEvent & event ACE_ENV_ARG_DECL) = 0; 
};

#if defined (__ACE_INLINE__)
#include "Consumer.inl"
#endif /* __ACE_INLINE__ */

#include "ace/post.h"
#endif /* TAO_NS_CONSUMER_H */
