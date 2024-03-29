/* -*- C++ -*- */
/**
 *  @file ProxyPushConsumer.h
 *
 *  @author Pradeep Gore <pradeep@oomworks.com>
 */

#ifndef TAO_Notify_PROXYPUSHCONSUMER_H
#define TAO_Notify_PROXYPUSHCONSUMER_H

#include /**/ "ace/pre.h"

#include "orbsvcs/Notify/notify_serv_export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "orbsvcs/CosNotifyChannelAdminS.h"
#include "orbsvcs/Notify/ProxyConsumer_T.h"

#if defined(_MSC_VER)
#pragma warning(push)
#pragma warning(disable:4250)
#endif /* _MSC_VER */

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

/**
 * @class TAO_Notify_ProxyPushConsumer
 *
 * @brief
 */
class TAO_Notify_Serv_Export TAO_Notify_ProxyPushConsumer
  : public virtual TAO_Notify_ProxyConsumer_T <
                       POA_CosNotifyChannelAdmin::ProxyPushConsumer>
{
 typedef TAO_Notify_ProxyConsumer_T <POA_CosNotifyChannelAdmin::ProxyPushConsumer> SuperClass;
 friend class TAO_Notify_Builder;
public:
  /// Constructor
  TAO_Notify_ProxyPushConsumer ();

  /// Destructor
  virtual ~TAO_Notify_ProxyPushConsumer ();

  virtual const char * get_proxy_type_name () const;

  virtual void load_attrs (const TAO_Notify::NVPList& attrs);
  virtual void validate ();

  virtual void configure(TAO_Notify_SupplierAdmin & admin, CosNotifyChannelAdmin::ProxyID_out proxy_id);

protected:
  ///= CosNotifyChannelAdmin::ProxyPushConsumer methods

  virtual CosNotifyChannelAdmin::ProxyType MyType ();

  virtual void push (const CORBA::Any & data);

  virtual void connect_any_push_supplier (CosEventComm::PushSupplier_ptr push_supplier);

  virtual void disconnect_push_consumer ();

private:
  /// Release
  virtual void release ();
};

TAO_END_VERSIONED_NAMESPACE_DECL

#if defined(_MSC_VER)
#pragma warning(pop)
#endif /* _MSC_VER */

#include /**/ "ace/post.h"
#endif /* TAO_Notify_PROXYPUSHCONSUMER_H */
