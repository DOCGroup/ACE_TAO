// $Id$

////////////////////////////////////////////////////////////////////////////////

template <class T> ACE_INLINE int
ACE_Noop_Key_Generator<T>::operator() (T &t)
{
  return -1;
}

////////////////////////////////////////////////////////////////////////////////

template <class T> ACE_INLINE
ACE_Incremental_Key_Generator<T>::ACE_Incremental_Key_Generator (void)
  : t_ (0)
{
}

template <class T> ACE_INLINE int
ACE_Incremental_Key_Generator<T>::operator() (T &t)
{
  t = ++this->t_;
  return 0;
}

template <class T> ACE_INLINE T &
ACE_Incremental_Key_Generator<T>::current_value (void)
{
  return this->t_;
}

////////////////////////////////////////////////////////////////////////////////

template <class T> ACE_INLINE
ACE_Iterator_Impl<T>::~ACE_Iterator_Impl (void)
{
}

////////////////////////////////////////////////////////////////////////////////

template <class T> ACE_INLINE
ACE_Reverse_Iterator_Impl<T>::~ACE_Reverse_Iterator_Impl (void)
{
}

////////////////////////////////////////////////////////////////////////////////

template <class T> ACE_INLINE
ACE_Iterator<T>::ACE_Iterator (ACE_Iterator_Impl<T> *impl)
  : implementation_ (impl)
{
}

template <class T> ACE_INLINE
ACE_Iterator<T>::ACE_Iterator (const ACE_Iterator<T> &rhs)
  : implementation_ (rhs.implementation_->clone ())
{
}

template <class T> ACE_INLINE
ACE_Iterator<T>::~ACE_Iterator (void)
{
  delete this->implementation_;
}

template <class T> ACE_INLINE ACE_Iterator<T> &
ACE_Iterator<T>::operator= (const ACE_Iterator<T> &rhs)
{
  delete this->implementation_;
  this->implementation_ = rhs.implementation_->clone ();
  return *this;
}

template <class T> ACE_INLINE int
ACE_Iterator<T>::operator== (const ACE_Iterator<T> &rhs) const
{
  return this->implementation_->compare (*rhs.implementation_);
}

template <class T> ACE_INLINE int
ACE_Iterator<T>::operator!= (const ACE_Iterator<T> &rhs) const
{
  return !this->operator== (rhs);
}

template <class T> ACE_INLINE T
ACE_Iterator<T>::operator* (void) const
{
  return this->implementation_->dereference ();
}

template <class T> ACE_INLINE ACE_Iterator<T> &
ACE_Iterator<T>::operator++ (void)
{
  this->implementation_->plus_plus ();
  return *this;
}

template <class T> ACE_INLINE ACE_Iterator<T>
ACE_Iterator<T>::operator++ (int)
{
  ACE_Iterator<T> tmp = *this;
  this->implementation_->plus_plus ();
  return tmp;
}

template <class T> ACE_INLINE ACE_Iterator<T> &
ACE_Iterator<T>::operator-- (void)
{
  this->implementation_->minus_minus ();
  return *this;
}

template <class T> ACE_INLINE ACE_Iterator<T>
ACE_Iterator<T>::operator-- (int)
{
  ACE_Iterator<T> tmp = *this;
  this->implementation_->minus_minus ();
  return tmp;
}

template <class T> ACE_INLINE ACE_Iterator_Impl<T> &
ACE_Iterator<T>::impl (void)
{
  return *this->implementation_;
}

////////////////////////////////////////////////////////////////////////////////

template <class T> ACE_INLINE
ACE_Reverse_Iterator<T>::ACE_Reverse_Iterator (ACE_Reverse_Iterator_Impl<T> *impl)
  : implementation_ (impl)
{
}

template <class T> ACE_INLINE
ACE_Reverse_Iterator<T>::ACE_Reverse_Iterator (const ACE_Reverse_Iterator<T> &rhs)
  : implementation_ (rhs.implementation_->clone ())
{
}

template <class T> ACE_INLINE
ACE_Reverse_Iterator<T>::~ACE_Reverse_Iterator (void)
{
  delete this->implementation_;
}

template <class T> ACE_INLINE ACE_Reverse_Iterator<T> &
ACE_Reverse_Iterator<T>::operator= (const ACE_Reverse_Iterator<T> &rhs)
{
  delete this->implementation_;
  this->implementation_ = rhs.implementation_->clone ();
  return *this;
}

template <class T> ACE_INLINE int
ACE_Reverse_Iterator<T>::operator== (const ACE_Reverse_Iterator<T> &rhs) const
{
  return this->implementation_->compare (*rhs.implementation_);
}

template <class T> ACE_INLINE int
ACE_Reverse_Iterator<T>::operator!= (const ACE_Reverse_Iterator<T> &rhs) const
{
  return !this->operator== (rhs);
}

template <class T> ACE_INLINE T
ACE_Reverse_Iterator<T>::operator* (void) const
{
  return this->implementation_->dereference ();
}

template <class T> ACE_INLINE ACE_Reverse_Iterator<T> &
ACE_Reverse_Iterator<T>::operator++ (void)
{
  this->implementation_->plus_plus ();
  return *this;
}

template <class T> ACE_INLINE ACE_Reverse_Iterator<T>
ACE_Reverse_Iterator<T>::operator++ (int)
{
  ACE_Reverse_Iterator<T> tmp = *this;
  this->implementation_->plus_plus ();
  return tmp;
}

template <class T> ACE_INLINE ACE_Reverse_Iterator<T> &
ACE_Reverse_Iterator<T>::operator-- (void)
{
  this->implementation_->minus_minus ();
  return *this;
}

template <class T> ACE_INLINE ACE_Reverse_Iterator<T>
ACE_Reverse_Iterator<T>::operator-- (int)
{
  ACE_Reverse_Iterator<T> tmp = *this;
  this->implementation_->minus_minus ();
  return tmp;
}

template <class T> ACE_INLINE ACE_Reverse_Iterator_Impl<T> &
ACE_Reverse_Iterator<T>::impl (void)
{
  return *this->implementation_;
}

////////////////////////////////////////////////////////////////////////////////

template <class KEY, class VALUE> ACE_INLINE
ACE_Map<KEY, VALUE>::ACE_Map (void)
{
}

template <class KEY, class VALUE> ACE_INLINE
ACE_Map<KEY, VALUE>::~ACE_Map (void)
{
}

template <class KEY, class VALUE> ACE_INLINE ACE_Iterator<ACE_Reference_Pair<const KEY, VALUE> >
ACE_Map<KEY, VALUE>::begin (void)
{
  return iterator (this->begin_impl ());
}

template <class KEY, class VALUE> ACE_INLINE ACE_Iterator<ACE_Reference_Pair<const KEY, VALUE> >
ACE_Map<KEY, VALUE>::end (void)
{
  return iterator (this->end_impl ());
}

template <class KEY, class VALUE> ACE_INLINE ACE_Reverse_Iterator<ACE_Reference_Pair<const KEY, VALUE> >
ACE_Map<KEY, VALUE>::rbegin (void)
{
  return reverse_iterator (this->rbegin_impl ());
}

template <class KEY, class VALUE> ACE_INLINE ACE_Reverse_Iterator<ACE_Reference_Pair<const KEY, VALUE> >
ACE_Map<KEY, VALUE>::rend (void)
{
  return reverse_iterator (this->rend_impl ());
}

////////////////////////////////////////////////////////////////////////////////

template <class T, class IMPLEMENTATION, class ENTRY> ACE_INLINE
ACE_Map_Impl_Iterator_Adapter<T, IMPLEMENTATION, ENTRY>::ACE_Map_Impl_Iterator_Adapter (const IMPLEMENTATION &impl)
  : implementation_ (impl)
{
}

template <class T, class IMPLEMENTATION, class ENTRY> ACE_INLINE
ACE_Map_Impl_Iterator_Adapter<T, IMPLEMENTATION, ENTRY>::~ACE_Map_Impl_Iterator_Adapter (void)
{
}

template <class T, class IMPLEMENTATION, class ENTRY> ACE_INLINE ACE_Iterator_Impl<T> *
ACE_Map_Impl_Iterator_Adapter<T, IMPLEMENTATION, ENTRY>::clone (void) const
{
  return new ACE_Map_Impl_Iterator_Adapter<T, IMPLEMENTATION, ENTRY> (*this);
}


template <class T, class IMPLEMENTATION, class ENTRY> ACE_INLINE int
ACE_Map_Impl_Iterator_Adapter<T, IMPLEMENTATION, ENTRY>::compare (const ACE_Iterator_Impl<T> &rhs_base) const
{
  const ACE_Map_Impl_Iterator_Adapter<T, IMPLEMENTATION, ENTRY> &rhs
    = ACE_dynamic_cast_3_ref (const ACE_Map_Impl_Iterator_Adapter, T, IMPLEMENTATION, ENTRY, rhs_base);

  return this->implementation_ == rhs.implementation_;
}

template <class T, class IMPLEMENTATION, class ENTRY> ACE_INLINE T
ACE_Map_Impl_Iterator_Adapter<T, IMPLEMENTATION, ENTRY>::dereference () const
{
  ENTRY &entry = *this->implementation_;
  return T (entry.ext_id_,
            entry.int_id_);
}

