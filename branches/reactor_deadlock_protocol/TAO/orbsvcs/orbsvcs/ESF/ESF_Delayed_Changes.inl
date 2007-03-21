// -*- C++ -*-
//
// $Id$

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

template<class PROXY, class C, class I,ACE_SYNCH_DECL> void
TAO_ESF_Delayed_Changes<PROXY,C,I,ACE_SYNCH_USE>::
    connected_i (PROXY *proxy)
{
  this->collection_.connected (proxy);
}

template<class PROXY, class C, class I,ACE_SYNCH_DECL> void
TAO_ESF_Delayed_Changes<PROXY,C,I,ACE_SYNCH_USE>::
    reconnected_i (PROXY *proxy)
{
  this->collection_.reconnected (proxy);
}

template<class PROXY, class C, class I,ACE_SYNCH_DECL> void
TAO_ESF_Delayed_Changes<PROXY,C,I,ACE_SYNCH_USE>::
    disconnected_i (PROXY *proxy)
{
  this->collection_.disconnected (proxy);
}

template<class PROXY, class C, class I,ACE_SYNCH_DECL> void
TAO_ESF_Delayed_Changes<PROXY,C,I,ACE_SYNCH_USE>::
    shutdown_i (void)
{
  this->collection_.shutdown ();
}

TAO_END_VERSIONED_NAMESPACE_DECL
