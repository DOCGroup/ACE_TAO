/* -*- C++ -*- */
/**
 *  @file RTCORBA_StructuredProxyPushConsumer.h
 *
 *  $Id$
 *
 *  @author Pradeep Gore <pradeep@oomworks.com>
 *
 *
 */

#ifndef TAO_NS_RTCORBA_STRUCTUREDPROXYPUSHCONSUMER_H
#define TAO_NS_RTCORBA_STRUCTUREDPROXYPUSHCONSUMER_H
#include "ace/pre.h"

#include "../rt_notify_export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "StructuredProxyPushConsumer.h"
#include "../RTCORBA_ProxyConsumer.h"

/**
 * @class TAO_NS_RTCORBA_StructuredProxyPushConsumer
 *
 * @brief RTCORBA specific StructuredProxyPushConsumer
 *
 */
class TAO_RT_Notify_Export TAO_NS_RTCORBA_StructuredProxyPushConsumer : public TAO_NS_StructuredProxyPushConsumer, public TAO_NS_RTCORBA_ProxyConsumer
{
public:
  /// Constuctor
  TAO_NS_RTCORBA_StructuredProxyPushConsumer (void);

  /// Destructor
  ~TAO_NS_RTCORBA_StructuredProxyPushConsumer ();
};

#if defined (__ACE_INLINE__)
#include "RTCORBA_StructuredProxyPushConsumer.inl"
#endif /* __ACE_INLINE__ */

#include "ace/post.h"
#endif /* TAO_NS_RTCORBA_STRUCTUREDPROXYPUSHCONSUMER_H */