template <class T, class IMPLEMENTATION, class ENTRY> ACE_INLINE void
ACE_Map_Impl_Iterator_Adapter<T, IMPLEMENTATION, ENTRY>::plus_plus (void)
{
  ++this->implementation_;
}

template <class T, class IMPLEMENTATION, class ENTRY> ACE_INLINE void
ACE_Map_Impl_Iterator_Adapter<T, IMPLEMENTATION, ENTRY>::minus_minus (void)
{
  --this->implementation_;
}

template <class T, class IMPLEMENTATION, class ENTRY> ACE_INLINE IMPLEMENTATION &
ACE_Map_Impl_Iterator_Adapter<T, IMPLEMENTATION, ENTRY>::impl (void)
{
  return this->implementation_;
}

////////////////////////////////////////////////////////////////////////////////

template <class T, class IMPLEMENTATION, class ENTRY> ACE_INLINE
ACE_Map_Impl_Reverse_Iterator_Adapter<T, IMPLEMENTATION, ENTRY>::ACE_Map_Impl_Reverse_Iterator_Adapter (const IMPLEMENTATION &impl)
  : implementation_ (impl)
{
}

template <class T, class IMPLEMENTATION, class ENTRY> ACE_INLINE
ACE_Map_Impl_Reverse_Iterator_Adapter<T, IMPLEMENTATION, ENTRY>::~ACE_Map_Impl_Reverse_Iterator_Adapter (void)
{
}

template <class T, class IMPLEMENTATION, class ENTRY> ACE_INLINE ACE_Reverse_Iterator_Impl<T> *
ACE_Map_Impl_Reverse_Iterator_Adapter<T, IMPLEMENTATION, ENTRY>::clone (void) const
{
  return new ACE_Map_Impl_Reverse_Iterator_Adapter<T, IMPLEMENTATION, ENTRY> (*this);
}


template <class T, class IMPLEMENTATION, class ENTRY> ACE_INLINE int
ACE_Map_Impl_Reverse_Iterator_Adapter<T, IMPLEMENTATION, ENTRY>::compare (const ACE_Reverse_Iterator_Impl<T> &rhs_base) const
{
  const ACE_Map_Impl_Reverse_Iterator_Adapter<T, IMPLEMENTATION, ENTRY> &rhs
    = ACE_dynamic_cast_3_ref (const ACE_Map_Impl_Reverse_Iterator_Adapter, T, IMPLEMENTATION, ENTRY, rhs_base);

  return this->implementation_ == rhs.implementation_;
}

template <class T, class IMPLEMENTATION, class ENTRY> ACE_INLINE T
ACE_Map_Impl_Reverse_Iterator_Adapter<T, IMPLEMENTATION, ENTRY>::dereference () const
{
  ENTRY &entry = *this->implementation_;
  return T (entry.ext_id_,
            entry.int_id_);
}

template <class T, class IMPLEMENTATION, class ENTRY> ACE_INLINE void
ACE_Map_Impl_Reverse_Iterator_Adapter<T, IMPLEMENTATION, ENTRY>::plus_plus (void)
{
  ++this->implementation_;
}

template <class T, class IMPLEMENTATION, class ENTRY> ACE_INLINE void
ACE_Map_Impl_Reverse_Iterator_Adapter<T, IMPLEMENTATION, ENTRY>::minus_minus (void)
{
  --this->implementation_;
}

template <class T, class IMPLEMENTATION, class ENTRY> ACE_INLINE IMPLEMENTATION &
ACE_Map_Impl_Reverse_Iterator_Adapter<T, IMPLEMENTATION, ENTRY>::impl (void)
{
  return this->implementation_;
}

////////////////////////////////////////////////////////////////////////////////

template <class KEY, class VALUE, class IMPLEMENTATION, class ITERATOR, class REVERSE_ITERATOR, class ENTRY> ACE_INLINE
ACE_Map_Impl<KEY, VALUE, IMPLEMENTATION, ITERATOR, REVERSE_ITERATOR, ENTRY>::ACE_Map_Impl (ACE_Allocator *alloc)
  : implementation_ (alloc)
{
}

template <class KEY, class VALUE, class IMPLEMENTATION, class ITERATOR, class REVERSE_ITERATOR, class ENTRY> ACE_INLINE
ACE_Map_Impl<KEY, VALUE, IMPLEMENTATION, ITERATOR, REVERSE_ITERATOR, ENTRY>::ACE_Map_Impl (size_t size,
                                                        ACE_Allocator *alloc)
  : implementation_ (size,
                     alloc)
{
}

template <class KEY, class VALUE, class IMPLEMENTATION, class ITERATOR, class REVERSE_ITERATOR, class ENTRY> ACE_INLINE
ACE_Map_Impl<KEY, VALUE, IMPLEMENTATION, ITERATOR, REVERSE_ITERATOR, ENTRY>::~ACE_Map_Impl (void)
{
}

template <class KEY, class VALUE, class IMPLEMENTATION, class ITERATOR, class REVERSE_ITERATOR, class ENTRY> ACE_INLINE int
ACE_Map_Impl<KEY, VALUE, IMPLEMENTATION, ITERATOR, REVERSE_ITERATOR, ENTRY>::open (size_t length,
                                                ACE_Allocator *alloc)
{
  return this->implementation_.open (length,
                                     alloc);
}

template <class KEY, class VALUE, class IMPLEMENTATION, class ITERATOR, class REVERSE_ITERATOR, class ENTRY> ACE_INLINE int
ACE_Map_Impl<KEY, VALUE, IMPLEMENTATION, ITERATOR, REVERSE_ITERATOR, ENTRY>::close (void)
{
  return this->implementation_.close ();
}

template <class KEY, class VALUE, class IMPLEMENTATION, class ITERATOR, class REVERSE_ITERATOR, class ENTRY> ACE_INLINE int
ACE_Map_Impl<KEY, VALUE, IMPLEMENTATION, ITERATOR, REVERSE_ITERATOR, ENTRY>::bind (const KEY &key,
                                                const VALUE &value)
{
  return this->implementation_.bind (key,
                                     value);
}

template <class KEY, class VALUE, class IMPLEMENTATION, class ITERATOR, class REVERSE_ITERATOR, class ENTRY> ACE_INLINE int
ACE_Map_Impl<KEY, VALUE, IMPLEMENTATION, ITERATOR, REVERSE_ITERATOR, ENTRY>::bind_modify_key (const VALUE &value,
                                                           KEY &key)
{
  return this->implementation_.bind_modify_key (value,
                                                key);
}

template <class KEY, class VALUE, class IMPLEMENTATION, class ITERATOR, class REVERSE_ITERATOR, class ENTRY> ACE_INLINE int
ACE_Map_Impl<KEY, VALUE, IMPLEMENTATION, ITERATOR, REVERSE_ITERATOR, ENTRY>::bind_create_key (const VALUE &value,
                                                           KEY &key)
{
  return this->implementation_.bind_create_key (value,
                                                key);
}

template <class KEY, class VALUE, class IMPLEMENTATION, class ITERATOR, class REVERSE_ITERATOR, class ENTRY> ACE_INLINE int
ACE_Map_Impl<KEY, VALUE, IMPLEMENTATION, ITERATOR, REVERSE_ITERATOR, ENTRY>::bind_create_key (const VALUE &value)
{
  return this->implementation_.bind_create_key (value);
}

template <class KEY, class VALUE, class IMPLEMENTATION, class ITERATOR, class REVERSE_ITERATOR, class ENTRY> ACE_INLINE int
ACE_Map_Impl<KEY, VALUE, IMPLEMENTATION, ITERATOR, REVERSE_ITERATOR, ENTRY>::recover_key (const KEY &modified_key,
                                                       KEY &original_key)
{
  return this->implementation_.recover_key (modified_key,
                                            original_key);
}

template <class KEY, class VALUE, class IMPLEMENTATION, class ITERATOR, class REVERSE_ITERATOR, class ENTRY> ACE_INLINE int
ACE_Map_Impl<KEY, VALUE, IMPLEMENTATION, ITERATOR, REVERSE_ITERATOR, ENTRY>::rebind (const KEY &key,
                                                  const VALUE &value)
{
  return this->implementation_.rebind (key,
                                       value);
}

template <class KEY, class VALUE, class IMPLEMENTATION, class ITERATOR, class REVERSE_ITERATOR, class ENTRY> ACE_INLINE int
ACE_Map_Impl<KEY, VALUE, IMPLEMENTATION, ITERATOR, REVERSE_ITERATOR, ENTRY>::rebind (const KEY &key,
                                                  const VALUE &value,
                                                  VALUE &old_value)
{
  return this->implementation_.rebind (key,
                                       value,
                                       old_value);
}

template <class KEY, class VALUE, class IMPLEMENTATION, class ITERATOR, class REVERSE_ITERATOR, class ENTRY> ACE_INLINE int
ACE_Map_Impl<KEY, VALUE, IMPLEMENTATION, ITERATOR, REVERSE_ITERATOR, ENTRY>::rebind (const KEY &key,
                                                  const VALUE &value,
                                                  KEY &old_key,
                                                  VALUE &old_value)
{
  return this->implementation_.rebind (key,
                                       value,
                                       old_key,
                                       old_value);
}

template <class KEY, class VALUE, class IMPLEMENTATION, class ITERATOR, class REVERSE_ITERATOR, class ENTRY> ACE_INLINE int
ACE_Map_Impl<KEY, VALUE, IMPLEMENTATION, ITERATOR, REVERSE_ITERATOR, ENTRY>::trybind (const KEY &key,
                                                   VALUE &value)
{
  return this->implementation_.trybind (key,
                                        value);
}

