/* -*- C++ -*- */
/**
 *  @file Method_Request_Lookup_Base.h
 *
 *  $Id$
 *
 *  @author Pradeep Gore <pradeep@oomworks.com>
 *
 *
 */

#ifndef TAO_Notify_METHOD_REQUEST_LOOKUP_BASE_H
#define TAO_Notify_METHOD_REQUEST_LOOKUP_BASE_H
#include /**/ "ace/pre.h"

#include "notify_serv_export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "orbsvcs/ESF/ESF_Worker.h"
#include "ProxySupplier.h"
class TAO_Notify_Event;
class TAO_Notify_ProxyConsumer;

/**
 * @class TAO_Notify_Method_Request_Lookup_Base
 *
 * @brief
 *
 */
class TAO_Notify_Serv_Export TAO_Notify_Method_Request_Lookup_Base
  : public TAO_ESF_Worker<TAO_Notify_ProxySupplier>
{
public:
  /// Constuctor
  TAO_Notify_Method_Request_Lookup_Base (const TAO_Notify_Event * event, TAO_Notify_ProxyConsumer * proxy);

  /// Destructor
  virtual ~TAO_Notify_Method_Request_Lookup_Base ();

  /// Execute the dispatch operation.
  int execute_i (ACE_ENV_SINGLE_ARG_DECL);

protected:
  ///= TAO_ESF_Worker method
  virtual void work (TAO_Notify_ProxySupplier* proxy_supplier ACE_ENV_ARG_DECL);

  /// The Event
  const TAO_Notify_Event * event_;

  /// The Proxy
  TAO_Notify_ProxyConsumer* proxy_consumer_;
};

#if defined (__ACE_INLINE__)
#include "Method_Request_Lookup_Base.inl"
#endif /* __ACE_INLINE__ */

#include /**/ "ace/post.h"
#endif /* TAO_Notify_METHOD_REQUEST_LOOKUP_BASE_H */
