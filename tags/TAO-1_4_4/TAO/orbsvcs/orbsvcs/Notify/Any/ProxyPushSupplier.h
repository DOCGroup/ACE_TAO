/* -*- C++ -*- */
/**
 *  @file ProxyPushSupplier.h
 *
 *  $Id$
 *
 *  @author Pradeep Gore <pradeep@oomworks.com>
 *
 *
 */

#ifndef TAO_Notify_PROXYPUSHSUPPLIER_H
#define TAO_Notify_PROXYPUSHSUPPLIER_H

#include /**/ "ace/pre.h"

#include "../notify_serv_export.h"

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
template class TAO_Notify_Serv_Export
TAO_Notify_ProxySupplier_T<POA_Event_Forwarder::ProxyPushSupplier>;
#endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION_EXPORT */

/**
 * @class TAO_Notify_ProxyPushSupplier
 *
 * @brief
 *
 */
class TAO_Notify_Serv_Export TAO_Notify_ProxyPushSupplier
  : public virtual TAO_Notify_ProxySupplier_T <POA_Event_Forwarder::ProxyPushSupplier>
{
  typedef TAO_Notify_ProxySupplier_T <POA_Event_Forwarder::ProxyPushSupplier> SuperClass;
  friend class TAO_Notify_Builder;
public:
  /// Constuctor
  TAO_Notify_ProxyPushSupplier (void);

  /// Destructor
  ~TAO_Notify_ProxyPushSupplier ();

  /// TAO_Notify_Destroy_Callback methods
  virtual void release (void);

  virtual const char * get_proxy_type_name (void) const;

  virtual void load_attrs (const TAO_Notify::NVPList& attrs);

  // = Interface methods
  virtual CosNotifyChannelAdmin::ProxyType MyType (ACE_ENV_SINGLE_ARG_DECL)
    ACE_THROW_SPEC ((
                     CORBA::SystemException
                     ));

  virtual void connect_any_push_consumer (
    CosEventComm::PushConsumer_ptr push_consumer
    ACE_ENV_ARG_DECL
  )
  ACE_THROW_SPEC ((
    CORBA::SystemException,
    CosEventChannelAdmin::AlreadyConnected,
    CosEventChannelAdmin::TypeError
  ));

  virtual void disconnect_push_supplier (
    ACE_ENV_SINGLE_ARG_DECL
  )
  ACE_THROW_SPEC ((
    CORBA::SystemException
  ));
};

#if defined(_MSC_VER) && (_MSC_VER >= 1200)
#pragma warning(pop)
#endif /* _MSC_VER */

#include /**/ "ace/post.h"

#endif /* TAO_Notify_PROXYPUSHSUPPLIER_H */
