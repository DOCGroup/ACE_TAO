// $Id$

#ifndef TAO_EC_COPY_ON_WRITE_CPP
#define TAO_EC_COPY_ON_WRITE_CPP

#include "EC_Copy_On_Write.h"

#if ! defined (__ACE_INLINE__)
#include "EC_Copy_On_Write.i"
#endif /* __ACE_INLINE__ */

ACE_RCSID(Event, EC_Copy_On_Write, "$Id$")

template<class COLLECTION, class ITERATOR> CORBA::ULong
TAO_EC_Copy_On_Write_Collection<COLLECTION,ITERATOR>::_incr_refcnt (void)
{
  // LOCKING: no locking is required, the caller grabs the mutex.
  return this->refcount_++;
}

template<class COLLECTION, class ITERATOR> CORBA::ULong
TAO_EC_Copy_On_Write_Collection<COLLECTION,ITERATOR>::_decr_refcnt (void)
{
  // LOCKING: no locking is required, the caller grabs the mutex.
  {
    this->refcount_--;
    if (this->refcount_ != 0)
      return this->refcount_;
  }

  ITERATOR end = this->collection.end ();
  for (ITERATOR i = this->collection.begin (); i != end; ++i)
    {
      (*i)->_decr_refcnt ();
    }

  delete this;
  return 0;
}

// ****************************************************************

template<class PROXY, class COLLECTION, class ITERATOR, ACE_SYNCH_DECL>
TAO_EC_Copy_On_Write<PROXY,COLLECTION,ITERATOR,ACE_SYNCH_USE>::
    TAO_EC_Copy_On_Write (void)
      :  writing_ (0),
         cond_ (mutex_)
{
  ACE_NEW (this->collection_, Collection);
}

template<class PROXY, class COLLECTION, class ITERATOR, ACE_SYNCH_DECL>
TAO_EC_Copy_On_Write<PROXY,COLLECTION,ITERATOR,ACE_SYNCH_USE>::
    ~TAO_EC_Copy_On_Write (void)
{
  this->collection_->_decr_refcnt ();
}

template<class PROXY, class COLLECTION, class ITERATOR, ACE_SYNCH_DECL> void
TAO_EC_Copy_On_Write<PROXY,COLLECTION,ITERATOR,ACE_SYNCH_USE>::
    for_each (TAO_EC_Worker<PROXY> *worker,
              CORBA::Environment &ACE_TRY_ENV)
{
  Read_Guard ace_mon (this->mutex_,
                      this->collection_);

  ITERATOR end = ace_mon.collection->collection.end ();
  for (ITERATOR i = ace_mon.collection->collection.begin (); i != end; ++i)
    {
      worker->work (*i, ACE_TRY_ENV);
      ACE_TRY_CHECK;
    }
}

template<class PROXY, class C, class I, ACE_SYNCH_DECL> void
TAO_EC_Copy_On_Write<PROXY,C,I,ACE_SYNCH_USE>::
    connected (PROXY *proxy,
               CORBA::Environment &ACE_TRY_ENV)
{
  Write_Guard ace_mon (this->mutex_,
                       this->cond_,
                       this->writing_,
                       this->collection_);

  proxy->_incr_refcnt ();
  ace_mon.copy->collection.connected (proxy, ACE_TRY_ENV);
}

template<class PROXY, class C, class I, ACE_SYNCH_DECL> void
TAO_EC_Copy_On_Write<PROXY,C,I,ACE_SYNCH_USE>::
    reconnected (PROXY *proxy,
                 CORBA::Environment &ACE_TRY_ENV)
{
  Write_Guard ace_mon (this->mutex_,
                       this->cond_,
                       this->writing_,
                       this->collection_);

  proxy->_incr_refcnt ();
  ace_mon.copy->collection.reconnected (proxy, ACE_TRY_ENV);
}

template<class PROXY, class C, class I, ACE_SYNCH_DECL> void
TAO_EC_Copy_On_Write<PROXY,C,I,ACE_SYNCH_USE>::
    disconnected (PROXY *proxy,
                  CORBA::Environment &ACE_TRY_ENV)
{
  Write_Guard ace_mon (this->mutex_,
                       this->cond_,
                       this->writing_,
                       this->collection_);

  ace_mon.copy->collection.disconnected (proxy, ACE_TRY_ENV);
}

template<class PROXY, class C, class I, ACE_SYNCH_DECL> void
TAO_EC_Copy_On_Write<PROXY,C,I,ACE_SYNCH_USE>::
    shutdown (CORBA::Environment &ACE_TRY_ENV)
{
  // @@ Do we really need to perform a copy here?
  //    I believe so, but i don't have a good scenario for it.
  Write_Guard ace_mon (this->mutex_,
                       this->cond_,
                       this->writing_,
                       this->collection_);

  ace_mon.copy->collection.shutdown (ACE_TRY_ENV);
}

// ****************************************************************

#endif /* TAO_EC_COPY_ON_WRITE_CPP */
