// $Id$


template<class PROXY> ACE_INLINE
TAO_ESF_Proxy_RB_Tree_Iterator<PROXY>::
    TAO_ESF_Proxy_RB_Tree_Iterator (const Implementation &i)
      :  impl_ (i)
{
}

template<class PROXY> ACE_INLINE int
TAO_ESF_Proxy_RB_Tree_Iterator<PROXY>::
    operator == (const TAO_ESF_Proxy_RB_Tree_Iterator<PROXY> &rhs) const
{
  return this->impl_ == rhs.impl_;
}

template<class PROXY> ACE_INLINE int
TAO_ESF_Proxy_RB_Tree_Iterator<PROXY>::
    operator != (const TAO_ESF_Proxy_RB_Tree_Iterator<PROXY> &rhs) const
{
  return this->impl_ != rhs.impl_;
}

template<class PROXY> ACE_INLINE TAO_ESF_Proxy_RB_Tree_Iterator<PROXY>&
TAO_ESF_Proxy_RB_Tree_Iterator<PROXY>::operator++ (void)
{
  ++this->impl_;
  return *this;
}

template<class PROXY> ACE_INLINE TAO_ESF_Proxy_RB_Tree_Iterator<PROXY>
TAO_ESF_Proxy_RB_Tree_Iterator<PROXY>::operator++ (int)
{
  TAO_ESF_Proxy_RB_Tree_Iterator<PROXY> tmp = *this;
  ++this->impl_;
  return tmp;
}

template<class PROXY> ACE_INLINE PROXY*
TAO_ESF_Proxy_RB_Tree_Iterator<PROXY>::operator *(void)
{
  return (*this->impl_).key ();
}

// ****************************************************************

template<class PROXY> ACE_INLINE TAO_ESF_Proxy_RB_Tree_Iterator<PROXY>
TAO_ESF_Proxy_RB_Tree<PROXY>::begin (void)
{
  return Iterator (this->impl_.begin ());
}

template<class PROXY> ACE_INLINE TAO_ESF_Proxy_RB_Tree_Iterator<PROXY>
TAO_ESF_Proxy_RB_Tree<PROXY>::end (void)
{
  return Iterator (this->impl_.end ());
}

template<class PROXY> ACE_INLINE size_t
TAO_ESF_Proxy_RB_Tree<PROXY>::size (void) const
{
  return this->impl_.current_size ();
}
