// -*- C++ -*-

//=============================================================================
/**
 *  @file CIAO_RTEvent.h
 *
 *  $Id$
 *
 *  @author George Edwards <g.edwards@vanderbilt.edu>
 */
//=============================================================================

#ifndef CIAO_RTEVENT_H
#define CIAO_RTEVENT_H
#include /**/ "ace/pre.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "CIAO_EventServiceBase.h"
#include "CIAO_RTEventS.h"

#include "orbsvcs/Event_Utilities.h"

namespace CIAO
{

  /**
   * @class RTEventService
   *
   * An implementation of EventServiceBase using the RT event channel.
   */
  class RTEventService :
    public virtual EventServiceBase
  {

  public:

    RTEventService (CORBA::ORB_ptr orb,
                    PortableServer::POA_ptr poa,
                    RtecEventChannelAdmin::EventChannel_ptr ec);

    ~RTEventService ();

    virtual void connect_event_supplier (
        CIAO::Supplier_Config_ptr supplier_config
        ACE_ENV_ARG_DECL)
      ACE_THROW_SPEC ((
        CORBA::SystemException));

    virtual void connect_event_consumer (
        CIAO::Consumer_Config_ptr consumer_config
        ACE_ENV_ARG_DECL)
      ACE_THROW_SPEC ((
        CORBA::SystemException));

    virtual void disconnect_event_supplier (
        ACE_ENV_SINGLE_ARG_DECL)
      ACE_THROW_SPEC ((
        CORBA::SystemException,
        Components::InvalidName,
        Components::InvalidConnection));

    virtual void disconnect_event_consumer (
        const char * connection_id
        ACE_ENV_ARG_DECL)
      ACE_THROW_SPEC ((
        CORBA::SystemException,
        Components::InvalidName,
        Components::InvalidConnection));

    virtual void push_event (
        Components::EventBase * ev
        ACE_ENV_ARG_DECL)
      ACE_THROW_SPEC ((
        CORBA::SystemException));

  private:

    // Reference to the ORB
    CORBA::ORB_var orb_;

    // Reference to the Root POA
    PortableServer::POA_var root_poa_;

    /**
     * @var RtecEventChannelAdmin::EventChannel_var rt_event_channel_
     *
     * Reference to the RT event channel.
     */
    RtecEventChannelAdmin::EventChannel_var rt_event_channel_;

    /**
     * @var RtecEventComm::EventType type_id_
     *
     * The type of event.
     */
    RtecEventComm::EventType type_id_;

    /**
     * @var RtecEventComm::EventSourceID source_id_
     *
     * The supplier id.
     */
    RtecEventComm::EventSourceID source_id_;

    /**
     * @var RtecEventChannelAdmin::ProxyPushConsumer_var proxy_consumer_
     *
     * The proxy consumer to which events are pushed.
     */
    RtecEventChannelAdmin::ProxyPushConsumer_var proxy_consumer_;
 
    /**
     * @var ACE_Hash_Map_Manager<> proxy_supplier_map_
     *
     * Mapping of each event sink to a proxy supplier for disconnect purposes.
     */
    ACE_Hash_Map_Manager_Ex<ACE_CString,
                            RtecEventChannelAdmin::ProxyPushSupplier_ptr,
                            ACE_Hash<ACE_CString>,
                            ACE_Equal_To<ACE_CString>,
                            ACE_Null_Mutex> proxy_supplier_map_;

  };

  /**
   * @class RTEventServiceSupplier_impl
   *
   * An implementation of the PushSupplier interface.
   */
  class RTEventServiceSupplier_impl :
    public virtual POA_RtecEventComm::PushSupplier,
    public virtual PortableServer::RefCountServantBase
  {

  public:

    RTEventServiceSupplier_impl (void);

    RTEventServiceSupplier_impl (
      CORBA::ORB_ptr orb);

    virtual void disconnect_push_supplier (
        ACE_ENV_SINGLE_ARG_DECL)
      ACE_THROW_SPEC ((
        CORBA::SystemException));

  private:

    CORBA::ORB_var orb_;

  };

  /**
   * @class RTEventServiceConsumer_impl
   *
   * An implementation of the PushConsumer interface.
   */
  class RTEventServiceConsumer_impl :
    public virtual POA_RtecEventComm::PushConsumer,
    public virtual PortableServer::RefCountServantBase
  {

  public:

    RTEventServiceConsumer_impl (void);

    RTEventServiceConsumer_impl (
      CORBA::ORB_ptr orb,
      Components::EventConsumerBase_ptr consumer);

    virtual void push (
      const RtecEventComm::EventSet& events);

    virtual void disconnect_push_consumer (
        ACE_ENV_SINGLE_ARG_DECL)
      ACE_THROW_SPEC ((
        CORBA::SystemException));

  private:

    CORBA::ORB_var orb_;

    Components::EventConsumerBase_var event_consumer_;

  };

