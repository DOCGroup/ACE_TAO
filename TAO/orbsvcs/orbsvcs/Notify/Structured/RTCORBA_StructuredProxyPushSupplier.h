/* -*- C++ -*- */
/**
 *  @file RTCORBA_StructuredProxyPushSupplier.h
 *
 *  $Id$
 *
 *  @author Pradeep Gore <pradeep@oomworks.com>
 *
 *
 */

#ifndef TAO_NS_RTCORBA_STRUCTUREDPROXYPUSHSUPPLIER_H
#define TAO_NS_RTCORBA_STRUCTUREDPROXYPUSHSUPPLIER_H
#include "ace/pre.h"

#include "../rt_notify_export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "StructuredProxyPushSupplier.h"
#include "../RTCORBA_ProxySupplier.h"

/**
 * @class TAO_NS_RTCORBA_StructuredProxyPushSupplier
 *
 * @brief RTCORBA specific StructuredProxyPushSupplier
 *
 */
class TAO_RT_Notify_Export TAO_NS_RTCORBA_StructuredProxyPushSupplier : public virtual TAO_NS_StructuredProxyPushSupplier, public TAO_NS_RTCORBA_ProxySupplier
{
  friend class TAO_NS_Builder;

public:
  /// Constuctor
  TAO_NS_RTCORBA_StructuredProxyPushSupplier (void);

  /// Destructor
  ~TAO_NS_RTCORBA_StructuredProxyPushSupplier ();

  /// Destroy this object.
  virtual void destroy (ACE_ENV_SINGLE_ARG_DECL);

  /// TAO_NS_Destroy_Callback methods
  virtual void release (void);
};

#if defined (__ACE_INLINE__)
#include "RTCORBA_StructuredProxyPushSupplier.inl"
#endif /* __ACE_INLINE__ */

#include "ace/post.h"
#endif /* TAO_NS_RTCORBA_STRUCTUREDPROXYPUSHSUPPLIER_H */
