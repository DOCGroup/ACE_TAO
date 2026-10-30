// -*- C++ -*-
TAO_BEGIN_VERSIONED_NAMESPACE_DECL

ACE_INLINE TAO_AcceptorSetIterator
TAO_Acceptor_Registry::begin ()
{
  return this->acceptors_.data ();
}

ACE_INLINE TAO_AcceptorSetIterator
TAO_Acceptor_Registry::end ()
{
  return this->acceptors_.empty ()
    ? this->acceptors_.data ()
    : this->acceptors_.data () + this->acceptors_.size ();
}

TAO_END_VERSIONED_NAMESPACE_DECL
