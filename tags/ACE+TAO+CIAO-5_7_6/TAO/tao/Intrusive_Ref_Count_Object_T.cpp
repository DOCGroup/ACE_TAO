// $Id$

#ifndef TAO_INTRUSIVE_REF_COUNT_OBJECT_T_CPP
#define TAO_INTRUSIVE_REF_COUNT_OBJECT_T_CPP

#include "tao/Intrusive_Ref_Count_Object_T.h"

#if !defined (__ACE_INLINE__)
#include "tao/Intrusive_Ref_Count_Object_T.inl"
#endif /* __ACE_INLINE__ */

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

template <typename OBJ, typename ACE_Lock>
TAO_Intrusive_Ref_Count_Object<OBJ,ACE_Lock>::~TAO_Intrusive_Ref_Count_Object()
{
  delete this->obj_;
}

TAO_END_VERSIONED_NAMESPACE_DECL

#endif /* TAO_INTRUSIVE_REF_COUNT_OBJECT_T_CPP */
