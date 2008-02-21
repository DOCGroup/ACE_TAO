// $Id$

#ifndef TAO_ESF_IMMEDIATE_CHANGES_CPP
#define TAO_ESF_IMMEDIATE_CHANGES_CPP

#include "orbsvcs/ESF/ESF_Immediate_Changes.h"

#if ! defined (__ACE_INLINE__)
#include "orbsvcs/ESF/ESF_Immediate_Changes.inl"
#endif /* __ACE_INLINE__ */

#include "orbsvcs/ESF/ESF_Worker.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

// ****************************************************************

template<class PROXY, class C, class ITERATOR, class ACE_LOCK>
TAO_ESF_Immediate_Changes<PROXY,C,ITERATOR,ACE_LOCK>::
    TAO_ESF_Immediate_Changes (void)
{
}

template<class PROXY, class C, class ITERATOR, class ACE_LOCK>
TAO_ESF_Immediate_Changes<PROXY,C,ITERATOR,ACE_LOCK>::
    TAO_ESF_Immediate_Changes (const C &collection)
      :  collection_ (collection)
{
}

template<class PROXY, class C, class ITERATOR, class ACE_LOCK> void
TAO_ESF_Immediate_Changes<PROXY,C,ITERATOR,ACE_LOCK>::
    for_each (TAO_ESF_Worker<PROXY> *worker)
{
  ACE_GUARD (ACE_LOCK, ace_mon, this->lock_);

  worker->set_size(this->collection_.size());
  ITERATOR end = this->collection_.end ();
  for (ITERATOR i = this->collection_.begin (); i != end; ++i)
    {
      worker->work ((*i));
    }
}

TAO_END_VERSIONED_NAMESPACE_DECL

#endif /* TAO_ESF_IMMEDIATE_CHANGES_CPP */
