// $Id$

template<class T> ACE_INLINE int
ACE_Intrusive_List<T>::empty (void) const
{
  return this->head_ == 0;
}
