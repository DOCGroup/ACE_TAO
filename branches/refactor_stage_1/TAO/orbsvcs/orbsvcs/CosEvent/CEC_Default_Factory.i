// $Id$

ACE_INLINE
TAO_CEC_Default_Factory::TAO_CEC_Default_Factory (void)
  :  dispatching_ (TAO_CEC_DEFAULT_DISPATCHING),
     pulling_strategy_ (TAO_CEC_DEFAULT_PULLING_STRATEGY),
     consumer_collection_ (TAO_CEC_DEFAULT_CONSUMER_COLLECTION),
     supplier_collection_ (TAO_CEC_DEFAULT_SUPPLIER_COLLECTION),
     consumer_lock_ (TAO_CEC_DEFAULT_CONSUMER_LOCK),
     supplier_lock_ (TAO_CEC_DEFAULT_SUPPLIER_LOCK),
     dispatching_threads_ (TAO_CEC_DEFAULT_DISPATCHING_THREADS),
     dispatching_threads_flags_ (TAO_CEC_DEFAULT_DISPATCHING_THREADS_FLAGS),
     dispatching_threads_priority_ (TAO_CEC_DEFAULT_DISPATCHING_THREADS_PRIORITY),
     dispatching_threads_force_active_ (TAO_CEC_DEFAULT_DISPATCHING_THREADS_FORCE_ACTIVE),
     reactive_pulling_period_ (TAO_CEC_DEFAULT_REACTIVE_PULLING_PERIOD),
     orbid_ ((char*)TAO_CEC_DEFAULT_ORB_ID),
     orbid_dupped_(0),
     consumer_control_ (TAO_CEC_DEFAULT_CONSUMER_CONTROL),
     supplier_control_ (TAO_CEC_DEFAULT_SUPPLIER_CONTROL),
     consumer_control_period_ (TAO_CEC_DEFAULT_CONSUMER_CONTROL_PERIOD),
     supplier_control_period_ (TAO_CEC_DEFAULT_SUPPLIER_CONTROL_PERIOD),
     consumer_control_timeout_ (0, TAO_CEC_DEFAULT_CONSUMER_CONTROL_TIMEOUT),
     supplier_control_timeout_ (0, TAO_CEC_DEFAULT_SUPPLIER_CONTROL_TIMEOUT)
{
}
