// $Id$

#ifndef TAO_ESF_IMMEDIATE_CHANGES_CPP
#define TAO_ESF_IMMEDIATE_CHANGES_CPP

#include "ESF_Immediate_Changes.h"

#if ! defined (__ACE_INLINE__)
#include "ESF_Immediate_Changes.i"
#endif /* __ACE_INLINE__ */

#include "ESF_Worker.h"

ACE_RCSID(ESF, ESF_Immediate_Changes, "$Id$")

// ****************************************************************

template<class PROXY, class COLLECTION, class ITERATOR, class ACE_LOCK>
TAO_ESF_Immediate_Changes<PROXY,COLLECTION,ITERATOR,ACE_LOCK>::
    TAO_ESF_Immediate_Changes (void)
{
}

template<class PROXY, class COLLECTION, class ITERATOR, class ACE_LOCK>
TAO_ESF_Immediate_Changes<PROXY,COLLECTION,ITERATOR,ACE_LOCK>::
    TAO_ESF_Immediate_Changes (const COLLECTION &collection)
      :  collection_ (collection)
{
}

template<class PROXY, class COLLECTION, class ITERATOR, class ACE_LOCK> void
TAO_ESF_Immediate_Changes<PROXY,COLLECTION,ITERATOR,ACE_LOCK>::
    for_each (TAO_ESF_Worker<PROXY> *worker,
              CORBA::Environment &ACE_TRY_ENV)
{
  ACE_GUARD (ACE_LOCK, ace_mon, this->lock_);

  ITERATOR end = this->collection_.end ();
  for (ITERATOR i = this->collection_.begin (); i != end; ++i)
    {
      worker->work ((*i), ACE_TRY_ENV);
      ACE_CHECK;
    }
}

#endif /* TAO_ESF_IMMEDIATE_CHANGES_CPP */
