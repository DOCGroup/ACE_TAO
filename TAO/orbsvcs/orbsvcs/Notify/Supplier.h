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

#ifndef TAO_NS_SUPPLIER_H
#define TAO_NS_SUPPLIER_H
#include "ace/pre.h"

#include "notify_export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "Peer.h"

class TAO_NS_ProxyConsumer;

/**
 * @class TAO_NS_Supplier
 *
 * @brief Base Wrappers for Suppliers that connect to the EventChannel.
 *
 */
class TAO_Notify_Export TAO_NS_Supplier : public TAO_NS_Peer
{
public:
  /// Constuctor
  TAO_NS_Supplier (TAO_NS_ProxyConsumer* proxy);

  /// Destructor
  ~TAO_NS_Supplier ();

  /// Access Specific Proxy.
  TAO_NS_ProxyConsumer* proxy_consumer (void);

  /// Access Base Proxy.
  virtual TAO_NS_Proxy* proxy (void);

protected:
  /// The proxy that we associate with.
  TAO_NS_ProxyConsumer* proxy_;
};

#if defined (__ACE_INLINE__)
#include "Supplier.inl"
#endif /* __ACE_INLINE__ */

#include "ace/post.h"
#endif /* TAO_NS_SUPPLIER_H */
