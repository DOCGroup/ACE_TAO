// $Id$

#ifndef ACE_RMCAST_COPY_ON_WRITE_CPP
#define ACE_RMCAST_COPY_ON_WRITE_CPP

#include "RMCast_Copy_On_Write.h"

#if ! defined (__ACE_INLINE__)
#include "RMCast_Copy_On_Write.i"
#endif /* __ACE_INLINE__ */

ACE_RCSID(RMCast, RMCast_Copy_On_Write, "$Id$")

template<class COLLECTION, class ITERATOR> void
ACE_RMCast_Copy_On_Write_Collection<COLLECTION,ITERATOR>::_incr_refcnt (void)
{
  // LOCKING: no locking is required, the caller grabs the mutex.
  this->refcount_++;
}

template<class COLLECTION, class ITERATOR> void
ACE_RMCast_Copy_On_Write_Collection<COLLECTION,ITERATOR>::_decr_refcnt (void)
{
  // LOCKING: no locking is required, the caller grabs the mutex.
  {
    this->refcount_--;
    if (this->refcount_ != 0)
      return;
  }
  //@@ TODO: If this wrapper is going to be completely general some
  // kind of functor has to be provided to remove the elements in the
  // collection, in case the are no self-managed

  delete this;
}

// ****************************************************************

template<class KEY, class ITEM, class COLLECTION, class ITERATOR>
ACE_RMCast_Copy_On_Write<KEY,ITEM,COLLECTION,ITERATOR>::
    ACE_RMCast_Copy_On_Write (void)
      : ACE_RMCast_Copy_On_Write_Container<COLLECTION,ITERATOR> ()
{
}

template<class KEY, class ITEM, class COLLECTION, class ITERATOR>
ACE_RMCast_Copy_On_Write<KEY,ITEM,COLLECTION,ITERATOR>::
    ~ACE_RMCast_Copy_On_Write (void)
{
  ACE_GUARD (ACE_SYNCH_MUTEX, ace_mon, this->mutex_);

  while (this->pending_writes_ != 0)
    this->cond_.wait ();

  this->collection_->_decr_refcnt ();
  this->collection_ = 0;
}

template<class KEY, class ITEM, class COLLECTION, class ITERATOR> int
ACE_RMCast_Copy_On_Write<KEY,ITEM,COLLECTION,ITERATOR>::
    for_each (ACE_RMCast_Worker<KEY,ITEM> *worker)
{
  Read_Guard ace_mon (*this);

  ITERATOR end = ace_mon.collection->collection.end ();
  for (ITERATOR i = ace_mon.collection->collection.begin (); i != end; ++i)
    {
      int r = worker->work ((*i).key (), (*i).item ());
      if (r == 1)
        return 0; // Abort loop, but no error
      if (r == -1)
        return -1;
    }
  return 0;
}

template<class KEY, class ITEM, class C, class ITERATOR> KEY
ACE_RMCast_Copy_On_Write<KEY,ITEM,C,ITERATOR>::first_key (void)
{
  Read_Guard ace_mon (*this);
  ITERATOR end = ace_mon.collection->collection.end ();
  ITERATOR begin = ace_mon.collection->collection.begin ();
  if (begin == end)
    {
      return KEY ();
    }
  return (*begin).key ();
}

template<class KEY, class ITEM, class C, class ITERATOR> int
ACE_RMCast_Copy_On_Write<KEY,ITEM,C,ITERATOR>::empty (void)
{
  Read_Guard ace_mon (*this);
  ITERATOR end = ace_mon.collection->collection.end ();
  ITERATOR begin = ace_mon.collection->collection.begin ();

  return end == begin;
}

template<class KEY, class ITEM, class C, class I> int
ACE_RMCast_Copy_On_Write<KEY,ITEM,C,I>::bind (KEY const & k,
                                              ITEM const & i)
{
  Write_Guard ace_mon (*this);

  return this->bind_i (ace_mon, k, i);
}

template<class KEY, class ITEM, class C, class I> int
ACE_RMCast_Copy_On_Write<KEY,ITEM,C,I>::unbind (KEY const & k)
{
  Write_Guard ace_mon (*this);

  return this->unbind_i (ace_mon, k);
}

template<class KEY, class ITEM, class C, class I> int
ACE_RMCast_Copy_On_Write<KEY,ITEM,C,I>::bind_i (Write_Guard &ace_mon,
                                                KEY const & k,
                                                ITEM const & i)
{
  return ace_mon.copy->collection.bind (k, i);
}

template<class KEY, class ITEM, class C, class I> int
ACE_RMCast_Copy_On_Write<KEY,ITEM,C,I>::unbind_i (Write_Guard &ace_mon,
                                                  KEY const & k)
{
  return ace_mon.copy->collection.unbind (k);
}

// ****************************************************************

template<class COLLECTION, class ITERATOR>
ACE_RMCast_Copy_On_Write_Container<COLLECTION,ITERATOR>::ACE_RMCast_Copy_On_Write_Container (void)
  : pending_writes_ (0)
  , writing_ (0)
  , cond_ (mutex_)
{
  ACE_NEW (this->collection_, Collection);
}

// ****************************************************************

template<class COLLECTION, class ITERATOR>
ACE_RMCast_Copy_On_Write_Write_Guard<COLLECTION,ITERATOR>::
  ACE_RMCast_Copy_On_Write_Write_Guard (Container &container)
  : copy (0)
  , mutex (container.mutex_)
  , cond (container.cond_)
  , pending_writes (container.pending_writes_)
  , writing_flag (container.writing_)
  , collection (container.collection_)
{
  {
    ACE_GUARD (ACE_SYNCH_MUTEX, ace_mon, this->mutex);

    this->pending_writes++;

    while (this->writing_flag != 0)
      this->cond.wait ();

    this->writing_flag = 1;
  }

  // Copy outside the mutex, because it may take a long time.
  // Nobody can change it, because it is protected by the
  // writing_flag.

  // First initialize it (with the correct reference count
  ACE_NEW (this->copy, Collection);
  // Copy the contents
  this->copy->collection = this->collection->collection;
}

template<class COLLECTION, class ITERATOR>
ACE_RMCast_Copy_On_Write_Write_Guard<COLLECTION,ITERATOR>::
    ~ACE_RMCast_Copy_On_Write_Write_Guard (void)
{
  Collection *tmp = 0;
  {
    ACE_GUARD (ACE_SYNCH_MUTEX, ace_mon, this->mutex);

    tmp = this->collection;
    this->collection = this->copy;
    this->writing_flag = 0;
    this->pending_writes--;

    this->cond.signal ();
  }
  // Delete outside the mutex, because it may take a long time.
  // @@ Is this right?  What happens if several readers are still
  // using the old copy?
  tmp->_decr_refcnt ();
}

// ****************************************************************

#endif /* ACE_RMCAST_COPY_ON_WRITE_CPP */
