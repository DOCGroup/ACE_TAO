/* -*- C++ -*- */
/**
 *  @file Delivery_Method_Lookup.h
 *
 *  $Id$
 *
 *  @author Pradeep Gore <pradeep@oomworks.com>
 *
  *
 */

#ifndef TAO_Notify_LOOKUP_METHOD_REQUEST_H
#define TAO_Notify_LOOKUP_METHOD_REQUEST_H
#include "ace/pre.h"

#include "notify_export.h"
#include "Delivery_Method.h"
#include "Types.h"
#include "orbsvcs/ESF/ESF_Worker.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

namespace TAO_NOTIFY
{
class TAO_Notify_Export Delivery_Method_Lookup
  : public Delivery_Method
  , public TAO_ESF_Worker<TAO_Notify_ProxySupplier>
{
public:
  /// an arbitrary code (Octet) to identify this type of event in persistent storage
  enum {persistence_code = 2};
  /// Constructor
  Delivery_Method_Lookup(const TAO_NOTIFY::Delivery_Request_Ptr & delivery_request, TAO_Notify_ProxyConsumer* proxy_consumer);
  Delivery_Method_Lookup(const Delivery_Method_Lookup & rhs);

  /// Destructor
  ~Delivery_Method_Lookup ();

  /// Create a copy of this object.
  TAO_Notify_Method_Request* copy (void);

  static Delivery_Method_Lookup * create (
    const Delivery_Request_Ptr& delivery_request,
    TAO_Notify_EventChannelFactory &ecf,
    TAO_InputCDR & cdr
    ACE_ENV_ARG_DECL);

  /// Execute the Request
  virtual int execute (ExecuteOption eo ACE_ENV_ARG_DECL);

  ///= TAO_ESF_Worker method
  void work (TAO_Notify_ProxySupplier* proxy_supplier ACE_ENV_ARG_DECL);

private:
  /// ProxyConsumer supplying event.
  TAO_Notify_ProxyConsumer* proxy_consumer_;

  /// Guard to automatically inc/decr ref count on the proxy.
  TAO_Notify_Refcountable_Guard refcountable_guard_;
};

} // namespace TAO_NOTIFY

#if defined (__ACE_INLINE__)
#include "Delivery_Method_Lookup.inl"
#endif /* __ACE_INLINE__ */

#include "ace/post.h"
#endif /* TAO_Notify_LOOKUP_METHOD_REQUEST_H */
