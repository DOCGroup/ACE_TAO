/* -*- C++ -*- */
/**
 *  @file RTCORBA_Factory.h
 *
 *  $Id$
 *
 *  @author Pradeep Gore <pradeep@oomworks.com>
 *
 *
 */

#ifndef TAO_NS_RTCORBA_FACTORY_H
#define TAO_NS_RTCORBA_FACTORY_H
#include "ace/pre.h"

#include "rt_notify_export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "Factory.h"

/**
 * @class TAO_NS_RTCORBA_Factory
 *
 * @brief The Factory for the RTCORBA aware Notify classes.
 *
 */
class TAO_RT_Notify_Export TAO_NS_RTCORBA_Factory : public TAO_NS_Factory
{
public:
  /// Constuctor
  TAO_NS_RTCORBA_Factory (void);

  /// Destructor
  ~TAO_NS_RTCORBA_Factory ();

  /// Create QoSAdmin
  virtual void create (TAO_NS_QoSAdmin*& qos_admin ACE_ENV_SINGLE_ARG_DECL);

  /// Create EventChannel
  virtual void create (TAO_NS_EventChannel*& ACE_ENV_SINGLE_ARG_DECL);

  /// Create StructuredProxyPushSupplier
  virtual void create (TAO_NS_StructuredProxyPushSupplier*& proxy ACE_ENV_SINGLE_ARG_DECL);

  /// Create StructuredProxyPushConsumer
  virtual void create (TAO_NS_StructuredProxyPushConsumer*& proxy ACE_ENV_SINGLE_ARG_DECL);
};

#if defined (__ACE_INLINE__)
#include "RTCORBA_Factory.inl"
#endif /* __ACE_INLINE__ */

#include "ace/post.h"
#endif /* TAO_NS_RTCORBA_FACTORY_H */
