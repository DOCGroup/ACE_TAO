// -*- C++ -*-

//=============================================================================
/**
 *  @file    CIAO_Events.h
 *
 *  $Id$
 *
 *  Header file for CIAO's event classes
 *
 *  @author George Edwards <g.edwards@vanderbilt.edu>
 */
//=============================================================================

#ifndef CIAO_EVENTS_H
#define CIAO_EVENTS_H
#include /**/ "ace/pre.h"

#include "orbsvcs/RtecEventCommS.h"
#include "orbsvcs/RtecEventChannelAdminS.h"
#include "Event_Utilities.h"
#include "orbsvcs/Event/EC_Event_Channel.h"
#include "orbsvcs/Event/EC_Default_Factory.h"
#include "CCM_ContainerC.h"
#include "ace/Active_Map_Manager.h"
#include "CIAO_EventsS.h"

namespace CIAO_Events
{

  // Forward declarations.
  class EventServiceBase;
  class Events_Manager;

  /// Stores information about event consumers. Stored in a map in CIAO::Container.
  struct EventServiceInfo
  {
    EventServiceType type;
    EventServiceBase * service;
    union
      {
        RtecEventComm::PushConsumer_ptr push_consumer;
        ACE_Active_Map_Manager_Key * consumer_key;
      } connection;
  };

  /// An abstract base class. Derived classes will provide appropriate implementations of
  /// the connect, disconnect, and push methods. Each event source has an associated
  /// EventServiceBase, stored in a map in CIAO::Container.
  class EventServiceBase
  {

  public:

    virtual void connect_event_supplier (
        CIAO_Events::Supplier_Config_ptr supplier_config
        ACE_ENV_ARG_DECL)
      ACE_THROW_SPEC ((
        CORBA::SystemException)) = 0;

    virtual CIAO_Events::EventServiceInfo connect_event_consumer (
        CIAO_Events::Consumer_Config_ptr consumer_config
        ACE_ENV_ARG_DECL)
      ACE_THROW_SPEC ((
        CORBA::SystemException)) = 0;

    virtual void disconnect_event_consumer (
        CIAO_Events::EventServiceInfo service_info
        ACE_ENV_ARG_DECL)
      ACE_THROW_SPEC ((
        CORBA::SystemException,
        Components::InvalidName,
        Components::InvalidConnection)) = 0;

    virtual void disconnect_event_supplier (
        ACE_ENV_SINGLE_ARG_DECL)
      ACE_THROW_SPEC ((
        CORBA::SystemException,
        Components::InvalidName,
        Components::InvalidConnection)) = 0;

    virtual void push_event (
        Components::EventBase *ev
        ACE_ENV_ARG_DECL)
      ACE_THROW_SPEC ((
        CORBA::SystemException)) = 0;

  };

  /// An implementation of EventServiceBase using TAO's RT Event Channel.
  class RTEventService :
    public virtual EventServiceBase
  {

  public:

    RTEventService (CORBA::ORB_ptr orb, RtecEventChannelAdmin::EventChannel_ptr ec);

    virtual void connect_event_supplier (
        CIAO_Events::Supplier_Config_ptr supplier_config
        ACE_ENV_ARG_DECL)
      ACE_THROW_SPEC ((
        CORBA::SystemException));

    virtual CIAO_Events::EventServiceInfo connect_event_consumer (
        CIAO_Events::Consumer_Config_ptr consumer_config
        ACE_ENV_ARG_DECL)
      ACE_THROW_SPEC ((
        CORBA::SystemException));

    virtual void disconnect_event_consumer (
        CIAO_Events::EventServiceInfo service_info
        ACE_ENV_ARG_DECL)
      ACE_THROW_SPEC ((
        CORBA::SystemException,
        Components::InvalidName,
        Components::InvalidConnection));

    virtual void disconnect_event_supplier (
        ACE_ENV_SINGLE_ARG_DECL)
      ACE_THROW_SPEC ((
        CORBA::SystemException,
        Components::InvalidName,
        Components::InvalidConnection));

    virtual void push_event (
        Components::EventBase *ev
        ACE_ENV_ARG_DECL)
      ACE_THROW_SPEC ((
        CORBA::SystemException));

  private:

    CORBA::ORB_var orb_;

    // Reference to the Root POA
    PortableServer::POA_var root_poa_;

    /// Reference to the RT event channel
    RtecEventChannelAdmin::EventChannel_var rt_event_channel_;

    /// The type of event
    RtecEventComm::EventType type_id_;

    /// Info for the event publisher
    RtecEventComm::EventSourceID source_id_;
    RtecEventComm::PushSupplier_var push_supplier_;
    RtecEventChannelAdmin::ProxyPushConsumer_var proxy_consumer_;

  };

