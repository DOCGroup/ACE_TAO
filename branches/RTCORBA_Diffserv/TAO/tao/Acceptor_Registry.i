// -*- C++ -*-
// $Id$

ACE_INLINE TAO_AcceptorSetIterator
TAO_Acceptor_Registry::begin (void)
{
  return this->acceptors_;
}

ACE_INLINE TAO_AcceptorSetIterator
TAO_Acceptor_Registry::end (void)
{
  return this->acceptors_ + this->size_;
}
