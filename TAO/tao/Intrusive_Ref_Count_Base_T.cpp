// $Id$

#ifndef TAO_INTRUSIVE_REF_COUNT_BASE_T_C
#define TAO_INTRUSIVE_REF_COUNT_BASE_T_C

#include "Intrusive_Ref_Count_Base_T.h"

#if !defined (__ACE_INLINE__)
#include "tao/Intrusive_Ref_Count_Base_T.inl"
#endif /* __ACE_INLINE__ */

ACE_RCSID (tao,
           Intrusive_Ref_Count_Base_T,
           "$Id$")

template <typename T>
TAO_Intrusive_Ref_Count_Base<T>::~TAO_Intrusive_Ref_Count_Base()
{
}

#endif /* TAO_INTRUSIVE_REF_COUNT_BASE_T_C */
