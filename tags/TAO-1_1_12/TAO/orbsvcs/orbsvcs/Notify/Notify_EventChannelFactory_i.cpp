// $Id$

#include "Notify_EventChannelFactory_i.h"
#include "Notify_EventChannel_i.h"
#include "Notify_Channel_Objects_Factory.h"
#include "Notify_POA_Factory.h"
#include "Notify_Factory.h"
#include "Notify_FilterFactory_i.h"
#include "Notify_Event.h"

ACE_RCSID(Notify, Notify_EventChannelFactory_i, "$Id$")

TAO_Notify_EventChannelFactory_i::TAO_Notify_EventChannelFactory_i (void)
{
}

TAO_Notify_EventChannelFactory_i::~TAO_Notify_EventChannelFactory_i (void)
{
  TAO_Notify_Factory::shutdown ();
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

  CORBA::Object_var obj = channelfactory->poa_factory_->
    activate_object (channelfactory->my_POA_.in (),
                     channelfactory, ACE_TRY_ENV);
  ACE_CHECK_RETURN (CosNotifyChannelAdmin::EventChannelFactory::_nil ());

  return CosNotifyChannelAdmin::EventChannelFactory
    ::_narrow (obj.in (), ACE_TRY_ENV);
}

void
TAO_Notify_EventChannelFactory_i::init_i (PortableServer::POA_ptr default_POA, CORBA::Environment &ACE_TRY_ENV)
{
  TAO_Notify_Factory::init (ACE_TRY_ENV);
  ACE_CHECK;

  this->my_POA_ = PortableServer::POA::_duplicate (default_POA);

  this->channel_objects_factory_ =
    TAO_Notify_Factory::get_channel_objects_factory ();

  this->poa_factory_ = TAO_Notify_Factory::get_poa_factory ();

  // The POA that we should activate the Event Channels in.
  this->ec_POA_ = this->poa_factory_->
    create_event_channel_POA (this->my_POA_.in (), ACE_TRY_ENV);
  ACE_CHECK;

  this->lock_ =
    this->channel_objects_factory_->create_channel_factory_lock (ACE_TRY_ENV);
  ACE_CHECK;

  this->default_filter_factory_ =
    this->create_default_filter_factory_i (ACE_TRY_ENV);
}

CosNotifyChannelAdmin::EventChannelFactory_ptr
TAO_Notify_EventChannelFactory_i::get_ref (CORBA::Environment &ACE_TRY_ENV)
{
  return CosNotifyChannelAdmin::EventChannelFactory
    ::_narrow (this->poa_factory_->
               servant_to_reference (this->my_POA_.in (), this, ACE_TRY_ENV));
}

void
TAO_Notify_EventChannelFactory_i::event_channel_destroyed (CosNotifyChannelAdmin::ChannelID channel_id)
{
  ACE_DEBUG ((LM_DEBUG, "event_channel_destroyed %d\n", channel_id));
}

void TAO_Notify_EventChannelFactory_i::shutdown (CORBA::Environment &ACE_TRY_ENV, CORBA::Boolean destroy_children)
{
  if (destroy_children == 1)
    {
      this->poa_factory_->destroy_POA (this->ec_POA_.in (), ACE_TRY_ENV);

      this->poa_factory_->deactivate_object(this->default_filter_factory_.in (),
                                            this->my_POA_.in (),
                                            ACE_TRY_ENV);
    }
  // Deactivate ourselves.
  this->poa_factory_->deactivate_object (this,
                                         this->my_POA_.in (),
                                         ACE_TRY_ENV);
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
    this->channel_objects_factory_->create_event_channel (this,
                                                          ACE_TRY_ENV);
  ACE_CHECK_RETURN (CosNotifyChannelAdmin::EventChannel::_nil ());

  PortableServer::ServantBase_var channel_var (channel);

  {
    ACE_GUARD_THROW_EX (ACE_Lock, ace_mon, *this->lock_,
                        CORBA::INTERNAL ());
    ACE_CHECK_RETURN (CosNotifyChannelAdmin::EventChannel::_nil ());

    ec_id = this->ec_ids_.get ();

    ACE_DEBUG ((LM_DEBUG, "event_channel created %d\n", ec_id));
  }

  channel->init (ec_id, initial_qos, initial_admin, this->my_POA_.in (), this->ec_POA_.in (),
                 ACE_TRY_ENV);
  ACE_CHECK_RETURN (CosNotifyChannelAdmin::EventChannel::_nil ());

  CORBA::Object_var obj = this->poa_factory_->
    activate_object_with_id (ec_id, this->ec_POA_.in (), channel,
                             ACE_TRY_ENV);
  ACE_CHECK_RETURN (CosNotifyChannelAdmin::EventChannel::_nil ());

  return CosNotifyChannelAdmin::EventChannel::_narrow (obj.in ());
}

CosNotifyChannelAdmin::ChannelIDSeq*
TAO_Notify_EventChannelFactory_i::get_all_channels (CORBA::Environment & ACE_TRY_ENV)
  ACE_THROW_SPEC ((
                   CORBA::SystemException
                   ))
{
  ACE_GUARD_RETURN (ACE_Lock, ace_mon, *this->lock_, 0);

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
      obj = this->poa_factory_->id_to_reference (id, this->ec_POA_.in (),
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

CosNotifyFilter::FilterFactory_ptr
TAO_Notify_EventChannelFactory_i::create_default_filter_factory_i (CORBA::Environment& ACE_TRY_ENV)
{
  TAO_Notify_FilterFactory_i* filterfactory =
    this->channel_objects_factory_->create_filter_factory (ACE_TRY_ENV);
  ACE_CHECK_RETURN (CosNotifyFilter::FilterFactory::_nil ());

  PortableServer::ServantBase_var filterfactory_var (filterfactory);

  // Init goes here.
  // ACE_CHECK_RETURN (CosNotifyFilter::FilterFactory::_nil ());

  CORBA::Object_var obj =
    this->poa_factory_->activate_object (this->my_POA_.in (),
                                         filterfactory, ACE_TRY_ENV);
  ACE_CHECK_RETURN (CosNotifyFilter::FilterFactory::_nil ());

  return CosNotifyFilter::FilterFactory::_narrow (obj.in ());
}

CosNotifyFilter::FilterFactory_ptr
TAO_Notify_EventChannelFactory_i::get_default_filter_factory (void)
{
  return CosNotifyFilter::FilterFactory::
    _duplicate (this->default_filter_factory_.in ());
}

#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)
template class TAO_Notify_ID_Pool_Ex<CosNotifyChannelAdmin::ChannelID, CosNotifyChannelAdmin::ChannelIDSeq>;

#elif defined (ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)
#pragma instantiate TAO_Notify_ID_Pool_Ex<CosNotifyChannelAdmin::ChannelID, CosNotifyChannelAdmin::ChannelIDSeq>;

#endif /*ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */
