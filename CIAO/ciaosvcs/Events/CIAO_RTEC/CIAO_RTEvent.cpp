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
#include "SimpleAddressServer.h"

#include "ciao/CIAO_common.h"

#include "tao/AnyTypeCode/Any_Unknown_IDL_Type.h"

#include "tao/ORB_Core.h"

#include <sstream>

namespace CIAO
{
  RTEventService::RTEventService (CORBA::ORB_ptr orb,
                                  PortableServer::POA_ptr poa,
                                  const char * ec_name) :
    orb_ (CORBA::ORB::_duplicate (orb)),
    root_poa_ (PortableServer::POA::_duplicate (poa))
  {
    this->create_rt_event_channel (ec_name);
  }

  RTEventService::~RTEventService (void)
  {
  }

  Supplier_Config_ptr
  RTEventService::create_supplier_config (void)
  {
    RTEvent_Supplier_Config_impl * supplier_config = 0;
    ACE_NEW_RETURN (supplier_config,
                    RTEvent_Supplier_Config_impl (this->root_poa_.in ()),
                    Supplier_Config::_nil ());
    RTEvent_Supplier_Config_var return_rtec =
      supplier_config->_this ();
    return return_rtec._retn ();
  }


  Consumer_Config_ptr
  RTEventService::create_consumer_config (void)
  {
    RTEvent_Consumer_Config_impl * consumer_config = 0;
    ACE_NEW_RETURN (consumer_config,
                    RTEvent_Consumer_Config_impl (this->root_poa_.in ()),
                    Consumer_Config::_nil ());
    RTEvent_Consumer_Config_var return_rtec =
      consumer_config->_this ();
    return return_rtec._retn ();
  }


  // @@TODO: We might want to maintain a map for managing multiple proxy consumers
  // to multiple event suppliers.
  void
  RTEventService::connect_event_supplier (
    Supplier_Config_ptr supplier_config)
  {
    ACE_DEBUG ((LM_TRACE, CLINFO
                "CIAO::RTEventService::connect_event_supplier\n"));

    RTEvent_Supplier_Config_ptr rt_config =
      RTEvent_Supplier_Config::_narrow (supplier_config);

    if (CORBA::is_nil (rt_config))
      {
        throw CORBA::BAD_PARAM ();
      }

    // Get a proxy push consumer from the EventChannel.
    RtecEventChannelAdmin::SupplierAdmin_var supplier_admin =
      this->rt_event_channel_->for_suppliers ();

    RtecEventChannelAdmin::ProxyPushConsumer_var proxy_push_consumer =
      supplier_admin->obtain_push_consumer();

    // Create and register supplier servant
    RTEventServiceSupplier_impl * supplier_servant = 0;
    ACE_NEW (supplier_servant,
             RTEventServiceSupplier_impl (root_poa_.in ()));
    RtecEventComm::PushSupplier_var push_supplier =
      supplier_servant->_this ();

    RtecEventChannelAdmin::SupplierQOS_var qos =
      rt_config->rt_event_qos ();

    ACE_SupplierQOS_Factory supplier_qos;
    supplier_qos.insert (ACE_ES_EVENT_SOURCE_ANY, ACE_ES_EVENT_ANY, 0, 1);

    supplier_qos.insert (ACE_ES_EVENT_SOURCE_ANY,
                         ACE_ES_EVENT_ANY,
                         0,    // handle to the rt_info structure
                         1);

    proxy_push_consumer->connect_push_supplier (
      push_supplier.in (),
      supplier_qos.get_SupplierQOS ());

    this->proxy_consumer_map_.bind (supplier_config->supplier_id (),
                                    proxy_push_consumer._retn ());
  }

