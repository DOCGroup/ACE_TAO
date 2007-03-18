// $Id$

#ifndef TAO_Notify_SEQ_WORKER_T_CPP
#define TAO_Notify_SEQ_WORKER_T_CPP

#include "orbsvcs/Notify/Seq_Worker_T.h"

#if ! defined (__ACE_INLINE__)
#include "orbsvcs/Notify/Seq_Worker_T.inl"
#endif /* __ACE_INLINE__ */

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

template <class T>
TAO_Notify_Seq_Worker_T<T>::TAO_Notify_Seq_Worker_T (void)
{
}

template<class TYPE> typename TAO_Notify_Seq_Worker_T<TYPE>::SEQ*
TAO_Notify_Seq_Worker_T<TYPE>::create (CONTAINER &container)
{
  SEQ* tmp;
  ACE_NEW_THROW_EX (tmp, //this->seq_,
                    SEQ (),
                    CORBA::INTERNAL ());

  this->seq_ = tmp;

  container.collection ()->for_each (this);

  return this->seq_._retn ();

}

TAO_END_VERSIONED_NAMESPACE_DECL

#endif /* TAO_Notify_SEQ_WORKER_T_CPP */
