// $Id$

#ifndef TAO_TYPECODE_VALUE_FIELD_CPP
#define TAO_TYPECODE_VALUE_FIELD_CPP

#include "TypeCode_Value_Field.h"

#ifndef __ACE_INLINE__
# include "tao/TypeCode_Value_Field.inl"
#endif /* __ACE_INLINE__ */


template <typename STRING_TYPE>
char const *
TAO::TypeCode::Value_Field<STRING_TYPE>::~Value_Field (void)
{
  if (this->type)
    CORBA::release (*type);
}

#endif  /* TAO_TYPECODE_VALUE_FIELD_CPP */
