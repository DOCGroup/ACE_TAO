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

#ifndef TAO_Notify_RT_STRUCTUREDPROXYPUSHSUPPLIER_H
#define TAO_Notify_RT_STRUCTUREDPROXYPUSHSUPPLIER_H
#include /**/ "ace/pre.h"

#include "orbsvcs/Notify/rt_notify_export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "orbsvcs/Notify/Structured/StructuredProxyPushSupplier.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

/**
 * @class TAO_Notify_RT_StructuredProxyPushSupplier
 *
 * @brief
 *
 */
class TAO_RT_Notify_Export TAO_Notify_RT_StructuredProxyPushSupplier :
  public virtual TAO_Notify_StructuredProxyPushSupplier
{
public:
  /// Constuctor
  TAO_Notify_RT_StructuredProxyPushSupplier (void);

  /// Destructor
  ~TAO_Notify_RT_StructuredProxyPushSupplier ();

  /// Activate this object and obtain the Event_Forwarder interface.
  virtual CORBA::Object_ptr activate (PortableServer::Servant servant);

  /// Dummy method to prevent hidden TAO_Notify_Proxy::activate warning.
  virtual CORBA::Object_ptr activate (PortableServer::Servant servant,
                                      CORBA::Long id);

  /// Dispatch Event to consumer
  void deliver (TAO_Notify_Method_Request_Dispatch_No_Copy & request);

  /// Dispatch Event to consumer, no filtering
  virtual void push_no_filtering (const TAO_Notify_Event* event);

private:
  /// Our ref.
  Event_Forwarder::StructuredProxyPushSupplier_var event_forwarder_;
};

TAO_END_VERSIONED_NAMESPACE_DECL

#include /**/ "ace/post.h"
#endif /* TAO_Notify_RT_STRUCTUREDPROXYPUSHSUPPLIER_H */
