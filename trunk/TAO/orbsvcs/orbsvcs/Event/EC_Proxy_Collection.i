// $Id$

template<class PROXY, class C, class I, class L> void
TAO_EC_Immediate_Changes<PROXY,C,I,L>::
    connected (PROXY *proxy,
               CORBA::Environment &ACE_TRY_ENV)
{
  ACE_GUARD (L, ace_mon, this->lock_);

  proxy->_incr_refcnt ();
  this->collection_.connected (proxy, ACE_TRY_ENV);
}

template<class PROXY, class C, class I, class L> void
TAO_EC_Immediate_Changes<PROXY,C,I,L>::
    reconnected (PROXY *proxy,
                 CORBA::Environment &ACE_TRY_ENV)
{
  ACE_GUARD (L, ace_mon, this->lock_);

  proxy->_incr_refcnt ();
  this->collection_.reconnected (proxy, ACE_TRY_ENV);
}

template<class PROXY, class C, class I, class L> void
TAO_EC_Immediate_Changes<PROXY,C,I,L>::
    disconnected (PROXY *proxy,
                  CORBA::Environment &ACE_TRY_ENV)
{
  ACE_GUARD (L, ace_mon, this->lock_);

  this->collection_.disconnected (proxy, ACE_TRY_ENV);
}

template<class PROXY, class C, class I, class L> void
TAO_EC_Immediate_Changes<PROXY,C,I,L>::
    shutdown (CORBA::Environment &ACE_TRY_ENV)
{
  ACE_GUARD (L, ace_mon, this->lock_);

  this->collection_.shutdown (ACE_TRY_ENV);
}

// ****************************************************************

template<class PROXY, class C, class I,ACE_SYNCH_DECL> void
TAO_EC_Delayed_Changes<PROXY,C,I,ACE_SYNCH_USE>::
    connected_i (PROXY *proxy,
               CORBA::Environment &ACE_TRY_ENV)
{
  this->collection_.connected (proxy, ACE_TRY_ENV);
}

template<class PROXY, class C, class I,ACE_SYNCH_DECL> void
TAO_EC_Delayed_Changes<PROXY,C,I,ACE_SYNCH_USE>::
    reconnected_i (PROXY *proxy,
                 CORBA::Environment &ACE_TRY_ENV)
{
  this->collection_.reconnected (proxy, ACE_TRY_ENV);
}

template<class PROXY, class C, class I,ACE_SYNCH_DECL> void
TAO_EC_Delayed_Changes<PROXY,C,I,ACE_SYNCH_USE>::
    disconnected_i (PROXY *proxy,
                  CORBA::Environment &ACE_TRY_ENV)
{
  this->collection_.disconnected (proxy, ACE_TRY_ENV);
}

template<class PROXY, class C, class I,ACE_SYNCH_DECL> void
TAO_EC_Delayed_Changes<PROXY,C,I,ACE_SYNCH_USE>::
    shutdown_i (CORBA::Environment &ACE_TRY_ENV)
{
  this->collection_.shutdown (ACE_TRY_ENV);
}

// ****************************************************************

template<class PROXY, class C, class I, class L> void
TAO_EC_Copy_On_Read<PROXY,C,I,L>::
    connected (PROXY *proxy,
               CORBA::Environment &ACE_TRY_ENV)
{
  ACE_GUARD (L, ace_mon, this->lock_);

  proxy->_incr_refcnt ();
  this->collection_.connected (proxy, ACE_TRY_ENV);
}

template<class PROXY, class C, class I, class L> void
TAO_EC_Copy_On_Read<PROXY,C,I,L>::
    reconnected (PROXY *proxy,
                 CORBA::Environment &ACE_TRY_ENV)
{
  ACE_GUARD (L, ace_mon, this->lock_);

  proxy->_incr_refcnt ();
  this->collection_.reconnected (proxy, ACE_TRY_ENV);
}

template<class PROXY, class C, class I, class L> void
TAO_EC_Copy_On_Read<PROXY,C,I,L>::
    disconnected (PROXY *proxy,
                  CORBA::Environment &ACE_TRY_ENV)
{
  ACE_GUARD (L, ace_mon, this->lock_);

  this->collection_.disconnected (proxy, ACE_TRY_ENV);
}

template<class PROXY, class C, class I, class L> void
TAO_EC_Copy_On_Read<PROXY,C,I,L>::
    shutdown (CORBA::Environment &ACE_TRY_ENV)
{
  ACE_GUARD (L, ace_mon, this->lock_);

  this->collection_.shutdown (ACE_TRY_ENV);
}
