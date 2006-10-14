// -*- C++ -*-

//=============================================================================
/**
 *  @file CIAO_RTEvent.cpp
 *
 *  $Id$
 *
 *  @author Gan Deng <dengg@dre.vanderbilt.edu>
 *  @author George Edwards <g.edwards@vanderbilt.edu>
 */
//=============================================================================

#include "CIAO_RTEvent.h"
#include "ciao/CIAO_common.h"
#include "SimpleAddressServer.h"
#include <tao/ORB_Core.h>

#include <sstream>

namespace CIAO
{

  RTEventService::RTEventService (CORBA::ORB_ptr orb,
                                  PortableServer::POA_ptr poa) :
    orb_ (CORBA::ORB::_duplicate (orb)),
    root_poa_ (PortableServer::POA::_duplicate (poa)),
    type_id_ (ACE_ES_EVENT_ANY),
    source_id_ (ACE_ES_EVENT_SOURCE_ANY)
  {
    this->create_rt_event_channel ();
  }


  RTEventService::~RTEventService (void)
  {
  }


  Supplier_Config_ptr
  RTEventService::create_supplier_config (void)
    ACE_THROW_SPEC ((CORBA::SystemException))
  {
    RTEvent_Supplier_Config_impl * supplier_config = 0;
    ACE_NEW_RETURN (supplier_config,
                    RTEvent_Supplier_Config_impl (this->root_poa_.in ()),
                    Supplier_Config::_nil ());
    RTEvent_Supplier_Config_var return_rtec =
      supplier_config->_this (ACE_ENV_SINGLE_ARG_PARAMETER);
    ACE_CHECK;
    return return_rtec._retn ();
  }


  Consumer_Config_ptr
  RTEventService::create_consumer_config (void)
    ACE_THROW_SPEC ((CORBA::SystemException))
  {
    RTEvent_Consumer_Config_impl * consumer_config = 0;
    ACE_NEW_RETURN (consumer_config,
                    RTEvent_Consumer_Config_impl (this->root_poa_.in ()),
                    Consumer_Config::_nil ());
    RTEvent_Consumer_Config_var return_rtec =
      consumer_config->_this (ACE_ENV_SINGLE_ARG_PARAMETER);
    ACE_CHECK;
    return return_rtec._retn ();
  }


  // @@TODO: We might want to maintain a map for managing multiple proxy consumers
  // to multiple event suppliers.
  void
  RTEventService::connect_event_supplier (
      Supplier_Config_ptr supplier_config
      ACE_ENV_ARG_DECL)
    ACE_THROW_SPEC ((
      CORBA::SystemException))
  {
    if (CIAO::debug_level () > 9)
      {
        ACE_DEBUG ((LM_DEBUG, "CIAO::RTEventService::connect_event_supplier\n"));
      }

    RTEvent_Supplier_Config_ptr rt_config =
      RTEvent_Supplier_Config::_narrow (supplier_config
                                        ACE_ENV_ARG_PARAMETER);
    ACE_CHECK;

    if (CORBA::is_nil (rt_config))
      {
        ACE_THROW (CORBA::BAD_PARAM ());
      }

    ACE_Hash<ACE_CString> hasher;
    this->source_id_ = hasher (supplier_config->supplier_id (ACE_ENV_SINGLE_ARG_PARAMETER));

    ACE_CHECK;
    this->type_id_ = this->source_id_;

    if (CIAO::debug_level () > 11)
      {
        ACE_DEBUG ((LM_DEBUG, "connect source id: %i\n", this->source_id_));
      }

    RtecEventChannelAdmin::SupplierAdmin_var supplier_admin =
      this->rt_event_channel_->for_suppliers (ACE_ENV_SINGLE_ARG_PARAMETER);
    ACE_CHECK;

    this->proxy_consumer_ =
      supplier_admin->obtain_push_consumer (ACE_ENV_SINGLE_ARG_PARAMETER);
    ACE_CHECK;

    // Create and register supplier servant
    RTEventServiceSupplier_impl * supplier_servant = 0;
    ACE_NEW (supplier_servant,
             RTEventServiceSupplier_impl (root_poa_.in ()));
    RtecEventComm::PushSupplier_var push_supplier =
      supplier_servant->_this (ACE_ENV_SINGLE_ARG_PARAMETER);
    ACE_CHECK;

    RtecEventChannelAdmin::SupplierQOS_var qos =
      rt_config->rt_event_qos (ACE_ENV_SINGLE_ARG_PARAMETER);
    ACE_CHECK;

    this->proxy_consumer_->connect_push_supplier (push_supplier.in (),
                                                  qos.in ()
                                                  ACE_ENV_ARG_PARAMETER);
    ACE_CHECK;
  }