  void
  RTEventService::connect_event_consumer (
    Consumer_Config_ptr consumer_config)
  {
    ACE_DEBUG ((LM_TRACE, CLINFO 
                "CIAO::RTEventService::connect_event_consumer\n"));

    RTEvent_Consumer_Config_ptr rt_config =
      RTEvent_Consumer_Config::_narrow (consumer_config);

    if (CORBA::is_nil (rt_config))
      {
        throw CORBA::BAD_PARAM ();
      }

    Components::EventConsumerBase_var consumer =
      consumer_config->consumer ();

    if (CORBA::is_nil (consumer.in ()))
      {
        ACE_DEBUG ((LM_DEBUG, "nil event consumer\n"));
      }

    RtecEventChannelAdmin::ConsumerAdmin_var consumer_admin =
      this->rt_event_channel_->for_consumers ();

    RtecEventChannelAdmin::ProxyPushSupplier_var proxy_supplier =
      consumer_admin->obtain_push_supplier ();

    // Create and register consumer servant.
    RTEventServiceConsumer_impl * consumer_servant = 0;
    ACE_NEW (consumer_servant,
             RTEventServiceConsumer_impl (
               root_poa_.in (),
               consumer.in ()));
    RtecEventComm::PushConsumer_var push_consumer =
      consumer_servant->_this ();

    RtecEventChannelAdmin::ConsumerQOS_var qos =
      rt_config->rt_event_qos ();

    ACE_DEBUG ((LM_DEBUG,
                "\n======== ConsumerQoS length is: %d\n\n",
                qos->dependencies.length ()));

    if (qos->dependencies.length () == 0)
      {
        qos->dependencies.length (1);
        qos->dependencies[0].event.header.type = ACE_ES_EVENT_ANY;
        qos->dependencies[0].event.header.source = ACE_ES_EVENT_SOURCE_ANY;
        qos->dependencies[0].rt_info = 0;

        ACE_DEBUG ((LM_DEBUG,
                    "\n======== Normalized ConsumerQoS length is: %d\n\n",
                    qos->dependencies.length ()));
      }

    proxy_supplier->connect_push_consumer (push_consumer.in (),
                                           qos.in ());

    ACE_CString consumer_id = consumer_config->consumer_id ();

    this->proxy_supplier_map_.bind (consumer_id.c_str (),
                                    proxy_supplier._retn ());
  }

  void
  RTEventService::disconnect_event_supplier (const char * connection_id)
  {
    RtecEventChannelAdmin::ProxyPushConsumer_var proxy_consumer;

    this->proxy_consumer_map_.unbind (connection_id, proxy_consumer);

    proxy_consumer->disconnect_push_consumer ();
  }

  void
  RTEventService::disconnect_event_consumer (const char * connection_id)
  {
    RtecEventChannelAdmin::ProxyPushSupplier_var proxy_supplier;

    this->proxy_supplier_map_.unbind (connection_id, proxy_supplier);

    proxy_supplier->disconnect_push_supplier ();
  }

  void
  RTEventService::push_event (Components::EventBase * /* ev */)
  {
        ACE_DEBUG ((LM_TRACE, CLINFO
                    "CIAO::RTEventService::push_event\n"));
  }

  void
  RTEventService::ciao_push_event (Components::EventBase * ev,
                                   const char * source_id,
                                   CORBA::TypeCode_ptr tc)
  {
    ACE_DEBUG ((LM_TRACE, CLINFO
                "CIAO::RTEventService::ciao_push_event\n"));
    RtecEventComm::EventSet events (1);
    events.length (1);

    ACE_Hash<ACE_CString> hasher;

    events[0].header.source = hasher (source_id);
    events[0].header.type = ACE_ES_EVENT_ANY;
    events[0].header.ttl = 10;

    // We can't use the Any insert operator here, since it will put the
    // EventBase typecode into the Any, and the actual eventtype's fields
    // (if any) will get truncated when the Any is demarshaled. So the
    // signature of this method has been changed to pass in the derived
    // typecode, and TAO-specific methods are used to assign it as the
    // Any's typecode and encode the value. This incurs an extra
    // encoding, which we may want to try to optimize someday.
    TAO_OutputCDR out;
    out << ev;
    TAO_InputCDR in (out);
    TAO::Unknown_IDL_Type *unk = 0;
    ACE_NEW (unk,
             TAO::Unknown_IDL_Type (tc, in));
    events[0].data.any_value.replace (unk);

    ACE_DEBUG ((LM_DEBUG,
                "******* push event for source string: %s\n",
                source_id));
    ACE_DEBUG ((LM_DEBUG,
                "******* push event for source id: %i\n",
                events[0].header.source));

    RtecEventChannelAdmin::ProxyPushConsumer_var proxy_consumer;

    if (this->proxy_consumer_map_.find (source_id, proxy_consumer) != 0)
      {
        ACE_DEBUG ((LM_DEBUG,
                    "CIAO (%P|%t) - RTEventService::ciao_push_event, "
                    "Error in finding the proxy consumer object.\n"));
        throw Components::BadEventType ();
      }

    proxy_consumer->push (events);
  }

