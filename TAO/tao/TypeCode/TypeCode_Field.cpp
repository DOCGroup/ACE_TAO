// $Id$

#ifndef TAO_TYPECODE_FIELD_CPP
#define TAO_TYPECODE_FIELD_CPP

#include "TypeCode_Field.h"

#ifndef __ACE_INLINE__
# include "tao/TypeCode_Field.inl"
#endif /* __ACE_INLINE__ */


template <typename STRING_TYPE>
ACE_INLINE char const *
TAO::TypeCode::Field<STRING_TYPE>::~Field (void)
{
  if (this->type)
    CORBA::release (*type);
}

#endif  /* TAO_TYPECODE_FIELD_CPP */