  void
  RTEventService::connect_event_consumer (
      Consumer_Config_ptr consumer_config
      ACE_ENV_ARG_DECL)
    ACE_THROW_SPEC ((
      CORBA::SystemException))
  {
    if (CIAO::debug_level () > 9)
      {
        ACE_DEBUG ((LM_DEBUG, "CIAO::RTEventService::connect_event_consumer\n"));
      }

    RTEvent_Consumer_Config_ptr rt_config =
      RTEvent_Consumer_Config::_narrow (consumer_config
                                        ACE_ENV_ARG_PARAMETER);
    ACE_CHECK;

    if (CORBA::is_nil (rt_config))
      {
        ACE_THROW (CORBA::BAD_PARAM ());
      }

    Components::EventConsumerBase_var consumer =
      consumer_config->consumer (ACE_ENV_SINGLE_ARG_PARAMETER);
    ACE_CHECK;

    if (CORBA::is_nil (consumer.in ()))
      ACE_DEBUG ((LM_DEBUG, "nil event consumer\n"));

    RtecEventChannelAdmin::ConsumerAdmin_var consumer_admin =
      this->rt_event_channel_->for_consumers (ACE_ENV_SINGLE_ARG_PARAMETER);
    ACE_CHECK;

    RtecEventChannelAdmin::ProxyPushSupplier_var proxy_supplier =
      consumer_admin->obtain_push_supplier (ACE_ENV_SINGLE_ARG_PARAMETER);
    ACE_CHECK;

    // Create and register consumer servant
    RTEventServiceConsumer_impl * consumer_servant = 0;
    ACE_NEW (consumer_servant,
             RTEventServiceConsumer_impl (
               root_poa_.in (),
               consumer.in ()));
    RtecEventComm::PushConsumer_var push_consumer =
      consumer_servant->_this (ACE_ENV_SINGLE_ARG_PARAMETER);
    ACE_CHECK;

    RtecEventChannelAdmin::ConsumerQOS_var qos =
      rt_config->rt_event_qos (ACE_ENV_SINGLE_ARG_PARAMETER);
    ACE_CHECK;

    ACE_DEBUG ((LM_DEBUG, "\n======== ConsumerQoS length is: %d\n\n", 
                qos->dependencies.length ()));

    if (qos->dependencies.length () == 0)
      {
        qos->dependencies.length (1);
        qos->dependencies[0].event.header.type = ACE_ES_EVENT_ANY;
        qos->dependencies[0].event.header.source = ACE_ES_EVENT_SOURCE_ANY;
        qos->dependencies[0].rt_info = 0;

        ACE_DEBUG ((LM_DEBUG, "\n======== Normalized ConsumerQoS length is: %d\n\n", 
                    qos->dependencies.length ()));
      }

    proxy_supplier->connect_push_consumer (push_consumer.in (),
                                           qos.in ()
                                           //qos_factory.get_ConsumerQOS ()
                                           ACE_ENV_ARG_PARAMETER);
    ACE_CHECK;

    ACE_CString consumer_id =
      consumer_config->consumer_id (ACE_ENV_SINGLE_ARG_PARAMETER);
    ACE_CHECK;

    this->proxy_supplier_map_.bind (consumer_id.c_str (), proxy_supplier._retn ());
  }

  void
  RTEventService::disconnect_event_supplier (
      const char * connection_id
      ACE_ENV_ARG_DECL)
    ACE_THROW_SPEC ((
      CORBA::SystemException,
      Components::InvalidConnection))
  {
    ACE_UNUSED_ARG (connection_id);

    /* @todo
    if (! CORBA::is_nil (this->proxy_consumer_.in ()))
      {
        this->proxy_consumer_->disconnect_push_consumer (
          ACE_ENV_SINGLE_ARG_PARAMETER);
        ACE_CHECK;
      }

    // What to do with the consumers?!
    */
  }