  void
  RTEventService::create_rt_event_channel (const char * /* ec_name */)
  {
    ACE_DEBUG ((LM_TRACE, CLINFO
                "CIAO::EventService_Factory_impl::"
                "create_rt_event_channel\n"));

    TAO_EC_Default_Factory::init_svcs ();

    TAO_EC_Event_Channel_Attributes attributes (this->root_poa_.in (),
                                                this->root_poa_.in ());
    TAO_EC_Event_Channel * ec_servant = 0;
    ACE_NEW (ec_servant, TAO_EC_Event_Channel (attributes));
    ec_servant->activate ();
    this->rt_event_channel_ = ec_servant->_this ();
  }

  ::CORBA::Boolean
  RTEventService::create_addr_serv (const char * name,
                                    ::CORBA::UShort port,
                                    const char * address)
  {
    ACE_DEBUG ((LM_ERROR,
                "Create an address server using port [%d]\n",
                port));

    // Initialize the address server with the desired address.
    // This will be used by the sender object and the multicast
    // receiver.
    ACE_INET_Addr send_addr (port, address);

    SimpleAddressServer * addr_srv_impl =
      new SimpleAddressServer (send_addr);

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
  RTEventService::create_sender (const char * addr_serv_id)
  {
    ACE_DEBUG ((LM_DEBUG,
                "Create a Sender object with addr_serv_id: %s\n",
                addr_serv_id ));

    // We need a local socket to send the data, open it and check
    // that everything is OK:
    TAO_ECG_Refcounted_Endpoint endpoint(new TAO_ECG_UDP_Out_Endpoint);

    if (endpoint->dgram ().open (ACE_Addr::sap_any) == -1)
      {
        ACE_ERROR_RETURN ((LM_ERROR,
                           "Cannot open send endpoint\n"),
                          1);
      }

    RtecUDPAdmin::AddrServer_var addr_srv;

    if (this->addr_serv_map_.find (addr_serv_id, addr_srv) != 0)
      {
        return false;
      }

    // Now we setup the sender:
    TAO_EC_Servant_Var<TAO_ECG_UDP_Sender> sender =
      TAO_ECG_UDP_Sender::create();
    sender->init (this->rt_event_channel_.in (),
                  addr_srv.in (),
                  endpoint);

    // Set up the subscription and connect to the EC.
    ACE_ConsumerQOS_Factory cons_qos_fact;
    cons_qos_fact.start_disjunction_group ();
    cons_qos_fact.insert (ACE_ES_EVENT_SOURCE_ANY, ACE_ES_EVENT_ANY, 0);
    RtecEventChannelAdmin::ConsumerQOS sub =
      cons_qos_fact.get_ConsumerQOS ();
    sub.is_gateway = 1;
    sender->connect (sub);

    return true;
  }

  ::CORBA::Boolean
  RTEventService::create_receiver (const char * addr_serv_id,
                                   ::CORBA::Boolean is_multicast,
                                   ::CORBA::UShort listen_port)
  {
    ACE_DEBUG ((LM_DEBUG,
                "Create a receiver object with addr_serv_id: %s\n",
                addr_serv_id ));

    // Create and initialize the receiver
    TAO_EC_Servant_Var<TAO_ECG_UDP_Receiver> receiver =
      TAO_ECG_UDP_Receiver::create();

    // AddressServer is necessary when "multicast" is
    // enabled, but not for "udp".
    if (is_multicast)
      {
        TAO_ECG_UDP_Out_Endpoint endpoint;

        if (endpoint.dgram ().open (ACE_Addr::sap_any) == -1)
          {
            ACE_DEBUG ((LM_ERROR, "Cannot open send endpoint\n"));
            return false;
          }

        // TAO_ECG_UDP_Receiver::init() takes a TAO_ECG_Refcounted_Endpoint.
        // If we don't clone our endpoint and
        // pass &endpoint, the receiver will
        // attempt to delete endpoint during shutdown.
        TAO_ECG_Refcounted_Endpoint clone (new TAO_ECG_UDP_Out_Endpoint (endpoint));

        RtecUDPAdmin::AddrServer_var addr_srv;

        if (this->addr_serv_map_.find (addr_serv_id, addr_srv) != 0)
          {
            return false;
          }

        receiver->init (this->rt_event_channel_.in (),
                        clone,
                        addr_srv.in ());
      }
    else
      {
        TAO_ECG_Refcounted_Endpoint nil_endpoint;
        receiver->init (this->rt_event_channel_.in (), nil_endpoint, 0);
      }

    // Setup the registration and connect to the event channel
    ACE_SupplierQOS_Factory supp_qos_fact;
    supp_qos_fact.insert (ACE_ES_EVENT_SOURCE_ANY, ACE_ES_EVENT_ANY, 0, 1);
    RtecEventChannelAdmin::SupplierQOS pub =
      supp_qos_fact.get_SupplierQOS ();
    receiver->connect (pub);

    // Create the appropriate event handler
    // and register it with the reactor.

    if (is_multicast)
      {
        auto_ptr<TAO_ECG_Mcast_EH> mcast_eh (
          new TAO_ECG_Mcast_EH (receiver.in ()));
        mcast_eh->reactor (this->orb_->orb_core ()->reactor ());
        mcast_eh->open (this->rt_event_channel_.in ());
        mcast_eh.release ();
      }
    else
      {
        ACE_DEBUG ((LM_DEBUG,
                    "\nUDP Event Handler Port [%d]\n",
                    listen_port));

        TAO_ECG_UDP_EH * udp_eh = new TAO_ECG_UDP_EH (receiver.in ());

        udp_eh->reactor (this->orb_->orb_core ()->reactor ());

        ACE_INET_Addr local_addr (listen_port);

        if (udp_eh->open (local_addr) == -1)
          {
            ACE_ERROR ((LM_ERROR,
                        "Cannot open event handler on port [%d]\n",
                        listen_port));
            return false;
          }
      }

    return true;
  }


  ::RtecEventChannelAdmin::EventChannel_ptr
  RTEventService::tao_rt_event_channel (void)
  {
    return this->rt_event_channel_.in ();
  }

  //////////////////////////////////////////////////////////////////////
  ///                 Supplier Servant Implementation
  //////////////////////////////////////////////////////////////////////

  RTEventServiceSupplier_impl::RTEventServiceSupplier_impl (
    PortableServer::POA_ptr poa)
    : poa_ (PortableServer::POA::_duplicate (poa))
  {
  }

  void
  RTEventServiceSupplier_impl::disconnect_push_supplier (void)
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
    Components::EventConsumerBase_ptr consumer)
    : poa_ (PortableServer::POA::_duplicate (poa)),
      event_consumer_ (Components::EventConsumerBase::_duplicate (consumer))
  {
  }

