/* -*- C++ -*- */
// $Id$

template <class T, class T_var> ACE_INLINE CORBA::ULong
CIAO::Object_Set<T, T_var>::size (void) const
{
  return this->size_;
}

template <class T, class T_var> ACE_INLINE CORBA::ULong
CIAO::Object_Set<T, T_var>::capacity (void) const
{
  return this->capacity_;
}

template <class T, class T_var> ACE_INLINE T_var &
CIAO::Object_Set<T, T_var>::at (CORBA::ULong index)
{
  return this->buffer_[index];
}
