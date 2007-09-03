// -*- C++ -*-
//
// $Id$

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

template<class EC, class P> ACE_INLINE
TAO_ESF_Proxy_RefCount_Guard<EC,P>::
    TAO_ESF_Proxy_RefCount_Guard (CORBA::ULong &refcount,
                                  EC *ec,
                                  P *proxy)
  :  refcount_ (refcount),
     event_channel_ (ec),
     proxy_ (proxy)
{
}

TAO_END_VERSIONED_NAMESPACE_DECL
