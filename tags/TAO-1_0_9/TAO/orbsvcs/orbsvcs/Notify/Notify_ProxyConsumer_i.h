/* -*- C++ -*- */
// $Id$
// ==========================================================================
//
// = LIBRARY
//   orbsvcs
//
// = FILENAME
//   Notify_ProxyConsumer_i.h
//
// = DESCRIPTION
//
//
// = AUTHOR
//    Pradeep Gore <pradeep@cs.wustl.edu>
//
// ==========================================================================

#ifndef NOTIFY_PROXYCONSUMER_I_H
#define NOTIFY_PROXYCONSUMER_I_H

#include "orbsvcs/orbsvcs/CosNotifyChannelAdminS.h"
#include "orbsvcs/orbsvcs/Notify/Notify_QoSAdmin_i.h"
#include "orbsvcs/orbsvcs/Notify/Notify_FilterAdmin_i.h"

class TAO_Notify_SupplierAdmin_i;

class TAO_Notify_ProxyConsumer_i :
public virtual POA_CosNotifyChannelAdmin::ProxyConsumer,
  public virtual TAO_Notify_QoSAdmin_i,
  public virtual TAO_Notify_FilterAdmin_i
{
  // = TITLE
  //   TAO_Notify_ProxyConsumer_i
  // = DESCRIPTION
  //
  //

public:
  //Constructor
  TAO_Notify_ProxyConsumer_i (TAO_Notify_SupplierAdmin_i &myadmin);

  //Destructor
  virtual ~TAO_Notify_ProxyConsumer_i (void);

  void init (CORBA::Environment &ACE_TRY_ENV);
  //

  virtual CosNotifyChannelAdmin::ProxyType MyType (
    CORBA::Environment &ACE_TRY_ENV
  )
  ACE_THROW_SPEC ((
    CORBA::SystemException
  ));

virtual CosNotifyChannelAdmin::SupplierAdmin_ptr MyAdmin (
    CORBA::Environment &ACE_TRY_ENV
  )
  ACE_THROW_SPEC ((
    CORBA::SystemException
  ));

virtual CosNotification::EventTypeSeq * obtain_subscription_types (
    CosNotifyChannelAdmin::ObtainInfoMode mode,
    CORBA::Environment &ACE_TRY_ENV
  )
  ACE_THROW_SPEC ((
    CORBA::SystemException
  ));

virtual void validate_event_qos (
    const CosNotification::QoSProperties & required_qos,
    CosNotification::NamedPropertyRangeSeq_out available_qos,
    CORBA::Environment &ACE_TRY_ENV
  )
  ACE_THROW_SPEC ((
    CORBA::SystemException,
    CosNotification::UnsupportedQoS
  ));

protected:
  TAO_Notify_SupplierAdmin_i &myadmin_;
  //
};
#endif /* NOTIFY_PROXYCONSUMER_I_H */
