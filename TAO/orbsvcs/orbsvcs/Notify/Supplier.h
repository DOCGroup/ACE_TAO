/* -*- C++ -*- */
/**
 *  @file Supplier.h
 *
 *  $Id$
 *
 *  @author Pradeep Gore <pradeep@oomworks.com>
 *
 *
 */

#ifndef TAO_Notify_SUPPLIER_H
#define TAO_Notify_SUPPLIER_H
#include /**/ "ace/pre.h"

#include "notify_export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "Peer.h"

#include "orbsvcs/CosNotifyCommC.h"
class TAO_Notify_ProxyConsumer;

/**
 * @class TAO_Notify_Supplier
 *
 * @brief Base Wrappers for Suppliers that connect to the EventChannel.
 *
 */
class TAO_Notify_Export TAO_Notify_Supplier : public TAO_Notify_Peer
{
public:
  /// Constuctor
  TAO_Notify_Supplier (TAO_Notify_ProxyConsumer* proxy);

  /// Destructor
  ~TAO_Notify_Supplier ();

  /// Access Specific Proxy.
  TAO_Notify_ProxyConsumer* proxy_consumer (void);

  /// Access Base Proxy.
  virtual TAO_Notify_Proxy* proxy (void);

protected:
  /// Dispatch updates implementation.
  virtual void dispatch_updates_i (const CosNotification::EventTypeSeq& added,
                                   const CosNotification::EventTypeSeq& removed
                                   ACE_ENV_ARG_DECL);

  /// The proxy that we associate with.
  TAO_Notify_ProxyConsumer* proxy_;

  /// Interface that accepts subscription_changes
  CosNotifyComm::NotifySubscribe_var subscribe_;
};

#if defined (__ACE_INLINE__)
#include "Supplier.inl"
#endif /* __ACE_INLINE__ */

#include /**/ "ace/post.h"
#endif /* TAO_Notify_SUPPLIER_H */
