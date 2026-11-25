TAO_BEGIN_VERSIONED_NAMESPACE_DECL

ACE_INLINE TAO_AcceptorSet const&
TAO_Acceptor_Registry::acceptors () const
{
  return this->acceptors_;
}

ACE_INLINE TAO_AcceptorSetIterator
TAO_Acceptor_Registry::begin ()
{
  return this->acceptor_view_.data ();
}

ACE_INLINE TAO_AcceptorSetIterator
TAO_Acceptor_Registry::end ()
{
  return this->acceptor_view_.empty ()
    ? this->acceptor_view_.data ()
    : this->acceptor_view_.data () + this->acceptor_view_.size ();
}

TAO_END_VERSIONED_NAMESPACE_DECL
