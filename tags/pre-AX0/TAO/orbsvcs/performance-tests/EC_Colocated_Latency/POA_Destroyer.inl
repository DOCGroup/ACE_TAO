// $Id$

ACE_INLINE
POA_Destroyer::POA_Destroyer (PortableServer::POA_ptr poa)
  :  poa_ (PortableServer::POA::_duplicate (poa))
{
}

ACE_INLINE void
POA_Destroyer::release ()
{
  this->poa_ = PortableServer::POA::_nil ();
}
