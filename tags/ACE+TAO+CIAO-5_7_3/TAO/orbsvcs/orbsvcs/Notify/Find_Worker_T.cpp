// $Id$

#ifndef TAO_Notify_FIND_WORKER_T_CPP
#define TAO_Notify_FIND_WORKER_T_CPP

#include "orbsvcs/Notify/Find_Worker_T.h"

#if ! defined (__ACE_INLINE__)
#include "orbsvcs/Notify/Find_Worker_T.inl"
#endif /* __ACE_INLINE__ */

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

template<class TYPE, class INTERFACE, class INTERFACE_PTR, class EXCEPTION>
TAO_Notify_Find_Worker_T<TYPE,INTERFACE,INTERFACE_PTR,EXCEPTION>::TAO_Notify_Find_Worker_T (void)
  :id_ (0), result_ (0)
{
}

template<class TYPE, class INTERFACE, class INTERFACE_PTR, class EXCEPTION> TYPE*
TAO_Notify_Find_Worker_T<TYPE,INTERFACE,INTERFACE_PTR,EXCEPTION>::find (const TAO_Notify_Object::ID id, CONTAINER& container)
{
  this->id_ = id;

  container.collection ()->for_each (this);

  return this->result_;
}

template<class TYPE, class INTERFACE, class INTERFACE_PTR, class EXCEPTION> INTERFACE_PTR
TAO_Notify_Find_Worker_T<TYPE,INTERFACE,INTERFACE_PTR,EXCEPTION>::resolve (const TAO_Notify_Object::ID id, CONTAINER& container)
{
  this->find (id, container);

  if (this->result_ == 0)
    throw EXCEPTION ();

  CORBA::Object_var object = this->result_->ref ();

  return INTERFACE::_narrow (object.in ());
}

TAO_END_VERSIONED_NAMESPACE_DECL

#endif /* TAO_Notify_FIND_WORKER_T_CPP */
