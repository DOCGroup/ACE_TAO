/* -*- C++ -*- */
/**
 *  @file Method_Request_Dispatch_Base.h
 *
 *  $Id$
 *
 *  @author Pradeep Gore <pradeep@oomworks.com>
 *
 *
 */

#ifndef TAO_Notify_METHOD_REQUEST_DISPATCH_BASE_H
#define TAO_Notify_METHOD_REQUEST_DISPATCH_BASE_H

#include /**/ "ace/pre.h"

#include "notify_serv_export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "tao/Basic_Types.h"

#include "ace/CORBA_macros.h"
#include "ProxySupplier.h"

class TAO_Notify_Event;

/**
 * @class TAO_Notify_Method_Request_Dispatch_Base
 *
 * @brief
 *
 */
class TAO_Notify_Method_Request_Dispatch_Base
{
public:
  /// Constuctor
  TAO_Notify_Method_Request_Dispatch_Base (const TAO_Notify_Event *,
                                        TAO_Notify_ProxySupplier* proxy,
                                        bool filtering);

  /// Destructor
  virtual ~TAO_Notify_Method_Request_Dispatch_Base ();

  /// Execute the dispatch operation.
  int execute_i (ACE_ENV_SINGLE_ARG_DECL);

protected:
  /// The Event
  const TAO_Notify_Event * event_;

  /// The Proxy
  TAO_Notify_ProxySupplier * proxy_supplier_;
//  TAO_Notify_ProxySupplier_Guard proxy_supplier_;
  /// Flag is true if we want to do fintering else false.
  bool filtering_;
};

#if defined (__ACE_INLINE__)
#include "Method_Request_Dispatch_Base.inl"
#endif /* __ACE_INLINE__ */

#include /**/ "ace/post.h"

#endif /* TAO_Notify_METHOD_REQUEST_DISPATCH_BASE_H */
