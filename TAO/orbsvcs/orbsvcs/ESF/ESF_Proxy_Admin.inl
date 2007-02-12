// -*- C++ -*-
//
// $Id$

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

template<class EC,class P,class I> ACE_INLINE void
TAO_ESF_Proxy_Admin<EC,P,I>::
    for_each (TAO_ESF_Worker<P> *worker)
{
  this->collection_->for_each (worker);
}

template <class C>
ACE_INLINE
TAO_ESF_RefCountedRef<C>::TAO_ESF_RefCountedRef (C *counted)
  : counted_ (counted)
{}

template <class C>
ACE_INLINE
TAO_ESF_RefCountedRef<C>::~TAO_ESF_RefCountedRef ()
{
  if (this->counted_ != 0)
    {
      this->counted_->_decr_refcnt ();
    }
}

TAO_END_VERSIONED_NAMESPACE_DECL
