/* -*- C++ -*- */
/**
 *  @file Method_Request_Dispatch_No_Filtering.h
 *
 *  $Id$
 *
 *  @author Pradeep Gore <pradeep@oomworks.com>
 *
 *
 */

#ifndef TAO_NS_DISPATCH_METHOD_REQUEST_NO_FILTERING_H
#define TAO_NS_DISPATCH_METHOD_REQUEST_NO_FILTERING_H
#include "ace/pre.h"

#include "notify_export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "Method_Request.h"
#include "Refcountable.h"

class TAO_NS_ProxySupplier;

/**
 * @class TAO_NS_Method_Request_Dispatch_No_Filtering
 *
 * @brief Dispatchs an event to a proxy supplier but does NOT perform filtering.
 *
 */
class TAO_Notify_Export TAO_NS_Method_Request_Dispatch_No_Filtering  : public TAO_NS_Method_Request_Event
{
public:
  /// Constuctor
  TAO_NS_Method_Request_Dispatch_No_Filtering (const TAO_NS_Event_var& event, TAO_NS_ProxySupplier* proxy_supplier);

  /// Destructor
  ~TAO_NS_Method_Request_Dispatch_No_Filtering ();

  /// Create a copy of this object.
  TAO_NS_Method_Request* copy (void);

  /// Execute the Request
  virtual int execute (ACE_ENV_SINGLE_ARG_DECL);

private:
  /// Proxy Supplier that we use.
  TAO_NS_ProxySupplier* proxy_supplier_;

  /// Guard to automatically inc/decr ref count on the proxy.
  TAO_NS_Refcountable_Guard refcountable_guard_;
};

#if defined (__ACE_INLINE__)
#include "Method_Request_Dispatch_No_Filtering.inl"
#endif /* __ACE_INLINE__ */

#include "ace/post.h"
#endif /* TAO_NS_DISPATCH_METHOD_REQUEST_NO_FILTERING_H */
