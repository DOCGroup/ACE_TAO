/* -*- C++ -*- $Id$ */

#include "orbsvcs/orbsvcs/CosNotifyChannelAdminS.h"
#include "orbsvcs/orbsvcs/Notify/Notify_ProxyConsumer_i.h"
#include "orbsvcs/orbsvcs/Notify/Notify_SupplierAdmin_i.h"

// Implementation skeleton constructor
TAO_Notify_ProxyConsumer_i::TAO_Notify_ProxyConsumer_i
(TAO_Notify_SupplierAdmin_i &myadmin)
  :myadmin_ (myadmin)
{
}

// Implementation skeleton destructor
TAO_Notify_ProxyConsumer_i::~TAO_Notify_ProxyConsumer_i (void)
{
}

void
TAO_Notify_ProxyConsumer_i::init (CORBA::Environment & /* ACE_TRY_ENV */)
{
}

CosNotifyChannelAdmin::ProxyType TAO_Notify_ProxyConsumer_i::MyType (
    CORBA::Environment & /* ACE_TRY_ENV */
  )
  ACE_THROW_SPEC ((
    CORBA::SystemException
  ))

  {
    //Add your implementation here
    return (CosNotifyChannelAdmin::ProxyType) 0;
  }

CosNotifyChannelAdmin::SupplierAdmin_ptr TAO_Notify_ProxyConsumer_i::MyAdmin (
    CORBA::Environment & /* ACE_TRY_ENV */
  )
  ACE_THROW_SPEC ((
    CORBA::SystemException
  ))

  {
    //Add your implementation here
    return 0;
  }

CosNotification::EventTypeSeq * TAO_Notify_ProxyConsumer_i::obtain_subscription_types (
    CosNotifyChannelAdmin::ObtainInfoMode /* mode */,
    CORBA::Environment & /* ACE_TRY_ENV */
  )
  ACE_THROW_SPEC ((
    CORBA::SystemException
  ))

  {
    //Add your implementation here
    return 0;
  }

void TAO_Notify_ProxyConsumer_i::validate_event_qos (
    const CosNotification::QoSProperties & /* required_qos */,
    CosNotification::NamedPropertyRangeSeq_out /* available_qos */,
    CORBA::Environment & /* ACE_TRY_ENV */
  )
  ACE_THROW_SPEC ((
    CORBA::SystemException,
    CosNotification::UnsupportedQoS
  ))

  {
    //Add your implementation here
  }
