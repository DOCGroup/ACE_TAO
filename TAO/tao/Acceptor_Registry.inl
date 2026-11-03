// -*- C++ -*-
TAO_BEGIN_VERSIONED_NAMESPACE_DECL

ACE_INLINE TAO_AcceptorSetIterator
TAO_Acceptor_Registry::begin ()
{
  return this->acceptors_.get ();
}

ACE_INLINE TAO_AcceptorSetIterator
TAO_Acceptor_Registry::end ()
{
  TAO_Acceptor ** const acceptors = this->acceptors_.get ();
  return this->size_ == 0 ? acceptors : acceptors + this->size_;
}

TAO_END_VERSIONED_NAMESPACE_DECL
