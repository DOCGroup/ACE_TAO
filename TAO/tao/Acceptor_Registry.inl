// -*- C++ -*-
TAO_BEGIN_VERSIONED_NAMESPACE_DECL

ACE_INLINE TAO_AcceptorSet::const_iterator
TAO_Acceptor_Registry::begin () const
{
  return this->acceptors_.begin ();
}

ACE_INLINE TAO_AcceptorSet::const_iterator
TAO_Acceptor_Registry::end () const
{
  return this->acceptors_.end ();
}

ACE_INLINE TAO_AcceptorSet const&
TAO_Acceptor_Registry::acceptors () const
{
  return this->acceptors_;
}

TAO_END_VERSIONED_NAMESPACE_DECL
