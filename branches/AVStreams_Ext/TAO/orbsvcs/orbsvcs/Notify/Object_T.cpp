// $Id$

#ifndef TAO_NS_OBJECT_T_CPP
#define TAO_NS_OBJECT_T_CPP

#include "Object_T.h"
#include "orbsvcs/ESF/ESF_RefCount_Guard.h"

#if ! defined (__ACE_INLINE__)
#include "Object_T.inl"
#endif /* __ACE_INLINE__ */

ACE_RCSID(RT_Notify, TAO_NS_Object, "$Id$")

template <class TYPE, class PARENT>
TAO_NS_Object_T<TYPE, PARENT>::TAO_NS_Object_T (void)
  :parent_ (0)
{
}

template <class TYPE, class PARENT>
TAO_NS_Object_T<TYPE, PARENT>::~TAO_NS_Object_T ()
{
  if (parent_)
    this->parent_-> _decr_refcnt ();
}

template <class TYPE, class PARENT> int
TAO_NS_Object_T<TYPE, PARENT>::destroy (TYPE *type ACE_ENV_ARG_DECL)
{
  if (this->shutdown (ACE_ENV_SINGLE_ARG_PARAMETER) == 1)
    return 1;

  ACE_CHECK_RETURN (1);

  if (parent_)
    this->parent_->remove (type ACE_ENV_ARG_PARAMETER);

  return 0;
}

#endif /* TAO_NS_OBJECT_T_CPP */
