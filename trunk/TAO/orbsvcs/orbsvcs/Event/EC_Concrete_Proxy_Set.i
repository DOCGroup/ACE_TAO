// $Id$

template<class PROXY> ACE_INLINE TAO_EC_List_Based_Proxy_Set<PROXY>::Iterator
TAO_EC_List_Based_Proxy_Set<PROXY>::begin (void)
{
  return this->impl_.begin ();
}

template<class PROXY> ACE_INLINE TAO_EC_List_Based_Proxy_Set<PROXY>::Iterator
TAO_EC_List_Based_Proxy_Set<PROXY>::end (void)
{
  return this->impl_.end ();
}

template<class PROXY> ACE_INLINE size_t
TAO_EC_List_Based_Proxy_Set<PROXY>::size (void) const
{
  return this->impl_.size ();
}

// ****************************************************************

template<class PROXY> ACE_INLINE
TAO_EC_RB_Tree_Based_Proxy_Set_Iterator<PROXY>::
    TAO_EC_RB_Tree_Based_Proxy_Set_Iterator (const Implementation &i)
      :  impl_ (i)
{
}

template<class PROXY> ACE_INLINE int
TAO_EC_RB_Tree_Based_Proxy_Set_Iterator<PROXY>::
    operator == (const TAO_EC_RB_Tree_Based_Proxy_Set_Iterator<PROXY> &rhs) const
{
  return this->impl_ == rhs.impl_;
}

template<class PROXY> ACE_INLINE int
TAO_EC_RB_Tree_Based_Proxy_Set_Iterator<PROXY>::
    operator != (const TAO_EC_RB_Tree_Based_Proxy_Set_Iterator<PROXY> &rhs) const
{
  return this->impl_ != rhs.impl_;
}

template<class PROXY> ACE_INLINE TAO_EC_RB_Tree_Based_Proxy_Set_Iterator<PROXY>&
TAO_EC_RB_Tree_Based_Proxy_Set_Iterator<PROXY>::operator++ (void)
{
  ++this->impl_;
  return *this;
}

template<class PROXY> ACE_INLINE TAO_EC_RB_Tree_Based_Proxy_Set_Iterator<PROXY>
TAO_EC_RB_Tree_Based_Proxy_Set_Iterator<PROXY>::operator++ (int)
{
  TAO_EC_RB_Tree_Based_Proxy_Set_Iterator<PROXY> tmp = *this;
  ++this->impl_;
  return tmp;
}

template<class PROXY> ACE_INLINE PROXY*
TAO_EC_RB_Tree_Based_Proxy_Set_Iterator<PROXY>::operator *(void)
{
  return (*this->impl_).key ();
}

// ****************************************************************

template<class PROXY> ACE_INLINE TAO_EC_RB_Tree_Based_Proxy_Set<PROXY>::Iterator
TAO_EC_RB_Tree_Based_Proxy_Set<PROXY>::begin (void)
{
  return Iterator (this->impl_.begin ());
}

template<class PROXY> ACE_INLINE TAO_EC_RB_Tree_Based_Proxy_Set<PROXY>::Iterator
TAO_EC_RB_Tree_Based_Proxy_Set<PROXY>::end (void)
{
  return Iterator (this->impl_.end ());
}

template<class PROXY> ACE_INLINE size_t
TAO_EC_RB_Tree_Based_Proxy_Set<PROXY>::size (void) const
{
  return this->impl_.current_size ();
}
