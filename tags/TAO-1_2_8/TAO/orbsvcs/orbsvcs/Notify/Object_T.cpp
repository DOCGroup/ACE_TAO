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
}

template <class TYPE, class PARENT> void
TAO_NS_Object_T<TYPE, PARENT>::destroy (TYPE *type ACE_ENV_ARG_DECL)
{
  TAO_NS_Object_RefCount_Guard guard (this->refcount_);

  this->shutdown (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK;

  if (parent_)
    parent_->remove (type ACE_ENV_ARG_PARAMETER);
}

#endif /* TAO_NS_OBJECT_T_CPP */
