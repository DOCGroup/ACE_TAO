// $Id$

// ============================================================================
//
// = FILENAME
//    DOVE_Supplier.cpp
//
// = DESCRIPTION
//    A wrapper around the event service initialization and 
//    marshalling
//
// = AUTHOR
//    Michael Kircher (mk1@cs.wustl.edu)
//
// ============================================================================

#include "DOVE_Supplier.h"

DOVE_Supplier::DOVE_Supplier ()
  : MIB_name_ (0),
    internal_DOVE_Supplier_ptr_ (new Internal_DOVE_Supplier (this))
{
}

DOVE_Supplier::~DOVE_Supplier ()
{
  delete internal_DOVE_Supplier_ptr_;
}

int
DOVE_Supplier::connect (const char* MIB_name)
{
  if (this->get_EventChannel () == -1)
    return -1;
  return this->connect_Supplier ();
}


void 
DOVE_Supplier::disconnect ()
{
}


void
DOVE_Supplier::notify (CORBA::Any &message)
{
  TAO_TRY
  {
    RtecEventComm::Event event;
    event.source_ = SOURCE_ID;
    event.type_ = ACE_ES_EVENT_NOTIFICATION;
    event.ttl_ = 1;
    event.creation_time_ = ORBSVCS_Time::zero;
    event.ec_recv_time_ = ORBSVCS_Time::zero;
    event.ec_send_time_ = ORBSVCS_Time::zero;
    //event.data_.x = 0;
    //event.data_.y = 0;
    event.data_.any_value = message;

    RtecEventComm::EventSet events;
    events.length (1);
    events[0] = event;

    // Now we invoke a RPC
    proxyPushConsumer_var_->push (events,
                                 TAO_TRY_ENV);
    TAO_CHECK_ENV;
  }
  TAO_CATCHANY
  {
    ACE_ERROR ((LM_ERROR, 
                "DOVE_Supplier::notify: "
                "unexpected exception.\n"));
  }
  TAO_ENDTRY;
}


// -------------------- Internal Demo Supplier -----------------------------

DOVE_Supplier::Internal_DOVE_Supplier::Internal_DOVE_Supplier (DOVE_Supplier *impl_ptr)
  : impl_ptr_ (impl_ptr)
{
}

// ----------------------------------------------------------------------------


int
DOVE_Supplier::get_EventChannel ()
{
  TAO_TRY
  {
    // Connect to the RootPOA.
    CORBA::Object_var poaObject_var =
      TAO_ORB_Core_instance()->orb()->resolve_initial_references("RootPOA");
      
    if (CORBA::is_nil (poaObject_var.in ()))
      ACE_ERROR_RETURN ((LM_ERROR,
                        " (%P|%t) Unable to initialize the POA.\n"),
                        1);

    PortableServer::POA_var root_POA_var =
      PortableServer::POA::_narrow (poaObject_var.in (), TAO_TRY_ENV);
    TAO_CHECK_ENV;

    PortableServer::POAManager_var poa_manager =
       root_POA_var->the_POAManager (TAO_TRY_ENV);
    TAO_CHECK_ENV;

    // Get the Naming Service object reference.
    CORBA::Object_var namingObj_var =
      TAO_ORB_Core_instance()->orb()->resolve_initial_references ("NameService");

    if (CORBA::is_nil (namingObj_var.in ()))
      ACE_ERROR_RETURN ((LM_ERROR,
                        " (%P|%t) Unable to get the Naming Service.\n"),
                        -1);

    CosNaming::NamingContext_var namingContext_var =
      CosNaming::NamingContext::_narrow (namingObj_var.in (),
                                       TAO_TRY_ENV);
    TAO_CHECK_ENV;

    // Tell the ScheduleService to use the same naming service.
    ACE_Scheduler_Factory::use_config (namingContext_var.in ());


    // Get a reference to the Event Servie
    CosNaming::Name channel_name (1);
    channel_name.length (1);
    channel_name[0].id = CORBA::string_dup ("EventService");

    CORBA::Object_var eventServiceObj_var =
      namingContext_var->resolve (channel_name, TAO_TRY_ENV);
    TAO_CHECK_ENV;

    this->eventChannel_var_ =
       RtecEventChannelAdmin::EventChannel::_narrow (eventServiceObj_var.in(),
                                                   TAO_TRY_ENV);
    TAO_CHECK_ENV;

    if (CORBA::is_nil (eventChannel_var_.in()))
      ACE_ERROR_RETURN ((LM_ERROR,
                         "The reference to the event channel is nil!"),
                         1);
  }
  TAO_CATCHANY
  {
    TAO_TRY_ENV.print_exception ("DOVE_Supplier::get_EventChannel");
    return -1;
  }
  TAO_ENDTRY;

  return 0;
}


