/* -*- C++ -*- */
/**
 *  @file RTCORBA_StructuredProxyPushSupplier_Tie.h
 *
 *  $Id$
 *
 *  @author Pradeep Gore <pradeep@oomworks.com>
 *
 *
 */
#ifndef TAO_NS_RTCORBA_STRUCTUREDPROXYPUSHSUPPLIER_TIE_H
#define TAO_NS_RTCORBA_STRUCTUREDPROXYPUSHSUPPLIER_TIE_H

#include "ace/pre.h"
#include "../rt_notify_export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "tao/corba.h"
#include "tao/PortableServer/PortableServer.h"
#include "orbsvcs/CosNotificationC.h"
#include "orbsvcs/CosNotifyCommC.h"
#include "orbsvcs/CosNotifyChannelAdminC.h"
#include "orbsvcs/Event_ForwarderS.h"
#include "orbsvcs/Event_ForwarderS_T.h"
#include "RTCORBA_StructuredProxyPushSupplier.h"
#include "../Destroy_Callback.h"

/**
 * @class TAO_NS_RTCORBA_StructuredProxyPushSupplier_Tie
 *
 * @brief Tie implementation of the StructuredProxyPushSupplier
 *
 */

class TAO_RT_Notify_Export TAO_NS_RTCORBA_StructuredProxyPushSupplier_Tie : public POA_Notify_Internal::Event_Forwarder_tie <TAO_NS_RTCORBA_StructuredProxyPushSupplier>, public TAO_NS_Destroy_Callback
{
  typedef POA_Notify_Internal::Event_Forwarder_tie <TAO_NS_RTCORBA_StructuredProxyPushSupplier> inherited;
public:
  /// Constuctor
  TAO_NS_RTCORBA_StructuredProxyPushSupplier_Tie (void);

  /// Destructor
  ~TAO_NS_RTCORBA_StructuredProxyPushSupplier_Tie ();  

  /// ServantBase refcount methods.
  virtual void _add_ref (ACE_ENV_SINGLE_ARG_DECL);
  virtual void _remove_ref (ACE_ENV_SINGLE_ARG_DECL);

  /// TAO_NS_Destroy_Callback methods
  virtual void release (void);

private:
  TAO_NS_RTCORBA_StructuredProxyPushSupplier tied_; 
};

#if defined (__ACE_INLINE__)
#include "RTCORBA_StructuredProxyPushSupplier_Tie.inl"
#endif /* __ACE_INLINE__ */

#include "ace/post.h"
#endif /* TAO_NS_RTCORBA_STRUCTUREDPROXYPUSHSUPPLIER_TIE_H */

