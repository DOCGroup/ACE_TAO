// $Id$

ACE_INLINE
TAO_EC_Null_Factory::TAO_EC_Null_Factory (PortableServer::POA_ptr poa)
  :  poa_ (PortableServer::POA::_duplicate (poa)),
     supplier_filtering_ (0)
{
}
