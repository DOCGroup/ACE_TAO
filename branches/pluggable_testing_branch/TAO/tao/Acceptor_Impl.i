// $Id$

template <class SVC_HANDLER, ACE_PEER_ACCEPTOR_1> ACE_INLINE
TAO_Acceptor_Impl<SVC_HANDLER, ACE_PEER_ACCEPTOR_2>::
    TAO_Acceptor_Impl (ACE_Reactor *reactor,
                       int use_select)
  : ACE_Acceptor<SVC_HANDLER, ACE_PEER_ACCEPTOR_2> (reactor, use_select),
    orb_core_ (0)
{
}

template <class SVC_HANDLER, ACE_PEER_ACCEPTOR_1> ACE_INLINE
TAO_Acceptor_Impl<SVC_HANDLER, ACE_PEER_ACCEPTOR_2>::
    TAO_Acceptor_Impl (const ACE_PEER_ACCEPTOR_ADDR &local_addr,
                       ACE_Reactor * reactor,
                       int flags,
                       int use_select,
                       int reuse_addr)
  : ACE_Acceptor<SVC_HANDLER, ACE_PEER_ACCEPTOR_2> (local_addr,
                                                    reactor,
                                                    flags,
                                                    use_select,
                                                    reuse_addr),
    orb_core_ (0)
{
}