template <class KEY, class VALUE, class IMPLEMENTATION, class ITERATOR, class REVERSE_ITERATOR, class ENTRY> ACE_INLINE int
ACE_Map_Impl<KEY, VALUE, IMPLEMENTATION, ITERATOR, REVERSE_ITERATOR, ENTRY>::find (const KEY &key,
                                                VALUE &value)
{
  return this->implementation_.find (key,
                                     value);
}

template <class KEY, class VALUE, class IMPLEMENTATION, class ITERATOR, class REVERSE_ITERATOR, class ENTRY> ACE_INLINE int
ACE_Map_Impl<KEY, VALUE, IMPLEMENTATION, ITERATOR, REVERSE_ITERATOR, ENTRY>::find (const KEY &key)
{
  return this->implementation_.find (key);
}

template <class KEY, class VALUE, class IMPLEMENTATION, class ITERATOR, class REVERSE_ITERATOR, class ENTRY> ACE_INLINE int
ACE_Map_Impl<KEY, VALUE, IMPLEMENTATION, ITERATOR, REVERSE_ITERATOR, ENTRY>::unbind (const KEY &key)
{
  return this->implementation_.unbind (key);
}

template <class KEY, class VALUE, class IMPLEMENTATION, class ITERATOR, class REVERSE_ITERATOR, class ENTRY> ACE_INLINE int
ACE_Map_Impl<KEY, VALUE, IMPLEMENTATION, ITERATOR, REVERSE_ITERATOR, ENTRY>::unbind (const KEY &key,
                                                  VALUE &value)
{
  return this->implementation_.unbind (key,
                                       value);
}

template <class KEY, class VALUE, class IMPLEMENTATION, class ITERATOR, class REVERSE_ITERATOR, class ENTRY> ACE_INLINE size_t
ACE_Map_Impl<KEY, VALUE, IMPLEMENTATION, ITERATOR, REVERSE_ITERATOR, ENTRY>::current_size (void)
{
  return this->implementation_.current_size ();
}

template <class KEY, class VALUE, class IMPLEMENTATION, class ITERATOR, class REVERSE_ITERATOR, class ENTRY> ACE_INLINE size_t
ACE_Map_Impl<KEY, VALUE, IMPLEMENTATION, ITERATOR, REVERSE_ITERATOR, ENTRY>::total_size (void)
{
  return this->implementation_.total_size ();
}

template <class KEY, class VALUE, class IMPLEMENTATION, class ITERATOR, class REVERSE_ITERATOR, class ENTRY> ACE_INLINE void
ACE_Map_Impl<KEY, VALUE, IMPLEMENTATION, ITERATOR, REVERSE_ITERATOR, ENTRY>::dump (void) const
{
  this->implementation_.dump ();
}

template <class KEY, class VALUE, class IMPLEMENTATION, class ITERATOR, class REVERSE_ITERATOR, class ENTRY> ACE_INLINE ACE_Iterator_Impl<ACE_Reference_Pair<const KEY, VALUE> > *
ACE_Map_Impl<KEY, VALUE, IMPLEMENTATION, ITERATOR, REVERSE_ITERATOR, ENTRY>::begin_impl (void)
{
  return new iterator_impl (this->implementation_.begin ());
}

template <class KEY, class VALUE, class IMPLEMENTATION, class ITERATOR, class REVERSE_ITERATOR, class ENTRY> ACE_INLINE ACE_Iterator_Impl<ACE_Reference_Pair<const KEY, VALUE> > *
ACE_Map_Impl<KEY, VALUE, IMPLEMENTATION, ITERATOR, REVERSE_ITERATOR, ENTRY>::end_impl (void)
{
  return new iterator_impl (this->implementation_.end ());
}

template <class KEY, class VALUE, class IMPLEMENTATION, class ITERATOR, class REVERSE_ITERATOR, class ENTRY> ACE_INLINE ACE_Reverse_Iterator_Impl<ACE_Reference_Pair<const KEY, VALUE> > *
ACE_Map_Impl<KEY, VALUE, IMPLEMENTATION, ITERATOR, REVERSE_ITERATOR, ENTRY>::rbegin_impl (void)
{
  return new reverse_iterator_impl (this->implementation_.rbegin ());
}

template <class KEY, class VALUE, class IMPLEMENTATION, class ITERATOR, class REVERSE_ITERATOR, class ENTRY> ACE_INLINE ACE_Reverse_Iterator_Impl<ACE_Reference_Pair<const KEY, VALUE> > *
ACE_Map_Impl<KEY, VALUE, IMPLEMENTATION, ITERATOR, REVERSE_ITERATOR, ENTRY>::rend_impl (void)
{
  return new reverse_iterator_impl (this->implementation_.rend ());
}

template <class KEY, class VALUE, class IMPLEMENTATION, class ITERATOR, class REVERSE_ITERATOR, class ENTRY> ACE_INLINE IMPLEMENTATION &
ACE_Map_Impl<KEY, VALUE, IMPLEMENTATION, ITERATOR, REVERSE_ITERATOR, ENTRY>::impl (void)
{
  return this->implementation_;
}

////////////////////////////////////////////////////////////////////////////////

template <class T, class VALUE> ACE_INLINE
ACE_Active_Map_Manager_Iterator_Adapter<T, VALUE>::ACE_Active_Map_Manager_Iterator_Adapter (const ACE_Map_Iterator<ACE_Active_Map_Manager_Key, VALUE, ACE_Null_Mutex> &impl)
  : implementation_ (impl)
{
}

template <class T, class VALUE> ACE_INLINE
ACE_Active_Map_Manager_Iterator_Adapter<T, VALUE>::~ACE_Active_Map_Manager_Iterator_Adapter (void)
{
}

template <class T, class VALUE> ACE_INLINE ACE_Iterator_Impl<T> *
ACE_Active_Map_Manager_Iterator_Adapter<T, VALUE>::clone (void) const
{
  return new ACE_Active_Map_Manager_Iterator_Adapter<T, VALUE> (*this);
}


template <class T, class VALUE> ACE_INLINE int
ACE_Active_Map_Manager_Iterator_Adapter<T, VALUE>::compare (const ACE_Iterator_Impl<T> &rhs_base) const
{
  const ACE_Active_Map_Manager_Iterator_Adapter<T, VALUE> &rhs
    = ACE_dynamic_cast_2_ref (const ACE_Active_Map_Manager_Iterator_Adapter, T, VALUE, rhs_base);

  return this->implementation_ == rhs.implementation_;
}

template <class T, class VALUE> ACE_INLINE T
ACE_Active_Map_Manager_Iterator_Adapter<T, VALUE>::dereference () const
{
  ACE_TYPENAME ACE_Active_Map_Manager<VALUE>::ENTRY &entry = *this->implementation_;
  return T (entry.int_id_.first (),
            entry.int_id_.second ());
}

template <class T, class VALUE> ACE_INLINE void
ACE_Active_Map_Manager_Iterator_Adapter<T, VALUE>::plus_plus (void)
{
  ++this->implementation_;
}

template <class T, class VALUE> ACE_INLINE void
ACE_Active_Map_Manager_Iterator_Adapter<T, VALUE>::minus_minus (void)
{
  --this->implementation_;
}

template <class T, class VALUE> ACE_INLINE ACE_Map_Iterator<ACE_Active_Map_Manager_Key, VALUE, ACE_Null_Mutex> &
ACE_Active_Map_Manager_Iterator_Adapter<T, VALUE>::impl (void)
{
  return this->implementation_;
}

////////////////////////////////////////////////////////////////////////////////

template <class T, class VALUE> ACE_INLINE
ACE_Active_Map_Manager_Reverse_Iterator_Adapter<T, VALUE>::ACE_Active_Map_Manager_Reverse_Iterator_Adapter (const ACE_Map_Reverse_Iterator<ACE_Active_Map_Manager_Key, VALUE, ACE_Null_Mutex> &impl)
  : implementation_ (impl)
{
}

template <class T, class VALUE> ACE_INLINE
ACE_Active_Map_Manager_Reverse_Iterator_Adapter<T, VALUE>::~ACE_Active_Map_Manager_Reverse_Iterator_Adapter (void)
{
}

template <class T, class VALUE> ACE_INLINE ACE_Reverse_Iterator_Impl<T> *
ACE_Active_Map_Manager_Reverse_Iterator_Adapter<T, VALUE>::clone (void) const
{
  return new ACE_Active_Map_Manager_Reverse_Iterator_Adapter<T, VALUE> (*this);
}


template <class T, class VALUE> ACE_INLINE int
ACE_Active_Map_Manager_Reverse_Iterator_Adapter<T, VALUE>::compare (const ACE_Reverse_Iterator_Impl<T> &rhs_base) const
{
  const ACE_Active_Map_Manager_Reverse_Iterator_Adapter<T, VALUE> &rhs
    = ACE_dynamic_cast_2_ref (const ACE_Active_Map_Manager_Reverse_Iterator_Adapter, T, VALUE, rhs_base);

  return this->implementation_ == rhs.implementation_;
}

