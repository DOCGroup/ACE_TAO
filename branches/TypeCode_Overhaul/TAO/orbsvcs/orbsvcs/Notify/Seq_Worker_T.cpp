// $Id$

#ifndef TAO_Notify_SEQ_WORKER_T_CPP
#define TAO_Notify_SEQ_WORKER_T_CPP

#include "Seq_Worker_T.h"

#if ! defined (__ACE_INLINE__)
#include "Seq_Worker_T.inl"
#endif /* __ACE_INLINE__ */

ACE_RCSID(Notify, TAO_Notify_Seq_Worker_T, "$Id$")

template <class T>
TAO_Notify_Seq_Worker_T<T>::TAO_Notify_Seq_Worker_T (void)
{
}

template<class TYPE> ACE_TYPENAME TAO_Notify_Seq_Worker_T<TYPE>::SEQ*
TAO_Notify_Seq_Worker_T<TYPE>::create (CONTAINER &container ACE_ENV_ARG_DECL)
{
  SEQ* tmp;
  ACE_NEW_THROW_EX (tmp, //this->seq_,
                    SEQ (),
                    CORBA::INTERNAL ());

  this->seq_ = tmp;

  container.collection ()->for_each (this ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (0);

  return this->seq_._retn ();

}

#endif /* TAO_Notify_SEQ_WORKER_T_CPP */
