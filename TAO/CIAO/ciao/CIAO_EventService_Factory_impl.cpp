// -*- C++ -*-

//=============================================================================
/**
 *  @file CIAO_EventService_Factory_impl.cpp
 *
 *  $Id$
 *
 *  @author George Edwards <g.edwards@vanderbilt.edu>
 */
//=============================================================================

#include "CIAO_EventService_Factory_impl.h"
#include "CIAO_DirectEvent.h"
#include "CIAO_RTEvent.h"
#include "CIAO_CosNotify.h"

#include "orbsvcs/Event/EC_Event_Channel.h"

/// Initialize the RTEventService object. Will eventually be moved to a separate init ()
/// method so exceptions can be handled properly.

// @@ George, an init () method in the base class sounds like a better option.

namespace CIAO
{

  EventService_Factory_impl::EventService_Factory_impl (void) :
    rt_event_channel_ (RtecEventChannelAdmin::EventChannel::_nil ()),
    notify_channel_ (CosNotifyChannelAdmin::EventChannel::_nil ())
  {

    /*this->orb_ = CORBA::ORB_init (ACE_ENV_SINGLE_ARG_PARAMETER);
    ACE_CHECK;

    CORBA::Object_var poa_object =
      this->orb_->resolve_initial_references("RootPOA" ACE_ENV_ARG_PARAMETER);
    ACE_CHECK;
    if (CORBA::is_nil (poa_object.in ()))
      ACE_ERROR ((LM_ERROR,
                  " (%P|%t) Unable to initialize the POA.\n"));
    this->root_poa_ =
      PortableServer::POA::_narrow (poa_object.in () ACE_ENV_ARG_PARAMETER);
    ACE_CHECK;

    PortableServer::POAManager_var poa_manager =
      root_poa->the_POAManager (ACE_ENV_SINGLE_ARG_PARAMETER);
    ACE_TRY_CHECK;

    poa_manager->activate (ACE_ENV_SINGLE_ARG_PARAMETER);
    ACE_TRY_CHECK;
    */

  }

  EventService_Factory_impl::~EventService_Factory_impl (void)
  {
  }

