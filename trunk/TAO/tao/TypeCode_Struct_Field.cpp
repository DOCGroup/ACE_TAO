// $Id$

#ifndef TAO_TYPECODE_STRUCT_FIELD_CPP
#define TAO_TYPECODE_STRUCT_FIELD_CPP

#include "TypeCode_Struct_Field.h"

#ifndef __ACE_INLINE__
# include "tao/TypeCode_Struct_Field.inl"
#endif /* __ACE_INLINE__ */


template <typename STRING_TYPE>
TAO::TypeCode::Struct_Field<STRING_TYPE>::~Struct_Field (void)
{
  if (this->type)
    CORBA::release (*type);
}

#endif  /* TAO_TYPECODE_STRUCT_FIELD_CPP */