  void
  RTEventService::disconnect_event_consumer (
      const char * connection_id
      ACE_ENV_ARG_DECL)
    ACE_THROW_SPEC ((
      CORBA::SystemException,
      Components::InvalidConnection))
  {
    RtecEventChannelAdmin::ProxyPushSupplier_var proxy_supplier;

    this->proxy_supplier_map_.unbind (connection_id, proxy_supplier);

    proxy_supplier->disconnect_push_supplier (
      ACE_ENV_SINGLE_ARG_PARAMETER);
    ACE_CHECK;
  }

  void
  RTEventService::push_event (
      Components::EventBase * ev
      ACE_ENV_ARG_DECL)
    ACE_THROW_SPEC ((
      CORBA::SystemException))
  {
    if (CIAO::debug_level () > 10)
      {
        ACE_DEBUG ((LM_DEBUG, "------CIAO::RTEventService::push_event------\n"));
      }

    RtecEventComm::EventSet events (1);
    events.length (1);
    events[0].header.source = ACE_ES_EVENT_SOURCE_ANY; //this->source_id_;
    events[0].header.type = ACE_ES_EVENT_ANY; //this->type_id_;
    events[0].data.any_value <<= ev;

    this->proxy_consumer_->push (events ACE_ENV_ARG_PARAMETER);
    ACE_CHECK;
  }

  void
  RTEventService::ciao_push_event (
      Components::EventBase * ev,
      const char * source_id
      ACE_ENV_ARG_DECL)
    ACE_THROW_SPEC ((
      CORBA::SystemException,
      Components::BadEventType))
  {
    if (CIAO::debug_level () > 10)
      {
        ACE_DEBUG ((LM_DEBUG, "------CIAO::RTEventService::ciao_push_event------\n"));
      }
    RtecEventComm::EventSet events (1);
    events.length (1);
    
    ACE_Hash<ACE_CString> hasher;

    events[0].header.source = hasher (source_id);
    //events[0].header.source = i++;

    events[0].header.type = ACE_ES_EVENT_ANY; //this->type_id_;
    events[0].data.any_value <<= ev;

    ACE_DEBUG ((LM_DEBUG, "******* push event for source string: %s\n", source_id));
    ACE_DEBUG ((LM_DEBUG, "******* push event for source id: %i\n", events[0].header.source));

    this->proxy_consumer_->push (events ACE_ENV_ARG_PARAMETER);
    ACE_CHECK;
  }

  void
  RTEventService::create_rt_event_channel (
      ACE_ENV_SINGLE_ARG_DECL)
    ACE_THROW_SPEC ((
      CORBA::SystemException))
  {
    if (CIAO::debug_level () > 10)
      {
        ACE_DEBUG ((LM_DEBUG, "CIAO::EventService_Factory_impl::create_rt_event_channel\n"));
      }

    // @@ (GD) Anything else to do to get the svc.conf file options?
    TAO_EC_Default_Factory::init_svcs ();

    TAO_EC_Event_Channel_Attributes attributes (this->root_poa_.in (),
                                                this->root_poa_.in ());
    TAO_EC_Event_Channel * ec_servant = 0;
    ACE_NEW (ec_servant, TAO_EC_Event_Channel (attributes));
    ec_servant->activate (ACE_ENV_SINGLE_ARG_PARAMETER);
    ACE_CHECK;
    this->rt_event_channel_ = ec_servant->_this (ACE_ENV_SINGLE_ARG_PARAMETER);
    ACE_CHECK;
  }

  ::CORBA::Boolean
  RTEventService::create_addr_serv (
        const char * name,
        ::CORBA::UShort port,
        const char * address
        ACE_ENV_ARG_DECL_WITH_DEFAULTS)
      ACE_THROW_SPEC ((
        ::CORBA::SystemException))
  {
    // Initialize the address server with the desired address.
    // This will be used by the sender object and the multicast
    // receiver.
    
    ACE_INET_Addr send_addr (port, address);
    //ACE_INET_Addr send_addr (10001, ACE_DEFAULT_MULTICAST_ADDR);

    SimpleAddressServer * addr_srv_impl = new SimpleAddressServer (send_addr);

    PortableServer::ObjectId_var addr_srv_oid =
      this->root_poa_->activate_object (addr_srv_impl);

    CORBA::Object_var addr_srv_obj = 
      this->root_poa_->id_to_reference (addr_srv_oid.in());

    RtecUDPAdmin::AddrServer_var addr_srv =
      RtecUDPAdmin::AddrServer::_narrow (addr_srv_obj.in());

    this->addr_serv_map_.bind (
      name, 
      RtecUDPAdmin::AddrServer::_duplicate (addr_srv.in ()));

    return true;
  }

