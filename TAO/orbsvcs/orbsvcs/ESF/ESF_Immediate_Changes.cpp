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

template<class PROXY, class C, class I, class L>
TAO_ESF_Immediate_Changes<PROXY,C,I,L>::
    TAO_ESF_Immediate_Changes (void)
{
}

template<class PROXY, class C, class I, class L>
TAO_ESF_Immediate_Changes<PROXY,C,I,L>::
    TAO_ESF_Immediate_Changes (const C &collection)
      :  collection_ (collection)
{
}

template<class PROXY, class C, class I, class L> void
TAO_ESF_Immediate_Changes<PROXY,C,I,L>::
    for_each (TAO_ESF_Worker<PROXY> *worker,
              CORBA::Environment &ACE_TRY_ENV)
{
  ACE_GUARD (L, ace_mon, this->lock_);

  I end = this->collection_.end ();
  for (I i = this->collection_.begin (); i != end; ++i)
    {
      worker->work ((*i), ACE_TRY_ENV);
      ACE_CHECK;
    }
}

#endif /* TAO_ESF_IMMEDIATE_CHANGES_CPP */
