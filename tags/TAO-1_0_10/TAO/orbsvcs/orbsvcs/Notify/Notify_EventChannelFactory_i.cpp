/* -*- C++ -*- $Id$ */
#include "Notify_EventChannelFactory_i.h"
#include "Notify_EventChannel_i.h"
#include "ace/Auto_Ptr.h"

// Implementation skeleton constructor
TAO_Notify_EventChannelFactory_i::TAO_Notify_EventChannelFactory_i (void)
  {
  }

// Implementation skeleton destructor
TAO_Notify_EventChannelFactory_i::~TAO_Notify_EventChannelFactory_i (void)
  {
  }

void
TAO_Notify_EventChannelFactory_i::activate (CORBA::Environment &ACE_TRY_ENV)
{
  this->self_ = _this (ACE_TRY_ENV);
  ACE_CHECK;
}

CosNotifyChannelAdmin::EventChannelFactory_ptr
TAO_Notify_EventChannelFactory_i::get_refx (CORBA::Environment &ACE_TRY_ENV)
{
  return _this (ACE_TRY_ENV);
}

CosNotifyChannelAdmin::EventChannel_ptr
TAO_Notify_EventChannelFactory_i::create_channel
(
 const CosNotification::QoSProperties & /* initial_qos */,
 const CosNotification::AdminProperties & /* initial_admin */,
 CosNotifyChannelAdmin::ChannelID_out /* id */,
 CORBA::Environment &ACE_TRY_ENV
 )
  ACE_THROW_SPEC ((
                   CORBA::SystemException,
                   CosNotification::UnsupportedQoS,
                   CosNotification::UnsupportedAdmin
                   ))
  {
    // TODO: Check initial_qos
    // TODO: check initial_admin
    // TODO: fill in id

    TAO_Notify_EventChannel_i* channel;
    CosNotifyChannelAdmin::EventChannel_var ec_ret;

    ACE_NEW_THROW_EX (channel,
                      TAO_Notify_EventChannel_i (*this),
                      CORBA::NO_MEMORY ());

    auto_ptr<TAO_Notify_EventChannel_i> auto_channel (channel);

    channel->init (ACE_TRY_ENV);
    ACE_CHECK_RETURN (CosNotifyChannelAdmin::EventChannel::_nil ());

    ec_ret = channel->get_ref (ACE_TRY_ENV);
    ACE_CHECK_RETURN (CosNotifyChannelAdmin::EventChannel::_nil ());

    auto_channel.release ();
    return ec_ret._retn ();
  }

CosNotifyChannelAdmin::ChannelIDSeq * TAO_Notify_EventChannelFactory_i::get_all_channels (CORBA::Environment & /* ACE_TRY_ENV */
  )
  ACE_THROW_SPEC ((
    CORBA::SystemException
  ))

  {
    //Add your implementation here
    return 0;
  }

CosNotifyChannelAdmin::EventChannel_ptr TAO_Notify_EventChannelFactory_i::get_event_channel (CosNotifyChannelAdmin::ChannelID /* id */,
              CORBA::Environment & /*ACE_TRY_ENV */
  )
  ACE_THROW_SPEC ((
    CORBA::SystemException,
    CosNotifyChannelAdmin::ChannelNotFound
  ))

  {
    //Add your implementation here
    return 0;
  }
