// -*- C++ -*-
//
// $Id$

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

template<class PROXY, class C, class I,ACE_SYNCH_DECL> void
TAO_ESF_Delayed_Changes<PROXY,C,I,ACE_SYNCH_USE>::
    connected_i (PROXY *proxy
               ACE_ENV_ARG_DECL)
{
  this->collection_.connected (proxy ACE_ENV_ARG_PARAMETER);
}

template<class PROXY, class C, class I,ACE_SYNCH_DECL> void
TAO_ESF_Delayed_Changes<PROXY,C,I,ACE_SYNCH_USE>::
    reconnected_i (PROXY *proxy
                 ACE_ENV_ARG_DECL)
{
  this->collection_.reconnected (proxy ACE_ENV_ARG_PARAMETER);
}

template<class PROXY, class C, class I,ACE_SYNCH_DECL> void
TAO_ESF_Delayed_Changes<PROXY,C,I,ACE_SYNCH_USE>::
    disconnected_i (PROXY *proxy
                  ACE_ENV_ARG_DECL)
{
  this->collection_.disconnected (proxy ACE_ENV_ARG_PARAMETER);
}

template<class PROXY, class C, class I,ACE_SYNCH_DECL> void
TAO_ESF_Delayed_Changes<PROXY,C,I,ACE_SYNCH_USE>::
    shutdown_i (ACE_ENV_SINGLE_ARG_DECL)
{
  this->collection_.shutdown (ACE_ENV_SINGLE_ARG_PARAMETER);
}

TAO_END_VERSIONED_NAMESPACE_DECL
