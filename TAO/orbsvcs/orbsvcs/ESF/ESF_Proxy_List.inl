// -*- C++ -*-
TAO_BEGIN_VERSIONED_NAMESPACE_DECL

template<class PROXY> ACE_INLINE typename TAO_ESF_Proxy_List<PROXY>::Iterator
TAO_ESF_Proxy_List<PROXY>::begin ()
{
  return this->impl_.begin ();
}

template<class PROXY> ACE_INLINE typename TAO_ESF_Proxy_List<PROXY>::Iterator
TAO_ESF_Proxy_List<PROXY>::end ()
{
  return this->impl_.end ();
}

template<class PROXY> ACE_INLINE std::size_t
TAO_ESF_Proxy_List<PROXY>::size () const
{
  return this->impl_.size ();
}

TAO_END_VERSIONED_NAMESPACE_DECL
