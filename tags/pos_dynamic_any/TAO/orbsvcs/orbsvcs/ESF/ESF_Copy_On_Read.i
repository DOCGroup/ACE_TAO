// $Id$

template<class PROXY, class C, class I, class L> void
TAO_ESF_Copy_On_Read<PROXY,C,I,L>::
    connected (PROXY *proxy,
               CORBA::Environment &ACE_TRY_ENV)
{
  ACE_GUARD (L, ace_mon, this->lock_);

  proxy->_incr_refcnt ();
  this->collection_.connected (proxy, ACE_TRY_ENV);
}

template<class PROXY, class C, class I, class L> void
TAO_ESF_Copy_On_Read<PROXY,C,I,L>::
    reconnected (PROXY *proxy,
                 CORBA::Environment &ACE_TRY_ENV)
{
  ACE_GUARD (L, ace_mon, this->lock_);

  proxy->_incr_refcnt ();
  this->collection_.reconnected (proxy, ACE_TRY_ENV);
}

template<class PROXY, class C, class I, class L> void
TAO_ESF_Copy_On_Read<PROXY,C,I,L>::
    disconnected (PROXY *proxy,
                  CORBA::Environment &ACE_TRY_ENV)
{
  ACE_GUARD (L, ace_mon, this->lock_);

  this->collection_.disconnected (proxy, ACE_TRY_ENV);
}

template<class PROXY, class C, class I, class L> void
TAO_ESF_Copy_On_Read<PROXY,C,I,L>::
    shutdown (CORBA::Environment &ACE_TRY_ENV)
{
  ACE_GUARD (L, ace_mon, this->lock_);

  this->collection_.shutdown (ACE_TRY_ENV);
}
