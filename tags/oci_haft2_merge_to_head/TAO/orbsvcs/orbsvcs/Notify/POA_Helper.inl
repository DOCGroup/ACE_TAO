// $Id$

ACE_INLINE PortableServer::POA_ptr
TAO_Notify_POA_Helper::poa (void)
{
  return this->poa_.in ();
}
