// $Id$

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