  void
  RTEventServiceConsumer_impl::push (const RtecEventComm::EventSet& events)
  {
    ACE_DEBUG ((LM_TRACE, CLINFO
                "CIAO::RTEventServiceConsumer_impl::push\n"));

    for (CORBA::ULong i = 0; i < events.length (); ++i)
      {
        std::ostringstream out;
        out << "Received event,"
            << "  type: "   << events[i].header.type
            << "  source: " << events[i].header.source;

        ACE_OS::printf("%s\n", out.str().c_str()); // printf is synchronized

        Components::EventBase * ev = 0;

        try
        {
          TAO::Unknown_IDL_Type *unk =
            dynamic_cast<TAO::Unknown_IDL_Type *> (
              events[i].data.any_value.impl ());
          TAO_InputCDR for_reading (unk->_tao_get_cdr ());

          if (for_reading >> ev)
            {
              ev->_add_ref ();
              this->event_consumer_->push_event (ev);
            }
          else
            {
              ACE_ERROR ((LM_ERROR, "CIAO::RTEventServiceConsumer_impl::push(), "
                                    "failed to extract event\n"));
            }
        }
        catch (const CORBA::Exception& ex)
        {
          ACE_ERROR ((LM_ERROR,
                      "CORBA EXCEPTION caught\n"));
          ex._tao_print_exception ("RTEventServiceConsumer_impl::push()\n");
        }
      }
  }

