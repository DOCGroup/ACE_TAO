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

#include "../notify_serv_export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "orbsvcs/CosNotifyChannelAdminS.h"
#include "../ProxyConsumer_T.h"

#if defined(_MSC_VER)
#if (_MSC_VER >= 1200)
#pragma warning(push)
#endif /* _MSC_VER >= 1200 */
#pragma warning(disable:4250)
#endif /* _MSC_VER */

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
  ~TAO_Notify_ProxyPushConsumer ();

  /// Release
  virtual void release (void);

//  virtual void save_persistent (TAO_Notify::Topology_Saver& saver ACE_ENV_ARG_DECL);
  virtual const char * get_proxy_type_name (void) const;

  virtual void load_attrs (const TAO_Notify::NVPList& attrs);
  // override and hide derived init methods and
  // explicitly direct the init call down the correct
  // leg of the d**n diamond.
  virtual void init (TAO_Notify_SupplierAdmin *supplier_admin ACE_ENV_ARG_DECL);

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
  // Overloaded TAO_Notify_ProxyConsumer::push to get around Borland compiler warnings.
  // I don't think this is necessary any more -- Dale.
//  virtual void push (TAO_Notify_Event_var &event);
};

#if defined(_MSC_VER) && (_MSC_VER >= 1200)
#pragma warning(pop)
#endif /* _MSC_VER */

#if defined (__ACE_INLINE__)
#include "ProxyPushConsumer.inl"
#endif /* __ACE_INLINE__ */

#include /**/ "ace/post.h"
#endif /* TAO_Notify_PROXYPUSHCONSUMER_H */
