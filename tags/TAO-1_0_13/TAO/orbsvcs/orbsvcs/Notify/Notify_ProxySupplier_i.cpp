/* -*- C++ -*- $Id$ */
#include "orbsvcs/orbsvcs/Notify/Notify_ProxySupplier_i.h"
#include "orbsvcs/orbsvcs/Notify/Notify_ConsumerAdmin_i.h"

// Implementation skeleton constructor
TAO_Notify_ProxySupplier_i::TAO_Notify_ProxySupplier_i
(TAO_Notify_ConsumerAdmin_i &myadmin)
  :myadmin_(myadmin)
{
}

// Implementation skeleton destructor
TAO_Notify_ProxySupplier_i::~TAO_Notify_ProxySupplier_i (void)
{
}

void
TAO_Notify_ProxySupplier_i::init (CORBA::Environment & /*ACE_TRY_ENV*/)
{
}

CosNotifyChannelAdmin::ProxyType TAO_Notify_ProxySupplier_i::MyType (
                                                                     CORBA::Environment & //ACE_TRY_ENV
                                                                     )
  ACE_THROW_SPEC ((
    CORBA::SystemException
  ))

  {
    //Add your implementation here
    return (CosNotifyChannelAdmin::ProxyType) 0;
  }

CosNotifyChannelAdmin::ConsumerAdmin_ptr TAO_Notify_ProxySupplier_i::MyAdmin (
                                                                              CORBA::Environment & //ACE_TRY_ENV
                                                                              )
  ACE_THROW_SPEC ((
    CORBA::SystemException
  ))

  {
    //Add your implementation here
    return 0;
  }

CosNotifyFilter::MappingFilter_ptr TAO_Notify_ProxySupplier_i::priority_filter (
                                                                                CORBA::Environment & /*ACE_TRY_ENV*/
  )
  ACE_THROW_SPEC ((
    CORBA::SystemException
  ))

  {
    //Add your implementation here
    return 0;
  }

void TAO_Notify_ProxySupplier_i::priority_filter (
                                                  CosNotifyFilter::MappingFilter_ptr /*priority_filter*/,
                                                  CORBA::Environment & //ACE_TRY_ENV
  )
  ACE_THROW_SPEC ((
    CORBA::SystemException
  ))

  {
    //Add your implementation here
  }

CosNotifyFilter::MappingFilter_ptr TAO_Notify_ProxySupplier_i::lifetime_filter (
                                                                                CORBA::Environment & //ACE_TRY_ENV
                                                                                )
  ACE_THROW_SPEC ((
                   CORBA::SystemException
                   ))

  {
    //Add your implementation here
    return 0;
  }

void TAO_Notify_ProxySupplier_i::lifetime_filter (
                                                  CosNotifyFilter::MappingFilter_ptr /*lifetime_filter */,
                                                  CORBA::Environment & //ACE_TRY_ENV
                                                  )
  ACE_THROW_SPEC ((
    CORBA::SystemException
  ))

  {
    //Add your implementation here
  }

CosNotification::EventTypeSeq*
TAO_Notify_ProxySupplier_i::obtain_offered_types (
                                                  CosNotifyChannelAdmin::ObtainInfoMode /*mode */,
                                                  CORBA::Environment & //ACE_TRY_ENV
  )
  ACE_THROW_SPEC ((
                   CORBA::SystemException
                   ))
  
{
  //Add your implementation here
  return 0;
}

void TAO_Notify_ProxySupplier_i::validate_event_qos (
                                                     const CosNotification::QoSProperties & /*required_qos */,
                                                     CosNotification::NamedPropertyRangeSeq_out /*available_qos*/,
                                                     CORBA::Environment & //ACE_TRY_ENV
                                                     )
  ACE_THROW_SPEC ((
                   CORBA::SystemException,
                   CosNotification::UnsupportedQoS
                   ))

  {
    //Add your implementation here
  }
