// $Id$

template<class PROXY, class COLLECTION, class ITERATOR, class ACE_LOCK> void
TAO_ESF_Immediate_Changes<PROXY,COLLECTION,ITERATOR,ACE_LOCK>::
    connected (PROXY *proxy,
               CORBA::Environment &ACE_TRY_ENV)
{
  ACE_GUARD (ACE_LOCK, ace_mon, this->lock_);

  proxy->_incr_refcnt ();
  this->collection_.connected (proxy, ACE_TRY_ENV);
}

template<class PROXY, class COLLECTION, class ITERATOR, class ACE_LOCK> void
TAO_ESF_Immediate_Changes<PROXY,COLLECTION,ITERATOR,ACE_LOCK>::
    reconnected (PROXY *proxy,
                 CORBA::Environment &ACE_TRY_ENV)
{
  ACE_GUARD (ACE_LOCK, ace_mon, this->lock_);

  proxy->_incr_refcnt ();
  this->collection_.reconnected (proxy, ACE_TRY_ENV);
}

template<class PROXY, class COLLECTION, class ITERATOR, class ACE_LOCK> void
TAO_ESF_Immediate_Changes<PROXY,COLLECTION,ITERATOR,ACE_LOCK>::
    disconnected (PROXY *proxy,
                  CORBA::Environment &ACE_TRY_ENV)
{
  ACE_GUARD (ACE_LOCK, ace_mon, this->lock_);

  this->collection_.disconnected (proxy, ACE_TRY_ENV);
}

template<class PROXY, class COLLECTION, class ITERATOR, class ACE_LOCK> void
TAO_ESF_Immediate_Changes<PROXY,COLLECTION,ITERATOR,ACE_LOCK>::
    shutdown (CORBA::Environment &ACE_TRY_ENV)
{
  ACE_GUARD (ACE_LOCK, ace_mon, this->lock_);

  this->collection_.shutdown (ACE_TRY_ENV);
}
