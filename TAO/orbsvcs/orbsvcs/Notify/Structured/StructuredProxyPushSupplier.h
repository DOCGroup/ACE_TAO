/* -*- C++ -*- */
/**
 *  @file StructuredProxyPushSupplier.h
 *
 *  $Id$
 *
 *  @author Pradeep Gore <pradeep@oomworks.com>
 *
 *
 */

#ifndef TAO_Notify_STRUCTUREDPROXYPUSHSUPPLIER_H
#define TAO_Notify_STRUCTUREDPROXYPUSHSUPPLIER_H
#include "ace/pre.h"

#include "../notify_export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "orbsvcs/CosNotifyChannelAdminS.h"
#include "orbsvcs/Event_ForwarderS.h"
#include "../ProxySupplier_T.h"

#if defined(_MSC_VER)
#if (_MSC_VER >= 1200)
#pragma warning(push)
#endif /* _MSC_VER >= 1200 */
#pragma warning(disable:4250)
#endif /* _MSC_VER */

#if defined ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION_EXPORT
template class TAO_Notify_Export
TAO_Notify_ProxySupplier_T<POA_Event_Forwarder::StructuredProxyPushSupplier>;
#endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION_EXPORT */

/**
 * @class TAO_Notify_StructuredProxyPushSupplier
 *
 * @brief Implements the CosNotifyChannelAdmin::StructuredProxyPushSupplier methods.
 *
 *
 */
class TAO_Notify_Export TAO_Notify_StructuredProxyPushSupplier : public virtual TAO_Notify_ProxySupplier_T <POA_Event_Forwarder::StructuredProxyPushSupplier>
{
  friend class TAO_Notify_Builder;

public:
  /// Constuctor
  TAO_Notify_StructuredProxyPushSupplier (void);

  /// Destructor
  ~TAO_Notify_StructuredProxyPushSupplier ();

  /// Destroy this object.
  virtual void destroy (ACE_ENV_SINGLE_ARG_DECL);

  /// Release
  virtual void release (void);

  /// = Servant methods
  // = interface methods
  virtual CosNotifyChannelAdmin::ProxyType MyType (ACE_ENV_SINGLE_ARG_DECL)
    ACE_THROW_SPEC ((
                     CORBA::SystemException
                     ));

  virtual void connect_structured_push_consumer (
    CosNotifyComm::StructuredPushConsumer_ptr push_consumer
    ACE_ENV_ARG_DECL
  )
  ACE_THROW_SPEC ((
    CORBA::SystemException,
    CosEventChannelAdmin::AlreadyConnected,
    CosEventChannelAdmin::TypeError
  ));

  virtual void disconnect_structured_push_supplier (
    ACE_ENV_SINGLE_ARG_DECL
  )
  ACE_THROW_SPEC ((
    CORBA::SystemException
  ));


};

#if defined(_MSC_VER) && (_MSC_VER >= 1200)
#pragma warning(pop)
#endif /* _MSC_VER */

#if defined (__ACE_INLINE__)
#include "StructuredProxyPushSupplier.inl"
#endif /* __ACE_INLINE__ */

#include "ace/post.h"
#endif /* TAO_Notify_STRUCTUREDPROXYPUSHSUPPLIER_H */
