// $Id$

template<class COLLECTION, class ITERATOR> ACE_INLINE
ACE_RMCast_Copy_On_Write_Collection<COLLECTION,ITERATOR>::
    ACE_RMCast_Copy_On_Write_Collection (void)
      :  refcount_ (1)
{
}

// ****************************************************************

template<class COLLECTION, class ITERATOR> ACE_INLINE
ACE_RMCast_Copy_On_Write_Read_Guard<COLLECTION,ITERATOR>::
    ACE_RMCast_Copy_On_Write_Read_Guard (ACE_SYNCH_MUTEX &m,
                                         Collection*& collection_ref)
  : collection (0)
  , mutex_ (m)
{
  ACE_GUARD (ACE_SYNCH_MUTEX, ace_mon, this->mutex_);
  this->collection = collection_ref;
  this->collection->_incr_refcnt ();
}

template<class COLLECTION, class ITERATOR> ACE_INLINE
ACE_RMCast_Copy_On_Write_Read_Guard<COLLECTION,ITERATOR>::
    ~ACE_RMCast_Copy_On_Write_Read_Guard (void)
{
  if (this->collection != 0)
    {
      ACE_GUARD (ACE_SYNCH_MUTEX, ace_mon, this->mutex_);
      this->collection->_decr_refcnt ();
    }
}

// ****************************************************************
