/* -*- C++ -*- $Id$ */

#include "orbsvcs/orbsvcs/Notify/Notify_QoSAdmin_i.h"

// Implementation skeleton constructor
TAO_Notify_QoSAdmin_i::TAO_Notify_QoSAdmin_i (void)
  {
  }

// Implementation skeleton destructor
TAO_Notify_QoSAdmin_i::~TAO_Notify_QoSAdmin_i (void)
  {
  }

CosNotification::QoSProperties * TAO_Notify_QoSAdmin_i::get_qos (
                                                                 CORBA::Environment & ///ACE_TRY_ENV
                                                                 )
  ACE_THROW_SPEC ((
                   CORBA::SystemException
                   ))
{
  //Add your implementation here
  return 0;
}

void TAO_Notify_QoSAdmin_i::set_qos (
                                     const CosNotification::QoSProperties & /*qos*/,
                                     CORBA::Environment & //ACE_TRY_ENV
  )
  ACE_THROW_SPEC ((
    CORBA::SystemException,
    CosNotification::UnsupportedQoS
  ))
  {
    //Add your implementation here
    return;
  }

void TAO_Notify_QoSAdmin_i::validate_qos (
                                          const CosNotification::QoSProperties & /*required_qos*/,
                                          CosNotification::NamedPropertyRangeSeq_out /*available_qos*/,
                                          CORBA::Environment & //ACE_TRY_ENV
  )
  ACE_THROW_SPEC ((
    CORBA::SystemException,
    CosNotification::UnsupportedQoS
  ))
  {
    //Add your implementation here
    return;
  }
