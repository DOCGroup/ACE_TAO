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

template<class COLLECTION, class ITERATOR, ACE_SYNCH_DECL> ACE_INLINE
TAO_ESF_Copy_On_Write_Write_Guard<COLLECTION,ITERATOR,ACE_SYNCH_USE>::
    TAO_ESF_Copy_On_Write_Write_Guard (ACE_SYNCH_MUTEX_T &m,
                                      ACE_SYNCH_CONDITION_T &c,
                                      int &w,
                                      Collection*& cr)
      :  copy (0),
         mutex (m),
         cond (c),
         writing_flag (w),
         collection (cr)
{
  {
    ACE_GUARD (ACE_SYNCH_MUTEX_T, ace_mon, this->mutex);

    while (this->writing_flag != 0)
      this->cond.wait ();

    this->writing_flag = 1;
  }

  // Copy outside the mutex, because it may take a long time.
  // Nobody can change it, because it is protected by the
  // writing_flag.
  ACE_NEW (this->copy, Collection (*this->collection));
  ITERATOR end = this->copy->collection.end ();
  for (ITERATOR i = this->copy->collection.begin (); i != end; ++i)
    {
      (*i)->_incr_refcnt ();
    }
}

template<class COLLECTION, class ITERATOR, ACE_SYNCH_DECL> ACE_INLINE
TAO_ESF_Copy_On_Write_Write_Guard<COLLECTION,ITERATOR,ACE_SYNCH_USE>::
    ~TAO_ESF_Copy_On_Write_Write_Guard (void)
{
  Collection *tmp = 0;
  {
    ACE_GUARD (ACE_SYNCH_MUTEX_T, ace_mon, this->mutex);

    tmp = this->collection;
    this->collection = this->copy;
    this->writing_flag = 0;
    this->cond.signal ();
  }
  // Delete outside the mutex, because it may take a long time.
  tmp->_decr_refcnt ();
}
