// $Id$

//==============================================================
/**
 * @file RTEventService_exec.h
 *
 * Source file for the actual RTEventService and RTEventServiceHome
 * component implementations.
 *
 * @author George Edwards <g.edwards@vanderbilt.edu>
 */
//==============================================================

#include "RTEventService_exec.h"


MyImpl::RTEventService_exec_impl::RTEventService_exec_impl ()
{
  // Get a reference to the ORB
  char * argv[1] = { "RTEventService_exec" };
  int argc = sizeof (argv) / sizeof (argv[0]);
  this->orb_ = CORBA::ORB_init (argc, argv ACE_ENV_ARG_PARAMETER);
  ACE_TRY_CHECK;

  // Get a reference to the POA
  CORBA::Object_var poa_object =
  orb_->resolve_initial_references ("RootPOA" ACE_ENV_ARG_PARAMETER);
  ACE_TRY_CHECK;
  this->poa_ =
    PortableServer::POA::_narrow (poa_object.in () ACE_ENV_ARG_PARAMETER);
  ACE_TRY_CHECK;
  if (CORBA::is_nil (this->poa_.in ()))
    ACE_ERROR ((LM_ERROR, "Nil RootPOA\n"));

  // Create event channel
  TAO_EC_Event_Channel_Attributes attributes (poa_.in (), poa_.in ());
  TAO_EC_Event_Channel * ec_servant;
  ACE_NEW (ec_servant, TAO_EC_Event_Channel (attributes));
  ec_servant->activate (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK;
  this->event_channel_ = ec_servant->_this (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK;
}

MyImpl::RTEventService_exec_impl::~RTEventService_exec_impl ()
{

}

void MyImpl::RTEventService_exec_impl::connect_consumer (
    const char * event_type,
    const char * sink_name,
    const char * consumer_oid
    ACE_ENV_SINGLE_ARG_DECL
  )
  ACE_THROW_SPEC ((
    CORBA::SystemException
  ))
{
  ACE_DEBUG ((LM_DEBUG, "RTEventService_exec_impl::connect_consumer\n"));

  CORBA::Object_var obj = this->orb_->string_to_object (consumer_oid
                                                        ACE_ENV_ARG_PARAMETER);
  Components::EventConsumerBase_var event_base =
    Components::EventConsumerBase::_narrow (obj.in ()
                                     ACE_ENV_ARG_PARAMETER);
  if (CORBA::is_nil (event_base.in ()))
    ACE_ERROR ((LM_ERROR, "Nil event consumer\n"));

  RtecEventChannelAdmin::ConsumerAdmin_var consumer_admin =
    this->event_channel_->for_consumers (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK;
  RtecEventChannelAdmin::ProxyPushSupplier_var proxy_supplier =
    consumer_admin->obtain_push_supplier (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK;
    
  // Create and register consumer servant
  // @@ Bala, the consumer_servant is deactivated in the disconnect_push_consumer method, below
  RTEventServiceConsumer_impl * consumer_servant;
  ACE_NEW (consumer_servant, RTEventServiceConsumer_impl
    (orb_.in (), Components::EventConsumerBase::_duplicate (event_base.in ())));
  RtecEventComm::PushConsumer_var consumer = consumer_servant->_this (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK;

  // Put reference to this PushConsumer in the map so we can disconnect later
  ACE_Active_Map_Manager_Key key;
  this->proxy_supplier_map_.bind (proxy_supplier.in (), key);

  // Set QoS properties and connect
  ACE_ConsumerQOS_Factory qos;
  CORBA::Long event_flag;
  CORBA::Long source_flag;
  
  // @@ Bala, let me know the right way to do this
  if (ACE_OS::strcmp (event_type, "TimeOut") == 0)
    event_flag = ACE_ES_EVENT_UNDEFINED + 1;

  if (ACE_OS::strcmp (event_type, "DataAvailable") == 0)
    event_flag = ACE_ES_EVENT_UNDEFINED + 2;

  if (ACE_OS::strcmp (sink_name, "timeout") == 0)
    source_flag = ACE_ES_EVENT_SOURCE_ANY + 1;

  if (ACE_OS::strcmp (sink_name, "in_avail") == 0)
    source_flag = ACE_ES_EVENT_SOURCE_ANY + 2;

  if (ACE_OS::strcmp (sink_name, "data_ready") == 0)
    source_flag = ACE_ES_EVENT_SOURCE_ANY + 3;

  qos.start_logical_and_group (2);
  qos.insert_type (event_flag,
                   0);
  qos.insert_source (source_flag,
                     0);
  proxy_supplier->connect_push_consumer (consumer.in (),
                                         qos.get_ConsumerQOS ()
                                         ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;

}

void MyImpl::RTEventService_exec_impl::connect_supplier (
    const char * event_type,
    const char * source_name,
    const char * supplier_oid
    ACE_ENV_SINGLE_ARG_DECL
  )
  ACE_THROW_SPEC ((
    CORBA::SystemException
  ))
{
  ACE_DEBUG ((LM_DEBUG, "RTEventService_exec_impl::connect_supplier\n"));

  RtecEventChannelAdmin::SupplierAdmin_var supplier_admin =
    this->event_channel_->for_suppliers (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK;
  RtecEventChannelAdmin::ProxyPushConsumer_var proxy_consumer =
    supplier_admin->obtain_push_consumer (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK;

  // Create and register supplier servant
  RTEventServiceSupplier_impl * supplier_servant;
  ACE_NEW (supplier_servant, RTEventServiceSupplier_impl (orb_.in ()));
  RtecEventComm::PushSupplier_var supplier = supplier_servant->_this (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK;

  // Set QoS properties and connect
  ACE_SupplierQOS_Factory qos;
  CORBA::Long event_flag;
  CORBA::Long source_flag; 

  if (ACE_OS::strcmp (event_type, "TimeOut") == 0)
    event_flag = ACE_ES_EVENT_UNDEFINED + 1;

  if (ACE_OS::strcmp (event_type, "DataAvailable") == 0)
    event_flag = ACE_ES_EVENT_UNDEFINED + 2;

  if (ACE_OS::strcmp (source_name, "timeout") == 0)
    source_flag = ACE_ES_EVENT_SOURCE_ANY + 1;

  if (ACE_OS::strcmp (source_name, "data_available") == 0)
    source_flag = ACE_ES_EVENT_SOURCE_ANY + 2;

  if (ACE_OS::strcmp (source_name, "out_avail") == 0)
    source_flag = ACE_ES_EVENT_SOURCE_ANY + 3;

  qos.insert (source_flag,
              event_flag,
              0,
              1);

  proxy_consumer->connect_push_supplier (supplier.in (),
                                         qos.get_SupplierQOS ()
                                         ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;

  CORBA::Object_var obj = this->orb_->string_to_object (supplier_oid
                                                        ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;

  Components::CCMObject_var ccm_obj =
    Components::CCMObject::_narrow (obj.in ()
                                     ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;

  if (CORBA::is_nil (ccm_obj.in ()))
    ACE_ERROR ((LM_ERROR, "Nil event supplier\n"));

  // @@ Bala, the connect_publisher method was added to the Events interface in
  // CCM_Event.idl. Let me know if there is a better way.
  ccm_obj->connect_publisher
    (RtecEventChannelAdmin::ProxyPushConsumer::_duplicate (proxy_consumer.in ())
     ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;
}

void MyImpl::RTEventService_exec_impl::set_session_context (Components::SessionContext_ptr ctx
                       ACE_ENV_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException,
                     Components::CCMException))
{
  context_ = BasicSP::CCM_RTEventService_Context::_narrow (ctx);
}

void MyImpl::RTEventService_exec_impl::ccm_activate (ACE_ENV_SINGLE_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException,
                     Components::CCMException))
{

}

void MyImpl::RTEventService_exec_impl::ccm_passivate (ACE_ENV_SINGLE_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException,
                     Components::CCMException))
{

}

void MyImpl::RTEventService_exec_impl::ccm_remove (ACE_ENV_SINGLE_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException,
                     Components::CCMException))
{

}

BasicSP::CCM_RTEventChannel_ptr
MyImpl::RTEventService_exec_impl::get_rt_event_channel (ACE_ENV_SINGLE_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  ACE_DEBUG ((LM_DEBUG, "RTEventService_exec_impl::get_rt_event_channel\n"));
  return BasicSP::CCM_RTEventChannel::_duplicate (this);
}

MyImpl::RTEventServiceHome_exec_impl::RTEventServiceHome_exec_impl ()
{
}

MyImpl::RTEventServiceHome_exec_impl::~RTEventServiceHome_exec_impl ()
{
}

::Components::EnterpriseComponent_ptr
MyImpl::RTEventServiceHome_exec_impl::create (ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   Components::CCMException))
{
  return new RTEventService_exec_impl;
}

extern "C" RTEVENTSERVICE_EXEC_Export ::Components::HomeExecutorBase_ptr
createRTEventServiceHome_Impl (void)
{
  return new MyImpl::RTEventServiceHome_exec_impl;
}

MyImpl::RTEventServiceSupplier_impl::RTEventServiceSupplier_impl (void)
{
}

MyImpl::RTEventServiceSupplier_impl::RTEventServiceSupplier_impl (CORBA::ORB_ptr orb) :
  orb_ (CORBA::ORB::_duplicate (orb))
{
}
  
void MyImpl::RTEventServiceSupplier_impl::disconnect_push_supplier (void)
{
  CORBA::Object_var poa_object =
    orb_->resolve_initial_references ("RootPOA" ACE_ENV_ARG_PARAMETER);
  ACE_TRY_CHECK;
  PortableServer::POA_var root_poa =
    PortableServer::POA::_narrow (poa_object.in () ACE_ENV_ARG_PARAMETER);
  ACE_TRY_CHECK;
  if (CORBA::is_nil (root_poa.in ()))
    ACE_ERROR ((LM_ERROR, "Nil RootPOA\n"));
  PortableServer::ObjectId_var oid = root_poa->servant_to_id (this);
  root_poa->deactivate_object (oid);
}

MyImpl::RTEventServiceConsumer_impl::RTEventServiceConsumer_impl (void)
{
}

MyImpl::RTEventServiceConsumer_impl::RTEventServiceConsumer_impl
  (CORBA::ORB_ptr orb,
   Components::EventConsumerBase_ptr consumer) :
    orb_ (CORBA::ORB::_duplicate (orb)),
    event_consumer_ (Components::EventConsumerBase::_duplicate (consumer))
{
}

void MyImpl::RTEventServiceConsumer_impl::push (const RtecEventComm::EventSet& events)
{
  ACE_DEBUG ((LM_DEBUG, "CIAO_GLUE_BasicSP::RTEventServiceConsumer_impl::push\n"));
  for (unsigned long i = 0; i < events.length (); i++)
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

void MyImpl::RTEventServiceConsumer_impl::disconnect_push_consumer (void)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  CORBA::Object_var poa_object =
    orb_->resolve_initial_references ("RootPOA" ACE_ENV_ARG_PARAMETER);
  ACE_TRY_CHECK;
  PortableServer::POA_var root_poa =
    PortableServer::POA::_narrow (poa_object.in () ACE_ENV_ARG_PARAMETER);
  ACE_TRY_CHECK;
  if (CORBA::is_nil (root_poa.in ()))
    ACE_ERROR ((LM_ERROR, "Nil RootPOA\n"));
  PortableServer::ObjectId_var oid = root_poa->servant_to_id (this);
  root_poa->deactivate_object (oid);
}
