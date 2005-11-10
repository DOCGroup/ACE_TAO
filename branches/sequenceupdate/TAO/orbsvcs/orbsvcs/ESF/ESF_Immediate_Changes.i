// $Id$

template<class PROXY, class COLLECTION, class ITERATOR, class ACE_LOCK> void
TAO_ESF_Immediate_Changes<PROXY,COLLECTION,ITERATOR,ACE_LOCK>::
    connected (PROXY *proxy
               ACE_ENV_ARG_DECL)
{
  ACE_GUARD (ACE_LOCK, ace_mon, this->lock_);

  proxy->_incr_refcnt ();
  this->collection_.connected (proxy ACE_ENV_ARG_PARAMETER);
}

template<class PROXY, class COLLECTION, class ITERATOR, class ACE_LOCK> void
TAO_ESF_Immediate_Changes<PROXY,COLLECTION,ITERATOR,ACE_LOCK>::
    reconnected (PROXY *proxy
                 ACE_ENV_ARG_DECL)
{
  ACE_GUARD (ACE_LOCK, ace_mon, this->lock_);

  proxy->_incr_refcnt ();
  this->collection_.reconnected (proxy ACE_ENV_ARG_PARAMETER);
}

template<class PROXY, class COLLECTION, class ITERATOR, class ACE_LOCK> void
TAO_ESF_Immediate_Changes<PROXY,COLLECTION,ITERATOR,ACE_LOCK>::
    disconnected (PROXY *proxy
                  ACE_ENV_ARG_DECL)
{
  ACE_GUARD (ACE_LOCK, ace_mon, this->lock_);

  this->collection_.disconnected (proxy ACE_ENV_ARG_PARAMETER);
}

template<class PROXY, class COLLECTION, class ITERATOR, class ACE_LOCK> void
TAO_ESF_Immediate_Changes<PROXY,COLLECTION,ITERATOR,ACE_LOCK>::
    shutdown (ACE_ENV_SINGLE_ARG_DECL)
{
  ACE_GUARD (ACE_LOCK, ace_mon, this->lock_);

  this->collection_.shutdown (ACE_ENV_SINGLE_ARG_PARAMETER);
}
