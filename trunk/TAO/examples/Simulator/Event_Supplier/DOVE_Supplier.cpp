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

ACE_RCSID(Event_Supplier, DOVE_Supplier, "$Id$")

// Static pointer member initialization for Singleton.

ACE_Scheduler_Factory::POD_RT_Info * 
DOVE_Supplier::rt_info_instance_ = 0;

// Constructor.

DOVE_Supplier::DOVE_Supplier ()
  : initialized_ (0),
    internal_DOVE_Supplier_ptr_ (new Internal_DOVE_Supplier (this)),
    MIB_name_ (0),
    es_name_ (0),
    ss_name_ (0)
{
}

// Destructor.

DOVE_Supplier::~DOVE_Supplier ()
{
  delete internal_DOVE_Supplier_ptr_;
}

// Initialize the ORB and the connection to the Name Service

int 
DOVE_Supplier::init (void)
{
  TAO_TRY
  {
    // Connect to the RootPOA.
    CORBA::Object_var poaObject_var =
      TAO_ORB_Core_instance()->orb()->resolve_initial_references("RootPOA");

    if (CORBA::is_nil (poaObject_var.in ()))
      ACE_ERROR_RETURN ((LM_ERROR,
                         " (%P|%t) Unable to initialize the POA.\n"),
                        -1);

    this->root_POA_var_ =
      PortableServer::POA::_narrow (poaObject_var.in (), TAO_TRY_ENV);
    TAO_CHECK_ENV;

    this->poa_manager_ =
       root_POA_var_->the_POAManager (TAO_TRY_ENV);
    TAO_CHECK_ENV;

    // Get the Naming Service object reference.
    CORBA::Object_var namingObj_var =
      TAO_ORB_Core_instance()->orb()->resolve_initial_references ("NameService");

    if (CORBA::is_nil (namingObj_var.in ()))
      ACE_ERROR_RETURN ((LM_ERROR,
                        " (%P|%t) Unable to get the Naming Service.\n"),
                        -1);

    this->namingContext_var_ =
      CosNaming::NamingContext::_narrow (namingObj_var.in (),
                                       TAO_TRY_ENV);
    TAO_CHECK_ENV;
  }
  TAO_CATCHANY
  {
    TAO_TRY_ENV.print_exception ("DOVE_Supplier::init");
    return -1;
  }
  TAO_ENDTRY;

  initialized_ = 1;
  return 0;
}

int
DOVE_Supplier::connect (const char* MIB_name, 
                        const char* es_name, 
                        const char * ss_name, 
                        ACE_Scheduler_Factory::POD_RT_Info * rt_info)
{
  // Save the passed MIB and Event Service names
  MIB_name_ = (MIB_name == 0) ? "MIB_unknown" : MIB_name;
  es_name_ = (es_name == 0) ?  "EventService" : es_name;
  ss_name_ = (ss_name == 0) ?  "ScheduleService" : ss_name;

  // Initialize the supplier if this has not already been done.
  if ((initialized_ == 0) &&  (this->init () == -1))
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         " (%P|%t) Unable to initialize the DOVE_Supplier.\n"),
                        -1);
    }

  // Resolve the event service reference.
  if (this->get_EventChannel () == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         " (%P|%t) Unable to resolved the event service.\n"),
                        -1);
    }

  // Resolve the scheduling service reference.
  if (this->get_Scheduler () == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         " (%P|%t) Unable to resolve the scheduler.\n"),
                        -1);
    }

  // Connect to the event service as a supplier.
  if (this->connect_Supplier (rt_info) == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         " (%P|%t) Unable to connect to the event service.\n"),
                        -1);
    }

  return 0;

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
    event.header.source = SOURCE_ID;
    event.header.type = ACE_ES_EVENT_NOTIFICATION;
    event.header.ttl = 1;
    ACE_hrtime_t creation_time = ACE_OS::gethrtime ();
    ORBSVCS_Time::hrtime_to_TimeT (event.header.creation_time, creation_time);
    event.header.ec_recv_time = ORBSVCS_Time::zero;
    event.header.ec_send_time = ORBSVCS_Time::zero;
    event.data.any_value = message;

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
DOVE_Supplier::get_Scheduler ()
{
  TAO_TRY
    {
      CosNaming::Name schedule_name (1);
      schedule_name.length (1);
      schedule_name[0].id = CORBA::string_dup (ss_name_);

      CORBA::Object_var objref =
          namingContext_var_->resolve (schedule_name,
                                       TAO_TRY_ENV);
      TAO_CHECK_ENV;

      scheduler_var_ =
        RtecScheduler::Scheduler::_narrow(objref.in (),
                                            TAO_TRY_ENV);
      TAO_CHECK_ENV;
    }
  TAO_CATCHANY
    {
      scheduler_var_ = 0;
      ACE_ERROR_RETURN ((LM_ERROR,
                         "DOVE_Supplier::get_Scheduler: "
                         "error while resolving scheduler %s\n", ss_name_),
                        -1);
    }
  TAO_ENDTRY;

  return 0;
}


