// -*- C++ -*-
//
// $Id$

#if defined (TAO_HAS_IIOP) && (TAO_HAS_IIOP != 0)

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

ACE_INLINE const ACE_INET_Addr&
TAO_IIOP_Acceptor::address (void) const
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
TAO_IIOP_Acceptor::endpoints (void)
{
  ACE_ASSERT (this->addrs_ != 0);

  return this->addrs_;
}

ACE_INLINE const ACE_INET_Addr&
TAO_IIOP_Acceptor::default_address (void) const
{
  return this->default_address_;
}

ACE_INLINE void
TAO_IIOP_Acceptor::set_default_address (const ACE_INET_Addr& addr)
{
  this->default_address_.set (addr);
}

TAO_END_VERSIONED_NAMESPACE_DECL

#endif /* TAO_HAS_IIOP && TAO_HAS_IIOP != 0 */