template <class T, class VALUE> ACE_INLINE T
ACE_Active_Map_Manager_Reverse_Iterator_Adapter<T, VALUE>::dereference () const
{
  ACE_TYPENAME ACE_Active_Map_Manager<VALUE>::ENTRY &entry = *this->implementation_;
  return T (entry.int_id_.first (),
            entry.int_id_.second ());
}

template <class T, class VALUE> ACE_INLINE void
ACE_Active_Map_Manager_Reverse_Iterator_Adapter<T, VALUE>::plus_plus (void)
{
  ++this->implementation_;
}

template <class T, class VALUE> ACE_INLINE void
ACE_Active_Map_Manager_Reverse_Iterator_Adapter<T, VALUE>::minus_minus (void)
{
  --this->implementation_;
}

template <class T, class VALUE> ACE_INLINE ACE_Map_Reverse_Iterator<ACE_Active_Map_Manager_Key, VALUE, ACE_Null_Mutex> &
ACE_Active_Map_Manager_Reverse_Iterator_Adapter<T, VALUE>::impl (void)
{
  return this->implementation_;
}

////////////////////////////////////////////////////////////////////////////////

template <class KEY, class VALUE, class KEY_ADAPTER> ACE_INLINE
ACE_Active_Map_Manager_Adapter<KEY, VALUE, KEY_ADAPTER>::ACE_Active_Map_Manager_Adapter (ACE_Allocator *alloc)
  : implementation_ (alloc)
{
}

template <class KEY, class VALUE, class KEY_ADAPTER> ACE_INLINE
ACE_Active_Map_Manager_Adapter<KEY, VALUE, KEY_ADAPTER>::ACE_Active_Map_Manager_Adapter (size_t size,
                                                                                         ACE_Allocator *alloc)
  : implementation_ (size,
                     alloc)
{
}

template <class KEY, class VALUE, class KEY_ADAPTER> ACE_INLINE
ACE_Active_Map_Manager_Adapter<KEY, VALUE, KEY_ADAPTER>::~ACE_Active_Map_Manager_Adapter (void)
{
}

template <class KEY, class VALUE, class KEY_ADAPTER> ACE_INLINE int
ACE_Active_Map_Manager_Adapter<KEY, VALUE, KEY_ADAPTER>::open (size_t length,
                                                               ACE_Allocator *alloc)
{
  return this->implementation_.open (length,
                                     alloc);
}

template <class KEY, class VALUE, class KEY_ADAPTER> ACE_INLINE int
ACE_Active_Map_Manager_Adapter<KEY, VALUE, KEY_ADAPTER>::close (void)
{
  return this->implementation_.close ();
}

template <class KEY, class VALUE, class KEY_ADAPTER> ACE_INLINE int
ACE_Active_Map_Manager_Adapter<KEY, VALUE, KEY_ADAPTER>::bind (const KEY &,
                                                               const VALUE &)
{
  ACE_NOTSUP_RETURN (-1);
}

template <class KEY, class VALUE, class KEY_ADAPTER> ACE_INLINE int
ACE_Active_Map_Manager_Adapter<KEY, VALUE, KEY_ADAPTER>::bind_modify_key (const VALUE &value,
                                                                          KEY &key)
{
  // Reserve a slot and create an active key.
  expanded_value *internal_value = 0;
  ACE_Active_Map_Manager_Key active_key;
  int result = this->implementation_.bind (active_key,
                                           internal_value);
  if (result == 0)
    {
      // Encode the active key and the existing user key into key part
      // of <expanded_value>.
      result = this->key_adapter_.encode (key,
                                          active_key,
                                          internal_value->first ());
      if (result == 0)
        {
          // Copy user value into <expanded_value>.
          internal_value->second (value);
          // Copy new, modified key back to the user key.
          key = internal_value->first ();
        }
      else
        {
          // In case of errors, unbind from map.
          this->implementation_.unbind (active_key);
        }
    }

  return result;
}

template <class KEY, class VALUE, class KEY_ADAPTER> ACE_INLINE int
ACE_Active_Map_Manager_Adapter<KEY, VALUE, KEY_ADAPTER>::bind_create_key (const VALUE &value,
                                                                          KEY &key)
{
  // Reserve a slot and create an active key.
  expanded_value *internal_value = 0;
  ACE_Active_Map_Manager_Key active_key;
  int result = this->implementation_.bind (active_key,
                                           internal_value);
  if (result == 0)
    {
      // Encode the active key into key part of <expanded_value>.
      result = this->key_adapter_.encode (internal_value->first (),
                                          active_key,
                                          internal_value->first ());
      if (result == 0)
        {
          // Copy user value into <expanded_value>.
          internal_value->second (value);
          // Copy new, modified key to the user key.
          key = internal_value->first ();
        }
      else
        {
          // In case of errors, unbind from map.
          this->implementation_.unbind (active_key);
        }
    }

  return result;
}

template <class KEY, class VALUE, class KEY_ADAPTER> ACE_INLINE int
ACE_Active_Map_Manager_Adapter<KEY, VALUE, KEY_ADAPTER>::bind_create_key (const VALUE &value)
{
  // Reserve a slot and create an active key.
  expanded_value *internal_value = 0;
  ACE_Active_Map_Manager_Key active_key;
  int result = this->implementation_.bind (active_key,
                                           internal_value);
  if (result == 0)
    {
      // Encode the active key into key part of <expanded_value>.
      result = this->key_adapter_.encode (internal_value->first (),
                                          active_key,
                                          internal_value->first ());
      if (result == 0)
        {
          // Copy user value into <expanded_value>.
          internal_value->second (value);
        }
      else
        {
          // In case of errors, unbind from map.
          this->implementation_.unbind (active_key);
        }
    }

  return result;
}

template <class KEY, class VALUE, class KEY_ADAPTER> ACE_INLINE int
ACE_Active_Map_Manager_Adapter<KEY, VALUE, KEY_ADAPTER>::recover_key (const KEY &modified_key,
                                                                      KEY &original_key)
{
  // Ask the <key_adapter_> to help out with recovering the original
  // user key, since it was the one that encode it in the first place.
  return this->key_adapter_.decode (modified_key,
                                    original_key);
}

template <class KEY, class VALUE, class KEY_ADAPTER> ACE_INLINE int
ACE_Active_Map_Manager_Adapter<KEY, VALUE, KEY_ADAPTER>::find (const KEY &key,
                                                               ACE_Pair<KEY, VALUE> *&internal_value)
{
  // Ask the <key_adapter_> to recover the active key.
  ACE_Active_Map_Manager_Key active_key;
  int result = this->key_adapter_.decode (key,
                                          active_key);
  if (result == 0)
    {
      // Find recovered active key in map.
      result = this->implementation_.find (active_key,
                                           internal_value);
    }

  return result;
}

template <class KEY, class VALUE, class KEY_ADAPTER> ACE_INLINE int
ACE_Active_Map_Manager_Adapter<KEY, VALUE, KEY_ADAPTER>::find (const KEY &key,
                                                               VALUE &value)
{
  expanded_value *internal_value = 0;
  int result = this->find (key,
                           internal_value);

  if (result == 0)
    {
      // Copy value.
      value = internal_value->second ();
    }

  return result;
}

template <class KEY, class VALUE, class KEY_ADAPTER> ACE_INLINE int
ACE_Active_Map_Manager_Adapter<KEY, VALUE, KEY_ADAPTER>::find (const KEY &key)
{
  expanded_value *internal_value = 0;
  return this->find (key,
                     internal_value);
}

template <class KEY, class VALUE, class KEY_ADAPTER> ACE_INLINE int
ACE_Active_Map_Manager_Adapter<KEY, VALUE, KEY_ADAPTER>::rebind (const KEY &key,
                                                                 const VALUE &value)
{
  expanded_value *internal_value = 0;
  int result = this->find (key,
                           internal_value);

  if (result == 0)
    {
      // Reset value.
      internal_value->second (value);
    }

  return result;
}

template <class KEY, class VALUE, class KEY_ADAPTER> ACE_INLINE int
ACE_Active_Map_Manager_Adapter<KEY, VALUE, KEY_ADAPTER>::rebind (const KEY &key,
                                                                 const VALUE &value,
                                                                 VALUE &old_value)
{
  expanded_value *internal_value = 0;
  int result = this->find (key,
                           internal_value);

  if (result == 0)
    {
      // Copy old value.
      old_value = internal_value->second ();

      // Reset to new value.
      internal_value->second (value);
    }

  return result;
}

template <class KEY, class VALUE, class KEY_ADAPTER> ACE_INLINE int
ACE_Active_Map_Manager_Adapter<KEY, VALUE, KEY_ADAPTER>::rebind (const KEY &key,
                                                                 const VALUE &value,
                                                                 KEY &old_key,
                                                                 VALUE &old_value)
{
  expanded_value *internal_value = 0;
  int result = this->find (key,
                           internal_value);

  if (result == 0)
    {
      // Copy old key and value.
      old_key = internal_value->first ();
      old_value = internal_value->second ();

      // Reset to new value.
      internal_value->second (value);
    }

  return result;
}

template <class KEY, class VALUE, class KEY_ADAPTER> ACE_INLINE int
ACE_Active_Map_Manager_Adapter<KEY, VALUE, KEY_ADAPTER>::trybind (const KEY &,
                                                                  VALUE &)
{
  ACE_NOTSUP_RETURN (-1);
}

