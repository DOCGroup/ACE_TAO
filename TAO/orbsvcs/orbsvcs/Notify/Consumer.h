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

#include "ace/Unbounded_Queue.h"
#include "Peer.h"
#include "Types.h"

/**
 * @class TAO_NS_Consumer
 *
 * @brief Astract Base class for wrapping consumer objects that connect to the EventChannel
 *
 */
class TAO_Notify_Export TAO_NS_Consumer : public TAO_NS_Peer
{
public:
  /// Constuctor
  TAO_NS_Consumer (TAO_NS_ProxySupplier* proxy);

  /// Destructor
  virtual ~TAO_NS_Consumer ();

  /// Access Specific Proxy.
  TAO_NS_ProxySupplier* proxy_supplier (void);

  /// Access Base Proxy.
  virtual TAO_NS_Proxy* proxy (void);

  /// Push <event> to this consumer.
  void push (const TAO_NS_Event_var& event ACE_ENV_ARG_DECL);

  /// Push <event> to this consumer.
  virtual void push (const CosNotification::StructuredEvent & event ACE_ENV_ARG_DECL) = 0;

  /// Set Observer.
  void event_dispatch_observer (TAO_NS_Event_Dispatch_Observer* event_dispatch_observer);

  /// Dispatch the pending events
  void dispatch_pending (void);

protected:

  /// Push Implementation.
  virtual void push_i (const TAO_NS_Event_var& event ACE_ENV_ARG_DECL) = 0;

  /// The Proxy that we associate with.
  TAO_NS_ProxySupplier* proxy_;

  /// Observer
  TAO_NS_Event_Dispatch_Observer* event_dispatch_observer_;

  /// Events pending to be delivered.
  TAO_NS_Event_Collection event_collection_;
};

#if defined (__ACE_INLINE__)
#include "Consumer.inl"
#endif /* __ACE_INLINE__ */

#include "ace/post.h"
#endif /* TAO_NS_CONSUMER_H */
