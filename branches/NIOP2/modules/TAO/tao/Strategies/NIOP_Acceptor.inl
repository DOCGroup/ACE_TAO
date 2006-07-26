// -*- C++ -*-
//
// $Id: NIOP_Acceptor.i,v 1.2 2005/11/04 18:39:05 ossama Exp $

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

//ACE_INLINE const ACE_INET_Addr&
//TAO_NIOP_Acceptor::address (void) const
//{
//  ACE_ASSERT (this->addrs_ != 0);

  // @@ This is busted.
  //    The Implementation Repository will have to start supporting
  //    IORs with multiple profiles.  For now, we just return the
  //    first addr.
  //                    -Ossama
  //return this->addrs_[0];
//}

//ACE_INLINE const ACE_INET_Addr *
//TAO_NIOP_Acceptor::endpoints (void)
//{
  //ACE_ASSERT (this->addrs_ != 0);
//
  //return this->addrs_;
//}

TAO_END_VERSIONED_NAMESPACE_DECL
