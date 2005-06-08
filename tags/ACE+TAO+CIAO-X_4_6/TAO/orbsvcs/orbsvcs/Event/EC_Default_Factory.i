// $Id$

ACE_INLINE
TAO_EC_Default_Factory::TAO_EC_Default_Factory (void)
  :  dispatching_ (TAO_EC_DEFAULT_DISPATCHING),
     filtering_ (TAO_EC_DEFAULT_CONSUMER_FILTER),
     supplier_filtering_ (TAO_EC_DEFAULT_SUPPLIER_FILTER),
     timeout_ (TAO_EC_DEFAULT_TIMEOUT),
     observer_ (TAO_EC_DEFAULT_OBSERVER),
     scheduling_ (TAO_EC_DEFAULT_SCHEDULING),
     consumer_collection_ (TAO_EC_DEFAULT_CONSUMER_COLLECTION),
     supplier_collection_ (TAO_EC_DEFAULT_SUPPLIER_COLLECTION),
     consumer_lock_ (TAO_EC_DEFAULT_CONSUMER_LOCK),
     supplier_lock_ (TAO_EC_DEFAULT_SUPPLIER_LOCK),
     dispatching_threads_ (TAO_EC_DEFAULT_DISPATCHING_THREADS),
     dispatching_threads_flags_ (TAO_EC_DEFAULT_DISPATCHING_THREADS_FLAGS),
     dispatching_threads_priority_ (TAO_EC_DEFAULT_DISPATCHING_THREADS_PRIORITY),
     dispatching_threads_force_active_ (TAO_EC_DEFAULT_DISPATCHING_THREADS_FORCE_ACTIVE),
     orbid_ (TAO_EC_DEFAULT_ORB_ID),
     consumer_control_ (TAO_EC_DEFAULT_CONSUMER_CONTROL),
     supplier_control_ (TAO_EC_DEFAULT_SUPPLIER_CONTROL),
     consumer_control_period_ (TAO_EC_DEFAULT_CONSUMER_CONTROL_PERIOD),
     supplier_control_period_ (TAO_EC_DEFAULT_SUPPLIER_CONTROL_PERIOD),
     consumer_control_timeout_ (0, TAO_EC_DEFAULT_CONSUMER_CONTROL_TIMEOUT),
     supplier_control_timeout_ (0, TAO_EC_DEFAULT_SUPPLIER_CONTROL_TIMEOUT),
     consumer_validate_connection_ (TAO_EC_DEFAULT_CONSUMER_VALIDATE_CONNECTION)
{
}

ACE_INLINE int
TAO_EC_Default_Factory::consumer_collection (void) const
{
  return this->consumer_collection_;
}

ACE_INLINE int
TAO_EC_Default_Factory::supplier_collection (void) const
{
  return this->supplier_collection_;
}

ACE_INLINE int
TAO_EC_Default_Factory::supplier_filtering (void) const
{
  return this->supplier_filtering_;
}

ACE_INLINE const ACE_CString&
TAO_EC_Default_Factory::orb_id (void) const
{
  return this->orbid_;
}
