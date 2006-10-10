// -*- C++ -*-

//=============================================================================
/**
 *  @file CIAO_RTEvent.h
 *
 *  $Id$
 *
 *  @author Gan Deng <dengg@dre.vanderbilt.edu>
 *  @author George Edwards <g.edwards@vanderbilt.edu>
 */
//=============================================================================

#ifndef CIAO_RTEVENT_H
#define CIAO_RTEVENT_H
#include /**/ "ace/pre.h"

#include "ace/config-all.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
#pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "CIAO_RTEVENT_Export.h"
#include "ciaosvcs/Events/CIAO_Events_Base/CIAO_EventServiceBase.h"
#include "CIAO_RTEventS.h"

#include "orbsvcs/orbsvcs/Event_Utilities.h"
#include "orbsvcs/orbsvcs/Event/EC_Event_Channel.h"
#include "orbsvcs/orbsvcs/Event/EC_Default_Factory.h"
#include "ace/Hash_Map_Manager.h"

namespace CIAO
{

  /**
   * @class RTEventService
   *
   * An implementation of EventServiceBase using the RT event channel.
   *
   * @@ (GD) There should be a place where the deployment tool could
   *         specify the RT Event Channel service configuration file.
   *         This should be the place where the RtecEventChannel servant was
   *         first time initialized.
   */
  class CIAO_RTEVENT_Export RTEventService :
    public virtual EventServiceBase
  {
  public:

    RTEventService (CORBA::ORB_ptr orb,
                    PortableServer::POA_ptr poa);

    virtual ~RTEventService (void);

    virtual Supplier_Config_ptr
    create_supplier_config (void)
      ACE_THROW_SPEC ((CORBA::SystemException));

    virtual Consumer_Config_ptr
    create_consumer_config (void)
      ACE_THROW_SPEC ((CORBA::SystemException));

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

    virtual void
    disconnect_event_supplier (
        const char * consumer_id
        ACE_ENV_ARG_DECL_WITH_DEFAULTS)
      ACE_THROW_SPEC ((
        CORBA::SystemException,
        Components::InvalidConnection));

    virtual void disconnect_event_consumer (
        const char * connection_id
        ACE_ENV_ARG_DECL)
      ACE_THROW_SPEC ((
        CORBA::SystemException,
        Components::InvalidConnection));

    virtual void push_event (
        Components::EventBase * ev
        ACE_ENV_ARG_DECL)
      ACE_THROW_SPEC ((
        CORBA::SystemException));

  private:
    // @@ (GD) This is the place where use could provide a parameter
    //         which specifies the event channel service configuration file.
    void create_rt_event_channel (
        ACE_ENV_SINGLE_ARG_DECL)
      ACE_THROW_SPEC ((
        CORBA::SystemException));

  private:

    /// Reference to the ORB
    CORBA::ORB_var orb_;

    /// Reference to the Root POA
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
     * @@@ Need to change this into a map, since multiple sources
     * could be connected to the event channel.
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
    public virtual POA_RtecEventComm::PushSupplier
  {
  public:

    RTEventServiceSupplier_impl (
      PortableServer::POA_ptr poa);

    virtual void disconnect_push_supplier (
        ACE_ENV_SINGLE_ARG_DECL)
      ACE_THROW_SPEC ((
        CORBA::SystemException));

  private:

    PortableServer::POA_var poa_;
  };

  /**
   * @class RTEventServiceConsumer_impl
   *
   * An implementation of the PushConsumer interface.
   */
  class RTEventServiceConsumer_impl :
    public virtual POA_RtecEventComm::PushConsumer
  {
  public:

    RTEventServiceConsumer_impl (
      PortableServer::POA_ptr poa,
      Components::EventConsumerBase_ptr consumer);

    virtual void push (
      const RtecEventComm::EventSet& events)
    ACE_THROW_SPEC ((
      CORBA::SystemException));

    virtual void disconnect_push_consumer (
        ACE_ENV_SINGLE_ARG_DECL)
      ACE_THROW_SPEC ((
        CORBA::SystemException));

  private:

    PortableServer::POA_var poa_;

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
    public virtual POA_CIAO::RTEvent_Consumer_Config,
    public virtual Event_Consumer_Config_Base
  {

  public:
    RTEvent_Consumer_Config_impl (PortableServer::POA_ptr poa);

    virtual ~RTEvent_Consumer_Config_impl (void);

    virtual void start_conjunction_group (CORBA::Long size ACE_ENV_ARG_DECL)
      ACE_THROW_SPEC ((CORBA::SystemException));

    virtual void start_disjunction_group (CORBA::Long size ACE_ENV_ARG_DECL)
      ACE_THROW_SPEC ((CORBA::SystemException));

    virtual void insert_source (const char * source_id ACE_ENV_ARG_DECL)
      ACE_THROW_SPEC ((CORBA::SystemException));

    virtual void insert_type (::CORBA::Long event_type
                              ACE_ENV_ARG_DECL_WITH_DEFAULTS)
      ACE_THROW_SPEC ((::CORBA::SystemException));

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

    //@@ (GD) There should be a place where the deployment tool could
  //        set up the rt_event_qos properties for Consumer Config.

    virtual RtecEventChannelAdmin::ConsumerQOS * rt_event_qos (ACE_ENV_SINGLE_ARG_DECL)
      ACE_THROW_SPEC ((CORBA::SystemException));

    virtual void destroy (ACE_ENV_SINGLE_ARG_DECL)
      ACE_THROW_SPEC ((CORBA::SystemException));

  private:

    ACE_CString consumer_id_;

    ACE_CString supplier_id_;

    Components::EventConsumerBase_var consumer_;

    EventServiceType service_type_;

    ACE_ConsumerQOS_Factory qos_;

    PortableServer::POA_var poa_;
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
    RTEvent_Supplier_Config_impl (PortableServer::POA_ptr poa);

    virtual ~RTEvent_Supplier_Config_impl (void);

    void supplier_id (const char * supplier_id ACE_ENV_ARG_DECL)
      ACE_THROW_SPEC ((CORBA::SystemException));

    CONNECTION_ID supplier_id (ACE_ENV_SINGLE_ARG_DECL)
      ACE_THROW_SPEC ((CORBA::SystemException));

    EventServiceType service_type (ACE_ENV_SINGLE_ARG_DECL)
      ACE_THROW_SPEC ((CORBA::SystemException));

    //@@ (GD) There should be a place where the deployment tool could
    //        set up the rt_event_qos properties for Supplier Config.

    RtecEventChannelAdmin::SupplierQOS * rt_event_qos (ACE_ENV_SINGLE_ARG_DECL)
      ACE_THROW_SPEC ((CORBA::SystemException));

    virtual void destroy (ACE_ENV_SINGLE_ARG_DECL)
      ACE_THROW_SPEC ((CORBA::SystemException));

  private:
    ACE_CString supplier_id_;

    EventServiceType service_type_;

    ACE_SupplierQOS_Factory qos_;

    PortableServer::POA_var poa_;
  };
}

#include /**/ "ace/post.h"
#endif /* CIAO_RTEVENT_H */
