// $Id$

ACE_INLINE
TAO_EC_Basic_Factory::TAO_EC_Basic_Factory (PortableServer::POA_ptr poa)
  :  poa_ (PortableServer::POA::_duplicate (poa)),
     supplier_filtering_ (0)
{
}
