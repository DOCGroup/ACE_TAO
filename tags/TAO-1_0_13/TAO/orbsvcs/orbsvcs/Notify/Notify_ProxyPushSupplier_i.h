/* -*- C++ -*- */
// $Id$
// ==========================================================================
//
// = LIBRARY
//   orbsvcs
//
// = FILENAME
//   Notify_ProxyPushSupplier_i.h
//
// = DESCRIPTION
//
//
// = AUTHOR
//    Pradeep Gore <pradeep@cs.wustl.edu>
//
// ==========================================================================
#ifndef NOTIFY_PROXYPUSHSUPPLIER_I_H
#define NOTIFY_PROXYPUSHSUPPLIER_I_H

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

class TAO_ORBSVCS_Export TAO_Notify_ProxyPushSupplier_i :
public virtual POA_CosNotifyChannelAdmin::ProxyPushSupplier,
  public virtual TAO_Notify_ProxySupplier_i,
  public virtual TAO_NotifySubscribe_i
{
  // = TITLE
  //   TAO_Notify_ProxyPushSupplier_i
  // = DESCRIPTION
  //
  //

public:
  //Constructor
  TAO_Notify_ProxyPushSupplier_i (TAO_Notify_ConsumerAdmin_i &consumeradmin);

  //Destructor
  virtual ~TAO_Notify_ProxyPushSupplier_i (void);

  virtual void dispatch_event (const CORBA::Any & data,
                               CORBA::Environment &ACE_TRY_ENV);
  //

  virtual void dispatch_event (const CosNotification::StructuredEvent& event,
                               CORBA::Environment &ACE_TRY_ENV);
  //

  virtual CosNotifyChannelAdmin::ProxySupplier_ptr get_ref (CORBA::Environment &ACE_TRY_ENV);

  virtual void connect_any_push_consumer (
    CosEventComm::PushConsumer_ptr push_consumer,
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

  // = CosEventChannelAdmin::PushSupplier methods
  virtual void disconnect_push_supplier (CORBA::Environment &ACE_TRY_ENV)
    ACE_THROW_SPEC ((
                     CORBA::SystemException
                     ));

 protected:
  CosEventComm::PushConsumer_var push_consumer_;
};

#if defined(_MSC_VER) && (_MSC_VER >= 1200)
#pragma warning(pop)
#endif /* _MSC_VER */

#endif /* NOTIFY_PROXYPUSHSUPPLIER_I_H */
