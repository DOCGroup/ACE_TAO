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

// @@ George Pragma please..

#include "orbsvcs/CosNotifyCommS.h"
#include "orbsvcs/CosNotifyChannelAdminS.h"
#include "orbsvcs/RtecEventCommS.h"
#include "orbsvcs/RtecEventChannelAdminS.h"
#include "Event_Utilities.h"
#include "orbsvcs/Event/EC_Event_Channel.h"
#include "orbsvcs/Event/EC_Default_Factory.h"
#include "CCM_ContainerC.h"
#include "ace/Active_Map_Manager.h"
#include "CIAO_EventsS.h"

// @@ George, can't we stuff everything in CIAO namespace?
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
        CosNotifyComm::StructuredPushConsumer_ptr notify_push_consumer;
        RtecEventComm::PushConsumer_ptr rtec_push_consumer;
        ACE_Active_Map_Manager_Key * consumer_key;
      } disconnect;
  };

  // @@ George, if you accept the suggestion in CIAO_Events.idl, we
  // should probably add a class per file.

  /// An abstract base class. Derived classes will provide appropriate implementations of
  /// the connect, disconnect, and push methods. Each event source has an associated
  /// EventServiceBase, stored in a map in CIAO::Container.
  /**
   *  @@ George, doxygen documentation please..
   */
  // @@ George, any particular reason that this cannot be a local
  /// interface in an idl? The above struct  can also be pushed into
  /// an IDL, unless you don't want to have object semantics.
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

  class CosNotifyService :
    public virtual EventServiceBase
  {

  public:

    CosNotifyService (CORBA::ORB_ptr orb, CosNotifyChannelAdmin::EventChannel_ptr ec);

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
    CosNotifyChannelAdmin::EventChannel_var notify_channel_;

    /// The type of event
    CORBA::String_var type_id_;

    /// Info for the event publisher
    CORBA::String_var source_id_;
    CosNotifyComm::StructuredPushSupplier_var push_supplier_;
    CosNotifyChannelAdmin::StructuredProxyPushConsumer_var proxy_consumer_;

  };

  /// An implementation of the Consumer_Config IDL interface that configures
  /// an RT Event Channel. An object of this type will be returned from
  /// CIAO::Container::_ciao_create_event_consumer_config () when "RTEC" is
  /// specified as the event service type.
  // @@ George, class documentations are done like this in doxygen
  //
  // /**
  //  * @class name
  //  *
  //  * @brief  brief explanation here
  //  *
  //  *
  //  */
  //


  class RTEvent_Consumer_Config :
    public virtual POA_CIAO_Events::Consumer_Config
  {

  public:

    RTEvent_Consumer_Config (Events_Manager * em);

    void start_conjunction_group (CORBA::Long size ACE_ENV_ARG_DECL)
      ACE_THROW_SPEC ((CORBA::SystemException));

    void start_disjunction_group (CORBA::Long size ACE_ENV_ARG_DECL)
      ACE_THROW_SPEC ((CORBA::SystemException));

    void set_consumer_id (CONNECTION_ID connection_id ACE_ENV_ARG_DECL)
      ACE_THROW_SPEC ((CORBA::SystemException));

    void insert_supplier_id (CONNECTION_ID connection_id ACE_ENV_ARG_DECL)
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

    CosNotifyFilter::Filter * get_notify_filter (ACE_ENV_SINGLE_ARG_DECL)
      ACE_THROW_SPEC ((CORBA::SystemException));

    CosNotification::QoSProperties * get_notify_qos (ACE_ENV_SINGLE_ARG_DECL)
      ACE_THROW_SPEC ((CORBA::SystemException));

  private:

    CONNECTION_ID consumer_id_;

    CONNECTION_ID supplier_id_;

    Components::EventConsumerBase_var consumer_;

    EventServiceType service_type_;

    Events_Manager * events_manager_;

    ACE_ConsumerQOS_Factory qos_;
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

    CosNotifyFilter::Filter * get_notify_filter (ACE_ENV_SINGLE_ARG_DECL)
      ACE_THROW_SPEC ((CORBA::SystemException));

    CosNotification::QoSProperties * get_notify_qos (ACE_ENV_SINGLE_ARG_DECL)
      ACE_THROW_SPEC ((CORBA::SystemException));

  private:

    CONNECTION_ID supplier_id_;

    EventServiceType service_type_;

    Events_Manager * events_manager_;

    ACE_SupplierQOS_Factory qos_;

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

    void start_conjunction_group (CORBA::Long size ACE_ENV_ARG_DECL)
      ACE_THROW_SPEC ((CORBA::SystemException));

    void start_disjunction_group (CORBA::Long size ACE_ENV_ARG_DECL)
      ACE_THROW_SPEC ((CORBA::SystemException));

    void set_consumer_id (CONNECTION_ID connection_id ACE_ENV_ARG_DECL)
      ACE_THROW_SPEC ((CORBA::SystemException));

    void insert_supplier_id (CONNECTION_ID connection_id ACE_ENV_ARG_DECL)
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

    CosNotifyFilter::Filter * get_notify_filter (ACE_ENV_SINGLE_ARG_DECL)
      ACE_THROW_SPEC ((CORBA::SystemException));

    CosNotification::QoSProperties * get_notify_qos (ACE_ENV_SINGLE_ARG_DECL)
      ACE_THROW_SPEC ((CORBA::SystemException));

  private:

    CONNECTION_ID consumer_id_;

    CONNECTION_ID supplier_id_;

    Components::EventConsumerBase_var consumer_;

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

    CosNotifyFilter::Filter * get_notify_filter (ACE_ENV_SINGLE_ARG_DECL)
      ACE_THROW_SPEC ((CORBA::SystemException));

    CosNotification::QoSProperties * get_notify_qos (ACE_ENV_SINGLE_ARG_DECL)
      ACE_THROW_SPEC ((CORBA::SystemException));

  private:

    CONNECTION_ID supplier_id_;

    EventServiceType service_type_;

    Events_Manager * events_manager_;
  };

  /// An implementation of the Consumer_Config IDL interface that configures
  /// the CosNotify service. An object of this type will be returned from
  /// CIAO::Container::_ciao_create_event_consumer_config () when "NOTIFY" is
  /// specified as the event service type.
  class CosNotify_Consumer_Config :
    public virtual POA_CIAO_Events::Consumer_Config
  {

  public:

    CosNotify_Consumer_Config (Events_Manager * em);

    void start_conjunction_group (CORBA::Long size ACE_ENV_ARG_DECL)
      ACE_THROW_SPEC ((CORBA::SystemException));

    void start_disjunction_group (CORBA::Long size ACE_ENV_ARG_DECL)
      ACE_THROW_SPEC ((CORBA::SystemException));

    void set_consumer_id (CONNECTION_ID connection_id ACE_ENV_ARG_DECL)
      ACE_THROW_SPEC ((CORBA::SystemException));

    void insert_supplier_id (CONNECTION_ID connection_id ACE_ENV_ARG_DECL)
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

    CosNotifyFilter::Filter * get_notify_filter (ACE_ENV_SINGLE_ARG_DECL)
      ACE_THROW_SPEC ((CORBA::SystemException));

    CosNotification::QoSProperties * get_notify_qos (ACE_ENV_SINGLE_ARG_DECL)
      ACE_THROW_SPEC ((CORBA::SystemException));

  private:

    CONNECTION_ID consumer_id_;

    CONNECTION_ID supplier_id_;

    Components::EventConsumerBase_var consumer_;

    EventServiceType service_type_;

    Events_Manager * events_manager_;

    CosNotifyFilter::Filter_var filter_;

    CosNotification::QoSProperties_var qos_;
  };

  /// An implementation of the Supplier_Config IDL interface that configures
  /// the CosNotify service. An object of this type will be returned from
  /// CIAO::Container::_ciao_create_event_supplier_config () when "NOTIFY" is
  /// specified as the event service type.
  class CosNotify_Supplier_Config :
    public virtual POA_CIAO_Events::Supplier_Config
  {

  public:

    CosNotify_Supplier_Config (Events_Manager * em);

    void set_supplier_id (CONNECTION_ID connection_id ACE_ENV_ARG_DECL)
      ACE_THROW_SPEC ((CORBA::SystemException));

    CONNECTION_ID get_supplier_id (ACE_ENV_SINGLE_ARG_DECL)
      ACE_THROW_SPEC ((CORBA::SystemException));

    EventServiceType get_service_type (ACE_ENV_SINGLE_ARG_DECL)
      ACE_THROW_SPEC ((CORBA::SystemException));

    RtecEventChannelAdmin::SupplierQOS * get_rt_event_qos (ACE_ENV_SINGLE_ARG_DECL)
      ACE_THROW_SPEC ((CORBA::SystemException));

    CosNotifyFilter::Filter * get_notify_filter (ACE_ENV_SINGLE_ARG_DECL)
      ACE_THROW_SPEC ((CORBA::SystemException));

    CosNotification::QoSProperties * get_notify_qos (ACE_ENV_SINGLE_ARG_DECL)
      ACE_THROW_SPEC ((CORBA::SystemException));

  private:

    CONNECTION_ID supplier_id_;

    EventServiceType service_type_;

    Events_Manager * events_manager_;

    CosNotifyFilter::Filter_var filter_;

    CosNotification::QoSProperties_var qos_;
  };

  /// A helper class that encapsulates management functions for CIAO::Container.
  class Events_Manager
  {

  public:
    // @@ George, why don't you initialize with the POA pointer. Just
    // curious.
    Events_Manager (CORBA::ORB_ptr orb);

    Consumer_Config_ptr create_consumer_config (const char * service_type);

    Supplier_Config_ptr create_supplier_config (const char * service_type);

    EventServiceBase * create_supplier (Supplier_Config * supplier_config);

    RtecEventComm::EventType get_rtec_type_id (CONNECTION_ID connection_id);

    RtecEventComm::EventSourceID get_rtec_source_id (CONNECTION_ID connection_id);

  private:

    // @George, all these can be moved to a different
    // library. Further, they probably need to be delegated to the
    // underlying concrete type to create it for you instead of
    // providing interfaces.
    void create_rt_event_channel (
        ACE_ENV_SINGLE_ARG_DECL)
      ACE_THROW_SPEC ((
        CORBA::SystemException));

    void create_notify_channel (
        ACE_ENV_SINGLE_ARG_DECL)
      ACE_THROW_SPEC ((
        CORBA::SystemException));

    CORBA::ORB_var orb_;

    PortableServer::POA_var root_poa_;

    /// Reference to the RT Event channel
    RtecEventChannelAdmin::EventChannel_var rt_event_channel_;

    /// Reference to the Notification channel
    CosNotifyChannelAdmin::EventChannel_var notify_channel_;

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

  /// An implementation for PushSupplier
  class CosNotifyServiceSupplier_impl :
    public virtual POA_CosNotifyComm::StructuredPushSupplier,
    public virtual PortableServer::RefCountServantBase
  {

  public:
    CosNotifyServiceSupplier_impl (void);

    CosNotifyServiceSupplier_impl (CORBA::ORB_ptr orb);

    virtual void disconnect_structured_push_supplier (ACE_ENV_SINGLE_ARG_DECL)
      ACE_THROW_SPEC ((CORBA::SystemException));

    virtual void subscription_change (
        const CosNotification::EventTypeSeq& events_added,
        const CosNotification::EventTypeSeq& events_removed
        ACE_ENV_ARG_DECL)
      ACE_THROW_SPEC ((
        CORBA::SystemException,
        CosNotifyComm::InvalidEventType));

  private:
    CORBA::ORB_var orb_;
  };

  /// An implementation for PushConsumer
  class CosNotifyServiceConsumer_impl :
    public virtual POA_CosNotifyComm::StructuredPushConsumer,
    public virtual PortableServer::RefCountServantBase
  {

  public:
    CosNotifyServiceConsumer_impl (void);

    CosNotifyServiceConsumer_impl (CORBA::ORB_ptr orb,
      Components::EventConsumerBase_ptr consumer);

    virtual void push_structured_event (const CosNotification::StructuredEvent& event);

    virtual void disconnect_structured_push_consumer (ACE_ENV_SINGLE_ARG_DECL)
      ACE_THROW_SPEC ((CORBA::SystemException));

    virtual void offer_change (
        const CosNotification::EventTypeSeq& events_added,
        const CosNotification::EventTypeSeq& events_removed
        ACE_ENV_ARG_DECL)
      ACE_THROW_SPEC ((
        CORBA::SystemException,
        CosNotifyComm::InvalidEventType));

  private:
    CORBA::ORB_var orb_;
    Components::EventConsumerBase_var event_consumer_;
  };

}

#include /**/ "ace/post.h"
#endif /* CIAO_EVENTS_H */
