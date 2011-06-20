
//=============================================================================
/**
 *  @file    DOVE_Supplier.cpp
 *
 *  $Id$
 *
 *  A wrapper around the event service initialization and
 *  marshalling
 *
 *
 *  @author Michael Kircher (mk1@cs.wustl.edu)
 */
//=============================================================================


#include "DOVE_Supplier.h"
#include "tao/ORB_Core.h"

// Static pointer member initialization for Singleton.

ACE_Scheduler_Factory::POD_RT_Info *
DOVE_Supplier::pod_rt_info_instance_ = 0;

// Constructor.

DOVE_Supplier::DOVE_Supplier ()
  : initialized_ (0),
    connected_ (0),
    connection_params_list_ (0),
    current_connection_params_ (0),
    connection_count_ (0),
    current_connection_index_ (0),
    internal_DOVE_Supplier_ptr_ (0),
    MIB_name_ (0)
{
  ACE_NEW (internal_DOVE_Supplier_ptr_,
           Internal_DOVE_Supplier (this));

  if (internal_DOVE_Supplier_ptr_ == 0)
    {
      ACE_ERROR ((LM_ERROR,
                  "DOVE_Supplier::DOVE_Supplier internal "
                  "supplier not allocated."));
    }
}

// Destructor.

DOVE_Supplier::~DOVE_Supplier ()
{
  for (int i = 0; i < this->connection_count_; ++i)
    {
      delete (this->connection_params_list_ [i]);
    }

  delete [] this->connection_params_list_;

  delete internal_DOVE_Supplier_ptr_;

}

// Initialize the ORB and the connection to the Name Service

int
DOVE_Supplier::init (void)
{
  try
  {
    // Connect to the RootPOA.
    CORBA::Object_var poaObject_var =
      TAO_ORB_Core_instance()->orb()->resolve_initial_references("RootPOA");

    if (CORBA::is_nil (poaObject_var.in ()))
      ACE_ERROR_RETURN ((LM_ERROR,
                         " (%P|%t) Unable to initialize the POA.\n"),
                        -1);

    this->root_POA_var_ =
      PortableServer::POA::_narrow (poaObject_var.in ());

    this->poa_manager_ =
       root_POA_var_->the_POAManager ();

    // Get the Naming Service object reference.
    CORBA::Object_var namingObj_var =
      TAO_ORB_Core_instance()->orb()->resolve_initial_references (
          "NameService");

    if (CORBA::is_nil (namingObj_var.in ()))
      ACE_ERROR_RETURN ((LM_ERROR,
                        " (%P|%t) Unable to get the Naming Service.\n"),
                        -1);

    this->namingContext_var_ =
      CosNaming::NamingContext::_narrow (namingObj_var.in ());
  }
  catch (const CORBA::Exception& ex)
  {
    ex._tao_print_exception ("DOVE_Supplier::init");
    return -1;
  }

  initialized_ = 1;
  return 0;
}

int
DOVE_Supplier::connect (const char* MIB_name,
                        const char* es_name,
                        const char * ss_name,
                        ACE_Scheduler_Factory::POD_RT_Info * pod_rt_info)
{
  // Initialize the supplier if this has not already been done.
  if ((initialized_ == 0) &&  (this->init () == -1))
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         " (%P|%t) Unable to initialize the DOVE_Supplier.\n"),
                        -1);
    }


  // Grab the default RT_Info settings if others were not provided.
  if (pod_rt_info == 0)
    {
      // Get the default singleton if we were not passed the data
      pod_rt_info = DOVE_Supplier::pod_rt_info_instance ();
      if (pod_rt_info == 0)
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             " (%P|%t) Unable to obtain"
                             " the default RT_Info data.\n"),
                            -1);
        }
    }

  // Save the passed MIB name
  MIB_name_ = (MIB_name == 0) ? "MIB_unknown" : MIB_name;

  // Create a new connection parameters structure.
  Connection_Params * cp_temp = 0;
  ACE_NEW_RETURN (cp_temp, Connection_Params, -1);

  // Populate the known fields of the new connection params struct.
  cp_temp->pod_rt_info_ = *pod_rt_info;
  cp_temp->es_name_ = (es_name == 0) ?  "EventService" : es_name;
  cp_temp->ss_name_ = (ss_name == 0) ?  "ScheduleService" : ss_name;

  // Allocate a new connection parameters pointer array.
  // Cannot use ACE_NEW_RETURN here, as we need to clean up
  // cp_temp if we fail here, and we need what cp_temp points
  // to after the current scope if we succeed here.
  Connection_Params ** cp_list_temp;
  cp_list_temp =
    new Connection_Params * [this->connection_count_ + 1];
  if (cp_list_temp == 0)
    {
      // Avoid a memory leak if we failed to allocate.
      delete cp_temp;

      ACE_ERROR_RETURN ((LM_ERROR,
                         " (%P|%t) DOVE_Supplier::connect could not "
                         "reallocate connection params list"),
                        -1);
    }

  // Copy the connection struct pointers from
  // the old list (if any) to the new one.
  for (int i = 0; i < this->connection_count_; ++i)
    {
      cp_list_temp [i] =
        this->connection_params_list_ [i];
    }

  // Put a pointer to the new connection params structure
  // in the new list, increment the connection params count,
  // and point to the latest connection parameters.
  cp_list_temp [this->connection_count_] = cp_temp;
  this->current_connection_params_ = cp_temp;
  current_connection_index_ = connection_count_;
  ++ (this->connection_count_);

  // Replace the old list of pointers with the new one
  delete [] this->connection_params_list_;
  this->connection_params_list_ = cp_list_temp;

  // Resolve the event service reference.
  if (this->get_EventChannel () == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         " (%P|%t) Unable to resolve the event service.\n"),
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
  if (this->connect_Supplier () == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         " (%P|%t) Unable to connect to the event service.\n"),
                        -1);
    }

  return 0;

}


