// $Id$

ACE_INLINE
TAO_EC_Default_Factory::TAO_EC_Default_Factory (void)
  :  dispatching_ (0),
     filtering_ (0),
     supplier_filtering_ (0),
     timeout_ (0),
     observer_ (0),
     supplier_set_ (0),
     consumer_lock_ (0),
     supplier_lock_ (0),
     consumer_admin_lock_ (0),
     supplier_admin_lock_ (0)
{
}
