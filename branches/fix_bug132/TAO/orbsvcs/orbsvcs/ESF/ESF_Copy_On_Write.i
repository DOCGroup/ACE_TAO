// $Id$

template<class COLLECTION, class ITERATOR> ACE_INLINE
TAO_ESF_Copy_On_Write_Collection<COLLECTION,ITERATOR>::
    TAO_ESF_Copy_On_Write_Collection (void)
      :  refcount_ (1)
{
}

// ****************************************************************

template<class COLLECTION, class ITERATOR, class ACE_LOCK> ACE_INLINE
TAO_ESF_Copy_On_Write_Read_Guard<COLLECTION,ITERATOR,ACE_LOCK>::
    TAO_ESF_Copy_On_Write_Read_Guard (ACE_LOCK &m,
                                      Collection*& collection_ref)
      :  collection (0),
         mutex (m)
{
  ACE_GUARD (ACE_LOCK, ace_mon, this->mutex);
  this->collection = collection_ref;
  this->collection->_incr_refcnt ();
}

template<class COLLECTION, class ITERATOR, class ACE_LOCK> ACE_INLINE
TAO_ESF_Copy_On_Write_Read_Guard<COLLECTION,ITERATOR,ACE_LOCK>::
    ~TAO_ESF_Copy_On_Write_Read_Guard (void)
{
  if (this->collection != 0)
    {
      ACE_GUARD (ACE_LOCK, ace_mon, this->mutex);
      this->collection->_decr_refcnt ();
    }
}

// ****************************************************************