// This method is invoked after all connect calls are done.

void
DOVE_Supplier::connected ()
{
  if (! connected_)
    {
      // Code to do post-connection-establishment
      // one-time logic goes here.

      connected_ = 1;
    }
}


void
DOVE_Supplier::disconnect ()
{
}


void
DOVE_Supplier::notify (CORBA::Any &message)
{
  // Finalize connection establishment no later than the first event notification
  if (! connected_)
    {
      this->connected ();
    }

  try
  {
    RtecEventComm::Event event;
    event.header.source = SOURCE_ID;
    event.header.type = ACE_ES_EVENT_NOTIFICATION;
    event.header.ttl = 1;
    ACE_hrtime_t creation_time = ACE_OS::gethrtime ();
    ORBSVCS_Time::hrtime_to_TimeT (event.header.creation_time, creation_time);
    event.header.ec_recv_time = ORBSVCS_Time::zero ();
    event.header.ec_send_time = ORBSVCS_Time::zero ();
    event.data.any_value = message;

    RtecEventComm::EventSet events;
    events.length (1);
    events[0] = event;

    // Now we invoke a RPC
    this->current_connection_params_->proxyPushConsumer_var_->push (events);
  }
  catch (const CORBA::Exception&)
  {
    ACE_ERROR ((LM_ERROR,
                "DOVE_Supplier::notify: "
                "unexpected exception.\n"));
  }
}


// Use the next connection in the list of established connections.

void
DOVE_Supplier::use_next_connection ()
{
  if (connection_count_ > 0)
    {
      current_connection_index_ =
        (current_connection_index_ == connection_count_ - 1)
        ? 0 : current_connection_index_ + 1;

      current_connection_params_ =
        connection_params_list_ [current_connection_index_];
    }
}


// Use the previous connection in the list of established connections.

void
DOVE_Supplier::use_prev_connection ()
{
  if (connection_count_ > 0)
    {
      current_connection_index_ =
        (current_connection_index_ == 0)
        ? connection_count_ - 1
        : current_connection_index_ - 1;

      current_connection_params_ =
        connection_params_list_ [current_connection_index_];
    }
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
  try
    {
      CosNaming::Name schedule_name (1);
      schedule_name.length (1);
      schedule_name[0].id =
        CORBA::string_dup (this->current_connection_params_->ss_name_);

      CORBA::Object_var objref =
          namingContext_var_->resolve (schedule_name);

      this->current_connection_params_->scheduler_var_ =
        RtecScheduler::Scheduler::_narrow(objref.in ());
    }
  catch (const CORBA::Exception&)
    {
      current_connection_params_->scheduler_var_ = 0;
      ACE_ERROR_RETURN ((LM_ERROR,
                         "DOVE_Supplier::get_Scheduler: "
                         "error while resolving scheduler %s\n",
                         this->current_connection_params_->ss_name_),
                        -1);
    }

  return 0;
}


int
DOVE_Supplier::get_EventChannel ()
{
  try
  {
    // Get a reference to the Event Service
    CosNaming::Name channel_name (1);
    channel_name.length (1);
    channel_name[0].id =
      CORBA::string_dup (this->current_connection_params_->es_name_);

    CORBA::Object_var eventServiceObj_var =
      this->namingContext_var_->resolve (channel_name);

    this->current_connection_params_->eventChannel_var_ =
       RtecEventChannelAdmin::EventChannel::_narrow (eventServiceObj_var.in());

    if (CORBA::is_nil (this->current_connection_params_->eventChannel_var_.in()))
      ACE_ERROR_RETURN ((LM_ERROR,
                         "The reference to the event channel is nil!"),
                         1);
  }
  catch (const CORBA::Exception& ex)
  {
    ex._tao_print_exception ("DOVE_Supplier::get_EventChannel");
    return -1;
  }

  return 0;
}


