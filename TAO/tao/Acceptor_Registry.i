// $Id$

ACE_INLINE TAO_AcceptorSetItor
TAO_Acceptor_Registry::begin (void)
{
  return this->acceptors_.begin ();
}

ACE_INLINE TAO_AcceptorSetItor
TAO_Acceptor_Registry::end (void)
{
  return this->acceptors_.end ();
}