  ::CORBA::Boolean
  RTEventService::create_sender (
        const char * addr_serv_id
        ACE_ENV_ARG_DECL_WITH_DEFAULTS)
      ACE_THROW_SPEC ((
        ::CORBA::SystemException))
  {
    // Create and initialize the sender object
    TAO_EC_Servant_Var<TAO_ECG_UDP_Sender> sender =
                                TAO_ECG_UDP_Sender::create();

    TAO_ECG_UDP_Out_Endpoint endpoint;

    if (endpoint.dgram ().open (ACE_Addr::sap_any) == -1) 
      {
        ACE_DEBUG ((LM_ERROR, "Cannot open send endpoint\n"));
        return false;
      }

    // TAO_ECG_UDP_Sender::init() takes a TAO_ECG_Refcounted_Endpoint.
    // If we don't clone our endpoint and pass &endpoint, the sender will
    // attempt to delete endpoint during shutdown.
    TAO_ECG_UDP_Out_Endpoint* clone;
    ACE_NEW_RETURN (clone,
                    TAO_ECG_UDP_Out_Endpoint (endpoint),
                    false);

    RtecUDPAdmin::AddrServer_var addr_srv;
    if (this->addr_serv_map_.find (addr_serv_id, addr_srv) != 0)
      return false;

    sender->init (this->rt_event_channel_.in (), 
                  addr_srv.in (),
                  clone);

    // Setup the subscription and connect to the EC
    ACE_ConsumerQOS_Factory cons_qos_fact;
    cons_qos_fact.start_disjunction_group ();
    cons_qos_fact.insert (ACE_ES_EVENT_SOURCE_ANY, ACE_ES_EVENT_ANY, 0);
    RtecEventChannelAdmin::ConsumerQOS sub = cons_qos_fact.get_ConsumerQOS ();
    sender->connect (sub);

    return true;
  }

  ::CORBA::Boolean
  RTEventService::create_receiver (
        const char * addr_serv_id,
        ::CORBA::Boolean is_multicast,
        ::CORBA::UShort listen_port
        ACE_ENV_ARG_DECL_WITH_DEFAULTS)
      ACE_THROW_SPEC ((
        ::CORBA::SystemException))
  {
    // Create and initialize the receiver
    TAO_EC_Servant_Var<TAO_ECG_UDP_Receiver> receiver =
                                      TAO_ECG_UDP_Receiver::create();

    TAO_ECG_UDP_Out_Endpoint endpoint;
    if (endpoint.dgram ().open (ACE_Addr::sap_any) == -1) 
      {
        ACE_DEBUG ((LM_ERROR, "Cannot open send endpoint\n"));
        return false;
      }

    // TAO_ECG_UDP_Receiver::init() takes a TAO_ECG_Refcounted_Endpoint.
    // If we don't clone our endpoint and pass &endpoint, the receiver will
    // attempt to delete endpoint during shutdown.
    TAO_ECG_UDP_Out_Endpoint* clone;
    ACE_NEW_RETURN (clone,
                    TAO_ECG_UDP_Out_Endpoint (endpoint),
                    false);

    RtecUDPAdmin::AddrServer_var addr_srv;

    // AddressServer is necessary when "multicast" is enabled, but not for "udp"
    if (is_multicast)
      {
        if (this->addr_serv_map_.find (addr_serv_id, addr_srv) != 0)
          return false;

        receiver->init (this->rt_event_channel_.in (), 
                        clone,
                        addr_srv.in ());
      }
    else
      {
        receiver->init (this->rt_event_channel_.in (), clone, 0);
      }

    // Setup the registration and connect to the event channel
    ACE_SupplierQOS_Factory supp_qos_fact;
    supp_qos_fact.insert (ACE_ES_EVENT_SOURCE_ANY, ACE_ES_EVENT_ANY, 0, 1);
    RtecEventChannelAdmin::SupplierQOS pub = supp_qos_fact.get_SupplierQOS ();
    receiver->connect (pub);

    // Create the appropriate event handler and register it with the reactor
    auto_ptr<ACE_Event_Handler> eh;

    if (is_multicast) 
      {
        auto_ptr<TAO_ECG_Mcast_EH> mcast_eh (new TAO_ECG_Mcast_EH (receiver.in()));
        mcast_eh->reactor (this->orb_->orb_core ()->reactor ());
        mcast_eh->open (this->rt_event_channel_.in());
        ACE_AUTO_PTR_RESET (eh, mcast_eh.release(), ACE_Event_Handler);
      } 
    else 
      {
        ACE_DEBUG ((LM_DEBUG, "\nUDP Event Hander Port [%d]\n\n", listen_port));

        auto_ptr<TAO_ECG_UDP_EH> udp_eh (new TAO_ECG_UDP_EH (receiver.in()));
        udp_eh->reactor (this->orb_->orb_core ()->reactor ());

        ACE_INET_Addr local_addr (listen_port);
        if (udp_eh->open (local_addr) == -1)
          {
            ACE_DEBUG ((LM_ERROR, "Cannot open event handler on port [%d]\n", listen_port));
            return false;
          }
        ACE_AUTO_PTR_RESET (eh, udp_eh.release(), ACE_Event_Handler);
      }

    return true;
  }


