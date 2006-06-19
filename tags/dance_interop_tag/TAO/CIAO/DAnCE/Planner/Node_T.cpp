
//Author: Stoyan Paunov
//$Id$

#include "Node_T.h"

template <class T>
Node<T>::Node (T* t)
: t_ (t),
  next_ (0),
  prev_ (0)
{
}

#if !defined (__ACE_INLINE__)
#include "Node_T.inl"
#endif /* __ACE_INLINE__ */