  /// An implementation of EventServiceBase using direct communication.
  class DirectEventService :
    public virtual EventServiceBase
  {

  public:

    virtual void connect_event_supplier (
        CIAO_Events::Supplier_Config_ptr supplier_config
        ACE_ENV_ARG_DECL)
      ACE_THROW_SPEC ((
        CORBA::SystemException));

    virtual CIAO_Events::EventServiceInfo connect_event_consumer (
        CIAO_Events::Consumer_Config_ptr consumer_config
        ACE_ENV_ARG_DECL)
      ACE_THROW_SPEC ((
        CORBA::SystemException));

    virtual void disconnect_event_consumer (
        CIAO_Events::EventServiceInfo service_info
        ACE_ENV_ARG_DECL)
      ACE_THROW_SPEC ((
        CORBA::SystemException,
        Components::InvalidName,
        Components::InvalidConnection));

    virtual void disconnect_event_supplier (
        ACE_ENV_SINGLE_ARG_DECL)
      ACE_THROW_SPEC ((
        CORBA::SystemException,
        Components::InvalidName,
        Components::InvalidConnection));

    virtual void push_event (
        Components::EventBase *ev
        ACE_ENV_ARG_DECL)
      ACE_THROW_SPEC ((
        CORBA::SystemException));

  private:

    /// Map of consumers
    ACE_Active_Map_Manager<Components::EventConsumerBase_ptr> consumer_map_;
    
  };

  /// An implementation of the Consumer_Config IDL interface that configures
  /// an RT Event Channel. An object of this type will be returned from
  /// CIAO::Container::_ciao_create_event_consumer_config () when "RTEC" is
  /// specified as the event service type.
  class RTEvent_Consumer_Config :
    public virtual POA_CIAO_Events::Consumer_Config
  {

  public:

    RTEvent_Consumer_Config (Events_Manager * em);

    void set_consumer_id (CONNECTION_ID connection_id ACE_ENV_ARG_DECL)
      ACE_THROW_SPEC ((CORBA::SystemException));

    void set_supplier_id (CONNECTION_ID connection_id ACE_ENV_ARG_DECL)
      ACE_THROW_SPEC ((CORBA::SystemException));

    void set_consumer (Components::EventConsumerBase_ptr consumer ACE_ENV_ARG_DECL)
      ACE_THROW_SPEC ((CORBA::SystemException));

    CONNECTION_ID get_consumer_id (ACE_ENV_SINGLE_ARG_DECL)
      ACE_THROW_SPEC ((CORBA::SystemException));

    CONNECTION_ID get_supplier_id (ACE_ENV_SINGLE_ARG_DECL)
      ACE_THROW_SPEC ((CORBA::SystemException));

    EventServiceType get_service_type (ACE_ENV_SINGLE_ARG_DECL)
      ACE_THROW_SPEC ((CORBA::SystemException));

    Components::EventConsumerBase_ptr get_consumer (ACE_ENV_SINGLE_ARG_DECL)
      ACE_THROW_SPEC ((CORBA::SystemException));

    RtecEventChannelAdmin::ConsumerQOS * get_rt_event_qos (ACE_ENV_SINGLE_ARG_DECL)
      ACE_THROW_SPEC ((CORBA::SystemException));

  private:

    CONNECTION_ID consumer_id_;
    
    CONNECTION_ID supplier_id_;

    Components::EventConsumerBase_ptr consumer_;

    EventServiceType service_type_;

    Events_Manager * events_manager_;
  };

  /// An implementation of the Supplier_Config IDL interface that configures
  /// TAO's RT Event Channel. An object of this type will be returned from
  /// CIAO::Container::_ciao_create_event_supplier_config () when "RTEC" is
  /// specified as the event service type.
  class RTEvent_Supplier_Config :
    public virtual POA_CIAO_Events::Supplier_Config
  {

  public:

    RTEvent_Supplier_Config (Events_Manager * em);

    void set_supplier_id (CONNECTION_ID connection_id ACE_ENV_ARG_DECL)
      ACE_THROW_SPEC ((CORBA::SystemException));

    CONNECTION_ID get_supplier_id (ACE_ENV_SINGLE_ARG_DECL)
      ACE_THROW_SPEC ((CORBA::SystemException));

    EventServiceType get_service_type (ACE_ENV_SINGLE_ARG_DECL)
      ACE_THROW_SPEC ((CORBA::SystemException));

    RtecEventChannelAdmin::SupplierQOS * get_rt_event_qos (ACE_ENV_SINGLE_ARG_DECL)
      ACE_THROW_SPEC ((CORBA::SystemException));

  private:

    CONNECTION_ID supplier_id_;

    EventServiceType service_type_;

    Events_Manager * events_manager_;
  };

