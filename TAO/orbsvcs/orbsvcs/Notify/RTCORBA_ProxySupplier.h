/* -*- C++ -*- */
/**
 *  @file RTCORBA_ProxySupplier.h
 *
 *  $Id$
 *
 *  @author Pradeep Gore <pradeep@oomworks.com>
 *
 *
 */

#ifndef TAO_NS_RTCORBA_PROXYSUPPLIER_H
#define TAO_NS_RTCORBA_PROXYSUPPLIER_H
#include "ace/pre.h"

#include "rt_notify_export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "ProxySupplier.h"

/**
 * @class TAO_NS_RTCORBA_ProxySupplier
 *
 * @brief ProxySupplier implementation for RTCORBA aware Suppliers.
 *
 */
class TAO_RT_Notify_Export TAO_NS_RTCORBA_ProxySupplier : public virtual TAO_NS_ProxySupplier
{
public:
  /// Constuctor
  TAO_NS_RTCORBA_ProxySupplier (void);

  /// Destructor
  ~TAO_NS_RTCORBA_ProxySupplier ();

  /// Init method.
  virtual void init_ps (ACE_ENV_SINGLE_ARG_DECL);

  /// Override TAO_NS_ProxySupplier::push
  virtual void push (TAO_NS_Event_var &event);

  /// Dispatch Event to consumer, no filtering
  virtual void push_no_filtering (TAO_NS_Event_var &event);

  /// POA_Notify_Internal::Event_Forwarder method
  virtual void forward (const CosNotification::StructuredEvent & event ACE_ENV_ARG_DECL)
    ACE_THROW_SPEC ((
                     CORBA::SystemException
                     ));

  /// POA_Notify_Internal::Event_Forwarder method
  virtual void forward_no_filtering (const CosNotification::StructuredEvent & event ACE_ENV_ARG_DECL)
    ACE_THROW_SPEC ((
                     CORBA::SystemException
                     ));

private:
  /// Our ref.
  Notify_Internal::Event_Forwarder_var event_forwarder_;
};

#if defined (__ACE_INLINE__)
#include "RTCORBA_ProxySupplier.inl"
#endif /* __ACE_INLINE__ */

#include "ace/post.h"
#endif /* TAO_NS_RTCORBA_PROXYSUPPLIER_H */
