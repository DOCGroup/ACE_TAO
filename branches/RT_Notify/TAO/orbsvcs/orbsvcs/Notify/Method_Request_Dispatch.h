/* -*- C++ -*- */
/**
 *  @file Method_Request_Dispatch.h
 *
 *  $Id$
 *
 *  @author Pradeep Gore <pradeep@oomworks.com>
 *
 *
 */

#ifndef TAO_NS_DISPATCH_METHOD_REQUEST_H
#define TAO_NS_DISPATCH_METHOD_REQUEST_H
#include "ace/pre.h"

#include "notify_export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "Method_Request.h"
#include "Types.h"

/**
 * @class TAO_NS_Method_Request_Dispatch
 *
 * @brief Dispatchs an event to a proxy supplier.
 *
 */
class TAO_Notify_Export TAO_NS_Method_Request_Dispatch  : public TAO_NS_Method_Request
{
public:
  /// Constuctor
  TAO_NS_Method_Request_Dispatch (TAO_NS_Event_var& event, TAO_NS_ProxySupplier* proxy_supplier);

  /// Destructor
  ~TAO_NS_Method_Request_Dispatch ();  

  /// Create a copy of this object.
  TAO_NS_Method_Request* copy (void);

  /// Execute method.
  virtual int call (void);

private:
  /// Proxy Supplier that we use.
  TAO_NS_ProxySupplier* proxy_supplier_;
};

#if defined (__ACE_INLINE__)
#include "Method_Request_Dispatch.inl"
#endif /* __ACE_INLINE__ */

#include "ace/post.h"
#endif /* TAO_NS_DISPATCH_METHOD_REQUEST_H */