  Consumer_Config_ptr
  EventService_Factory_impl::create_consumer_config (EventServiceType type)
  {

    switch (type)
    {
    case DIRECT:
      {
      Direct_Consumer_Config_impl * consumer_config = 0;
      ACE_NEW_RETURN (consumer_config,
                      Direct_Consumer_Config_impl,
                      Consumer_Config::_nil ());
      Direct_Consumer_Config_var return_direct =
        consumer_config->_this (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_CHECK;
      return return_direct._retn ();
      break;
      }
    case RTEC:
      {
      RTEvent_Consumer_Config_impl * consumer_config = 0;
      ACE_NEW_RETURN (consumer_config,
                      RTEvent_Consumer_Config_impl,
                      Consumer_Config::_nil ());
      RTEvent_Consumer_Config_var return_rtec =
        consumer_config->_this (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_CHECK;
      return return_rtec._retn ();
      break;
      }
    case NOTIFY:
      {
      CosNotify_Consumer_Config_impl * consumer_config = 0;
      ACE_NEW_RETURN (consumer_config,
                      CosNotify_Consumer_Config_impl,
                      Consumer_Config::_nil ());
      CosNotify_Consumer_Config_var return_notify =
        consumer_config->_this (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_CHECK;
      return return_notify._retn ();
      break;
      }
    }

    return Consumer_Config::_nil ();

  }

  Supplier_Config_ptr
  EventService_Factory_impl::create_supplier_config (EventServiceType type)
  {

    // @@George, at this place we should be able to load from a library.
    // @@ And oh, BTW, keep direct as default. If there are no strings
    // for then we should use the direct mode of connection.

    switch (type)
    {
    case DIRECT:
    {
      Direct_Supplier_Config_impl * supplier_config = 0;
      ACE_NEW_RETURN (supplier_config,
                      Direct_Supplier_Config_impl,
                      Supplier_Config::_nil ());
      Direct_Supplier_Config_var return_direct =
        supplier_config->_this (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_CHECK;
      return return_direct._retn ();
      break;
    }
    case RTEC:
    {
      RTEvent_Supplier_Config_impl * supplier_config = 0;
      ACE_NEW_RETURN (supplier_config,
                      RTEvent_Supplier_Config_impl,
                      Supplier_Config::_nil ());
      RTEvent_Supplier_Config_var return_rtec =
        supplier_config->_this (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_CHECK;
      return return_rtec._retn ();
      break;
    }
    case NOTIFY:
    {
      CosNotify_Supplier_Config_impl * supplier_config = 0;
      ACE_NEW_RETURN (supplier_config,
                      CosNotify_Supplier_Config_impl,
                      Supplier_Config::_nil ());
      CosNotify_Supplier_Config_var return_notify =
        supplier_config->_this (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_CHECK;
      return return_notify._retn ();
      break;
    }
    }

    return Supplier_Config::_nil ();

  }

  EventServiceBase *
  EventService_Factory_impl::create (EventServiceType type)
  {

    ACE_DEBUG ((LM_DEBUG, "CIAO::EventService_Factory_impl::create_event_service\n"));

    switch (type)
    {
    case DIRECT:
    {
      DirectEventService * event_service = 0;
      ACE_NEW_RETURN (event_service,
                      DirectEventService (this->orb_.in (),
                                          this->root_poa_.in ()),
                      0);
      return event_service;
      break;
    }
    case RTEC:
    {
      if (CORBA::is_nil (this->rt_event_channel_.in ()))
        {
          this->create_rt_event_channel (ACE_ENV_SINGLE_ARG_DECL);
          ACE_CHECK;
        }
      RTEventService * event_service = 0;
      ACE_NEW_RETURN (event_service,
                      RTEventService (this->orb_.in (),
                                      this->root_poa_.in (),
                                      this->rt_event_channel_.in ()),
                      0);
      return event_service;
      break;
    }
    case NOTIFY:
    {
      if (CORBA::is_nil (this->notify_channel_.in ()))
        {
          this->create_notify_channel (ACE_ENV_SINGLE_ARG_DECL);
          ACE_CHECK;
        }
      CosNotifyService * event_service = 0;
      ACE_NEW_RETURN (event_service,
                      CosNotifyService (this->orb_.in (),
                                        this->root_poa_.in (),
                                        this->notify_channel_.in ()),
                      0);
      return event_service;
      break;
    }
    }

    return 0;

  }

  /*EventConsumerInfo
  Events_Manager::find_consumer_info (CONNECTION_ID connection_id)
  {
  
    EventConsumerInfo consumer_info;

    if (this->consumer_info_map_.find (connection_id, consumer_info) == -1)
      {
        ACE_THROW ((CORBA::BAD_PARAM ()));
      }

    return consumer_info;

  }

  void
  Events_Manager::bind_consumer_info (EventConsumerInfo consumer_info,
                                                   CONNECTION_ID connection_id)
  {
  
    /// Save the consumer's disconnect info in a map.
    this->consumer_info_map_.bind (connection_id,
                                   consumer_info);

  }

  RtecEventComm::EventType
  Events_Manager::find_rtec_type_id (CONNECTION_ID connection_id)
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

  RtecEventComm::EventSourceID
  Events_Manager::find_rtec_source_id (CONNECTION_ID connection_id)
  {

    RtecEventComm::EventSourceID event_source_id;

    if (this->publishers_map_.find (connection_id, event_source_id) == -1)
      {
        event_source_id =
          ACE_ES_EVENT_SOURCE_ANY + 1 + this->publishers_map_.current_size ();
        this->publishers_map_.bind (connection_id, event_source_id);
      }

    return event_source_id;

  }*/

  // @@ George, not sure how we plan to get the svc.conf file options
  // into the channel. Just a place holder to think about it.
  void
  EventService_Factory_impl::create_rt_event_channel (
      ACE_ENV_SINGLE_ARG_DECL)
    ACE_THROW_SPEC ((
      CORBA::SystemException))
  {
    ACE_DEBUG ((LM_DEBUG, "CIAO::EventService_Factory_impl::create_rt_event_channel\n"));

    TAO_EC_Event_Channel_Attributes attributes (this->root_poa_.in (),
                                                this->root_poa_.in ());
    TAO_EC_Event_Channel * ec_servant;
    ACE_NEW (ec_servant, TAO_EC_Event_Channel (attributes));
    ec_servant->activate (ACE_ENV_SINGLE_ARG_PARAMETER);
    ACE_CHECK;
    this->rt_event_channel_ = ec_servant->_this (ACE_ENV_SINGLE_ARG_PARAMETER);
    ACE_CHECK;
  }

  void
  EventService_Factory_impl::create_notify_channel (
      ACE_ENV_SINGLE_ARG_DECL)
    ACE_THROW_SPEC ((
      CORBA::SystemException))
  {

    ACE_DEBUG ((LM_DEBUG, "CIAO::EventService_Factory_impl::create_notify_channel\n"));

    CosNotifyChannelAdmin::EventChannelFactory_var notify_factory;
    CosNotifyChannelAdmin::ChannelID id;
    CosNotification::QoSProperties initial_qos;
    CosNotification::AdminProperties initial_admin;
    this->notify_channel_ = notify_factory->create_channel (initial_qos,
                                                            initial_admin,
                                                            id
                                                            ACE_ENV_ARG_PARAMETER);
    ACE_CHECK;
  }

  int
  EventService_Factory_impl::Initializer (void)
  {
    return
      ACE_Service_Config::process_directive (
          ace_svc_desc_EventService_Factory_impl
        );
  }

  void EventService_Factory_impl::init (CORBA::ORB_ptr orb, PortableServer::POA_ptr poa)
  {
    this->orb_ = CORBA::ORB::_duplicate (orb);
    this->root_poa_ = PortableServer::POA::_duplicate (poa);
  }

ACE_STATIC_SVC_DEFINE (
    EventService_Factory_impl,
    ACE_TEXT ("CIAO_EventService_Factory"),
    ACE_SVC_OBJ_T,
    &ACE_SVC_NAME (EventService_Factory_impl),
    ACE_Service_Type::DELETE_THIS | ACE_Service_Type::DELETE_OBJ,
    0
  )

ACE_FACTORY_DEFINE (CIAO_EVENTS, EventService_Factory_impl)

} // namespace CIAO