  /**
   * @class RTEvent_Consumer_Config_impl
   * 
   * Implementation of the RTEvent_Consumer_Config IDL interface that
   * configures TAO's RT event channel. An object of this type will be returned
   * from @c CIAO::Container::create_consumer_config () when @c RTEC is
   * specified as the event service type.
   */
  class RTEvent_Consumer_Config_impl :
    public virtual POA_CIAO::RTEvent_Consumer_Config
  {

  public:

    RTEvent_Consumer_Config_impl ();

    ~RTEvent_Consumer_Config_impl ();

    virtual void start_conjunction_group (CORBA::Long size ACE_ENV_ARG_DECL)
      ACE_THROW_SPEC ((CORBA::SystemException));

    virtual void start_disjunction_group (CORBA::Long size ACE_ENV_ARG_DECL)
      ACE_THROW_SPEC ((CORBA::SystemException));

    virtual void insert_source (const char * source_id ACE_ENV_ARG_DECL)
      ACE_THROW_SPEC ((CORBA::SystemException));

    virtual void consumer_id (const char * consumer_id ACE_ENV_ARG_DECL)
      ACE_THROW_SPEC ((CORBA::SystemException));

    virtual CONNECTION_ID consumer_id (ACE_ENV_SINGLE_ARG_DECL)
      ACE_THROW_SPEC ((CORBA::SystemException));

    virtual void supplier_id (const char * supplier_id ACE_ENV_ARG_DECL)
      ACE_THROW_SPEC ((CORBA::SystemException));

    virtual CONNECTION_ID supplier_id (ACE_ENV_SINGLE_ARG_DECL)
      ACE_THROW_SPEC ((CORBA::SystemException));

    virtual void consumer (Components::EventConsumerBase_ptr consumer ACE_ENV_ARG_DECL)
      ACE_THROW_SPEC ((CORBA::SystemException));

    virtual Components::EventConsumerBase_ptr consumer (ACE_ENV_SINGLE_ARG_DECL)
      ACE_THROW_SPEC ((CORBA::SystemException));

    virtual EventServiceType service_type (ACE_ENV_SINGLE_ARG_DECL)
      ACE_THROW_SPEC ((CORBA::SystemException));

    virtual RtecEventChannelAdmin::ConsumerQOS * rt_event_qos (ACE_ENV_SINGLE_ARG_DECL)
      ACE_THROW_SPEC ((CORBA::SystemException));

  private:

    ACE_CString consumer_id_;

    ACE_CString supplier_id_;

    Components::EventConsumerBase_var consumer_;

    EventServiceType service_type_;

    ACE_ConsumerQOS_Factory qos_;

  };

  /**
   * @class RTEvent_Supplier_Config_impl
   * 
   * Implementation of the RTEvent_Supplier_Config IDL interface that
   * configures TAO's RT event channel. An object of this type will be returned
   * from @c CIAO::Container::create_supplier_config () when @c RTEC is
   * specified as the event service type.
   */
  class RTEvent_Supplier_Config_impl :
    public virtual POA_CIAO::RTEvent_Supplier_Config
  {

  public:

    RTEvent_Supplier_Config_impl ();

    ~RTEvent_Supplier_Config_impl ();

    void supplier_id (const char * supplier_id ACE_ENV_ARG_DECL)
      ACE_THROW_SPEC ((CORBA::SystemException));

    CONNECTION_ID supplier_id (ACE_ENV_SINGLE_ARG_DECL)
      ACE_THROW_SPEC ((CORBA::SystemException));

    EventServiceType service_type (ACE_ENV_SINGLE_ARG_DECL)
      ACE_THROW_SPEC ((CORBA::SystemException));

    RtecEventChannelAdmin::SupplierQOS * rt_event_qos (ACE_ENV_SINGLE_ARG_DECL)
      ACE_THROW_SPEC ((CORBA::SystemException));

  private:

    ACE_CString supplier_id_;

    EventServiceType service_type_;

    ACE_SupplierQOS_Factory qos_;

  };

}

#include /**/ "ace/post.h"
#endif /* CIAO_RTEVENT_H */