template <class KEY, class VALUE, class KEY_ADAPTER> ACE_INLINE int
ACE_Active_Map_Manager_Adapter<KEY, VALUE, KEY_ADAPTER>::unbind (const KEY &key,
                                                                 ACE_Pair<KEY, VALUE> *&internal_value)
{
  // Ask the <key_adapter_> to recover the active key.
  ACE_Active_Map_Manager_Key active_key;
  int result = this->key_adapter_.decode (key,
                                          active_key);
  if (result == 0)
    {
      // Unbind recovered active key from map.
      result = this->implementation_.unbind (active_key,
                                             internal_value);
    }

  return result;
}

template <class KEY, class VALUE, class KEY_ADAPTER> ACE_INLINE int
ACE_Active_Map_Manager_Adapter<KEY, VALUE, KEY_ADAPTER>::unbind (const KEY &key)
{
  expanded_value *internal_value = 0;
  return this->unbind (key,
                       internal_value);
}

template <class KEY, class VALUE, class KEY_ADAPTER> ACE_INLINE int
ACE_Active_Map_Manager_Adapter<KEY, VALUE, KEY_ADAPTER>::unbind (const KEY &key,
                                                                 VALUE &value)
{
  expanded_value *internal_value = 0;
  int result = this->unbind (key,
                             internal_value);

  if (result == 0)
    {
      // Copy value.
      value = internal_value->second ();
    }

  return result;
}

template <class KEY, class VALUE, class KEY_ADAPTER> ACE_INLINE size_t
ACE_Active_Map_Manager_Adapter<KEY, VALUE, KEY_ADAPTER>::current_size (void)
{
  return this->implementation_.current_size ();
}

template <class KEY, class VALUE, class KEY_ADAPTER> ACE_INLINE size_t
ACE_Active_Map_Manager_Adapter<KEY, VALUE, KEY_ADAPTER>::total_size (void)
{
  return this->implementation_.total_size ();
}

template <class KEY, class VALUE, class KEY_ADAPTER> ACE_INLINE void
ACE_Active_Map_Manager_Adapter<KEY, VALUE, KEY_ADAPTER>::dump (void) const
{
  this->implementation_.dump ();
}

template <class KEY, class VALUE, class KEY_ADAPTER> ACE_INLINE ACE_Iterator_Impl<ACE_Reference_Pair<const KEY, VALUE> > *
ACE_Active_Map_Manager_Adapter<KEY, VALUE, KEY_ADAPTER>::begin_impl (void)
{
  return new iterator_impl (this->implementation_.begin ());
}

template <class KEY, class VALUE, class KEY_ADAPTER> ACE_INLINE ACE_Iterator_Impl<ACE_Reference_Pair<const KEY, VALUE> > *
ACE_Active_Map_Manager_Adapter<KEY, VALUE, KEY_ADAPTER>::end_impl (void)
{
  return new iterator_impl (this->implementation_.end ());
}

template <class KEY, class VALUE, class KEY_ADAPTER> ACE_INLINE ACE_Reverse_Iterator_Impl<ACE_Reference_Pair<const KEY, VALUE> > *
ACE_Active_Map_Manager_Adapter<KEY, VALUE, KEY_ADAPTER>::rbegin_impl (void)
{
  return new reverse_iterator_impl (this->implementation_.rbegin ());
}

template <class KEY, class VALUE, class KEY_ADAPTER> ACE_INLINE ACE_Reverse_Iterator_Impl<ACE_Reference_Pair<const KEY, VALUE> > *
ACE_Active_Map_Manager_Adapter<KEY, VALUE, KEY_ADAPTER>::rend_impl (void)
{
  return new reverse_iterator_impl (this->implementation_.rend ());
}

template <class KEY, class VALUE, class KEY_ADAPTER> ACE_INLINE ACE_Active_Map_Manager<ACE_Pair<KEY, VALUE> > &
ACE_Active_Map_Manager_Adapter<KEY, VALUE, KEY_ADAPTER>::impl (void)
{
  return this->implementation_;
}

template <class KEY, class VALUE, class KEY_ADAPTER> ACE_INLINE KEY_ADAPTER &
ACE_Active_Map_Manager_Adapter<KEY, VALUE, KEY_ADAPTER>::key_adapter (void)
{
  return this->key_adapter_;
}

////////////////////////////////////////////////////////////////////////////////

template <class T, class KEY, class VALUE, class HASH_KEY, class COMPARE_KEYS> ACE_INLINE
ACE_Hash_Map_Manager_Ex_Iterator_Adapter<T, KEY, VALUE, HASH_KEY, COMPARE_KEYS>::ACE_Hash_Map_Manager_Ex_Iterator_Adapter (const ACE_Hash_Map_Iterator_Ex<KEY, VALUE, HASH_KEY, COMPARE_KEYS, ACE_Null_Mutex> &impl)
  : implementation_ (impl)
{
}

template <class T, class KEY, class VALUE, class HASH_KEY, class COMPARE_KEYS> ACE_INLINE
ACE_Hash_Map_Manager_Ex_Iterator_Adapter<T, KEY, VALUE, HASH_KEY, COMPARE_KEYS>::~ACE_Hash_Map_Manager_Ex_Iterator_Adapter (void)
{
}

template <class T, class KEY, class VALUE, class HASH_KEY, class COMPARE_KEYS> ACE_INLINE ACE_Iterator_Impl<T> *
ACE_Hash_Map_Manager_Ex_Iterator_Adapter<T, KEY, VALUE, HASH_KEY, COMPARE_KEYS>::clone (void) const
{
  return new ACE_Hash_Map_Manager_Ex_Iterator_Adapter<T, KEY, VALUE, HASH_KEY, COMPARE_KEYS> (*this);
}


template <class T, class KEY, class VALUE, class HASH_KEY, class COMPARE_KEYS> ACE_INLINE int
ACE_Hash_Map_Manager_Ex_Iterator_Adapter<T, KEY, VALUE, HASH_KEY, COMPARE_KEYS>::compare (const ACE_Iterator_Impl<T> &rhs_base) const
{
  const ACE_Hash_Map_Manager_Ex_Iterator_Adapter<T, KEY, VALUE, HASH_KEY, COMPARE_KEYS> &rhs
    = ACE_dynamic_cast_5_ref (const ACE_Hash_Map_Manager_Ex_Iterator_Adapter, T, KEY, VALUE, HASH_KEY, COMPARE_KEYS, rhs_base);

  return this->implementation_ == rhs.implementation_;
}

template <class T, class KEY, class VALUE, class HASH_KEY, class COMPARE_KEYS> ACE_INLINE T
ACE_Hash_Map_Manager_Ex_Iterator_Adapter<T, KEY, VALUE, HASH_KEY, COMPARE_KEYS>::dereference () const
{
  ACE_TYPENAME ACE_Hash_Map_Manager_Ex<KEY, VALUE, HASH_KEY, COMPARE_KEYS, ACE_Null_Mutex>::ENTRY &entry = *this->implementation_;
  return T (entry.ext_id_,
            entry.int_id_);
}

template <class T, class KEY, class VALUE, class HASH_KEY, class COMPARE_KEYS> ACE_INLINE void
ACE_Hash_Map_Manager_Ex_Iterator_Adapter<T, KEY, VALUE, HASH_KEY, COMPARE_KEYS>::plus_plus (void)
{
  ++this->implementation_;
}

template <class T, class KEY, class VALUE, class HASH_KEY, class COMPARE_KEYS> ACE_INLINE void
ACE_Hash_Map_Manager_Ex_Iterator_Adapter<T, KEY, VALUE, HASH_KEY, COMPARE_KEYS>::minus_minus (void)
{
  --this->implementation_;
}

template <class T, class KEY, class VALUE, class HASH_KEY, class COMPARE_KEYS> ACE_INLINE ACE_Hash_Map_Iterator_Ex<KEY, VALUE, HASH_KEY, COMPARE_KEYS, ACE_Null_Mutex> &
ACE_Hash_Map_Manager_Ex_Iterator_Adapter<T, KEY, VALUE, HASH_KEY, COMPARE_KEYS>::impl (void)
{
  return this->implementation_;
}

////////////////////////////////////////////////////////////////////////////////

template <class T, class KEY, class VALUE, class HASH_KEY, class COMPARE_KEYS> ACE_INLINE
ACE_Hash_Map_Manager_Ex_Reverse_Iterator_Adapter<T, KEY, VALUE, HASH_KEY, COMPARE_KEYS>::ACE_Hash_Map_Manager_Ex_Reverse_Iterator_Adapter (const ACE_Hash_Map_Reverse_Iterator_Ex<KEY, VALUE, HASH_KEY, COMPARE_KEYS, ACE_Null_Mutex> &impl)
  : implementation_ (impl)
{
}

template <class T, class KEY, class VALUE, class HASH_KEY, class COMPARE_KEYS> ACE_INLINE
ACE_Hash_Map_Manager_Ex_Reverse_Iterator_Adapter<T, KEY, VALUE, HASH_KEY, COMPARE_KEYS>::~ACE_Hash_Map_Manager_Ex_Reverse_Iterator_Adapter (void)
{
}

template <class T, class KEY, class VALUE, class HASH_KEY, class COMPARE_KEYS> ACE_INLINE ACE_Reverse_Iterator_Impl<T> *
ACE_Hash_Map_Manager_Ex_Reverse_Iterator_Adapter<T, KEY, VALUE, HASH_KEY, COMPARE_KEYS>::clone (void) const
{
  return new ACE_Hash_Map_Manager_Ex_Reverse_Iterator_Adapter<T, KEY, VALUE, HASH_KEY, COMPARE_KEYS> (*this);
}