int
DOVE_Supplier::connect_Supplier ()
{
  TAO_TRY
  {
    // Get a Scheduling server.
    this->scheduler_var_ = ACE_Scheduler_Factory::server ();

    // Generate the Real-time information.
    RtecScheduler::handle_t rt_info;
    rt_info = scheduler_var_->create ("ABC", TAO_TRY_ENV);

    scheduler_var_->set (rt_info,
                         RtecScheduler::VERY_LOW_CRITICALITY,
                         ORBSVCS_Time::zero,
                         ORBSVCS_Time::zero,
                         ORBSVCS_Time::zero,
                         2500000,
                         RtecScheduler::VERY_LOW_IMPORTANCE,
                         ORBSVCS_Time::zero,
                         1,
                         RtecScheduler::OPERATION,
                         TAO_TRY_ENV);
    TAO_CHECK_ENV;

    // Set the publications to report them to the event channel.

    CORBA::Short x = 0;
    RtecEventChannelAdmin::SupplierQOS qos;
    qos.publications.length (1);
    qos.publications[0].event.source_ = SOURCE_ID;
    qos.publications[0].event.type_ = ACE_ES_EVENT_NOTIFICATION;
    qos.publications[0].event.ttl_ = 1;
    qos.publications[0].event.creation_time_ = ORBSVCS_Time::zero; 
    qos.publications[0].event.ec_recv_time_ = ORBSVCS_Time::zero;
    qos.publications[0].event.ec_send_time_ = ORBSVCS_Time::zero;
    qos.publications[0].event.data_.any_value.replace (CORBA::_tc_short,
                                                       &x,
                                                       CORBA::B_FALSE,
                                                       TAO_TRY_ENV);
    qos.publications[0].dependency_info.number_of_calls = 1;
    qos.publications[0].dependency_info.rt_info = rt_info;

    // = Connect as a supplier.
    this->supplierAdmin_var_ =
      this->eventChannel_var_->for_suppliers (TAO_TRY_ENV);
    TAO_CHECK_ENV;

    this->proxyPushConsumer_var_ =
      this->supplierAdmin_var_->obtain_push_consumer (TAO_TRY_ENV);
    TAO_CHECK_ENV;

    // In calling _this we get back an object reference and register
    // the servant with the POA.
    RtecEventComm::PushSupplier_var pushSupplier_var =
      this->internal_DOVE_Supplier_ptr_->_this (TAO_TRY_ENV);
    TAO_CHECK_ENV;

    // Connect the supplier to the proxy consumer.
    ACE_SupplierQOS_Factory::debug (qos);
    this->proxyPushConsumer_var_->connect_push_supplier (pushSupplier_var.in (),
                                                         qos,
                                                         TAO_TRY_ENV);
    TAO_CHECK_ENV;
  }
  TAO_CATCHANY
  {
    TAO_TRY_ENV.print_exception ("DOVE_Supplier::open");
    return -1;
  }
  TAO_ENDTRY;

  return 0;
}