  //////////////////////////////////////////////////////////////////////
  ///                 Supplier Servant Implementation
  //////////////////////////////////////////////////////////////////////

  RTEventServiceSupplier_impl::RTEventServiceSupplier_impl (
    PortableServer::POA_ptr poa) :
  poa_ (PortableServer::POA::_duplicate (poa))
  {
  }

  void
  RTEventServiceSupplier_impl::disconnect_push_supplier (void)
    ACE_THROW_SPEC ((
         CORBA::SystemException))
  {
    PortableServer::ObjectId_var oid = this->poa_->servant_to_id (this);
    this->poa_->deactivate_object (oid);
    this->_remove_ref ();
  }

  //////////////////////////////////////////////////////////////////////
  ///                   Consumer Servant Implementation
  //////////////////////////////////////////////////////////////////////

  RTEventServiceConsumer_impl::RTEventServiceConsumer_impl (
    PortableServer::POA_ptr poa,
    Components::EventConsumerBase_ptr consumer) :
  poa_ (PortableServer::POA::_duplicate (poa)),
  event_consumer_ (Components::EventConsumerBase::_duplicate (consumer))
  {
  }

  void
  RTEventServiceConsumer_impl::push (const RtecEventComm::EventSet& events)
    ACE_THROW_SPEC ((CORBA::SystemException))
  {
    if (CIAO::debug_level () > 10)
      {
        ACE_DEBUG ((LM_DEBUG, "CIAO::RTEventServiceConsumer_impl::push\n"));
      }

    for (size_t i = 0; i < events.length (); ++i)
      {
        std::ostringstream out;
        out << "Received event,"
            << "  type: "   << events[i].header.type
            << "  source: " << events[i].header.source;

        ACE_OS::printf("%s\n", out.str().c_str()); // printf is synchronized 

        Components::EventBase * ev = 0;
        if (events[i].data.any_value >>= ev)
          {
            ev->_add_ref ();
            this->event_consumer_->push_event (ev
                                               ACE_ENV_ARG_PARAMETER);
            ACE_CHECK;
          }
      }

  }

  void
  RTEventServiceConsumer_impl::disconnect_push_consumer (void)
    ACE_THROW_SPEC ((CORBA::SystemException))
  {
    if (CIAO::debug_level () > 10)
      {
        ACE_DEBUG ((LM_DEBUG, "CIAO::RTEventServiceConsumer_impl::disconnect_push_consumer\n"));
      }

    PortableServer::ObjectId_var oid = this->poa_->servant_to_id (this);
    this->poa_->deactivate_object (oid);
    this->_remove_ref ();
  }


  //////////////////////////////////////////////////////////////////////
  ///                   Supplier Config Implementation
  //////////////////////////////////////////////////////////////////////

