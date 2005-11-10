// -*- C++ -*-
//
// $Id$

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

template<class T> ACE_INLINE
TAO_EC_Busy_Lock_Adapter<T>::TAO_EC_Busy_Lock_Adapter (T* adaptee)
  :  adaptee_ (adaptee)
{
}

TAO_END_VERSIONED_NAMESPACE_DECL