  void
  RTEventServiceConsumer_impl::disconnect_push_consumer (void)
  {
    ACE_DEBUG ((LM_TRACE, CLINFO
                "CIAO::RTEventServiceConsumer_impl::"
                "disconnect_push_consumer\n"));

    PortableServer::ObjectId_var oid = this->poa_->servant_to_id (this);
    this->poa_->deactivate_object (oid);
    this->_remove_ref ();
  }

  //////////////////////////////////////////////////////////////////////
  ///                   Supplier Config Implementation
  //////////////////////////////////////////////////////////////////////

  RTEvent_Supplier_Config_impl::RTEvent_Supplier_Config_impl (
    PortableServer::POA_ptr poa)
    : service_type_ (RTEC),
      poa_ (PortableServer::POA::_duplicate (poa))
  {
  }

  RTEvent_Supplier_Config_impl::~RTEvent_Supplier_Config_impl (void)
  {
    ACE_DEBUG ((LM_TRACE,
                "RTEvent_Supplier_Config_impl::"
                "~RTEvent_Supplier_Config_impl\n"));
  }

  void
  RTEvent_Supplier_Config_impl::supplier_id (const char * supplier_id)
  {
    ACE_DEBUG ((LM_DEBUG, CLINFO
                "RTEvent_Supplier_Config_impl::supplier_id::supplier's id: %s\n", supplier_id));

    this->supplier_id_ = supplier_id;

    ACE_Hash<ACE_CString> hasher;

    RtecEventComm::EventSourceID source_id =
      hasher (this->supplier_id_.c_str ());

    this->qos_.insert (ACE_ES_EVENT_SOURCE_ANY,
                       ACE_ES_EVENT_ANY,
                       0,    // handle to the rt_info structure
                       1);

    ACE_DEBUG ((LM_DEBUG, "supplier's source id is: %d\n", source_id));
  }

  CONNECTION_ID
  RTEvent_Supplier_Config_impl::supplier_id (void)
  {
    return CORBA::string_dup (this->supplier_id_.c_str ());
  }

  EventServiceType
  RTEvent_Supplier_Config_impl::service_type (void)
  {
    return this->service_type_;
  }

  RtecEventChannelAdmin::SupplierQOS *
  RTEvent_Supplier_Config_impl::rt_event_qos (void)
  {
    RtecEventChannelAdmin::SupplierQOS * supplier_qos = 0;
    ACE_NEW_RETURN (supplier_qos,
                    RtecEventChannelAdmin::SupplierQOS (
                      this->qos_.get_SupplierQOS ()),
                    0);
    return supplier_qos;
  }

  void
  RTEvent_Supplier_Config_impl::destroy (void)
  {
    PortableServer::ObjectId_var oid = this->poa_->servant_to_id (this);
    this->poa_->deactivate_object (oid);
    this->_remove_ref ();
  }

