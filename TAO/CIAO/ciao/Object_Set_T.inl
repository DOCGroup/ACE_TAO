/* -*- C++ -*- */
// $Id$

template <class COBJ> ACE_INLINE CORBA::ULong
CIAO::Object_Set<COBJ>::size (void) const
{
  return this->size_;
}

template <class COBJ> ACE_INLINE CORBA::ULong
CIAO::Object_Set<COBJ>::capacity (void) const
{
  return this->capacity_;
}