template <class T, class KEY, class VALUE, class HASH_KEY, class COMPARE_KEYS> ACE_INLINE int
ACE_Hash_Map_Manager_Ex_Reverse_Iterator_Adapter<T, KEY, VALUE, HASH_KEY, COMPARE_KEYS>::compare (const ACE_Reverse_Iterator_Impl<T> &rhs_base) const
{
  const ACE_Hash_Map_Manager_Ex_Reverse_Iterator_Adapter<T, KEY, VALUE, HASH_KEY, COMPARE_KEYS> &rhs
    = ACE_dynamic_cast_5_ref (const ACE_Hash_Map_Manager_Ex_Reverse_Iterator_Adapter, T, KEY, VALUE, HASH_KEY, COMPARE_KEYS, rhs_base);

  return this->implementation_ == rhs.implementation_;
}

template <class T, class KEY, class VALUE, class HASH_KEY, class COMPARE_KEYS> ACE_INLINE T
ACE_Hash_Map_Manager_Ex_Reverse_Iterator_Adapter<T, KEY, VALUE, HASH_KEY, COMPARE_KEYS>::dereference () const
{
  ACE_TYPENAME ACE_Hash_Map_Manager_Ex<KEY, VALUE, HASH_KEY, COMPARE_KEYS, ACE_Null_Mutex>::ENTRY &entry = *this->implementation_;
  return T (entry.ext_id_,
            entry.int_id_);
}

template <class T, class KEY, class VALUE, class HASH_KEY, class COMPARE_KEYS> ACE_INLINE void
ACE_Hash_Map_Manager_Ex_Reverse_Iterator_Adapter<T, KEY, VALUE, HASH_KEY, COMPARE_KEYS>::plus_plus (void)
{
  ++this->implementation_;
}

template <class T, class KEY, class VALUE, class HASH_KEY, class COMPARE_KEYS> ACE_INLINE void
ACE_Hash_Map_Manager_Ex_Reverse_Iterator_Adapter<T, KEY, VALUE, HASH_KEY, COMPARE_KEYS>::minus_minus (void)
{
  --this->implementation_;
}

template <class T, class KEY, class VALUE, class HASH_KEY, class COMPARE_KEYS> ACE_INLINE ACE_Hash_Map_Reverse_Iterator_Ex<KEY, VALUE, HASH_KEY, COMPARE_KEYS, ACE_Null_Mutex> &
ACE_Hash_Map_Manager_Ex_Reverse_Iterator_Adapter<T, KEY, VALUE, HASH_KEY, COMPARE_KEYS>::impl (void)
{
  return this->implementation_;
}

////////////////////////////////////////////////////////////////////////////////

template <class KEY, class VALUE, class HASH_KEY, class COMPARE_KEYS, class KEY_GENERATOR> ACE_INLINE
ACE_Hash_Map_Manager_Ex_Adapter<KEY, VALUE, HASH_KEY, COMPARE_KEYS, KEY_GENERATOR>::ACE_Hash_Map_Manager_Ex_Adapter (ACE_Allocator *alloc)
  : implementation_ (alloc)
{
}

template <class KEY, class VALUE, class HASH_KEY, class COMPARE_KEYS, class KEY_GENERATOR> ACE_INLINE
ACE_Hash_Map_Manager_Ex_Adapter<KEY, VALUE, HASH_KEY, COMPARE_KEYS, KEY_GENERATOR>::ACE_Hash_Map_Manager_Ex_Adapter (size_t size,
                                                                                                                     ACE_Allocator *alloc)
  : implementation_ (size,
                     alloc)
{
}

template <class KEY, class VALUE, class HASH_KEY, class COMPARE_KEYS, class KEY_GENERATOR> ACE_INLINE
ACE_Hash_Map_Manager_Ex_Adapter<KEY, VALUE, HASH_KEY, COMPARE_KEYS, KEY_GENERATOR>::~ACE_Hash_Map_Manager_Ex_Adapter (void)
{
}

template <class KEY, class VALUE, class HASH_KEY, class COMPARE_KEYS, class KEY_GENERATOR> ACE_INLINE int
ACE_Hash_Map_Manager_Ex_Adapter<KEY, VALUE, HASH_KEY, COMPARE_KEYS, KEY_GENERATOR>::open (size_t length,
                                                                                          ACE_Allocator *alloc)
{
  return this->implementation_.open (length,
                                     alloc);
}

template <class KEY, class VALUE, class HASH_KEY, class COMPARE_KEYS, class KEY_GENERATOR> ACE_INLINE int
ACE_Hash_Map_Manager_Ex_Adapter<KEY, VALUE, HASH_KEY, COMPARE_KEYS, KEY_GENERATOR>::close (void)
{
  return this->implementation_.close ();
}

template <class KEY, class VALUE, class HASH_KEY, class COMPARE_KEYS, class KEY_GENERATOR> ACE_INLINE int
ACE_Hash_Map_Manager_Ex_Adapter<KEY, VALUE, HASH_KEY, COMPARE_KEYS, KEY_GENERATOR>::bind (const KEY &key,
                                                                                          const VALUE &value)
{
  return this->implementation_.bind (key,
                                     value);
}

template <class KEY, class VALUE, class HASH_KEY, class COMPARE_KEYS, class KEY_GENERATOR> ACE_INLINE int
ACE_Hash_Map_Manager_Ex_Adapter<KEY, VALUE, HASH_KEY, COMPARE_KEYS, KEY_GENERATOR>::bind_modify_key (const VALUE &value,
                                                                                                     KEY &key)
{
  return this->implementation_.bind (key,
                                     value);
}

template <class KEY, class VALUE, class HASH_KEY, class COMPARE_KEYS, class KEY_GENERATOR> ACE_INLINE int
ACE_Hash_Map_Manager_Ex_Adapter<KEY, VALUE, HASH_KEY, COMPARE_KEYS, KEY_GENERATOR>::bind_create_key (const VALUE &value,
                                                                                                     KEY &key)
{
  // Invoke the user specified key generation functor.
  int result = this->key_generator_ (key);

  if (result == 0)
    {
      // Try to add.
      result = this->implementation_.bind (key,
                                           value);
    }

  return result;
}

template <class KEY, class VALUE, class HASH_KEY, class COMPARE_KEYS, class KEY_GENERATOR> ACE_INLINE int
ACE_Hash_Map_Manager_Ex_Adapter<KEY, VALUE, HASH_KEY, COMPARE_KEYS, KEY_GENERATOR>::bind_create_key (const VALUE &value)
{
  KEY key;
  return this->bind_create_key (value,
                                key);
}

template <class KEY, class VALUE, class HASH_KEY, class COMPARE_KEYS, class KEY_GENERATOR> ACE_INLINE int
ACE_Hash_Map_Manager_Ex_Adapter<KEY, VALUE, HASH_KEY, COMPARE_KEYS, KEY_GENERATOR>::recover_key (const KEY &modified_key,
                                                                                                 KEY &original_key)
{
  original_key = modified_key;
  return 0;
}

template <class KEY, class VALUE, class HASH_KEY, class COMPARE_KEYS, class KEY_GENERATOR> ACE_INLINE int
ACE_Hash_Map_Manager_Ex_Adapter<KEY, VALUE, HASH_KEY, COMPARE_KEYS, KEY_GENERATOR>::rebind (const KEY &key,
                                                                                            const VALUE &value)
{
  return this->implementation_.rebind (key,
                                       value);
}

template <class KEY, class VALUE, class HASH_KEY, class COMPARE_KEYS, class KEY_GENERATOR> ACE_INLINE int
ACE_Hash_Map_Manager_Ex_Adapter<KEY, VALUE, HASH_KEY, COMPARE_KEYS, KEY_GENERATOR>::rebind (const KEY &key,
                                                                                            const VALUE &value,
                                                                                            VALUE &old_value)
{
  return this->implementation_.rebind (key,
                                       value,
                                       old_value);
}

template <class KEY, class VALUE, class HASH_KEY, class COMPARE_KEYS, class KEY_GENERATOR> ACE_INLINE int
ACE_Hash_Map_Manager_Ex_Adapter<KEY, VALUE, HASH_KEY, COMPARE_KEYS, KEY_GENERATOR>::rebind (const KEY &key,
                                                                                            const VALUE &value,
                                                                                            KEY &old_key,
                                                                                            VALUE &old_value)
{
  return this->implementation_.rebind (key,
                                       value,
                                       old_key,
                                       old_value);
}

template <class KEY, class VALUE, class HASH_KEY, class COMPARE_KEYS, class KEY_GENERATOR> ACE_INLINE int
ACE_Hash_Map_Manager_Ex_Adapter<KEY, VALUE, HASH_KEY, COMPARE_KEYS, KEY_GENERATOR>::trybind (const KEY &key,
                                                                                             VALUE &value)
{
  return this->implementation_.trybind (key,
                                        value);
}

template <class KEY, class VALUE, class HASH_KEY, class COMPARE_KEYS, class KEY_GENERATOR> ACE_INLINE int
ACE_Hash_Map_Manager_Ex_Adapter<KEY, VALUE, HASH_KEY, COMPARE_KEYS, KEY_GENERATOR>::find (const KEY &key,
                                                                                          VALUE &value)
{
  return this->implementation_.find (key,
                                     value);
}

