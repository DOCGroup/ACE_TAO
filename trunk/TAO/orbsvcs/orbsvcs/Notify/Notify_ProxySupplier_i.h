/* -*- C++ -*- */
// $Id$
// ==========================================================================
//
// = LIBRARY
//   orbsvcs
//
// = FILENAME
//
//
// = DESCRIPTION
//
//
// = AUTHOR
//    Pradeep Gore <pradeep@cs.wustl.edu>
//
// ==========================================================================

#ifndef TAO_NOTIFY_PROXYSUPPLIER_H
#define TAO_NOTIFY_PROXYSUPPLIER_H

#include "orbsvcs/orbsvcs/CosNotifyChannelAdminS.h"
#include "orbsvcs/orbsvcs/Notify/Notify_QoSAdmin_i.h"
#include "orbsvcs/orbsvcs/Notify/Notify_FilterAdmin_i.h"
#include "ace/Auto_Ptr.h"
#include "Notify_Dispatcher.h"

class TAO_Notify_ConsumerAdmin_i;

class TAO_Notify_ProxySupplier_i :
public virtual POA_CosNotifyChannelAdmin::ProxySupplier,
public virtual TAO_Notify_QoSAdmin_i,
public virtual TAO_Notify_FilterAdmin_i,
public TAO_Notify_Dispatcher
{
  // = TITLE
  //   TAO_Notify_ProxySupplier_i
  // = DESCRIPTION
  //
  //

public:
  //Constructor
  TAO_Notify_ProxySupplier_i (TAO_Notify_ConsumerAdmin_i &myadmin);

  //Destructor
  virtual ~TAO_Notify_ProxySupplier_i (void);

  void init (CORBA::Environment &ACE_TRY_ENV);
  //

  virtual CosNotifyChannelAdmin::ProxyType MyType (
    CORBA::Environment &ACE_TRY_ENV
  )
  ACE_THROW_SPEC ((
    CORBA::SystemException
  ));

virtual CosNotifyChannelAdmin::ConsumerAdmin_ptr MyAdmin (
    CORBA::Environment &ACE_TRY_ENV
  )
  ACE_THROW_SPEC ((
    CORBA::SystemException
  ));

virtual CosNotifyFilter::MappingFilter_ptr priority_filter (
    CORBA::Environment &ACE_TRY_ENV
  )
  ACE_THROW_SPEC ((
    CORBA::SystemException
  ));

virtual void priority_filter (
    CosNotifyFilter::MappingFilter_ptr priority_filter,
    CORBA::Environment &ACE_TRY_ENV
  )
  ACE_THROW_SPEC ((
    CORBA::SystemException
  ));

virtual CosNotifyFilter::MappingFilter_ptr lifetime_filter (
    CORBA::Environment &ACE_TRY_ENV
  )
  ACE_THROW_SPEC ((
    CORBA::SystemException
  ));

virtual void lifetime_filter (
    CosNotifyFilter::MappingFilter_ptr lifetime_filter,
    CORBA::Environment &ACE_TRY_ENV
  )
  ACE_THROW_SPEC ((
    CORBA::SystemException
  ));

virtual CosNotification::EventTypeSeq * obtain_offered_types (
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
  TAO_Notify_ConsumerAdmin_i &myadmin_;
  //
};
#endif /* TAO_NOTIFY_PROXYSUPPLIER_H */
