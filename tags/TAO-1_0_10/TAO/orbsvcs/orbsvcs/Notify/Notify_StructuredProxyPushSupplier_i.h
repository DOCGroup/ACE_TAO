/* -*- C++ -*- */
// $Id$
// ==========================================================================
//
// = LIBRARY
//   orbsvcs
//
// = FILENAME
//   Notify_StructuredProxyPushSupplier_i.h
//
// = DESCRIPTION
//
//
// = AUTHOR
//    Pradeep Gore <pradeep@cs.wustl.edu>
//
// ==========================================================================
#ifndef NOTIFY_STRUCTUREDPROXYPUSHSUPPLIER_I_H
#define NOTIFY_STRUCTUREDPROXYPUSHSUPPLIER_I_H

#include "orbsvcs/orbsvcs/CosNotifyChannelAdminS.h"
#include "orbsvcs/orbsvcs/Notify/Notify_ProxySupplier_i.h"
#include "orbsvcs/orbsvcs/Notify/NotifySubscribe_i.h"

class TAO_Notify_ConsumerAdmin_i;

#if defined(_MSC_VER)
#if (_MSC_VER >= 1200)
#pragma warning(push)
#endif /* _MSC_VER >= 1200 */
#pragma warning(disable:4250)
#endif /* _MSC_VER */

class TAO_Notify_StructuredProxyPushSupplier_i :
public virtual POA_CosNotifyChannelAdmin::StructuredProxyPushSupplier,
  public virtual TAO_Notify_ProxySupplier_i,
  public virtual TAO_NotifySubscribe_i
{
  // = TITLE
  //   TAO_Notify_StructuredProxyPushSupplier_i
  // = DESCRIPTION
  //
  //

 public:
  //Constructor
  TAO_Notify_StructuredProxyPushSupplier_i (TAO_Notify_ConsumerAdmin_i &consumeradmin);

  //Destructor
  virtual ~TAO_Notify_StructuredProxyPushSupplier_i (void);

  virtual void dispatch_event (const CORBA::Any & data,
                               CORBA::Environment &ACE_TRY_ENV);
  //

  virtual void dispatch_event (const CosNotification::StructuredEvent& event,
                               CORBA::Environment &ACE_TRY_ENV);

  CosNotifyChannelAdmin::ProxySupplier_ptr get_ref (CORBA::Environment &ACE_TRY_ENV);
  // convert to object ref.

  virtual void connect_structured_push_consumer
    (
     CosNotifyComm::StructuredPushConsumer_ptr push_consumer,
     CORBA::Environment &ACE_TRY_ENV
     )
  ACE_THROW_SPEC ((
    CORBA::SystemException,
    CosEventChannelAdmin::AlreadyConnected,
    CosEventChannelAdmin::TypeError
  ));

  virtual void suspend_connection (
    CORBA::Environment &ACE_TRY_ENV
  )
  ACE_THROW_SPEC ((
    CORBA::SystemException,
    CosNotifyChannelAdmin::ConnectionAlreadyInactive,
    CosNotifyChannelAdmin::NotConnected
  ));

  virtual void resume_connection (
    CORBA::Environment &ACE_TRY_ENV
  )
  ACE_THROW_SPEC ((
    CORBA::SystemException,
    CosNotifyChannelAdmin::ConnectionAlreadyActive,
    CosNotifyChannelAdmin::NotConnected
  ));

  // = CosNotifyComm::StructuredPushSupplier method
  virtual void disconnect_structured_push_supplier (
    CORBA::Environment &ACE_TRY_ENV
    )
  ACE_THROW_SPEC ((
    CORBA::SystemException
  ));

 protected:
 CosNotifyComm::StructuredPushConsumer_var push_consumer_;
 // The consumer that we are connected with.
};

#if defined(_MSC_VER) && (_MSC_VER >= 1200)
#pragma warning(pop)
#endif /* _MSC_VER */

#endif /* NOTIFY_STRUCTUREDPROXYPUSHSUPPLIER_I_H */