template <class KEY, class VALUE, class HASH_KEY, class COMPARE_KEYS, class KEY_GENERATOR> ACE_INLINE int
ACE_Hash_Map_Manager_Ex_Adapter<KEY, VALUE, HASH_KEY, COMPARE_KEYS, KEY_GENERATOR>::find (const KEY &key)
{
  return this->implementation_.find (key);
}

template <class KEY, class VALUE, class HASH_KEY, class COMPARE_KEYS, class KEY_GENERATOR> ACE_INLINE int
ACE_Hash_Map_Manager_Ex_Adapter<KEY, VALUE, HASH_KEY, COMPARE_KEYS, KEY_GENERATOR>::unbind (const KEY &key)
{
  return this->implementation_.unbind (key);
}

template <class KEY, class VALUE, class HASH_KEY, class COMPARE_KEYS, class KEY_GENERATOR> ACE_INLINE int
ACE_Hash_Map_Manager_Ex_Adapter<KEY, VALUE, HASH_KEY, COMPARE_KEYS, KEY_GENERATOR>::unbind (const KEY &key,
                                                                                            VALUE &value)
{
  return this->implementation_.unbind (key,
                                       value);
}

template <class KEY, class VALUE, class HASH_KEY, class COMPARE_KEYS, class KEY_GENERATOR> ACE_INLINE size_t
ACE_Hash_Map_Manager_Ex_Adapter<KEY, VALUE, HASH_KEY, COMPARE_KEYS, KEY_GENERATOR>::current_size (void)
{
  return this->implementation_.current_size ();
}

template <class KEY, class VALUE, class HASH_KEY, class COMPARE_KEYS, class KEY_GENERATOR> ACE_INLINE size_t
ACE_Hash_Map_Manager_Ex_Adapter<KEY, VALUE, HASH_KEY, COMPARE_KEYS, KEY_GENERATOR>::total_size (void)
{
  return this->implementation_.total_size ();
}

template <class KEY, class VALUE, class HASH_KEY, class COMPARE_KEYS, class KEY_GENERATOR> ACE_INLINE void
ACE_Hash_Map_Manager_Ex_Adapter<KEY, VALUE, HASH_KEY, COMPARE_KEYS, KEY_GENERATOR>::dump (void) const
{
  this->implementation_.dump ();
}

template <class KEY, class VALUE, class HASH_KEY, class COMPARE_KEYS, class KEY_GENERATOR> ACE_INLINE ACE_Iterator_Impl<ACE_Reference_Pair<const KEY, VALUE> > *
ACE_Hash_Map_Manager_Ex_Adapter<KEY, VALUE, HASH_KEY, COMPARE_KEYS, KEY_GENERATOR>::begin_impl (void)
{
  return new iterator_impl (this->implementation_.begin ());
}

template <class KEY, class VALUE, class HASH_KEY, class COMPARE_KEYS, class KEY_GENERATOR> ACE_INLINE ACE_Iterator_Impl<ACE_Reference_Pair<const KEY, VALUE> > *
ACE_Hash_Map_Manager_Ex_Adapter<KEY, VALUE, HASH_KEY, COMPARE_KEYS, KEY_GENERATOR>::end_impl (void)
{
  return new iterator_impl (this->implementation_.end ());
}

template <class KEY, class VALUE, class HASH_KEY, class COMPARE_KEYS, class KEY_GENERATOR> ACE_INLINE ACE_Reverse_Iterator_Impl<ACE_Reference_Pair<const KEY, VALUE> > *
ACE_Hash_Map_Manager_Ex_Adapter<KEY, VALUE, HASH_KEY, COMPARE_KEYS, KEY_GENERATOR>::rbegin_impl (void)
{
  return new reverse_iterator_impl (this->implementation_.rbegin ());
}

template <class KEY, class VALUE, class HASH_KEY, class COMPARE_KEYS, class KEY_GENERATOR> ACE_INLINE ACE_Reverse_Iterator_Impl<ACE_Reference_Pair<const KEY, VALUE> > *
ACE_Hash_Map_Manager_Ex_Adapter<KEY, VALUE, HASH_KEY, COMPARE_KEYS, KEY_GENERATOR>::rend_impl (void)
{
  return new reverse_iterator_impl (this->implementation_.rend ());
}

template <class KEY, class VALUE, class HASH_KEY, class COMPARE_KEYS, class KEY_GENERATOR> ACE_INLINE ACE_Hash_Map_Manager_Ex<KEY, VALUE, HASH_KEY, COMPARE_KEYS, ACE_Null_Mutex> &
ACE_Hash_Map_Manager_Ex_Adapter<KEY, VALUE, HASH_KEY, COMPARE_KEYS, KEY_GENERATOR>::impl (void)
{
  return this->implementation_;
}

template <class KEY, class VALUE, class HASH_KEY, class COMPARE_KEYS, class KEY_GENERATOR> ACE_INLINE KEY_GENERATOR &
ACE_Hash_Map_Manager_Ex_Adapter<KEY, VALUE, HASH_KEY, COMPARE_KEYS, KEY_GENERATOR>::key_generator (void)
{
  return this->key_generator_;
}

////////////////////////////////////////////////////////////////////////////////

template <class T, class KEY, class VALUE> ACE_INLINE
ACE_Map_Manager_Iterator_Adapter<T, KEY, VALUE>::ACE_Map_Manager_Iterator_Adapter (const ACE_Map_Iterator<KEY, VALUE, ACE_Null_Mutex> &impl)
  : implementation_ (impl)
{
}

template <class T, class KEY, class VALUE> ACE_INLINE
ACE_Map_Manager_Iterator_Adapter<T, KEY, VALUE>::~ACE_Map_Manager_Iterator_Adapter (void)
{
}

template <class T, class KEY, class VALUE> ACE_INLINE ACE_Iterator_Impl<T> *
ACE_Map_Manager_Iterator_Adapter<T, KEY, VALUE>::clone (void) const
{
  return new ACE_Map_Manager_Iterator_Adapter<T, KEY, VALUE> (*this);
}


template <class T, class KEY, class VALUE> ACE_INLINE int
ACE_Map_Manager_Iterator_Adapter<T, KEY, VALUE>::compare (const ACE_Iterator_Impl<T> &rhs_base) const
{
  const ACE_Map_Manager_Iterator_Adapter<T, KEY, VALUE> &rhs
    = ACE_dynamic_cast_3_ref (const ACE_Map_Manager_Iterator_Adapter, T, KEY, VALUE, rhs_base);

  return this->implementation_ == rhs.implementation_;
}

template <class T, class KEY, class VALUE> ACE_INLINE T
ACE_Map_Manager_Iterator_Adapter<T, KEY, VALUE>::dereference () const
{
  ACE_TYPENAME ACE_Map_Manager<KEY, VALUE, ACE_Null_Mutex>::ENTRY &entry = *this->implementation_;
  return T (entry.ext_id_,
            entry.int_id_);
}

template <class T, class KEY, class VALUE> ACE_INLINE void
ACE_Map_Manager_Iterator_Adapter<T, KEY, VALUE>::plus_plus (void)
{
  ++this->implementation_;
}

template <class T, class KEY, class VALUE> ACE_INLINE void
ACE_Map_Manager_Iterator_Adapter<T, KEY, VALUE>::minus_minus (void)
{
  --this->implementation_;
}

template <class T, class KEY, class VALUE> ACE_INLINE ACE_Map_Iterator<KEY, VALUE, ACE_Null_Mutex> &
ACE_Map_Manager_Iterator_Adapter<T, KEY, VALUE>::impl (void)
{
  return this->implementation_;
}

////////////////////////////////////////////////////////////////////////////////

template <class T, class KEY, class VALUE> ACE_INLINE
ACE_Map_Manager_Reverse_Iterator_Adapter<T, KEY, VALUE>::ACE_Map_Manager_Reverse_Iterator_Adapter (const ACE_Map_Reverse_Iterator<KEY, VALUE, ACE_Null_Mutex> &impl)
  : implementation_ (impl)
{
}

template <class T, class KEY, class VALUE> ACE_INLINE
ACE_Map_Manager_Reverse_Iterator_Adapter<T, KEY, VALUE>::~ACE_Map_Manager_Reverse_Iterator_Adapter (void)
{
}

template <class T, class KEY, class VALUE> ACE_INLINE ACE_Reverse_Iterator_Impl<T> *
ACE_Map_Manager_Reverse_Iterator_Adapter<T, KEY, VALUE>::clone (void) const
{
  return new ACE_Map_Manager_Reverse_Iterator_Adapter<T, KEY, VALUE> (*this);
}


template <class T, class KEY, class VALUE> ACE_INLINE int
ACE_Map_Manager_Reverse_Iterator_Adapter<T, KEY, VALUE>::compare (const ACE_Reverse_Iterator_Impl<T> &rhs_base) const
{
  const ACE_Map_Manager_Reverse_Iterator_Adapter<T, KEY, VALUE> &rhs
    = ACE_dynamic_cast_3_ref (const ACE_Map_Manager_Reverse_Iterator_Adapter, T, KEY, VALUE, rhs_base);

  return this->implementation_ == rhs.implementation_;
}