int
DOVE_Supplier::connect_Supplier ()
{
  try
  {
    // Generate the Real-time information descriptor.
    this->current_connection_params_->rt_info_ =
      this->current_connection_params_->
        scheduler_var_->
          create (this->current_connection_params_->pod_rt_info_.entry_point);


    this->current_connection_params_->scheduler_var_->
      set (this->current_connection_params_->rt_info_,
           static_cast<RtecScheduler::Criticality_t> (this->current_connection_params_->pod_rt_info_.criticality),
           this->current_connection_params_->pod_rt_info_.worst_case_execution_time,
           this->current_connection_params_->pod_rt_info_.typical_execution_time,
           this->current_connection_params_->pod_rt_info_.cached_execution_time,
           this->current_connection_params_->pod_rt_info_.period,
           static_cast<RtecScheduler::Importance_t> (this->current_connection_params_->pod_rt_info_.importance),
           this->current_connection_params_->pod_rt_info_.quantum,
           this->current_connection_params_->pod_rt_info_.threads,
           static_cast<RtecScheduler::Info_Type_t> (this->current_connection_params_->pod_rt_info_.info_type));



    // Set the publications to report them to the event channel.

    CORBA::Short x = 0;
    RtecEventChannelAdmin::SupplierQOS qos;
    qos.publications.length (1);
    qos.publications[0].event.header.source = SOURCE_ID;
    qos.publications[0].event.header.type = ACE_ES_EVENT_NOTIFICATION;
    qos.publications[0].event.header.ttl = 1;
    qos.publications[0].event.header.creation_time = ORBSVCS_Time::zero ();
    qos.publications[0].event.header.ec_recv_time = ORBSVCS_Time::zero ();
    qos.publications[0].event.header.ec_send_time = ORBSVCS_Time::zero ();
    qos.publications[0].event.data.any_value <<= x;
    qos.publications[0].dependency_info.number_of_calls = 1;
    qos.publications[0].dependency_info.rt_info =
      this->current_connection_params_->rt_info_;

    // = Connect as a supplier.
    this->current_connection_params_->supplierAdmin_var_ =
      this->current_connection_params_->eventChannel_var_->for_suppliers ();

    this->current_connection_params_->proxyPushConsumer_var_ =
      this->current_connection_params_->supplierAdmin_var_->obtain_push_consumer ();

    // In calling _this we get back an object reference and register
    // the servant with the POA.
    RtecEventComm::PushSupplier_var pushSupplier_var =
      this->internal_DOVE_Supplier_ptr_->_this ();

    // Connect the supplier to the proxy consumer.
    ACE_SupplierQOS_Factory::debug (qos);
    this->current_connection_params_->
      proxyPushConsumer_var_->connect_push_supplier (pushSupplier_var.in (),
                                                     qos);
  }
  catch (const CORBA::Exception& ex)
  {
    ex._tao_print_exception ("DOVE_Supplier::connect_supplier");
    return -1;
  }

  return 0;

}


// Access the default rt_info singleton.

ACE_Scheduler_Factory::POD_RT_Info *
DOVE_Supplier::pod_rt_info_instance ()
{
  if (DOVE_Supplier::pod_rt_info_instance_ == 0)
    {
      ACE_NEW_RETURN (DOVE_Supplier::pod_rt_info_instance_,
                      ACE_Scheduler_Factory::POD_RT_Info,
                      0);

      // Set up the default data.
      DOVE_Supplier::pod_rt_info_instance_->entry_point = "ABC";
      DOVE_Supplier::pod_rt_info_instance_->criticality =
        RtecScheduler::VERY_LOW_CRITICALITY;
      DOVE_Supplier::pod_rt_info_instance_->worst_case_execution_time =
        ORBSVCS_Time::zero ();
      DOVE_Supplier::pod_rt_info_instance_->typical_execution_time =
        ORBSVCS_Time::zero ();
      DOVE_Supplier::pod_rt_info_instance_->cached_execution_time =
        ORBSVCS_Time::zero ();
      DOVE_Supplier::pod_rt_info_instance_->period = 10000000;
      DOVE_Supplier::pod_rt_info_instance_->importance =
        RtecScheduler::VERY_LOW_IMPORTANCE;
      DOVE_Supplier::pod_rt_info_instance_->quantum = ORBSVCS_Time::zero ();
      DOVE_Supplier::pod_rt_info_instance_->threads = 1;
      DOVE_Supplier::pod_rt_info_instance_->info_type =
        RtecScheduler::OPERATION;
    }

  return DOVE_Supplier::pod_rt_info_instance_;
}