  RTEvent_Supplier_Config_impl::RTEvent_Supplier_Config_impl (PortableServer::POA_ptr poa) :
    service_type_ (RTEC),
    poa_ (PortableServer::POA::_duplicate (poa))
  {
  }

  RTEvent_Supplier_Config_impl::~RTEvent_Supplier_Config_impl (void)
  {
    if (CIAO::debug_level () > 10)
      {
        ACE_DEBUG
          ((LM_DEBUG, "RTEvent_Supplier_Config_impl::~RTEvent_Supplier_Config_impl\n"));
      }
  }

  void
  RTEvent_Supplier_Config_impl::supplier_id (
      const char * supplier_id
      ACE_ENV_ARG_DECL)
    ACE_THROW_SPEC ((
      CORBA::SystemException))
  {
  if (CIAO::debug_level () > 10)
    {
      ACE_DEBUG ((LM_DEBUG, "supplier's id: %s\n", supplier_id));
    }

    this->supplier_id_ = supplier_id;

    ACE_Hash<ACE_CString> hasher;

    RtecEventComm::EventSourceID source_id =
      hasher (this->supplier_id_.c_str ());
/*
    this->qos_.insert (source_id,
                       ACE_ES_EVENT_ANY,
                       0,
                       1);

*/

    this->qos_.insert (ACE_ES_EVENT_SOURCE_ANY,
                       ACE_ES_EVENT_ANY,
                       0,    // handle to the rt_info structure
                       1);        

    ACE_DEBUG ((LM_DEBUG, "supplier's source id is: %d\n", source_id));
  }

  CONNECTION_ID
  RTEvent_Supplier_Config_impl::supplier_id (
      ACE_ENV_SINGLE_ARG_DECL)
    ACE_THROW_SPEC ((
      CORBA::SystemException))
  {
    return CORBA::string_dup (this->supplier_id_.c_str ());
  }

  EventServiceType
  RTEvent_Supplier_Config_impl::service_type (
      ACE_ENV_SINGLE_ARG_DECL)
    ACE_THROW_SPEC ((
      CORBA::SystemException))
  {
    return this->service_type_;
  }

  RtecEventChannelAdmin::SupplierQOS *
  RTEvent_Supplier_Config_impl::rt_event_qos (
      ACE_ENV_SINGLE_ARG_DECL)
    ACE_THROW_SPEC ((
      CORBA::SystemException))
  {
    RtecEventChannelAdmin::SupplierQOS * supplier_qos = 0;
    ACE_NEW_RETURN (supplier_qos,
                    RtecEventChannelAdmin::SupplierQOS (this->qos_.get_SupplierQOS ()),
                    0);
    return supplier_qos;
  }

  void
  RTEvent_Supplier_Config_impl::destroy (
      ACE_ENV_SINGLE_ARG_DECL)
    ACE_THROW_SPEC ((
      CORBA::SystemException))
  {
    PortableServer::ObjectId_var oid = this->poa_->servant_to_id (this);
    this->poa_->deactivate_object (oid);
    this->_remove_ref ();
  }

  //////////////////////////////////////////////////////////////////////
  ///                   Consumer Config Implementation
  //////////////////////////////////////////////////////////////////////

  RTEvent_Consumer_Config_impl::RTEvent_Consumer_Config_impl (PortableServer::POA_ptr poa) :
    service_type_ (RTEC),
    poa_ (PortableServer::POA::_duplicate (poa))
  {
  }

  RTEvent_Consumer_Config_impl::~RTEvent_Consumer_Config_impl (void)
  {
    ACE_DEBUG
      ((LM_DEBUG, "RTEvent_Consumer_Config_impl::~RTEvent_Consumer_Config_impl\n"));
  }

  void
  RTEvent_Consumer_Config_impl::start_conjunction_group (
      CORBA::Long size ACE_ENV_ARG_DECL)
    ACE_THROW_SPEC ((
      CORBA::SystemException))
  {
    ACE_DEBUG
      ((LM_DEBUG, "RTEvent_Consumer_Config_impl::start_conjunction_group\n"));

    this->qos_.start_conjunction_group (size);
  }

