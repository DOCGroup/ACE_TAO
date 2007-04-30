// -*- C++ -*-

/**
 *  @file Supplier.h
 *
 *  $Id$
 *
 *  @author Pradeep Gore <pradeep@oomworks.com>
 */

#ifndef TAO_Notify_SUPPLIER_H
#define TAO_Notify_SUPPLIER_H
#include /**/ "ace/pre.h"

#include "orbsvcs/Notify/notify_serv_export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "orbsvcs/Notify/Peer.h"

#include "orbsvcs/CosNotifyCommC.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

class TAO_Notify_ProxyConsumer;

/**
 * @class TAO_Notify_Supplier
 *
 * @brief Base Wrappers for Suppliers that connect to the EventChannel.
 *
 */
class TAO_Notify_Serv_Export TAO_Notify_Supplier : public TAO_Notify_Peer
{
public:
  /// Constuctor
  TAO_Notify_Supplier (TAO_Notify_ProxyConsumer* proxy);

  /// Destructor
  virtual ~TAO_Notify_Supplier ();

  /// Access Specific Proxy.
  TAO_Notify_ProxyConsumer* proxy_consumer (void);

  /// Access Base Proxy.
  virtual TAO_Notify_Proxy* proxy (void);

protected:
  /// Dispatch updates implementation.
  virtual void dispatch_updates_i (const CosNotification::EventTypeSeq& added,
                                   const CosNotification::EventTypeSeq& removed);

  /// The proxy that we associate with.
  TAO_Notify_ProxyConsumer* proxy_;

  /// Interface that accepts subscription_changes
  CosNotifyComm::NotifySubscribe_var subscribe_;
};

TAO_END_VERSIONED_NAMESPACE_DECL

#if defined (__ACE_INLINE__)
#include "orbsvcs/Notify/Supplier.inl"
#endif /* __ACE_INLINE__ */

#include /**/ "ace/post.h"
#endif /* TAO_Notify_SUPPLIER_H */