  //////////////////////////////////////////////////////////////////////
  ///                   Consumer Config Implementation
  //////////////////////////////////////////////////////////////////////

  RTEvent_Consumer_Config_impl::RTEvent_Consumer_Config_impl (
    PortableServer::POA_ptr poa)
    : service_type_ (RTEC),
      poa_ (PortableServer::POA::_duplicate (poa))
  {
  }

  RTEvent_Consumer_Config_impl::~RTEvent_Consumer_Config_impl (void)
  {
    ACE_DEBUG ((LM_DEBUG,
                "RTEvent_Consumer_Config_impl::"
                "~RTEvent_Consumer_Config_impl\n"));
  }

  void
  RTEvent_Consumer_Config_impl::start_conjunction_group (CORBA::Long size)
  {
    ACE_DEBUG ((LM_DEBUG,
                "RTEvent_Consumer_Config_impl::start_conjunction_group\n"));

    this->qos_.start_conjunction_group (size);
  }

  void
  RTEvent_Consumer_Config_impl::start_disjunction_group (CORBA::Long size)
  {
    // Note, since we only support basic builder here...
    if (size == 0L)
      {
        this->qos_.start_disjunction_group ();
      }
    else
      {
        this->qos_.start_disjunction_group (size);
      }
  }

  void
  RTEvent_Consumer_Config_impl::insert_source (const char * source_id)
  {
    ACE_Hash<ACE_CString> hasher;
    RtecEventComm::EventSourceID int_source_id = hasher (source_id);

    ACE_DEBUG ((LM_DEBUG, "******* the source string is: %s\n", source_id));
    ACE_DEBUG ((LM_DEBUG, "******* the source id is: %i\n", int_source_id));

    this->qos_.insert_source (int_source_id, 0);
  }

  void
  RTEvent_Consumer_Config_impl::insert_type (CORBA::Long event_type)
  {
    if (event_type == 0L)
      {
        this->qos_.insert_type (ACE_ES_EVENT_ANY, 0);
      }
    else
      {
        this->qos_.insert_type (event_type, 0);
      }
  }

  void
  RTEvent_Consumer_Config_impl::consumer_id (const char * consumer_id)
  {
    ACE_DEBUG ((LM_DEBUG, CLINFO
                "RTEvent_Consumer_Config_impl::set_consumer_id:%s\n",
                consumer_id));

    this->consumer_id_ = consumer_id;
  }

  void
  RTEvent_Consumer_Config_impl::consumer (
    Components::EventConsumerBase_ptr consumer)
  {
    this->consumer_ = Components::EventConsumerBase::_duplicate (consumer);
  }

  CONNECTION_ID
  RTEvent_Consumer_Config_impl::consumer_id (void)
  {
    return CORBA::string_dup (this->consumer_id_.c_str ());
  }

  EventServiceType
  RTEvent_Consumer_Config_impl::service_type (void)
  {
    return this->service_type_;
  }

  Components::EventConsumerBase_ptr
  RTEvent_Consumer_Config_impl::consumer (void)
  {
    ACE_DEBUG ((LM_TRACE,
                "RTEvent_Consumer_Config_impl::get_consumer\n"));

    return Components::EventConsumerBase::_duplicate (this->consumer_.in ());
  }

  RtecEventChannelAdmin::ConsumerQOS *
  RTEvent_Consumer_Config_impl::rt_event_qos (void)
  {
    RtecEventChannelAdmin::ConsumerQOS * consumer_qos = 0;
    ACE_NEW_RETURN (consumer_qos,
                    RtecEventChannelAdmin::ConsumerQOS (
                      this->qos_.get_ConsumerQOS ()),
                    0);

    return consumer_qos;
  }

  void
  RTEvent_Consumer_Config_impl::destroy (void)
  {
    ACE_DEBUG ((LM_DEBUG, "RTEvent_Consumer_Config_impl::destroy\n"));

    PortableServer::ObjectId_var oid = this->poa_->servant_to_id (this);
    this->poa_->deactivate_object (oid);
    this->_remove_ref ();
  }
}
