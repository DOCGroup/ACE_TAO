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
  virtual ~TAO_Notify_ProxyPushSupplier ();

  virtual const char * get_proxy_type_name (void) const;

  virtual void load_attrs (const TAO_Notify::NVPList& attrs);

  // = Interface methods
  virtual CosNotifyChannelAdmin::ProxyType MyType (void);

  virtual void connect_any_push_consumer (
    CosEventComm::PushConsumer_ptr push_consumer
  );

  virtual void disconnect_push_supplier (
  );

private:
  /// TAO_Notify_Destroy_Callback methods
  virtual void release (void);
};

TAO_END_VERSIONED_NAMESPACE_DECL

#if defined(_MSC_VER)
#pragma warning(pop)
#endif /* _MSC_VER */

#include /**/ "ace/post.h"

#endif /* TAO_Notify_PROXYPUSHSUPPLIER_H */
