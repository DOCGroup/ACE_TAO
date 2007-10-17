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

#include "CIAO_RTEVENT_Export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
#pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "CIAO_RTEventS.h"

#include "ciaosvcs/Events/CIAO_Events_Base/CIAO_EventServiceBase.h"

#include "orbsvcs/orbsvcs/Event_Utilities.h"
#include "orbsvcs/orbsvcs/Event/EC_Event_Channel.h"
#include "orbsvcs/orbsvcs/Event/EC_Default_Factory.h"
#include "orbsvcs/Event/ECG_Mcast_EH.h"
#include "orbsvcs/Event/ECG_UDP_Sender.h"
#include "orbsvcs/Event/ECG_UDP_Receiver.h"
#include "orbsvcs/Event/ECG_UDP_EH.h"

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
  class CIAO_RTEVENT_Export RTEventService
    : public virtual EventServiceBase,
      public virtual POA_CIAO::CIAO_RT_Event_Service
  {
  public:
    RTEventService (CORBA::ORB_ptr orb,
                    PortableServer::POA_ptr poa,
                    const char * ec_name);

    virtual ~RTEventService (void);

    virtual Supplier_Config_ptr create_supplier_config (void);
    virtual Consumer_Config_ptr create_consumer_config (void);

    virtual void connect_event_supplier (
      CIAO::Supplier_Config_ptr supplier_config);

    virtual void connect_event_consumer (
      CIAO::Consumer_Config_ptr consumer_config);

    virtual void disconnect_event_supplier (const char * consumer_id);
    virtual void disconnect_event_consumer (const char * connection_id);

    virtual void push_event (Components::EventBase * ev);

    virtual void ciao_push_event (Components::EventBase * evt,
                                  const char * source_id,
                                  CORBA::TypeCode_ptr tc);

    virtual ::CORBA::Boolean create_addr_serv (const char * name,
                                               ::CORBA::UShort port,
                                               const char * address);

    virtual ::CORBA::Boolean create_sender (const char * addr_serv_id);

    virtual ::CORBA::Boolean create_receiver (const char * addr_serv_id,
                                              ::CORBA::Boolean is_multicast,
                                              ::CORBA::UShort listen_port);

    virtual
    ::RtecEventChannelAdmin::EventChannel_ptr tao_rt_event_channel (void);

  private:
    // @@ (GD) This is the place where use could provide a parameter
    //         which specifies the event channel service configuration file.
    void create_rt_event_channel (const char * ec_name);

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
     * @var ACE_Hash_Map_Manager<> proxy_supplier_map_
     *
     * Mapping of each event sink to a proxy supplier for disconnect purposes.
     */
    ACE_Hash_Map_Manager_Ex<ACE_CString,
                            RtecEventChannelAdmin::ProxyPushConsumer_var,
                            ACE_Hash<ACE_CString>,
                            ACE_Equal_To<ACE_CString>,
                            ACE_Null_Mutex>
      proxy_consumer_map_;

    /**
     * @var ACE_Hash_Map_Manager<> proxy_supplier_map_
     *
     * Mapping of each event sink to a proxy supplier for disconnect purposes.
     */
    ACE_Hash_Map_Manager_Ex<ACE_CString,
                            RtecEventChannelAdmin::ProxyPushSupplier_var,
                            ACE_Hash<ACE_CString>,
                            ACE_Equal_To<ACE_CString>,
                            ACE_Null_Mutex>
      proxy_supplier_map_;

    /**
     * @var ACE_Hash_Map_Manager<> addr_serv_map_
     *
     * A map which managers a set of address servers for event channel
     * federation purpose.
     */
    ACE_Hash_Map_Manager_Ex<ACE_CString,
                            RtecUDPAdmin::AddrServer_var,
                            ACE_Hash<ACE_CString>,
                            ACE_Equal_To<ACE_CString>,
                            ACE_Null_Mutex>
      addr_serv_map_;
  };

  /**
   * @class RTEventServiceSupplier_impl
   *
   * An implementation of the PushSupplier interface.
   */
  class RTEventServiceSupplier_impl
    : public virtual POA_RtecEventComm::PushSupplier
  {
  public:
    RTEventServiceSupplier_impl (PortableServer::POA_ptr poa);

    virtual void disconnect_push_supplier (void);

  private:
    PortableServer::POA_var poa_;
  };

  /**
   * @class RTEventServiceConsumer_impl
   *
   * An implementation of the PushConsumer interface.
   */
  class RTEventServiceConsumer_impl
    : public virtual POA_RtecEventComm::PushConsumer
  {
  public:
    RTEventServiceConsumer_impl (PortableServer::POA_ptr poa,
                                 Components::EventConsumerBase_ptr consumer);

    virtual void push (const RtecEventComm::EventSet& events);

    virtual void disconnect_push_consumer (void);

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
  class RTEvent_Consumer_Config_impl
    : public virtual POA_CIAO::RTEvent_Consumer_Config,
      public virtual Event_Consumer_Config_Base
  {
  public:
    RTEvent_Consumer_Config_impl (PortableServer::POA_ptr poa);

    virtual ~RTEvent_Consumer_Config_impl (void);

    virtual void start_conjunction_group (CORBA::Long size);

    virtual void start_disjunction_group (CORBA::Long size);

    virtual void insert_source (const char * source_id);

    virtual void insert_type (::CORBA::Long event_type);

    virtual void consumer_id (const char * consumer_id);

    virtual CONNECTION_ID consumer_id ();

    virtual void consumer (Components::EventConsumerBase_ptr consumer);

    virtual Components::EventConsumerBase_ptr consumer ();

    virtual EventServiceType service_type ();

    //@@ (GD) There should be a place where the deployment tool could
    //        set up the rt_event_qos properties for Consumer Config.

    virtual RtecEventChannelAdmin::ConsumerQOS * rt_event_qos (void);

    virtual void destroy (void);

  private:
    ACE_CString consumer_id_;

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
  class RTEvent_Supplier_Config_impl
    : public virtual POA_CIAO::RTEvent_Supplier_Config
  {
  public:
    RTEvent_Supplier_Config_impl (PortableServer::POA_ptr poa);

    virtual ~RTEvent_Supplier_Config_impl (void);

    void supplier_id (const char * supplier_id);

    CONNECTION_ID supplier_id (void);

    EventServiceType service_type (void);

    //@@ (GD) There should be a place where the deployment tool could
    //        set up the rt_event_qos properties for Supplier Config.

    RtecEventChannelAdmin::SupplierQOS * rt_event_qos (void);

    virtual void destroy (void);

  private:
    ACE_CString supplier_id_;

    EventServiceType service_type_;

    ACE_SupplierQOS_Factory qos_;

    PortableServer::POA_var poa_;
  };
}

#include /**/ "ace/post.h"

#endif /* CIAO_RTEVENT_H */
