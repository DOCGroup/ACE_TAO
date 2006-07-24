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

#include /**/ "ace/pre.h"

#include "orbsvcs/Notify/notify_serv_export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "orbsvcs/CosNotifyChannelAdminS.h"
#include "orbsvcs/Event_ForwarderS.h"

#include "orbsvcs/Notify/ProxySupplier_T.h"

#if defined(_MSC_VER)
#pragma warning(push)
#pragma warning(disable:4250)
#endif /* _MSC_VER */

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

#if defined ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION_EXPORT
template class TAO_Notify_Serv_Export
TAO_Notify_ProxySupplier_T<POA_Event_Forwarder::StructuredProxyPushSupplier>;
#endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION_EXPORT */

/**
 * @class TAO_Notify_StructuredProxyPushSupplier
 *
 * @brief Implements the CosNotifyChannelAdmin::StructuredProxyPushSupplier methods.
 *
 *
 */
class TAO_Notify_Serv_Export TAO_Notify_StructuredProxyPushSupplier
  : public virtual TAO_Notify_ProxySupplier_T <POA_Event_Forwarder::StructuredProxyPushSupplier>
{
  typedef TAO_Notify_ProxySupplier_T <POA_Event_Forwarder::StructuredProxyPushSupplier> SuperClass;
  friend class TAO_Notify_Builder;

public:
  /// Constuctor
  TAO_Notify_StructuredProxyPushSupplier (void);

  /// Destructor
  virtual ~TAO_Notify_StructuredProxyPushSupplier ();


  virtual void load_attrs (const TAO_Notify::NVPList& attrs);

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

  virtual const char * get_proxy_type_name (void) const;

private:

  /// Release
  virtual void release (void);
};

TAO_END_VERSIONED_NAMESPACE_DECL

#if defined(_MSC_VER)
#pragma warning(pop)
#endif /* _MSC_VER */

#include /**/ "ace/post.h"

#endif /* TAO_Notify_STRUCTUREDPROXYPUSHSUPPLIER_H */
