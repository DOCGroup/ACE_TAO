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
     supplier_admin_lock_ (TAO_EC_DEFAULT_SUPPLIER_ADMIN_LOCK)
{
}
