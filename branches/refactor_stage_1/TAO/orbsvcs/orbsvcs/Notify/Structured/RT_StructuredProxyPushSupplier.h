/* -*- C++ -*- */
/**
 *  @file RT_StructuredProxyPushSupplier.h
 *
 *  $Id$
 *
 *  @author Pradeep Gore <pradeep@oomworks.com>
 *
 *
 */

#ifndef TAO_NS_RT_STRUCTUREDPROXYPUSHSUPPLIER_H
#define TAO_NS_RT_STRUCTUREDPROXYPUSHSUPPLIER_H
#include "ace/pre.h"

#include "../rt_notify_export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "StructuredProxyPushSupplier.h"

/**
 * @class TAO_NS_RT_StructuredProxyPushSupplier
 *
 * @brief
 *
 */
class TAO_RT_Notify_Export TAO_NS_RT_StructuredProxyPushSupplier : public virtual TAO_NS_StructuredProxyPushSupplier
{
public:
  /// Constuctor
  TAO_NS_RT_StructuredProxyPushSupplier (void);

  /// Destructor
  ~TAO_NS_RT_StructuredProxyPushSupplier ();

  /// Activate this object and obtain the Event_Forwarder interface.
  virtual CORBA::Object_ptr activate (PortableServer::Servant servant ACE_ENV_ARG_DECL);

  /// Override TAO_NS_ProxySupplier::push
  virtual void push (const TAO_NS_Event_var &event);

  /// Dispatch Event to consumer, no filtering
  virtual void push_no_filtering (const TAO_NS_Event_var &event);

private:
  /// Our ref.
  Event_Forwarder::StructuredProxyPushSupplier_var event_forwarder_;
};

#if defined (__ACE_INLINE__)
#include "RT_StructuredProxyPushSupplier.inl"
#endif /* __ACE_INLINE__ */

#include "ace/post.h"
#endif /* TAO_NS_RT_STRUCTUREDPROXYPUSHSUPPLIER_H */
