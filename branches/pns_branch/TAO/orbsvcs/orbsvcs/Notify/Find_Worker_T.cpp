// $Id$

#ifndef TAO_Notify_FIND_WORKER_T_CPP
#define TAO_Notify_FIND_WORKER_T_CPP

#include "Find_Worker_T.h"

#if ! defined (__ACE_INLINE__)
#include "Find_Worker_T.inl"
#endif /* __ACE_INLINE__ */

ACE_RCSID(Notify, TAO_Notify_Find_Worker_T, "$Id$")

template<class TYPE, class INTERFACE, class INTERFACE_PTR, class EXCEPTION>
TAO_Notify_Find_Worker_T<TYPE,INTERFACE,INTERFACE_PTR,EXCEPTION>::TAO_Notify_Find_Worker_T (void)
  :id_ (0), result_ (0)
{
}

template<class TYPE, class INTERFACE, class INTERFACE_PTR, class EXCEPTION> TYPE*
TAO_Notify_Find_Worker_T<TYPE,INTERFACE,INTERFACE_PTR,EXCEPTION>::find (TAO_Notify_Object::ID id, CONTAINER& container ACE_ENV_ARG_DECL)
{
  this->id_ = id;

  container.collection ()->for_each (this ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (0);

  return this->result_;
}

template<class TYPE, class INTERFACE, class INTERFACE_PTR, class EXCEPTION> INTERFACE_PTR
TAO_Notify_Find_Worker_T<TYPE,INTERFACE,INTERFACE_PTR,EXCEPTION>::resolve (TAO_Notify_Object::ID id, CONTAINER& container ACE_ENV_ARG_DECL)
{
  this->find (id, container ACE_ENV_ARG_PARAMETER);

  if (this->result_ == 0)
    ACE_THROW_RETURN (EXCEPTION ()
                      , INTERFACE::_nil ());

  CORBA::Object_var object = this->result_->ref (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK_RETURN (INTERFACE::_nil ());

  return INTERFACE::_narrow (object.in () ACE_ENV_ARG_PARAMETER);

}

#endif /* TAO_Notify_FIND_WORKER_T_CPP */
