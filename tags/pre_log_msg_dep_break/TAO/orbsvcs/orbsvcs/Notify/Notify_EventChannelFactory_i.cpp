// $Id$

#include "tao/Servant_Base.h"
#include "tao/POAC.h"
#include "Notify_EventChannelFactory_i.h"
#include "Notify_EventChannel_i.h"
#include "Notify_Resource_Manager.h"

ACE_RCSID(Notify, Notify_EventChannelFactory_i, "$Id$")

TAO_Notify_EventChannelFactory_i::TAO_Notify_EventChannelFactory_i (void)
{
}

TAO_Notify_EventChannelFactory_i::~TAO_Notify_EventChannelFactory_i (void)
{
  this->cleanup_i ();
}

CosNotifyChannelAdmin::EventChannelFactory_ptr
TAO_Notify_EventChannelFactory_i::create (PortableServer::POA_ptr default_POA, CORBA::Environment &ACE_TRY_ENV)
{
  TAO_Notify_EventChannelFactory_i* channelfactory;
  ACE_NEW_THROW_EX (channelfactory,
                    TAO_Notify_EventChannelFactory_i (),
                    CORBA::NO_MEMORY ());
  ACE_CHECK_RETURN (CosNotifyChannelAdmin::EventChannelFactory::_nil ());

  PortableServer::ServantBase_var channelfactory_var (channelfactory);

  channelfactory->init_i (default_POA, ACE_TRY_ENV);
  ACE_CHECK_RETURN (CosNotifyChannelAdmin::EventChannelFactory::_nil ());

  CORBA::Object_var obj = channelfactory->resource_manager_->
    activate_object (channelfactory->my_POA_.in (),
                     channelfactory, ACE_TRY_ENV);
  ACE_CHECK_RETURN (CosNotifyChannelAdmin::EventChannelFactory::_nil ());

  return CosNotifyChannelAdmin::EventChannelFactory
    ::_narrow (obj.in (), ACE_TRY_ENV);
}

void
TAO_Notify_EventChannelFactory_i::init_i (PortableServer::POA_ptr default_POA, CORBA::Environment &ACE_TRY_ENV)
{
  this->my_POA_ = PortableServer::POA::_duplicate (default_POA);

  this->resource_manager_ =
    TAO_Notify_Resource_Manager::create (default_POA,
                                         ACE_TRY_ENV);
  ACE_CHECK;

  // Which POA should we activate the Event Channels in?
  this->ec_POA_ = this->resource_manager_->
    create_event_channel_POA (this->my_POA_.in (), ACE_TRY_ENV);
}

CosNotifyChannelAdmin::EventChannelFactory_ptr
TAO_Notify_EventChannelFactory_i::get_ref (CORBA::Environment &ACE_TRY_ENV)
{
  return CosNotifyChannelAdmin::EventChannelFactory
    ::_narrow (this->resource_manager_->
               servant_to_reference (this->my_POA_.in (), this, ACE_TRY_ENV));
}

void
TAO_Notify_EventChannelFactory_i::event_channel_destroyed (CosNotifyChannelAdmin::ChannelID channel_id)
{
  this->ec_ids_.put (channel_id);
}

void
TAO_Notify_EventChannelFactory_i::cleanup_i (void)
{
  if (this->resource_manager_ != 0)
      this->resource_manager_->destroy_POA (this->ec_POA_.in ());

  delete this->resource_manager_;

  my_POA_ = PortableServer::POA::_nil ();
  ec_POA_ = PortableServer::POA::_nil ();
  this->resource_manager_ = 0;
}

CosNotifyChannelAdmin::EventChannel_ptr
TAO_Notify_EventChannelFactory_i::create_channel(const CosNotification::QoSProperties& initial_qos,
                                                 const CosNotification::AdminProperties& initial_admin,
                                                 CosNotifyChannelAdmin::ChannelID_out ec_id,
                                                 CORBA::Environment &ACE_TRY_ENV
                                                 )
  ACE_THROW_SPEC ((
                   CORBA::SystemException,
                   CosNotification::UnsupportedQoS,
                   CosNotification::UnsupportedAdmin
                   ))
{
  TAO_Notify_EventChannel_i* channel =
    this->resource_manager_->create_event_channel (this,
                                                   ACE_TRY_ENV);
  ACE_CHECK_RETURN (CosNotifyChannelAdmin::EventChannel::_nil ());

  PortableServer::ServantBase_var channel_var (channel);

  ec_id = this->ec_ids_.get ();

  channel->init (ec_id, initial_qos, initial_admin, this->ec_POA_.in (),
                 ACE_TRY_ENV);
  ACE_CHECK_RETURN (CosNotifyChannelAdmin::EventChannel::_nil ());

  CORBA::Object_var obj = this->resource_manager_->
    activate_object_with_id (ec_id,
                             this->ec_POA_.in (),
                             channel,
                             ACE_TRY_ENV);
  ACE_CHECK_RETURN (CosNotifyChannelAdmin::EventChannel::_nil ());

  this->ec_ids_.next ();

  return CosNotifyChannelAdmin::EventChannel::_narrow (obj.in ());
}

CosNotifyChannelAdmin::ChannelIDSeq*
TAO_Notify_EventChannelFactory_i::get_all_channels (CORBA::Environment & ACE_TRY_ENV)
  ACE_THROW_SPEC ((
                   CORBA::SystemException
                   ))
{
  return this->ec_ids_.get_sequence (ACE_TRY_ENV);
}

CosNotifyChannelAdmin::EventChannel_ptr
TAO_Notify_EventChannelFactory_i::get_event_channel (CosNotifyChannelAdmin::ChannelID id, CORBA::Environment & ACE_TRY_ENV)
  ACE_THROW_SPEC ((
                   CORBA::SystemException,
                   CosNotifyChannelAdmin::ChannelNotFound
                   ))
{
  CORBA::Object_var obj;

  ACE_TRY
    {
      obj = this->resource_manager_->id_to_reference (id, this->ec_POA_.in (),
                                                      ACE_TRY_ENV);
      ACE_TRY_CHECK;
    }
  ACE_CATCHANY // Translate any exception to "not found"
    {
      ACE_TRY_THROW (CosNotifyChannelAdmin::ChannelNotFound ());
    }
  ACE_ENDTRY;
  ACE_CHECK_RETURN (CosNotifyChannelAdmin::EventChannel::_nil ());

  return CosNotifyChannelAdmin::EventChannel::_narrow (obj.in (), ACE_TRY_ENV);
}

#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)
template class TAO_Notify_ID_Pool_Ex<CosNotifyChannelAdmin::ChannelID, CosNotifyChannelAdmin::ChannelIDSeq>;

#elif defined (ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)
#pragma instantiate TAO_Notify_ID_Pool_Ex<CosNotifyChannelAdmin::ChannelID, CosNotifyChannelAdmin::ChannelIDSeq>;

#endif /*ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */
