/* -*- C++ -*- */
/**
 *  @file Delivery_Method_Dispatch.h
 *
 *  $Id$
 *
 *  @author Pradeep Gore <pradeep@oomworks.com>
 *
 *
 */

#ifndef TAO_NOTIFY_DELIVERY_METHOD_DISPATCH_H
#define TAO_NOTIFY_DELIVERY_METHOD_DISPATCH_H
#include "ace/pre.h"

#include "notify_export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "Delivery_Method.h"
#include "Types.h"

namespace TAO_NOTIFY
{
/**
 * @class Delivery_Method_Dispatch
 *
 * @brief Dispatchs an event to a proxy supplier.
 *
 */
class TAO_Notify_Export Delivery_Method_Dispatch  : public Delivery_Method
{
public:
  /// an arbitrary code (Octet) to identify this delivery method type in persistent storage
  enum {persistence_code = 1};
  /// Constructor
  Delivery_Method_Dispatch (
    const Delivery_Request_Ptr& delivery_request,
    TAO_Notify_ProxySupplier* proxy_supplier,
    bool filtering = true);

  Delivery_Method_Dispatch (const Delivery_Method_Dispatch & rhs);

  static Delivery_Method_Dispatch * create (
    const Delivery_Request_Ptr& delivery_request,
    TAO_Notify_EventChannelFactory &ecf,
    TAO_InputCDR & cdr
    ACE_ENV_ARG_DECL);

  /// Destructor
  ~Delivery_Method_Dispatch ();

  /// Create a copy of this object.
  TAO_Notify_Method_Request* copy (void);

  /// Execute the Request
  virtual int execute (ExecuteOption eo ACE_ENV_ARG_DECL);

private:
  /// Proxy Supplier that we use.
  TAO_Notify_ProxySupplier* proxy_supplier_;

  /// Guard to automatically inc/decr ref count on the proxy.
  TAO_Notify_Refcountable_Guard refcountable_guard_;

  bool filtering_;
};

} //namespace TAO_NOTIFY

#if defined (__ACE_INLINE__)
#include "Delivery_Method_Dispatch.inl"
#endif /* __ACE_INLINE__ */

#include "ace/post.h"
#endif /* TAO_NOTIFY_DELIVERY_METHOD_DISPATCH_H */