template <class T, class KEY, class VALUE> ACE_INLINE T
ACE_Map_Manager_Reverse_Iterator_Adapter<T, KEY, VALUE>::dereference () const
{
  ACE_TYPENAME ACE_Map_Manager<KEY, VALUE, ACE_Null_Mutex>::ENTRY &entry = *this->implementation_;
  return T (entry.ext_id_,
            entry.int_id_);
}

template <class T, class KEY, class VALUE> ACE_INLINE void
ACE_Map_Manager_Reverse_Iterator_Adapter<T, KEY, VALUE>::plus_plus (void)
{
  ++this->implementation_;
}

template <class T, class KEY, class VALUE> ACE_INLINE void
ACE_Map_Manager_Reverse_Iterator_Adapter<T, KEY, VALUE>::minus_minus (void)
{
  --this->implementation_;
}

template <class T, class KEY, class VALUE> ACE_INLINE ACE_Map_Reverse_Iterator<KEY, VALUE, ACE_Null_Mutex> &
ACE_Map_Manager_Reverse_Iterator_Adapter<T, KEY, VALUE>::impl (void)
{
  return this->implementation_;
}

////////////////////////////////////////////////////////////////////////////////

template <class KEY, class VALUE, class KEY_GENERATOR> ACE_INLINE
ACE_Map_Manager_Adapter<KEY, VALUE, KEY_GENERATOR>::ACE_Map_Manager_Adapter (ACE_Allocator *alloc)
  : implementation_ (alloc)
{
}

template <class KEY, class VALUE, class KEY_GENERATOR> ACE_INLINE
ACE_Map_Manager_Adapter<KEY, VALUE, KEY_GENERATOR>::ACE_Map_Manager_Adapter (size_t size,
                                                                             ACE_Allocator *alloc)
  : implementation_ (size,
                     alloc)
{
}

template <class KEY, class VALUE, class KEY_GENERATOR> ACE_INLINE
ACE_Map_Manager_Adapter<KEY, VALUE, KEY_GENERATOR>::~ACE_Map_Manager_Adapter (void)
{
}

template <class KEY, class VALUE, class KEY_GENERATOR> ACE_INLINE int
ACE_Map_Manager_Adapter<KEY, VALUE, KEY_GENERATOR>::open (size_t length,
                                                          ACE_Allocator *alloc)
{
  return this->implementation_.open (length,
                                     alloc);
}

template <class KEY, class VALUE, class KEY_GENERATOR> ACE_INLINE int
ACE_Map_Manager_Adapter<KEY, VALUE, KEY_GENERATOR>::close (void)
{
  return this->implementation_.close ();
}

template <class KEY, class VALUE, class KEY_GENERATOR> ACE_INLINE int
ACE_Map_Manager_Adapter<KEY, VALUE, KEY_GENERATOR>::bind (const KEY &key,
                                                          const VALUE &value)
{
  return this->implementation_.bind (key,
                                     value);
}

template <class KEY, class VALUE, class KEY_GENERATOR> ACE_INLINE int
ACE_Map_Manager_Adapter<KEY, VALUE, KEY_GENERATOR>::bind_modify_key (const VALUE &value,
                                                                     KEY &key)
{
  return this->implementation_.bind (key,
                                     value);
}

template <class KEY, class VALUE, class KEY_GENERATOR> ACE_INLINE int
ACE_Map_Manager_Adapter<KEY, VALUE, KEY_GENERATOR>::bind_create_key (const VALUE &value,
                                                                     KEY &key)
{
  // Invoke the user specified key generation functor.
  int result = this->key_generator_ (key);

  if (result == 0)
    {
      // Try to add.
      result = this->implementation_.bind (key,
                                           value);
    }

  return result;
}

template <class KEY, class VALUE, class KEY_GENERATOR> ACE_INLINE int
ACE_Map_Manager_Adapter<KEY, VALUE, KEY_GENERATOR>::bind_create_key (const VALUE &value)
{
  KEY key;
  return this->bind_create_key (value,
                                key);
}

template <class KEY, class VALUE, class KEY_GENERATOR> ACE_INLINE int
ACE_Map_Manager_Adapter<KEY, VALUE, KEY_GENERATOR>::recover_key (const KEY &modified_key,
                                                                 KEY &original_key)
{
  original_key = modified_key;
  return 0;
}

template <class KEY, class VALUE, class KEY_GENERATOR> ACE_INLINE int
ACE_Map_Manager_Adapter<KEY, VALUE, KEY_GENERATOR>::rebind (const KEY &key,
                                                            const VALUE &value)
{
  return this->implementation_.rebind (key,
                                       value);
}

template <class KEY, class VALUE, class KEY_GENERATOR> ACE_INLINE int
ACE_Map_Manager_Adapter<KEY, VALUE, KEY_GENERATOR>::rebind (const KEY &key,
                                                            const VALUE &value,
                                                            VALUE &old_value)
{
  return this->implementation_.rebind (key,
                                       value,
                                       old_value);
}

template <class KEY, class VALUE, class KEY_GENERATOR> ACE_INLINE int
ACE_Map_Manager_Adapter<KEY, VALUE, KEY_GENERATOR>::rebind (const KEY &key,
                                                            const VALUE &value,
                                                            KEY &old_key,
                                                            VALUE &old_value)
{
  return this->implementation_.rebind (key,
                                       value,
                                       old_key,
                                       old_value);
}

template <class KEY, class VALUE, class KEY_GENERATOR> ACE_INLINE int
ACE_Map_Manager_Adapter<KEY, VALUE, KEY_GENERATOR>::trybind (const KEY &key,
                                                             VALUE &value)
{
  return this->implementation_.trybind (key,
                                        value);
}

template <class KEY, class VALUE, class KEY_GENERATOR> ACE_INLINE int
ACE_Map_Manager_Adapter<KEY, VALUE, KEY_GENERATOR>::find (const KEY &key,
                                                          VALUE &value)
{
  return this->implementation_.find (key,
                                     value);
}

template <class KEY, class VALUE, class KEY_GENERATOR> ACE_INLINE int
ACE_Map_Manager_Adapter<KEY, VALUE, KEY_GENERATOR>::find (const KEY &key)
{
  return this->implementation_.find (key);
}

template <class KEY, class VALUE, class KEY_GENERATOR> ACE_INLINE int
ACE_Map_Manager_Adapter<KEY, VALUE, KEY_GENERATOR>::unbind (const KEY &key)
{
  return this->implementation_.unbind (key);
}

template <class KEY, class VALUE, class KEY_GENERATOR> ACE_INLINE int
ACE_Map_Manager_Adapter<KEY, VALUE, KEY_GENERATOR>::unbind (const KEY &key,
                                                            VALUE &value)
{
  return this->implementation_.unbind (key,
                                       value);
}

template <class KEY, class VALUE, class KEY_GENERATOR> ACE_INLINE size_t
ACE_Map_Manager_Adapter<KEY, VALUE, KEY_GENERATOR>::current_size (void)
{
  return this->implementation_.current_size ();
}

template <class KEY, class VALUE, class KEY_GENERATOR> ACE_INLINE size_t
ACE_Map_Manager_Adapter<KEY, VALUE, KEY_GENERATOR>::total_size (void)
{
  return this->implementation_.total_size ();
}

template <class KEY, class VALUE, class KEY_GENERATOR> ACE_INLINE void
ACE_Map_Manager_Adapter<KEY, VALUE, KEY_GENERATOR>::dump (void) const
{
  this->implementation_.dump ();
}

template <class KEY, class VALUE, class KEY_GENERATOR> ACE_INLINE ACE_Iterator_Impl<ACE_Reference_Pair<const KEY, VALUE> > *
ACE_Map_Manager_Adapter<KEY, VALUE, KEY_GENERATOR>::begin_impl (void)
{
  return new iterator_impl (this->implementation_.begin ());
}

template <class KEY, class VALUE, class KEY_GENERATOR> ACE_INLINE ACE_Iterator_Impl<ACE_Reference_Pair<const KEY, VALUE> > *
ACE_Map_Manager_Adapter<KEY, VALUE, KEY_GENERATOR>::end_impl (void)
{
  return new iterator_impl (this->implementation_.end ());
}

template <class KEY, class VALUE, class KEY_GENERATOR> ACE_INLINE ACE_Reverse_Iterator_Impl<ACE_Reference_Pair<const KEY, VALUE> > *
ACE_Map_Manager_Adapter<KEY, VALUE, KEY_GENERATOR>::rbegin_impl (void)
{
  return new reverse_iterator_impl (this->implementation_.rbegin ());
}

template <class KEY, class VALUE, class KEY_GENERATOR> ACE_INLINE ACE_Reverse_Iterator_Impl<ACE_Reference_Pair<const KEY, VALUE> > *
ACE_Map_Manager_Adapter<KEY, VALUE, KEY_GENERATOR>::rend_impl (void)
{
  return new reverse_iterator_impl (this->implementation_.rend ());
}

template <class KEY, class VALUE, class KEY_GENERATOR> ACE_INLINE ACE_Map_Manager<KEY, VALUE, ACE_Null_Mutex> &
ACE_Map_Manager_Adapter<KEY, VALUE, KEY_GENERATOR>::impl (void)
{
  return this->implementation_;
}

template <class KEY, class VALUE, class KEY_GENERATOR> ACE_INLINE KEY_GENERATOR &
ACE_Map_Manager_Adapter<KEY, VALUE, KEY_GENERATOR>::key_generator (void)
{
  return this->key_generator_;
}

////////////////////////////////////////////////////////////////////////////////