  /// An implementation of the Consumer_Config IDL interface that configures
  /// direct communication. An object of this type will be returned from
  /// CIAO::Container::_ciao_create_event_consumer_config () when "DIRECT" is
  /// specified as the event service type.
  class Direct_Consumer_Config :
    public virtual POA_CIAO_Events::Consumer_Config
  {

  public:

    Direct_Consumer_Config (Events_Manager * em);

    void set_consumer_id (CONNECTION_ID connection_id ACE_ENV_ARG_DECL)
      ACE_THROW_SPEC ((CORBA::SystemException));

    void set_supplier_id (CONNECTION_ID connection_id ACE_ENV_ARG_DECL)
      ACE_THROW_SPEC ((CORBA::SystemException));

    void set_consumer (Components::EventConsumerBase_ptr consumer ACE_ENV_ARG_DECL)
      ACE_THROW_SPEC ((CORBA::SystemException));

    CONNECTION_ID get_consumer_id (ACE_ENV_SINGLE_ARG_DECL)
      ACE_THROW_SPEC ((CORBA::SystemException));

    CONNECTION_ID get_supplier_id (ACE_ENV_SINGLE_ARG_DECL)
      ACE_THROW_SPEC ((CORBA::SystemException));

    EventServiceType get_service_type (ACE_ENV_SINGLE_ARG_DECL)
      ACE_THROW_SPEC ((CORBA::SystemException));

    Components::EventConsumerBase_ptr get_consumer (ACE_ENV_SINGLE_ARG_DECL)
      ACE_THROW_SPEC ((CORBA::SystemException));

    RtecEventChannelAdmin::ConsumerQOS * get_rt_event_qos (ACE_ENV_SINGLE_ARG_DECL)
      ACE_THROW_SPEC ((CORBA::SystemException));

  private:

    CONNECTION_ID consumer_id_;
    
    CONNECTION_ID supplier_id_;

    Components::EventConsumerBase_ptr consumer_;

    EventServiceType service_type_;

    Events_Manager * events_manager_;
  };

  /// An implementation of the Supplier_Config IDL interface that configures
  /// direct communication. An object of this type will be returned from
  /// CIAO::Container::_ciao_create_event_supplier_config () when "DIRECT" is
  /// specified as the event service type.
  class Direct_Supplier_Config :
    public virtual POA_CIAO_Events::Supplier_Config
  {

  public:

    Direct_Supplier_Config (Events_Manager * em);

    void set_supplier_id (CONNECTION_ID connection_id ACE_ENV_ARG_DECL)
      ACE_THROW_SPEC ((CORBA::SystemException));

    CONNECTION_ID get_supplier_id (ACE_ENV_SINGLE_ARG_DECL)
      ACE_THROW_SPEC ((CORBA::SystemException));

    EventServiceType get_service_type (ACE_ENV_SINGLE_ARG_DECL)
      ACE_THROW_SPEC ((CORBA::SystemException));

    RtecEventChannelAdmin::SupplierQOS * get_rt_event_qos (ACE_ENV_SINGLE_ARG_DECL)
      ACE_THROW_SPEC ((CORBA::SystemException));

  private:

    CONNECTION_ID supplier_id_;

    EventServiceType service_type_;

    Events_Manager * events_manager_;
  };

  /// A helper class that encapsulates management functions for CIAO::Container.
  class Events_Manager
  {

  public:

    Events_Manager (CORBA::ORB_ptr orb);

    Consumer_Config_ptr create_consumer_config (const char * service_type);

    Supplier_Config_ptr create_supplier_config (const char * service_type);

    EventServiceBase * create_supplier (Supplier_Config * supplier_config);

    RtecEventComm::EventType get_rtec_type_id (CONNECTION_ID connection_id);

    RtecEventComm::EventSourceID get_rtec_source_id (CONNECTION_ID connection_id);

  private:

    void create_rt_event_channel (
        ACE_ENV_SINGLE_ARG_DECL)
      ACE_THROW_SPEC ((
        CORBA::SystemException));

    CORBA::ORB_var orb_;

    PortableServer::POA_var root_poa_;

    /// Reference to the RT event channel
    RtecEventChannelAdmin::EventChannel_var rt_event_channel_;

    /// Map of event type ids
    ACE_Hash_Map_Manager<CONNECTION_ID, RtecEventComm::EventType, ACE_Null_Mutex>
      event_types_map_;

    /// Map of supplier ids
    ACE_Hash_Map_Manager<CONNECTION_ID, RtecEventComm::EventSourceID, ACE_Null_Mutex>
      publishers_map_;

  };

  /// An implementation for PushSupplier
  class RTEventServiceSupplier_impl :
    public virtual POA_RtecEventComm::PushSupplier,
    public virtual PortableServer::RefCountServantBase
  {

  public:
    RTEventServiceSupplier_impl (void);

    RTEventServiceSupplier_impl (CORBA::ORB_ptr orb);

    virtual void disconnect_push_supplier (ACE_ENV_SINGLE_ARG_DECL)
      ACE_THROW_SPEC ((CORBA::SystemException));

  private:
    CORBA::ORB_var orb_;
  };

  /// An implementation for PushConsumer
  class RTEventServiceConsumer_impl :
    public virtual POA_RtecEventComm::PushConsumer,
    public virtual PortableServer::RefCountServantBase
  {

  public:
    RTEventServiceConsumer_impl (void);

    RTEventServiceConsumer_impl (CORBA::ORB_ptr orb,
      Components::EventConsumerBase_ptr consumer);

    virtual void push (const RtecEventComm::EventSet& events);

    virtual void disconnect_push_consumer (ACE_ENV_SINGLE_ARG_DECL)
      ACE_THROW_SPEC ((CORBA::SystemException));

  private:
    CORBA::ORB_var orb_;
    Components::EventConsumerBase_var event_consumer_;
  };

}

#include /**/ "ace/post.h"
#endif /* CIAO_EVENTS_H */
