// -*- C++ -*-

//=============================================================================
/**
 *  @file    CIAO_Events.cpp
 *
 *  $Id$
 *
 *  Method definitions for CIAO's event classes
 *
 *  @author George Edwards <g.edwards@vanderbilt.edu>
 */
//=============================================================================

#include "CIAO_Events.h"

/// Initialize the RTEventService object. Will eventually be moved to a separate init ()
/// method so exceptions can be handled properly.
CIAO_Events::RTEventService::RTEventService (CORBA::ORB_ptr orb, RtecEventChannelAdmin::EventChannel_ptr ec) :
  orb_ (CORBA::ORB::_duplicate (orb)),
  rt_event_channel_ (RtecEventChannelAdmin::EventChannel::_duplicate (ec)),
  type_id_ (ACE_ES_EVENT_ANY),
  source_id_ (ACE_ES_EVENT_SOURCE_ANY)
{
  CORBA::Object_var poa_object =
    orb->resolve_initial_references("RootPOA" ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;
  if (CORBA::is_nil (poa_object.in ()))
    ACE_ERROR ((LM_ERROR,
                " (%P|%t) Unable to initialize the POA.\n"));
  this->root_poa_ =
    PortableServer::POA::_narrow (poa_object.in () ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;
}

/// Connect a supplier to the RT event channel.
void CIAO_Events::RTEventService::connect_event_supplier (
    CIAO_Events::Supplier_Config_ptr supplier_config
    ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((
    CORBA::SystemException))
{

  ACE_DEBUG ((LM_DEBUG, "CIAO_Events::RTEventService::connect_event_supplier\n"));

  RtecEventChannelAdmin::SupplierAdmin_var supplier_admin =
    this->rt_event_channel_->for_suppliers (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK;

  this->proxy_consumer_ =
    supplier_admin->obtain_push_consumer (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK;

  // Create and register supplier servant
  CIAO_Events::RTEventServiceSupplier_impl * supplier_servant;
  ACE_NEW (supplier_servant,
           CIAO_Events::RTEventServiceSupplier_impl (orb_.in ()));
  this->push_supplier_ =
    supplier_servant->_this (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK;

  this->proxy_consumer_->connect_push_supplier (this->push_supplier_.in (),
                                                *supplier_config->get_rt_event_qos (ACE_ENV_SINGLE_ARG_PARAMETER)
                                                ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;

}

/// Connect a consumer to the RT event channel.
CIAO_Events::EventServiceInfo CIAO_Events::RTEventService::connect_event_consumer (
    CIAO_Events::Consumer_Config_ptr consumer_config
    ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((
    CORBA::SystemException))
{
  ACE_DEBUG ((LM_DEBUG, "CIAO_Events::RTEventService::connect_event_consumer\n"));

  CIAO_Events::EventServiceInfo service_info;
  service_info.type = RTEC;
  service_info.service = this;

  Components::EventConsumerBase_var consumer =
    consumer_config->get_consumer (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK;

  RtecEventChannelAdmin::ConsumerAdmin_var consumer_admin =
    this->rt_event_channel_->for_consumers (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK;
  RtecEventChannelAdmin::ProxyPushSupplier_var proxy_supplier =
    consumer_admin->obtain_push_supplier (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK;

  // Create and register consumer servant
  CIAO_Events::RTEventServiceConsumer_impl * consumer_servant;
  ACE_NEW_RETURN (consumer_servant,
                  CIAO_Events::RTEventServiceConsumer_impl (orb_.in (), consumer.in ()),
                  service_info);
  RtecEventComm::PushConsumer_var push_consumer =
    consumer_servant->_this (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK;

  proxy_supplier->connect_push_consumer (push_consumer.in (),
                                         *consumer_config->get_rt_event_qos (ACE_ENV_SINGLE_ARG_PARAMETER)
                                         ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;

  service_info.connection.push_consumer = push_consumer._retn ();

  return service_info;
}

void CIAO_Events::RTEventService::disconnect_event_consumer (
    CIAO_Events::EventServiceInfo service_info
    ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((
    CORBA::SystemException,
    Components::InvalidName,
    Components::InvalidConnection))
{
  service_info.connection.push_consumer->disconnect_push_consumer (
    ACE_ENV_SINGLE_ARG_PARAMETER);
	ACE_CHECK;
}

void CIAO_Events::RTEventService::disconnect_event_supplier (
    ACE_ENV_SINGLE_ARG_DECL)
  ACE_THROW_SPEC ((
    CORBA::SystemException,
    Components::InvalidName,
    Components::InvalidConnection))
{
  this->push_supplier_->disconnect_push_supplier (
    ACE_ENV_SINGLE_ARG_PARAMETER);
	ACE_CHECK;
}

void CIAO_Events::RTEventService::push_event (
    ::Components::EventBase *ev
    ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((
    CORBA::SystemException))
{
  ACE_DEBUG ((LM_DEBUG, "CIAO_Events::RTEventService::push_event\n"));

  RtecEventComm::EventSet events (1);
  events.length (1);
  events[0].header.source = this->source_id_;
  events[0].header.type = this->type_id_;
  events[0].data.any_value <<= ev;
  ev->_add_ref ();

  this->proxy_consumer_->push (events ACE_ENV_ARG_PARAMETER);
	ACE_CHECK;
}

/// Nothing to do here.
void CIAO_Events::DirectEventService::connect_event_supplier (
        CIAO_Events::Supplier_Config_ptr supplier_config
        ACE_ENV_ARG_DECL)
      ACE_THROW_SPEC ((
        CORBA::SystemException))
{
}

/// Add a consumer to the map.
CIAO_Events::EventServiceInfo CIAO_Events::DirectEventService::connect_event_consumer (
        CIAO_Events::Consumer_Config_ptr consumer_config
        ACE_ENV_ARG_DECL)
      ACE_THROW_SPEC ((
        CORBA::SystemException))
{
  CIAO_Events::EventServiceInfo service_info;
  service_info.type = RTEC;
  service_info.service = this;

  ACE_NEW_RETURN (service_info.connection.consumer_key, ACE_Active_Map_Manager_Key, service_info);
  Components::EventConsumerBase_var consumer =
    consumer_config->get_consumer (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK;
  this->consumer_map_.bind (consumer._retn (), *service_info.connection.consumer_key);

  return service_info;

}

void CIAO_Events::DirectEventService::disconnect_event_consumer (
        CIAO_Events::EventServiceInfo service_info
        ACE_ENV_ARG_DECL)
      ACE_THROW_SPEC ((
        CORBA::SystemException,
        Components::InvalidName,
        Components::InvalidConnection))
{
}

void CIAO_Events::DirectEventService::disconnect_event_supplier (
        ACE_ENV_ARG_DECL)
      ACE_THROW_SPEC ((
        CORBA::SystemException,
        Components::InvalidName,
        Components::InvalidConnection))
{
}

/// Iterate through the map, pushing the event to each consumer.
void CIAO_Events::DirectEventService::push_event (
        Components::EventBase *ev
        ACE_ENV_ARG_DECL)
      ACE_THROW_SPEC ((
        CORBA::SystemException))
{
  ACE_DEBUG ((LM_DEBUG, "CIAO_Events::DirectEventService::push_event\n"));

  ev->_add_ref ();

  ACE_Active_Map_Manager<Components::EventConsumerBase_ptr>::iterator end =
    this->consumer_map_.end ();

  for (ACE_Active_Map_Manager<Components::EventConsumerBase_ptr>::iterator iter =
    this->consumer_map_.begin ();
    iter != end;
    ++iter)
    {
      ACE_Active_Map_Manager<Components::EventConsumerBase_ptr>::ENTRY &entry = *iter;

      entry.int_id_->push_event (
        ev
        ACE_ENV_ARG_PARAMETER);
      ACE_CHECK;
    }
}

CIAO_Events::RTEvent_Consumer_Config::RTEvent_Consumer_Config (Events_Manager * em) :
  service_type_ (RTEC),
  events_manager_ (em)
{
}

void CIAO_Events::RTEvent_Consumer_Config::set_consumer_id (CONNECTION_ID connection_id ACE_ENV_ARG_DECL)
      ACE_THROW_SPEC ((CORBA::SystemException))
{
  this->consumer_id_ = connection_id;
}

void CIAO_Events::RTEvent_Consumer_Config::set_supplier_id (CONNECTION_ID connection_id ACE_ENV_ARG_DECL)
      ACE_THROW_SPEC ((CORBA::SystemException))
{
  this->supplier_id_ = connection_id;
}

void CIAO_Events::RTEvent_Consumer_Config::set_consumer (Components::EventConsumerBase_ptr consumer ACE_ENV_ARG_DECL)
      ACE_THROW_SPEC ((CORBA::SystemException))
{
  this->consumer_ = consumer;
}

CIAO_Events::CONNECTION_ID CIAO_Events::RTEvent_Consumer_Config::get_consumer_id (ACE_ENV_SINGLE_ARG_DECL)
      ACE_THROW_SPEC ((CORBA::SystemException))
{
  return this->consumer_id_;
}

CIAO_Events::CONNECTION_ID CIAO_Events::RTEvent_Consumer_Config::get_supplier_id (ACE_ENV_SINGLE_ARG_DECL)
      ACE_THROW_SPEC ((CORBA::SystemException))
{
  return this->supplier_id_;
}

CIAO_Events::EventServiceType CIAO_Events::RTEvent_Consumer_Config::get_service_type (ACE_ENV_SINGLE_ARG_DECL)
      ACE_THROW_SPEC ((CORBA::SystemException))
{
  return this->service_type_;
}

Components::EventConsumerBase_ptr CIAO_Events::RTEvent_Consumer_Config::get_consumer (ACE_ENV_SINGLE_ARG_DECL)
      ACE_THROW_SPEC ((CORBA::SystemException))
{
  return this->consumer_;
}

RtecEventChannelAdmin::ConsumerQOS * CIAO_Events::RTEvent_Consumer_Config::get_rt_event_qos (ACE_ENV_SINGLE_ARG_DECL)
      ACE_THROW_SPEC ((CORBA::SystemException))
{
  ACE_DEBUG ((LM_DEBUG, "CIAO_Events::RTEvent_Consumer_Config::get_rt_event_qos\n"));

  ACE_ConsumerQOS_Factory qos;
  qos.start_logical_and_group (2);
  qos.insert_type (this->events_manager_->get_rtec_type_id (this->supplier_id_),
                   0);
  qos.insert_source (this->events_manager_->get_rtec_source_id (this->supplier_id_),
                     0);

  return (RtecEventChannelAdmin::ConsumerQOS *) &qos.get_ConsumerQOS ();
}

CIAO_Events::RTEvent_Supplier_Config::RTEvent_Supplier_Config (Events_Manager * em) :
  service_type_ (RTEC),
  events_manager_ (em)
{
}

void CIAO_Events::RTEvent_Supplier_Config::set_supplier_id (CONNECTION_ID connection_id ACE_ENV_ARG_DECL)
      ACE_THROW_SPEC ((CORBA::SystemException))
{
  this->supplier_id_ = connection_id;
}

CIAO_Events::CONNECTION_ID CIAO_Events::RTEvent_Supplier_Config::get_supplier_id (ACE_ENV_SINGLE_ARG_DECL)
      ACE_THROW_SPEC ((CORBA::SystemException))
{
  return this->supplier_id_;
}

CIAO_Events::EventServiceType CIAO_Events::RTEvent_Supplier_Config::get_service_type (ACE_ENV_SINGLE_ARG_DECL)
      ACE_THROW_SPEC ((CORBA::SystemException))
{
  return this->service_type_;
}

RtecEventChannelAdmin::SupplierQOS * CIAO_Events::RTEvent_Supplier_Config::get_rt_event_qos (ACE_ENV_SINGLE_ARG_DECL)
      ACE_THROW_SPEC ((CORBA::SystemException))
{

  ACE_DEBUG ((LM_DEBUG, "CIAO_Events::RTEvent_Supplier_Config::get_rt_event_qos\n"));

  ACE_SupplierQOS_Factory qos;
  qos.insert (this->events_manager_->get_rtec_type_id (this->supplier_id_),
              this->events_manager_->get_rtec_source_id (this->supplier_id_),
              0,
              1);

  return (RtecEventChannelAdmin::SupplierQOS *) &qos.get_SupplierQOS ();
}

CIAO_Events::Direct_Consumer_Config::Direct_Consumer_Config (Events_Manager * em) :
  service_type_ (DIRECT),
  events_manager_ (em)
{
}

void CIAO_Events::Direct_Consumer_Config::set_consumer_id (CONNECTION_ID connection_id ACE_ENV_ARG_DECL)
      ACE_THROW_SPEC ((CORBA::SystemException))
{
  this->consumer_id_ = connection_id;
}

void CIAO_Events::Direct_Consumer_Config::set_supplier_id (CONNECTION_ID connection_id ACE_ENV_ARG_DECL)
      ACE_THROW_SPEC ((CORBA::SystemException))
{
  this->supplier_id_ = connection_id;
}

void CIAO_Events::Direct_Consumer_Config::set_consumer (Components::EventConsumerBase_ptr consumer ACE_ENV_ARG_DECL)
      ACE_THROW_SPEC ((CORBA::SystemException))
{
  this->consumer_ = consumer;
}

CIAO_Events::CONNECTION_ID CIAO_Events::Direct_Consumer_Config::get_consumer_id (ACE_ENV_SINGLE_ARG_DECL)
      ACE_THROW_SPEC ((CORBA::SystemException))
{
  return this->consumer_id_;
}

CIAO_Events::CONNECTION_ID CIAO_Events::Direct_Consumer_Config::get_supplier_id (ACE_ENV_SINGLE_ARG_DECL)
      ACE_THROW_SPEC ((CORBA::SystemException))
{
  return this->supplier_id_;
}

CIAO_Events::EventServiceType CIAO_Events::Direct_Consumer_Config::get_service_type (ACE_ENV_SINGLE_ARG_DECL)
      ACE_THROW_SPEC ((CORBA::SystemException))
{
  return this->service_type_;
}

Components::EventConsumerBase_ptr CIAO_Events::Direct_Consumer_Config::get_consumer (ACE_ENV_SINGLE_ARG_DECL)
      ACE_THROW_SPEC ((CORBA::SystemException))
{
  return this->consumer_;
}

RtecEventChannelAdmin::ConsumerQOS * CIAO_Events::Direct_Consumer_Config::get_rt_event_qos (ACE_ENV_SINGLE_ARG_DECL)
      ACE_THROW_SPEC ((CORBA::SystemException))
{
  return 0;
}

CIAO_Events::Direct_Supplier_Config::Direct_Supplier_Config (Events_Manager * em) :
  service_type_ (DIRECT),
  events_manager_ (em)
{
}

void CIAO_Events::Direct_Supplier_Config::set_supplier_id (CONNECTION_ID connection_id ACE_ENV_ARG_DECL)
      ACE_THROW_SPEC ((CORBA::SystemException))
{
  this->supplier_id_ = connection_id;
}

CIAO_Events::CONNECTION_ID CIAO_Events::Direct_Supplier_Config::get_supplier_id (ACE_ENV_SINGLE_ARG_DECL)
      ACE_THROW_SPEC ((CORBA::SystemException))
{
  return this->supplier_id_;
}

CIAO_Events::EventServiceType CIAO_Events::Direct_Supplier_Config::get_service_type (ACE_ENV_SINGLE_ARG_DECL)
      ACE_THROW_SPEC ((CORBA::SystemException))
{
  return this->service_type_;
}

RtecEventChannelAdmin::SupplierQOS * CIAO_Events::Direct_Supplier_Config::get_rt_event_qos (ACE_ENV_SINGLE_ARG_DECL)
      ACE_THROW_SPEC ((CORBA::SystemException))
{
  return 0;
}

/// Initialize the Events_Manager
CIAO_Events::Events_Manager::Events_Manager (CORBA::ORB_ptr orb) :
  orb_ (CORBA::ORB::_duplicate (orb)),
  rt_event_channel_ (RtecEventChannelAdmin::EventChannel::_nil ())
{
  CORBA::Object_var poa_object =
    orb->resolve_initial_references("RootPOA" ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;
  if (CORBA::is_nil (poa_object.in ()))
    ACE_ERROR ((LM_ERROR,
                " (%P|%t) Unable to initialize the POA.\n"));
  this->root_poa_ =
    PortableServer::POA::_narrow (poa_object.in () ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;
}

CIAO_Events::Consumer_Config_ptr CIAO_Events::Events_Manager::create_consumer_config (const char * service_type)
{
  if (ACE_OS::strcmp (service_type, "DIRECT") == 0)
  {
    CIAO_Events::Direct_Consumer_Config * consumer_config = 0;
    ACE_NEW_RETURN (consumer_config, CIAO_Events::Direct_Consumer_Config (this), 0);
    CIAO_Events::Consumer_Config_var return_config =
      consumer_config->_this (ACE_ENV_SINGLE_ARG_PARAMETER);
    ACE_CHECK;
    return return_config._retn ();
  }
  else if (ACE_OS::strcmp (service_type, "RTEC") == 0)
  {
    CIAO_Events::RTEvent_Consumer_Config * consumer_config = 0;
    ACE_NEW_RETURN (consumer_config, CIAO_Events::RTEvent_Consumer_Config (this), 0);
    CIAO_Events::Consumer_Config_var return_config =
      consumer_config->_this (ACE_ENV_SINGLE_ARG_PARAMETER);
    ACE_CHECK;
    return return_config._retn ();
  }

  return 0;

  /*
  else if (ACE_OS::strcmp (service_name, "NS") == 0)
  {
    service_info.type = NS;
    if (this->notify_service_ == 0)
      {
        ACE_NEW_RETURN (this->notify_service_, CIAO_NotificationService, 0);
      }
    service_info.service = this->notify_service_;
  }
  else if (ACE_OS::strcmp (service_name, "RTNS") == 0)
  {
    service_info.type = RTNS;
    if (this->rt_notify_service_ == 0)
      {
        ACE_NEW_RETURN (this->rt_notify_service_, CIAO_RTNotificationService, 0);
      }
    service_info.service = this->rt_notify_service_;
  }
  else if (ACE_OS::strcmp (service_name, "DIRECT") == 0)
  {
    service_info.type = DIRECT;
    if (this->direct_event_service_ == 0)
      {
        ACE_NEW_RETURN (this->direct_event_service_, CIAO_DirectEventService, 0);
      }
    service_info.service = this->direct_event_service_;
  }
  */

}

CIAO_Events::Supplier_Config_ptr CIAO_Events::Events_Manager::create_supplier_config (const char * service_type)
{

  if (ACE_OS::strcmp (service_type, "DIRECT") == 0)
  {
    CIAO_Events::Direct_Supplier_Config * supplier_config;
    ACE_NEW_RETURN (supplier_config, CIAO_Events::Direct_Supplier_Config (this), 0);
    CIAO_Events::Supplier_Config_var return_config =
      supplier_config->_this (ACE_ENV_SINGLE_ARG_PARAMETER);
    ACE_CHECK;
    return return_config._retn ();
  }
  else if (ACE_OS::strcmp (service_type, "RTEC") == 0)
  {
    CIAO_Events::RTEvent_Supplier_Config * supplier_config;
    ACE_NEW_RETURN (supplier_config, CIAO_Events::RTEvent_Supplier_Config (this), 0);
    CIAO_Events::Supplier_Config_var return_config =
      supplier_config->_this (ACE_ENV_SINGLE_ARG_PARAMETER);
    ACE_CHECK;
    return return_config._retn ();
  }

  return 0;
}

CIAO_Events::EventServiceBase * CIAO_Events::Events_Manager::create_supplier (Supplier_Config_ptr supplier_config)
{

  ACE_DEBUG ((LM_DEBUG, "CIAO_Events::Events_Manager::create_supplier\n"));

  CIAO_Events::EventServiceBase * event_service = 0;

  switch (supplier_config->get_service_type ())
  {
  case DIRECT:
    ACE_NEW_RETURN (event_service, CIAO_Events::DirectEventService, 0);
    break;
  case RTEC:
    if (CORBA::is_nil (this->rt_event_channel_.in ()))
      {
        this->create_rt_event_channel (ACE_ENV_SINGLE_ARG_DECL);
        ACE_CHECK;
      }
    ACE_NEW_RETURN (event_service, CIAO_Events::RTEventService (orb_.in (), rt_event_channel_.in ()), 0);
    break;
  }

  return event_service;
}

RtecEventComm::EventType CIAO_Events::Events_Manager::get_rtec_type_id (CONNECTION_ID connection_id)
{

  RtecEventComm::EventType event_type_id;

  if (this->event_types_map_.find (connection_id, event_type_id) == -1)
    {
      event_type_id =
        ACE_ES_EVENT_ANY + 1 + this->event_types_map_.current_size ();
      this->event_types_map_.bind (connection_id, event_type_id);
    }

  return event_type_id;
}

RtecEventComm::EventSourceID CIAO_Events::Events_Manager::get_rtec_source_id (CONNECTION_ID connection_id)
{

  RtecEventComm::EventSourceID event_source_id;

  if (this->publishers_map_.find (connection_id, event_source_id) == -1)
    {
      event_source_id =
        ACE_ES_EVENT_SOURCE_ANY + 1 + this->publishers_map_.current_size ();
      this->publishers_map_.bind (connection_id, event_source_id);
    }

  return event_source_id;
}

void CIAO_Events::Events_Manager::create_rt_event_channel (
    ACE_ENV_SINGLE_ARG_DECL)
  ACE_THROW_SPEC ((
    CORBA::SystemException))
{
  ACE_DEBUG ((LM_DEBUG, "CIAO_Events::Events_Manager::create_rt_event_channel\n"));

  TAO_EC_Event_Channel_Attributes attributes (this->root_poa_.in (),
                                              this->root_poa_.in ());
  TAO_EC_Event_Channel * ec_servant;
  ACE_NEW (ec_servant, TAO_EC_Event_Channel (attributes));
  ec_servant->activate (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK;
  this->rt_event_channel_ = ec_servant->_this (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK;
}

CIAO_Events::RTEventServiceSupplier_impl::RTEventServiceSupplier_impl (void)
{
}

CIAO_Events::RTEventServiceSupplier_impl::RTEventServiceSupplier_impl (CORBA::ORB_ptr orb) :
  orb_ (CORBA::ORB::_duplicate (orb))
{
}
  
void CIAO_Events::RTEventServiceSupplier_impl::disconnect_push_supplier (void)
{
  CORBA::Object_var poa_object =
    orb_->resolve_initial_references ("RootPOA" ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;
  PortableServer::POA_var root_poa =
    PortableServer::POA::_narrow (poa_object.in () ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;
  if (CORBA::is_nil (root_poa.in ()))
    ACE_ERROR ((LM_ERROR, "Nil RootPOA\n"));
  PortableServer::ObjectId_var oid = root_poa->servant_to_id (this);
  root_poa->deactivate_object (oid);
}

CIAO_Events::RTEventServiceConsumer_impl::RTEventServiceConsumer_impl (void)
{
}

CIAO_Events::RTEventServiceConsumer_impl::RTEventServiceConsumer_impl
  (CORBA::ORB_ptr orb,
   Components::EventConsumerBase_ptr consumer) :
    orb_ (CORBA::ORB::_duplicate (orb)),
    event_consumer_ (Components::EventConsumerBase::_duplicate (consumer))
{
}

void CIAO_Events::RTEventServiceConsumer_impl::push (const RtecEventComm::EventSet& events)
{
  ACE_DEBUG ((LM_DEBUG, "CIAO::RTEventServiceConsumer_impl::push\n"));
  for (unsigned long i = 0; i < events.length (); ++i)
    {
      ::Components::EventBase * ev;
      if (events[i].data.any_value >>= ev)
        {
          this->event_consumer_->push_event (ev
                                             ACE_ENV_ARG_PARAMETER);
          ACE_CHECK;
        }
    }
}

void CIAO_Events::RTEventServiceConsumer_impl::disconnect_push_consumer (void)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  ACE_DEBUG ((LM_DEBUG, "CIAO::RTEventServiceConsumer_impl::disconnect_push_consumer\n"));

  CORBA::Object_var poa_object =
    orb_->resolve_initial_references ("RootPOA" ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;
  PortableServer::POA_var root_poa =
    PortableServer::POA::_narrow (poa_object.in () ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;
  if (CORBA::is_nil (root_poa.in ()))
    ACE_ERROR ((LM_ERROR, "Nil RootPOA\n"));
  PortableServer::ObjectId_var oid = root_poa->servant_to_id (this);
  root_poa->deactivate_object (oid);
}
