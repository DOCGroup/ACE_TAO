// $Id$

ACE_INLINE
TAO_EC_Default_Factory::TAO_EC_Default_Factory (void)
  :  dispatching_ (TAO_EC_DEFAULT_DISPATCHING),
     filtering_ (TAO_EC_DEFAULT_CONSUMER_FILTER),
     supplier_filtering_ (TAO_EC_DEFAULT_SUPPLIER_FILTER),
     timeout_ (TAO_EC_DEFAULT_TIMEOUT),
     observer_ (TAO_EC_DEFAULT_OBSERVER),
     scheduling_ (TAO_EC_DEFAULT_SCHEDULING),
     supplier_set_ (TAO_EC_DEFAULT_SUPPLIER_SET),
     consumer_lock_ (TAO_EC_DEFAULT_CONSUMER_LOCK),
     supplier_lock_ (TAO_EC_DEFAULT_SUPPLIER_LOCK),
     consumer_admin_lock_ (TAO_EC_DEFAULT_CONSUMER_ADMIN_LOCK),
     supplier_admin_lock_ (TAO_EC_DEFAULT_SUPPLIER_ADMIN_LOCK),
     dispatching_threads_ (TAO_EC_DEFAULT_DISPATCHING_THREADS),
     dispatching_threads_flags_ (TAO_EC_DEFAULT_DISPATCHING_THREADS_FLAGS),
     dispatching_threads_priority_ (TAO_EC_DEFAULT_DISPATCHING_THREADS_PRIORITY),
     dispatching_threads_force_active_ (TAO_EC_DEFAULT_DISPATCHING_THREADS_FORCE_ACTIVE),
     orbid_ (TAO_EC_DEFAULT_ORB_ID),
     consumer_control_ (TAO_EC_DEFAULT_CONSUMER_CONTROL),
     supplier_control_ (TAO_EC_DEFAULT_SUPPLIER_CONTROL),
     consumer_control_period_ (TAO_EC_DEFAULT_CONSUMER_CONTROL_PERIOD),
     supplier_control_period_ (TAO_EC_DEFAULT_SUPPLIER_CONTROL_PERIOD)
{
}
