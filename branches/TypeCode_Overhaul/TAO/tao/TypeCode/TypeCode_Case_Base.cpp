// $Id$

#ifndef TAO_TYPECODE_CASE_BASE_CPP
#define TAO_TYPECODE_CASE_BASE_CPP

#include "TypeCode_Case_Base.h"

#ifndef __ACE_INLINE__
# include "tao/TypeCode_Case_Base.inl"
#endif /* __ACE_INLINE__ */


template <typename DISCRIMINATOR, typename STRING_TYPE>
char const *
TAO::TypeCode::Case_Base<DISCRIMINATOR, STRING_TYPE>::~Case_Base (void)
{
  if (this->type_)
    CORBA::release (*(this->type_));
}

#endif  /* TAO_TYPECODE_CASE_BASE_CPP */