  void
  RTEvent_Consumer_Config_impl::start_disjunction_group (
      CORBA::Long size
      ACE_ENV_ARG_DECL)
    ACE_THROW_SPEC ((
      CORBA::SystemException))
  {
    // Note, since we only support basic builder here...
    if (size == 0L)
      this->qos_.start_disjunction_group ();
    else
      this->qos_.start_disjunction_group (size);
  }

  void
  RTEvent_Consumer_Config_impl::insert_source (
      const char * source_id
      ACE_ENV_ARG_DECL)
    ACE_THROW_SPEC ((
      CORBA::SystemException))
  {
    ACE_Hash<ACE_CString> hasher;
    RtecEventComm::EventSourceID int_source_id = hasher (source_id);

    ACE_DEBUG ((LM_DEBUG, "******* the source string is: %s\n", source_id));
    ACE_DEBUG ((LM_DEBUG, "******* the source id is: %i\n", int_source_id));

    this->qos_.insert_source (int_source_id, 0);
  }

  void
  RTEvent_Consumer_Config_impl::insert_type (
        ::CORBA::Long event_type
        ACE_ENV_ARG_DECL)
      ACE_THROW_SPEC ((::CORBA::SystemException))
  {
    if (event_type == 0L)
      this->qos_.insert_type (ACE_ES_EVENT_ANY, 0);
    else
      this->qos_.insert_type (event_type,
                              0);
  }

  void
  RTEvent_Consumer_Config_impl::consumer_id (
      const char * consumer_id
      ACE_ENV_ARG_DECL)
    ACE_THROW_SPEC ((
      CORBA::SystemException))
  {
    if (CIAO::debug_level () > 10)
      {
        ACE_DEBUG ((LM_DEBUG,
                    "RTEvent_Consumer_Config_impl::set_consumer_id:%s\n",
                    consumer_id));
      }

    this->consumer_id_ = consumer_id;
  }


  void
  RTEvent_Consumer_Config_impl::consumer (
      Components::EventConsumerBase_ptr consumer
      ACE_ENV_ARG_DECL)
    ACE_THROW_SPEC ((
      CORBA::SystemException))
  {
    this->consumer_ = Components::EventConsumerBase::_duplicate (consumer);
  }

  CONNECTION_ID
  RTEvent_Consumer_Config_impl::consumer_id (
      ACE_ENV_SINGLE_ARG_DECL)
    ACE_THROW_SPEC ((
      CORBA::SystemException))
  {
    return CORBA::string_dup (this->consumer_id_.c_str ());
  }


  EventServiceType
  RTEvent_Consumer_Config_impl::service_type (
      ACE_ENV_SINGLE_ARG_DECL)
    ACE_THROW_SPEC ((
      CORBA::SystemException))
  {
    return this->service_type_;
  }

  Components::EventConsumerBase_ptr
  RTEvent_Consumer_Config_impl::consumer (
      ACE_ENV_SINGLE_ARG_DECL)
    ACE_THROW_SPEC ((
      CORBA::SystemException))
  {
    if (CIAO::debug_level () > 10)
      {
        ACE_DEBUG ((LM_DEBUG, "RTEvent_Consumer_Config_impl::get_consumer\n"));
      }

    return Components::EventConsumerBase::_duplicate (this->consumer_.in ());
  }

  RtecEventChannelAdmin::ConsumerQOS *
  RTEvent_Consumer_Config_impl::rt_event_qos (
      ACE_ENV_SINGLE_ARG_DECL)
    ACE_THROW_SPEC ((
      CORBA::SystemException))
  {
    RtecEventChannelAdmin::ConsumerQOS * consumer_qos = 0;
    ACE_NEW_RETURN (consumer_qos,
                    RtecEventChannelAdmin::ConsumerQOS (this->qos_.get_ConsumerQOS ()),
                    0);

    return consumer_qos;
  }

  void
  RTEvent_Consumer_Config_impl::destroy (
      ACE_ENV_SINGLE_ARG_DECL)
    ACE_THROW_SPEC ((
      CORBA::SystemException))
  {
    if (CIAO::debug_level () > 10)
      {
        ACE_DEBUG
          ((LM_DEBUG, "RTEvent_Consumer_Config_impl::destroy\n"));
      }

    PortableServer::ObjectId_var oid = this->poa_->servant_to_id (this);
    this->poa_->deactivate_object (oid);
    this->_remove_ref ();
  }
}
