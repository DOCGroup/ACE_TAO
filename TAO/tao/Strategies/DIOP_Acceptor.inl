// -*- C++ -*-
TAO_BEGIN_VERSIONED_NAMESPACE_DECL

ACE_INLINE const ACE_INET_Addr&
TAO_DIOP_Acceptor::address () const
{
  ACE_ASSERT (this->addrs_ != 0);

  // @@ This is busted.
  //    The Implementation Repository will have to start supporting
  //    IORs with multiple profiles.  For now, we just return the
  //    first addr.
  //                    -Ossama
  return this->addrs_[0];
}

ACE_INLINE const ACE_INET_Addr *
TAO_DIOP_Acceptor::endpoints ()
{
  ACE_ASSERT (this->addrs_ != 0);

  return this->addrs_;
}

ACE_INLINE const ACE_INET_Addr&
TAO_DIOP_Acceptor::default_address () const
{
  return this->default_address_;
}

ACE_INLINE void
TAO_DIOP_Acceptor::set_default_address (const ACE_INET_Addr& addr)
{
  this->default_address_.set (addr);
}

TAO_END_VERSIONED_NAMESPACE_DECL
