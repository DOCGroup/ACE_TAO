/* -*- C++ -*- */
/**
 *  @file ProxyPushConsumer.h
 *
 *  $Id$
 *
 *  @author Pradeep Gore <pradeep@oomworks.com>
 *
 *
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
 *
 */
class TAO_Notify_Serv_Export TAO_Notify_ProxyPushConsumer
  : public virtual TAO_Notify_ProxyConsumer_T <
                       POA_CosNotifyChannelAdmin::ProxyPushConsumer
                     >
{
 typedef TAO_Notify_ProxyConsumer_T <POA_CosNotifyChannelAdmin::ProxyPushConsumer> SuperClass;
 friend class TAO_Notify_Builder;
public:
  /// Constuctor
  TAO_Notify_ProxyPushConsumer (void);

  /// Destructor
  virtual ~TAO_Notify_ProxyPushConsumer ();


//  virtual void save_persistent (TAO_Notify::Topology_Saver& saver ACE_ENV_ARG_DECL);
  virtual const char * get_proxy_type_name (void) const;

  virtual void load_attrs (const TAO_Notify::NVPList& attrs);

protected:
  ///= CosNotifyChannelAdmin::ProxyPushConsumer methods

  virtual CosNotifyChannelAdmin::ProxyType MyType (ACE_ENV_SINGLE_ARG_DECL)
    ACE_THROW_SPEC ((
                     CORBA::SystemException
                     ));

  virtual void push (const CORBA::Any & data ACE_ENV_ARG_DECL)
    ACE_THROW_SPEC ((
                   CORBA::SystemException,
                   CosEventComm::Disconnected
                   ));

  virtual void connect_any_push_supplier (CosEventComm::PushSupplier_ptr push_supplier ACE_ENV_ARG_DECL)
    ACE_THROW_SPEC ((
                     CORBA::SystemException,
                     CosEventChannelAdmin::AlreadyConnected
                     ));

  virtual void disconnect_push_consumer (ACE_ENV_SINGLE_ARG_DECL)
    ACE_THROW_SPEC ((
                     CORBA::SystemException
                     ));

private:
  /// Release
  virtual void release (void);
};

TAO_END_VERSIONED_NAMESPACE_DECL

#if defined(_MSC_VER)
#pragma warning(pop)
#endif /* _MSC_VER */

#include /**/ "ace/post.h"
#endif /* TAO_Notify_PROXYPUSHCONSUMER_H */
