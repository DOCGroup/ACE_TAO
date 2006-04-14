// $Id$

template<class T> ACE_INLINE
TAO_ESF_RefCount_Guard<T>::
    TAO_ESF_RefCount_Guard (T &refcount)
  :  refcount_ (refcount)
{
  this->refcount_++;
}

template<class T> ACE_INLINE
TAO_ESF_RefCount_Guard<T>::
    ~TAO_ESF_RefCount_Guard (void)
{
  this->refcount_--;
}