int
DOVE_Supplier::get_EventChannel ()
{
  TAO_TRY
  {
    // Get a reference to the Event Service
    CosNaming::Name channel_name (1);
    channel_name.length (1);
    channel_name[0].id = CORBA::string_dup (es_name_);

    CORBA::Object_var eventServiceObj_var =
      this->namingContext_var_->resolve (channel_name, TAO_TRY_ENV);
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
DOVE_Supplier::connect_Supplier (ACE_Scheduler_Factory::POD_RT_Info * rt_info)
{
  if (rt_info == 0)
    {
      // Get the default singleton if we were not passed the data
      rt_info = DOVE_Supplier::rt_info_instance ();
      if (rt_info == 0)
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             " (%P|%t) Unable to obtain"
                             " the default RT_Info data.\n"),
                            -1);
        }
    }

  TAO_TRY
  {
    // Generate the Real-time information descriptor.
    this->rt_info_ = scheduler_var_->create (rt_info->entry_point, 
                                             TAO_TRY_ENV);

    this->scheduler_var_->set (this->rt_info_,
                               ACE_static_cast (RtecScheduler::Criticality_t,
	                                        rt_info->criticality),
                               rt_info->worst_case_execution_time,
                               rt_info->typical_execution_time,
                               rt_info->cached_execution_time,
                               rt_info->period,
                               ACE_static_cast (RtecScheduler::Importance_t,
                                                rt_info->importance),
                               rt_info->quantum,
                               rt_info->threads,
                               ACE_static_cast (RtecScheduler::Info_Type_t,
                                                rt_info->info_type),
                               TAO_TRY_ENV);
    TAO_CHECK_ENV;


    // Set the publications to report them to the event channel.

    CORBA::Short x = 0;
    RtecEventChannelAdmin::SupplierQOS qos;
    qos.publications.length (1);
    qos.publications[0].event.header.source = SOURCE_ID;
    qos.publications[0].event.header.type = ACE_ES_EVENT_NOTIFICATION;
    qos.publications[0].event.header.ttl = 1;
    qos.publications[0].event.header.creation_time = ORBSVCS_Time::zero;
    qos.publications[0].event.header.ec_recv_time = ORBSVCS_Time::zero;
    qos.publications[0].event.header.ec_send_time = ORBSVCS_Time::zero;
    qos.publications[0].event.data.any_value.replace (CORBA::_tc_short,
						      &x,
						      0,
						      TAO_TRY_ENV);
    TAO_CHECK_ENV;
    qos.publications[0].dependency_info.number_of_calls = 1;
    qos.publications[0].dependency_info.rt_info = this->rt_info_;

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
    TAO_TRY_ENV.print_exception ("DOVE_Supplier::connect_supplier");
    return -1;
  }
  TAO_ENDTRY;

  return 0;

}


// Access the default rt_info singleton.

ACE_Scheduler_Factory::POD_RT_Info * 
DOVE_Supplier::rt_info_instance ()
{
  if (! rt_info_instance_)
    {
      ACE_NEW_RETURN (rt_info_instance_,
                      ACE_Scheduler_Factory::POD_RT_Info,
                      0);

      // Set up the default data.
      rt_info_instance_->entry_point = "ABC";
      rt_info_instance_->criticality = RtecScheduler::VERY_LOW_CRITICALITY;
      rt_info_instance_->worst_case_execution_time = ORBSVCS_Time::zero;
      rt_info_instance_->typical_execution_time = ORBSVCS_Time::zero;
      rt_info_instance_->cached_execution_time = ORBSVCS_Time::zero;
      rt_info_instance_->period = 2500000;
      rt_info_instance_->importance = RtecScheduler::VERY_LOW_IMPORTANCE;
      rt_info_instance_->quantum = ORBSVCS_Time::zero;
      rt_info_instance_->threads = 1;
      rt_info_instance_->info_type = RtecScheduler::OPERATION;
    }

  return rt_info_instance_;
}


