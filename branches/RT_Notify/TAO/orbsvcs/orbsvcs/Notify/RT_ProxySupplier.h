/* -*- C++ -*- */
/**
 *  @file RT_ProxySupplier.h
 *
 *  $Id$
 *
 *  @author Pradeep Gore <pradeep@oomworks.com>
 *
 *
 */

#ifndef TAO_NS_RT_PROXYSUPPLIER_H
#define TAO_NS_RT_PROXYSUPPLIER_H
#include "ace/pre.h"

#include "rt_notify_export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "ProxySupplier.h"

/**
 * @class TAO_NS_RT_ProxySupplier
 *
 * @brief ProxySupplier implementation for RT aware Suppliers.
 *
 */
class TAO_RT_Notify_Export TAO_NS_RT_ProxySupplier : public virtual TAO_NS_ProxySupplier
{
public:
  /// Constuctor
  TAO_NS_RT_ProxySupplier (void);

  /// Destructor
  virtual ~TAO_NS_RT_ProxySupplier ();

  /// Init
  virtual void init (TAO_NS_ConsumerAdmin* consumer_admin ACE_ENV_ARG_DECL);

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
#include "RT_ProxySupplier.inl"
#endif /* __ACE_INLINE__ */

#include "ace/post.h"
#endif /* TAO_NS_RT_PROXYSUPPLIER_H */
