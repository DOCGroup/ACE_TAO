// $Id$

template<class T> ACE_INLINE
TAO_EC_Busy_Lock_Adapter<T>::TAO_EC_Busy_Lock_Adapter (T* adaptee)
  :  adaptee_ (adaptee)
{
}

